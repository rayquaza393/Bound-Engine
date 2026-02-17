#pragma once

#include <string>
#include <memory>

namespace Bound {

	class Editor;
	class IconManager;

	/**
	 * EditorUI - Handles all ImGui UI rendering for the editor
	 * Implements Roblox Studio 2013-style layout
	 */
	class EditorUI {
	public:
		EditorUI();
		~EditorUI();

		void render(Editor* editor);

	private:
		// UI state
		bool showDemoWindow_;
		float hierarchyWidth_;   // Width of hierarchy panel (left side, resizable from right edge)
		float propertiesWidth_;  // Width of properties panel (right side, resizable from left edge)
		float consoleHeight_;    // Height of console panel (bottom, resizable from top edge)
		
		// Icon management
		std::unique_ptr<IconManager> iconManager_;
		bool iconsLoaded_;

		// Panel rendering
		void renderMenuBar(Editor* editor);
		void renderToolbar(Editor* editor);
		void renderHierarchyPanel(Editor* editor);
		void renderPropertiesPanel(Editor* editor);
		void renderViewport(Editor* editor);
		void renderStatusBar(Editor* editor, float yPosition, float width);

		// Icon loading
		void loadIcons();
	};

}
