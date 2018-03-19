#include "core/resource.h"
#include "core/array.h"
#include "osc/OscReceivedElements.h"
#include "ip/IpEndpointName.h"
#include <iostream>

#ifndef GDOSC_MESSAGE_H
#define GDOSC_MESSAGE_H

class OSCmessage: public Resource {
	GDCLASS(OSCmessage, Resource);
	RES_BASE_EXTENSION("oscmessage");

public:
	
	OSCmessage();
	OSCmessage(const osc::ReceivedMessage &m, const IpEndpointName &remoteEndpoint);
	~OSCmessage();
	
	// getter
	const bool& is_valid() const { return _valid; }
	
	// getter, exposed in gdscript
	const bool& empty() const { return !_valid; }
	const String& ip() const { return _ip; }
	const int& port() const { return _port; }
	const String& address() const { return _address; }
	const String& typetag() const { return _typetag; }
	const int& arg_num() const { return _arg_num; }
	const Variant& arg(int p_idx) const { return _arguments[p_idx]; }
	
	// operators
	void copy( const OSCmessage& src );
	
protected:
	
	static void _bind_methods();
	const Array& arguments() const { return _arguments; }
	
private:
	
	bool _valid;
	String _ip;
	int _port;
	String _address;
	String _typetag;
	int _arg_num;
	Array _arguments;
	
};


#endif