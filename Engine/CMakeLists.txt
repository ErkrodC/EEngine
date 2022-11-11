project(engine)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# add sub directories for projects with cmake support (currently glad, spdlog, GLFW)

add_subdirectory(vendor/spdlog)
#add_subdirectory(vendor/glad)
add_subdirectory(vendor/glfw)

# source code
file(GLOB_RECURSE
	SOURCES CONFIGURE_DEPENDS
	src/*.cpp
	src/*.hpp
	src/*.c
	src/*.h
#	src/engine/*.cpp
#	src/engine/Rendering/*.cpp
#	src/engine/Rendering/Camera/*.cpp
#	src/engine/ImGui/*.cpp
#	src/Platform/OpenGL/*.cpp
#	src/Platform/GLFW/*.cpp

	# stb_image
#	vendor/stb_image/*.cpp

	# must compile and link imgui directly to project
#	vendor/imgui/*.cpp
	)

# library creation
add_library(engine
	STATIC
	${SOURCES}
)

target_compile_definitions(engine PRIVATE -DEE_BUILD_DLL)

target_link_libraries(engine
	PUBLIC
#	GLAD
	glfw
	spdlog
)

target_precompile_headers(engine
	PRIVATE
	src/EEnginePCH.hpp
)

target_include_directories(engine
	PUBLIC
	src
	src/engine
#	src/Platform
#
#	vendor/imgui
#	vendor/glm
#
	vendor/spdlog/include
	PRIVATE
	vendor/glfw/include
#	vendor/glad/include
#	vendor/stb_image
)

if(MSVC)
	target_compile_options(engine PRIVATE /FI ${PROJECT_SOURCE_DIR}/src/EEnginePCH.hpp)
endif()