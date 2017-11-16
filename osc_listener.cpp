#include "osc_listener.h"

OSCListener::OSCListener() {
  std::cout << "OSC listener" << std::endl;
  osc_rcv = new OSCReceiver();
}

void OSCListener::_notification(int what) {
  if (!is_inside_tree()) return;



  switch(what) {
  case NOTIFICATION_READY: {
    set_process(true);
  } break;

    case NOTIFICATION_PROCESS: {
      if(osc_rcv->hasWaitingMessages()){
        gdOscMessage message;
        osc_rcv->getNextMessage(&message);
        std::cout << message.getAddress() << std::endl;
      }
    }
  }
}

void OSCListener::_bind_methods() {
  std::cout << "will be binding here" << std::endl;
}
