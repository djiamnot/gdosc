#include <string>
#include <iostream>

#include "oscReceiver.h"


OSCReceiver::OSCReceiver() {
  std::cout << "OSC receiver" << std::endl;
  listenSocket = nullptr;
  setup(18000);
  start();
}

bool OSCReceiver::setup(int port) {
  // if (listenSocket) {
  //   stop();
  // }
  settings.port = port;
  return start();
}

bool OSCReceiver::start(){
  try {
    IpEndpointName name(IpEndpointName::ANY_ADDRESS, settings.port);
    listenSocket = new UdpListeningReceiveSocket(name, this);
  }
  catch (const std::exception& e) {
    std::cout << "receiver: " << e.what() << std::endl;
  }

  listenThread = std::thread([this]{
      std::cout << "start thread" << std::endl;
      while(listenSocket) {
        std::cout << "do stuff" << std::endl;
        try {
          listenSocket->Run();
        }
        catch (std::exception &e) {
          std::cout << " cannot listen " << e.what() << std::endl;
        }
      }
      std::cout << "end thread" << std::endl;
    });
  listenThread.detach();
  return true;
}

void OSCReceiver::ProcessMessage(const osc::ReceivedMessage &m, const IpEndpointName &remoteEndpoint){
  // for (::osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin(); arg != m.ArgumentsEnd(); ++arg){
  //   std::cout << arg << std::endl;
  // }

  gdOscMessage* msg = new gdOscMessage();

  msg->setAddress(m.AddressPattern());

  char endpointHost[IpEndpointName::ADDRESS_STRING_LENGTH];
  remoteEndpoint.AddressAsString(endpointHost);

  msg->setRemoteEndpoint(endpointHost, remoteEndpoint.port);


  try{
    for (::osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin(); arg != m.ArgumentsEnd(); ++arg){
      if(arg->IsInt32()) {
        msg->addIntArg(arg->AsInt32Unchecked());
      }
      else if (arg->IsFloat()){
        msg->addFloatArg(arg->AsFloatUnchecked());
      }
      else if (arg->IsString()) {
        msg->addStringArg(arg->AsStringUnchecked());
      }
    }
    messages.push_back(msg);
  }catch( osc::Exception& e ){
    // any parsing errors such as unexpected argument types, or
    // missing arguments get thrown as exceptions.
    std::cout << "error while parsing message: "
              << m.AddressPattern() << ": " << e.what() << "\n";
  }
}

bool OSCReceiver::hasWaitingMessages(){
  int queue_length = (int)messages.size();

  return queue_length > 0;
}

bool OSCReceiver::getNextMessage(gdOscMessage* message){
  if (messages.size() == 0) {
    return false;
  }
  gdOscMessage* src_message = messages.front();
	message->copy(*src_message);
	delete src_message;

	messages.pop_front();

  return true;
}

// void OSCReceiver::stop() {
//   listenSocket.reset();
// }
