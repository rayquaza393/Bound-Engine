#include "SDLWindow.h"
#include <GL/glew.h>
#include <SDL.h>
#include <cstring>
#include <cstdio>

namespace Bound {

	SDLWindow::SDLWindow(const char* title, int width, int height)
		: window_(nullptr), glContext_(nullptr), isOpen_(true), width_(width), height_(height),
		  keyboardState_(nullptr), keyboardStateSize_(0) {
		
		// Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("SDL_Init failed: %s\n", SDL_GetError());
			return;
		}

		// Set GL attributes BEFORE creating window
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		// Create window
		window_ = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width, height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
		);

		if (!window_) {
			printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
			SDL_Quit();
			return;
		}

		// Create GL context
		glContext_ = SDL_GL_CreateContext(window_);
		if (!glContext_) {
			printf("SDL_GL_CreateContext failed: %s\n", SDL_GetError());
			SDL_DestroyWindow(window_);
			SDL_Quit();
			return;
		}

		// Enable vsync
		SDL_GL_SetSwapInterval(1);

		// Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum glewErr = glewInit();
		if (glewErr != GLEW_OK) {
			printf("GLEW initialization failed: %s\n", glewGetErrorString(glewErr));
		}

		// Get keyboard state
		keyboardState_ = SDL_GetKeyboardState(&keyboardStateSize_);

		printf("SDL Window created: %dx%d\n", width, height);
		printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
		printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	SDLWindow::~SDLWindow() {
		if (glContext_) {
			SDL_GL_DeleteContext(glContext_);
		}
		if (window_) {
			SDL_DestroyWindow(window_);
		}
		SDL_Quit();
	}

	void SDLWindow::update() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					isOpen_ = false;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						// Don't close on ESC - let the app handle it
						// isOpen_ = false;
					}
					break;
			}
		}

		// Update keyboard state
		keyboardState_ = SDL_GetKeyboardState(&keyboardStateSize_);
	}

	bool SDLWindow::isKeyPressed(SDL_Scancode key) const {
		if (!keyboardState_) return false;
		if (key < 0 || key >= keyboardStateSize_) return false;
		return keyboardState_[key] != 0;
	}

	bool SDLWindow::isMouseButtonPressed(int button) const {
		uint32_t mouseState = SDL_GetMouseState(nullptr, nullptr);
		return (mouseState & SDL_BUTTON(button)) != 0;
	}

	void SDLWindow::getMousePosition(int& x, int& y) const {
		SDL_GetMouseState(&x, &y);
	}

	void SDLWindow::swapBuffers() const {
		SDL_GL_SwapWindow(window_);
	}

}
