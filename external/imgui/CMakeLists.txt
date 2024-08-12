file(GLOB_RECURSE IMGUI_SOURCE_FILES CONFIGURE_DEPENDS "*.cpp" "*.h" "*.inl")

add_library(imgui
	${IMGUI_SOURCE_FILES}
)

target_include_directories(imgui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

set_target_properties(imgui PROPERTIES FOLDER "External")
