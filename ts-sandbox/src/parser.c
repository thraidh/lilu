#include <tree_sitter/parser.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 10
#define STATE_COUNT 38
#define SYMBOL_COUNT 22
#define ALIAS_COUNT 3
#define TOKEN_COUNT 12
#define EXTERNAL_TOKEN_COUNT 0
#define FIELD_COUNT 0
#define MAX_ALIAS_SEQUENCE_LENGTH 9

enum {
  sym_ID = 1,
  anon_sym_function = 2,
  anon_sym_LPAREN = 3,
  anon_sym_COMMA = 4,
  anon_sym_RPAREN = 5,
  anon_sym_COLON = 6,
  anon_sym_is = 7,
  anon_sym_end = 8,
  anon_sym_PLUS = 9,
  anon_sym_STAR = 10,
  sym_NUMBER = 11,
  sym_source_file = 12,
  sym__element = 13,
  sym_funcdef = 14,
  sym_isblock = 15,
  sym_exprstmt = 16,
  sym_arg = 17,
  sym__expr = 18,
  sym_binop = 19,
  aux_sym_source_file_repeat1 = 20,
  aux_sym_funcdef_repeat1 = 21,
  anon_alias_sym_name = 22,
  anon_alias_sym_type = 23,
  anon_alias_sym_rtype = 24,
};

static const char *ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [sym_ID] = "ID",
  [anon_sym_function] = "function",
  [anon_sym_LPAREN] = "(",
  [anon_sym_COMMA] = ",",
  [anon_sym_RPAREN] = ")",
  [anon_sym_COLON] = ":",
  [anon_sym_is] = "is",
  [anon_sym_end] = "end",
  [anon_sym_PLUS] = "+",
  [anon_sym_STAR] = "*",
  [sym_NUMBER] = "NUMBER",
  [sym_source_file] = "source_file",
  [sym__element] = "_element",
  [sym_funcdef] = "funcdef",
  [sym_isblock] = "isblock",
  [sym_exprstmt] = "exprstmt",
  [sym_arg] = "arg",
  [sym__expr] = "_expr",
  [sym_binop] = "binop",
  [aux_sym_source_file_repeat1] = "source_file_repeat1",
  [aux_sym_funcdef_repeat1] = "funcdef_repeat1",
  [anon_alias_sym_name] = "name",
  [anon_alias_sym_type] = "type",
  [anon_alias_sym_rtype] = "rtype",
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {
    .visible = false,
    .named = true,
  },
  [sym_ID] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_function] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LPAREN] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_COMMA] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RPAREN] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_COLON] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_is] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_end] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_PLUS] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_STAR] = {
    .visible = true,
    .named = false,
  },
  [sym_NUMBER] = {
    .visible = true,
    .named = true,
  },
  [sym_source_file] = {
    .visible = true,
    .named = true,
  },
  [sym__element] = {
    .visible = false,
    .named = true,
  },
  [sym_funcdef] = {
    .visible = true,
    .named = true,
  },
  [sym_isblock] = {
    .visible = true,
    .named = true,
  },
  [sym_exprstmt] = {
    .visible = true,
    .named = true,
  },
  [sym_arg] = {
    .visible = true,
    .named = true,
  },
  [sym__expr] = {
    .visible = false,
    .named = true,
  },
  [sym_binop] = {
    .visible = true,
    .named = true,
  },
  [aux_sym_source_file_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_funcdef_repeat1] = {
    .visible = false,
    .named = false,
  },
  [anon_alias_sym_name] = {
    .visible = true,
    .named = false,
  },
  [anon_alias_sym_type] = {
    .visible = true,
    .named = false,
  },
  [anon_alias_sym_rtype] = {
    .visible = true,
    .named = false,
  },
};

static TSSymbol ts_alias_sequences[5][MAX_ALIAS_SEQUENCE_LENGTH] = {
  [1] = {
    [0] = anon_alias_sym_name,
    [2] = anon_alias_sym_type,
  },
  [2] = {
    [1] = anon_alias_sym_name,
    [5] = anon_alias_sym_rtype,
  },
  [3] = {
    [1] = anon_alias_sym_name,
    [6] = anon_alias_sym_rtype,
  },
  [4] = {
    [1] = anon_alias_sym_name,
    [7] = anon_alias_sym_rtype,
  },
};

static bool ts_lex(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  switch (state) {
    case 0:
      if (lookahead == 0)
        ADVANCE(1);
      if (lookahead == '(')
        ADVANCE(2);
      if (lookahead == ')')
        ADVANCE(3);
      if (lookahead == '*')
        ADVANCE(4);
      if (lookahead == '+')
        ADVANCE(5);
      if (lookahead == ',')
        ADVANCE(6);
      if (lookahead == ':')
        ADVANCE(7);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(0);
      if (('0' <= lookahead && lookahead <= '9'))
        ADVANCE(8);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(9);
      END_STATE();
    case 1:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 2:
      ACCEPT_TOKEN(anon_sym_LPAREN);
      END_STATE();
    case 3:
      ACCEPT_TOKEN(anon_sym_RPAREN);
      END_STATE();
    case 4:
      ACCEPT_TOKEN(anon_sym_STAR);
      END_STATE();
    case 5:
      ACCEPT_TOKEN(anon_sym_PLUS);
      END_STATE();
    case 6:
      ACCEPT_TOKEN(anon_sym_COMMA);
      END_STATE();
    case 7:
      ACCEPT_TOKEN(anon_sym_COLON);
      END_STATE();
    case 8:
      ACCEPT_TOKEN(sym_NUMBER);
      if (('0' <= lookahead && lookahead <= '9'))
        ADVANCE(8);
      END_STATE();
    case 9:
      ACCEPT_TOKEN(sym_ID);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(9);
      END_STATE();
    case 10:
      if (lookahead == 0)
        ADVANCE(1);
      if (lookahead == '*')
        ADVANCE(4);
      if (lookahead == '+')
        ADVANCE(5);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(10);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(9);
      END_STATE();
    case 11:
      if (lookahead == '(')
        ADVANCE(2);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(11);
      END_STATE();
    case 12:
      if (lookahead == ')')
        ADVANCE(3);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(12);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(9);
      END_STATE();
    case 13:
      if (lookahead == ':')
        ADVANCE(7);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(13);
      END_STATE();
    case 14:
      if (lookahead == ')')
        ADVANCE(3);
      if (lookahead == ',')
        ADVANCE(6);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(14);
      END_STATE();
    default:
      return false;
  }
}

static bool ts_lex_keywords(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  switch (state) {
    case 0:
      if (lookahead == 'e')
        ADVANCE(1);
      if (lookahead == 'f')
        ADVANCE(2);
      if (lookahead == 'i')
        ADVANCE(3);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(0);
      END_STATE();
    case 1:
      if (lookahead == 'n')
        ADVANCE(4);
      END_STATE();
    case 2:
      if (lookahead == 'u')
        ADVANCE(5);
      END_STATE();
    case 3:
      if (lookahead == 's')
        ADVANCE(6);
      END_STATE();
    case 4:
      if (lookahead == 'd')
        ADVANCE(7);
      END_STATE();
    case 5:
      if (lookahead == 'n')
        ADVANCE(8);
      END_STATE();
    case 6:
      ACCEPT_TOKEN(anon_sym_is);
      END_STATE();
    case 7:
      ACCEPT_TOKEN(anon_sym_end);
      END_STATE();
    case 8:
      if (lookahead == 'c')
        ADVANCE(9);
      END_STATE();
    case 9:
      if (lookahead == 't')
        ADVANCE(10);
      END_STATE();
    case 10:
      if (lookahead == 'i')
        ADVANCE(11);
      END_STATE();
    case 11:
      if (lookahead == 'o')
        ADVANCE(12);
      END_STATE();
    case 12:
      if (lookahead == 'n')
        ADVANCE(13);
      END_STATE();
    case 13:
      ACCEPT_TOKEN(anon_sym_function);
      END_STATE();
    default:
      return false;
  }
}

static TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0},
  [1] = {.lex_state = 10},
  [2] = {.lex_state = 10},
  [3] = {.lex_state = 10},
  [4] = {.lex_state = 10},
  [5] = {.lex_state = 0},
  [6] = {.lex_state = 11},
  [7] = {.lex_state = 10},
  [8] = {.lex_state = 10},
  [9] = {.lex_state = 10},
  [10] = {.lex_state = 12},
  [11] = {.lex_state = 10},
  [12] = {.lex_state = 10},
  [13] = {.lex_state = 13},
  [14] = {.lex_state = 13},
  [15] = {.lex_state = 14},
  [16] = {.lex_state = 10},
  [17] = {.lex_state = 10},
  [18] = {.lex_state = 10},
  [19] = {.lex_state = 13},
  [20] = {.lex_state = 14},
  [21] = {.lex_state = 14},
  [22] = {.lex_state = 10},
  [23] = {.lex_state = 14},
  [24] = {.lex_state = 10},
  [25] = {.lex_state = 13},
  [26] = {.lex_state = 14},
  [27] = {.lex_state = 10},
  [28] = {.lex_state = 10},
  [29] = {.lex_state = 10},
  [30] = {.lex_state = 10},
  [31] = {.lex_state = 10},
  [32] = {.lex_state = 10},
  [33] = {.lex_state = 10},
  [34] = {.lex_state = 10},
  [35] = {.lex_state = 10},
  [36] = {.lex_state = 10},
  [37] = {.lex_state = 10},
};

static uint16_t ts_parse_table[STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [anon_sym_COLON] = ACTIONS(1),
    [sym_ID] = ACTIONS(1),
    [ts_builtin_sym_end] = ACTIONS(1),
    [anon_sym_RPAREN] = ACTIONS(1),
    [anon_sym_function] = ACTIONS(1),
    [anon_sym_is] = ACTIONS(1),
    [sym_NUMBER] = ACTIONS(1),
    [anon_sym_LPAREN] = ACTIONS(1),
    [anon_sym_end] = ACTIONS(1),
    [anon_sym_STAR] = ACTIONS(1),
    [anon_sym_COMMA] = ACTIONS(1),
    [anon_sym_PLUS] = ACTIONS(1),
  },
  [1] = {
    [sym__expr] = STATE(3),
    [sym_exprstmt] = STATE(4),
    [sym__element] = STATE(4),
    [sym_funcdef] = STATE(4),
    [sym_binop] = STATE(3),
    [aux_sym_source_file_repeat1] = STATE(4),
    [sym_source_file] = STATE(5),
    [anon_sym_function] = ACTIONS(3),
    [sym_ID] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(7),
  },
  [2] = {
    [sym_ID] = ACTIONS(9),
  },
  [3] = {
    [sym_ID] = ACTIONS(11),
    [ts_builtin_sym_end] = ACTIONS(13),
    [anon_sym_end] = ACTIONS(11),
    [anon_sym_function] = ACTIONS(11),
    [anon_sym_STAR] = ACTIONS(15),
    [anon_sym_PLUS] = ACTIONS(17),
  },
  [4] = {
    [sym__expr] = STATE(3),
    [sym_exprstmt] = STATE(9),
    [sym__element] = STATE(9),
    [sym_funcdef] = STATE(9),
    [sym_binop] = STATE(3),
    [aux_sym_source_file_repeat1] = STATE(9),
    [anon_sym_function] = ACTIONS(3),
    [sym_ID] = ACTIONS(5),
    [ts_builtin_sym_end] = ACTIONS(19),
  },
  [5] = {
    [ts_builtin_sym_end] = ACTIONS(21),
  },
  [6] = {
    [anon_sym_LPAREN] = ACTIONS(23),
  },
  [7] = {
    [sym__expr] = STATE(11),
    [sym_binop] = STATE(11),
    [sym_ID] = ACTIONS(25),
  },
  [8] = {
    [sym__expr] = STATE(12),
    [sym_binop] = STATE(12),
    [sym_ID] = ACTIONS(27),
  },
  [9] = {
    [sym__expr] = STATE(3),
    [sym_exprstmt] = STATE(9),
    [sym__element] = STATE(9),
    [sym_funcdef] = STATE(9),
    [sym_binop] = STATE(3),
    [aux_sym_source_file_repeat1] = STATE(9),
    [anon_sym_function] = ACTIONS(29),
    [sym_ID] = ACTIONS(32),
    [ts_builtin_sym_end] = ACTIONS(35),
  },
  [10] = {
    [sym_arg] = STATE(15),
    [sym_ID] = ACTIONS(37),
    [anon_sym_RPAREN] = ACTIONS(39),
  },
  [11] = {
    [sym_ID] = ACTIONS(41),
    [ts_builtin_sym_end] = ACTIONS(43),
    [anon_sym_end] = ACTIONS(41),
    [anon_sym_function] = ACTIONS(41),
    [anon_sym_STAR] = ACTIONS(43),
    [anon_sym_PLUS] = ACTIONS(43),
  },
  [12] = {
    [sym_ID] = ACTIONS(41),
    [ts_builtin_sym_end] = ACTIONS(43),
    [anon_sym_end] = ACTIONS(41),
    [anon_sym_function] = ACTIONS(41),
    [anon_sym_STAR] = ACTIONS(15),
    [anon_sym_PLUS] = ACTIONS(43),
  },
  [13] = {
    [anon_sym_COLON] = ACTIONS(45),
  },
  [14] = {
    [anon_sym_COLON] = ACTIONS(47),
  },
  [15] = {
    [aux_sym_funcdef_repeat1] = STATE(20),
    [anon_sym_COMMA] = ACTIONS(49),
    [anon_sym_RPAREN] = ACTIONS(51),
  },
  [16] = {
    [sym_ID] = ACTIONS(53),
  },
  [17] = {
    [sym_ID] = ACTIONS(55),
  },
  [18] = {
    [sym_arg] = STATE(23),
    [sym_ID] = ACTIONS(37),
  },
  [19] = {
    [anon_sym_COLON] = ACTIONS(57),
  },
  [20] = {
    [aux_sym_funcdef_repeat1] = STATE(26),
    [anon_sym_COMMA] = ACTIONS(49),
    [anon_sym_RPAREN] = ACTIONS(59),
  },
  [21] = {
    [anon_sym_COMMA] = ACTIONS(61),
    [anon_sym_RPAREN] = ACTIONS(61),
  },
  [22] = {
    [sym_isblock] = STATE(28),
    [anon_sym_is] = ACTIONS(63),
  },
  [23] = {
    [anon_sym_COMMA] = ACTIONS(65),
    [anon_sym_RPAREN] = ACTIONS(65),
  },
  [24] = {
    [sym_ID] = ACTIONS(67),
  },
  [25] = {
    [anon_sym_COLON] = ACTIONS(69),
  },
  [26] = {
    [aux_sym_funcdef_repeat1] = STATE(26),
    [anon_sym_COMMA] = ACTIONS(71),
    [anon_sym_RPAREN] = ACTIONS(65),
  },
  [27] = {
    [sym__expr] = STATE(3),
    [sym_exprstmt] = STATE(32),
    [sym__element] = STATE(32),
    [sym_funcdef] = STATE(32),
    [sym_binop] = STATE(3),
    [aux_sym_source_file_repeat1] = STATE(32),
    [anon_sym_function] = ACTIONS(3),
    [sym_ID] = ACTIONS(5),
    [anon_sym_end] = ACTIONS(74),
  },
  [28] = {
    [sym_ID] = ACTIONS(76),
    [ts_builtin_sym_end] = ACTIONS(78),
    [anon_sym_end] = ACTIONS(76),
    [anon_sym_function] = ACTIONS(76),
  },
  [29] = {
    [sym_isblock] = STATE(33),
    [anon_sym_is] = ACTIONS(63),
  },
  [30] = {
    [sym_ID] = ACTIONS(80),
  },
  [31] = {
    [sym_ID] = ACTIONS(82),
    [ts_builtin_sym_end] = ACTIONS(84),
    [anon_sym_end] = ACTIONS(82),
    [anon_sym_function] = ACTIONS(82),
  },
  [32] = {
    [sym__expr] = STATE(3),
    [sym_exprstmt] = STATE(36),
    [sym__element] = STATE(36),
    [sym_funcdef] = STATE(36),
    [sym_binop] = STATE(3),
    [aux_sym_source_file_repeat1] = STATE(36),
    [anon_sym_function] = ACTIONS(3),
    [sym_ID] = ACTIONS(5),
    [anon_sym_end] = ACTIONS(86),
  },
  [33] = {
    [sym_ID] = ACTIONS(88),
    [ts_builtin_sym_end] = ACTIONS(90),
    [anon_sym_end] = ACTIONS(88),
    [anon_sym_function] = ACTIONS(88),
  },
  [34] = {
    [sym_isblock] = STATE(37),
    [anon_sym_is] = ACTIONS(63),
  },
  [35] = {
    [sym_ID] = ACTIONS(92),
    [ts_builtin_sym_end] = ACTIONS(94),
    [anon_sym_end] = ACTIONS(92),
    [anon_sym_function] = ACTIONS(92),
  },
  [36] = {
    [sym__expr] = STATE(3),
    [sym_exprstmt] = STATE(36),
    [sym__element] = STATE(36),
    [sym_funcdef] = STATE(36),
    [sym_binop] = STATE(3),
    [aux_sym_source_file_repeat1] = STATE(36),
    [anon_sym_function] = ACTIONS(29),
    [sym_ID] = ACTIONS(32),
    [anon_sym_end] = ACTIONS(96),
  },
  [37] = {
    [sym_ID] = ACTIONS(98),
    [ts_builtin_sym_end] = ACTIONS(100),
    [anon_sym_end] = ACTIONS(98),
    [anon_sym_function] = ACTIONS(98),
  },
};

static TSParseActionEntry ts_parse_actions[] = {
  [0] = {.count = 0, .reusable = false},
  [1] = {.count = 1, .reusable = false}, RECOVER(),
  [3] = {.count = 1, .reusable = false}, SHIFT(2),
  [5] = {.count = 1, .reusable = false}, SHIFT(3),
  [7] = {.count = 1, .reusable = true}, REDUCE(sym_source_file, 0),
  [9] = {.count = 1, .reusable = true}, SHIFT(6),
  [11] = {.count = 1, .reusable = false}, REDUCE(sym_exprstmt, 1),
  [13] = {.count = 1, .reusable = true}, REDUCE(sym_exprstmt, 1),
  [15] = {.count = 1, .reusable = true}, SHIFT(7),
  [17] = {.count = 1, .reusable = true}, SHIFT(8),
  [19] = {.count = 1, .reusable = true}, REDUCE(sym_source_file, 1),
  [21] = {.count = 1, .reusable = true},  ACCEPT_INPUT(),
  [23] = {.count = 1, .reusable = true}, SHIFT(10),
  [25] = {.count = 1, .reusable = true}, SHIFT(11),
  [27] = {.count = 1, .reusable = true}, SHIFT(12),
  [29] = {.count = 2, .reusable = false}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(2),
  [32] = {.count = 2, .reusable = false}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(3),
  [35] = {.count = 1, .reusable = true}, REDUCE(aux_sym_source_file_repeat1, 2),
  [37] = {.count = 1, .reusable = true}, SHIFT(13),
  [39] = {.count = 1, .reusable = true}, SHIFT(14),
  [41] = {.count = 1, .reusable = false}, REDUCE(sym_binop, 3),
  [43] = {.count = 1, .reusable = true}, REDUCE(sym_binop, 3),
  [45] = {.count = 1, .reusable = true}, SHIFT(16),
  [47] = {.count = 1, .reusable = true}, SHIFT(17),
  [49] = {.count = 1, .reusable = true}, SHIFT(18),
  [51] = {.count = 1, .reusable = true}, SHIFT(19),
  [53] = {.count = 1, .reusable = true}, SHIFT(21),
  [55] = {.count = 1, .reusable = true}, SHIFT(22),
  [57] = {.count = 1, .reusable = true}, SHIFT(24),
  [59] = {.count = 1, .reusable = true}, SHIFT(25),
  [61] = {.count = 1, .reusable = true}, REDUCE(sym_arg, 3, .production_id = 1),
  [63] = {.count = 1, .reusable = true}, SHIFT(27),
  [65] = {.count = 1, .reusable = true}, REDUCE(aux_sym_funcdef_repeat1, 2),
  [67] = {.count = 1, .reusable = true}, SHIFT(29),
  [69] = {.count = 1, .reusable = true}, SHIFT(30),
  [71] = {.count = 2, .reusable = true}, REDUCE(aux_sym_funcdef_repeat1, 2), SHIFT_REPEAT(18),
  [74] = {.count = 1, .reusable = false}, SHIFT(31),
  [76] = {.count = 1, .reusable = false}, REDUCE(sym_funcdef, 7, .production_id = 2),
  [78] = {.count = 1, .reusable = true}, REDUCE(sym_funcdef, 7, .production_id = 2),
  [80] = {.count = 1, .reusable = true}, SHIFT(34),
  [82] = {.count = 1, .reusable = false}, REDUCE(sym_isblock, 2),
  [84] = {.count = 1, .reusable = true}, REDUCE(sym_isblock, 2),
  [86] = {.count = 1, .reusable = false}, SHIFT(35),
  [88] = {.count = 1, .reusable = false}, REDUCE(sym_funcdef, 8, .production_id = 3),
  [90] = {.count = 1, .reusable = true}, REDUCE(sym_funcdef, 8, .production_id = 3),
  [92] = {.count = 1, .reusable = false}, REDUCE(sym_isblock, 3),
  [94] = {.count = 1, .reusable = true}, REDUCE(sym_isblock, 3),
  [96] = {.count = 1, .reusable = false}, REDUCE(aux_sym_source_file_repeat1, 2),
  [98] = {.count = 1, .reusable = false}, REDUCE(sym_funcdef, 9, .production_id = 4),
  [100] = {.count = 1, .reusable = true}, REDUCE(sym_funcdef, 9, .production_id = 4),
};

#ifdef _WIN32
#define extern __declspec(dllexport)
#endif

extern const TSLanguage *tree_sitter_lilu(void) {
  static TSLanguage language = {
    .version = LANGUAGE_VERSION,
    .symbol_count = SYMBOL_COUNT,
    .alias_count = ALIAS_COUNT,
    .token_count = TOKEN_COUNT,
    .symbol_metadata = ts_symbol_metadata,
    .parse_table = (const unsigned short *)ts_parse_table,
    .parse_actions = ts_parse_actions,
    .lex_modes = ts_lex_modes,
    .symbol_names = ts_symbol_names,
    .alias_sequences = (const TSSymbol *)ts_alias_sequences,
    .field_count = FIELD_COUNT,
    .max_alias_sequence_length = MAX_ALIAS_SEQUENCE_LENGTH,
    .lex_fn = ts_lex,
    .keyword_lex_fn = ts_lex_keywords,
    .keyword_capture_token = sym_ID,
    .external_token_count = EXTERNAL_TOKEN_COUNT,
  };
  return &language;
}
