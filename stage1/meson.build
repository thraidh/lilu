genstage1 = custom_target('make-grm',
    input:'grammar.grm',
    output:['Grammar.gen.cpp','Grammar.gen.h','Grammar.visitors.h'],
    command:[stage0prog,'@INPUT@','@OUTDIR@'])

stage1prog = executable('stage1', 'main.cpp',
    'HeaderGenerator.cpp',
    'ImplementationGenerator.cpp',
    'VisitorGenerator.cpp',
    genstage1)

