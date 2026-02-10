#include "Application.h"
#include <windows.h>
#include <chrono>
#include <thread>

namespace Bound {

	Application::Application(const char* title, int width, int height)
		: isRunning_(true), lastFrameTime_(0), deltaTime_(0.0f) {
		
		// Create window
		window_ = new Window(title, width, height);

		// Create framebuffer
		framebuffer_ = new Framebuffer(width, height);

		// Create renderer
		renderer_ = new Renderer(framebuffer_);

		onInit();
	}

	Application::~Application() {
		onShutdown();

		delete renderer_;
		delete framebuffer_;
		delete window_;
	}

	void Application::run() {
		while (isRunning_ && window_->isOpen()) {
			// Update window and input
			window_->update();
			updateInput();

			// Calculate delta time
			auto currentTime = std::chrono::high_resolution_clock::now();
			static auto lastTime = currentTime;
			std::chrono::duration<float> elapsed = currentTime - lastTime;
			deltaTime_ = elapsed.count();
			lastTime = currentTime;

			// Game update
			onUpdate(deltaTime_);

			// Rendering
			renderer_->beginFrame();
			onRender();
			renderer_->endFrame();

			// Present to screen
			window_->present(framebuffer_);

			// Frame rate limiting (60 FPS target)
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void Application::updateInput() {
		// Camera movement
		Camera* camera = renderer_->getCamera();
		float moveSpeed = 5.0f * deltaTime_; // 5 units per second
		float lookSpeed = 0.002f; // Radians per pixel

		if (window_->isKeyPressed('W')) camera->moveForward(moveSpeed);
		if (window_->isKeyPressed('S')) camera->moveForward(-moveSpeed);
		if (window_->isKeyPressed('A')) camera->moveRight(-moveSpeed);
		if (window_->isKeyPressed('D')) camera->moveRight(moveSpeed);
		if (window_->isKeyPressed(VK_SPACE)) camera->moveUp(moveSpeed);
		if (window_->isKeyPressed(VK_CONTROL)) camera->moveUp(-moveSpeed);

		// Mouse look
		if (window_->isMouseButtonPressed(1)) { // Right mouse button
			static int lastMouseX = 0, lastMouseY = 0;
			int mouseX, mouseY;
			window_->getMousePosition(mouseX, mouseY);

			float yawDelta = (mouseX - lastMouseX) * lookSpeed;
			float pitchDelta = (mouseY - lastMouseY) * lookSpeed;

			camera->rotate(yawDelta, pitchDelta);

			lastMouseX = mouseX;
			lastMouseY = mouseY;
		}
	}

}
