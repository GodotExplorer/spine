This module adds [Spine](http://esotericsoftware.com/) animation support for godot game engine 3.0. It has been tested with Godot 3.0.2, but may contain bugs.
The 2.1 module is also avaiable in the [2.1 branch](https://github.com/GodotExplorer/spine/tree/2.1)

Current Spine runtime version for master branch **3.6.52.1** ([Github link](https://github.com/EsotericSoftware/spine-runtimes/tree/spine-libgdx-3.6.52.1)).
Current Spine runtime version for 2.1 branch **3.5.51**.

## About the license

This module is forked from [sanikoyes's godot branch](https://github.com/sanikoyes/godot/tree/develop/modules/spine) and some of the code is forked from [godot-spine-module](https://github.com/jjay/godot-spine-module). Both of the code are declared as MIT license.

The license of this module is under the [Spine Runtimes Software License](https://github.com/EsotericSoftware/spine-runtimes/blob/3.6/LICENSE).

## Usage

Add this code under `modules/spine` in your Godot source tree. You may either copy it or use `git submodule add`.

Build Godot using `scons platform=x11 tools=yes target=release_debug` or whatever [build options](http://docs.godotengine.org/en/latest/development/compiling/) you prefer.

Use the `Spine` type in your scene tree and load your animation into it as a resource.

## Further reference

The [Spine API Reference](http://esotericsoftware.com/spine-api-reference) is useful to learning more about how the code works.

