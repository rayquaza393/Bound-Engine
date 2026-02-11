#pragma once

#include "Core/Application.h"
#include "Core/Game/World.h"
#include "Core/Math/Geometry.h"
#include "Core/Editor/Editor.h"

using namespace Bound;

class Game : public Application {
public:
	Game();
	~Game();

protected:
	void onInit() override;
	void onUpdate(float deltaTime) override;
	void onRender() override;
	void onShutdown() override;

private:
	World* world_;
	Editor* editor_;
	Mesh testMesh_;
	bool meshInitialized_;
	
	void initializeMesh();
};
