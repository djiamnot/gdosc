#include "OSCreceiver.h"

OSCreceiver::OSCreceiver():
_lsocket(0),
_port(23000),
_max_queue(100),
_autostart(true)
{
}

OSCreceiver::~OSCreceiver() {
	stop();
}

void OSCreceiver::stop() {
	if (_lsocket) {
		delete _lsocket;
		_lsocket = 0;
		_lthread.join();
	}
}

bool OSCreceiver::init(int port) {

	if ( port < 1 ) return false;
	stop();
	_port = port;
	return true;
	
}

bool OSCreceiver::start() {
	
	if ( _port < 1 ) return false;
	
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
	
	gdOscMessage* msg = new gdOscMessage();
	msg->setAddress(m.AddressPattern());
	char endpointHost[IpEndpointName::ADDRESS_STRING_LENGTH];
	remoteEndpoint.AddressAsString(endpointHost);
	msg->setRemoteEndpoint(endpointHost, remoteEndpoint.port);
	try{
		for (::osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin(); arg != m.ArgumentsEnd(); ++arg){
			if(arg->IsInt32()) {
				msg->addIntArg(arg->AsInt32Unchecked());
			}
			else if (arg->IsFloat()){
				msg->addFloatArg(arg->AsFloatUnchecked());
			}
			else if (arg->IsString()) {
				msg->addStringArg(arg->AsStringUnchecked());
			}
		}
		std::cout << "OSCreceiver::ProcessMessage " << m.AddressPattern() << " / " << _msg_queue.size() << std::endl;
		_msg_queue.push_back(msg);
		check_queue();
	} catch( osc::Exception& e ){
		// any parsing errors such as unexpected argument types, or
		// missing arguments get thrown as exceptions.
		std::cout << "error while parsing message: "
		<< m.AddressPattern() << ": " << e.what() << "\n";
	}
	
}

bool OSCreceiver::hasWaitingMessages(){
	
	return !_msg_queue.empty();
	
}

bool OSCreceiver::getNextMessage(gdOscMessage* message){
	
	if (_msg_queue.size() == 0) {
		return false;
	}
	
	gdOscMessage* src_message = _msg_queue.front();
	message->copy(*src_message);
	delete src_message;
	_msg_queue.pop_front();
	
	return true;
	
}

void OSCreceiver::set_autostart( bool autostart ) {
	_autostart = autostart;
}

void OSCreceiver::check_queue() {
	if ( _msg_queue.size() > _max_queue ) {
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
		
	}
	//std::cout << p_what << std::endl;
}

void OSCreceiver::_bind_methods() {
	ClassDB::bind_method(D_METHOD("init", "port"), &OSCreceiver::init);
	ClassDB::bind_method(D_METHOD("start"), &OSCreceiver::start);
	ClassDB::bind_method(D_METHOD("stop"), &OSCreceiver::stop);
	ClassDB::bind_method(D_METHOD("set_max_queue", "max_queue"), &OSCreceiver::set_max_queue);
	ClassDB::bind_method(D_METHOD("set_autostart", "autostart"), &OSCreceiver::set_autostart);
	ClassDB::bind_method(D_METHOD("get_port"), &OSCreceiver::get_port);
	ClassDB::bind_method(D_METHOD("get_max_queue"), &OSCreceiver::get_max_queue);
	ClassDB::bind_method(D_METHOD("get_autostart"), &OSCreceiver::get_autostart);
	//ADD_SIGNAL(MethodInfo("osc_message"));
	//ADD_SIGNAL(MethodInfo("osc_listener_ready"));
	ADD_GROUP("Network", "");
	ADD_PROPERTYNO(PropertyInfo(Variant::INT, "port", PROPERTY_HINT_RANGE, "1,99999,1"), "init", "get_port");
	ADD_PROPERTYNZ(PropertyInfo(Variant::INT, "max_queue", PROPERTY_HINT_RANGE, "1,1024,1"), "set_max_queue", "get_max_queue");
	ADD_PROPERTYNZ(PropertyInfo(Variant::BOOL, "autostart"), "set_autostart", "get_autostart");
}