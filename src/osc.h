/* osc.h */
#include "reference.h"
#include <cstdlib>
#include <iostream>

#ifndef OSCSENDER_H
#define OSCSENDER_H

#include "reference.h"

class OSCsender : public Reference {
  GDCLASS(OSCsender, Reference);

  std::string destHost, destPort;

 protected:
  static void _bind_methods();

 public:
  OSCsender();
  void testSend();
};

#endif
