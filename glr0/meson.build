libdl = meson.get_compiler('cpp').find_library('dl')
glr0prog = executable('parsergenerator', 
    'parser.cpp', 'death_handler.cc','death_handler.h',
    dependencies: [libdl]
    )
