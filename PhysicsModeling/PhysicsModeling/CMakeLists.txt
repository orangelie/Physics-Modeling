cmake_minimum_required (VERSION 3.8)

set(WIN32_SRC "../includes/Windows/Win32.cpp")

project ("1.dx9Init")
add_subdirectory ("1.dx9Init")

project ("2.meshloader")
add_subdirectory ("2.meshloader")

add_subdirectory ("includes")

target_include_directories(1.dx9Init PUBLIC "./Dx9Include")
target_include_directories(2.meshloader PUBLIC "./Dx9Include")