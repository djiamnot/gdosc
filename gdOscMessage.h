#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>

#include "gdOscArg.h"

class gdOscMessage{
 public:
  gdOscMessage();
  ~gdOscMessage();

  void clear();
  void setAddress(const std::string &address);
  std::string getAddress() const;
  std::string getRemoteIp() const;
  void setRemoteEndpoint(const std::string &host, int port);
  /// \return the remote host name/ip or "" if not set
	std::string getRemoteHost() const;

	/// \return the remote port or 0 if not set
  int getRemotePort() const;

  void addIntArg(int32_t arg);
  void addFloatArg(float arg);
  void addStringArg(std::string arg);

  int32_t getArgAsInt32(int index, bool typeConvert = false) const;
  float getArgAsFloat(int index, bool typeConvert = false) const;
  std::string getArgAsString(int index, bool typeConvert = false) const;

  gdOscArgType getArgType(int index) const;

  gdOscMessage& copy(const gdOscMessage& other);


 private:

	std::string address; //< OSC address, must start with a /
	// std::vector<ofxOscArg*> args; //< current arguments

	std::string remoteHost; //< host name/ip the message was sent from
  int remotePort; //< port the message was sent from

  std::vector<gdOscArg*> args;
};

class OscExc {
};
class OscExcInvalidArgumentType : public OscExc {
};
class OscExcOutOfBounds : public OscExc {
};
