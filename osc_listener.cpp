#include "osc_listener.h"

OSCListener::OSCListener() {
  std::cout << "OSC listener" << std::endl;
  osc_rcv = new OSCReceiver();
}

OSCListener::~OSCListener() {
  if(osc_rcv) {
    delete osc_rcv;
  }
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
        // String msg = getOscMsgAsString(message);
        Array msg = getOscMessageAsArray(message);
        // print_line(msg);
        // cur_msg = message;
        emit_signal("osc_message", msg);
      }
    }
  }
}

String OSCListener::getOscMsgAsString(gdOscMessage m) {
  std::string msg_string;
  msg_string = m.getAddress();
  msg_string += ":";
  for(int i =0; i < m.getNumArgs(); i++){
    msg_string += " " + m.getArgTypeName(i);
    msg_string += ":";
    if(m.getArgType(i) == TYPE_INT32) {
      msg_string += std::to_string(m.getArgAsInt32(i));
    }
    else if(m.getArgType(i) == TYPE_FLOAT) {
      msg_string += std::to_string(m.getArgAsFloat(i));
    }
    else if(m.getArgType(i) == TYPE_STRING) {
      msg_string += m.getArgAsString(i);
    }
    else {
      msg_string += "unknown";
    }
  }
  return String(msg_string.c_str());
}

Array OSCListener::getOscMessageAsArray(gdOscMessage m) {
  Array a;
  a.append(1);
  a.append(3.15);
  a.append("boohoo");

  return a;
}

// gdOscMessage OSCListener::_osc_message(){
//   print_line("signal callback called");
//   return cur_msg;
// }

void OSCListener::_bind_methods() {
  std::cout << "will be binding here" << std::endl;
  ADD_SIGNAL(MethodInfo("osc_message"));

}
