project(app)

add_executable(app src/Runner.cpp #[[src/TestLayer.cpp src/Sandbox2D.cpp]])
target_link_libraries(app
	PRIVATE
	engine
)