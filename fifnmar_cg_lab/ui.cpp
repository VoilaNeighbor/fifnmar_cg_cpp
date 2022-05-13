#include "ui.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "glfw_context.hpp"

namespace {
	i32 _draw_mode;

	void translate_mouse_event(auto, i32 button_code, i32 action, auto) {
		if (ImGui::GetIO().WantCaptureMouse || action != GLFW_PRESS) { return; }
		MouseButton button;
		if (button_code == GLFW_MOUSE_BUTTON_LEFT) {
			button = MouseButton::Left;
		} else if (button_code == GLFW_MOUSE_BUTTON_RIGHT) {
			button = MouseButton::Right;
		} else {
			return;
		}
		g_cursor_click_signal.send(button);
	}
} // namespace

namespace ui {
	// don't know why some functions are not in a namespace… Perhaps too old.
	using namespace ImGui;

	void init() {
		glfwSetMouseButtonCallback(glfw::raw_window, translate_mouse_event);
		IMGUI_CHECKVERSION();
		CreateContext();
		StyleColorsLight();
		ImGui_ImplGlfw_InitForOpenGL(glfw::raw_window, true);
		ImGui_ImplOpenGL3_Init();
		GetIO().Fonts->AddFontFromFileTTF("Merriweather-Regular.ttf", 16);
	}

	void render() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		NewFrame();

		SetNextWindowPos({ 0, 0 });
		Begin("Draw mode selector");
		Combo("Draw mode", &_draw_mode, "Line\0Circle\0");
		Button("Button"); SameLine(); Text("Same line text");
		End();

		Render();
		ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
	}
} // namespace ui