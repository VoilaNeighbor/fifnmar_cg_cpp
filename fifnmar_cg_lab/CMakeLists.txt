add_executable(fifnmar_cg_lab main.cpp board.cpp board.hpp signal.hpp draw.hpp signal.cpp)
target_link_libraries(fifnmar_cg_lab PUBLIC glad2 glfw fmt magic_enum)