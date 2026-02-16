#pragma once
#include <string>
#include <vector>

namespace Bound {

	struct Vec3Def {
		float x, y, z;

		Vec3Def() : x(0.0f), y(0.0f), z(0.0f) {}
		Vec3Def(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	};

	struct EntityDefinition {
		std::string name;
		std::string meshName;
		Vec3Def position;
		Vec3Def rotation;
		Vec3Def scale;

		EntityDefinition() : scale(1.0f, 1.0f, 1.0f) {}
	};

	struct LevelDefinition {
		std::string name;
		std::vector<EntityDefinition> entities;
	};

	class LevelSerializer {
	public:
		static bool save(const std::string& filename, const LevelDefinition& level);
		static bool load(const std::string& filename, LevelDefinition& level);
		static LevelDefinition createDefaultLevel();
	};

}
