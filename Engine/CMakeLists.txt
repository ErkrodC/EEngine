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
	src/engine/Core/Core.hpp
	src/engine/Event/Event.hpp
	#[[src/engine/Core/Log.cpp
	src/engine/Renderer/Buffer.cpp
	src/engine/Renderer/Camera.cpp
	src/engine/Renderer/IShader.cpp
	src/engine/Renderer/Renderer.cpp
	src/engine/Renderer/RendererAPI.cpp
	src/Platform/OpenGL/OpenGLBuffer.cpp
	src/Platform/OpenGL/OpenGLContext.cpp]]
	#src/engine/Rendering/OpenGL/OpenGLRendererAPI.cpp
	#[[src/Platform/OpenGL/OpenGLShader.cpp
	src/Platform/OpenGL/OpenGLTexture.cpp
	src/Platform/OpenGL/OpenGLVertexArray.cpp
	src/Platform/Windows/WindowsInput.cpp
	src/Platform/Windows/WindowsWindow.cpp
	src/engine/Application/Application.cpp
	src/engine/Application/CameraController.cpp
	src/engine/Application/Layer.cpp
	src/engine/Application/LayerStack.cpp
	src/engine/Application/IMGUI/IMGUILayer.cpp]]
	#[[src/EEnginePCH.hpp
	src/EEngine.hpp]]
)

target_sources(engine
	PUBLIC
		FILE_SET cxx_modules TYPE CXX_MODULES FILES
			src/engine/EEngine.ixx
			src/engine/Core/EEngine.Core.ixx
			#[[src/engine/Core/Core.ixx]]
			src/engine/Core/IInput.ixx
			#[[src/engine/Core/Input.ixx
			src/engine/Core/KeyCode.ixx
			src/engine/Core/Log.ixx
			src/engine/Core/MouseButtonCode.ixx
			src/engine/Core/Timestep.ixx]]
			src/engine/Event/EEngine.Event.ixx
			#[[src/engine/Event/ApplicationEvent.ixx
			src/engine/Event/Event.ixx]]
			src/engine/Event/KeyEvent.ixx
			src/engine/Event/MouseEvent.ixx
			src/engine/Rendering/EEngine.Rendering.ixx
			src/engine/Rendering/API.ixx
			src/engine/Rendering/Buffer.ixx
			src/engine/Rendering/Camera.ixx
			src/engine/Rendering/IGraphicsContext.ixx
			src/engine/Rendering/IRendererAPI.ixx
			src/engine/Rendering/IShader.ixx
			src/engine/Rendering/ITexture.ixx
			src/engine/Rendering/IVertexArray.ixx
			src/engine/Rendering/Renderer.ixx
			src/engine/Rendering/RendererAPI.ixx
			src/engine/Rendering/ShaderLibrary.ixx
			src/engine/Rendering/OpenGL/OpenGLBuffer.ixx
			src/engine/Rendering/OpenGL/OpenGLContext.ixx
			src/engine/Rendering/OpenGL/OpenGLRendererAPI.ixx
			src/engine/Rendering/OpenGL/OpenGLShader.ixx
			src/engine/Rendering/OpenGL/OpenGLTexture.ixx
			src/engine/Rendering/OpenGL/OpenGLVertexArray.ixx
			src/engine/Application/Windows/WindowsInput.ixx
			src/engine/Application/Windows/WindowsWindow.ixx
			src/engine/Application/EEngine.Application.ixx
			src/engine/Application/Application.ixx
			src/engine/Application/CameraController.ixx
			src/engine/Application/IWindow.ixx
			src/engine/Application/Layer.ixx
			src/engine/Application/LayerStack.ixx
			src/engine/Application/IMGUI/IMGUILayer.ixx
			#[[src/engine/EntryPoint.ixx]]
		#FILE_SET cxx_module_internal_partitions TYPE CXX_MODULE_INTERNAL_PARTITIONS FILES
			#src/engine/Core/Log.cpp
			#src/engine/Rendering/Buffer.cpp
			#src/engine/Rendering/Camera.cpp
			#src/engine/Rendering/ShaderLibrary.cpp
			#src/engine/Rendering/Renderer.cpp
			#src/engine/Rendering/RendererAPI.cpp
			#src/engine/Rendering/OpenGL/OpenGLBuffer.cpp
			#src/engine/Rendering/OpenGL/OpenGLContext.cpp
			#src/engine/Rendering/OpenGL/OpenGLRendererAPI.cpp
			#src/engine/Rendering/OpenGL/OpenGLShader.cpp
			#src/engine/Rendering/OpenGL/OpenGLTexture.cpp
			#src/engine/Rendering/OpenGL/OpenGLVertexArray.cpp
			#src/engine/Application/Windows/WindowsInput.cpp
			#src/engine/Application/Windows/WindowsWindow.cpp
			#src/engine/Application/Application.cpp
			#src/engine/Application/CameraController.cpp
			#src/engine/Application/Layer.cpp
			#src/engine/Application/LayerStack.cpp
			#src/engine/Application/IMGUI/IMGUILayer.cpp
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

#[[target_precompile_headers(engine
	PRIVATE
	src/EEnginePCH.hpp
)]]

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
	#C4100 is unused parameter warning
	#C4201 is nameless union/struct warning, glm was causing this, but probably better to disable per file
	target_compile_options(engine PRIVATE /W4 /WX /wd4100 /wd4201 /FI ${PROJECT_SOURCE_DIR}/src/EEnginePCH.hpp)
endif()