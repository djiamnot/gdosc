#include "osc_transmitter.h"

OSCtransmitter::OSCtransmitter() {
  std::string host = "localhost";
  osc_sender = new gdOscSender();
  // osc_sender->init(host, 9020);
}

OSCtransmitter::~OSCtransmitter() {
  if(osc_sender){
    delete osc_sender;
  }
}


void OSCtransmitter::init(String host, int port) {
  std::string _host(utils::gdStringToString(host));
  // std::wstring ws(host.c_str());
  // std::string _host(ws.begin(), ws.end());
  // osc_sender = new gdOscSender();
  osc_sender->init(_host, port );
}

void OSCtransmitter::testSend() {
  msg.addStringArg("test");
  osc_sender->sendMessage(msg);
}

void OSCtransmitter::setAddress(String a) {
  std::string _addy(utils::gdStringToString(a));
  msg.setAddress(_addy);
}

void OSCtransmitter::appendInt(int m) {
  msg.addIntArg(m);
}

void OSCtransmitter::appendFloat(float m) {
  msg.addFloatArg(m);
}

void OSCtransmitter::appendString(String m) {
  msg.addStringArg(m);
}

void OSCtransmitter::sendMessage() {
  osc_sender->sendMessage(msg);
}

void OSCtransmitter::reset() {
  msg.clear();
}

void OSCtransmitter::_bind_methods() {
  std::cout << "binding should occur...." << std::endl;
  ClassDB::bind_method(D_METHOD("testSend"), &OSCtransmitter::testSend);
  ClassDB::bind_method(D_METHOD("init"), &OSCtransmitter::init);
  ClassDB::bind_method(D_METHOD("setAddress", "a"), &OSCtransmitter::setAddress);
  ClassDB::bind_method(D_METHOD("appendInt", "m"), &OSCtransmitter::appendInt);
  ClassDB::bind_method(D_METHOD("appendFloat", "m"), &OSCtransmitter::appendFloat);
  ClassDB::bind_method(D_METHOD("appendString", "m"), &OSCtransmitter::appendString);
  ClassDB::bind_method("sendMessage", &OSCtransmitter::sendMessage);
  ClassDB::bind_method("reset", &OSCtransmitter::reset);
}
