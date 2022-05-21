cmake_minimum_required (VERSION 3.8) # not mutable

# PRIVATE
set(PRIVATE_DxInterface "../STATIC/HEADER/PRIVATE/DxInterface/d3dx12.h") # not mutable
set(PRIVATE_Texture "../STATIC/HEADER/PRIVATE/Texture/DDSTextureLoader.h" "../STATIC/SOURCE/PRIVATE/Texture/DDSTextureLoader.cpp") # not mutable

# PUBLIC
set(PUBLIC_Camera "../STATIC/HEADER/PUBLIC/Camera/DefaultCamera.h" "../STATIC/SOURCE/PUBLIC/Camera/DefaultCamera.cpp") # not mutable
set(PUBLIC_DxInterface "../STATIC/HEADER/PUBLIC/DxInterface/InterfaceD3D12.h" "../STATIC/SOURCE/PUBLIC/DxInterface/InterfaceD3D12.cpp" "../STATIC/HEADER/PUBLIC/DxInterface/InterfaceDxgi.h" "../STATIC/SOURCE/PUBLIC/DxInterface/InterfaceDxgi.cpp") # not mutable
set(PUBLIC_Engine "../STATIC/HEADER/PUBLIC/Engine/ZekrosEngine.h" "../STATIC/SOURCE/PUBLIC/Engine/ZekrosEngine.cpp" "../STATIC/HEADER/PUBLIC/Engine/ZekrosEnginePacked.h" "../STATIC/SOURCE/PUBLIC/Engine/ZekrosEnginePacked.cpp") # not mutable
set(PUBLIC_Font "../STATIC/HEADER/PUBLIC/Font/FontEngine.h" "../STATIC/SOURCE/PUBLIC/Font/FontEngine.cpp" "../STATIC/HEADER/PUBLIC/Font/Fontloader.h" "../STATIC/SOURCE/PUBLIC/Font/Fontloader.cpp") # not mutable


set(GridAndCameraMovement ${PRIVATE_DxInterface} ${PRIVATE_Texture} ${PUBLIC_Camera} ${PUBLIC_DxInterface} ${PUBLIC_Engine} ${PUBLIC_Font})

add_subdirectory ("engines") # not mutable


project("Dx11Init")
target_include_directories(Dx11Init PUBLIC "${CMAKE_SOURCE_DIR}/DxInclude")

project("Dx11Draw")
target_include_directories(Dx11Draw PUBLIC "${CMAKE_SOURCE_DIR}/DxInclude")