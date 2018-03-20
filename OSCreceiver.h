#include "scene/main/node.h"
#include "core/engine.h"
#include "core/os/main_loop.h"
#include "core/object.h"
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
#include "OSCmessage.h"

#ifndef GDOSC_RECEIVER_H
#define GDOSC_RECEIVER_H

class OSCreceiver : 
	public Node,
	public osc::OscPacketListener {
	
	GDCLASS(OSCreceiver, Node);
	
public:
	
	OSCreceiver();
	~OSCreceiver();
	
	/* Set native mode to true to prevent the object to turn OSC messages to gd Dicitionaries. 
	 * If set to true, use method getNextMessage( gdOscMessage& msg ) to retrieve messages.
	 * @remark native_mode is set to false by default.
	 */
	void native_mode( bool enable );
	
	bool init(int port);
	bool start();
	void stop();

	bool has_waiting_messages();
	Ref<OSCmessage> get_next_message();
	
	/* USe this method to retrieve messages in C++. It is not exposed in gdscript, and
	 * will return false if native_mode is not set to true.
	 */
	bool getNextMessage( gdOscMessage& msg );

	// setters
	void set_max_queue( int max_queue );
	void set_autostart( bool autostart );
	
	// getters
	const int& get_port() const { return _port; }
	int get_max_queue() const { return (int)_max_queue; }
	const bool& get_autostart() const { return _autostart; }
	
protected:
	
	void ProcessMessage(const osc::ReceivedMessage &m, const IpEndpointName &remoteEndpoint);
	void _notification(int p_what);
	static void _bind_methods();
	
private:
	
	std::thread _lthread;
	UdpListeningReceiveSocket* _lsocket;
	
	int _port;
	std::size_t _max_queue;
	bool _autostart;
	
	bool _native_mode;
	std::deque<gdOscMessage> _msg_queue;
	std::deque<OSCmessage> _gd_queue;
	
	Ref<OSCmessage> _gd_signal_msg;
	Ref<OSCmessage> _gd_next_msg;
	
	
	void check_queue();
	
};
	
#endif