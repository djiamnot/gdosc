/* register_types.cpp */

#include "register_types.h"
#include "osc_transmitter.h"
#include "osc_listener.h"

#include "OSCreceiver.h"

void register_gdosc_types() {
  //ObjectTypeDB::register_type<OSCtransmitter>();
  ClassDB::register_class<OSCtransmitter>();
  ClassDB::register_class<OSCListener>();
  // new classes
  ClassDB::register_class<OSCreceiver>();
}

void unregister_gdosc_types() {
   //nothing to do here
}
