add_executable(fifnmar_cg_lab main.cpp board.cpp cursor.cpp draw_line_controller.cpp)
target_link_libraries(fifnmar_cg_lab PUBLIC glad2 glfw fmt magic_enum)