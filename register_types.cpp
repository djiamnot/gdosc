/* register_types.cpp */

#include "register_types.h"

#include "OSCreceiver.h"
#include "OSCmessage.h"
#include "OSCsender.h"

void register_gdosc_types() {
	ClassDB::register_class<OSCreceiver>();
	ClassDB::register_class<OSCmessage>();
	ClassDB::register_class<OSCsender>();
}

void unregister_gdosc_types() {
   //nothing to do here
}
