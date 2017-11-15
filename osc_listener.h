/*osc_listener*/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <thread>

#include "scene/main/node.h"

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

 protected:
  static void _bind_methods();

 private:
  OSCReceiver* osc_rcv;
};
#endif
