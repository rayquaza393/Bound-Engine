#pragma once

#include "Level.h"

namespace Bound {

class GLRenderer;

class World {
public:
    World() = default;
    ~World() = default;
    
    bool loadLevel(const std::string& filename);
    void render(GLRenderer* renderer);
    
    Level* getLevel() { return &level_; }
    bool isLevelLoaded() const { return !level_.chunks.empty(); }
    
private:
    Level level_;
};

}
