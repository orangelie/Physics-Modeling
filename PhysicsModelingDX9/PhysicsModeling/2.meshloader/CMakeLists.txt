cmake_minimum_required (VERSION 3.8)

add_executable (2.meshloader "d3d9framework.cpp" "d3d9framework.h" "main.cpp" ${WIN32_SRC})
target_link_libraries(2.meshloader ${CMAKE_SOURCE_DIR}/PhysicsModeling/Lib/x64/d3dx9.lib)