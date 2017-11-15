#include <iostream>
#include <cstring>
#include <cstdlib>
#include <thread>


#include "osc/OscReceivedElements.h"
#include "osc/OscPrintReceivedElements.h"
#include "osc/OscPacketListener.h"

#include "ip/UdpSocket.h"
#include "ip/PacketListener.h"

struct oscReceiverSettings {
	int port = 18000;        //< port to listen on
	bool reuse = true;   //< should the port be reused by other receivers?
	// bool start = true;   //< start listening after setup?
};

class OSCReceiver : public ::osc::OscPacketListener {


 public:
  OSCReceiver();

 protected:

  bool start();
  bool setup(int port);
  //void stop();
  virtual void ProcessMessage(const osc::ReceivedMessage &m, const IpEndpointName &remoteEndpoint);

 private:
  std::thread listenThread;
  UdpListeningReceiveSocket* listenSocket;
  oscReceiverSettings settings;
};
