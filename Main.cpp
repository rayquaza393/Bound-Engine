#include "Main.h"
#include <windows.h>
#include <cstdio>

using namespace Bound;

Game::Game() : Application("Bound Engine", 1280, 720), world_(nullptr), editor_(nullptr), meshInitialized_(false) {
}

Game::~Game() {
}

void Game::onInit() {
	OutputDebugStringA("=== Game::onInit() CALLED ===\n");
	
	// Initialize world
	world_ = new World();
	OutputDebugStringA("World created\n");
	
	// Initialize editor
	OutputDebugStringA("Creating editor...\n");
	editor_ = new Editor();
	OutputDebugStringA("Editor created successfully\n");
	
	// Create default scene with pyramid + floor
	OutputDebugStringA("About to call initializeMesh\n");
	initializeMesh();
	OutputDebugStringA("initializeMesh returned\n");
}

void Game::initializeMesh() {
	OutputDebugStringA("=== initializeMesh() called ===\n");
	
	if (meshInitialized_) {
		OutputDebugStringA("Already initialized, returning\n");
		return;
	}
	
	OutputDebugStringA("Setting meshInitialized_ = true\n");
	meshInitialized_ = true;
	
	OutputDebugStringA("Adding pyramid vertices...\n");
	
	// Create pyramid mesh - single color (white)
	testMesh_.vertices.push_back(Vertex{Vec3(0.0f, -1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f)});      // Top
	testMesh_.vertices.push_back(Vertex{Vec3(-1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f)});    // Front-left
	testMesh_.vertices.push_back(Vertex{Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f)});     // Front-right
	testMesh_.vertices.push_back(Vertex{Vec3(1.0f, 1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f)});    // Back-right
	testMesh_.vertices.push_back(Vertex{Vec3(-1.0f, 1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f)});   // Back-left

	OutputDebugStringA("Adding pyramid indices...\n");

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

	OutputDebugStringA("Adding floor vertices...\n");

	// Add floor plane at Y=1.0f (same as pyramid base)
	// Dark green grass color (0.2, 0.5, 0.2)
	int floorStartIdx = testMesh_.vertices.size();
	testMesh_.vertices.push_back(Vertex{Vec3(-3.0f, 1.0f, -3.0f), Vec3(0.2f, 0.5f, 0.2f)});  // Back-left
	testMesh_.vertices.push_back(Vertex{Vec3(3.0f, 1.0f, -3.0f), Vec3(0.2f, 0.5f, 0.2f)});   // Back-right
	testMesh_.vertices.push_back(Vertex{Vec3(3.0f, 1.0f, 3.0f), Vec3(0.2f, 0.5f, 0.2f)});    // Front-right
	testMesh_.vertices.push_back(Vertex{Vec3(-3.0f, 1.0f, 3.0f), Vec3(0.2f, 0.5f, 0.2f)});   // Front-left

	OutputDebugStringA("Adding floor indices...\n");

	// Floor triangles (2 triangles to make a quad)
	// Triangle 1
	testMesh_.indices.push_back(floorStartIdx + 0);
	testMesh_.indices.push_back(floorStartIdx + 1);
	testMesh_.indices.push_back(floorStartIdx + 2);

	// Triangle 2
	testMesh_.indices.push_back(floorStartIdx + 0);
	testMesh_.indices.push_back(floorStartIdx + 2);
	testMesh_.indices.push_back(floorStartIdx + 3);

	char buffer[256];
	sprintf_s(buffer, sizeof(buffer), "Mesh complete: %zu vertices, %zu indices\n", 
		testMesh_.vertices.size(), testMesh_.indices.size());
	OutputDebugStringA(buffer);

	// Position camera
	OutputDebugStringA("Setting camera position...\n");
	getRenderer()->getCamera()->setPosition(Vec3(0.0f, 0.5f, 3.0f));
	
	OutputDebugStringA("=== initializeMesh() complete ===\n");
}

void Game::onUpdate(float deltaTime) {
	// Toggle editor mode with Tab
	static bool tabPressed = false;
	if (getWindow()->isKeyPressed(VK_TAB)) {
		if (!tabPressed) {
			if (editor_) {
				editor_->toggleActive();
				const char* mode = editor_->isActive() ? "EDITOR" : "PLAY";
				char buffer[256];
				sprintf_s(buffer, sizeof(buffer), "=== Switched to %s mode ===\n", mode);
				OutputDebugStringA(buffer);
			}
			tabPressed = true;
		}
	} else {
		tabPressed = false;
	}
}

void Game::onRender() {
	// Render game pyramid and floor
	getFramebuffer()->drawRect(100, 100, 400, 400, Framebuffer::makeColor(255, 0, 0), true);
	getRenderer()->drawMesh(testMesh_, Mat4::identity());
}

void Game::onShutdown() {
	// Cleanup
	if (world_) {
		delete world_;
		world_ = nullptr;
	}
	if (editor_) {
		delete editor_;
		editor_ = nullptr;
	}
}

int main(int argc, char* argv[]) {
	Game game;
	game.run();
	return 0;
}
