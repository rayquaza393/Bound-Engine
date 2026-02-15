#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "../Render/Mesh.h"
#include "MeshLibrary.h"

namespace Bound {

	/**
	 * AssetManager - Centralized asset loading from disk
	 * 
	 * Loads level files and creates meshes from disk or primitives
	 * Keeps assets modular - nothing hardcoded in Main.cpp
	 */
	class AssetManager {
	public:
		AssetManager(const std::string& assetRoot);
		~AssetManager();

		// Load level from file
		bool loadLevel(const std::string& levelName);
		bool saveLevel(const std::string& levelName);

		// Get mesh by primitive name or loaded path
		Mesh getMesh(const std::string& meshName);

		// Access to library
		MeshLibrary& getMeshLibrary() { return meshLibrary_; }
		const std::string& getAssetRoot() const { return assetRoot_; }

	private:
		std::string assetRoot_;
		MeshLibrary meshLibrary_;
		std::unordered_map<std::string, Mesh> meshCache_;
	};

}
