#include "Editor.h"
#include "EditorUI.h"
#include "ImGuiManager.h"
#include "../Render/GLRenderer.h"
#include "../Render/Camera.h"
#include "../../Platform/SDLWindow.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cstdio>

namespace Bound {

	Editor::Editor()
		: active_(true), playMode_(false), renderer_(nullptr), editorCamera_(nullptr),
		  selectedObject_(nullptr), nextObjectId_(1), gizmoScale_(1.0f),
		  showGrid_(true), showHierarchy_(true), showProperties_(true), showViewport_(true) {
		printf("Editor created\n");
	}

	Editor::~Editor() {
		printf("Editor destroyed\n");
	}

	void Editor::initialize(GLRenderer* renderer, SDLWindow* window) {
		renderer_ = renderer;
		editorCamera_ = renderer_->getCamera();
		editorCamera_->setPosition(Vec3(0.0f, 2.0f, 5.0f));
		
		// Initialize ImGui FIRST with window before anything else uses it
		imguiManager_ = std::make_unique<ImGuiManager>();
		imguiManager_->initialize(renderer, window);
		
		// Then initialize UI after ImGui context exists
		ui_ = std::make_unique<EditorUI>();
		
		printf("Editor initialized with ImGui\n");
	}

	void Editor::shutdown() {
		objects_.clear();
		selectedObject_ = nullptr;
		printf("Editor shutdown complete\n");
	}

	void Editor::update(float deltaTime) {
		if (!active_ || playMode_) return;
		
		if (imguiManager_) {
			imguiManager_->beginFrame();
		}
	}

	void Editor::render() {
		if (!active_) return;

		// Render ImGui UI first (on top layer)
		if (ui_) {
			ui_->render(this);
		}

		// Render 3D objects in viewport
		for (auto& obj : objects_) {
			glm::mat4 transform = glm::mat4(1.0f);
			transform = glm::translate(transform, glm::vec3(obj->position.x, obj->position.y, obj->position.z));
			transform = glm::rotate(transform, obj->rotation.x, glm::vec3(1, 0, 0));
			transform = glm::rotate(transform, obj->rotation.y, glm::vec3(0, 1, 0));
			transform = glm::rotate(transform, obj->rotation.z, glm::vec3(0, 0, 1));
			transform = glm::scale(transform, glm::vec3(obj->scale.x, obj->scale.y, obj->scale.z));

			renderer_->drawMesh(obj->mesh, transform);
		}
		
		// Render ImGui after 3D objects
		if (imguiManager_) {
			imguiManager_->endFrame();
		}
	}

	void Editor::handleInput() {
		// Input handling for editor
	}

	EditorObject* Editor::createObject(ObjectType type, const glm::vec3& position) {
		auto obj = std::make_unique<EditorObject>(nextObjectId_++, type, 
			Vec3(position.x, position.y, position.z),
			Vec3(1.0f, 1.0f, 1.0f));
		obj->generateMesh();
		
		EditorObject* ptr = obj.get();
		objects_.push_back(std::move(obj));
		
		printf("Created object %d of type %d at (%.2f, %.2f, %.2f)\n", 
			ptr->id, (int)type, position.x, position.y, position.z);
		
		return ptr;
	}

	void Editor::deleteObject(int id) {
		auto it = std::find_if(objects_.begin(), objects_.end(),
			[id](const std::unique_ptr<EditorObject>& obj) { return obj->id == id; });
		
		if (it != objects_.end()) {
			if (selectedObject_ && selectedObject_->id == id) {
				selectedObject_ = nullptr;
			}
			objects_.erase(it);
			printf("Deleted object %d\n", id);
		}
	}

	void Editor::resetCamera() {
		if (editorCamera_) {
			editorCamera_->setPosition(Vec3(0.0f, 2.0f, 5.0f));
			editorCamera_->rotate(0.0f, 0.0f);
		}
	}

	bool Editor::saveScene(const std::string& filename) {
		printf("Saving scene to %s\n", filename.c_str());
		return true;
	}

	bool Editor::loadScene(const std::string& filename) {
		printf("Loading scene from %s\n", filename.c_str());
		return true;
	}

	void Editor::renderMainMenuBar() {
	}

	void Editor::renderHierarchyPanel() {
	}

	void Editor::renderPropertiesPanel() {
	}

	void Editor::renderViewport() {
	}

	void Editor::renderToolbar() {
	}

	void Editor::renderGizmos() {
	}

	void Editor::updateGizmoInteraction() {
	}

}
