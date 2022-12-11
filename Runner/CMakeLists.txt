project(app)

add_executable(app src/Runner.ixx src/Main.ixx)
target_link_libraries(app
	#[[PRIVATE]]
	engine
)

#[[target_sources(app
	PUBLIC
	FILE_SET cxx_modules TYPE CXX_MODULES FILES
	src/Runner.ixx
	#[=[../Engine/src/engine/EntryPoint.ixx]=]
	)]]

if(MSVC)
	#[[target_compile_options(app PRIVATE /FI ${PROJECT_SOURCE_DIR}/../Engine/src/EEnginePCH.hpp)]]
endif()
