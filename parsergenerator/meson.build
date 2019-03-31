genpg = custom_target('make-grm-pg',
    input:'grammar.grm',
    output:['Grammar.gen.cpp','Grammar.gen.h','Grammar.visitors.h'],
    command:[bootstrap_pg,'@INPUT@','@OUTDIR@'])

parsergenerator = executable('parsergenerator2', 'main.cpp',
    'HeaderGenerator.cpp',
    'ImplementationGenerator.cpp',
    'VisitorGenerator.cpp',
    genpg,
    include_directories : inc,
    )
