add_library(glad2 STATIC ${CMAKE_CURRENT_LIST_DIR}/src/gl.c)
target_include_directories(glad2 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)