/*osc_listener*/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <thread>

#include "scene/main/node.h"
#include "print_string.h"

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
  std::string getOscMsgAsString(gdOscMessage message);

 protected:
  static void _bind_methods();

 private:
  void _notification(int what);
  OSCReceiver* osc_rcv;
};
#endif
