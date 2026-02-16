# ImGui Integration - Complete Setup Guide

## Folder Structure

```
C:\Projects\Bound-Engine\
??? Bound-Engine\                    ? Your Visual Studio project
?   ??? Bound-Engine.sln
?   ??? Bound-Engine.vcxproj
?   ??? ... (your source files)
?
??? imgui\                           ? NEW: Copy ImGui master here
    ??? imgui.h
    ??? imgui.cpp
    ??? imgui_demo.cpp
    ??? imgui_draw.cpp
    ??? imgui_widgets.cpp
    ??? imgui_tables.cpp
    ??? imgui_internal.h
    ??? imconfig.h
    ??? imstb_*.h
    ??? backends\
    ?   ??? imgui_impl_opengl3.h
    ?   ??? imgui_impl_opengl3.cpp
    ?   ??? imgui_impl_win32.h
    ?   ??? imgui_impl_win32.cpp
    ??? ... (other files)
```

## Setup Steps (5 minutes)

### Step 1: Copy ImGui Files

Copy your ImGui master folder contents to `C:\Projects\Bound-Engine\imgui\`

**You need:**
- All `.h` files from ImGui root
- All `.cpp` files: imgui.cpp, imgui_draw.cpp, imgui_widgets.cpp, imgui_tables.cpp
- `backends/imgui_impl_opengl3.h` and `.cpp`
- `backends/imgui_impl_win32.h` and `.cpp`

### Step 2: Add Include Path in Visual Studio

1. Right-click `Bound-Engine` project ? `Properties`
2. Go to: `Configuration Properties ? C/C++ ? General`
3. Find: `Additional Include Directories`
4. Add:
   ```
   ../imgui
   ../imgui/backends
   ```
5. Click OK

### Step 3: Add ImGui Source Files to Project

1. Right-click project ? `Add ? Existing Item...`
2. Navigate to `C:\Projects\Bound-Engine\imgui\`
3. Select and add:
   - imgui.cpp
   - imgui_draw.cpp
   - imgui_widgets.cpp
   - imgui_tables.cpp
   - imgui_demo.cpp (optional)

4. Add from `backends/`:
   - imgui_impl_opengl3.cpp
   - imgui_impl_win32.cpp

### Step 4: Build

```
Ctrl+Shift+B
```

If you get errors, verify:
- ? Include paths added correctly
- ? All .cpp files added to project
- ? Configuration (Debug vs Release) matches

### Step 5: Run

```
F5
```

You should see:
- Console: "ImGuiManager initialized"
- Empty viewport (3D objects can be created)
- ImGui ready to use

## Quick Test

Add this to `Core/Editor/Editor.cpp` render function:

```cpp
// Simple ImGui window
ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiCond_FirstUseEver);
if (ImGui::Begin("Test Window")) {
    ImGui::Text("ImGui is working!");
    ImGui::SliderFloat("Value", &testValue, 0.0f, 1.0f);
    ImGui::End();
}
```

If you see a window appear, ImGui is integrated! ?

## Next Steps

1. **Build the menu bar** (File, Edit, View, Help)
2. **Create hierarchy panel** (scene tree)
3. **Add properties panel** (transform editor)
4. **Implement gizmos** (move/rotate/scale)

---

**You're ready to integrate ImGui!** ??
