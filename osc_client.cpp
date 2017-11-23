#include "osc_client.h"

OSCclient::OSCclient() {
  std::string host = "localhost";
  osc_sender = new gdOscSender();
  osc_sender->init(host, 9020);
}

void OSCclient::testSend() {
  msg.addStringArg("test");
  osc_sender->sendMessage(msg);
}

void OSCclient::_bind_methods() {
  std::cout << "binding should occur...." << std::endl;
  ClassDB::bind_method(D_METHOD("testSend"), &OSCclient::testSend);
}
