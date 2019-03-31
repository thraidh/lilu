# Stage 1

- compile simple c++ program with meson

# Stage 2

- add LLVM libs

# Stage 3

- steal example
- call c function from LLVM code

# Stage 4

- load file [done]
- implement internal DSL parser (stage0) [done]
- implement recursive-descent parser [done]
- implement expression precedence parser [done]
- create AST [done]
- create parsergenerator: lilu.grm -> [hardcoded IDSL stage0 grm parser] -> lilu.gen.{cpp,h} (results in: stage1 lilu parser) [done]

# Stage 5

- generate visitor output for stage1 parser [done]
- create stage1 parsergenerator
    - grammar.grm -> [hardcoded IDSL stage0 grm parser] -> grammar.gen.{cpp,h} => stage1 grammar parser [done]
    - scaffolding/driver (like main(), parse() etc) [done]
    - code generator (implement visitors and such)
- create stage2 parsergenerator
    - grammar.grm -> [generated stage1 grm parser] -> grammar.gen.{cpp,h} => stage2 grammar parser
    - scaffolding/driver (like main(), parse() etc)
    - code generator (implement visitors and such)
    - scaffolding and code generator should be the same as for stage1
- stage1 and stage2 output should be identical
- stage2 is now bootstrap
- create parsergenerator: grammar.grm -> [generated stage2 grm parser] -> grammar.gen.{cpp,h} (results in: stage3 grammar parser)
- stage3 is for parsergenerator development, upon successful changes to stage3, stage2 will be updated from stage3 and stage3 regenerated from stage2

# Stage 6

- create parsergenerator: lilu.grm -> [generated stage3 grm parser] -> grammar.gen.{cpp,h} (results in: stage4 lilu parser)


# Stage

- create LLVM IR from AST
- run LLVM IR

