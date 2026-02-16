#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace Bound {

struct AssetEntry {
    std::string name;
    std::string filepath;
    std::string category;
    std::string type;
    uint64_t fileSize;
};

class AssetDatabase {
public:
    AssetDatabase();
    ~AssetDatabase();

    bool scanDirectory(const std::string& rootPath);
    void scanFolder(const std::string& folderPath, const std::string& category);
    AssetEntry* findMesh(const std::string& name);
    AssetEntry* findLevel(const std::string& name);
    void clear();

private:
    std::vector<AssetEntry> meshes_;
    std::vector<AssetEntry> levels_;
    std::vector<std::string> categories_;
};

}