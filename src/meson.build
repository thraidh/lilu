gensrc = custom_target('make-lilu',
    input:'lilu.grm',
    output:['lilu.gen.cpp','lilu.gen.h','lilu.visitors.h'],
    command:[parsergenerator,'@INPUT@','@OUTDIR@'])

llvm = dependency('LLVM')

executable('lilu', 'main.cpp', 'lilu.cpp', gensrc,  dependencies: [llvm])

