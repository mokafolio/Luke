find_package(OpenGL REQUIRED)

add_executable (LukePlayground LukePlayground.cpp)
target_link_libraries(LukePlayground LukeStatic ${LUKEDEPS} ${OPENGL_LIBRARIES})