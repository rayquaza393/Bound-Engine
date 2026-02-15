#pragma once

#include "Core/GLApplication.h"
#include "Core/Game/World.h"
#include "Core/Render/Mesh.h"
#include "Core/Assets/AssetManager.h"
#include <memory>

using namespace Bound;

class Game : public GLApplication {
public:
	Game();
	~Game();

protected:
	void onInit() override;
	void onUpdate(float deltaTime) override;
	void onRender() override;
	void onShutdown() override;

private:
	std::unique_ptr<World> world_;
	std::unique_ptr<AssetManager> assetManager_;
	Mesh testMesh_;
	
	void loadScene();
};
