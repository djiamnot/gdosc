#include "osc.h"


OSCsender::OSCsender() {
    std::cout << "OSCsender instantiated!" << std::endl;
}

void OSCsender::testSend() {
    std::cout << "sending message" << std::endl;
}

void OSCsender::_bind_methods() {
  ClassDB::bind_method(D_METHOD("testSend"), &OSCsender::testSend);
    // ObjectTypeDB::bind_method("set_port", &OSC::set_port);
}
