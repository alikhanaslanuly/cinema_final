# Cinema Seat Preview: Real-Time Shadow Mapping

Computer Graphics Fundamentals SE-2409 Final Project

**Team**: Alikhan Amanzholov, Kuanysh Asaubaev, Arman Jussupgaliyev

## Building

### Windows
**Prerequisites**: Visual Studio 2019/2022, CMake in PATH

Open project folder in Visual Studio, or run `build.bat`

### Linux
**Prerequisites**: CMake

Run:
```
chmod +x build.sh && ./build.s
```

## Controls

- WASD & Mouse - Movement (hold shift to run)
- Left Mouse Button - Select
- T - Turn on/off shadows
- Arrow keys - Move light source
- P - Pause animation
- O - Switch camera view
- V - Turn on/off V-Sync
- [ , ] - Reduce/Increase bias
- K - Switch PCF core (1x1/3x3/5x5)
- C - Switch culling depth
- F5 - Start FPS benchmark
- F6 - Reload shaders
- F1/H - Help
- Esc - Back or exit

## Libraries and Technologies Used

### Libraries
- GLFW - Window and input
- GLAD - GL initialization
- glm - Math helpers
- stb_image - Textures decoding
- Dear ImGui - UI elements

### Technologies
- CMake - Building system
- C++17 - Language standard
- OpenGL 3.3 Core
