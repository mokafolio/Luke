cmake_minimum_required (VERSION 2.8.11)
project (Luke)
set(CMAKE_CXX_FLAGS "-std=c++11 -fno-exceptions -w")

option(BuildSubmodules "BuildSubmodules" OFF)

set(LUKEIMPL "SDL" CACHE STRING "The implementation to compile")

include_directories (${CMAKE_CURRENT_SOURCE_DIR} /usr/local/include/)

if(BuildSubmodules)
    link_directories(subprojects/Stick)
else()
    link_directories(/usr/local/lib)
endif()

set (LUKEINC 
Luke/Constants.hpp
Luke/Display.hpp
Luke/DisplayMode.hpp
Luke/KeyCodes.hpp
Luke/KeyEvents.hpp
Luke/Luke.hpp
Luke/MouseEvents.hpp
Luke/MouseState.hpp
Luke/TextInputEvent.hpp
Luke/Window.hpp
Luke/WindowEvents.hpp
Luke/WindowSettings.hpp
)

set (LUKESRC 
Luke/Display.cpp
Luke/DisplayMode.cpp
Luke/KeyEvents.cpp
Luke/MouseEvents.cpp
Luke/MouseState.cpp
Luke/TextInputEvent.cpp
Luke/Window.cpp
Luke/WindowEvents.cpp
Luke/WindowSettings.cpp
)

if(${LUKEIMPL} STREQUAL "GLFW")
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})
    add_definitions(-DLUKE_IMPLEMENTATION_GLFW)
    set (LUKEDEPS Stick ${GLFW_STATIC_LIBRARIES})
    set (LUKEINC ${LUKEINC}
        Luke/GLFW/GLFWDisplayImpl.hpp
        Luke/GLFW/GLFWInitializer.hpp
        Luke/GLFW/GLFWWindowImpl.hpp
    )
    set (LUKESRC ${LUKESRC}
        Luke/GLFW/GLFWDisplayImpl.cpp
        Luke/GLFW/GLFWInitializer.cpp
        Luke/GLFW/GLFWWindowImpl.cpp
    )
elseif(${LUKEIMPL} STREQUAL "SDL")
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    add_definitions(-DLUKE_IMPLEMENTATION_SDL)
    set (LUKEDEPS Stick ${SDL2_LIBRARIES})
    set (LUKEINC ${LUKEINC}
        Luke/SDL/SDLDisplayImpl.hpp
        Luke/SDL/SDLInitializer.hpp
        Luke/SDL/SDLWindowImpl.hpp
    )
    set (LUKESRC ${LUKESRC}
        Luke/SDL/SDLDisplayImpl.cpp
        Luke/SDL/SDLInitializer.cpp
        Luke/SDL/SDLWindowImpl.cpp
    )
endif()

if(BuildSubmodules)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/subprojects/Stick)
endif()

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
