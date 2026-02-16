#pragma once

#include <string>

namespace Bound {

	class Editor;

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

		// Panel rendering
		void renderMenuBar(Editor* editor);
		void renderToolbar(Editor* editor);
		void renderHierarchyPanel(Editor* editor);
		void renderPropertiesPanel(Editor* editor);
		void renderViewport(Editor* editor);
		void renderStatusBar(Editor* editor);
	};

}
