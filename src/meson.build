gensrc = custom_target('make-lilu',
    input:'lilu.grm',
    output:['Lilu.gen.cpp','Lilu.gen.h','Lilu.visitors.h'],
    command:[parsergenerator,'@INPUT@','@OUTDIR@'])

llvm = dependency('LLVM')

executable('lilu',
    'main.cpp', 'lilu.cpp',
    gensrc,
    dependencies: [llvm],
    include_directories : inc,
    )

