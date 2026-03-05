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
		# EEngine.Application module implementations
		src/Application/EEngine.Application_IMGUILayer.cpp
		src/Application/EEngine.Application_Input.cpp
		src/Application/EEngine.Application_Window.cpp

		# EEngine.Rendering module implementations
		src/Rendering/EEngine.Rendering_GraphicsContext.cpp
		src/Rendering/EEngine.Rendering_IndexBuffer.cpp
		src/Rendering/EEngine.Rendering_Renderer.cpp
		src/Rendering/EEngine.Rendering_RendererAPI.cpp
		src/Rendering/EEngine.Rendering_Shader.cpp
		src/Rendering/EEngine.Rendering_Texture.cpp
		src/Rendering/EEngine.Rendering_UniformBuffer.cpp
		src/Rendering/EEngine.Rendering_VertexArray.cpp
		src/Rendering/EEngine.Rendering_VertexBuffer.cpp
)

target_sources(engine
	PUBLIC
		FILE_SET modules TYPE CXX_MODULES FILES
		# Root module
        src/EEngine.ixx

		# EEngine.Application module and partitions
		src/Application/EEngine.Application.ixx
		src/Application/EEngine.Application_Application.ixx
		src/Application/EEngine.Application_CameraController.ixx
		src/Application/EEngine.Application_IMGUILayer.ixx
		src/Application/EEngine.Application_Input.ixx
		src/Application/EEngine.Application_Layer.ixx
		src/Application/EEngine.Application_Window.ixx

		# EEngine.Core module and partitions
        src/Core/EEngine.Core.ixx
        src/Core/EEngine.Core_Input.ixx
		src/Core/EEngine.Core_Log.ixx
		src/Core/EEngine.Core_Memory.ixx
		src/Core/EEngine.Core_Platform.ixx
		src/Core/EEngine.Core_Time.ixx

		# EEngine.Event module and partitions
		src/Event/EEngine.Event.ixx
		src/Event/EEngine.Event_Application.ixx
		src/Event/EEngine.Event_Base.ixx
		src/Event/EEngine.Event_Keyboard.ixx
		src/Event/EEngine.Event_Mouse.ixx

		# EEngine.Rendering module and partitions
		src/Rendering/EEngine.Rendering.ixx
		src/Rendering/EEngine.Rendering_API.ixx
		src/Rendering/EEngine.Rendering_Buffers.ixx
		src/Rendering/EEngine.Rendering_Camera.ixx
		src/Rendering/EEngine.Rendering_GraphicsContext.ixx
		src/Rendering/EEngine.Rendering_IndexBuffer.ixx
		src/Rendering/EEngine.Rendering_Renderer.ixx
		src/Rendering/EEngine.Rendering_RendererAPI.ixx
		src/Rendering/EEngine.Rendering_Shader.ixx
		src/Rendering/EEngine.Rendering_Texture.ixx
		src/Rendering/EEngine.Rendering_UniformBuffer.ixx
		src/Rendering/EEngine.Rendering_VertexArray.ixx
		src/Rendering/EEngine.Rendering_VertexBuffer.ixx

		# Other modules (no partitions)
		src/Editor/EEngine.Editor.ixx
		src/Math/EEngine.Math.ixx
		src/Profiling/EEngine.Profiling.ixx
		src/Standard/EEngine.Standard.ixx

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