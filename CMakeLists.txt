cmake_minimum_required (VERSION 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
project (projOne)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

file(GLOB IMGUI_HEADER_FILES ${CMAKE_SOURCE_DIR}/src/imgui/*.h)
file(GLOB IMGUI_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/imgui/*.cpp)

set(HEADER_FILES
	src/VAO.hpp
	src/VBO.hpp
	src/EBO.hpp
	src/Shader.hpp
	src/Texture.hpp
	src/Camera.hpp
	src/Mesh.hpp

	# Namespaces
	src/GUI.hpp
)

set(SOURCE_FILES
	# Main files
	src/main.cpp
	src/glad.c
	src/stb.cpp

	# Classes
	src/VAO.cpp
	src/VBO.cpp
	src/EBO.cpp
	src/Shader.cpp
	src/Texture.cpp
	src/Camera.cpp
	src/Mesh.cpp

	# Namespaces
	src/GUI.cpp
)

include_directories(
	"./libraries/include/"
	"./libraries/include/imgui"
	"./libraries/include/imgui/backends"
)
add_executable(projOne
	${HEADER_FILES}
	${SOURCE_FILES}
	${IMGUI_HEADER_FILES}
	${IMGUI_SOURCE_FILES}
)
target_link_libraries(projOne
	${OPENGL_LIBRARY} # filled by "find_package(OpenGL REQUIRED)"
	${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/libglfw3.a
)