#include "Main.h"
#include "Core/Render/GLRenderer.h"
#include <cstdio>

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
	
	printf("Editor initialized\n");
}

void Game::onUpdate(float deltaTime) {
	editor_->update(deltaTime);
}

void Game::onRender() {
	editor_->render();
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
