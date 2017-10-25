#include "osc.h"

OSCsender::OSCsender() {
    std::cout << "OSCsender instantiated!" << std::endl;
}

void OSCsender::testSend() {
    std::cout << "sending message" << std::endl;

    // UdpTransmitSocket transmitSocket( IpEndpointName( ADDRESS, PORT ) );

    // char buffer[OUTPUT_BUFFER_SIZE];
    // osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

    // p << osc::BeginBundleImmediate
    //   << osc::BeginMessage( "/test1" )
    //   << true << 23 << (float)3.1415 << "hello" << osc::EndMessage
    //   << osc::BeginMessage( "/test2" )
    //   << true << 24 << (float)10.8 << "world" << osc::EndMessage
    //   << osc::EndBundle;

    // transmitSocket.Send( p.Data(), p.Size() );
}

void OSCsender::_bind_methods() {
  ClassDB::bind_method(D_METHOD("testSend"), &OSCsender::testSend);
    // ObjectTypeDB::bind_method("set_port", &OSC::set_port);
}
