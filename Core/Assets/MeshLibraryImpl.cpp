#include "MeshLibrary.h"
#include <glm/glm.hpp>
#include <cmath>

namespace Bound {

	MeshLibrary::MeshLibrary() {
	}

	MeshLibrary::~MeshLibrary() {
	}

	Mesh MeshLibrary::createCube(float size) {
		Mesh mesh;
		float s = size * 0.5f;

		mesh.vertices.push_back(Vertex(glm::vec3(-s, -s, s), glm::vec3(1, 1, 1)));
		mesh.vertices.push_back(Vertex(glm::vec3(s, -s, s), glm::vec3(1, 1, 1)));
		mesh.vertices.push_back(Vertex(glm::vec3(s, s, s), glm::vec3(1, 1, 1)));
		mesh.vertices.push_back(Vertex(glm::vec3(-s, s, s), glm::vec3(1, 1, 1)));

		mesh.vertices.push_back(Vertex(glm::vec3(s, -s, -s), glm::vec3(1, 1, 1)));
		mesh.vertices.push_back(Vertex(glm::vec3(-s, -s, -s), glm::vec3(1, 1, 1)));
		mesh.vertices.push_back(Vertex(glm::vec3(-s, s, -s), glm::vec3(1, 1, 1)));
		mesh.vertices.push_back(Vertex(glm::vec3(s, s, -s), glm::vec3(1, 1, 1)));

		mesh.indices = {
			0, 2, 1, 0, 3, 2,
			4, 6, 5, 4, 7, 6,
			5, 6, 3, 5, 3, 0,
			1, 2, 7, 1, 7, 4,
			3, 6, 7, 3, 7, 2,
			5, 0, 1, 5, 1, 4
		};

		mesh.gpuDirty = true;
		return mesh;
	}

	Mesh MeshLibrary::createPyramid(float size) {
		Mesh mesh;
		float s = size * 0.5f;

		mesh.vertices.push_back(Vertex(glm::vec3(0, s, 0), glm::vec3(1, 1, 1)));
		mesh.vertices.push_back(Vertex(glm::vec3(-s, -s, s), glm::vec3(1, 1, 1)));
		mesh.vertices.push_back(Vertex(glm::vec3(s, -s, s), glm::vec3(1, 1, 1)));
		mesh.vertices.push_back(Vertex(glm::vec3(s, -s, -s), glm::vec3(1, 1, 1)));
		mesh.vertices.push_back(Vertex(glm::vec3(-s, -s, -s), glm::vec3(1, 1, 1)));

		mesh.indices = {
			0, 2, 1,
			0, 3, 2,
			0, 4, 3,
			0, 1, 4
		};

		mesh.gpuDirty = true;
		return mesh;
	}

	Mesh MeshLibrary::createPlane(float width, float height) {
		Mesh mesh;
		float w = width * 0.5f;
		float h = height * 0.5f;

		mesh.vertices.push_back(Vertex(glm::vec3(-w, 0, -h), glm::vec3(1, 1, 1)));
		mesh.vertices.push_back(Vertex(glm::vec3(w, 0, -h), glm::vec3(1, 1, 1)));
		mesh.vertices.push_back(Vertex(glm::vec3(w, 0, h), glm::vec3(1, 1, 1)));
		mesh.vertices.push_back(Vertex(glm::vec3(-w, 0, h), glm::vec3(1, 1, 1)));

		mesh.indices = { 0, 1, 2, 0, 2, 3 };

		mesh.gpuDirty = true;
		return mesh;
	}

	Mesh MeshLibrary::createSphere(float radius, int segments) {
		Mesh mesh;

		for (int i = 0; i <= segments; ++i) {
			float lat = 3.14159f * i / segments;
			for (int j = 0; j <= segments; ++j) {
				float lon = 6.28318f * j / segments;
				float x = radius * sin(lat) * cos(lon);
				float y = radius * cos(lat);
				float z = radius * sin(lat) * sin(lon);
				mesh.vertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec3(1, 1, 1)));
			}
		}

		int vertPerLat = segments + 1;
		for (int i = 0; i < segments; ++i) {
			for (int j = 0; j < segments; ++j) {
				uint32_t a = i * vertPerLat + j;
				uint32_t b = a + 1;
				uint32_t c = a + vertPerLat;
				uint32_t d = c + 1;

				mesh.indices.push_back(a); mesh.indices.push_back(c); mesh.indices.push_back(b);
				mesh.indices.push_back(b); mesh.indices.push_back(c); mesh.indices.push_back(d);
			}
		}

		mesh.gpuDirty = true;
		return mesh;
	}

	void MeshLibrary::colorMesh(Mesh& mesh, const glm::vec3& color) {
		for (auto& vertex : mesh.vertices) {
			vertex.color = color;
		}
	}

}
