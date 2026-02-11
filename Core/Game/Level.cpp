#include "Level.h"
#include <fstream>
#include <cstring>
#include <algorithm>

namespace Bound {

    bool Level::save(const std::string& filename) const {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) return false;
        
        // Magic + Version
        char magic[4] = {'L', 'E', 'V', 'L'};
        file.write(magic, 4);
        
        uint32_t version = 1;
        file.write(reinterpret_cast<const char*>(&version), sizeof(version));
        
        // Level name
        char nameBuf[256] = {0};
        strncpy_s(nameBuf, sizeof(nameBuf), name.c_str(), 255);
        file.write(nameBuf, 256);
        
        // Num chunks
        uint32_t numChunks = chunks.size();
        file.write(reinterpret_cast<const char*>(&numChunks), sizeof(numChunks));
        
        // Each chunk
        for (const auto& chunk : chunks) {
            // Chunk name
            strncpy_s(nameBuf, sizeof(nameBuf), chunk.name.c_str(), 255);
            file.write(nameBuf, 256);
            
            // Vertices
            uint32_t numVertices = chunk.vertices.size();
            file.write(reinterpret_cast<const char*>(&numVertices), sizeof(numVertices));
            file.write(reinterpret_cast<const char*>(chunk.vertices.data()), 
                      numVertices * sizeof(Vertex));
            
            // Indices
            uint32_t numIndices = chunk.indices.size();
            file.write(reinterpret_cast<const char*>(&numIndices), sizeof(numIndices));
            file.write(reinterpret_cast<const char*>(chunk.indices.data()), 
                      numIndices * sizeof(uint32_t));
        }
        
        return file.good();
    }

    bool Level::load(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) return false;
        
        char magic[4];
        file.read(magic, 4);
        if (std::strncmp(magic, "LEVL", 4) != 0) return false;
        
        uint32_t version;
        file.read(reinterpret_cast<char*>(&version), sizeof(version));
        if (version != 1) return false;
        
        // Level name
        char nameBuf[256];
        file.read(nameBuf, 256);
        name = std::string(nameBuf);
        
        // Num chunks
        uint32_t numChunks;
        file.read(reinterpret_cast<char*>(&numChunks), sizeof(numChunks));
        
        chunks.clear();
        chunks.resize(numChunks);
        
        // Each chunk
        for (uint32_t i = 0; i < numChunks; ++i) {
            Chunk& chunk = chunks[i];
            
            // Chunk name
            file.read(nameBuf, 256);
            chunk.name = std::string(nameBuf);
            
            // Vertices
            uint32_t numVertices;
            file.read(reinterpret_cast<char*>(&numVertices), sizeof(numVertices));
            chunk.vertices.resize(numVertices);
            file.read(reinterpret_cast<char*>(chunk.vertices.data()), 
                     numVertices * sizeof(Vertex));
            
            // Indices
            uint32_t numIndices;
            file.read(reinterpret_cast<char*>(&numIndices), sizeof(numIndices));
            chunk.indices.resize(numIndices);
            file.read(reinterpret_cast<char*>(chunk.indices.data()), 
                     numIndices * sizeof(uint32_t));
            
            // Compute bounds
            chunk.computeBounds();
        }
        
        return file.good();
    }

}
