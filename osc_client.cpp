#include "osc_client.h"

OSCclient::OSCclient() {
  // create socket
  IpEndpointName name = IpEndpointName(ADDRESS, PORT);
  UdpTransmitSocket *socket = nullptr;
  socket = new UdpTransmitSocket( name );
  sendSocket.reset(socket);
  std::cout << "OSCclient instantiated!" << std::endl;
  testSend();
}



void OSCclient::testSend() {
    std::cout << "sending message" << std::endl;
    std::cout << get_parent() << std::endl;

    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

    p << osc::BeginBundleImmediate
      << osc::BeginMessage( "/test1" )
      << true << 23 << (float)3.1415 << "hello" << osc::EndMessage
      << osc::BeginMessage( "/test2" )
      << true << 24 << (float)10.8 << "world" << osc::EndMessage
      << osc::EndBundle;

    sendSocket->Send( p.Data(), p.Size() );
}

void OSCclient::_notification(int what) {
  if (!is_inside_tree()) return;

  switch (what){
  case NOTIFICATION_READY: {
    parent = get_parent_spatial();
    print_line(parent->get_name());

    if (parent) {
      set_process(true);
      OSCclient::set_id(get_name());
    }
  } break;

  case NOTIFICATION_PROCESS: {
    print_line(parent->get_translation());
    //    print_line(parent->get_name ;
    // spatial = parent->get_path();
    // print_line(spatial->get_translation());

  }

  }
}

void OSCclient::set_id(String id) {
  // std::cout << "Setting id: " << _id << std::endl;
  satieData.id = id;
  print_line("set : "+satieData.id);
}

String OSCclient:: get_id ()  const{
  //String id = get_name();
  print_line("get : "+satieData.id);
  return satieData.id;
}

void OSCclient::set_send_update(bool b) {
  satieData.send_update = b;
}

bool OSCclient::get_send_update() const{
  return satieData.send_update;
}

void OSCclient::set_satie_category(String category) {
  satieData.satie_category = category;
}

String OSCclient::get_satie_category() const {
  return satieData.satie_category;
}

void OSCclient::set_satie_type(String type) {
  satieData.satie_type = type;
}

String OSCclient::get_satie_type() const {
  return satieData.satie_type;
}

void OSCclient::set_satie_group(String group) {
  satieData.satie_group = group;
}

String OSCclient::get_satie_group() const {
  return satieData.satie_group;
}



void OSCclient::_bind_methods() {
  ClassDB::bind_method(D_METHOD("testSend"), &OSCclient::testSend);
  ClassDB::bind_method(D_METHOD("set_id"), &OSCclient::set_id);
  ClassDB::bind_method(D_METHOD("get_id"), &OSCclient::get_id);
  ClassDB::bind_method(D_METHOD("set_send_update"), &OSCclient::set_send_update);
  ClassDB::bind_method(D_METHOD("get_send_update"), &OSCclient::get_send_update);
  ClassDB::bind_method(D_METHOD("set_satie_category"), &OSCclient::set_satie_category);
  ClassDB::bind_method(D_METHOD("get_satie_category"), &OSCclient::get_satie_category);
  ClassDB::bind_method(D_METHOD("set_satie_type"), &OSCclient::set_satie_type);
  ClassDB::bind_method(D_METHOD("get_satie_type"), &OSCclient::get_satie_type);
  ClassDB::bind_method(D_METHOD("set_satie_group"), &OSCclient::set_satie_group);
  ClassDB::bind_method(D_METHOD("get_satie_group"), &OSCclient::get_satie_group);

    // ObjectTypeDB::bind_method("set_port", &OSC::set_port);
  ADD_PROPERTY(PropertyInfo(Variant::STRING, "id"), "set_id", "get_id");
  ADD_PROPERTY(PropertyInfo(Variant::BOOL, "send_update", PROPERTY_HINT_NONE, "Update satie continuously"), "set_send_update", "get_send_update");
    // ADD_PROPERTY(PropertyInfo(Variant::INT, "type", PROPERTY_HINT_ENUM, "None,Sin"), "set_type", "get_type");

}
