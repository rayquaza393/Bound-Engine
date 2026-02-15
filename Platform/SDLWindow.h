#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <cstdint>

namespace Bound {

	class SDLWindow {
	public:
		SDLWindow(const char* title, int width, int height);
		~SDLWindow();

		// Window operations
		void update();
		bool isOpen() const { return isOpen_; }
		void close() { isOpen_ = false; }

		// Input
		bool isKeyPressed(SDL_Scancode key) const;
		bool isMouseButtonPressed(int button) const;
		void getMousePosition(int& x, int& y) const;

		// GL context
		SDL_GLContext getGLContext() const { return glContext_; }
		void swapBuffers() const;

		// Dimensions
		int getWidth() const { return width_; }
		int getHeight() const { return height_; }

	private:
		SDL_Window* window_;
		SDL_GLContext glContext_;
		bool isOpen_;
		int width_;
		int height_;

		// Input state
		const uint8_t* keyboardState_;
		int keyboardStateSize_;
	};

}
