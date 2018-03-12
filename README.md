# gdosc
This is a Godot game engine module for receiving and sending Open Sound Control messages [OSC](http://opensoundcontrol.org/introduction-osc).
It uses oscpack as OSC protocol implementation so it potentially it works on all platforms that can handle oscpack. However it has been tested only on Linux (and, for the moment, I am not interested in any other platforms).

## Installation
### Godot engine
First, you will need the Godot engine sources. Head over to [Godot compilation instructions](http://docs.godotengine.org/en/latest/development/compiling/) to get Godot building on your system (make sure it can build with `use_llvm=yes`).

Then copy or link `gdosc` to `godot/modules` and recompile Godot with `scons platform=x11 use_llvm=yes`

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

## Worlkflow

First you need to instantiate OSCListener class. In my current workflow, I attach a script to root node in the scene. OSCListener emits a signal `osc_message` which you can pick up and do with what you want

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

And here's an approach for sending:

```python
extends OSCtransmitter

var new_sender = OSCtransmitter.new()
var parent

signal exit()

func _ready():
    set_process(true)
    parent = get_parent()
	# set the destination address and port
    new_sender.init("localhost", 9020)

func _process(delta):
	# build the message
	# OSC address
    new_sender.setAddress("/update")
	# some other parameters that we want to include
    new_sender.appendString(parent.get_name())
    new_sender.appendFloat(parent.translation.x)
    new_sender.appendFloat(parent.translation.y)
    new_sender.appendFloat(parent.translation.z)
	# actually send the message
    new_sender.sendMessage()
	# reset the queue
    new_sender.reset()
```
