# Bound Engine - Foundation Architecture

A lightweight, CPU-focused 3D game engine emphasizing efficiency and compatibility.

## Project Structure

```
Bound-Engine/
??? Core/
?   ??? Math/
?   ?   ??? Vector.h          # Vec2, Vec3, Vec4, Mat4 math types
?   ??? Render/
?   ?   ??? Framebuffer.h/.cpp    # Pixel buffer and rendering target
?   ?   ??? Camera.h/.cpp         # 3D camera with view/projection matrices
?   ?   ??? Renderer.h/.cpp       # Main rendering system
?   ??? Application.h/.cpp        # Main engine application class
??? Platform/
?   ??? Window.h/.cpp             # Windows windowing system
??? Main.h/.cpp                   # Game entry point (example)
??? [project files]
```

## Current Status

? **Completed:**
- Core math library (Vec3, Mat4, basic transforms)
- Framebuffer (2D pixel buffer with depth testing)
- Camera system (FPS-style with yaw/pitch)
- Basic renderer (mesh rendering, world-to-screen transform)
- Windows platform integration
- Main application loop with input handling
- Test pyramid mesh

## Building & Running

The project compiles with C++14 on Windows. The test application creates a 1920x1080 window and renders a colored pyramid.

**Controls:**
- **WASD** - Camera movement (forward/back/strafe)
- **Space/Ctrl** - Camera up/down
- **Right Mouse Button** - Look around (mouse look)
- **ESC** - Quit

## Next Steps

### Immediate Priorities:
1. **Triangle Rasterization** - Implement proper filled triangle rendering with barycentric interpolation
2. **Texture Support** - Add texture mapping and UV coordinates to meshes
3. **Mesh Loading** - Import OBJ/FBX files for game content
4. **Scene Manager** - Organize objects in a scene graph with culling
5. **Input Manager** - Abstract input for cross-platform support

### Medium-term:
1. **Lighting System** - Implement Gouraud shading with point lights
2. **LOD System** - Distance-based geometry simplification
3. **Fog Effects** - Distance-based dithering and chromatic aberration
4. **Asset Manager** - Load/cache meshes and textures

### Platform Support:
- [ ] Windows (Current - basic implementation)
- [ ] Linux (Unix/X11 windowing)
- [ ] macOS (Cocoa windowing)

### Editor Integration:
- [ ] Dear ImGui integration
- [ ] Scene hierarchy panel
- [ ] Property inspector
- [ ] Viewport rendering

## Architecture Notes

**Bound::Application** - Base class for creating games. Override `onInit()`, `onUpdate()`, `onRender()`, and `onShutdown()` to implement game logic.

**Bound::Framebuffer** - Represents the 2D target surface. Currently 32-bit ARGB, supports depth buffer for Z-ordering.

**Bound::Renderer** - Takes 3D scene data and projects it to the framebuffer. Currently renders triangle outlines.

**Bound::Camera** - FPS-style camera with perspective projection. Handles view matrix generation and camera movement.

**Math Types** - Minimal custom math library optimized for game use. Can be extended or replaced with GLM later.

## Notes for Future Development

- The engine uses Windows-specific windowing currently. Abstract to platform-agnostic interface before Linux/Mac support.
- Triangle rasterization is stubbed but needs proper edge-walking and barycentric interpolation.
- Fog effects and post-processing pipeline are outlined but not implemented.
- Depth buffer uses 16-bit values; consider 32-bit float for better precision in complex scenes.
- Lighting and shading are core to the "pseudo-3D" aesthetic - Gouraud shading with aggressive LOD is the target.

## File Manifest

| File | Purpose |
|------|---------|
| Core/Math/Vector.h | Vec2, Vec3, Vec4, Mat4 with operators and transforms |
| Core/Render/Framebuffer.h/.cpp | Pixel buffer, depth test, drawing primitives |
| Core/Render/Camera.h/.cpp | 3D camera with view/projection matrices |
| Core/Render/Renderer.h/.cpp | Main rendering pipeline |
| Core/Application.h/.cpp | Engine main loop and game base class |
| Platform/Window.h/.cpp | Windows windowing and input |
| Main.h/.cpp | Game example with test pyramid mesh |

---

Good luck building the engine! ??

