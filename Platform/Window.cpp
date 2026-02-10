#include "Window.h"
#include <windowsx.h>

namespace Bound {

	static Window* g_windowInstance = nullptr;

	LRESULT CALLBACK Window::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (g_windowInstance) {
			switch (msg) {
				case WM_CLOSE:
					g_windowInstance->close();
					return 0;
				case WM_KEYDOWN:
					if (wparam < 256) {
						g_windowInstance->keyState_[wparam] = true;
					}
					if (wparam == VK_ESCAPE) {
						g_windowInstance->close();
					}
					return 0;
				case WM_KEYUP:
					if (wparam < 256) {
						g_windowInstance->keyState_[wparam] = false;
					}
					return 0;
				case WM_MOUSEMOVE:
					g_windowInstance->mouseX_ = GET_X_LPARAM(lparam);
					g_windowInstance->mouseY_ = GET_Y_LPARAM(lparam);
					return 0;
				case WM_LBUTTONDOWN:
					g_windowInstance->mouseButtonState_[0] = true;
					return 0;
				case WM_LBUTTONUP:
					g_windowInstance->mouseButtonState_[0] = false;
					return 0;
				case WM_RBUTTONDOWN:
					g_windowInstance->mouseButtonState_[1] = true;
					return 0;
				case WM_RBUTTONUP:
					g_windowInstance->mouseButtonState_[1] = false;
					return 0;
			}
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	Window::Window(const char* title, int width, int height)
		: hwnd_(nullptr), hdc_(nullptr), hbitmap_(nullptr),
		  width_(width), height_(height), isOpen_(true),
		  mouseX_(0), mouseY_(0) {

		g_windowInstance = this;
		memset(keyState_, 0, sizeof(keyState_));
		memset(mouseButtonState_, 0, sizeof(mouseButtonState_));

		// Register window class
		WNDCLASSA wndClass = {};
		wndClass.lpfnWndProc = WindowProcedure;
		wndClass.hInstance = GetModuleHandle(nullptr);
		wndClass.lpszClassName = "BoundEngineWindow";
		wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

		RegisterClassA(&wndClass);

		// Create window
		hwnd_ = CreateWindowA(
			"BoundEngineWindow",
			title,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			nullptr, nullptr,
			GetModuleHandle(nullptr),
			nullptr
		);

		// Get device context
		hdc_ = GetDC(hwnd_);

		// Create a bitmap for double buffering
		hbitmap_ = CreateCompatibleBitmap(hdc_, width, height);

		// Update window size if needed (account for borders)
		RECT rect;
		GetClientRect(hwnd_, &rect);
		width_ = rect.right - rect.left;
		height_ = rect.bottom - rect.top;
	}

	Window::~Window() {
		if (hbitmap_) DeleteObject(hbitmap_);
		if (hdc_) ReleaseDC(hwnd_, hdc_);
		if (hwnd_) DestroyWindow(hwnd_);
		UnregisterClassA("BoundEngineWindow", GetModuleHandle(nullptr));
	}

	void Window::update() {
		MSG msg = {};
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				isOpen_ = false;
				return;
			}
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	void Window::present(const Framebuffer* framebuffer) {
		// Create a device context for the framebuffer
		HDC memdc = CreateCompatibleDC(hdc_);
		SelectObject(memdc, hbitmap_);

		// Convert framebuffer to DIB (Device Independent Bitmap)
		const uint32_t* pixels = framebuffer->getPixels();
		
		BITMAPINFO bmi = {};
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = framebuffer->getWidth();
		bmi.bmiHeader.biHeight = -framebuffer->getHeight(); // Negative for top-down
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;

		// Set pixels
		SetDIBitsToDevice(
			hdc_,
			0, 0,
			framebuffer->getWidth(),
			framebuffer->getHeight(),
			0, 0,
			0,
			framebuffer->getHeight(),
			pixels,
			&bmi,
			DIB_RGB_COLORS
		);

		DeleteDC(memdc);
	}

	bool Window::isKeyPressed(int key) const {
		return keyState_[key];
	}

	bool Window::isMouseButtonPressed(int button) const {
		if (button >= 0 && button < 3) {
			return mouseButtonState_[button];
		}
		return false;
	}

	void Window::getMousePosition(int& x, int& y) const {
		x = mouseX_;
		y = mouseY_;
	}

}
