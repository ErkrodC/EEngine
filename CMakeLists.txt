cmake_minimum_required(VERSION 4.1.2)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
project(EEngineV2 LANGUAGES CXX)

message(STATUS "CMake Version: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")

# C++23 with modules support
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# Experimental std module support
set(CMAKE_CXX_MODULE_STD ON)

# Output binaries to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

# Git submodule management
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if(GIT_SUBMODULE)
		message(STATUS "Updating git submodules...")
		execute_process(
			COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT
		)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update failed with ${GIT_SUBMOD_RESULT}")
		endif()
	endif()
endif()

# Verify required vendor dependencies
set(PROJECT_CMAKE_SUBMODULES glad glfw glm imgui spdlog)
foreach(SUBMOD_NAME ${PROJECT_CMAKE_SUBMODULES})
	if(NOT EXISTS "${PROJECT_SOURCE_DIR}/Engine/vendor/${SUBMOD_NAME}/CMakeLists.txt")
		message(FATAL_ERROR "Submodule '${SUBMOD_NAME}' is missing. Run: git submodule update --init --recursive")
	endif()
endforeach()

# Platform definitions
if(WIN32)
	add_definitions(-DEE_PLATFORM_WINDOWS)
endif()

# Build type definitions
if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DEE_DEBUG -DEE_ENABLE_LOGGING)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	add_definitions(-DEE_RELEASE)
elseif(CMAKE_BUILD_TYPE MATCHES MinSizeRel)
	add_definitions(-DEE_DIST)
endif()

# Subdirectories
add_subdirectory(Engine)
add_subdirectory(Runner)