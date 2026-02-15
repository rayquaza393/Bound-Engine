# Bound Engine v2.1 - Foundation Architecture

A lightweight, CPU-focused 3D game engine emphasizing efficiency and compatibility, specifically designed for proprietary MMO development with **clear CPU/GPU separation**: the GPU handles rendering, while the CPU manages world orchestration, AI, navigation, and physics.

**Current Focus**: Modular testing framework, v2.1 (SDL2 + OpenGL)

---

## ?? Quick Links

- **[PROJECT_SCOPE.md](./PROJECT_SCOPE.md)** - Full architecture, roadmap, and design principles
- **[TESTING_FRAMEWORK.md](./TESTING_FRAMEWORK.md)** - Modular test system design and implementation
- **[CONTRIBUTING.md](./CONTRIBUTING.md)** - Development guidelines and code standards
- **[EDITOR.md](./EDITOR.md)** - In-engine editor documentation

---

## ?? What is Bound-Engine?

Bound-Engine is a **proprietary game engine** designed specifically for MMO development with:

- **OpenGL Rendering**: Lightweight GPU-focused rendering pipeline
- **World Orchestration**: CPU-driven entity management, AI, pathfinding
- **Server Integration**: Custom C++ API for connecting to proprietary MMO server
- **Lean Philosophy**: No bloat, only features needed for production gameplay
- **Cross-Platform**: SDL2 support for Windows, Linux, macOS

### Architecture at a Glance

Rendering (GPU)          World Sim (CPU)           Platform (SDL2)
?? OpenGL               ?? Entities              ?? Window
?? Shaders              ?? AI/Pathfinding        ?? Input
?? Meshes               ?? Physics               ?? Cross-platform
?? Camera               ?? Server Sync
                        ?? Level Management

---

## ??? Project Structure

Bound-Engine/
??? Core/
?   ??? Render/          # OpenGL rendering pipeline
?   ??? Game/            # Entity, World, Level management
?   ??? Assets/          # Asset loading & caching
?   ??? Editor/          # In-engine editor (ImGui)
?   ??? Serialization/   # Level format I/O
?   ??? Math/            # Vector, matrix, geometry
?
??? Platform/            # SDL2 window & input abstraction
??? Tests/               # Modular test framework
?   ??? RenderTests/
?   ??? AssetTests/
?   ??? SimulationTests/
?   ??? IntegrationTests/
?
??? Assets/              # Game content (models, textures, shaders, levels)
??? Main.cpp/.h          # Game entry point
?
??? Documentation/
    ??? PROJECT_SCOPE.md         # This document
    ??? TESTING_FRAMEWORK.md     # Test system design
    ??? CONTRIBUTING.md          # Code standards
    ??? EDITOR.md                # Editor guide

---

## ?? Getting Started

### Prerequisites
- C++14 or later
- SDL2 (already in `packages.config`)
- OpenGL 4.5+
- CMake 3.16+ (or Visual Studio 2022)

### Building

# Using Visual Studio
# Open Bound-Engine.sln and build

# Using CMake (future)
mkdir build && cd build
cmake ..
cmake --build .

### Running Tests

# Run all tests
./TestRunner.exe ./Tests

# Run specific suite
./TestRunner.exe ./Tests/RenderTests

### Running Game

./Bound-Engine.exe


---

## ?? Current Status (v2.1)

### ? Completed
- [x] SDL2 + OpenGL integration
- [x] Basic mesh rendering
- [x] Camera system (FPS-style)
- [x] Asset manager framework
- [x] World & Level containers
- [x] Shader compilation & management
- [x] Math library (Vec3, Mat4, etc.)

### ??? In Progress
- [ ] **Modular test framework** (active)
- [ ] Entity component system
- [ ] Level serialization
- [ ] Mesh import (OBJ/FBX)

### ?? Planned (v2.2+)
- [ ] Physics system (AABB, raycasts)
- [ ] Pathfinding (A*, navigation mesh)
- [ ] In-engine editor (ImGui integration)
- [ ] Texture support & material system
- [ ] LOD & frustum culling
- [ ] Server integration protocol

---

## ?? Game Loop

Main Loop (locked to 60 FPS):
  1. Poll input (SDL2)
  2. Update game logic (CPU)
  3. Update AI/physics
  4. Render to backbuffer (GPU)
  5. Present frame

The engine uses **double buffering** with VSync to maintain smooth 60 FPS gameplay.

---

## ?? Server Integration

Bound-Engine connects to a proprietary C++ MMO server via a custom API:

Server ? Updates (entity state, ability results) ? Client
Client ? Commands (movement, actions) ? Server (validated)

See `PROJECT_SCOPE.md` for detailed data flow diagrams.

---

## ?? Key Systems

| System | Location | Purpose |
|--------|----------|---------|
| **GLRenderer** | `Core/Render/` | Main OpenGL rendering pipeline |
| **AssetManager** | `Core/Assets/` | Load/cache meshes, textures, shaders |
| **World** | `Core/Game/` | Entity container and spatial organization |
| **Camera** | `Core/Render/` | View matrix, perspective projection |
| **Shader** | `Core/Render/` | GLSL compilation, uniform management |
| **SDLWindow** | `Platform/` | Cross-platform window & input |

---

## ?? Testing

The engine uses a **modular test framework** that loads test assets and configurations from files, not hardcoded:

Tests/
??? RenderTests/
?   ??? testmanifest.json
?   ??? test_assets/
?   ??? expected_outputs/
??? AssetTests/
??? SimulationTests/
??? IntegrationTests/

Each test suite is independent and configurable. See **TESTING_FRAMEWORK.md** for complete design.

---

## ??? Development Guidelines

- **Code Style**: See `.editorconfig` and `CONTRIBUTING.md`
- **Namespaces**: `Bound::` prefix for all engine code
- **Memory**: Prefer `std::unique_ptr` for RAII
- **Threading**: CPU/GPU work should be decoupled (no blocking GPU calls from CPU)

---

## ?? Documentation

- **[PROJECT_SCOPE.md](./PROJECT_SCOPE.md)** - Full architecture, design goals, roadmap
- **[TESTING_FRAMEWORK.md](./TESTING_FRAMEWORK.md)** - Test system implementation guide
- **[EDITOR.md](./EDITOR.md)** - Editor features and usage
- **[CONTRIBUTING.md](./CONTRIBUTING.md)** - Code standards and contribution process

---

## ?? License

Proprietary - Bound-Engine is custom-built for MMO development.

---

## ?? Team

Built with a **lean and resourceful philosophy** by a small, focused team.

For questions or contributions, see **CONTRIBUTING.md**.

---

**Last Updated**: February 15, 2026  
**Version**: v2.1 (SDL2 + OpenGL)  
**Target**: Proprietary MMO Server Integration

---

Good luck building the engine! ??

