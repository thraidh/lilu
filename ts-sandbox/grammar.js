const PREC_ASSIGN = 1
const PREC_OR = 2
const PREC_AND = 3
const PREC_EQ = 5
const PREC_PLUS = 10
const PREC_MUL = 11

module.exports = grammar({
    name: 'lilu',
    word: $ => $.ID,
    rules: {
        source_file: $ => repeat($._element),

        _element: $ => choice(
            $.funcdef,
            $.exprstmt
        ),

        funcdef: $ => seq(
            'function',
            alias($.ID, 'name'),
            '(',
            optional(seq($.arg, repeat(seq(',', $.arg)))),
            ')',
            ':',
            alias($.ID, 'rtype'),
            $.isblock
        ),

        isblock: $ => seq(
            'is',
            repeat($._element),
            'end'
        ),

        exprstmt: $ => $._expr,

        arg: $ => seq(alias($.ID, 'name'), ':', alias($.ID, 'type')),

        _expr: $ => choice(
            $.binop,
            $.ID,
        ),

        binop: $ => choice(
            prec.right(PREC_ASSIGN, seq($._expr, ':=', $._expr)),
            prec.left(PREC_OR, seq($._expr, 'or', $._expr)),
            prec.left(PREC_AND, seq($._expr, 'and', $._expr)),
            prec.left(PREC_EQ, seq($._expr, '=', $._expr)),
            prec.left(PREC_EQ, seq($._expr, '!=', $._expr)),
            prec.left(PREC_PLUS, seq($._expr, '+', $._expr)),
            prec.left(PREC_PLUS, seq($._expr, '-', $._expr)),
            prec.left(PREC_MUL, seq($._expr, '*', $._expr)),
            prec.left(PREC_MUL, seq($._expr, '/', $._expr)),
            prec.left(PREC_MUL, seq($._expr, '%', $._expr)),
        ),

        ID: $ => /[a-zA-Z_][a-zA-Z_0-9]*/,

        NUMBER: $ => /[0-9]+/
    }
});


/*
grammar Lilu;

lilufile ::= ^ block $;
block ::= element*;
element ::= funcdef | expr ;
extern expr;
args ::= [ arg [ "," arg ]* ]?;
arg ::= name=ID ":" type=ID;
funcdef ::= "function" name=ID "(" args ")" ":" rtype=ID "is" body=block "end";
primary ::= funcall | id | number | "(" expr ")" ;
id ::= ID;
number ::= NUMBER;
funcall ::= name=ID "(" [ expr [ "," expr ]* ]? ")";


  source_file: $ => repeat($._definition),

    _definition: $ => choice(
      $.function_definition
      // TODO: other kinds of definitions
    ),

    function_definition: $ => seq(
      'func',
      $.identifier,
      $.parameter_list,
      $._type,
      $.block
    ),

*/