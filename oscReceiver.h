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

#ifndef _OSCRECEIVER_
#define _OSCRECEIVER_

struct oscReceiverSettings {
	int port = 18000;
	bool reuse = true;
};

class OSCReceiver : public ::osc::OscPacketListener {


 public:
  OSCReceiver(int port);
  ~OSCReceiver();
  bool hasWaitingMessages();
  bool getNextMessage(gdOscMessage*);
  //   bool setup(int port);
  
  bool start();

 protected:
	 
  void stop();
  virtual void ProcessMessage(const osc::ReceivedMessage &m, const IpEndpointName &remoteEndpoint);

 private:
  std::thread listenThread;
  UdpListeningReceiveSocket* listenSocket;
  oscReceiverSettings settings;

  std::deque<gdOscMessage*> messages;
};

#endif
