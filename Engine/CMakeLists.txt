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
        src/Core/Core.hpp
        src/Event/Event.hpp
        src/Profiling/Profiling.hpp
        src/GLFW_fwd.hpp
)

target_sources(engine
	PRIVATE
        src/EEngine.Rendering_GraphicsContext.cpp
        src/EEngine.Rendering_IndexBuffer.cpp
        src/EEngine.Rendering_Renderer.cpp
        src/EEngine.Rendering_RendererAPI.cpp
        src/EEngine.Rendering_Shader.cpp
        src/EEngine.Rendering_Texture.cpp
        src/EEngine.Rendering_VertexArray.cpp
        src/EEngine.Rendering_VertexBuffer.cpp
)

target_sources(engine
	PUBLIC
		FILE_SET modules TYPE CXX_MODULES FILES
			# Root module
        src/EEngine.ixx

			# EEngine.Core module and partitions
        src/EEngine.Core.ixx
        src/EEngine.Core_Memory.ixx
        src/EEngine.Core_Time.ixx
        src/EEngine.Core_Input.ixx
        src/EEngine.Core_Log.ixx

			# EEngine.Event module and partitions
        src/EEngine.Event.ixx
        src/EEngine.Event_Base.ixx
        src/EEngine.Event_Application.ixx
        src/EEngine.Event_Keyboard.ixx
        src/EEngine.Event_Mouse.ixx

			# EEngine.Application module and partitions
        src/EEngine.Application.ixx
        src/EEngine.Application_Window.ixx
        src/EEngine.Application_Input.ixx
        src/EEngine.Application_Layer.ixx
        src/EEngine.Application_IMGUILayer.ixx
        src/EEngine.Application_CameraController.ixx
        src/EEngine.Application_Application.ixx

			# EEngine.Rendering module and partitions
        src/EEngine.Rendering.ixx
        src/EEngine.Rendering_API.ixx
        src/EEngine.Rendering_Buffers.ixx
        src/EEngine.Rendering_Camera.ixx
        src/EEngine.Rendering_GraphicsContext.ixx
        src/EEngine.Rendering_IndexBuffer.ixx
        src/EEngine.Rendering_Renderer.ixx
        src/EEngine.Rendering_RendererAPI.ixx
        src/EEngine.Rendering_Shader.ixx
        src/EEngine.Rendering_Texture.ixx
        src/EEngine.Rendering_VertexArray.ixx
        src/EEngine.Rendering_VertexBuffer.ixx

			# Other modules (no partitions)
        src/EEngine.Math.ixx
        src/EEngine.Editor.ixx
        src/EEngine.Standard.ixx
        src/Profiling/EEngine.Profiling.ixx

		FILE_SET headers TYPE HEADERS FILES
        src/EntryPoint.ixx
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