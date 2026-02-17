#include "SceneSerializer.h"
#include <cstdio>
#include <cstring>

namespace Bound {

	const char SceneSerializer::MAGIC[4] = { 'S', 'C', 'N', 'E' };
	const uint8_t SceneSerializer::VERSION = 0x01;
	const uint32_t SceneSerializer::HEADER_SIZE = 9;  // 4 (magic) + 1 (version) + 4 (count)

	void SceneSerializer::writeFloat(FILE* file, float value) {
		fwrite(&value, sizeof(float), 1, file);
	}

	void SceneSerializer::writeUInt32(FILE* file, uint32_t value) {
		fwrite(&value, sizeof(uint32_t), 1, file);
	}

	void SceneSerializer::writeUInt8(FILE* file, uint8_t value) {
		fwrite(&value, sizeof(uint8_t), 1, file);
	}

	float SceneSerializer::readFloat(FILE* file) {
		float value = 0.0f;
		fread(&value, sizeof(float), 1, file);
		return value;
	}

	uint32_t SceneSerializer::readUInt32(FILE* file) {
		uint32_t value = 0;
		fread(&value, sizeof(uint32_t), 1, file);
		return value;
	}

	uint8_t SceneSerializer::readUInt8(FILE* file) {
		uint8_t value = 0;
		fread(&value, sizeof(uint8_t), 1, file);
		return value;
	}

	bool SceneSerializer::saveScene(const std::string& filepath, 
									 const std::vector<std::unique_ptr<EditorObject>>& objects) {
		FILE* file = nullptr;
		fopen_s(&file, filepath.c_str(), "wb");

		if (!file) {
			printf("Error: Could not open file for writing: %s\n", filepath.c_str());
			return false;
		}

		// Write header
		fwrite(MAGIC, sizeof(char), 4, file);
		writeUInt8(file, VERSION);

		// Write object count
		uint32_t objectCount = static_cast<uint32_t>(objects.size());
		writeUInt32(file, objectCount);

		printf("Saving scene with %u objects to %s\n", objectCount, filepath.c_str());

		// Write each object
		for (const auto& obj : objects) {
			writeUInt32(file, obj->id);
			writeUInt8(file, static_cast<uint8_t>(obj->type));

			// Position
			writeFloat(file, obj->position.x);
			writeFloat(file, obj->position.y);
			writeFloat(file, obj->position.z);

			// Rotation
			writeFloat(file, obj->rotation.x);
			writeFloat(file, obj->rotation.y);
			writeFloat(file, obj->rotation.z);

			// Scale
			writeFloat(file, obj->scale.x);
			writeFloat(file, obj->scale.y);
			writeFloat(file, obj->scale.z);

			// Color
			writeFloat(file, obj->color.x);
			writeFloat(file, obj->color.y);
			writeFloat(file, obj->color.z);

			printf("  Saved object %u (type %u): pos(%.2f,%.2f,%.2f) rot(%.2f,%.2f,%.2f) scale(%.2f,%.2f,%.2f) color(%.2f,%.2f,%.2f)\n",
				obj->id, static_cast<uint8_t>(obj->type),
				obj->position.x, obj->position.y, obj->position.z,
				obj->rotation.x, obj->rotation.y, obj->rotation.z,
				obj->scale.x, obj->scale.y, obj->scale.z,
				obj->color.x, obj->color.y, obj->color.z);
		}

		fclose(file);
		printf("Scene saved successfully to %s (%.1f KB)\n", filepath.c_str(), 
			   (HEADER_SIZE + objects.size() * 53) / 1024.0f);

		return true;
	}

	bool SceneSerializer::loadScene(const std::string& filepath,
									 std::vector<std::unique_ptr<EditorObject>>& outObjects) {
		FILE* file = nullptr;
		fopen_s(&file, filepath.c_str(), "rb");

		if (!file) {
			printf("Error: Could not open file for reading: %s\n", filepath.c_str());
			return false;
		}

		// Read and verify header
		char magic[4];
		fread(magic, sizeof(char), 4, file);

		if (std::memcmp(magic, MAGIC, 4) != 0) {
			printf("Error: Invalid file format (magic mismatch)\n");
			fclose(file);
			return false;
		}

		uint8_t version = readUInt8(file);
		if (version != VERSION) {
			printf("Error: Unsupported version (got %u, expected %u)\n", version, VERSION);
			fclose(file);
			return false;
		}

		// Read object count
		uint32_t objectCount = readUInt32(file);
		printf("Loading scene with %u objects from %s\n", objectCount, filepath.c_str());

		// Clear existing objects
		outObjects.clear();

		// Read each object
		for (uint32_t i = 0; i < objectCount; ++i) {
			uint32_t id = readUInt32(file);
			uint8_t typeValue = readUInt8(file);
			ObjectType type = static_cast<ObjectType>(typeValue);

			Vec3 position(readFloat(file), readFloat(file), readFloat(file));
			Vec3 rotation(readFloat(file), readFloat(file), readFloat(file));
			Vec3 scale(readFloat(file), readFloat(file), readFloat(file));
			Vec3 color(readFloat(file), readFloat(file), readFloat(file));

			// Create object
			auto obj = std::make_unique<EditorObject>(static_cast<int>(id), type, position, color);
			obj->rotation = rotation;
			obj->scale = scale;
			obj->generateMesh();

			printf("  Loaded object %u (type %u): pos(%.2f,%.2f,%.2f) rot(%.2f,%.2f,%.2f) scale(%.2f,%.2f,%.2f) color(%.2f,%.2f,%.2f)\n",
				id, typeValue,
				position.x, position.y, position.z,
				rotation.x, rotation.y, rotation.z,
				scale.x, scale.y, scale.z,
				color.x, color.y, color.z);

			outObjects.push_back(std::move(obj));
		}

		fclose(file);
		printf("Scene loaded successfully from %s\n", filepath.c_str());

		return true;
	}

}
