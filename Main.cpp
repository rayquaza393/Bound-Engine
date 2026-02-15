#include "Main.h"
#include "Core/Render/GLRenderer.h"
#include <glm/glm.hpp>
#include <cstdio>

using namespace Bound;

Game::Game() : GLApplication("Bound Engine - OpenGL", 1280, 720) {
}

Game::~Game() {
}

void Game::onInit() {
	printf("=== Game::onInit() ===\n");
	
	// Initialize asset manager (loads from Assets/ folder)
	assetManager_ = std::make_unique<AssetManager>("Assets/");
	
	// Initialize world
	world_ = std::make_unique<World>();
	printf("World created\n");
	
	// Load scene from file or create default
	loadScene();
	printf("Scene loaded\n");
	
	// Set camera position
	getRenderer()->getCamera()->setPosition(Vec3(0.0f, 0.5f, 3.0f));
}

void Game::loadScene() {
	// For now, create scene from primitives
	// Later: Load from Assets/Levels/defaultLevel.level
	
	printf("Creating scene from primitives...\n");

	// Create pyramid
	testMesh_ = assetManager_->getMeshLibrary().createPyramid(2.0f);
	MeshLibrary::colorMesh(testMesh_, glm::vec3(1.0f, 1.0f, 1.0f));

	// Add floor plane
	Mesh floorMesh = assetManager_->getMeshLibrary().createPlane(10.0f, 10.0f);
	MeshLibrary::colorMesh(floorMesh, glm::vec3(0.2f, 0.5f, 0.2f));
	
	// Combine for now (later: render separately with transforms)
	uint32_t vertexOffset = testMesh_.vertices.size();
	for (const auto& vertex : floorMesh.vertices) {
		Vertex v = vertex;
		v.position.y = -2.0f;  // Move floor down
		testMesh_.vertices.push_back(v);
	}
	for (uint32_t idx : floorMesh.indices) {
		testMesh_.indices.push_back(idx + vertexOffset);
	}

	testMesh_.gpuDirty = true;
	printf("Scene ready: %zu vertices, %zu indices\n", testMesh_.vertices.size(), testMesh_.indices.size());
}

void Game::onUpdate(float deltaTime) {
	// Game logic
}

void Game::onRender() {
	// Render test mesh
	getRenderer()->drawMesh(testMesh_, glm::mat4(1.0f));
}

void Game::onShutdown() {
	world_.reset();
	assetManager_.reset();
}

int main(int argc, char* argv[]) {
	Game game;
	game.run();
	return 0;
}
