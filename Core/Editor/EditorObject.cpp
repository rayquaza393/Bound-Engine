#include "EditorObject.h"
#include <cmath>

namespace Bound {

	void EditorObject::generateMesh() {
		mesh.vertices.clear();
		mesh.indices.clear();

		switch (type) {
			case ObjectType::Cube:
				// Simple cube: 8 vertices, 12 triangles (6 faces)
				// Front face (z+)
				mesh.vertices.push_back(Vertex{Vec3(-0.5f, -0.5f, 0.5f), color});   // 0
				mesh.vertices.push_back(Vertex{Vec3(0.5f, -0.5f, 0.5f), color});    // 1
				mesh.vertices.push_back(Vertex{Vec3(0.5f, 0.5f, 0.5f), color});     // 2
				mesh.vertices.push_back(Vertex{Vec3(-0.5f, 0.5f, 0.5f), color});    // 3

				// Back face (z-)
				mesh.vertices.push_back(Vertex{Vec3(0.5f, -0.5f, -0.5f), color});   // 4
				mesh.vertices.push_back(Vertex{Vec3(-0.5f, -0.5f, -0.5f), color});  // 5
				mesh.vertices.push_back(Vertex{Vec3(-0.5f, 0.5f, -0.5f), color});   // 6
				mesh.vertices.push_back(Vertex{Vec3(0.5f, 0.5f, -0.5f), color});    // 7

				// Front face
				mesh.indices.push_back(0); mesh.indices.push_back(2); mesh.indices.push_back(1);
				mesh.indices.push_back(0); mesh.indices.push_back(3); mesh.indices.push_back(2);

				// Back face
				mesh.indices.push_back(4); mesh.indices.push_back(6); mesh.indices.push_back(5);
				mesh.indices.push_back(4); mesh.indices.push_back(7); mesh.indices.push_back(6);

				// Left face
				mesh.indices.push_back(5); mesh.indices.push_back(6); mesh.indices.push_back(3);
				mesh.indices.push_back(5); mesh.indices.push_back(3); mesh.indices.push_back(0);

				// Right face
				mesh.indices.push_back(1); mesh.indices.push_back(2); mesh.indices.push_back(7);
				mesh.indices.push_back(1); mesh.indices.push_back(7); mesh.indices.push_back(4);

				// Top face
				mesh.indices.push_back(3); mesh.indices.push_back(6); mesh.indices.push_back(7);
				mesh.indices.push_back(3); mesh.indices.push_back(7); mesh.indices.push_back(2);

				// Bottom face
				mesh.indices.push_back(5); mesh.indices.push_back(0); mesh.indices.push_back(1);
				mesh.indices.push_back(5); mesh.indices.push_back(1); mesh.indices.push_back(4);
				break;

			case ObjectType::Pyramid:
				// Simple pyramid
				mesh.vertices.push_back(Vertex{Vec3(0.0f, 0.5f, 0.0f), color});       // 0 - top
				mesh.vertices.push_back(Vertex{Vec3(-0.5f, -0.5f, 0.5f), color});     // 1 - front-left
				mesh.vertices.push_back(Vertex{Vec3(0.5f, -0.5f, 0.5f), color});      // 2 - front-right
				mesh.vertices.push_back(Vertex{Vec3(0.5f, -0.5f, -0.5f), color});     // 3 - back-right
				mesh.vertices.push_back(Vertex{Vec3(-0.5f, -0.5f, -0.5f), color});    // 4 - back-left

				// Front face
				mesh.indices.push_back(0); mesh.indices.push_back(2); mesh.indices.push_back(1);
				// Right face
				mesh.indices.push_back(0); mesh.indices.push_back(3); mesh.indices.push_back(2);
				// Back face
				mesh.indices.push_back(0); mesh.indices.push_back(4); mesh.indices.push_back(3);
				// Left face
				mesh.indices.push_back(0); mesh.indices.push_back(1); mesh.indices.push_back(4);
				break;

			case ObjectType::Floor:
				// Large plane for floor
				mesh.vertices.push_back(Vertex{Vec3(-5.0f, 0.0f, -5.0f), color});   // 0
				mesh.vertices.push_back(Vertex{Vec3(5.0f, 0.0f, -5.0f), color});    // 1
				mesh.vertices.push_back(Vertex{Vec3(5.0f, 0.0f, 5.0f), color});     // 2
				mesh.vertices.push_back(Vertex{Vec3(-5.0f, 0.0f, 5.0f), color});    // 3

				mesh.indices.push_back(0); mesh.indices.push_back(1); mesh.indices.push_back(2);
				mesh.indices.push_back(0); mesh.indices.push_back(2); mesh.indices.push_back(3);
				break;
		}
	}

}
