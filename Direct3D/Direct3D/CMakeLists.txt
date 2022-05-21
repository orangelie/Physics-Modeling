cmake_minimum_required (VERSION 3.8)

project ("Direct3D")
add_executable (Direct3D "InertiaTensor/InertiaTensor.cpp" "InertiaTensor/InertiaTensor.h" "InertiaTensor/main.cpp")

# target_link_libraries(Direct3D dxgi.lib)