cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

project(EEngineV2)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	# Update submodules as needed
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if(GIT_SUBMODULE)
		message(STATUS "Submodule update")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/Engine/vendor/spdlog/CMakeLists.txt")
	message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

if (WIN32)
	add_definitions(-DEE_PLATFORM_WINDOWS)
endif(WIN32)

if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DEE_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	add_definitions(-DEE_RELEASE)
elseif(CMAKE_BUILD_TYPE MATCHES MinSizeRel)
	add_definitions(-DEE_DIST)
endif()

add_subdirectory(Engine)
add_subdirectory(Runner)