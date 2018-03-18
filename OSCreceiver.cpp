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
		_dict_queue.clear();
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
				std::cout << "_lsocket->Run()" << std::endl;
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
	msg->setTypetag(m.TypeTags());
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
		
		emit_signal( "osc_message_received", add_gd_message( msg ) );
		
		check_queue();
		
	} catch( osc::Exception& e ){
		
		// any parsing errors such as unexpected argument types, or
		// missing arguments get thrown as exceptions.
		std::cout << "error while parsing message: "
		<< m.AddressPattern() << ": " << e.what() << "\n";
		
	}
	
}

const Dictionary& OSCreceiver::add_gd_message( gdOscMessage* msg ) {
	
	Dictionary d;

	d["valid"] = true;
	d["ip"] = String(msg->getRemoteIp().c_str());
	d["port"] = msg->getRemotePort();
	d["address"] = String(msg->getAddress().c_str());
	d["typetag"] = String(msg->getTypetag().c_str());
	Array a;
	
	for(int i =0; i < msg->getNumArgs(); i++){
		switch(msg->getArgType(i)) {
			case TYPE_INT32:
				a.append(msg->getArgAsInt32(i));
				break;
			case TYPE_FLOAT:
				a.append(msg->getArgAsFloat(i));
				break;
			case TYPE_STRING:
				a.append(msg->getArgAsString(i).c_str());
				break;
			default:
				a.append("UNKNOWN");
				break;
		}
	}
	
	d["data"] = a;
	
	_dict_queue.push_back(d);
	
	return _dict_queue[_dict_queue.size()-1];

}

bool OSCreceiver::has_waiting_messages(){
	
	//return !_msg_queue.empty();
	return !_dict_queue.empty();
	
}

Dictionary OSCreceiver::get_next_message() {
	
	Dictionary d;
	d["valid"] = false;
	
	if (_dict_queue.size() > 0) {
		d = _dict_queue.front();
		_dict_queue.pop_front();
	}
	
	return d;
	
}

void OSCreceiver::set_autostart( bool autostart ) {
	_autostart = autostart;
}

void OSCreceiver::check_queue() {
	if ( _dict_queue.size() > _max_queue ) {
		_dict_queue.resize( _max_queue );
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
	ClassDB::bind_method(D_METHOD("has_waiting_messages"), &OSCreceiver::has_waiting_messages);
	ClassDB::bind_method(D_METHOD("get_next_message"), &OSCreceiver::get_next_message);
	
	ADD_SIGNAL(MethodInfo("osc_message_received"));

	ADD_GROUP("Network", "");
	ADD_PROPERTYNO(PropertyInfo(Variant::INT, "port", PROPERTY_HINT_RANGE, "1,99999,1"), "init", "get_port");
	ADD_PROPERTYNZ(PropertyInfo(Variant::INT, "max_queue", PROPERTY_HINT_RANGE, "1,1024,1"), "set_max_queue", "get_max_queue");
	ADD_PROPERTYNZ(PropertyInfo(Variant::BOOL, "autostart"), "set_autostart", "get_autostart");
}