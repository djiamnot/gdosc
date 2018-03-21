/* register_types.cpp */

#include "register_types.h"
#include "osc_transmitter.h"
#include "osc_listener.h"

#include "OSCreceiver.h"
#include "OSCmessage.h"
#include "OSCsender.h"

void register_gdosc_types() {
	// old classes
	ClassDB::register_class<OSCtransmitter>();
	ClassDB::register_class<OSCListener>();
	// new classes
	ClassDB::register_class<OSCreceiver>();
	ClassDB::register_class<OSCmessage>();
	ClassDB::register_class<OSCsender>();
}

void unregister_gdosc_types() {
   //nothing to do here
}
