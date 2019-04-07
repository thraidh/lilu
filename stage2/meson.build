gens2 = custom_target('make-grm-s2',
    input:'grammar.grm',
    output:['Grammar.gen.cpp','Grammar.gen.h','Grammar.visitors.h'],
    command:[parsergenerator,'@INPUT@','@OUTDIR@'])

parsergenerator = executable('parsergenerator2', 'main.cpp',
    'HeaderGenerator.cpp',
    'ImplementationGenerator.cpp',
    'VisitorGenerator.cpp',
    'AstHeaderGenerator.cpp',
    'Grammar.ast.h',
    'GrammarAnalyzer.cpp',
    'common.h',
    gens2,
    include_directories : inc,
    )
