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
  std::cout << "Process message called" << std::endl;
  // char endpointHost[IpEndpointName::ADDRESS_STRING_LENGTH];
  // std::cout << "address: " << (void) remoteEndpoint.AddressAsString(endpointHost); << std::endl;
  // for (::osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin(); arg != m.ArgumentsEnd(); ++arg){
  //   std::cout << arg << std::endl;
  // }

  try{
    // example of parsing single messages. osc::OsckPacketListener
    // handles the bundle traversal.

    if( std::strcmp( m.AddressPattern(), "/test1" ) == 0 ){
      // example #1 -- argument stream interface
      osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
      bool a1;
      osc::int32 a2;
      float a3;
      const char *a4;
      args >> a1 >> a2 >> a3 >> a4 >> osc::EndMessage;

      std::cout << "received '/test1' message with arguments: "
                << a1 << " " << a2 << " " << a3 << " " << a4 << "\n";

    }else if( std::strcmp( m.AddressPattern(), "/test2" ) == 0 ){
      // example #2 -- argument iterator interface, supports
      // reflection for overloaded messages (eg you can call
      // (*arg)->IsBool() to check if a bool was passed etc).
      osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
      // bool a1 = (arg++)->AsBool();
      int a2 = (arg++)->AsInt32();
      float a3 = (arg++)->AsFloat();
      const char *a4 = (arg++)->AsString();
      if( arg != m.ArgumentsEnd() )
        throw osc::ExcessArgumentException();

      std::cout << "received '/test2' message with arguments: "
                <<  " " << a2 << " " << a3 << " " << a4 << "\n";
    }
  }catch( osc::Exception& e ){
    // any parsing errors such as unexpected argument types, or
    // missing arguments get thrown as exceptions.
    std::cout << "error while parsing message: "
              << m.AddressPattern() << ": " << e.what() << "\n";
  }
}

// void OSCReceiver::stop() {
//   listenSocket.reset();
// }
