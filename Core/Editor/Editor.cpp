#include "Editor.h"
#include <windows.h>

namespace Bound {

	Editor::Editor()
		: active_(false) {
		OutputDebugStringA("=== Editor constructor ===\n");
	}

	Editor::~Editor() {
		OutputDebugStringA("=== Editor destructor ===\n");
	}

}
