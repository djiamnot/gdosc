#include "scene/main/node.h"
#include "core/engine.h"
#include "core/os/main_loop.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <deque>
#include <thread>
#include "osc/OscReceivedElements.h"
#include "osc/OscPrintReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"
#include "ip/PacketListener.h"
#include "gdOscMessage.h"

#ifndef GDOSC_RECEIVER_H
#define GDOSC_RECEIVER_H

class OSCreceiver : 
	public Node,
	public osc::OscPacketListener {
	
	GDCLASS(OSCreceiver, Node);
	
public:
	
	OSCreceiver();
	~OSCreceiver();
	bool init(int port);
	
	bool start();
	void stop();
		
	bool has_waiting_messages();
	Dictionary get_next_message();

	// setters
	void set_max_queue( int max_queue );
	void set_autostart( bool autostart );
	
	// getters
	const int& get_port() const { return _port; }
	int get_max_queue() const { return (int)_max_queue; }
	const bool& get_autostart() const { return _autostart; }
	
protected:
	
	void ProcessMessage(const osc::ReceivedMessage &m, const IpEndpointName &remoteEndpoint);
	const Dictionary& add_gd_message( gdOscMessage* msg );
	void _notification(int p_what);
	static void _bind_methods();
	
private:
	
	std::thread _lthread;
	UdpListeningReceiveSocket* _lsocket;
	
	int _port;
	std::size_t _max_queue;
	bool _autostart;
	
	std::deque<Dictionary> _dict_queue;
	
	
	void check_queue();
	
};
	
#endif