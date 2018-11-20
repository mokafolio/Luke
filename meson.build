project('Luke', 'cpp', default_options : ['cpp_std=c++14'])

if meson.is_subproject() == false
    # disable this bogus warning (should be deprecated in newer clang versions)
    add_global_arguments('-Wno-missing-braces', '-fno-exceptions', language: 'cpp')
endif

lukeInc = [
    'Luke/Constants.hpp',
    'Luke/Display.hpp',
    'Luke/DisplayMode.hpp',
    'Luke/KeyCodes.hpp',
    'Luke/KeyEvents.hpp',
    'Luke/Luke.hpp',
    'Luke/MouseEvents.hpp',
    'Luke/MouseState.hpp',
    'Luke/TextInputEvent.hpp',
    'Luke/Window.hpp',
    'Luke/WindowEvents.hpp',
    'Luke/WindowSettings.hpp'
]
install_headers(lukeInc, subdir: 'Luke')

lukeSrc = [
    'Luke/Display.cpp',
    'Luke/DisplayMode.cpp',
    'Luke/KeyEvents.cpp',
    'Luke/MouseEvents.cpp',
    'Luke/MouseState.cpp',
    'Luke/TextInputEvent.cpp',
    'Luke/Window.cpp',
    'Luke/WindowEvents.cpp',
    'Luke/WindowSettings.cpp'
]

if get_option('buildSubmodules') == false
    stickDep = meson.get_compiler('cpp').find_library('Stick')
    lukeDeps = [stickDep, dependency('threads')]
else
    stickProj = subproject('Stick')
    lukeDeps = [stickProj.get_variable('stickDep')]
endif

if get_option('implementation') == 'sdl'
    add_project_arguments('-DLUKE_IMPLEMENTATION_SDL', language: 'cpp')
    lukeDeps += dependency('sdl2')
    lukeSDLInc = [
        'Luke/SDL/SDLDisplayImpl.hpp',
        'Luke/SDL/SDLInitializer.hpp',
        'Luke/SDL/SDLWindowImpl.hpp'
    ]
    install_headers(lukeSDLInc, subdir: 'Luke/SDL')
    lukeSrc += [
        'Luke/SDL/SDLDisplayImpl.cpp',
        'Luke/SDL/SDLInitializer.cpp',
        'Luke/SDL/SDLWindowImpl.cpp'
    ]
elif get_option('implementation') == 'glfw'
    add_project_arguments('-DLUKE_IMPLEMENTATION_GLFW', language: 'cpp')
    lukeDeps += dependency('glfw3')
    lukeGLFWInc = [
        'Luke/GLFW/GLFWDisplayImpl.hpp',
        'Luke/GLFW/GLFWInitializer.hpp',
        'Luke/GLFW/GLFWWindowImpl.hpp'
    ]
    install_headers(lukeGLFWInc, subdir: 'Luke/GLFW')
    lukeSrc += [
        'Luke/GLFW/GLFWDisplayImpl.cpp',
        'Luke/GLFW/GLFWInitializer.cpp',
        'Luke/GLFW/GLFWWindowImpl.cpp'
    ]
else
    error('Unsupported implementation, sdl and glfw are the only supported ones as of now')
endif

incDirs = include_directories('.')

if get_option('static')
    luke = static_library('Luke', lukeSrc, dependencies: lukeDeps, include_directories : incDirs, install: true)
else
    luke = shared_library('Luke', lukeSrc, dependencies: lukeDeps, include_directories : incDirs, install: true)
endif

lukeDep = declare_dependency(link_with : luke, 
    dependencies: lukeDeps, 
    include_directories: incDirs)

# subdir('Playground')
