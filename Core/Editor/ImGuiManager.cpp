#include "ImGuiManager.h"
#include "../Render/GLRenderer.h"
#include "../../Platform/SDLWindow.h"
#include <cstdio>
#include <SDL.h>

// Suppress warnings before including ImGui
#pragma warning(push)
#pragma warning(disable: 4245 4267 4996 4191 4555 4456 4457 4668 4746 4755)

#include "imgui.h"
#include "imgui_impl_opengl3.h"

#pragma warning(pop)

namespace Bound {

	ImGuiManager::ImGuiManager()
		: initialized_(false), renderer_(nullptr), window_(nullptr) {
		printf("ImGuiManager created\n");
	}

	ImGuiManager::~ImGuiManager() {
		if (initialized_) {
			shutdown();
		}
	}

	void ImGuiManager::initialize(GLRenderer* renderer, SDLWindow* window) {
		renderer_ = renderer;
		window_ = window;
		
		// Create ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		
		// Set display size
		io.DisplaySize = ImVec2(1280.0f, 720.0f);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		
		// Setup style
		ImGui::StyleColorsLight();
		
		// Setup Renderer backend
		ImGui_ImplOpenGL3_Init("#version 330");
		
		initialized_ = true;
		printf("ImGuiManager initialized with SDL2 + OpenGL3 (1280x720)\n");
	}

	void ImGuiManager::shutdown() {
		if (!initialized_) return;
		
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
		
		initialized_ = false;
		printf("ImGuiManager shutdown\n");
	}

	void ImGuiManager::beginFrame() {
		if (!initialized_) return;
		
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(1280.0f, 720.0f);
		
		// Handle SDL2 input events manually
		handleSDLInput();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiManager::endFrame() {
		if (!initialized_) return;
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiManager::render() {
		// ImGui render is done in endFrame()
	}

	void ImGuiManager::handleSDLInput() {
		if (!window_) return;

		ImGuiIO& io = ImGui::GetIO();

		// Handle keyboard - just modifier keys
		const uint8_t* keyboardState = SDL_GetKeyboardState(nullptr);
		
		io.KeyCtrl = keyboardState[SDL_SCANCODE_LCTRL] || keyboardState[SDL_SCANCODE_RCTRL];
		io.KeyShift = keyboardState[SDL_SCANCODE_LSHIFT] || keyboardState[SDL_SCANCODE_RSHIFT];
		io.KeyAlt = keyboardState[SDL_SCANCODE_LALT] || keyboardState[SDL_SCANCODE_RALT];
		io.KeySuper = keyboardState[SDL_SCANCODE_LGUI] || keyboardState[SDL_SCANCODE_RGUI];

		// Handle mouse position and buttons
		int mouseX, mouseY;
		window_->getMousePosition(mouseX, mouseY);
		io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));

		// Mouse buttons (SDL uses 1-based button numbering)
		io.MouseDown[0] = window_->isMouseButtonPressed(SDL_BUTTON_LEFT);
		io.MouseDown[1] = window_->isMouseButtonPressed(SDL_BUTTON_RIGHT);
		io.MouseDown[2] = window_->isMouseButtonPressed(SDL_BUTTON_MIDDLE);

		// Mouse wheel
		io.MouseWheel = 0.0f;
	}

}
