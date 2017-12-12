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


class OSCListener : public Object {
  GDCLASS(OSCListener, Object);

 public:
  OSCListener();
  ~OSCListener();
  bool setup(int port);
  String getOscMsgAsString(gdOscMessage m);
  Array getOscMessageAsArray(gdOscMessage m);
  void set_port(int port);
  int get_port () { return _port;}
  Array get_msg();

 protected:
  static void _bind_methods();
  // gdOscMessage _osc_message();
  std::thread listener;

 private:
  // void _notification(int what);
  bool start();
  OSCReceiver* osc_rcv;
  int _port;
  // gdOscMessage cur_msg;
  Array msg;

};
#endif
