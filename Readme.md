# UnnamedEngine

## What is this?

This is an unfinished game engine.  It inspired by the architecture of [Overwatch](https://www.gdcvault.com/play/1024653/Networking-Scripted-Weapons-and-Abilities).

It is a personal project of Ryan Annis, but hopefully can implement some novel features in the future.

## Overview

### Implemented

- Client framework
- Data driven resource management and static registration system
- Level loading
- ECS framework and memory pools
- Input

### Current Work

- The renderer is being rewritten in Vulkan.  

### Short Term Goals

-  More generalized render pass system
-  Voxel cone real time global illumination
-  Custom binary formats and compression for data driven resources, levels and meshes.  Strong typing for data resource format. 
-  Global Rewind + Playback
-  Physics
-  Better generalized allocators than malloc

### Unimplemented

- Sound
- Network and seperation of server/client duties
- Scripting

### Contribution Warning

This project is being actively worked on, but is currently highly unstable and is subject to change at anytime.

Anyone is welcome to work on it, look at implementation details, or use it as a base.  However it may be unwise to build a game on it or build features to contribute back.

### "Sometime"

- Custom state based scripting language
- ECS paralellization
- Predictive network sync (?)
- Unique NPR styles

## License

MIT
