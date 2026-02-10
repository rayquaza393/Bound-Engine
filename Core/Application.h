#pragma once

#include "../Platform/Window.h"
#include "Render/Framebuffer.h"
#include "Render/Renderer.h"

namespace Bound {

	class Application {
	public:
		Application(const char* title, int width, int height);
		~Application();

		// Main loop
		void run();
		bool isRunning() const { return isRunning_; }
		void quit() { isRunning_ = false; }

		// Access to subsystems
		Window* getWindow() { return window_; }
		Framebuffer* getFramebuffer() { return framebuffer_; }
		Renderer* getRenderer() { return renderer_; }

	protected:
		// Virtual functions for subclasses to override
		virtual void onInit() {}
		virtual void onUpdate(float deltaTime) {}
		virtual void onRender() {}
		virtual void onShutdown() {}

	private:
		Window* window_;
		Framebuffer* framebuffer_;
		Renderer* renderer_;
		bool isRunning_;

		// Timing
		uint64_t lastFrameTime_;
		float deltaTime_;

		void updateInput();
	};

}
