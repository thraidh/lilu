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

# Stage 7

- create LLVM IR from AST [done]
- run LLVM IR [done]

# Stage 8

- use ORC JIT [done]

# Stage 9

- implement function calls [done]

# Stage 10

- parser should produce AST instead of parse tree
    - a rule with only an alternation and no explicit names, passes its result through
    - all other rules produce an AST node
    - explicitly named results are added to that name in the current AST node
    - not explicitly named rule invocations are added to that name in the current AST node
    - if it is possible that multiple values can be added to a name it is a vector<AstNode*>, otherwise "AstNode*"
    - if a name occurs in a * or + repeated context, it is automatically multi
    - "name=(x)" makes all x go into the name
        - x and all its subrules (also x) are placed into "name" context
        - if x is a sequence, pass through name context to all elements, except literals
        - if x is a repetition, pass name and multi=true context to repeated rule
        - if x is an alternation, pass name to each alt
        - if x is a literal or terminal, or rule call, convert it to AstNode and add the result to the name of the context
        - in any context: if x is a terminal or rule call, add its AstNode result to to positional elements of the rule
    - what needs to be generated
        - enum
        - ast nodes
        - parser code
        - visitors
    - AstNodeInfo
        - `name: string`
        - `names: map<string, NameInfo>`
        - `rule: GrammarAstNode`
        - `isNecessary: bool`
        - `nodeTypeId: int`
        - `actualType: string` (differs from name+"_Node", if this node is just a proxy for a terminal)
    - NameInfo
        - isMulti: bool
        - type: string (contains name of type narrowest type for values in this name)
    - to generate target language use two passes
        - first pass discovers structure of target AST nodes
            - if subrule is seq, isNecessary=true
            - if subrule is repetition, isNecessary=true
            - if subrule is named, isNecessary=true
            - if subrule is terminal, isNecessary=true
            - if rule just contains a terminal, the AST node should be a that Terminal type with a new nodeTypeId
                - id ::= ID
                - id_Node does not exist (it is actually ID_Node), but id_NodeTypeId exists
                - instead of id_Node.children.push_back(ID_Node), do ID_Node.nodeTypeId=id_NodeTypeId, return ID_Node
            - a rule that results in just one not explicitly named result, will just pass that result through
                - this can happen for
                    - alternation alts, which are just one rule invocation
                    - repetitions, where exactly one element is matched
                    - sequences, where some elements are optional
                - the check has to be done at runtime
                    - eg: named ::= [ name=ID "=" ]? element
                    - if the optional clause is not matched, there is only element and the result can be passed through
                    - if the optional is matched, we need to create a new AST node
                - every subrule has a
                    - min: minimal number of results
                    - one: number of results if one element is given (eg in an optional or repetition)
                    - isnamed: if this subrule or its elements are named
                - don't need to do a runtime check for passthrough, if
                    - min>1
                    - min==1 && isnamed
                    - min==0 && one>1
                - rules for calculation
                    - literal
                        - min: 0
                        - one: 0
                        - isnamed: false
                    - terminal, rulecall/ref, special
                        - min: 1
                        - one: 1
                        - isnamed: false
                    - named literal
                        - min: 1
                        - one: 1
                        - isnamed: true
                    - named other
                        - min: other.min
                        - one: other.one
                        - isnamed: true
                    - alt
                        - min: minimum of all alts.min
                        - one: minimum of all alts.one
                        - isnamed: true if for all alts a: a.min>0 && a.isnamed (otherwise there can be an unnamed alt)
                    - seq
                        - min: sum of elem.min
                        - one: min/i(min + elem[i].one - elem[i].min) (pretend all elements are at min, except for one. if even the smallest of those is above 1, the result can never be a passthrough)
                        - isnamed: true if for any elem e: e.min>0 && e.isnamed
                    - rep *
                        - min: 0
                        - one: elem.one (the smallest possible non-empty result)
                        - isnamed: false (in the min case, there is nothing named)
                    - rep ?
                        - min: 0
                        - one: elem.one
                        - isnamed: false
                    - rep +
                        - min: elem.min (but it is an error to repeat something that could be empty)
                        - one: elem.one
                        - isnamed: elem.isnamed (min is at least 1)
                    - capture, noncapture
                        - min: elem.min
                        - one: elem.one
                        - isnamed: elem.isnamed
            - each subrule carries information about the vars (implicit names, explicit names, positional) that may be changed, if the subrule was successful
                - on failure the subrule cleans up after itself and reverts its changes
                - on success the calling subrule is owner of the changes and has to clean them up, if it fails
        - second pass generates sources
    - AstNode
        - nodeTypeId: enum (generated with one entry for each node)
        - text: string_view (all the matched text)
    - TerminalNode: AstNode
    - ID_Node: TerminalNode
    - NUMBER_Node: TerminalNode
        - num: int
    - STRING_Node: TerminalNode
        - str: string (text without quotes but with replaced control sequences)
    - KeywordNode: TerminalNode
    - InnerNode: AstNode
        - children: vector<AstNode*> (positional children)
    - _rulename_`_Node`: InnerNode
        - _name_: AstNode* (named child can occur 0-1 times)
        - _name_: vector<AstNode*> (named children can occur 0-n times, n>=2)
    - enum AstNodeTypeId
        - _rulename_`_NodeTypeId`
    - add possibility to add nodes, that are mentioned in visitor and enum, but are not generated
    - add possibility to add names to nodes
    - add possibility to change parent of node
    - add possibility to change type of names in nodes
        - if type for a named result is changed to string it will only contain the text of the result
        - if type is bool it is true if the name is filled, otherwise false
    - subrules
        - `AstNode* `_rulename_`_seq_`_num_`(`_rulename_`Node* outer, Cursor* c)` returns null, if not matched, implicitly knows about name context and multi context from AstNodeInfo
    - visitors can have a variable number of additional args
    - ParsingSession
        - text: string (the full text, that should be parsed)
        - charToLine: map<int, int> (maps character index of start of line to line number)
- rewrite lilu to use the new parse results
- rewrite parser generator
    - bootstrap is version N, generates version N code
    - stage 1 is generated by bootstrap using version N output and version N visitors
    - stage 1 generates stage 2 using version N generator and visitors, generates version N output
    - stage 2 uses version N generated code and generates version N code (copy of stage 1)
    - stage 1 is updated to generate version N+1 code
    - stage 2 is generated using version N+1 code
        - must be updated to use version N+1 visitors
        - must be updated to generate version N+1 code
    - code generated by stage 1 and 2 should be identical
    - bootstrap and stage 1 should be replaced with stage 2, stage 2 can be removed
    - stage 1 is parsergenerator
    - stage 2 is temporary during development

# Stage

- implement variables
- implement multiple statements
- implement "if"
- implement "while"

# Stage

- add keywords automatically, so ID doesn't match them
- add AST mode to parser, so it generates AST directly instead of parse tree?
