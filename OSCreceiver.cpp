#include "OSCreceiver.h"

OSCreceiver::OSCreceiver():
_lsocket(0),
_port(23000),
_max_queue(100),
_autostart(true),
_emit_signal(true),
_native_mode(false),
_gd_queue_write(0),
_gd_queue_read(0)
{
	
	Ref<OSCmessage> rom = memnew(OSCmessage);
	_gd_next_msg = rom;
	
}

OSCreceiver::~OSCreceiver() {
	stop();
}

void OSCreceiver::stop() {
	
	if (_lsocket) {
		delete _lsocket;
		_lsocket = 0;
	}
	
	purge_buffers();
	
	_msg_queue.clear();
	
}

void OSCreceiver::native_mode( bool enable ) {
	
	if ( _native_mode != enable ) {
		if (enable ) {
			purge_buffers();
		} else if (!enable && !_msg_queue.empty() ) {
			_msg_queue.clear();
		}
		_native_mode = enable;
	}
	
}

void OSCreceiver::create_buffers() {
	
	if ( !_gd_queue_write ) {
		_gd_queue_write = new std::deque<OSCmessage>();
		_gd_queue_read = new std::deque<OSCmessage>();
	}
	
}

void OSCreceiver::purge_buffers() {
	
	if ( _gd_queue_write ) {
		_gd_queue_write->clear();
		_gd_queue_read->clear();
		delete _gd_queue_write;
		delete _gd_queue_read;
		_gd_queue_write = 0;
		_gd_queue_read = 0;
	}
	
}

void OSCreceiver::swap_buffers() {
	
	_lmutex.lock();
	std::deque<OSCmessage>* tmp = _gd_queue_write;
	_gd_queue_write = _gd_queue_read;
	_gd_queue_read = tmp;
	_lmutex.unlock();
	
}

bool OSCreceiver::init(int port) {

	if ( port < 1 ) return false;
	stop();
	_port = port;
	return true;
	
}

bool OSCreceiver::start() {
	
	if ( _port < 1 ) return false;
	
	create_buffers();
	
	try {
		IpEndpointName name(IpEndpointName::ANY_ADDRESS, _port);
		_lsocket = new UdpListeningReceiveSocket(name, this);
		std::cout << "OSCreceiver::start on port " << _port << std::endl;
	} catch (const std::exception& e) {
		std::cout << "OSCreceiver::start: " << e.what() << std::endl;
		return false;
	}
	
	_lthread = std::thread([this]{
		while(_lsocket) {			
			try {
				_lsocket->Run();
			} catch (std::exception &e) {
				std::cout << " cannot listen " << e.what() << std::endl;
			}
		}
		std::cout << "end thread" << std::endl;
	});
	
	_lthread.detach();
	
	return true;
	
}

void OSCreceiver::ProcessMessage(const osc::ReceivedMessage &m, const IpEndpointName &remoteEndpoint){
	
	if (!_native_mode) {
		
		OSCmessage gdm( m, remoteEndpoint );

		if ( gdm.is_valid() ) {
			_lmutex.lock();
			_gd_queue_write->push_back( gdm );
			check_queue();
			_lmutex.unlock();
		}
		
		return;
		
	}
	
	gdOscMessage msg ;
	msg.setAddress(m.AddressPattern());
	msg.setTypetag(m.TypeTags());
	char endpointHost[IpEndpointName::ADDRESS_STRING_LENGTH];
	remoteEndpoint.AddressAsString(endpointHost);
	msg.setRemoteEndpoint(endpointHost, remoteEndpoint.port);
	
	try{
		
		for (::osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin(); arg != m.ArgumentsEnd(); ++arg){
			if(arg->IsInt32()) {
				msg.addIntArg(arg->AsInt32Unchecked());
			}
			else if (arg->IsFloat()){
				msg.addFloatArg(arg->AsFloatUnchecked());
			}
			else if (arg->IsString()) {
				msg.addStringArg(arg->AsStringUnchecked());
			}
		}
			
		_msg_queue.push_back( msg );
		
		check_queue();
		
	} catch( osc::Exception& e ){
		
		// any parsing errors such as unexpected argument types, or
		// missing arguments get thrown as exceptions.
		std::cout << "error while parsing message: "
		<< m.AddressPattern() << ": " << e.what() << "\n";
		
	}
	
}

bool OSCreceiver::has_waiting_messages(){
	
	if ( !_native_mode ) { 
		if ( _gd_queue_read && !_gd_queue_read->empty() ) return true;
		return false;
	}
	return !_msg_queue.empty();
	
}

Ref<OSCmessage> OSCreceiver::get_next_message() {
	
	
	if ( _native_mode || !_gd_queue_read || _gd_queue_read->empty()) {
		return Ref<OSCmessage>( nullptr );
	}
	
	_gd_next_msg->copy( _gd_queue_read->front() );
	_gd_queue_read->pop_front();
	
	return _gd_next_msg;
	
}

bool OSCreceiver::getNextMessage( gdOscMessage& msg ) {
	
	if ( !_native_mode || _msg_queue.empty() ) return false;
	
	const gdOscMessage& m = _msg_queue.front();
	msg.copy( m );
	_msg_queue.pop_front();
	
	return true;
	
}

void OSCreceiver::set_autostart( bool autostart ) {
	_autostart = autostart;
}

void OSCreceiver::set_emit_signal( bool emit_signal ) {
	_emit_signal = emit_signal;
}

void OSCreceiver::check_queue() {
	
	if ( 
		!_native_mode && 
		_gd_queue_write && 
		_gd_queue_write->size() > _max_queue 
	) {
		
		_gd_queue_write->resize( _max_queue );
		
	} else if ( 
		_native_mode && 
		_msg_queue.size() > _max_queue
	) {
		
		_msg_queue.resize( _max_queue );
		
	}
	
}

void OSCreceiver::set_max_queue( int max_queue ) {
	
	if ( max_queue < 1 ) return;
	_max_queue = (std::size_t) max_queue;
	check_queue();
	
}

void OSCreceiver::_notification(int p_what) {
	
	switch (p_what) {
		case MainLoop::NOTIFICATION_WM_QUIT_REQUEST: {
			std::cout << "OSCreceiver::_notification, stopping reception" << std::endl;
			stop();
		} break;
		case NOTIFICATION_READY: {
			if(_autostart && !Engine::get_singleton()->is_editor_hint()) {
				std::cout << "OSCreceiver::_notification, starting reception" << std::endl;
				start();
			}
		} break;
		case NOTIFICATION_PROCESS: {
			if(_gd_queue_read) {
// 				std::cout << "OSCreceiver::_notification, NOTIFICATION_PROCESS" << std::endl;
				swap_buffers();
				if( _emit_signal ) {
					while( has_waiting_messages() ) {
						std::cout << "osc_message_received" << std::endl;
						emit_signal( "osc_message_received", get_next_message() );
					}
				}
			}
		} break;
		
	}
	//std::cout << p_what << std::endl;
	
}

void OSCreceiver::_bind_methods() {
	
	ClassDB::bind_method(D_METHOD("init", "port"), &OSCreceiver::init);
	ClassDB::bind_method(D_METHOD("start"), &OSCreceiver::start);
	ClassDB::bind_method(D_METHOD("stop"), &OSCreceiver::stop);
	ClassDB::bind_method(D_METHOD("set_max_queue", "max_queue"), &OSCreceiver::set_max_queue);
	ClassDB::bind_method(D_METHOD("set_autostart", "autostart"), &OSCreceiver::set_autostart);
	ClassDB::bind_method(D_METHOD("set_emit_signal", "emit_signal"), &OSCreceiver::set_emit_signal);
	ClassDB::bind_method(D_METHOD("get_port"), &OSCreceiver::get_port);
	ClassDB::bind_method(D_METHOD("get_max_queue"), &OSCreceiver::get_max_queue);
	ClassDB::bind_method(D_METHOD("get_autostart"), &OSCreceiver::get_autostart);
	ClassDB::bind_method(D_METHOD("get_emit_signal"), &OSCreceiver::get_emit_signal);
	ClassDB::bind_method(D_METHOD("has_waiting_messages"), &OSCreceiver::has_waiting_messages);
	ClassDB::bind_method(D_METHOD("get_next_message"), &OSCreceiver::get_next_message);
	
	ADD_SIGNAL(MethodInfo("osc_message_received"));

	ADD_GROUP("Network", "");
	ADD_PROPERTYNO(PropertyInfo(Variant::INT, "port", PROPERTY_HINT_RANGE, "1,99999,1"), "init", "get_port");
	ADD_PROPERTYNZ(PropertyInfo(Variant::INT, "max_queue", PROPERTY_HINT_RANGE, "1,1024,1"), "set_max_queue", "get_max_queue");
	ADD_PROPERTYNZ(PropertyInfo(Variant::BOOL, "autostart"), "set_autostart", "get_autostart");
	ADD_PROPERTYNZ(PropertyInfo(Variant::BOOL, "emit_signal"), "set_emit_signal", "get_emit_signal");
	
}