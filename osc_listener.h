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

#ifndef GDOSCL_H
#define GDOSCL_H


class OSCListener : public Node {
  GDCLASS(OSCListener, Node);

 public:
  OSCListener();
  ~OSCListener();
  String getOscMsgAsString(gdOscMessage m);

 protected:
  static void _bind_methods();
  // gdOscMessage _osc_message();

 private:
  void _notification(int what);
  OSCReceiver* osc_rcv;
  gdOscMessage cur_msg;

};
#endif
