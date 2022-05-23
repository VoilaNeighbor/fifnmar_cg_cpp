add_executable(fifnmar_cg_lab
	board.cpp
	draw_controller.cpp
	glfw_context.cpp
	main.cpp
	ui.cpp
)
target_link_libraries(fifnmar_cg_lab PUBLIC glad2 glfw fmt magic_enum imgui)
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
	set_target_properties(fifnmar_cg_lab PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()