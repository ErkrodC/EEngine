project(app)

add_executable(app src/Runner.cpp)
target_link_libraries(app
	PRIVATE
	engine
)

if(MSVC)
	target_compile_options(app PRIVATE /FI ${PROJECT_SOURCE_DIR}/../Engine/src/EEnginePCH.hpp)
endif()