# Setting Up External Dependencies

This guide walks you through installing the libraries we discussed for Phase 2+ development.

## Dependencies Overview

| Library | Purpose | Phase | Optional |
|---------|---------|-------|----------|
| **GLM** | Math library (replace our custom math) | 2+ | ? Later |
| **SDL2** | Cross-platform windowing | 2+ | ? For X-platform |
| **stb_image.h** | Image loading (PNG, JPG) | 2 | ? Embedded |
| **Assimp** | 3D mesh loading (OBJ, FBX) | 2 | ? Phase 2 |
| **Dear ImGui** | UI framework (for editor) | 4 | ? Phase 4 |

## Phase 1: Current State (NO EXTERNAL LIBS)

? We're using:
- Standard C++ library only
- Windows.h for windowing (built-in)
- Custom math library

This is intentional for compatibility!

---

## Phase 2: When You Need GLM + Assimp

### Setting Up GLM (Header-Only Math Library)

GLM is a **header-only library**, so installation is simple:

#### Option 1: NuGet (Easiest)
```
Tools ? NuGet Package Manager ? Manage NuGet Packages for Solution
Search: "glm"
Install: glm by G-Truc Creation
```

#### Option 2: Manual
1. Download from: https://github.com/g-truc/glm/releases
2. Extract to a folder (e.g., `C:\Libraries\glm`)
3. In Visual Studio:
   - Project ? Properties
   - C/C++ ? General
   - Add to "Additional Include Directories": `C:\Libraries\glm`

#### Usage in Code:
```cpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 position(0, 0, 5);
glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
```

---

### Setting Up Assimp (3D Model Loading)

Assimp lets you load OBJ, FBX, and other 3D formats.

#### Option 1: NuGet (Recommended)
```
Tools ? NuGet Package Manager ? Manage NuGet Packages for Solution
Search: "assimp"
Install: Assimp by Kim Kulling (or AssimpNet)
```

#### Option 2: vcpkg (Package Manager)
```bash
# Install vcpkg if you don't have it
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./vcpkg integrate install

# Install assimp
./vcpkg install assimp:x64-windows
```

#### Option 3: Manual Build
1. Download: https://github.com/assimp/assimp/releases
2. Extract and open CMakeLists.txt
3. Build with Visual Studio (follow their README)
4. Copy built DLLs and libs to your project

#### Usage in Code:
```cpp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Assimp::Importer importer;
const aiScene* scene = importer.ReadFile("model.obj", aiProcess_Triangulate);
```

---

## Phase 2: When You Need SDL2 (Cross-Platform)

SDL2 handles windowing on Windows/Linux/Mac.

### Option 1: NuGet (Easiest)
```
Search: "sdl2"
Install: SDL2 by libsdl (or compatible version)
```

### Option 2: Manual
1. Download: https://www.libsdl.org/download-2.0.php
2. Extract to `C:\Libraries\SDL2`
3. In Visual Studio Properties:
   - Linker ? Input ? Add: `SDL2.lib`
   - C/C++ ? General ? Add include path
   - Copy SDL2.dll to output folder

#### Usage (replacing Win32):
```cpp
#include <SDL2/SDL.h>

SDL_Window* window = SDL_CreateWindow(
    "Bound Engine",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    1920, 1080,
    SDL_WINDOW_SHOWN
);
```

---

## Phase 4: When You Need Dear ImGui (Editor)

ImGui provides the UI for level editing.

### Installation:
1. Download: https://github.com/ocornut/imgui
2. Add `imgui.h`, `imgui.cpp`, etc. to your project
3. Link with your renderer (SDL2 or DirectX)

#### Usage:
```cpp
#include "imgui.h"

ImGui::Begin("Scene Editor");
ImGui::Text("Hello from ImGui!");
if (ImGui::Button("Load Mesh")) {
    // Load mesh code
}
ImGui::End();
```

---

## For Phase 1 ? Phase 2 Transition

When you're ready for Phase 2:

1. **Keep current code** - Our custom math and Win32 still works
2. **Add GLM optionally** - Can run in parallel with our math
3. **Add Assimp** - For loading OBJ/FBX files
4. **Decide on SDL2** - Only needed if going cross-platform

**Example Phase 2 setup:**
```cpp
// Can use both!
#include "Core/Math/Vector.h"  // Our lightweight math
#include <glm/glm.hpp>         // GLM for advanced features
#include <assimp/Importer.hpp> // Mesh loading
```

---

## Quick Reference: Installing a NuGet Package

1. Right-click Solution in Solution Explorer
2. Select "Manage NuGet Packages for Solution"
3. Click "Browse" tab
4. Search for the package name
5. Select it, click "Install"
6. Accept any dialogs
7. Rebuild project

---

## Troubleshooting

### "Cannot find include file"
- Check Project Properties ? C/C++ ? General ? Additional Include Directories
- Ensure path exists and is correct

### "LNK1104: cannot open file 'xxx.lib'"
- Check Project Properties ? Linker ? Input ? Additional Dependencies
- Verify .lib file exists in library path

### DLL not found at runtime
- Copy .DLL files to same folder as .EXE
- Or add library path to system PATH environment variable

---

## For Now (Phase 1)

**You don't need to install anything yet!** We're using pure Win32 + standard C++.

When Phase 2 begins and you need:
- **Mesh loading** ? Install Assimp
- **Better math** ? Install GLM
- **Cross-platform** ? Install SDL2
- **Editor UI** ? Install Dear ImGui

---

## Next Steps

1. Finish Phase 1 (current state - building the foundation)
2. When ready for Phase 2, come back to this guide
3. Install dependencies as needed
4. Gradually migrate from custom code to libraries

**Keep the engine lean during Phase 1. Add dependencies strategically in Phase 2+.**

