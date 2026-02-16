#include "GLApplication.h"
#include "../Platform/SDLWindow.h"
#include "Render/Camera.h"
#include <SDL.h>
#include <chrono>
#include <glm/glm.hpp>

namespace Bound {

	GLApplication::GLApplication(const char* title, int width, int height)
		: isRunning_(true), initialized_(false), deltaTime_(0.0f),
		  wasMousePressed_(false), lastMouseX_(0), lastMouseY_(0) {
		
		// Create SDL window with OpenGL context
		window_ = std::make_unique<SDLWindow>(title, width, height);

		// Create GL renderer
		renderer_ = std::make_unique<GLRenderer>();

		// Call user init
		onInit();
	}

	GLApplication::~GLApplication() {
		onShutdown();
		renderer_.reset();
		window_.reset();
	}

	void GLApplication::run() {
		static bool initialized = false;
		if (!initialized) {
			initialized = true;
			onInit();
		}

		auto lastTime = std::chrono::high_resolution_clock::now();

		while (isRunning_ && window_->isOpen()) {
			// Update window
			window_->update();

			// Calculate delta time
			auto currentTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> elapsed = currentTime - lastTime;
			deltaTime_ = elapsed.count();
			lastTime = currentTime;

			// Update input
			updateInput();

			// Game update
			onUpdate(deltaTime_);

			// Rendering
			renderer_->beginFrame();
			onRender();
			renderer_->endFrame();

			// Present to screen
			window_->swapBuffers();
		}
	}

	void GLApplication::updateInput() {
		// Camera controls
		GLRenderer* renderer = renderer_.get();
		Camera* camera = renderer->getCamera();

		float moveSpeed = 5.0f * deltaTime_;
		float rotSpeed = 2.0f * deltaTime_;

		// Movement (WASD)
		if (window_->isKeyPressed(SDL_SCANCODE_W)) {
			camera->moveForward(moveSpeed);
		}
		if (window_->isKeyPressed(SDL_SCANCODE_S)) {
			camera->moveForward(-moveSpeed);
		}
		if (window_->isKeyPressed(SDL_SCANCODE_A)) {
			camera->moveRight(moveSpeed);
		}
		if (window_->isKeyPressed(SDL_SCANCODE_D)) {
			camera->moveRight(-moveSpeed);
		}

		// Up/Down (Space/Shift)
		if (window_->isKeyPressed(SDL_SCANCODE_SPACE)) {
			camera->moveUp(-moveSpeed);
		}
		if (window_->isKeyPressed(SDL_SCANCODE_LSHIFT)) {
			camera->moveUp(moveSpeed);
		}

		// Rotation (Arrow keys)
		if (window_->isKeyPressed(SDL_SCANCODE_LEFT)) {
			camera->rotate(rotSpeed, 0.0f);
		}
		if (window_->isKeyPressed(SDL_SCANCODE_RIGHT)) {
			camera->rotate(-rotSpeed, 0.0f);
		}
		if (window_->isKeyPressed(SDL_SCANCODE_UP)) {
			camera->rotate(0.0f, rotSpeed);
		}
		if (window_->isKeyPressed(SDL_SCANCODE_DOWN)) {
			camera->rotate(0.0f, -rotSpeed);
		}

		// Mouse look (left or right button)
		bool isMousePressed = window_->isMouseButtonPressed(1) || window_->isMouseButtonPressed(3);
		
		if (isMousePressed) {
			int mouseX, mouseY;
			window_->getMousePosition(mouseX, mouseY);
			
			// Only update camera if mouse was already pressed last frame
			if (wasMousePressed_) {
				float yawDelta = -(mouseX - lastMouseX_) * 0.002f;
				float pitchDelta = -(mouseY - lastMouseY_) * 0.002f;  // Inverted Y
				camera->rotate(yawDelta, pitchDelta);
			}
			
			lastMouseX_ = mouseX;
			lastMouseY_ = mouseY;
			wasMousePressed_ = true;
		} else {
			wasMousePressed_ = false;
		}
	}

}
