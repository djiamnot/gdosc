#include "gdOscMessage.h"

gdOscMessage::gdOscMessage() : remoteHost(""), remotePort(0) {}

gdOscMessage::~gdOscMessage(){
	clear();
}

void gdOscMessage::setAddress(const std::string &address){
	this->address = address;
}

//--------------------------------------------------------------
std::string gdOscMessage::getAddress() const{
	return address;
}

//--------------------------------------------------------------
std::string gdOscMessage::getRemoteIp() const{
	return remoteHost;
}

//--------------------------------------------------------------
std::string gdOscMessage::getRemoteHost() const{
	return remoteHost;
}

//--------------------------------------------------------------
int gdOscMessage::getRemotePort() const{
	return remotePort;
}

void gdOscMessage::setRemoteEndpoint(const std::string &host, int port){
	remoteHost = host;
	remotePort = port;
}

void gdOscMessage::addIntArg(int32_t arg) {
  args.push_back( new gdOscArgInt32(arg));
}

void gdOscMessage::addFloatArg(float arg) {
  args.push_back( new gdOscArgFloat(arg));
}

void gdOscMessage::addStringArg(std::string arg) {
  args.push_back( new gdOscArgString(arg));
}
void gdOscMessage::clear(){
	address = "";
	remoteHost = "";
	remotePort = 0;
	// for(unsigned int i = 0; i < args.size(); ++i){
	// 	delete args[i];
	// }
	// args.clear();
}
