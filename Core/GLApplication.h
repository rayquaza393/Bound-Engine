#pragma once

#include "../Platform/SDLWindow.h"
#include "Render/GLRenderer.h"
#include <memory>
#include <cstdint>

namespace Bound {

	class GLApplication {
	public:
		GLApplication(const char* title, int width, int height);
		virtual ~GLApplication();

		// Main loop
		void run();
		bool isRunning() const { return isRunning_; }
		void quit() { isRunning_ = false; }

		// Access to subsystems
		SDLWindow* getWindow() { return window_.get(); }
		GLRenderer* getRenderer() { return renderer_.get(); }

	protected:
		// Virtual functions for subclasses to override
		virtual void onInit() {}
		virtual void onUpdate(float deltaTime) {}
		virtual void onRender() {}
		virtual void onShutdown() {}

	private:
		std::unique_ptr<SDLWindow> window_;
		std::unique_ptr<GLRenderer> renderer_;
		bool isRunning_;
		bool initialized_;
		bool wasMousePressed_;
		int lastMouseX_;
		int lastMouseY_;

		// Timing
		float deltaTime_;

		void updateInput();
	};

}
