# Bound Engine - Quick Start Guide

## Creating a Game

The simplest game looks like this:

```cpp
#include "Core/Application.h"

class MyGame : public Bound::Application {
public:
    MyGame() : Bound::Application("My Game", 1920, 1080) {}
    
    void onInit() override {
        // Called once at startup
        // Load meshes, initialize game state
        auto camera = getRenderer()->getCamera();
        camera->setPosition(Bound::Vec3(0, 0, 5));
    }
    
    void onUpdate(float deltaTime) override {
        // Called every frame
        // Update game logic, move objects, etc.
    }
    
    void onRender() override {
        // Called every frame after update
        // Draw meshes, render scene
    }
    
    void onShutdown() override {
        // Called on exit
        // Cleanup, save data, etc.
    }
};

int main() {
    MyGame game;
    game.run();
    return 0;
}
```

## Working with Meshes

```cpp
Bound::Mesh mesh;

// Add vertices
mesh.vertices.push_back(Bound::Vertex{
    Bound::Vec3(0, 1, 0),      // position
    Bound::Vec3(1, 0, 0),      // color (RGB)
    Bound::Vec3(0, 1, 0)       // normal
});

// Add indices (3 per triangle)
mesh.indices.push_back(0);
mesh.indices.push_back(1);
mesh.indices.push_back(2);

// Render the mesh
getRenderer()->drawMesh(mesh, Bound::Mat4::identity());
```

## Camera Control

```cpp
auto camera = getRenderer()->getCamera();

// Position
camera->setPosition(Bound::Vec3(0, 0, 5));
camera->move(Bound::Vec3(1, 0, 0)); // Relative move

// Rotation (yaw, pitch in radians)
camera->rotate(0.1f, 0.05f);

// Properties
camera->setFOV(Bound::radians(45.0f));
camera->setAspect(16.0f / 9.0f);
camera->setNearPlane(0.1f);
camera->setFarPlane(1000.0f);

// Direction vectors
Bound::Vec3 forward = camera->getForward();
Bound::Vec3 right = camera->getRight();
Bound::Vec3 up = camera->getUp();
```

## Framebuffer Operations

```cpp
auto fb = getFramebuffer();

// Clear
fb->clear(Bound::Framebuffer::makeColor(50, 50, 50)); // Dark gray

// Set pixel
Bound::Framebuffer::Color red = Bound::Framebuffer::makeColor(255, 0, 0);
fb->setPixel(100, 100, red);

// Draw shapes
fb->drawLine(0, 0, 100, 100, red);
fb->drawRect(50, 50, 200, 150, Bound::Framebuffer::makeColor(0, 255, 0), true);

// Depth-aware pixel
uint16_t depth = 100;
fb->setPixelWithDepth(100, 100, red, depth);
```

## Math Utilities

```cpp
// Vector operations
Bound::Vec3 a(1, 2, 3);
Bound::Vec3 b(4, 5, 6);
Bound::Vec3 c = a + b;
Bound::Vec3 d = a * 2.0f;
float dist = a.distance(b);
Bound::Vec3 cross = a.cross(b);
float dot = a.dot(b);

// Matrix operations
Bound::Mat4 translation = Bound::Mat4::translation(1, 2, 3);
Bound::Mat4 rotation = Bound::Mat4::rotationY(Bound::radians(90.0f));
Bound::Mat4 scale = Bound::Mat4::scale(2, 2, 2);
Bound::Mat4 combined = translation * rotation * scale;

// Utilities
float angle = Bound::radians(45.0f);  // degrees to radians
float deg = Bound::degrees(angle);     // radians to degrees
float clamped = Bound::clamp(value, 0.0f, 1.0f);
float interpolated = Bound::lerp(start, end, 0.5f);
```

## Input Handling

```cpp
// In onUpdate() or onRender()

// Keyboard (Windows VK codes)
if (getWindow()->isKeyPressed('W')) {
    // Handle W key
}

if (getWindow()->isKeyPressed(VK_SPACE)) {
    // Handle space
}

// Mouse
if (getWindow()->isMouseButtonPressed(0)) { // Left button
    int x, y;
    getWindow()->getMousePosition(x, y);
    // Handle left click at (x, y)
}

if (getWindow()->isMouseButtonPressed(1)) { // Right button
    // Handle right click
}
```

## Color Utilities

```cpp
// Create color
auto color = Bound::Framebuffer::makeColor(255, 128, 64, 255); // RGBA

// Unpack color
uint8_t r, g, b, a;
Bound::Framebuffer::unpackColor(color, r, g, b, a);

// Common colors
Bound::Framebuffer::makeColor(255, 0, 0);     // Red
Bound::Framebuffer::makeColor(0, 255, 0);     // Green
Bound::Framebuffer::makeColor(0, 0, 255);     // Blue
Bound::Framebuffer::makeColor(255, 255, 255); // White
Bound::Framebuffer::makeColor(0, 0, 0);       // Black
```

## Common Patterns

### Rotating Object
```cpp
static float rotation = 0;
rotation += deltaTime;

auto transform = Bound::Mat4::rotationY(rotation);
getRenderer()->drawMesh(mesh, transform);
```

### Moving Camera Forward/Back
```cpp
auto camera = getRenderer()->getCamera();
float speed = 5.0f * deltaTime;

if (getWindow()->isKeyPressed('W')) {
    camera->moveForward(speed);
}
if (getWindow()->isKeyPressed('S')) {
    camera->moveForward(-speed);
}
```

### Depth-Sorted Drawing
```cpp
// Calculate depth in world space
float depth = (uint16_t)((distance / max_distance) * 65535);
getFramebuffer()->setPixelWithDepth(x, y, color, depth);
```

---

For more details, see README.md and the header files in Core/ directory.

