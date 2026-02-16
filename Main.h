#pragma once

#include "Core/GLApplication.h"
#include "Core/Editor/Editor.h"
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
	std::unique_ptr<Editor> editor_;
};
