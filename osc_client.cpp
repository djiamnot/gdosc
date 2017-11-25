#include "osc_client.h"
// #include "core/ustring.h"

OSCclient::OSCclient() {
  std::string host = "localhost";
  osc_sender = new gdOscSender();
  osc_sender->init(host, 9020);
}

void OSCclient::testSend() {
  msg.addStringArg("test");
  osc_sender->sendMessage(msg);
}

void OSCclient::appendInt(int m) {
  msg.addIntArg(m);
}

void OSCclient::appendFloat(float m) {
  msg.addFloatArg(m);
}

void OSCclient::appendString(String m) {
  msg.addStringArg(m);
}

void OSCclient::sendMessage() {
  osc_sender->sendMessage(msg);
}

void OSCclient::reset() {
  msg.clear();
}

void OSCclient::_bind_methods() {
  std::cout << "binding should occur...." << std::endl;
  ClassDB::bind_method(D_METHOD("testSend"), &OSCclient::testSend);
  ClassDB::bind_method(D_METHOD("appendInt", "m"), &OSCclient::appendInt);
  ClassDB::bind_method(D_METHOD("appendFloat", "m"), &OSCclient::appendFloat);
  ClassDB::bind_method(D_METHOD("appendString", "m"), &OSCclient::appendString);
  ClassDB::bind_method(D_METHOD("sendMessage"), &OSCclient::sendMessage);
  ClassDB::bind_method(D_METHOD("reset"), &OSCclient::reset);
}
