#include "Renderer.h"
#include <algorithm>
#include <cstdio>
#include <windows.h>

// Undefine min/max macros from windows.h to avoid conflicts with std::min/max
#undef min
#undef max

namespace Bound {

static int frameCounter = 0;  // Frame counter for debug output throttling

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
		
		// Increment frame counter for throttled debug output
		frameCounter++;
	}

	void Renderer::endFrame() {
		// Post-processing would go here (fog effects, dithering, etc.)
	}

	void Renderer::drawMesh(const Mesh& mesh, const Mat4& transform) {
		char buffer[256];
		sprintf_s(buffer, sizeof(buffer), "drawMesh: %zu triangles\n", mesh.indices.size() / 3);
		OutputDebugStringA(buffer);
		
		// Draw each triangle in the mesh
		int triCount = 0;
		for (size_t i = 0; i < mesh.indices.size(); i += 3) {
			uint32_t i0 = mesh.indices[i];
			uint32_t i1 = mesh.indices[i + 1];
			uint32_t i2 = mesh.indices[i + 2];

			const Vertex& v0 = mesh.vertices[i0];
			const Vertex& v1 = mesh.vertices[i1];
			const Vertex& v2 = mesh.vertices[i2];

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
		}
	}

void Renderer::drawTriangle(const Vec3& v0, const Vec3& v1, const Vec3& v2,
                           const Framebuffer::Color& c0, const Framebuffer::Color& c1,
                           const Framebuffer::Color& c2) {
    // Calculate face normal in world space for flat shading
    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;
    Vec3 faceNormal = edge1.cross(edge2).normalize();  // Standard cross product

    // Transform vertices to screen space (keep z for depth)
    Vec3 screen0 = worldToScreen(v0);
    Vec3 screen1 = worldToScreen(v1);
    Vec3 screen2 = worldToScreen(v2);

    // Bounding box - compute min/max manually for C++14 compatibility
    float minXf = std::min(screen0.x, std::min(screen1.x, screen2.x));
    float maxXf = std::max(screen0.x, std::max(screen1.x, screen2.x));
    float minYf = std::min(screen0.y, std::min(screen1.y, screen2.y));
    float maxYf = std::max(screen0.y, std::max(screen1.y, screen2.y));

    int minX = static_cast<int>(std::max(0.0f, std::floor(minXf)));
    int maxX = static_cast<int>(std::min(static_cast<float>(framebuffer_->getWidth() - 1), std::ceil(maxXf)));
    int minY = static_cast<int>(std::max(0.0f, std::floor(minYf)));
    int maxY = static_cast<int>(std::min(static_cast<float>(framebuffer_->getHeight() - 1), std::ceil(maxYf)));

    // Precompute edge function (correct cross product formula)
    auto edgeFunction = [](const Vec3& a, const Vec3& b, const Vec3& c) {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    };
    float area = edgeFunction(screen0, screen1, screen2);

	if (area <= 0.0f) {
		return; // Cull back-facing triangles (negative/zero area = facing away)
	}

    // Lighting: simple point light with FLAT SHADING
    Vec3 lightPos(3.0f, 1.0f, 2.0f);
    float lightIntensity = 1.5f;
    float lightRadius = 8.0f;          // Larger radius for better illumination

    // Calculate lighting once for the entire face using face normal
    Vec3 faceCenter = (v0 + v1 + v2) * (1.0f / 3.0f);
    Vec3 toLightDir = (lightPos - faceCenter).normalize();
    float diffuse = std::max(0.0f, faceNormal.dot(toLightDir));
    float distToLight = (lightPos - faceCenter).length();
    float falloff = std::max(0.0f, 1.0f - (distToLight / lightRadius));
    float lightAmount = 0.35f + 0.65f * diffuse * falloff * lightIntensity;
    
    // Debug: Print face lighting info (throttled - once every 60 frames)
    if (frameCounter % 60 == 0) {  // Print every 60 frames
        char debugBuf[256];
        sprintf_s(debugBuf, sizeof(debugBuf), 
            "Face Normal: (%.2f, %.2f, %.2f) | Diffuse: %.2f | Distance: %.2f | LightAmount: %.2f\n",
            faceNormal.x, faceNormal.y, faceNormal.z, diffuse, distToLight, lightAmount);
        OutputDebugStringA(debugBuf);
    }

    // Unpack vertex colors
    uint8_t r0, g0, b0, a0;
    uint8_t r1, g1, b1, a1;
    uint8_t r2, g2, b2, a2;
    Framebuffer::unpackColor(c0, r0, g0, b0, a0);
    Framebuffer::unpackColor(c1, r1, g1, b1, a1);
    Framebuffer::unpackColor(c2, r2, g2, b2, a2);

    Vec3 col0(r0 / 255.0f, g0 / 255.0f, b0 / 255.0f);
    Vec3 col1(r1 / 255.0f, g1 / 255.0f, b1 / 255.0f);
    Vec3 col2(r2 / 255.0f, g2 / 255.0f, b2 / 255.0f);

    // For each pixel in bounding box
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            Vec3 p(static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f, 0.0f);
            // Compute barycentric coordinates
            float w0 = edgeFunction(screen1, screen2, p);
            float w1 = edgeFunction(screen2, screen0, p);
            float w2 = edgeFunction(screen0, screen1, p);
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area;
                w2 /= area;

                // Interpolate depth (z)
                float z = screen0.z * w0 + screen1.z * w1 + screen2.z * w2;
                z = std::max(0.0f, std::min(1.0f, z));
                uint16_t z16 = static_cast<uint16_t>(z * 65535.0f);

                // Interpolate vertex colors but apply FLAT lighting (same for whole face)
                Vec3 color = col0 * w0 + col1 * w1 + col2 * w2;
                color = color * lightAmount;

                // Clamp and convert to 8-bit
                color.x = std::max(0.0f, std::min(1.0f, color.x));
                color.y = std::max(0.0f, std::min(1.0f, color.y));
                color.z = std::max(0.0f, std::min(1.0f, color.z));
                uint8_t r = static_cast<uint8_t>(color.x * 255.0f);
                uint8_t g = static_cast<uint8_t>(color.y * 255.0f);
                uint8_t b = static_cast<uint8_t>(color.z * 255.0f);
                Framebuffer::Color finalColor = Framebuffer::makeColor(r, g, b);

                framebuffer_->setPixelWithDepth(x, y, finalColor, z16);
            }
        }
    }
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