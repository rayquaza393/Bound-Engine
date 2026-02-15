#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "../Math/Vector.h"
#include <glm/glm.hpp>
#include <memory>

namespace Bound {

	class Camera;

	class GLRenderer {
	public:
		GLRenderer();
		~GLRenderer();

		// Frame management
		void beginFrame();
		void endFrame();

		// Rendering
		void drawMesh(const Mesh& mesh, const glm::mat4& transform);

		// Camera
		Camera* getCamera() { return camera_.get(); }

		// Cleanup
		void shutdown();

	private:
		std::unique_ptr<Camera> camera_;
		std::unique_ptr<Shader> basicShader_;

		void initializeShaders();
	};

}
