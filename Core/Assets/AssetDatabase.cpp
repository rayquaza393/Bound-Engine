#include "AssetDatabase.h"
#include <string>
#include <Windows.h>
#include <cstdio>
#include <algorithm>

namespace Bound {

	AssetDatabase::AssetDatabase() {
	}

	AssetDatabase::~AssetDatabase() {
		clear();
	}

	bool AssetDatabase::scanDirectory(const std::string& rootPath) {
		clear();

		// Check if root exists
		DWORD attribs = GetFileAttributesA(rootPath.c_str());
		if (attribs == INVALID_FILE_ATTRIBUTES || !(attribs & FILE_ATTRIBUTE_DIRECTORY)) {
			printf("Asset root directory not found: %s\n", rootPath.c_str());
			return false;
		}

		printf("Scanning asset directory: %s\n", rootPath.c_str());

		// Scan main folders: Meshes, Levels, etc.
		std::string meshPath = rootPath + "Meshes/";
		std::string levelPath = rootPath + "Levels/";

		scanFolder(meshPath, "Meshes");
		scanFolder(levelPath, "Levels");

		printf("Asset scan complete: %zu meshes, %zu levels\n", meshes_.size(), levels_.size());
		return true;
	}

	void AssetDatabase::scanFolder(const std::string& folderPath, const std::string& category) {
		WIN32_FIND_DATAA findData;
		HANDLE findHandle = FindFirstFileA((folderPath + "*").c_str(), &findData);

		if (findHandle == INVALID_HANDLE_VALUE) {
			printf("Could not scan folder: %s\n", folderPath.c_str());
			return;
		}

		// Track categories
		if (std::find(categories_.begin(), categories_.end(), category) == categories_.end()) {
			categories_.push_back(category);
		}

		do {
			std::string filename = findData.cFileName;

			// Skip . and ..
			if (filename == "." || filename == "..") continue;

			std::string fullPath = folderPath + filename;

			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				// Recurse into subdirectories
				scanFolder(fullPath + "/", category + "/" + filename);
			} else {
				// Check file extension
				size_t dotPos = filename.find_last_of('.');
				if (dotPos == std::string::npos) continue;

				std::string ext = filename.substr(dotPos + 1);
				std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

				AssetEntry entry;
				entry.filepath = fullPath;
				entry.fileSize = ((uint64_t)findData.nFileSizeHigh << 32) | findData.nFileSizeLow;
				entry.category = category;

				// Remove extension from display name
				entry.name = filename.substr(0, dotPos);

				if (ext == "mesh" || ext == "obj" || ext == "fbx") {
					entry.type = "mesh";
					meshes_.push_back(entry);
					printf("  Found mesh: %s\n", entry.name.c_str());
				} else if (ext == "level" || ext == "lvl") {
					entry.type = "level";
					levels_.push_back(entry);
					printf("  Found level: %s\n", entry.name.c_str());
				}
			}
		} while (FindNextFileA(findHandle, &findData));

		FindClose(findHandle);
	}

	AssetEntry* AssetDatabase::findMesh(const std::string& name) {
		for (auto& mesh : meshes_) {
			if (mesh.name == name) return &mesh;
		}
		return nullptr;
	}

	AssetEntry* AssetDatabase::findLevel(const std::string& name) {
		for (auto& level : levels_) {
			if (level.name == name) return &level;
		}
		return nullptr;
	}

	void AssetDatabase::clear() {
		meshes_.clear();
		levels_.clear();
		categories_.clear();
	}

}
