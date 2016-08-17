#include "osc.h"


OSCsender::OSCsender() {
    std::cout << "OSCsender instantiated" << std::endl;
    destination = lo_address_new("localhost", "9000");
}

void OSCsender::testSend() {
    std::cout << "sending message" << std::endl;
    lo_send(destination, "/test", "i", 999);
}

void OSCsender::_bind_methods() {
    ObjectTypeDB::bind_method("testSend", &OSCsender::testSend);
    // ObjectTypeDB::bind_method("set_port", &OSC::set_port);
}


