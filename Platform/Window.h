#pragma once

#include <windows.h>
#include "../Core/Render/Framebuffer.h"

namespace Bound {

	class Window {
	public:
		Window(const char* title, int width, int height);
		~Window();

		// Window management
		bool isOpen() const { return isOpen_; }
		void close() { isOpen_ = false; }
		void update();

		// Display framebuffer
		void present(const Framebuffer* framebuffer);

		// Input
		bool isKeyPressed(int key) const;
		bool isMouseButtonPressed(int button) const;
		void getMousePosition(int& x, int& y) const;

		// Getters
		int getWidth() const { return width_; }
		int getHeight() const { return height_; }
		HWND getNativeWindow() { return hwnd_; }

	private:
		HWND hwnd_;
		HDC hdc_;
		HBITMAP hbitmap_;
		int width_;
		int height_;
		bool isOpen_;

		// Input state
		bool keyState_[256];
		int mouseX_, mouseY_;
		bool mouseButtonState_[3];

		static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};

}
