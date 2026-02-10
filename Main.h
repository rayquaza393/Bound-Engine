#pragma once

#include "Core/Application.h"

class Game : public Bound::Application {
public:
	Game();
	~Game();

protected:
	void onInit() override;
	void onUpdate(float deltaTime) override;
	void onRender() override;
	void onShutdown() override;

private:
	Bound::Mesh testMesh_;
	bool meshInitialized_ = false;
	
	void initializeMesh();
};

