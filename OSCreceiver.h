#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include "OSCmessage.h"
#include "core/engine.h"
#include "core/object.h"
#include "core/os/main_loop.h"
#include "gdOscMessage.h"
#include "ip/PacketListener.h"
#include "ip/UdpSocket.h"
#include "osc/OscPacketListener.h"
#include "osc/OscPrintReceivedElements.h"
#include "osc/OscReceivedElements.h"
#include "scene/main/node.h"

#ifndef GDOSC_RECEIVER_H
#define GDOSC_RECEIVER_H

class OSCreceiver : public Node, public osc::OscPacketListener {
  GDCLASS(OSCreceiver, Node);

 public:
  OSCreceiver();
  ~OSCreceiver();

  /* Set native mode to true to prevent the object to turn OSC messages to gd Dicitionaries.
   * If set to true, use method getNextMessage( gdOscMessage& msg ) to retrieve messages.
   * @remark native_mode is set to false by default.
   */
  void native_mode(bool enable);

  bool init(int port);
  bool start();
  void stop();

  bool has_waiting_messages();
  Ref<OSCmessage> get_next_message();

  /* USe this method to retrieve messages in C++. It is not exposed in gdscript, and
   * will return false if native_mode is not set to true.
   */
  bool getNextMessage(gdOscMessage& msg);

  // setters
  void set_max_queue(int max_queue);
  void set_autostart(bool autostart);
  void set_emit_signal(bool emit_signal);

  // getters
  _FORCE_INLINE_ const int& get_port() const { return _port; }
  _FORCE_INLINE_ int get_max_queue() const { return (int)_max_queue; }
  _FORCE_INLINE_ const bool& is_autostart() const { return _autostart; }
  _FORCE_INLINE_ const bool& is_emit_signal() const { return _emit_signal; }

 protected:
  void ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint);
  void _notification(int p_what);
  static void _bind_methods();

 private:
  std::mutex _lmutex;
  std::thread _lthread;
  UdpListeningReceiveSocket* _lsocket;

  int _port;
  std::size_t _max_queue;
  bool _autostart;
  bool _emit_signal;

  bool _native_mode;
  std::deque<gdOscMessage> _msg_queue;

  std::deque<OSCmessage>* _gd_queue_write;
  std::deque<OSCmessage>* _gd_queue_read;

  Ref<OSCmessage> _gd_next_msg;

  void create_buffers();
  void purge_buffers();
  void swap_buffers();
  void check_queue();
};

#endif