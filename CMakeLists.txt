cmake_minimum_required (VERSION 2.8.11)
project (Luke)
set(CMAKE_CXX_FLAGS "-std=c++11 -fno-exceptions -w")

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories (${CMAKE_CURRENT_SOURCE_DIR} /usr/local/include)
link_directories(/usr/local/lib ${GLFW_INCLUDE_DIRS})

set (LUKEDEPS Stick ${GLFW_STATIC_LIBRARIES})

set (LUKEINC 
Luke/GLFW/GLFWDisplayImpl.hpp
Luke/GLFW/GLFWInitializer.hpp
Luke/GLFW/GLFWWindowImpl.hpp
Luke/Display.hpp
Luke/DisplayMode.hpp
Luke/MouseEvents.hpp
Luke/MouseState.hpp
Luke/Window.hpp
Luke/WindowEvents.hpp
Luke/WindowSettings.hpp
)

set (LUKESRC 
Luke/GLFW/GLFWDisplayImpl.cpp
Luke/GLFW/GLFWWindowImpl.cpp
Luke/Display.cpp
Luke/DisplayMode.cpp
Luke/MouseEvents.cpp
Luke/MouseState.cpp
Luke/Window.cpp
Luke/WindowEvents.cpp
Luke/WindowSettings.cpp
)

add_library (Luke SHARED ${LUKESRC})
target_link_libraries(Luke ${LUKEDEPS})
add_library (LukeStatic STATIC ${LUKESRC})
target_link_libraries(LukeStatic ${LUKEDEPS})

install (TARGETS Luke LukeStatic DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
foreach ( file ${LUKEINC} )
    get_filename_component( dir ${file} DIRECTORY )
    install( FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${dir} )
endforeach()

add_subdirectory (Playground)
