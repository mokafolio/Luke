extraCppArgs = []
if host_machine.system() == 'darwin'
    extraCppArgs += '-DGL_SILENCE_DEPRECATION'
endif
playground = executable('LukePlayground', 'LukePlayground.cpp', 
    dependencies: [lukeDep, dependency('GL')], 
    cpp_args : ['-fsanitize=address'] + extraCppArgs,
    link_args : '-fsanitize=address')