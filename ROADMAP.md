# Bound Engine - Development Roadmap

## Vision
A lightweight, CPU-driven 3D engine that achieves PS2-era graphics quality with minimal hardware requirements through intelligent software rasterization and aggressive LOD systems.

## Phase 1: Foundation (Current - Complete ?)

### ? Complete
- [x] Core math library (Vec3, Mat4, transforms)
- [x] Framebuffer and pixel manipulation
- [x] Camera system with FPS controls
- [x] Basic mesh rendering (wireframe)
- [x] Windows windowing and input
- [x] Main application loop
- [x] Test scene (pyramid)

### In Progress
- [ ] Full triangle rasterization (filled triangles with depth testing)
- [ ] Barycentric coordinate interpolation
- [ ] Backface culling optimization

## Phase 2: Core Rendering (Weeks 2-3)

### Priority 1: Triangle Rendering
- [ ] Implement scanline-based triangle rasterization
- [ ] Perspective-correct texture coordinate interpolation
- [ ] Proper depth buffering (z-test)
- [ ] Anti-aliasing (MSAA or post-process)

### Priority 2: Texture Support
- [ ] Mesh UV coordinate support
- [ ] Texture loading (PNG, TGA via stb_image)
- [ ] Texture atlasing for batch rendering
- [ ] Bilinear filtering

### Priority 3: Lighting
- [ ] Gouraud shading (vertex-based lighting)
- [ ] Point light sources
- [ ] Distance attenuation
- [ ] Ambient/specular components

## Phase 3: Performance & Optimization (Weeks 4-5)

### LOD System
- [ ] Distance-based geometry simplification
- [ ] Billboard system for far objects
- [ ] Sprite impostor system
- [ ] Seamless LOD transitions

### Culling & Visibility
- [ ] Frustum culling
- [ ] Occlusion culling (basic)
- [ ] Portal/cell rendering for indoor scenes
- [ ] Spatial partitioning (Octree/BSP)

### Effects
- [ ] Distance fog (dithering + chromatic aberration)
- [ ] Motion blur for camera movement
- [ ] Depth-of-field hints
- [ ] Bloom/glow effects (post-process)

## Phase 4: Asset Pipeline (Weeks 6-7)

### Mesh Import
- [ ] OBJ file loader
- [ ] FBX support (rigged meshes)
- [ ] Custom binary mesh format
- [ ] Mesh optimization (vertex cache, index optimization)

### Material System
- [ ] Material properties (albedo, normal, roughness)
- [ ] Shader variants for different quality levels
- [ ] Dynamic material swapping

### Asset Manager
- [ ] Centralized asset loading/caching
- [ ] Memory budgeting
- [ ] Async loading
- [ ] Hot reload support (for editor)

## Phase 5: Editor (Weeks 8-9)

### Dear ImGui Integration
- [ ] Scene viewport rendering
- [ ] Game run/pause/stop controls
- [ ] Hierarchy panel (scene tree)
- [ ] Property inspector
- [ ] Asset browser

### Editor Features
- [ ] Scene saving/loading (custom format)
- [ ] Object creation tools (translate/rotate/scale)
- [ ] Prefab system
- [ ] Undo/redo system
- [ ] Performance profiler integration

## Phase 6: Gameplay Systems (Weeks 10-11)

### Physics (Simplified)
- [ ] AABB collision detection
- [ ] Basic character controller
- [ ] Gravity and jump
- [ ] World constraints

### Animation
- [ ] Skeletal animation playback
- [ ] Animation blending
- [ ] IK (inverse kinematics) - optional
- [ ] Timeline/sequencer for cutscenes

### Audio (Placeholder)
- [ ] Basic sound effect playback
- [ ] Music system
- [ ] 3D spatial audio
- [ ] Procedural audio generation

## Phase 7: Platform Support (Weeks 12-13)

### Linux Support
- [ ] X11 windowing
- [ ] Input handling
- [ ] Hardware integration testing

### macOS Support
- [ ] Cocoa windowing
- [ ] Metal graphics backend (future, optional)
- [ ] Input handling

### Web (Future Consideration)
- [ ] Emscripten compilation
- [ ] WebGL target (or keep CPU-driven)
- [ ] Browser optimization

## Phase 8: Polish & Release (Weeks 14+)

### Optimization
- [ ] SIMD math operations (SSE/AVX on x86)
- [ ] Cache-friendly data layouts
- [ ] Memory profiling and optimization
- [ ] Benchmark suite

### Documentation
- [ ] API documentation (Doxygen)
- [ ] Tutorial series
- [ ] Example games
- [ ] Architecture whitepaper

### Distribution
- [ ] Licensing system (CLA/one-time fee)
- [ ] Version control setup
- [ ] CI/CD pipeline (GitHub Actions)
- [ ] Release packages (Windows, Linux, macOS)

## Parallel Tracks

### Testing Framework
- [ ] Unit tests for math library
- [ ] Integration tests for rendering
- [ ] Automated build testing
- [ ] Performance regression testing

### Performance Profiling
- [ ] Frame time analysis
- [ ] Memory usage tracking
- [ ] CPU utilization profiling
- [ ] Triangle budget estimation

### Example Projects
1. **Simple 3D Scene** - Prove basic rendering works
2. **First-Person Explorer** - FPS camera with environment
3. **Isometric RPG** - Fixed camera angle scene
4. **Grid-Based Dungeon Crawler** - Old-school exploration
5. **3D Platformer** - Movement and jumping mechanics

## Architecture Decisions Made

| Area | Decision | Rationale |
|------|----------|-----------|
| Math | Custom lightweight library | Control, no external dependencies |
| Rendering | Software rasterization | CPU control, maximum portability |
| Windowing | Platform-specific (Windows first) | Simplicity, performance, can abstract later |
| Color Format | 32-bit ARGB | Standard, hardware-friendly |
| Depth | 16-bit unsigned short | Good enough for most scenes, low memory |
| Mesh Format | Custom binary + OBJ support | Fast loading, editor-friendly |
| Threading | None initially | Single-threaded simplicity, add later if needed |

## Known Limitations (Current)

- **No dynamic lighting** - Static lights baked into vertex colors
- **Simple geometry LOD** - No skeletal animation or complex deformation
- **No physics** - Collision detection only
- **Limited post-processing** - CPU overhead too high for complex effects
- **Single-threaded** - Rendering, logic all on main thread
- **Windows only** - Currently

## Performance Targets

| Metric | Target | Notes |
|--------|--------|-------|
| Resolution | 1080p | Target gameplay resolution |
| FPS | 60 | Smooth gameplay |
| Triangle Budget | 100k-500k | Depends on scene complexity |
| Memory | <500MB | Including assets |
| Startup Time | <5 seconds | Including level load |
| Frame Time | <16ms | At 60 FPS |

## Success Criteria

- [ ] Render static scenes at 60 FPS on average hardware
- [ ] Import and display complex meshes (10k+ triangles)
- [ ] Implement convincing lighting with Gouraud shading
- [ ] Run on 5+ year old laptops with integrated graphics
- [ ] Complete integrated editor for level design
- [ ] Documented enough for third-party game creation
- [ ] Licensing model in place (CLA + fee)
- [ ] Community enthusiasm and first external projects

---

Last Updated: [Current Sprint]
Next Review: [End of Phase 1]

