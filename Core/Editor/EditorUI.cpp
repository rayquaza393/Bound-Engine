#include "EditorUI.h"
#include "Editor.h"
#include <imgui.h>
#include <vector>
#include <cstdarg>

namespace Bound {

	// Simple debug console with vector-based logging
	class DebugConsole {
	public:
		static const int MAX_LOGS = 256;

		void addLog(const char* fmt, ...) {
			va_list args;
			va_start(args, fmt);
			addLogV(fmt, args);
			va_end(args);
		}

		void addLogV(const char* fmt, va_list args) {
			char buf[256];
			vsnprintf(buf, sizeof(buf), fmt, args);
			logs_.push_back(std::string(buf));
			if (logs_.size() > MAX_LOGS) {
				logs_.erase(logs_.begin());
			}
			autoScroll_ = true;
		}

		void draw(const char* title, bool* open = nullptr) {
			if (!ImGui::Begin(title, open)) {
				ImGui::End();
				return;
			}

			// Always show controls and content
			if (ImGui::Button("Clear")) {
				logs_.clear();
			}
			ImGui::SameLine();
			ImGui::Text("Logs: %zu", logs_.size());

			ImGui::Separator();
			
			// Create scrolling region with fixed height for content
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -30));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

			// Display all logs in order
			for (const auto& log : logs_) {
				ImGui::TextUnformatted(log.c_str());
			}

			// Auto-scroll to bottom when new logs are added
			if (autoScroll_) {
				ImGui::SetScrollHereY(1.0f);
				autoScroll_ = false;
			}

			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::End();
		}

	private:
		std::vector<std::string> logs_;
		bool autoScroll_;
	};

	// Global console instance
	static DebugConsole g_console;

	EditorUI::EditorUI()
		: showDemoWindow_(false) {
		// Set light theme
		ImGui::StyleColorsLight();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 4.0f;
		style.FrameRounding = 3.0f;
		style.GrabRounding = 2.0f;

		// Add some logs
		g_console.addLog("=== Bound Engine Editor ===");
		g_console.addLog("Ready to create objects!");
	}

	EditorUI::~EditorUI() {
	}

	void EditorUI::render(Editor* editor) {
		// Get window dimensions
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 viewportSize = io.DisplaySize;

		// Menu bar
		renderMenuBar(editor);

		// Toolbar
		renderToolbar(editor);

		// CONSTRAIN WINDOWS - Calculate safe areas with boundaries
		float leftEdge = 0;
		float rightEdge = viewportSize.x - 250;  // Reserve 250px for right panel
		float topEdge = 70;  // Below toolbar
		float bottomEdge = 470;  // Above console
		float centerLeft = 250;
		float centerWidth = rightEdge - centerLeft;

		// Hierarchy panel (left) - CONSTRAINED
		ImGui::SetNextWindowPos(ImVec2(leftEdge, topEdge), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(250, bottomEdge - topEdge), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSizeConstraints(ImVec2(150, 200), ImVec2(400, 500));
		renderHierarchyPanel(editor);

		// Properties panel (right) - CONSTRAINED
		ImGui::SetNextWindowPos(ImVec2(rightEdge, topEdge), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(250, bottomEdge - topEdge), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSizeConstraints(ImVec2(150, 200), ImVec2(400, 500));
		renderPropertiesPanel(editor);

		// Viewport (center) - CONSTRAINED - PRIMARY 3D VIEW
		ImGui::SetNextWindowPos(ImVec2(centerLeft, topEdge), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(centerWidth, bottomEdge - topEdge), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSizeConstraints(ImVec2(300, 200), ImVec2(800, 500));
		renderViewport(editor);

		// Debug console (bottom) - CONSTRAINED - ALWAYS VISIBLE
		ImGui::SetNextWindowPos(ImVec2(0, 470), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(viewportSize.x, viewportSize.y - 490), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSizeConstraints(ImVec2(300, 100), ImVec2(viewportSize.x, viewportSize.y - 100));
		g_console.draw("Debug Console##main", nullptr);

		// Status bar (very bottom) - ALWAYS VISIBLE
		renderStatusBar(editor);

		// Demo windows for reference
		// if (showDemoWindow_) {
		//	ImGui::ShowDemoWindow(&showDemoWindow_);
		// }
	}

	void EditorUI::renderMenuBar(Editor* editor) {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
					// TODO: New scene
				}
				if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
					// TODO: Open scene
				}
				if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
					editor->saveScene("untitled.bound");
					g_console.addLog("Scene saved");
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit")) {
					// TODO: Exit app
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
					// TODO: Undo
				}
				if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
					// TODO: Redo
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Delete", "Delete")) {
					if (auto selected = editor->getSelectedObject()) {
						g_console.addLog("Deleted Object %d", selected->id);
						editor->deleteObject(selected->id);
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View")) {
				if (ImGui::MenuItem("Reset Camera")) {
					editor->resetCamera();
					g_console.addLog("Camera reset");
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help")) {
				if (ImGui::MenuItem("ImGui Demo")) {
					showDemoWindow_ = !showDemoWindow_;
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void EditorUI::renderToolbar(Editor* editor) {
		ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(1280, 50), ImGuiCond_FirstUseEver);
		
		if (ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {
			ImGui::Text("Transform:");
			ImGui::SameLine();
			if (ImGui::Button("Move", ImVec2(50, 0))) {}
			ImGui::SameLine();
			if (ImGui::Button("Rotate", ImVec2(60, 0))) {}
			ImGui::SameLine();
			if (ImGui::Button("Scale", ImVec2(50, 0))) {}

			ImGui::SameLine(300);
			ImGui::Text("Create:");
			ImGui::SameLine();
			if (ImGui::Button("Cube", ImVec2(60, 0))) {
				editor->createObject(ObjectType::Cube, glm::vec3(0, 0, 0));
				g_console.addLog("Created Cube");
			}
			ImGui::SameLine();
			if (ImGui::Button("Pyramid", ImVec2(70, 0))) {
				editor->createObject(ObjectType::Pyramid, glm::vec3(0, 0, 0));
				g_console.addLog("Created Pyramid");
			}
			ImGui::SameLine();
			if (ImGui::Button("Plane", ImVec2(60, 0))) {
				editor->createObject(ObjectType::Floor, glm::vec3(0, -2, 0));
				g_console.addLog("Created Plane");
			}
		}
		ImGui::End();
	}

	void EditorUI::renderHierarchyPanel(Editor* editor) {
		ImGui::SetNextWindowPos(ImVec2(0, 70), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(250, 400), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Hierarchy", nullptr)) {
			ImGui::Text("Scene Objects (%zu)", editor->getObjects().size());
			ImGui::Separator();

			const auto& objects = editor->getObjects();
			for (const auto& obj : objects) {
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				if (obj.get() == editor->getSelectedObject()) {
					flags |= ImGuiTreeNodeFlags_Selected;
				}

				const char* type_str = 
					obj->type == ObjectType::Cube ? "Cube" :
					obj->type == ObjectType::Pyramid ? "Pyramid" : "Floor";

				ImGui::TreeNodeEx((void*)(intptr_t)obj->id, flags, "%s##%d", type_str, obj->id);

				if (ImGui::IsItemClicked()) {
					editor->selectObject(obj.get());
					g_console.addLog("Selected %s %d", type_str, obj->id);
				}
			}

			ImGui::Separator();
			if (ImGui::Button("Delete Selected", ImVec2(-1, 0))) {
				if (auto selected = editor->getSelectedObject()) {
					g_console.addLog("Deleted Object %d", selected->id);
					editor->deleteObject(selected->id);
				}
			}
		}
		ImGui::End();
	}

	void EditorUI::renderPropertiesPanel(Editor* editor) {
		ImGui::SetNextWindowPos(ImVec2(1030, 70), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(250, 400), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Properties")) {
			if (auto selected = editor->getSelectedObject()) {
				ImGui::Text("Object %d Properties", selected->id);
				ImGui::Separator();

				// Position
				ImGui::Text("Position");
				ImGui::DragFloat("X##pos", &selected->position.x, 0.1f);
				ImGui::DragFloat("Y##pos", &selected->position.y, 0.1f);
				ImGui::DragFloat("Z##pos", &selected->position.z, 0.1f);

				ImGui::Separator();

				// Rotation (in degrees)
				ImGui::Text("Rotation");
				float rotX = glm::degrees(selected->rotation.x);
				float rotY = glm::degrees(selected->rotation.y);
				float rotZ = glm::degrees(selected->rotation.z);

				if (ImGui::DragFloat("X##rot", &rotX, 0.5f)) {
					selected->rotation.x = glm::radians(rotX);
				}
				if (ImGui::DragFloat("Y##rot", &rotY, 0.5f)) {
					selected->rotation.y = glm::radians(rotY);
				}
				if (ImGui::DragFloat("Z##rot", &rotZ, 0.5f)) {
					selected->rotation.z = glm::radians(rotZ);
				}

				ImGui::Separator();

				// Scale
				ImGui::Text("Scale");
				ImGui::DragFloat("X##scale", &selected->scale.x, 0.1f, 0.1f, 10.0f);
				ImGui::DragFloat("Y##scale", &selected->scale.y, 0.1f, 0.1f, 10.0f);
				ImGui::DragFloat("Z##scale", &selected->scale.z, 0.1f, 0.1f, 10.0f);

				ImGui::Separator();

				// Color
				ImGui::Text("Color");
				ImGui::ColorEdit3("##color", &selected->color.x);
			} else {
				ImGui::Text("No object selected");
				ImGui::Text("Select an object from the Hierarchy");
			}
		}
		ImGui::End();
	}

	void EditorUI::renderViewport(Editor* editor) {
		ImGui::SetNextWindowPos(ImVec2(250, 70), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(780, 400), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Viewport")) {
			// Show viewport info
			ImGui::Text("3D Scene Viewport");
			ImGui::Text("WASD = Move | Right-Click = Rotate | Scroll = Zoom");
			ImGui::Separator();

			// Scene statistics
			ImGui::Text("Objects in Scene: %zu", editor->getObjects().size());
			if (auto selected = editor->getSelectedObject()) {
				ImGui::Text("Selected: %d", selected->id);
				ImGui::Text("Position: (%.2f, %.2f, %.2f)", 
					selected->position.x, selected->position.y, selected->position.z);
				ImGui::Text("Scale: (%.2f, %.2f, %.2f)", 
					selected->scale.x, selected->scale.y, selected->scale.z);
			} else {
				ImGui::Text("Selected: None");
			}

			ImGui::Separator();

			// Camera info
			if (auto camera = editor->getEditorCamera()) {
				ImGui::Text("Camera Controls:");
				ImGui::BulletText("WASD - Move forward/back/left/right");
				ImGui::BulletText("Mouse Right-Click - Look around");
				ImGui::BulletText("Scroll - Change FOV");
			}

			ImGui::Separator();
			ImGui::TextWrapped("The 3D scene is rendered in the viewport above. "
				"Create objects from the toolbar, select them in the hierarchy, "
				"and modify their properties on the right panel.");

			// Show available space for rendering
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			ImGui::Text("Viewport Size: %.0fx%.0f", viewportSize.x, viewportSize.y);
		}
		ImGui::End();
	}

	void EditorUI::renderStatusBar(Editor* editor) {
		ImGui::SetNextWindowPos(ImVec2(0, 700), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(1280, 20), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("##StatusBar", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
			ImGui::Text("Objects: %zu | FPS: %.1f | Mode: %s", 
				editor->getObjects().size(), 
				ImGui::GetIO().Framerate,
				editor->isPlayMode() ? "PLAY" : "EDIT");
		}
		ImGui::End();
	}

}
