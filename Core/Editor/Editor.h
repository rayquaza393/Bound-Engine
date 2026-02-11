#pragma once

namespace Bound {

	class Editor {
	public:
		Editor();
		~Editor();

		// Editor mode state
		bool isActive() const { return active_; }
		void toggleActive() { active_ = !active_; }

	private:
		bool active_;
	};

}
