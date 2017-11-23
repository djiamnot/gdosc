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
  // ClassDB::bind_method(D_METHOD("testSend"), &OSCclient::testSend);
  // ClassDB::bind_method(D_METHOD("set_id"), &OSCclient::set_id);
  // ClassDB::bind_method(D_METHOD("get_id"), &OSCclient::get_id);
  // ClassDB::bind_method(D_METHOD("set_send_update"), &OSCclient::set_send_update);
  // ClassDB::bind_method(D_METHOD("get_send_update"), &OSCclient::get_send_update);
  // ClassDB::bind_method(D_METHOD("set_satie_category"), &OSCclient::set_satie_category);
  // ClassDB::bind_method(D_METHOD("get_satie_category"), &OSCclient::get_satie_category);
  // ClassDB::bind_method(D_METHOD("set_satie_type"), &OSCclient::set_satie_type);
  // ClassDB::bind_method(D_METHOD("get_satie_type"), &OSCclient::get_satie_type);
  // ClassDB::bind_method(D_METHOD("set_satie_group"), &OSCclient::set_satie_group);
  // ClassDB::bind_method(D_METHOD("get_satie_group"), &OSCclient::get_satie_group);

  //   // ObjectTypeDB::bind_method("set_port", &OSC::set_port);
  // ADD_GROUP("SATIE", "");
  // ADD_PROPERTY(PropertyInfo(Variant::STRING, "id"), "set_id", "get_id");
  // ADD_PROPERTY(PropertyInfo(Variant::BOOL, "send_update", PROPERTY_HINT_NONE, "Update satie continuously"), "set_send_update", "get_send_update");
  // ADD_PROPERTY(PropertyInfo(Variant::STRING, "Category", PROPERTY_HINT_ENUM, "Audiosource, Effect"), "set_satie_category", "get_satie_category");
  // ADD_PROPERTY(PropertyInfo(Variant::STRING, "Type", PROPERTY_HINT_ENUM, "None"), "set_satie_type", "get_satie_type");
  //   // ADD_PROPERTY(PropertyInfo(Variant::INT, "type", PROPERTY_HINT_ENUM, "None,Sin"), "set_type", "get_type");

}
