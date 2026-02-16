#pragma once

#include <memory>

namespace Bound {

	class GLRenderer;
	class SDLWindow;
	class Editor;

	/**
	 * ImGuiManager - Initializes and manages ImGui for the editor
	 * Handles SDL2 input and OpenGL3 rendering
	 */
	class ImGuiManager {
	public:
		ImGuiManager();
		~ImGuiManager();

		void initialize(GLRenderer* renderer, SDLWindow* window);
		void shutdown();
		
		void beginFrame();
		void endFrame();
		void render();

		bool isInitialized() const { return initialized_; }

	private:
		bool initialized_;
		GLRenderer* renderer_;
		SDLWindow* window_;

		void handleSDLInput();
	};

}
