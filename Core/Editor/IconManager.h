#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Bound {

	/**
	 * IconManager - Loads and caches icon textures for the editor UI
	 * Handles PNG/image loading via stb_image
	 */
	class IconManager {
	public:
		IconManager();
		~IconManager();

		// Load an icon texture from file
		bool loadIcon(const std::string& name, const std::string& filepath);

		// Get a loaded icon texture ID (for ImGui::Image)
		unsigned int getIconTexture(const std::string& name) const;

		// Get icon dimensions
		glm::ivec2 getIconSize(const std::string& name) const;

		// Check if icon is loaded
		bool hasIcon(const std::string& name) const;

		// Unload a specific icon
		void unloadIcon(const std::string& name);

		// Unload all icons
		void unloadAll();

	private:
		struct IconData {
			unsigned int textureID;
			glm::ivec2 size;
		};

		std::unordered_map<std::string, IconData> icons_;

		// Helper to load image data
		unsigned int loadImageAsTexture(const std::string& filepath, int& outWidth, int& outHeight);
	};

}
