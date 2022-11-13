project(engine)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# add sub directories for projects with cmake support (currently glad, spdlog, GLFW)

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/imgui)

# library creation
add_library(engine
	src/EEnginePCH.hpp
	src/EEngine.hpp
	src/engine/Events/ApplicationEvent.hpp
	src/engine/Events/Event.hpp
	src/engine/Events/KeyEvent.hpp
	src/engine/Events/MouseEvent.hpp
	src/engine/imgui/IMGUILayer.cpp
	src/engine/imgui/IMGUILayer.hpp
	src/engine/Application.cpp
	src/engine/Application.hpp
	src/engine/Core.hpp
	src/engine/EntryPoint.hpp
	src/engine/Layer.cpp
	src/engine/Layer.hpp
	src/engine/LayerStack.cpp
	src/engine/LayerStack.hpp
	src/engine/Log.cpp
	src/engine/Log.hpp
	src/engine/Window.hpp
	src/Platform/Windows/WindowsWindow.cpp
	src/Platform/Windows/WindowsWindow.hpp
	src/engine/Input.hpp src/Platform/Windows/WindowsInput.cpp src/Platform/Windows/WindowsInput.hpp)

target_compile_definitions(engine PRIVATE -DEE_BUILD_DLL -DGLFW_INCLUDE_NONE)

target_link_libraries(engine
	PUBLIC
	imgui
	glad
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
#	vendor/glm
	vendor/imgui

	PRIVATE
	vendor/glfw/include
	vendor/spdlog/include
	vendor/glad/include
#	vendor/stb_image
)

if(MSVC)
	target_compile_options(engine PRIVATE /FI ${PROJECT_SOURCE_DIR}/src/EEnginePCH.hpp)
endif()