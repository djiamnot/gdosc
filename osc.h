/* osc.h */
#include "reference.h"
#include <cstdlib>
#include <iostream>
extern"C" {
#include <lo/lo.h>
}
#ifndef OSCSENDER_H
#define OSCSENDER_H

#include "reference.h"

class OSCsender : public Reference {
  OBJ_TYPE(OSCsender, Reference);

  std::string destHost, destPort;
  lo_address destination;
  lo_message msg;
  
 protected:
  static void _bind_methods();

 public:
  OSCsender();
  void testSend();
};

#endif
