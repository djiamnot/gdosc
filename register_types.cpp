/* register_types.cpp */

#include "register_types.h"
// #include "object_type_db.h"
#include "osc_transmitter.h"
#include "osc_listener.h"

void register_gdosc_types() {

  //ObjectTypeDB::register_type<OSCtransmitter>();
  ClassDB::register_class<OSCtransmitter>();
  ClassDB::register_class<OSCListener>();
  ClassDB::register_class<gdOscMessage>();
}

void unregister_gdosc_types() {
   //nothing to do here
}
