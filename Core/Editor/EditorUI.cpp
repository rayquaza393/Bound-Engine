#include "EditorUI.h"
#include "IconManager.h"
#include "Editor.h"
#include "../Render/GLRenderer.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <vector>
#include <cstdarg>

namespace Bound {

	// Simple debug console
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

			if (ImGui::Button("Clear")) {
				logs_.clear();
			}
			ImGui::SameLine();
			ImGui::Text("Logs: %zu", logs_.size());
			ImGui::Separator();
			
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -30));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

			for (const auto& log : logs_) {
				ImGui::TextUnformatted(log.c_str());
			}

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

	static DebugConsole g_console;

	EditorUI::EditorUI()
		: showDemoWindow_(false), 
		  hierarchyWidth_(250.0f),
		  propertiesWidth_(250.0f),
		  consoleHeight_(120.0f),
		  iconManager_(std::make_unique<IconManager>()),
		  iconsLoaded_(false) {
		ImGui::StyleColorsLight();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 4.0f;
		style.FrameRounding = 3.0f;
		style.GrabRounding = 2.0f;
		
		ImVec4 bgColor = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
		bgColor.w = 1.0f;  // opacity
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = bgColor;

		g_console.addLog("=== Bound Engine Editor ===");
		g_console.addLog("Ready to create objects!");
	}

	EditorUI::~EditorUI() {
	}

	void EditorUI::loadIcons() {
		if (iconsLoaded_) return;
		iconManager_->loadIcon("move", "Core/Assets/Icons/move.png");
		iconManager_->loadIcon("rotate", "Core/Assets/Icons/rotate.png");
		iconManager_->loadIcon("scale", "Core/Assets/Icons/resize.png");
		iconManager_->loadIcon("add", "Core/Assets/Icons/add.png");
		iconManager_->loadIcon("select", "Core/Assets/Icons/select.png");
		iconManager_->loadIcon("save", "Core/Assets/Icons/save.png");
		iconManager_->loadIcon("delete", "Core/Assets/Icons/delete.png");
		iconsLoaded_ = true;
	}

	void EditorUI::render(Editor* editor) {
		if (!iconsLoaded_) {
			loadIcons();
		}

		ImGuiIO& io = ImGui::GetIO();
		ImVec2 viewportSize = io.DisplaySize;

		float toolbarHeight = 50;
		float toolbarY = 20;
		float statusBarHeight = 20;
		float topEdge = toolbarY + toolbarHeight;
		float bottomEdge = viewportSize.y - statusBarHeight;
		float totalWidth = viewportSize.x;
		float totalHeight = bottomEdge - topEdge;
		
		const float minHierarchyWidth = 150;
		const float minPropertiesWidth = 150;
		const float minViewportWidth = 300;
		const float minMiddleHeight = 200;
		const float minConsoleHeight = 80;
		
		hierarchyWidth_ = glm::max(minHierarchyWidth, glm::min(hierarchyWidth_, totalWidth - minPropertiesWidth - minViewportWidth));
		propertiesWidth_ = glm::max(minPropertiesWidth, glm::min(propertiesWidth_, totalWidth - minHierarchyWidth - minViewportWidth));
		consoleHeight_ = glm::max(minConsoleHeight, glm::min(consoleHeight_, totalHeight - minMiddleHeight));
		
		float middleHeight = totalHeight - consoleHeight_;
		float consoleY = topEdge + middleHeight;
		float viewportWidth = totalWidth - hierarchyWidth_ - propertiesWidth_;
		
		renderMenuBar(editor);
		renderToolbar(editor);

		// Hierarchy
		ImGui::SetNextWindowPos(ImVec2(0, topEdge), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(hierarchyWidth_, middleHeight), ImGuiCond_Always);
		renderHierarchyPanel(editor);

		// Properties
		ImGui::SetNextWindowPos(ImVec2(totalWidth - propertiesWidth_, topEdge), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(propertiesWidth_, middleHeight), ImGuiCond_Always);
		renderPropertiesPanel(editor);

		// Viewport
		ImGui::SetNextWindowPos(ImVec2(hierarchyWidth_, topEdge), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(viewportWidth, middleHeight), ImGuiCond_Always);
		renderViewport(editor);

		// Console
		ImGui::SetNextWindowPos(ImVec2(0, consoleY), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(totalWidth, consoleHeight_), ImGuiCond_Always);
		g_console.draw("Debug Console##main", nullptr);

		// Status bar
		renderStatusBar(editor, bottomEdge, viewportSize.x);
	}

	void EditorUI::renderMenuBar(Editor* editor) {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
					editor->clearScene();
					g_console.addLog("New scene created");
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
					if (editor->loadScene("scene.scne")) {
						g_console.addLog("Scene loaded successfully");
					} else {
						g_console.addLog("Failed to load scene");
					}
				}
				if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
					if (editor->saveScene("scene.scne")) {
						g_console.addLog("Scene saved successfully");
					} else {
						g_console.addLog("Failed to save scene");
					}
				}
				ImGui::Separator();
				if (ImGui::BeginMenu("Import")) {
					if (ImGui::MenuItem("Import Model")) {
						g_console.addLog("[TODO] Import model dialog");
					}
					if (ImGui::MenuItem("Import Texture")) {
						g_console.addLog("[TODO] Import texture dialog");
					}
					if (ImGui::MenuItem("Import Asset")) {
						g_console.addLog("[TODO] Import asset dialog");
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Export")) {
					if (ImGui::MenuItem("Export Model")) {
						g_console.addLog("[TODO] Export model dialog");
					}
					if (ImGui::MenuItem("Export Scene")) {
						g_console.addLog("[TODO] Export scene dialog");
					}
					if (ImGui::MenuItem("Export As Prefab")) {
						g_console.addLog("[TODO] Export as prefab dialog");
					}
					ImGui::EndMenu();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit", "Alt+F4")) {
					g_console.addLog("Exiting application...");
					editor->requestExit();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit")) {
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

			ImGui::EndMainMenuBar();
		}
	}

	void EditorUI::renderToolbar(Editor* editor) {
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, 50), ImGuiCond_Always);
		
		if (ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
			const float iconSize = 32.0f;
			
			// Transform tools with icons
			ImGui::Text("Transform:");
			ImGui::SameLine();
			
			// Select tool
			if (iconManager_->hasIcon("select")) {
				ImGui::PushID("select_tool");
				if (ImGui::ImageButton("##select", (ImTextureID)(intptr_t)iconManager_->getIconTexture("select"), ImVec2(iconSize, iconSize))) {
					g_console.addLog("Select tool active");
				}
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select (1)");
				ImGui::PopID();
			} else {
				if (ImGui::Button("Select", ImVec2(60, 0))) {
					g_console.addLog("Select tool active");
				}
			}
			ImGui::SameLine();
			
			// Move tool
			if (iconManager_->hasIcon("move")) {
				ImGui::PushID("move_tool");
				if (ImGui::ImageButton("##move", (ImTextureID)(intptr_t)iconManager_->getIconTexture("move"), ImVec2(iconSize, iconSize))) {
					g_console.addLog("Move tool selected");
				}
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Move (2)");
				ImGui::PopID();
			} else {
				if (ImGui::Button("Move", ImVec2(50, 0))) {
					g_console.addLog("Move tool selected");
				}
			}
			ImGui::SameLine();
			
			// Rotate tool
			if (iconManager_->hasIcon("rotate")) {
				ImGui::PushID("rotate_tool");
				if (ImGui::ImageButton("##rotate", (ImTextureID)(intptr_t)iconManager_->getIconTexture("rotate"), ImVec2(iconSize, iconSize))) {
					g_console.addLog("Rotate tool selected");
				}
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Rotate (3)");
				ImGui::PopID();
			} else {
				if (ImGui::Button("Rotate", ImVec2(60, 0))) {
					g_console.addLog("Rotate tool selected");
				}
			}
			ImGui::SameLine();
			
			// Scale tool
			if (iconManager_->hasIcon("scale")) {
				ImGui::PushID("scale_tool");
				if (ImGui::ImageButton("##scale", (ImTextureID)(intptr_t)iconManager_->getIconTexture("scale"), ImVec2(iconSize, iconSize))) {
					g_console.addLog("Scale tool selected");
				}
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Scale (4)");
				ImGui::PopID();
			} else {
				if (ImGui::Button("Scale", ImVec2(50, 0))) {
					g_console.addLog("Scale tool selected");
				}
			}
			
			ImGui::SameLine(300);
			ImGui::Text("File:");
			ImGui::SameLine();
			
			// Save tool
			if (iconManager_->hasIcon("save")) {
				ImGui::PushID("save_tool");
				if (ImGui::ImageButton("##save", (ImTextureID)(intptr_t)iconManager_->getIconTexture("save"), ImVec2(iconSize, iconSize))) {
					if (editor->saveScene("scene.scne")) {
						g_console.addLog("Scene saved successfully");
					} else {
						g_console.addLog("Failed to save scene");
					}
				}
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save Scene (Ctrl+S)");
				ImGui::PopID();
			} else {
				if (ImGui::Button("Save", ImVec2(50, 0))) {
					if (editor->saveScene("scene.scne")) {
						g_console.addLog("Scene saved successfully");
					} else {
						g_console.addLog("Failed to save scene");
					}
				}
			}
			ImGui::SameLine();
			
			// Delete tool
			if (iconManager_->hasIcon("delete")) {
				ImGui::PushID("delete_tool");
				if (ImGui::ImageButton("##delete", (ImTextureID)(intptr_t)iconManager_->getIconTexture("delete"), ImVec2(iconSize, iconSize))) {
					if (auto selected = editor->getSelectedObject()) {
						g_console.addLog("Deleted Object %d", selected->id);
						editor->deleteObject(selected->id);
					} else {
						g_console.addLog("No object selected to delete");
					}
				}
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete Selected (Del)");
				ImGui::PopID();
			} else {
				if (ImGui::Button("Delete", ImVec2(60, 0))) {
					if (auto selected = editor->getSelectedObject()) {
						g_console.addLog("Deleted Object %d", selected->id);
						editor->deleteObject(selected->id);
					} else {
						g_console.addLog("No object selected to delete");
					}
				}
			}
			
			ImGui::SameLine(600);
			ImGui::Text("Create:");
			ImGui::SameLine();
			
			// Create object dropdown combo
			static const char* object_types[] = { "Cube", "Pyramid", "Plane" };
			static int object_current = -1;
			
			ImGui::SetNextItemWidth(100);
			if (ImGui::Combo("##CreateObject", &object_current, object_types, IM_ARRAYSIZE(object_types))) {
				switch (object_current) {
					case 0:  // Cube
						editor->createObject(ObjectType::Cube, glm::vec3(0, 0, 0));
						g_console.addLog("Created Cube");
						break;
					case 1:  // Pyramid
						editor->createObject(ObjectType::Pyramid, glm::vec3(0, 0, 0));
						g_console.addLog("Created Pyramid");
						break;
					case 2:  // Plane
						editor->createObject(ObjectType::Floor, glm::vec3(0, -2, 0));
						g_console.addLog("Created Plane");
						break;
				}
				object_current = -1;  // Reset combo
			}
		}
		ImGui::End();
	}

	void EditorUI::renderHierarchyPanel(Editor* editor) {
		if (ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoMove)) {
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
		}
		ImGui::End();
	}

	void EditorUI::renderPropertiesPanel(Editor* editor) {
		if (ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoMove)) {
			if (auto selected = editor->getSelectedObject()) {
				ImGui::Text("Object %d Properties", selected->id);
				ImGui::Separator();

				// Position with up/down spinners
				ImGui::Text("Position");
				ImGui::PushItemWidth(-40.0f);
				ImGui::InputFloat("##PosX", &selected->position.x, 0.1f, 0.5f);
				ImGui::SameLine();
				if (ImGui::Button("^##PosXUp", ImVec2(18, 0))) selected->position.x += 0.1f;
				ImGui::SameLine();
				if (ImGui::Button("v##PosXDown", ImVec2(18, 0))) selected->position.x -= 0.1f;
				
				ImGui::InputFloat("##PosY", &selected->position.y, 0.1f, 0.5f);
				ImGui::SameLine();
				if (ImGui::Button("^##PosYUp", ImVec2(18, 0))) selected->position.y += 0.1f;
				ImGui::SameLine();
				if (ImGui::Button("v##PosYDown", ImVec2(18, 0))) selected->position.y -= 0.1f;
				
				ImGui::InputFloat("##PosZ", &selected->position.z, 0.1f, 0.5f);
				ImGui::SameLine();
				if (ImGui::Button("^##PosZUp", ImVec2(18, 0))) selected->position.z += 0.1f;
				ImGui::SameLine();
				if (ImGui::Button("v##PosZDown", ImVec2(18, 0))) selected->position.z -= 0.1f;
				ImGui::PopItemWidth();

				ImGui::Separator();

				// Rotation with up/down spinners (in degrees)
				ImGui::Text("Rotation (degrees)");
				float rotX = glm::degrees(selected->rotation.x);
				float rotY = glm::degrees(selected->rotation.y);
				float rotZ = glm::degrees(selected->rotation.z);

				ImGui::PushItemWidth(-40.0f);
				if (ImGui::InputFloat("##RotX", &rotX, 1.0f, 5.0f)) {
					selected->rotation.x = glm::radians(rotX);
				}
				ImGui::SameLine();
				if (ImGui::Button("^##RotXUp", ImVec2(18, 0))) rotX += 5.0f, selected->rotation.x = glm::radians(rotX);
				ImGui::SameLine();
				if (ImGui::Button("v##RotXDown", ImVec2(18, 0))) rotX -= 5.0f, selected->rotation.x = glm::radians(rotX);
				
				if (ImGui::InputFloat("##RotY", &rotY, 1.0f, 5.0f)) {
					selected->rotation.y = glm::radians(rotY);
				}
				ImGui::SameLine();
				if (ImGui::Button("^##RotYUp", ImVec2(18, 0))) rotY += 5.0f, selected->rotation.y = glm::radians(rotY);
				ImGui::SameLine();
				if (ImGui::Button("v##RotYDown", ImVec2(18, 0))) rotY -= 5.0f, selected->rotation.y = glm::radians(rotY);
				
				if (ImGui::InputFloat("##RotZ", &rotZ, 1.0f, 5.0f)) {
					selected->rotation.z = glm::radians(rotZ);
				}
				ImGui::SameLine();
				if (ImGui::Button("^##RotZUp", ImVec2(18, 0))) rotZ += 5.0f, selected->rotation.z = glm::radians(rotZ);
				ImGui::SameLine();
				if (ImGui::Button("v##RotZDown", ImVec2(18, 0))) rotZ -= 5.0f, selected->rotation.z = glm::radians(rotZ);
				ImGui::PopItemWidth();

				ImGui::Separator();

				// Scale with up/down spinners
				ImGui::Text("Scale");
				ImGui::PushItemWidth(-40.0f);
				ImGui::InputFloat("##ScaleX", &selected->scale.x, 0.1f, 0.5f);
				ImGui::SameLine();
				if (ImGui::Button("^##ScaleXUp", ImVec2(18, 0))) selected->scale.x += 0.1f;
				ImGui::SameLine();
				if (ImGui::Button("v##ScaleXDown", ImVec2(18, 0))) selected->scale.x = glm::max(0.1f, selected->scale.x - 0.1f);
				
				ImGui::InputFloat("##ScaleY", &selected->scale.y, 0.1f, 0.5f);
				ImGui::SameLine();
				if (ImGui::Button("^##ScaleYUp", ImVec2(18, 0))) selected->scale.y += 0.1f;
				ImGui::SameLine();
				if (ImGui::Button("v##ScaleYDown", ImVec2(18, 0))) selected->scale.y = glm::max(0.1f, selected->scale.y - 0.1f);
				
				ImGui::InputFloat("##ScaleZ", &selected->scale.z, 0.1f, 0.5f);
				ImGui::SameLine();
				if (ImGui::Button("^##ScaleZUp", ImVec2(18, 0))) selected->scale.z += 0.1f;
				ImGui::SameLine();
				if (ImGui::Button("v##ScaleZDown", ImVec2(18, 0))) selected->scale.z = glm::max(0.1f, selected->scale.z - 0.1f);
				ImGui::PopItemWidth();

				ImGui::Separator();

				// Color
				ImGui::Text("Color");
				float oldColor[3] = { selected->color.x, selected->color.y, selected->color.z };
				if (ImGui::ColorEdit3("##color", &selected->color.x)) {
					// Only update mesh if color actually changed
					if (oldColor[0] != selected->color.x || 
					    oldColor[1] != selected->color.y || 
					    oldColor[2] != selected->color.z) {
						selected->updateMeshColor();
					}
				}
			} else {
				ImGui::Text("No object selected");
			}
		}
		ImGui::End();
	}

	void EditorUI::renderViewport(Editor* editor) {
		if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoMove)) {
			ImGui::Text("3D Scene Viewport");
			ImGui::Text("WASD = Move | Right-Click = Rotate | Left-Click = Select");
			ImGui::Text("Objects in Scene: %zu", editor->getObjects().size());
			
			if (auto selected = editor->getSelectedObject()) {
				ImGui::Separator();
				ImGui::Text("Selected: Object %d", selected->id);
				ImGui::Text("Position: (%.2f, %.2f, %.2f)", 
					selected->position.x, selected->position.y, selected->position.z);
			}
		}
		ImGui::End();
	}

	void EditorUI::renderStatusBar(Editor* editor, float yPosition, float width) {
		ImGui::SetNextWindowPos(ImVec2(0, yPosition), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(width, 20), ImGuiCond_Always);

		if (ImGui::Begin("##StatusBar", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
			ImGui::Text("Objects: %zu | FPS: %.1f", 
				editor->getObjects().size(), 
				ImGui::GetIO().Framerate);
		}
		ImGui::End();
	}

}
