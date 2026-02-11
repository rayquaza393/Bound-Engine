#pragma once

#include "Level.h"

namespace Bound {

class World {
public:
    World() = default;
    ~World() = default;
    
    bool loadLevel(const std::string& filename);
    void render(class Renderer* renderer);
    
    Level* getLevel() { return &level_; }
    bool isLevelLoaded() const { return !level_.chunks.empty(); }
    
private:
    Level level_;
};

}
