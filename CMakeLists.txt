cmake_minimum_required(VERSION 3.22)
project(fifnmar_cg_cpp)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG 8.1.1)
FetchContent_Declare(Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG v3.0.0-preview3)
FetchContent_Declare(magic_enum
	GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
	GIT_TAG v0.7.3)
FetchContent_MakeAvailable(fmt Catch2 magic_enum)

add_subdirectory(dep/glad2)
add_subdirectory(dep/glfw-3.3.7)

add_subdirectory(fifnmar_cg_lab)
