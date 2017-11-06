/* osc.h */
#include "scene/3d/spatial.h"
#include "scene/3d/position_3d.h"
#include "scene/main/node.h"
#include "node_path.h"
#include "print_string.h"
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

class OSCclient : public Spatial {
  GDCLASS(OSCclient, Spatial);

 protected:
  static void _bind_methods();

 public:
  // UdpTransmitSocket transmitSocket( const osc::IpEndpointName& remoteEndpoint);
  OSCclient();
  void testSend();
  void _notification(int what);

  Spatial *parent;

  void set_id(String id);
  String get_id() const ;

 private:

  struct SatieData {
    String id;
  } satieData;

  std::unique_ptr<UdpTransmitSocket> sendSocket; //< sender socket
};

#endif
