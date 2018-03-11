# gdosc
This is a Godot game engine module for receiving and sending Open Sound Control messages [OSC](http://opensoundcontrol.org/introduction-osc).
It uses oscpack as OSC protocol implementation so it potentially it works on all platforms that can handle oscpack. However it has been tested only on Linux (and, for the moment, I am not interested in any other platforms).

## Installation
### Godot engine
First, you will need the Godot engine sources. Head over to [Godot compilation instructions] (http://docs.godotengine.org/en/latest/development/compiling/) to get Godot building on your system (make sure it can build with `use_llvm=yes`).

### gdosc
Then, in your sources directory `git clone --recurse-submodules https://github.com/djiamnot/gdosc` and after that:

`cd godot`

`ln -s <path/to/gdosc/ modules/`

`scons platform=x11 use_llvm=yes`

### Example
Clone https://github.com/djiamnot/gdOscTest. See the README for information about the example.

## Known issues
At this time, gdosc supports only three data-types: _float_, _int_, and _string_. This is sufficient for me right now, but merge requests are welcome!

Theoretically, this should work on any OS but it has been developed and tested on Linux.
