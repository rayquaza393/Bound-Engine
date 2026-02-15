#pragma once

#include <string>
#include <map>
#include <memory>
#include "../Render/Mesh.h"

namespace Bound {

	/**
	 * MeshLibrary - Creates primitive meshes
	 * Generates: Cube, Pyramid, Plane, Sphere
	 */
	class MeshLibrary {
	public:
		MeshLibrary();
		~MeshLibrary();

		Mesh createCube(float size = 1.0f);
		Mesh createPyramid(float size = 1.0f);
		Mesh createPlane(float width = 1.0f, float height = 1.0f);
		Mesh createSphere(float radius = 1.0f, int segments = 16);

		static void colorMesh(Mesh& mesh, const glm::vec3& color);

	private:
		void addQuad(Mesh& mesh, const glm::vec3& p0, const glm::vec3& p1, 
		             const glm::vec3& p2, const glm::vec3& p3);
	};

}
