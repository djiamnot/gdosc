/* osc.h */
#include "reference.h"
#include <cstdlib>
#include <iostream>
#include <memory>

#include <osc/OscOutboundPacketStream.h>
#include <ip/UdpSocket.h>

#define ADDRESS "127.0.0.1"
#define PORT 9000
#define OUTPUT_BUFFER_SIZE 1024

#ifndef GDOSC_H
#define GDOSC_H

class OSCclient : public Reference {
  GDCLASS(OSCclient, Reference);

 protected:
  static void _bind_methods();

 public:
  // UdpTransmitSocket transmitSocket( const osc::IpEndpointName& remoteEndpoint);
  OSCclient();
  void testSend();

 private:
  std::unique_ptr<UdpTransmitSocket> sendSocket; //< sender socket
};

#endif
