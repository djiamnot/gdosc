/*osc_listener*/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <thread>

#include "scene/main/node.h"
#include "print_string.h"
#include "core/array.h"
#include "core/variant.h"

#include "osc/OscReceivedElements.h"
#include "osc/OscPrintReceivedElements.h"
#include "osc/OscPacketListener.h"

#include "ip/UdpSocket.h"
#include "ip/PacketListener.h"

#include "oscReceiver.h"
#include"core/dictionary.h"
#include "core/object.h")

#ifndef GDOSCL_H
#define GDOSCL_H


class OSCListener : public Node {
  GDCLASS(OSCListener, Node);

 public:
  OSCListener();
  ~OSCListener();
  
  bool setup(int port);
  void stop();
  
  String getOscMsgAsString(gdOscMessage m);
  Array getOscMessageAsArray(gdOscMessage m);
  //void setPort(int port);
  int getPort () { return _port;}
  Array getMessage();
  
  void dump( const bool& b ) {
	  _dump = b;
	}

 protected:
  static void _bind_methods();
  std::thread listener;
  bool _running;

 private:
  bool start();
  
  /*Hard cleanup: stops the thread and delete the receiver.
   Used in destructor.
   */
  void purge();
  
  OSCReceiver* osc_rcv;
  int _port;
  Array msg;
  
  bool _dump;

};
#endif
