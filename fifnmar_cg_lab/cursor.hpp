#pragma once

#include "utils.hpp"
#include "signal.hpp"

enum class MouseButton { Left, Right };

extern ChainSignal<MouseButton> g_cursor_click_signal;

// Normalized within 0..1, could be out of range.
std::pair<f64, f64> cursor_coordinate();
void set_cursor_laser_image(u32 radius, u32 len);

struct GLFWwindow;
void mouse_button_callback(GLFWwindow* window, i32 button_code, i32 action, i32 mods);
