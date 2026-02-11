#pragma once

#include "Vector.h"
#include <vector>

namespace Bound {

	// Shared geometry types
	struct Vertex {
		Vec3 position;
		Vec3 color;
		Vec3 normal;

		Vertex() : position(0, 0, 0), color(1, 1, 1), normal(0, 1, 0) {}
		Vertex(const Vec3& pos, const Vec3& col) : position(pos), color(col), normal(0, 1, 0) {}
	};

	struct Mesh {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	};

}
