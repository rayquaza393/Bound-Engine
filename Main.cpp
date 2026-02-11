#include "Main.h"
#include <windows.h>
#include <cstdio>

using namespace Bound;

Game::Game() : Application("Bound Engine", 1024, 768), world_(nullptr), meshInitialized_(false) {
}

Game::~Game() {
}

void Game::onInit() {
	// Initialize world
	world_ = new World();
	
	// Try to load a level from Assets/Levels/Level1.data
	if (!world_->loadLevel("Assets/Levels/Level1.data")) {
		OutputDebugStringA("No level file found - falling back to test pyramid\n");
	}
}

void Game::initializeMesh() {
	if (meshInitialized_) return;
	meshInitialized_ = true;
	
	OutputDebugStringA("=== Creating pyramid and floor ===\n");
	
	// Create pyramid mesh - single color (white)
	testMesh_.vertices.push_back(Vertex{Vec3(0.0f, -1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)});      // Top
	testMesh_.vertices.push_back(Vertex{Vec3(-1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f)});    // Front-left
	testMesh_.vertices.push_back(Vertex{Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f)});     // Front-right
	testMesh_.vertices.push_back(Vertex{Vec3(1.0f, 1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f)});    // Back-right
	testMesh_.vertices.push_back(Vertex{Vec3(-1.0f, 1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f)});   // Back-left

	// Pyramid faces
	// Front face
	testMesh_.indices.push_back(0);
	testMesh_.indices.push_back(2);
	testMesh_.indices.push_back(1);

	// Right face
	testMesh_.indices.push_back(0);
	testMesh_.indices.push_back(3);
	testMesh_.indices.push_back(2);

	// Back face
	testMesh_.indices.push_back(0);
	testMesh_.indices.push_back(4);
	testMesh_.indices.push_back(3);

	// Left face
	testMesh_.indices.push_back(0);
	testMesh_.indices.push_back(1);
	testMesh_.indices.push_back(4);

	// NOTE: Bottom face intentionally not rendered - sits on floor plane

	// Add floor plane at Y=1.0f (same as pyramid base)
	// Dark green grass color (0.2, 0.5, 0.2)
	int floorStartIdx = testMesh_.vertices.size();
	testMesh_.vertices.push_back(Vertex{Vec3(-3.0f, 1.0f, -3.0f), Vec3(0.2f, 0.5f, 0.2f)});  // Back-left
	testMesh_.vertices.push_back(Vertex{Vec3(3.0f, 1.0f, -3.0f), Vec3(0.2f, 0.5f, 0.2f)});   // Back-right
	testMesh_.vertices.push_back(Vertex{Vec3(3.0f, 1.0f, 3.0f), Vec3(0.2f, 0.5f, 0.2f)});    // Front-right
	testMesh_.vertices.push_back(Vertex{Vec3(-3.0f, 1.0f, 3.0f), Vec3(0.2f, 0.5f, 0.2f)});   // Front-left

	// Floor triangles (2 triangles to make a quad)
	// Triangle 1
	testMesh_.indices.push_back(floorStartIdx + 0);
	testMesh_.indices.push_back(floorStartIdx + 1);
	testMesh_.indices.push_back(floorStartIdx + 2);

	// Triangle 2
	testMesh_.indices.push_back(floorStartIdx + 0);
	testMesh_.indices.push_back(floorStartIdx + 2);
	testMesh_.indices.push_back(floorStartIdx + 3);

	// Position camera
	getRenderer()->getCamera()->setPosition(Vec3(0.0f, 0.5f, 3.0f));
	
	OutputDebugStringA("=== Mesh initialized ===\n");
}

void Game::onUpdate(float deltaTime) {
	// Update game logic here
}

void Game::onRender() {
	// Initialize on first render
	if (!meshInitialized_) {
		initializeMesh();
	}
	
	// Render pyramid and floor
	getFramebuffer()->drawRect(100, 100, 400, 400, Framebuffer::makeColor(255, 0, 0), true);
	getRenderer()->drawMesh(testMesh_, Mat4::identity());
}

void Game::onShutdown() {
	// Cleanup
	if (world_) {
		delete world_;
		world_ = nullptr;
	}
}

int main(int argc, char* argv[]) {
	Game game;
	game.run();
	return 0;
}
