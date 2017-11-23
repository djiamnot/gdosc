#include <cstdlib>
#include <iostream>
#include <memory>

#include <osc/OscOutboundPacketStream.h>
#include <ip/UdpSocket.h>

#include "gdOscMessage.h"

#ifndef _SENDERH_
#define _SENDERH_

struct gdOscSenderSettings {
  std::string host = "localhost";
  int port = 0;
};

class gdOscSender{
 public:
  gdOscSender(){};
  bool init(std::string host, int port);
  bool setup(gdOscSenderSettings &settings);
  void clear();

  void sendMessage(gdOscMessage &message);
  void appendMessage(gdOscMessage &message, osc::OutboundPacketStream &p);


 private:
  gdOscSenderSettings settings;

  std::unique_ptr<UdpTransmitSocket> sendSocket;
};

#endif
