#include "osc_listener.h"

OSCListener::OSCListener() {
  std::cout << "OSC listener" << std::endl;
  osc_rcv = new OSCReceiver();


}

void OSCListener::_bind_methods() {
  std::cout << "will be binding here" << std::endl;
}
