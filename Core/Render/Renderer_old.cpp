#include "Renderer.h"
#include <algorithm>
#include <cstdio>
#include <windows.h>

namespace Bound {

	Renderer::Renderer(Framebuffer* framebuffer)
		: framebuffer_(framebuffer) {
		// Set camera aspect ratio based on framebuffer size
		camera_.setAspect(static_cast<float>(framebuffer->getWidth()) / 
						  static_cast<float>(framebuffer->getHeight()));
		
		char buffer[256];
		sprintf_s(buffer, sizeof(buffer), "Renderer created: %dx%d aspect=%.2f\n", 
			framebuffer->getWidth(), framebuffer->getHeight(), camera_.getAspect());
		OutputDebugStringA(buffer);
		
		Vec3 camPos = camera_.getPosition();
		sprintf_s(buffer, sizeof(buffer), "Camera position: (%.2f, %.2f, %.2f)\n", 
			camPos.x, camPos.y, camPos.z);
		OutputDebugStringA(buffer);
	}

	Renderer::~Renderer() {
	}

	void Renderer::beginFrame() {
		// Clear the framebuffer and depth buffer
		framebuffer_->clear(0xFF1a1a1a); // Dark gray background
		framebuffer_->clearDepth();
	}

	void Renderer::endFrame() {
		// Post-processing would go here (fog effects, dithering, etc.)
	}

	void Renderer::drawMesh(const Mesh& mesh, const Mat4& transform) {
		OutputDebugStringA("=== drawMesh called ===\n");
		char buffer[256];
		
		// Draw each triangle in the mesh
		int triCount = 0;
		for (size_t i = 0; i < mesh.indices.size(); i += 3) {
			uint32_t i0 = mesh.indices[i];
			uint32_t i1 = mesh.indices[i + 1];
			uint32_t i2 = mesh.indices[i + 2];

			const Vertex& v0 = mesh.vertices[i0];
			const Vertex& v1 = mesh.vertices[i1];
			const Vertex& v2 = mesh.vertices[i2];

			sprintf_s(buffer, sizeof(buffer), "Triangle %d world: (%.2f,%.2f,%.2f) (%.2f,%.2f,%.2f) (%.2f,%.2f,%.2f)\n",
				triCount, v0.position.x, v0.position.y, v0.position.z,
				v1.position.x, v1.position.y, v1.position.z,
				v2.position.x, v2.position.y, v2.position.z);
			OutputDebugStringA(buffer);

			// Use vertex colors
			Framebuffer::Color c0 = Framebuffer::makeColor(
				static_cast<uint8_t>(v0.color.x * 255.0f),
				static_cast<uint8_t>(v0.color.y * 255.0f),
				static_cast<uint8_t>(v0.color.z * 255.0f)
			);
			Framebuffer::Color c1 = Framebuffer::makeColor(
				static_cast<uint8_t>(v1.color.x * 255.0f),
				static_cast<uint8_t>(v1.color.y * 255.0f),
				static_cast<uint8_t>(v1.color.z * 255.0f)
			);
			Framebuffer::Color c2 = Framebuffer::makeColor(
				static_cast<uint8_t>(v2.color.x * 255.0f),
				static_cast<uint8_t>(v2.color.y * 255.0f),
				static_cast<uint8_t>(v2.color.z * 255.0f)
			);

			drawTriangle(v0.position, v1.position, v2.position, c0, c1, c2);
			triCount++;
		}
		sprintf_s(buffer, sizeof(buffer), "Total triangles: %d\n", triCount);
		OutputDebugStringA(buffer);
	}

	void Renderer::drawTriangle(const Vec3& v0, const Vec3& v1, const Vec3& v2,
							   const Framebuffer::Color& c0, const Framebuffer::Color& c1,
							   const Framebuffer::Color& c2) {
		// Transform vertices to screen space
		Vec3 screen0 = worldToScreen(v0);
		Vec3 screen1 = worldToScreen(v1);
		Vec3 screen2 = worldToScreen(v2);

		char buffer[256];
		sprintf_s(buffer, sizeof(buffer), "  Screen: (%.1f,%.1f) (%.1f,%.1f) (%.1f,%.1f)\n",
			screen0.x, screen0.y, screen1.x, screen1.y, screen2.x, screen2.y);
		OutputDebugStringA(buffer);

		// Backface culling: check if triangle is facing the camera
		Vec3 edge1 = screen1 - screen0;
		Vec3 edge2 = screen2 - screen0;
		Vec3 normal = edge1.cross(edge2);
		
		sprintf_s(buffer, sizeof(buffer), "  Normal.z: %.2f %s\n", normal.z, normal.z > 0.0f ? "FRONT" : "BACK");
		OutputDebugStringA(buffer);
		
		if (normal.z <= 0.0f) {
			return; // Back-facing triangle
		}

		// Draw white cross to show vertex position
		Framebuffer::Color white = Framebuffer::makeColor(255, 255, 255);
		for (int dx = -3; dx <= 3; dx++) {
			framebuffer_->setPixel(static_cast<int>(screen0.x) + dx, static_cast<int>(screen0.y), white);
			framebuffer_->setPixel(static_cast<int>(screen0.x), static_cast<int>(screen0.y) + dx, white);
		}

		// Draw triangle with vertex colors
		framebuffer_->drawLine(
			static_cast<int>(screen0.x), static_cast<int>(screen0.y),
			static_cast<int>(screen1.x), static_cast<int>(screen1.y),
			c0
		);
		framebuffer_->drawLine(
			static_cast<int>(screen1.x), static_cast<int>(screen1.y),
			static_cast<int>(screen2.x), static_cast<int>(screen2.y),
			c1
		);
		framebuffer_->drawLine(
			static_cast<int>(screen2.x), static_cast<int>(screen2.y),
			static_cast<int>(screen0.x), static_cast<int>(screen0.y),
			c2
		);
	}

	Vec3 Renderer::worldToScreen(const Vec3& worldPos) const {
		Mat4 vp = camera_.getViewProjectionMatrix();
		Vec4 clipSpace = vp * Vec4(worldPos, 1.0f);

		// Perspective divide
		if (clipSpace.w != 0.0f) {
			clipSpace = clipSpace / clipSpace.w;
		}

		// NDC to screen space
		float x = (clipSpace.x + 1.0f) * 0.5f * framebuffer_->getWidth();
		float y = (1.0f - clipSpace.y) * 0.5f * framebuffer_->getHeight();
		float z = clipSpace.z;

		return Vec3(x, y, z);
	}

	bool Renderer::isInFrustum(const Vec3& pos) const {
		// Basic frustum check: is the point in front of the camera?
		Vec3 toPoint = pos - camera_.getPosition();
		float distance = toPoint.dot(camera_.getForward());
		return distance > camera_.getNear() && distance < camera_.getFar();
	}

	Framebuffer::Color Renderer::interpolateColor(const Vec3& barycoords,
												   const Framebuffer::Color& c0,
												   const Framebuffer::Color& c1,
												   const Framebuffer::Color& c2) const {
		uint8_t r0, g0, b0, a0;
		uint8_t r1, g1, b1, a1;
		uint8_t r2, g2, b2, a2;

		Framebuffer::unpackColor(c0, r0, g0, b0, a0);
		Framebuffer::unpackColor(c1, r1, g1, b1, a1);
		Framebuffer::unpackColor(c2, r2, g2, b2, a2);

		uint8_t r = static_cast<uint8_t>(r0 * barycoords.x + r1 * barycoords.y + r2 * barycoords.z);
		uint8_t g = static_cast<uint8_t>(g0 * barycoords.x + g1 * barycoords.y + g2 * barycoords.z);
		uint8_t b = static_cast<uint8_t>(b0 * barycoords.x + b1 * barycoords.y + b2 * barycoords.z);
		uint8_t a = static_cast<uint8_t>(a0 * barycoords.x + a1 * barycoords.y + a2 * barycoords.z);

		return Framebuffer::makeColor(r, g, b, a);
	}

}

	