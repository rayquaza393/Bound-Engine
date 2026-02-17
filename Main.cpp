#include "Main.h"
#include "Core/Render/GLRenderer.h"
#include "Core/Editor/Editor.h"
#include "Platform/SDLWindow.h"
#include <cstdio>
#include <imgui.h>
#include <SDL.h>

using namespace Bound;

Game::Game() : GLApplication("Bound Engine - Editor", 1280, 720) {
}

Game::~Game() {
}

void Game::onInit() {
	printf("=== Game::onInit() - Bound Engine Editor ===\n");
	
	// Initialize editor with both renderer and window
	// ImGui is initialized internally during editor initialization
	editor_ = std::make_unique<Editor>();
	editor_->initialize(getRenderer(), getWindow());
	
	// Create some test objects to display
	printf("Creating test objects...\n");
	editor_->createObject(ObjectType::Cube, glm::vec3(-2.0f, 0.0f, 0.0f));
	editor_->createObject(ObjectType::Pyramid, glm::vec3(0.0f, 0.0f, 0.0f));
	editor_->createObject(ObjectType::Floor, glm::vec3(0.0f, -2.0f, 0.0f));
	
	printf("Editor initialized with test scene\n");
}

void Game::onUpdate(float deltaTime) {
	editor_->update(deltaTime);
	
	// Handle tool hotkeys (1-4)
	if (getWindow()->isKeyPressed(SDL_SCANCODE_1)) {
		editor_->setCurrentTool(EditorTool::Select);
	}
	if (getWindow()->isKeyPressed(SDL_SCANCODE_2)) {
		editor_->setCurrentTool(EditorTool::Move);
	}
	if (getWindow()->isKeyPressed(SDL_SCANCODE_3)) {
		editor_->setCurrentTool(EditorTool::Rotate);
	}
	if (getWindow()->isKeyPressed(SDL_SCANCODE_4)) {
		editor_->setCurrentTool(EditorTool::Scale);
	}
	
	// Handle left-click object selection
	if (getWindow()->isMouseButtonPressed(SDL_BUTTON_LEFT)) {
		ImGuiIO& io = ImGui::GetIO();
		// Only select if not hovering over ImGui
		if (!io.WantCaptureMouse) {
			int mouseX, mouseY;
			getWindow()->getMousePosition(mouseX, mouseY);
			
			EditorObject* selected = editor_->raycastFromScreenPos(mouseX, mouseY, getWindow()->getWidth(), getWindow()->getHeight());
			if (selected) {
				editor_->selectObject(selected);
			} else {
				editor_->selectObject(nullptr);
			}
		}
	}
	
	// If editor requests exit, quit the application
	if (!editor_->isActive()) {
		quit();
	}
}

void Game::onRender() {
	// Render 3D scene (to offscreen framebuffer)
	editor_->render();
	
	// Render ImGui UI (to main framebuffer - happens after endFrame unbinds FBO)
	editor_->renderUI();
}

void Game::onShutdown() {
	editor_->shutdown();
	editor_.reset();
}

int main(int argc, char* argv[]) {
	printf("=== main() starting - Bound Engine Editor ===\n");
	Game game;
	printf("Game created, calling run()\n");
	game.run();
	printf("run() returned\n");
	return 0;
}
