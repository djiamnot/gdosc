/* register_types.cpp */

#include "register_types.h"
// #include "object_type_db.h"
#include "osc_client.h"

void register_gdosc_types() {

  //ObjectTypeDB::register_type<OSCclient>();
  ClassDB::register_class<OSCclient>();
}

void unregister_gdosc_types() {
   //nothing to do here
}
