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
			# Root module
			src/engine/EEngine.ixx

			# EEngine.Core module and partitions
			src/engine/EEngine.Core.ixx
			src/engine/EEngine.Core-Memory.ixx
			src/engine/EEngine.Core-Time.ixx
			src/engine/EEngine.Core-Input.ixx
			src/engine/EEngine.Core-Log.ixx

			# EEngine.Event module and partitions
			src/engine/EEngine.Event.ixx
			src/engine/EEngine.Event-Base.ixx
			src/engine/EEngine.Event-Application.ixx
			src/engine/EEngine.Event-Keyboard.ixx
			src/engine/EEngine.Event-Mouse.ixx

			# EEngine.Application module and partitions
			src/engine/EEngine.Application.ixx
			src/engine/EEngine.Application-Window.ixx
			src/engine/EEngine.Application-Input.ixx
			src/engine/EEngine.Application-Layer.ixx
			src/engine/EEngine.Application-IMGUILayer.ixx
			src/engine/EEngine.Application-CameraController.ixx
			src/engine/EEngine.Application-Application.ixx

			# EEngine.Rendering module and partitions
			src/engine/EEngine.Rendering.ixx
			src/engine/EEngine.Rendering-API.ixx
			src/engine/EEngine.Rendering-Buffers.ixx
			src/engine/EEngine.Rendering-Interfaces.ixx
			src/engine/EEngine.Rendering-Camera.ixx
			src/engine/EEngine.Rendering-OpenGL.ixx
			src/engine/EEngine.Rendering-Renderer.ixx
			src/engine/EEngine.Rendering-Renderer2D.ixx

			# Other modules (no partitions)
			src/engine/EEngine.Math.ixx
			src/engine/EEngine.Editor.ixx
			src/engine/EEngine.Standard.ixx
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