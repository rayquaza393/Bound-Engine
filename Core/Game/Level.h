#pragma once

#include "../Math/Vector.h"
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

#undef min
#undef max

namespace Bound {

// Level-specific vertex for binary serialization (uses custom Vec3)
struct LevelVertex {
	Vec3 position;
	Vec3 color;
	Vec3 normal;

	LevelVertex() : position(0, 0, 0), color(1, 1, 1), normal(0, 1, 0) {}
	LevelVertex(const Vec3& pos, const Vec3& col) : position(pos), color(col), normal(0, 1, 0) {}
};

struct BoundingBox {
Vec3 min;
Vec3 max;
    
BoundingBox() = default;
BoundingBox(const Vec3& minPoint, const Vec3& maxPoint) 
{ 
    min = minPoint;
    max = maxPoint;
}
    
    bool isValid() const { 
        return min.x <= max.x && min.y <= max.y && min.z <= max.z; 
    }
};

struct Chunk {
    std::vector<LevelVertex> vertices;
    std::vector<uint32_t> indices;
    BoundingBox bounds;
    std::string name;
    
    Chunk() = default;
    
    void computeBounds() {
        if (vertices.empty()) {
            bounds = BoundingBox();
            return;
        }
        
        Vec3 minPos = vertices[0].position;
        Vec3 maxPos = vertices[0].position;
        
        for (const auto& v : vertices) {
            minPos.x = std::min(minPos.x, v.position.x);
            minPos.y = std::min(minPos.y, v.position.y);
            minPos.z = std::min(minPos.z, v.position.z);
            
            maxPos.x = std::max(maxPos.x, v.position.x);
            maxPos.y = std::max(maxPos.y, v.position.y);
            maxPos.z = std::max(maxPos.z, v.position.z);
        }
        
        bounds = BoundingBox(minPos, maxPos);
    }
};

struct Level {
    std::string name;
    std::vector<Chunk> chunks;
    
    Level() = default;
    Level(const std::string& _name) : name(_name) {}
    
    bool save(const std::string& filename) const;
    bool load(const std::string& filename);
};

}
