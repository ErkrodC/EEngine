project(app)

# Runner executable
add_executable(app)

# Link against engine library
target_link_libraries(app
	PUBLIC
		engine
)

# Source files
target_sources(app
	PUBLIC
		FILE_SET modules TYPE CXX_MODULES FILES
			src/RunnerLayer2D.ixx
			src/RunnerApp.ixx

	PRIVATE
		# EntryPoint provides main() and calls CreateApplication()
		${CMAKE_SOURCE_DIR}/Engine/src/engine/EntryPoint.ixx
)
