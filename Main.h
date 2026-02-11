#pragma once

#include "Core/Application.h"
#include "Core/Game/World.h"
#include "Core/Math/Geometry.h"

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
	Mesh testMesh_;
	bool meshInitialized_;
	
	void initializeMesh();
};
