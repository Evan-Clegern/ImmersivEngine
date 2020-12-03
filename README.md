# ImmersivEngine
PROTECTED UNDER GNU AFFERO PUBLIC LICENSE.
## Current Version
```ImmersivEngine-v0.0-ba27-sz0.3:7```, passing current compile tests.

## About
ImmersivEngine is a prototype game engine written in C++, applying to OpenGL and aimed at Ubuntu-derived Linux installations. It is still heavily under construction.
### Reason For Inception
ImmersivEngine began as IECAI - a C++ project which was aiming to improve how video game NPCs would behave. However, it became very apparent that IECAI would require a lot more functionality than could be provided by a basic 'implementation' file and a large amount of JSON files. So, the decision was made to migrate the entire project and produce a full game engine. It's much more ambitious, but is unfortunately a requirement to enable Immersive NPC Behavior.
### :ledger: Includes
- [LibSDL](http://www.libsdl.org/hg.php)  : Window, Audio, 2D Graphics + I/O
- [OpenGL](https://www.opengl.org/)  : 3D Graphics and GPU Offloading
- [Assimp](https://github.com/assimp/assimp) : Model Format Loading :repeat:
- [Bullet3](https://github.com/bulletphysics/bullet3) : Physics Engine and Collisions :repeat:
- [Open-Source-Parsers/JSONCPP](https://github.com/open-source-parsers/jsoncpp)  : File Persistence I/O

## Systems
ImmersivEngine uses a mostly-custom setup for *materials* and *lighting* when rendering, but will be utilizing Collada-type models (via Assimp), Simple Directmedia and OpenGL for rendering and custom-named JSON files (via `jsoncpp`), providing a fair amount of standardization.
