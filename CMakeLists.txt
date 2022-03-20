cmake_minimum_required(VERSION 3.17)
project(DX9Hook)

# Local paths
set(DIR_ROOT "${PROJECT_SOURCE_DIR}")
set(DIR_INCLUDE "${DIR_ROOT}/include")
set(DIR_SOURCE "${DIR_ROOT}/source")
set(DIR_LIBRARY "${DIR_ROOT}/lib")
set(DIR_TEST "${DIR_ROOT}/test")

# VCPKG paths
set(DIR_VCPKG_INSTALL "${DIR_VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}")
set(DIR_VCPKG_INCLUDE "${DIR_VCPKG_INSTALL}/include")
set(DIR_VCPKG_LIBRARY "${DIR_VCPKG_INSTALL}/lib")
set(DIR_VCPKG_LIBRARY_DEBUG "${DIR_VCPKG_INSTALL}/debug/lib")

# Binary output paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${DIR_LIBRARY}/deb")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${DIR_LIBRARY}/deb")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${DIR_ROOT})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${DIR_LIBRARY}/rel")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${DIR_LIBRARY}/rel")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${DIR_ROOT})

set(CMAKE_CXX_STANDARD 14)

link_directories(
        ./
        ./cmake-build-debug/minhook
        ./cmake-build-debug/DLLInject)

include_directories(
        source/
        imgui/
        imgui/backends/
        minhook/include/
        DLLInject/source/
)

# Add Minhook
add_subdirectory(minhook)
set_target_properties(minhook PROPERTIES DEBUG_POSTFIX "")
set_target_properties(minhook PROPERTIES RELEASE_POSTFIX "")
set_target_properties(minhook PROPERTIES RELWITHDEBINFO_POSTFIX "")
set_target_properties(minhook PROPERTIES MINSIZEREL_POSTFIX "")
set_target_properties(minhook PROPERTIES PREFIX "lib")

# Add DLLInject
add_subdirectory(DLLInject)

# Add Demo
add_executable(demo demo/demo.cpp demo/DX9.cpp)
target_link_libraries(demo d3d9)

add_executable(inject demo/inject.cpp)
target_link_libraries(inject DLLInject)

# Add DLL
add_library(hook SHARED
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_demo.cpp
        imgui/backends/imgui_impl_win32.cpp
        imgui/backends/imgui_impl_dx9.cpp
        source/dll.cpp
        source/dx9hook.cpp)
target_link_libraries(hook d3d9 d3dcompiler minhook imgui imm32 dwmapi)
