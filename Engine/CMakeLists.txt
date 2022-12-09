project(engine)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# add sub directories for projects with cmake support (currently glad, spdlog, GLFW)

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/stb_image)

# library creation
# ER TODO try making a shared lib (i.e. dll) if any interest in making a commercial engine.
# just for toy projects for now.
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
	src/engine/IWindow.hpp
	src/Platform/Windows/WindowsWindow.cpp
	src/Platform/Windows/WindowsWindow.hpp
	src/engine/Input.hpp
	src/Platform/Windows/WindowsInput.cpp
	src/Platform/Windows/WindowsInput.hpp
	src/engine/KeyCode.hpp
	src/engine/MouseButtonCode.hpp
	src/engine/Renderer/IGraphicsContext.hpp
	src/Platform/OpenGL/OpenGLContext.cpp
	src/Platform/OpenGL/OpenGLContext.hpp
	src/engine/Renderer/IShader.cpp
	src/engine/Renderer/IShader.hpp
	src/engine/Renderer/Buffer.cpp
	src/engine/Renderer/Buffer.hpp
	src/Platform/OpenGL/OpenGLBuffer.cpp
	src/Platform/OpenGL/OpenGLBuffer.hpp
	src/engine/Renderer/Renderer.cpp
	src/engine/Renderer/Renderer.hpp
	src/engine/Renderer/IVertexArray.cpp
	src/engine/Renderer/IVertexArray.hpp
	src/Platform/OpenGL/OpenGLVertexArray.cpp
	src/Platform/OpenGL/OpenGLVertexArray.hpp
	src/engine/Renderer/IRendererAPI.hpp
	src/engine/Renderer/RendererAPI.cpp
	src/engine/Renderer/RendererAPI.hpp
	src/Platform/OpenGL/OpenGLRendererAPI.cpp
	src/Platform/OpenGL/OpenGLRendererAPI.hpp
	src/engine/Renderer/Camera.cpp
	src/engine/Renderer/Camera.hpp
	src/engine/Core/Timestep.hpp
	src/Platform/OpenGL/OpenGLShader.cpp
	src/Platform/OpenGL/OpenGLShader.hpp
	src/engine/Renderer/ITexture.cpp
	src/engine/Renderer/ITexture.hpp
	src/Platform/OpenGL/OpenGLTexture.cpp
	src/Platform/OpenGL/OpenGLTexture.hpp
	src/engine/CameraController.cpp
	src/engine/CameraController.hpp
)

target_compile_definitions(engine PRIVATE -DEE_BUILD_DLL -DGLFW_INCLUDE_NONE)

target_link_libraries(engine
	PUBLIC
	imgui
	spdlog
	PRIVATE
	stb_image
	glad
	glfw
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
	vendor/glm
	vendor/imgui

	PRIVATE
	vendor/glfw/include
	vendor/spdlog/include
	vendor/glad/include
	vendor/stb_image
)

#[[if (CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif ()]]

if(MSVC)
	target_compile_options(engine PRIVATE /W4 /WX /FI ${PROJECT_SOURCE_DIR}/src/EEnginePCH.hpp)
endif()