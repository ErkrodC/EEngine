project(stb_image)

add_library(stb_image
	STATIC
	stb_image.cpp
	stb_image.hpp
)

target_include_directories(stb_image
	PUBLIC
	${PROJECT_SOURCE_DIR}
)

#[[get_target_property(INCLUDES stb_image INCLUDE_DIRECTORIES)
message(WARNING "${INCLUDES}")]]
