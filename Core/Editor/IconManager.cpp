#include "IconManager.h"
#include <GL/glew.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../../stb-master/stb_image.h"

namespace Bound {

	IconManager::IconManager() {
	}

	IconManager::~IconManager() {
		unloadAll();
	}

	bool IconManager::loadIcon(const std::string& name, const std::string& filepath) {
		// Check if already loaded
		if (icons_.find(name) != icons_.end()) {
			return true;
		}

		int width, height;
		unsigned int textureID = loadImageAsTexture(filepath, width, height);

		if (textureID == 0) {
			std::cerr << "Failed to load icon: " << filepath << std::endl;
			return false;
		}

		icons_[name] = { textureID, glm::ivec2(width, height) };
		std::cout << "Icon loaded: " << name << " (" << filepath << ") - " << width << "x" << height << std::endl;
		return true;
	}

	unsigned int IconManager::getIconTexture(const std::string& name) const {
		auto it = icons_.find(name);
		if (it != icons_.end()) {
			return it->second.textureID;
		}
		return 0;
	}

	glm::ivec2 IconManager::getIconSize(const std::string& name) const {
		auto it = icons_.find(name);
		if (it != icons_.end()) {
			return it->second.size;
		}
		return glm::ivec2(0, 0);
	}

	bool IconManager::hasIcon(const std::string& name) const {
		return icons_.find(name) != icons_.end();
	}

	void IconManager::unloadIcon(const std::string& name) {
		auto it = icons_.find(name);
		if (it != icons_.end()) {
			glDeleteTextures(1, &it->second.textureID);
			icons_.erase(it);
		}
	}

	void IconManager::unloadAll() {
		for (auto& pair : icons_) {
			glDeleteTextures(1, &pair.second.textureID);
		}
		icons_.clear();
	}

	unsigned int IconManager::loadImageAsTexture(const std::string& filepath, int& outWidth, int& outHeight) {
		// Load image with stb_image
		// Don't flip vertically - ImGui expects standard top-left origin textures
		stbi_set_flip_vertically_on_load(false);
		int channels;
		unsigned char* data = stbi_load(filepath.c_str(), &outWidth, &outHeight, &channels, 4);

		if (!data) {
			std::cerr << "Failed to load image file: " << filepath << std::endl;
			std::cerr << "stbi_failure_reason: " << stbi_failure_reason() << std::endl;
			return 0;
		}

		// Create OpenGL texture
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Upload texture data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, outWidth, outHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}

}
