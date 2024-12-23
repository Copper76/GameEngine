# ECS Game Engine

This project is a practice project to make a game engine using Entity-Component System, the graphics utilises GLFW library for rendering. The engine is called Fengshui at at the moment. The work is done in the [Engine](https://github.com/Copper76/GameEngine/tree/Engine) Branch, main will only display the latest stable build.

## Features
- 2D Renderer, supporting custom textures and shaders.
- 3D renderer, supporting directional lighting and custom shapes as well as built-in porimitives
- Batch rendering for both 2D and 3D
- Perspective and Orgthographic camera
- Editor where component data can be adjusted
- Event system which would reflectively call functions according to their dynamic type
- Entity-Component System for rendering, transform, position and camera

## Functions implemented
### Exploration and basics
- [X] Create a simple snake game using pure SFML to learn the basics of input handling and rendering. It was a simple practice with no connection with the rest of the engine. It is implemented in the [Snake Branch](https://github.com/Copper76/GameEngine/tree/Snake).
- [X] Remade the snake game with Fengshui using the Fengshui editor interface in [NewSnake Branch](https://github.com/Copper76/GameEngine/tree/NewSnake), all the game code are implemented in the editor layer which only calls functions from the Fengshui engine.
- [X] Get Logging working with spdlog
- [X] Create a entity component system structure, a simple ECS is created in the ECS folder.
### Rendering
- [X] Integrate GLFW into the ECS. The GLFW now works under render component, which stores all the information required for rendering.
- [X] Continue with the Cherno series on Hazel to further develop the renderer. The 2D renderer now supports batch rendering, subtexture 
- [X] Added perspective camera and 3d rendering to support the 3d physics system.
- [X] expand the 3d renderer to allow rendering with custom shapes, it now uses the same technique as physical shapes to build convex shape from a set of points
- [X] Added functionality to render a sphere, ready to use it in raytracing.
### UI
- [X] Added a preliminary editor, which can display entities in the scene in hierarchy, as well as modifying components
- [X] Improve the panels. The panels now allow add and removal of components at runtime, as well as the ability to modify most value based data, with the exception of hierarchy.
### Physics
- [X] Integrated the 3D Physics Engine that was previously made in [Physics Engine](https://github.com/Copper76/Physics-Engine), the engine doesn't play nicely with hierarchy yet.
- [X] Added offset and scale for colliders relative to the object scale. Sphere can only be scaled uniformly in physics to avoid complication
### Audio
- [X] Implement an audio system which can allow user to use all available audio input and output devices.
- [X] Add support of playing wav files through an audio source.
- [X] Add support for flac, ogg and mp3 files.
- [X] Add support for having multiple buffer for an audio file so it can play larger files seamlessly.
- [X] Add support to pause, stop audio as well as looping.
- [X] Implement the audio player component to the existing ECS system.
- [X] Added editor function to modify some properties.
- [X] adds audio listener
- [X] Add ability to modify audios through pitch shift.

## TODO
### Rendering
- [ ] Text Rendering
- [ ] game UI rendering
- [ ] 2D animation
### Packaging
- [ ] Allow packaging into web game
- [ ] Able to open other webpages
- [ ] Link to GPT
### Audio
- [ ] Look into more ways of modifying audio.
### Raytracing
- [ ] Look into raytracing and integrate it into the engine(stretch goal).

## Knonw Issues
- Rendering breaks when 2D is used with spheres
- Physics doesn't work well with hierarchy, I believe due to how transform is calculated

## Deprecated Objectives
- [ ] Batch the audio system similar to how rendering is done. (batching doesn't seem to work as nicely as opengl due to the natural of audio)
- [ ] Integrate SFML into the ECS
- [X] Develop several components and entities, simple ones to start with such as simple data such as transform. The system now supports a hierarchy component that stores the position of entity in hierarchy, and transform to represent the location and orientation of the component in the world as the basics, with more to come. The ECS system was not a real ECS system so is now deprecated.

## Requirement
- [sfml 2.6.1](https://github.com/SFML/SFML) (for snake game, deprecated)
- [glfw 3](https://glfw.org)
- [spdlog](https://github.com/gabime/spdlog.git)
- [glad](https://glad.dav1d.de/)
- [imgui](https://github.com/ocornut/imgui.git)
- [glm](https://github.com/g-truc/glm.git)
- [stb](https://github.com/nothings/stb)
- [OpenAL-Soft](https://github.com/kcat/openal-soft)
- [MiniAudio](https://github.com/mackron/miniaudio)

## Credit
Major thank you to Cherno for the series on [Hazel Engine](https://github.com/TheCherno/Hazel)
