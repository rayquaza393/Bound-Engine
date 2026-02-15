#include "AssetManager.h"
#include "../Serialization/LevelFormat.h"
#include <cstdio>
#include <windows.h>

namespace Bound {

	AssetManager::AssetManager(const std::string& assetRoot)
		: assetRoot_(assetRoot) {
		printf("AssetManager initialized with root: %s\n", assetRoot_.c_str());

		// Create directories if they don't exist
		CreateDirectoryA((assetRoot_ + "Meshes/").c_str(), nullptr);
		CreateDirectoryA((assetRoot_ + "Levels/").c_str(), nullptr);
	}

	AssetManager::~AssetManager() {
	}

	bool AssetManager::loadLevel(const std::string& levelName) {
		std::string filepath = assetRoot_ + "Levels/" + levelName + ".level";
		printf("Loading level: %s\n", filepath.c_str());

		LevelDefinition level;
		if (!LevelSerializer::load(filepath, level)) {
			printf("Failed to load level: %s\n", levelName.c_str());
			return false;
		}

		printf("Successfully loaded level: %s with %zu entities\n", 
			level.name.c_str(), level.entities.size());
		return true;
	}

	bool AssetManager::saveLevel(const std::string& levelName) {
		std::string filepath = assetRoot_ + "Levels/" + levelName + ".level";
		printf("Saving level: %s\n", filepath.c_str());

		LevelDefinition level = LevelSerializer::createDefaultLevel();
		if (!LevelSerializer::save(filepath, level)) {
			printf("Failed to save level: %s\n", levelName.c_str());
			return false;
		}

		printf("Successfully saved level: %s\n", levelName.c_str());
		return true;
	}

	Mesh AssetManager::getMesh(const std::string& meshName) {
		// Check if cached
		auto it = meshCache_.find(meshName);
		if (it != meshCache_.end()) {
			return it->second;
		}

		// Create primitive or load from file
		Mesh mesh;
		if (meshName == "cube") {
			mesh = meshLibrary_.createCube();
		} else if (meshName == "pyramid") {
			mesh = meshLibrary_.createPyramid();
		} else if (meshName == "plane") {
			mesh = meshLibrary_.createPlane();
		} else if (meshName == "sphere") {
			mesh = meshLibrary_.createSphere();
		} else {
			printf("Unknown mesh: %s\n", meshName.c_str());
			return mesh;
		}

		meshCache_[meshName] = mesh;
		return mesh;
	}

}
