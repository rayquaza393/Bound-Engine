#pragma once

#include "Framebuffer.h"
#include "Camera.h"
#include "../Math/Geometry.h"

namespace Bound {

	class Renderer {
	public:
		Renderer(Framebuffer* framebuffer);
		~Renderer();

		// Rendering
		void beginFrame();
		void endFrame();
		void drawMesh(const Mesh& mesh, const Mat4& transform);
		void drawTriangle(const Vec3& v0, const Vec3& v1, const Vec3& v2,
						  const Framebuffer::Color& c0, const Framebuffer::Color& c1,
						  const Framebuffer::Color& c2);

		// Camera access
		Camera* getCamera() { return &camera_; }
		const Camera* getCamera() const { return &camera_; }

	private:
		Framebuffer* framebuffer_;
		Camera camera_;

		// Helper methods
		Vec3 worldToScreen(const Vec3& worldPos) const;
		bool isInFrustum(const Vec3& pos) const;
		Framebuffer::Color interpolateColor(const Vec3& barycoords, 
											const Framebuffer::Color& c0,
											const Framebuffer::Color& c1,
											const Framebuffer::Color& c2) const;
	};

}
