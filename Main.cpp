#include "Main.h"
#include <windows.h>
#include <cstdio>

Game::Game() : Bound::Application("Bound Engine", 1024, 768) {
}

Game::~Game() {
}

void Game::onInit() {
	OutputDebugStringA("=== Game::onInit() called ===\n");
}

void Game::initializeMesh() {
	if (meshInitialized_) return;
	meshInitialized_ = true;
	
	OutputDebugStringA("=== initializeMesh() creating pyramid ===\n");
	
	// Create a simple test pyramid mesh
	testMesh_.vertices.push_back(Bound::Vertex{Bound::Vec3(0.0f, 1.0f, 0.0f), Bound::Vec3(1.0f, 0.0f, 0.0f)});      // Top (red)
	testMesh_.vertices.push_back(Bound::Vertex{Bound::Vec3(-1.0f, -1.0f, 1.0f), Bound::Vec3(0.0f, 1.0f, 0.0f)});    // Front-left (green)
	testMesh_.vertices.push_back(Bound::Vertex{Bound::Vec3(1.0f, -1.0f, 1.0f), Bound::Vec3(0.0f, 0.0f, 1.0f)});     // Front-right (blue)
	testMesh_.vertices.push_back(Bound::Vertex{Bound::Vec3(1.0f, -1.0f, -1.0f), Bound::Vec3(1.0f, 1.0f, 0.0f)});    // Back-right (yellow)
	testMesh_.vertices.push_back(Bound::Vertex{Bound::Vec3(-1.0f, -1.0f, -1.0f), Bound::Vec3(1.0f, 0.0f, 1.0f)});   // Back-left (magenta)

	char buffer[256];
	sprintf_s(buffer, sizeof(buffer), "Created %zu vertices\n", testMesh_.vertices.size());
	OutputDebugStringA(buffer);

	// Front face
	testMesh_.indices.push_back(0);
	testMesh_.indices.push_back(1);
	testMesh_.indices.push_back(2);

	// Right face
	testMesh_.indices.push_back(0);
	testMesh_.indices.push_back(2);
	testMesh_.indices.push_back(3);

	// Back face
	testMesh_.indices.push_back(0);
	testMesh_.indices.push_back(3);
	testMesh_.indices.push_back(4);

	// Left face
	testMesh_.indices.push_back(0);
	testMesh_.indices.push_back(4);
	testMesh_.indices.push_back(1);

	// Bottom face
	testMesh_.indices.push_back(1);
	testMesh_.indices.push_back(2);
	testMesh_.indices.push_back(3);

	testMesh_.indices.push_back(1);
	testMesh_.indices.push_back(3);
	testMesh_.indices.push_back(4);

	sprintf_s(buffer, sizeof(buffer), "Created %zu indices\n", testMesh_.indices.size());
	OutputDebugStringA(buffer);

	// Position camera
	getRenderer()->getCamera()->setPosition(Bound::Vec3(0.0f, 0.0f, 5.0f));
	
	OutputDebugStringA("=== initializeMesh() complete ===\n");
}

void Game::onUpdate(float deltaTime) {
	// Update game logic here
}

void Game::onRender() {
	// Initialize mesh on first render
	initializeMesh();
	
	// First, draw a test rectangle to verify rendering works
	getFramebuffer()->drawRect(100, 100, 400, 400, Bound::Framebuffer::makeColor(255, 0, 0), true);
	
	// Then render the test mesh
	getRenderer()->drawMesh(testMesh_, Bound::Mat4::identity());
}

void Game::onShutdown() {
	// Cleanup
}

int main(int argc, char* argv[]) {
	Game game;
	game.run();
	return 0;
}
