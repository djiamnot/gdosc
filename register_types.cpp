/* register_types.cpp */

#include "register_types.h"
// #include "object_type_db.h"
#include "osc.h"

void register_oscsender_types() {

  //ObjectTypeDB::register_type<OSCsender>();
  ClassDB::register_class<OSCsender>();
}

void unregister_oscsender_types() {
   //nothing to do here
}
