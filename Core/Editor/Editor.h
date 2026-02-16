#pragma once

#include <vector>
#include <memory>
#include <string>
#include "EditorObject.h"
#include "../Render/Mesh.h"
#include "../Math/Vector.h"
#include <glm/glm.hpp>

namespace Bound {

	class GLRenderer;
	class Camera;
	class EditorUI;

	/**
	 * Editor - Main editor UI and scene management
	 * Runs in the engine when not in play mode
	 * Provides scene manipulation, object editing, and viewport preview
	 */
	class Editor {
	public:
		Editor();
		~Editor();

	// Initialization
		void initialize(GLRenderer* renderer, class SDLWindow* window);
		void shutdown();

		// Editor state
		bool isActive() const { return active_; }
		void toggleActive() { active_ = !active_; }
		bool isPlayMode() const { return playMode_; }
		void setPlayMode(bool play) { playMode_ = play; }

		// Main update/render loop
		void update(float deltaTime);
		void render();
		void handleInput();

		// Scene management
		EditorObject* createObject(ObjectType type, const glm::vec3& position);
		void deleteObject(int id);
		EditorObject* getSelectedObject() const { return selectedObject_; }
		void selectObject(EditorObject* obj) { selectedObject_ = obj; }
		const std::vector<std::unique_ptr<EditorObject>>& getObjects() const { return objects_; }

		// Camera control
		Camera* getEditorCamera() { return editorCamera_; }
		void resetCamera();

		// File operations
		bool saveScene(const std::string& filename);
		bool loadScene(const std::string& filename);

	private:
		bool active_;
		bool playMode_;
		GLRenderer* renderer_;
		Camera* editorCamera_;

		// Scene data
		std::vector<std::unique_ptr<EditorObject>> objects_;
		EditorObject* selectedObject_;
		int nextObjectId_;

		// UI state
		float gizmoScale_;
		bool showGrid_;
		bool showHierarchy_;
		bool showProperties_;
		bool showViewport_;

		// UI Managers
		std::unique_ptr<class ImGuiManager> imguiManager_;
		std::unique_ptr<EditorUI> ui_;

		// UI rendering
		void renderMainMenuBar();
		void renderHierarchyPanel();
		void renderPropertiesPanel();
		void renderViewport();
		void renderToolbar();

		// Gizmo operations
		void renderGizmos();
		void updateGizmoInteraction();
	};

}
