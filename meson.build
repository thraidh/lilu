project('lilu', 'cpp')

add_project_arguments(['-std=c++17'], language : 'cpp')
add_project_link_arguments(['-lstdc++'], language : 'cpp')
#add_project_arguments(['-stdlib=libc++', '-std=c++2a'], language : 'cpp')
#add_project_link_arguments(['-stdlib=libc++', '-lc++'], language : 'cpp')

llvm = dependency('LLVM')

executable('lilu', 'src/main.cpp', 'src/parser.cpp', dependencies: [llvm])

