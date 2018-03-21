# gdosc
This is a Godot game engine module for receiving and sending Open Sound Control messages [OSC](http://opensoundcontrol.org/introduction-osc).
It uses oscpack as OSC protocol implementation so it potentially it works on all platforms that can handle oscpack. However it has been tested only on Linux (and, for the moment, I am not interested in any other platforms).

## Installation

### Godot engine
First, you will need the Godot engine sources. Head over to [Godot compilation instructions](http://docs.godotengine.org/en/latest/development/compiling/) to get Godot building on your system (make sure it can build with `use_llvm=yes`).

Then copy or link `gdosc` to `godot/modules` and recompile Godot with `scons platform=x11 use_llvm=yes`

### deps

#### linux

If you wants to use clang, install it with:

`sudo apt install clang`

If you are using gcc, just skip the *use_llvm=yes* and it will be ok.

#### git

If you are not familiar with git, make sure it is installed:

`sudo apt install git`

Generate SSH key by following this [tutorial](https://www.siteground.com/kb/generate_ssh_key_in_linux/) and add it to your [github](https://help.github.com/articles/adding-a-new-ssh-key-to-your-github-account/) account - HTTPS links are not working with submodules!

Once done, use these 2 commands to get the repo up & running:

`git clone git@github.com:djiamnot/gdosc.git`

`git pull && git submodule init && git submodule update && git submodule status`

It will retrieve *oscpack* submodule.

### gdosc

Then, in your sources directory `git clone --recurse-submodules https://github.com/djiamnot/gdosc` and after that:

`cd godot`

`ln -s <path/to/gdosc/ modules/`

`scons platform=x11 use_llvm=yes`

### Example
Clone https://github.com/djiamnot/gdOscTest. See the README for information about the example.

## Known issues
At this time, gdosc supports only three data-types: _float_, _int_, and _string_. This is sufficient for me right now, but merge requests are welcome!

Theoretically, this should work on any OS but it has been developed and tested on Linux with Godot3.0

## Worlkflow (old school)

First you need to instantiate OSCListener class. In my current workflow, I attach a script to root node in the scene. OSCListener emits a signal `osdc_message` which you can pick up and do with what you want

```python

extends Spatial

signal ready()
signal osc_message()
var osc_ready = false

var osc_listener
# This is how we instantiate the main receiver class. It will probably exist in your scene and
# interested object will connect to its signal
func _ready():
    osc_listener  = OSCListener.new() # port 18002 by default
    #  port can be changed:
    # osc_listener.setPort(18004)

    # osc_message is a signal emitted on every reception of OSC message
    osc_liprobablementstener.connect("osc_message", self, "_on_osc_msg")
    # there is also 'osc_ready' signal for your convenience, emited as soon as the OSC receiver is
    # successfully instantiated

# I like to forward the message (at this time, I am considering this my top-level node and scenes
# will be instantiated below it)
func _on_osc_msg(val):
    emit_signal("osc_message", val)
```

So in the code above, pick it up and simply forward it so that scenes that I will instantiate under my root will subscribe to it. This is the kind of workflow that makes sense to me.

Then, something we can subscribe from any other node:

```python
var osc

func _ready():
    set_process_input(true)
    set_process(true)
    # here we get the node where OSC communication is received.
    # it emits a 'osc_message' signal
    osc = get_parent_spatial()
    osc.connect("osc_message", self, "_on_osc_message")

func _on_osc_message(val):
    print(get_name(), " ", val)
    # test for my name
    if val[0].right(1).match(get_name()):
        print("matching name -- _on_osc_message")
        if val[1] == "pos":
            translate(Vector3(val[2], val[3], val[4]))
```

### Sending messages

First of all, you need to add an **OSCtransmitter** node in your scene. It is located in the first level of the tree, between *HTTPRequest* and *ResourcePreloader*.

![OSCtransmitter in creation menu](https://frankiezafe.org/images/7/7c/Godot_gdosc_OSCtransmitter.png)

Once done, attach a script to it that looks like this:

```python
extends OSCtransmitter

signal exit()

func _ready():
	set_process(true)
	# initialisation of OSC sender, on port 25000 and with a buffer size of 1024
	init("localhost", 25000, 1024)
	framecount = 0
	pass

func _process(delta):
	# creation of the new message
	setAddress("/update")
	# appending data
	appendInt(framecount)
	# sending the message to the client
	sendMessage()
	# cleanup of the message
	reset()
	framecount += 1
	pass
```

## Worlkflow (new classes)

See [gdosc-demo](https://github.com/frankiezafe/gdosc-demo/commits/master) for an example on how to use these objects.

### OSCreceiver - reception of OSC message in godot

First step is to add an *OSCreceiver* to your scene and configure it.

* **Port**: number of the socket receiving messages;
* **Max Queue**: maximum number of messages to keep in memory - can be seen as the maximum number of messages that might be received between 2 frames;
* **Autostart**: set this to true to start the reception as soon as the scene starts;
* **Emit signal**: set this to true to fire *osc_message_received* signals automatically.

If **Emit signal** is disabled, you have to parse received messages via a loop in * _process* (or any other method called regularly).

#### parsing messages at each *_process*

To do so, append a script to your *OSCreceiver*.

```python
extends OSCreceiver

func _process(delta):
	while( has_waiting_messages() ):
		var msg = get_next_message()
		# the osc message is ready to be used at this point
		print( msg )

func _ready():
	set_process(true)
	pass
```

#### using *signals*

If **Emit signal** is enabled, at each [NOTIFICATION_PROCESS](http://docs.godotengine.org/en/3.0/getting_started/step_by_step/scripting_continued.html?highlight=NOTIFICATION_PROCESS), *OSCreceiver* will fire an **osc_message_received** signal.

To broadcast signals, you have to connect it to *func* of other objects. Let say that your scene is structured as follows:

* *root*
* * *OSCreceiver*
* * *MeshObject*, with a script containing a func **parse_osc(msg)**
* * *TextEdit*, with a script containing a func **dump_osc(msg)**

To send messages to *MeshObject* and *TextEdit*, attach a script to *OSCreceiver*:

```python
extends OSCreceiver

func _ready():
	connect( "osc_message_received", get_parent().get_node( "TextEdit"), "dump_osc" )
	connect( "osc_message_received", get_parent().get_node( "MeshObject"), "parse_osc" )
	pass
```

**Important**: Do mix retrieval methods! Why? All receveid messages are temporarily stored in a buffer. Retrieving a message from the buffer deletes it. As signals are fired after the execution of *_process()*, if you are mixing both retrieval methods, **no message** will be fired via signals. Indeed, all messages would already have been consumed by the * while* loop of the previous chapter.

#### OSCmessage

The object returned by *get_next_message()* or passed along with *osc_message_received* is an *OSCmessage* object.

Here are its gdscipt methods:

* *empty()*: if true,there nothing in this message.
* *ip()* : IP of the sender;
* *port()* : port of the sender;
* *address()* : the address of the message, looking like "/smthng/else";
* *typetag()* : a list of characters specifying the type of each arguments received;
* *arg_num()* : number of arguments in the received message;
* *arg(i)* : returns the arguments at position *i*.

Example of script using a message:

```python
func parse_osc( msg ):
	if ( msg.empty() ):
		return
	if ( !msg.ip() == "127.0.0.1" ):
		return
	if ( !msg.address() == "/pm/pos" ):
		return
	received_pos = Vector3( msg.arg(0),msg.arg(1),msg.arg(2))
	set_translation(received_pos)
```

### OSCsender - emisson of OSC message from godot

First step is to add an *OSCsender* to your scene and configure it.

* **Ip**: IP address of the computer to send messages to, *127.0.0.1* by default;
* **Port**: number of the socket to send messages to;
* **Buffersize**: maximum size of the messages sent from this object, in bytes;
* **Autostart**: set this to true to start the emission as soon as the scene starts;
* **Autoclear**: set this to true to cleanup the messages automatically after each call to *msg_send()*.

If *Autostart* is disabled, you will have to call *start()* from gdscript before starting the emission.

If *Autoclear* is disabled, you will have call *msg_clear()* from gdscript. This can be usefull when the object is sending the same message continuously. In this case, configure the message once, then just call *msg_send()* to send it.

#### Checking the status of the object

You might need to test the status of the object from gdscript.

* **init(String, int)**: stops the emission and verify the validity of the IP address and the port (equivalent to *is_ready()*);
* **is_ready()**: returns true if the IP and the port are 'valid', meaning that the IP is not an empty string and the port is above 0, this does not test the connection;
* **is_started()**: returns true the socket has been successfully binded, message emission is enabled if true.

#### Creating a message

Good practice involves setting the address of the message before adding values in it. But it makes no difference if you do it in reverse order.

List of methods available:

* **msg_address(String)**: set the address of the message, a */* separated string, looking like */some/thing*;
* **msg_add_int(int)**: append an int to the message;
* **msg_add_real(real)**: append an real (float or double) to the message;
* **msg_add_string(String)**: append a String to the message;
* **msg_add_v2(Vector2)**: append a Vector2 to the message, as a suite of real;
* **msg_add_v3(Vector3)**: append a Vector3 to the message, as a suite of real;
* **msg_add_quat(Quat)**: append a quaternion to the message, in W,X,Y,Z order;
* **msg_add_transform(Transform)**: append a complete Transform to the message, the matrix3x3 first, followed by the position;
* **msg_send()**: sends the message if the object is successfullt started;
* **msg_clear()**: clear the content of the previous message (done automatically after *msg_send()* if *Autoclear* is enabled).

Script sample:

```python
extends OSCsender

var parent = null

func _process(delta):
	if (parent == null):
		return
	msg_address("/emitter/rot" )
	msg_add_quat( Quat(parent.global_transform.basis) )
	msg_send()
	
	msg_address("/emitter/pos" )
	msg_add_v3( parent.global_transform.origin )
	msg_send()

func _ready():
	set_process(true)
	pass
```

The parent of this *OSCsender* register itself as the parent at startup:

```python
extends Spatial

func _process(delta):
	rotate_y( delta )
	pass

func _ready():
	set_process(true)
	get_node("OSCsender").parent = self
	pass
```