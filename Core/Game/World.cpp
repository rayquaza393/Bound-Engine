#include "World.h"
#include "../Render/GLRenderer.h"
#include "../Render/Mesh.h"
#include "../Math/Vector.h"
#include <cstdio>
#include <windows.h>

namespace Bound {

    bool World::loadLevel(const std::string& filename) {
        char buffer[256];
        sprintf_s(buffer, sizeof(buffer), "Loading level: %s\n", filename.c_str());
        OutputDebugStringA(buffer);
        
        if (!level_.load(filename)) {
            sprintf_s(buffer, sizeof(buffer), "Failed to load level: %s\n", filename.c_str());
            OutputDebugStringA(buffer);
            return false;
        }
        
        sprintf_s(buffer, sizeof(buffer), "Loaded level '%s' with %zu chunks\n", 
                 level_.name.c_str(), level_.chunks.size());
        OutputDebugStringA(buffer);
        
        return true;
    }

    void World::render(GLRenderer* renderer) {
        // Frustum cull and render each chunk
        for (const auto& chunk : level_.chunks) {
            // TODO: Implement frustum culling on chunk bounds
            // For now, just render everything
            
            // Create a mesh from the chunk - note: would need proper vertex conversion
            // This is a stub for now since Level uses old Vertex format
            // Mesh mesh;
            // renderer->drawMesh(mesh, glm::mat4(1.0f));
        }
    }

}
