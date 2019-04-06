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
    - code generator (implement visitors and such) [done]
- create stage2 parsergenerator [done]
    - grammar.grm -> [generated stage1 grm parser] -> grammar.gen.{cpp,h} => stage2 grammar parser [done]
    - scaffolding/driver (like main(), parse() etc) [done]
    - code generator (implement visitors and such) [done]
    - scaffolding and code generator should be the same as for stage1 [done]
- stage1 and stage2 output should be identical [done]
- convert stage2 to selfcontained bootstrap-pg, disable stage0 and stage1 [done]
- create parsergenerator: grammar.grm -> [bootstrap-pg] -> grammar.gen.{cpp,h} (results in: (stage3) parsergenerator) [done]
- stage3 is for parsergenerator development, upon successful changes to stage3, stage2 will be updated from stage3 and stage3 regenerated from stage2 [done]

# Stage 6

- create stage 4 lilu compiler
    - lilu.grm -> [parsergenerator] -> Lilu.gen.{cpp,h} => stage4 lilu parser [done]
    - scaffolding/driver (like main(), parse() etc) [done]
    - AST generator (implement visitors and such) [done]
- add keywords automatically, so ID doesn't match them
- add AST mode to parser, so it generates AST directly instead of parse tree?

# Stage

- create LLVM IR from AST
- run LLVM IR

