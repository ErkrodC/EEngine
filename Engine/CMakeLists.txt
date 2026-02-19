project(engine)

# GLFW configuration - disable unnecessary builds
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Vendor dependencies
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/stb_image)

# Engine library (static for now)
add_library(engine
	src/engine/Core/Core.hpp
	src/engine/Event/Event.hpp
	src/engine/Profiling/Profiling.hpp
)

target_sources(engine
	PUBLIC
		FILE_SET modules TYPE CXX_MODULES FILES
			# Root modules
			src/engine/EEngine.ixx
			src/engine/EEngine.Math.ixx
			src/engine/EEngine.Editor.ixx
			src/engine/EEngine.Standard.ixx

			# Core
			src/engine/Core/EEngine.Core.ixx
			src/engine/Core/Core.ixx
			src/engine/Core/KeyCode.ixx
			src/engine/Core/Log.ixx
			src/engine/Core/MouseButtonCode.ixx
			src/engine/Core/Timestep.ixx

			# Events
			src/engine/Event/EEngine.Event.ixx
			src/engine/Event/ApplicationEvent.ixx
			src/engine/Event/Event.ixx
			src/engine/Event/KeyEvent.ixx
			src/engine/Event/MouseEvent.ixx

			# Rendering
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
			src/engine/Rendering/Renderer2D.ixx
			src/engine/Rendering/RendererAPI.ixx
			src/engine/Rendering/ShaderLibrary.ixx

			# OpenGL Implementation
			src/engine/Rendering/OpenGL/OpenGLBuffer.ixx
			src/engine/Rendering/OpenGL/OpenGLContext.ixx
			src/engine/Rendering/OpenGL/OpenGLRendererAPI.ixx
			src/engine/Rendering/OpenGL/OpenGLShader.ixx
			src/engine/Rendering/OpenGL/OpenGLTexture.ixx
			src/engine/Rendering/OpenGL/OpenGLVertexArray.ixx

			# Application
			src/engine/Application/EEngine.Application.ixx
			src/engine/Application/Application.ixx
			src/engine/Application/CameraController.ixx
			src/engine/Application/IInput.ixx
			src/engine/Application/Input.ixx
			src/engine/Application/IWindow.ixx
			src/engine/Application/Layer.ixx
			src/engine/Application/LayerStack.ixx
			src/engine/Application/TBD.ixx

			# Platform - Windows
			src/engine/Application/Windows/WindowsInput.ixx
			src/engine/Application/Windows/WindowsWindow.ixx

			# ImGui
			src/engine/Application/IMGUI/IMGUILayer.ixx

			# Profiling
			src/engine/Profiling/EEngine.Profiling.ixx

		FILE_SET headers TYPE HEADERS FILES
			src/engine/EntryPoint.ixx
)

# Compile definitions
target_compile_definitions(engine PRIVATE
	-DEE_BUILD_DLL
	-DGLFW_INCLUDE_NONE
	-D_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
	-D_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
)

# Link libraries
target_link_libraries(engine
	PUBLIC
		imgui
		spdlog
	PRIVATE
		stb_image
		glad
		glfw
)

# Include directories
target_include_directories(engine
	PUBLIC
		src
		src/engine
		vendor/glm
		vendor/imgui
	PRIVATE
		vendor/glfw/include
		vendor/spdlog/include
		vendor/glad/include
		vendor/stb_image
)

# Compiler-specific flags
if(MSVC)
	target_compile_options(engine PRIVATE
		/W4                # Warning level 4
		/wd4100            # Unreferenced formal parameter
		/wd4201            # Nameless struct/union (from GLM)
		/wd4291            # No matching operator delete
	)
endif()