#include "EditorObject.h"
#include <cmath>

namespace Bound {

	void EditorObject::generateMesh() {
		mesh.vertices.clear();
		mesh.indices.clear();

		glm::vec3 glmColor(color.x, color.y, color.z);

		switch (type) {
			case ObjectType::Cube:
				// Simple cube: 8 vertices
				mesh.vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glmColor));

				mesh.indices = {
					0, 2, 1, 0, 3, 2,
					4, 6, 5, 4, 7, 6,
					5, 6, 3, 5, 3, 0,
					1, 2, 7, 1, 7, 4,
					3, 6, 7, 3, 7, 2,
					5, 0, 1, 5, 1, 4
				};
				break;

			case ObjectType::Pyramid:
				mesh.vertices.push_back(Vertex(glm::vec3(0.0f, 0.5f, 0.0f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glmColor));

				mesh.indices = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1 };
				break;

			case ObjectType::Floor:
				mesh.vertices.push_back(Vertex(glm::vec3(-5.0f, 0.0f, -5.0f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(5.0f, 0.0f, -5.0f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(5.0f, 0.0f, 5.0f), glmColor));
				mesh.vertices.push_back(Vertex(glm::vec3(-5.0f, 0.0f, 5.0f), glmColor));

				mesh.indices = { 0, 1, 2, 0, 2, 3 };
				break;
		}

		mesh.gpuDirty = true;
	}

}
