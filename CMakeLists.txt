cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

# set the project name
project(stb-sdf-text-demo)

# add the executable
add_executable(stb-sdf-text-demo 
	"main.cpp" 
	"imgui_impl/imgui_impl_sdl.h" 
	"imgui_impl/imgui_impl_sdl.cpp"
	"imgui_impl/imgui_impl_opengl3.h" 
	"imgui_impl/imgui_impl_opengl3.cpp" 
	"main_scene.hpp" "main_scene.cpp" "texture.cpp" "texture.hpp")

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

find_path(STB_INCLUDE_DIRS "stb.h")
target_link_libraries(stb-sdf-text-demo PRIVATE 
    SDL2::SDL2 
    SDL2::SDL2main 
    imgui::imgui 
    unofficial::gl3w::gl3w 
    spdlog::spdlog 
    spdlog::spdlog_header_only
    fmt::fmt)
    
target_compile_features(stb-sdf-text-demo PRIVATE cxx_std_17)

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "shaders/*.frag"
    "shaders/*.vert"
)

add_custom_target(SHADER_COMPILE 
    SOURCES ${GLSL_SOURCE_FILES} 
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy ${GLSL_SOURCE_FILES} "${CMAKE_CURRENT_BINARY_DIR}/shaders/"
)
add_dependencies(stb-sdf-text-demo SHADER_COMPILE)


target_include_directories(stb-sdf-text-demo PRIVATE ${STB_INCLUDE_DIRS})