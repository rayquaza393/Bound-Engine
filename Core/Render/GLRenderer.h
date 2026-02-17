#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "../Math/Vector.h"
#include <glm/glm.hpp>
#include <memory>

namespace Bound {

	class Camera;
	class SDLWindow;

	class GLRenderer {
	public:
		GLRenderer();
		~GLRenderer();

		// Initialization with window reference
		void initialize(SDLWindow* window);

		// Frame management
		void beginFrame();
		void endFrame();

		// Rendering
		void drawMesh(const Mesh& mesh, const glm::mat4& transform);

		// Camera
		Camera* getCamera() { return camera_.get(); }

		// Scene texture for ImGui display
		unsigned int getSceneTexture() const { return sceneTexture_; }
		int getSceneTextureWidth() const { return sceneTextureWidth_; }
		int getSceneTextureHeight() const { return sceneTextureHeight_; }

		// Cleanup
		void shutdown();

	private:
		std::unique_ptr<Camera> camera_;
		std::unique_ptr<Shader> basicShader_;
		SDLWindow* window_;

		// Framebuffer object for off-screen rendering
		unsigned int framebufferObject_;
		unsigned int sceneTexture_;
		unsigned int depthRenderbuffer_;
		int sceneTextureWidth_;
		int sceneTextureHeight_;

		void initializeShaders();
		void initializeFramebuffer(int width, int height);
	};

}


