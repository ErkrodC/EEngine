project(app)

add_executable(app)
target_link_libraries(app
	PUBLIC
	engine
)

target_sources(app
	PUBLIC
	FILE_SET cxx_modules TYPE CXX_MODULES FILES
	src/RunnerLayer2D.ixx
	src/RunnerApp.ixx
	#FILE_SET cxx_module_HEADER_UNITS TYPE CXX_MODULE_HEADER_UNITS FILES
		#ER NOTE can make header units once importing is supported: https://gitlab.kitware.com/cmake/cmake/-/issues/18355
	#FILE_SET cxx_module_internal_partitions TYPE CXX_MODULE_INTERNAL_PARTITIONS FILES

	# EntryPoint provides main() and calls into the app's CreateApplication()
	PRIVATE
	${CMAKE_SOURCE_DIR}/Engine/src/engine/EntryPoint.ixx
)
