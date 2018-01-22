# UnnamedEngine

## What is this?

This is an unfinished game engine.  It inspired by the architecture of [Overwatch](https://www.gdcvault.com/play/1024653/Networking-Scripted-Weapons-and-Abilities).

It is a personal project of Ryan Annis, but hopefully can implement some novel features in the future.

## Building

Install the Vulkan SDK https://www.lunarg.com/vulkan-sdk/ and ensure the environment variable $VK_SDK is set in your PATH.

The program can then be built using CMakeLists.txt in the root folder.  This requiress a C++17 compiler and has been tested on Windows 10 with Microsoft Visual Studio 2017. 

## Roadmap

It is difficult to give a roadmap at present as the project is in very early stages.  

### Implemented

- Client framework and connection to window manager functionality.
- Data driven resource management and static registration system.
- Strongly typed data specialization language, binary serialization for data files and meshes.
- Level loading.
- Multithreaded ECS framework and component memory management.
- Input system.
- Basic Vulkan renderer.

### Short Term Goals

-  More generalized render pass system.(https://www.gdcvault.com/play/1024612/FrameGraph-Extensible-Rendering-Architecture-in).
-  Voxel cone real time global illumination (http://leifnode.com/2015/05/voxel-cone-traced-global-illumination/).
-  Asset loading deferrment (asset loading is currently blocking).
-  Global Rewind + Playback (not important but must be done early).
-  Physics.
-  Specialized allocators (STL and lloc is just used).

### Unimplemented

- Sound
- Network and seperation of server/client duties
- Scripting

### "Sometime"

- Custom state based scripting language
- ECS paralellization
- Predictive network sync (?)
- Unique NPR styles

### Contribution Warning

This project is being actively worked on, but is currently highly unstable and is subject to change at anytime.

Anyone is welcome to work on it, look at implementation details, or use it as a base.  However it may be unwise to build a game on it or build features specifically for contribution.  No support will be provided at present.

## License

MIT
