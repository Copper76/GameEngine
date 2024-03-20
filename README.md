# ECS Game Engine

This project is a practice project to make a game engine using Entity-Component System, the graphics utilises GLFW library for rendering. The engine is called Fengshui at at the moment. The work is done in the [Engine](https://github.com/Copper76/GameEngine/tree/Engine) Branch, main will only display the latest stable build.

## Functions implemented
- [X] Create a simple snake game using pure SFML to learn the basics of input handling and rendering. It is implemented in the [Snake Branch](https://github.com/Copper76/GameEngine/tree/Snake)
- [X] Get Logging working with spdlog
- [X] Integrate GLFW into the ECS. The GLFW now works under render component, which stores all the information required for rendering.
- [X] Create a entity component system structure, a simple ECS is created in the ECS folder.
- [X] Develop several components and entities, simple ones to start with such as simple data such as transform. The system now supports a hierarchy component that stores the position of entity in hierarchy, and transform to represent the location and orientation of the component in the world as the basics, with more to come.

## TODO
- [ ] Create the snake game with the game engine after its completion
- [ ] Continue with the Cherno series on Hazel to further develop the renderer
- [ ] Review the physics engine and the rest to determine whether it's more viable to reduce the physics engine to 2d or create a 3d renderer.

## Deprecated Objectives
- [ ] Integrate SFML into the ECS

## Stretch Goal
- [ ] Integrate my physics engine into this game engine

## Requirement
- [sfml 2.6.1](https://github.com/SFML/SFML) (for snake game, deprecated)
- [glfw 3](https://glfw.org)
- [spdlog](https://github.com/gabime/spdlog.git)
- [glad](https://glad.dav1d.de/)
- [imgui](https://github.com/ocornut/imgui.git)
- [glm](https://github.com/g-truc/glm.git)
- [stb](https://github.com/nothings/stb)

## Credit
Major thank you to Cherno for the series on [Hazel Engine](https://github.com/TheCherno/Hazel)
