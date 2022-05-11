add_executable(fifnmar_cg_lab main.cpp board.cpp board.hpp signal.hpp draw.hpp cursor.cpp cursor.hpp)
target_link_libraries(fifnmar_cg_lab PUBLIC glad2 glfw fmt magic_enum)