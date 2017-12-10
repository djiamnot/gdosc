#include "osc_listener.h"

OSCListener::OSCListener() {
  int port = 18002;
  try {
  osc_rcv = new OSCReceiver(port);
  std::cout << "OSC listener port: " << port << std::endl;
  }
  catch (const std::exception& e) {
    std::cout << "Constructor tried to instantiate OSCReceiver but " << e.what() << std::endl;
  }
}

OSCListener::~OSCListener() {
  if(osc_rcv) {
    delete osc_rcv;
  }
}

// void OSCListener::set_port(int port) {
//   delete osc_rcv;
//   _port = port;
//   osc_rcv = new OSCReceiver(_port);

// }

bool OSCListener::setup(int port) {

  try {
    osc_rcv->setup(port);
    std::cout << "setting port: " << port << std::endl;
  }
  catch (const std::exception& e) {
    std::cout << "listener.setup failed: " << e.what() << std::endl;
  }
}

// void OSCListener::_notification(int what) {
//   if (!is_inside_tree()) return;

//   switch(what) {
//   case NOTIFICATION_READY: {
//     set_process(true);
//     emit_signal("ready");
//   } break;

//   case NOTIFICATION_PROCESS: {
//     if(osc_rcv->hasWaitingMessages()){
//       gdOscMessage message;
//       osc_rcv->getNextMessage(&message);
//       // String msg_str = getOscMsgAsString(message);
//       // Array msg;
//       msg = getOscMessageAsArray(message);
//       // print_line(msg_str);
//       // cur_msg = message;
//       std::cout << "got a message of size " << msg.size() << std::endl;
//       // emit_signal("osc_message", msg);
//       schlep_msg();
//       std::cout << "processed message" << std::endl;
//     }
//   }
//   }
// }

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
  a.append(String(m.getAddress().c_str()));
  for(int i =0; i < m.getNumArgs(); i++){
    if(m.getArgType(i) == TYPE_INT32) {
      a.append(m.getArgAsInt32(i));
    }
    else if(m.getArgType(i) == TYPE_FLOAT) {
      a.append(m.getArgAsFloat(i));
    }
    else if(m.getArgType(i) == TYPE_STRING) {
      a.append(m.getArgAsString(i).c_str());
    }
    else {
      a.append("unknown");
    }
  }
  return a;
}

Array OSCListener::get_msg(){
  std::cout << "get_msg called" << std::endl;
  if(osc_rcv->hasWaitingMessages()){
    gdOscMessage message;
    osc_rcv->getNextMessage(&message);
    // String msg_str = getOscMsgAsString(message);
    // Array msg;
    msg = getOscMessageAsArray(message);
    // emit_signal("osc_message");
    std::cout << "osc_message emited" << std::endl;
    return msg;
    }
}

// gdOscMessage OSCListener::_osc_message(){
//   print_line("signal callback called");
//   return cur_msg;
// }

void OSCListener::_bind_methods() {
  std::cout << "will be binding here" << std::endl;

  ClassDB::bind_method(D_METHOD("setup", "port"), &OSCListener::setup);
  // ClassDB::bind_method(D_METHOD("set_port", "port"), &OSCListener::set_port);
  // ClassDB::bind_method(D_METHOD("get_port"), &OSCListener::get_port);
  // ClassDB::bind_method(D_METHOD("get_msg"), &OSCListener::get_msg);
  ClassDB::bind_method(D_METHOD("get_msg"), &OSCListener::get_msg);
  // ADD_PROPERTY(PropertyInfo(Variant::INT, "port"), "set_port", "get_port");
  // ADD_SIGNAL(MethodInfo("osc_message"));
  // ADD_SIGNAL(MethodInfo("ready"));


}
