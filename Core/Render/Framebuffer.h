#pragma once

#include "../Math/Vector.h"
#include <cstring>
#include <algorithm>

namespace Bound {

	class Framebuffer {
	public:
		// Color is stored as ARGB uint32_t
		typedef uint32_t Color;

		Framebuffer(int width, int height);
		~Framebuffer();

		// Getters
		int getWidth() const { return width_; }
		int getHeight() const { return height_; }
		Color* getPixels() const { return pixels_; }
		uint16_t* getDepthBuffer() const { return depthBuffer_; }

		// Pixel operations
		void setPixel(int x, int y, Color color);
		void setPixelWithDepth(int x, int y, Color color, uint16_t depth);
		Color getPixel(int x, int y) const;

		// Bulk operations
		void clear(Color color);
		void clearDepth(uint16_t depth = 0xFFFF);

		// Drawing primitives
		void drawLine(int x0, int y0, int x1, int y1, Color color);
		void drawRect(int x, int y, int w, int h, Color color, bool filled = true);
		void drawTriangle(Vec2 p0, Vec2 p1, Vec2 p2, Color color);

		// Post-processing effects
		void applyFogEffect(float distance, float maxDistance);

		// Utility
		static Color makeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
		static void unpackColor(Color c, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a);

	private:
		int width_;
		int height_;
		Color* pixels_;
		uint16_t* depthBuffer_; // Z-buffer for depth testing

		// Helper for bounds checking
		bool isInBounds(int x, int y) const {
			return x >= 0 && x < width_ && y >= 0 && y < height_;
		}
	};

}
