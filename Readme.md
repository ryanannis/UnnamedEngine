# UnnamedEngine

![Alt text](/Screenshots/ss01.png "Engine Screenshot")
*Yes, this is the best image I could find.*

**Warning:  Why is the entire screen white?** 

Go back to 90a4738f9742ac2bd9b3b22ce2ee1b4d923be564.  I've currently deleted the entire renderer due to bad design and am redoing it.  (I should really be using Git branching).

## What is this?

This is a game engine (in VERY EARLY STAGES) inspired by the architecture of Overwatch    https://www.gdcvault.com/play/1024653/Networking-Scripted-Weapons-and-Abilities.

It is a personal project of Ryan Annis.

## Building

Install the Vulkan SDK https://www.lunarg.com/vulkan-sdk/ and ensure the environment variable $VULKAN_SDK is set in your PATH.

The program can then be built using CMakeLists.txt in the root folder.  This requiress a C++17 compiler and has been tested on Windows 10 with Microsoft Visual Studio 2017.

## Roadmap

It is difficult to give a roadmap at present as the project is in very early stages.  

### Implemented

- Client framework and connection to window manager functionality.
- Data driven resource management and static registration system.
- Strongly typed data specialization language, binary serialization for data files and meshes.
- Level format & loading.
- Efficient ECS framework and component memory management.
- Input system.
- Basic Vulkan renderer.

### Short Term Goals

-  Remove stringy references to resources & serialized field IDs.
-  More generalized render pass system.(https://www.gdcvault.com/play/1024612/FrameGraph-Extensible-Rendering-Architecture-in).
-  Deferred voxel cone real time global illumination (http://leifnode.com/2015/05/voxel-cone-traced-global-illumination/).
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

I've recieved a few messages about this:

This project is an in-progress learning project by the author and hence is very unstable.

Feel free to reference the code for implementation notes (many ideas in here are not available in non-huge engines), but it may be wise to wait for more stability to contribute or build an actual game with it.

## License

MIT License
