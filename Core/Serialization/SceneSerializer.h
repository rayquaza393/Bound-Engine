#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../Editor/EditorObject.h"

namespace Bound {

	/**
	 * SceneSerializer - Binary scene format (.scne)
	 * 
	 * Format (little-endian):
	 * - Header: 4 bytes "SCNE"
	 * - Version: 1 byte (0x01)
	 * - Object count: 4 bytes (uint32_t)
	 * - For each object:
	 *   - Object ID: 4 bytes (uint32_t)
	 *   - Type: 1 byte (0=Cube, 1=Pyramid, 2=Floor)
	 *   - Position: 3 * 4 bytes (float x, y, z)
	 *   - Rotation: 3 * 4 bytes (float x, y, z)
	 *   - Scale: 3 * 4 bytes (float x, y, z)
	 *   - Color: 3 * 4 bytes (float r, g, b)
	 *
	 * Total per object: 4 + 1 + 12 + 12 + 12 + 12 = 53 bytes
	 * File size: 9 bytes header + (53 * object_count)
	 */
	class SceneSerializer {
	public:
		// Serialize scene to binary file
		static bool saveScene(const std::string& filepath, 
							  const std::vector<std::unique_ptr<EditorObject>>& objects);

		// Deserialize scene from binary file
		static bool loadScene(const std::string& filepath,
							  std::vector<std::unique_ptr<EditorObject>>& outObjects);

	private:
		static const char MAGIC[4];     // "SCNE"
		static const uint8_t VERSION;   // 0x01
		static const uint32_t HEADER_SIZE;

		// Helper functions
		static void writeFloat(FILE* file, float value);
		static void writeUInt32(FILE* file, uint32_t value);
		static void writeUInt8(FILE* file, uint8_t value);

		static float readFloat(FILE* file);
		static uint32_t readUInt32(FILE* file);
		static uint8_t readUInt8(FILE* file);
	};

}
