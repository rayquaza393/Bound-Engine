# Bound Engine - Project Structure

## Flat Structure (Option A)

All header files and implementation files live together in their respective folders for clarity and ease of maintenance.

```
Bound-Engine/
?
??? Core/
?   ??? Math/
?   ?   ??? Vector.h                 (Math types: Vec2, Vec3, Vec4, Mat4)
?   ?   ??? Geometry.h               (Shared: Vertex, Mesh)
?   ?
?   ??? Render/
?   ?   ??? Renderer.h / Renderer.cpp         (Main rendering pipeline)
?   ?   ??? Camera.h / Camera.cpp             (3D camera system)
?   ?   ??? Framebuffer.h / Framebuffer.cpp   (Pixel buffer + depth)
?   ?
?   ??? Game/
?   ?   ??? World.h / World.cpp              (Level manager)
?   ?   ??? Level.h / Level.cpp              (Level serialization)
?   ?
?   ??? Application.h / Application.cpp       (Engine loop)
?
??? Platform/
?   ??? Window.h / Window.cpp                (Win32 windowing)
?
??? Main.h / Main.cpp                        (Game entry point)
?
??? Assets/
    ??? Levels/
        ??? Level1.data                      (Binary level files)
```

## Include Path Conventions

**From any file, include paths follow the directory structure:**

```cpp
// From Main.cpp (root level):
#include "Core/Application.h"
#include "Core/Game/World.h"
#include "Core/Math/Geometry.h"

// From Core/Render/Renderer.cpp:
#include "Renderer.h"          // Same folder
#include "Camera.h"            // Same folder
#include "../Math/Geometry.h"  // Go up to Core, then into Math
#include "../Application.h"    // Go up to Core

// From Core/Game/World.cpp:
#include "World.h"             // Same folder
#include "Level.h"             // Same folder
#include "../Render/Renderer.h"  // Go up to Core, then into Render
```

## Key Rules

1. ? **Headers and implementations live together** - Easy to find & modify
2. ? **Include guards on all headers** - `#pragma once`
3. ? **Relative paths only** - Use `../` to navigate directories
4. ? **No external libraries** (Phase 1) - Just standard C++ and Windows.h
5. ? **Undefine min/max macros** - Done in Vector.h to avoid Windows conflicts
6. ? **C++14 compatible** - All code must compile with `-std=c++14`

## Namespace Organization

Everything lives in `namespace Bound`:

```cpp
namespace Bound {
    struct Vec3 { ... };
    struct Vertex { ... };
    class Renderer { ... };
    class World { ... };
    struct Level { ... };
}
```

## File Purposes

| File | Purpose |
|------|---------|
| `Vector.h` | Math types (Vec2, Vec3, Vec4, Mat4) |
| `Geometry.h` | Shared rendering structs (Vertex, Mesh) |
| `Renderer.h/cpp` | Triangle rasterization, transform pipeline |
| `Camera.h/cpp` | FPS-style camera with view/projection matrices |
| `Framebuffer.h/cpp` | Pixel buffer + depth buffer management |
| `Application.h/cpp` | Main game loop and subsystem initialization |
| `Window.h/cpp` | Win32 window management and input handling |
| `World.h/cpp` | Level loading and rendering orchestration |
| `Level.h/cpp` | Binary level file format (save/load) |
| `Main.h/cpp` | Game class and entry point |

## Build System

Visual Studio project filters are organized to match directory structure:

- **Filters ? Core ? Math** ? Vector.h, Geometry.h
- **Filters ? Core ? Render** ? All rendering files
- **Filters ? Core ? Game** ? World.h/cpp, Level.h/cpp
- **Filters ? Platform** ? Window.h/cpp
- **Root** ? Main.h/cpp, Application.h/cpp

## Adding New Files

When adding a new file:

1. Create it in the appropriate folder (e.g., `Core/Render/NewSystem.h`)
2. Add `#pragma once` at the top
3. Use relative includes based on your location
4. Add namespace `Bound { ... }`
5. Update .vcxproj filters to match folder structure

Example:

```cpp
// Core/Render/NewSystem.h
#pragma once

#include "Renderer.h"           // Same folder
#include "../Math/Vector.h"     // Up one level, into Math

namespace Bound {

class NewSystem {
    // ...
};

}
```

---

**This structure is now LOCKED IN. All future development follows these conventions.**
