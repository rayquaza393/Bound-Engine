#include "Framebuffer.h"
#include <cmath>

namespace Bound {

	Framebuffer::Framebuffer(int width, int height)
		: width_(width), height_(height) {
		pixels_ = new Color[width * height];
		depthBuffer_ = new uint16_t[width * height];
		clear(0xFF000000); // Black with full alpha
		clearDepth();
	}

	Framebuffer::~Framebuffer() {
		delete[] pixels_;
		delete[] depthBuffer_;
	}

	void Framebuffer::setPixel(int x, int y, Color color) {
		if (isInBounds(x, y)) {
			pixels_[y * width_ + x] = color;
		}
	}

	void Framebuffer::setPixelWithDepth(int x, int y, Color color, uint16_t depth) {
		if (isInBounds(x, y)) {
			int idx = y * width_ + x;
			if (depth < depthBuffer_[idx]) {
				pixels_[idx] = color;
				depthBuffer_[idx] = depth;
			}
		}
	}

	Framebuffer::Color Framebuffer::getPixel(int x, int y) const {
		if (isInBounds(x, y)) {
			return pixels_[y * width_ + x];
		}
		return 0xFF000000;
	}

	void Framebuffer::clear(Color color) {
		for (int i = 0; i < width_ * height_; ++i) {
			pixels_[i] = color;
		}
	}

	void Framebuffer::clearDepth(uint16_t depth) {
		for (int i = 0; i < width_ * height_; ++i) {
			depthBuffer_[i] = depth;
		}
	}

	void Framebuffer::drawLine(int x0, int y0, int x1, int y1, Color color) {
		// Bresenham's line algorithm
		int dx = std::abs(x1 - x0);
		int dy = std::abs(y1 - y0);
		int sx = (x0 < x1) ? 1 : -1;
		int sy = (y0 < y1) ? 1 : -1;
		int err = dx - dy;

		int x = x0;
		int y = y0;

		while (true) {
			setPixel(x, y, color);
			if (x == x1 && y == y1) break;
			int e2 = 2 * err;
			if (e2 > -dy) {
				err -= dy;
				x += sx;
			}
			if (e2 < dx) {
				err += dx;
				y += sy;
			}
		}
	}

	void Framebuffer::drawRect(int x, int y, int w, int h, Color color, bool filled) {
		if (filled) {
			for (int yy = y; yy < y + h; ++yy) {
				for (int xx = x; xx < x + w; ++xx) {
					setPixel(xx, yy, color);
				}
			}
		} else {
			drawLine(x, y, x + w, y, color);
			drawLine(x + w, y, x + w, y + h, color);
			drawLine(x + w, y + h, x, y + h, color);
			drawLine(x, y + h, x, y, color);
		}
	}

	void Framebuffer::drawTriangle(Vec2 p0, Vec2 p1, Vec2 p2, Color color) {
		// Simple triangle rasterization (scanline approach)
		// TODO: Implement proper edge walking and barycentric interpolation

		// For now, just draw the outline
		drawLine((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, color);
		drawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, color);
		drawLine((int)p2.x, (int)p2.y, (int)p0.x, (int)p0.y, color);
	}

	void Framebuffer::applyFogEffect(float distance, float maxDistance) {
		// Simple fog effect: add noise based on distance
		// TODO: Implement actual fog effect with dithering
	}

	Framebuffer::Color Framebuffer::makeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		return (a << 24) | (r << 16) | (g << 8) | b;
	}

	void Framebuffer::unpackColor(Framebuffer::Color c, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) {
		a = (c >> 24) & 0xFF;
		r = (c >> 16) & 0xFF;
		g = (c >> 8) & 0xFF;
		b = c & 0xFF;
	}

}
