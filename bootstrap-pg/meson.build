#genstage2 = custom_target('make-grm2',
#    input:'grammar.grm',
#    output:['Grammar.gen.cpp','Grammar.gen.h','Grammar.visitors.h'],
#    command:[stage1prog,'@INPUT@','@OUTDIR@'])

#stage2prog = executable('stage2', 'main.cpp',
#    'HeaderGenerator.cpp',
#    'ImplementationGenerator.cpp',
#    'VisitorGenerator.cpp',
#    genstage2)

bootstrap_pg = executable('bootstrap_pg', 'main.cpp',
    'HeaderGenerator.cpp',
    'ImplementationGenerator.cpp',
    'VisitorGenerator.cpp',
    'Grammar.gen.cpp',
    'Grammar.gen.h',
    'Grammar.visitors.h'
    )

