#include <tree_sitter/parser.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 10
#define STATE_COUNT 77
#define SYMBOL_COUNT 38
#define ALIAS_COUNT 3
#define TOKEN_COUNT 23
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
  anon_sym_if = 9,
  anon_sym_then = 10,
  anon_sym_else = 11,
  anon_sym_COLON_EQ = 12,
  anon_sym_or = 13,
  anon_sym_and = 14,
  anon_sym_EQ = 15,
  anon_sym_BANG_EQ = 16,
  anon_sym_PLUS = 17,
  anon_sym_DASH = 18,
  anon_sym_STAR = 19,
  anon_sym_SLASH = 20,
  anon_sym_PERCENT = 21,
  sym_NUMBER = 22,
  sym_source_file = 23,
  sym__element = 24,
  sym_funcdef = 25,
  sym_isblock = 26,
  sym_exprstmt = 27,
  sym_ifstmt = 28,
  sym_thenblock = 29,
  sym_elseifblock = 30,
  sym_elseblock = 31,
  sym_arg = 32,
  sym__expr = 33,
  sym_binop = 34,
  aux_sym_source_file_repeat1 = 35,
  aux_sym_funcdef_repeat1 = 36,
  aux_sym_ifstmt_repeat1 = 37,
  anon_alias_sym_name = 38,
  anon_alias_sym_type = 39,
  anon_alias_sym_rtype = 40,
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
  [anon_sym_if] = "if",
  [anon_sym_then] = "then",
  [anon_sym_else] = "else",
  [anon_sym_COLON_EQ] = ":=",
  [anon_sym_or] = "or",
  [anon_sym_and] = "and",
  [anon_sym_EQ] = "=",
  [anon_sym_BANG_EQ] = "!=",
  [anon_sym_PLUS] = "+",
  [anon_sym_DASH] = "-",
  [anon_sym_STAR] = "*",
  [anon_sym_SLASH] = "/",
  [anon_sym_PERCENT] = "%",
  [sym_NUMBER] = "NUMBER",
  [sym_source_file] = "source_file",
  [sym__element] = "_element",
  [sym_funcdef] = "funcdef",
  [sym_isblock] = "isblock",
  [sym_exprstmt] = "exprstmt",
  [sym_ifstmt] = "ifstmt",
  [sym_thenblock] = "thenblock",
  [sym_elseifblock] = "elseifblock",
  [sym_elseblock] = "elseblock",
  [sym_arg] = "arg",
  [sym__expr] = "_expr",
  [sym_binop] = "binop",
  [aux_sym_source_file_repeat1] = "source_file_repeat1",
  [aux_sym_funcdef_repeat1] = "funcdef_repeat1",
  [aux_sym_ifstmt_repeat1] = "ifstmt_repeat1",
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
  [anon_sym_if] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_then] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_else] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_COLON_EQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_or] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_and] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_EQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_BANG_EQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_PLUS] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DASH] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_STAR] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_SLASH] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_PERCENT] = {
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
  [sym_ifstmt] = {
    .visible = true,
    .named = true,
  },
  [sym_thenblock] = {
    .visible = true,
    .named = true,
  },
  [sym_elseifblock] = {
    .visible = true,
    .named = true,
  },
  [sym_elseblock] = {
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
  [aux_sym_ifstmt_repeat1] = {
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
      if (lookahead == '!')
        ADVANCE(2);
      if (lookahead == '%')
        ADVANCE(3);
      if (lookahead == '(')
        ADVANCE(4);
      if (lookahead == ')')
        ADVANCE(5);
      if (lookahead == '*')
        ADVANCE(6);
      if (lookahead == '+')
        ADVANCE(7);
      if (lookahead == ',')
        ADVANCE(8);
      if (lookahead == '-')
        ADVANCE(9);
      if (lookahead == '/')
        ADVANCE(10);
      if (lookahead == ':')
        ADVANCE(11);
      if (lookahead == '=')
        ADVANCE(12);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(0);
      if (('0' <= lookahead && lookahead <= '9'))
        ADVANCE(13);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(14);
      END_STATE();
    case 1:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 2:
      if (lookahead == '=')
        ADVANCE(15);
      END_STATE();
    case 3:
      ACCEPT_TOKEN(anon_sym_PERCENT);
      END_STATE();
    case 4:
      ACCEPT_TOKEN(anon_sym_LPAREN);
      END_STATE();
    case 5:
      ACCEPT_TOKEN(anon_sym_RPAREN);
      END_STATE();
    case 6:
      ACCEPT_TOKEN(anon_sym_STAR);
      END_STATE();
    case 7:
      ACCEPT_TOKEN(anon_sym_PLUS);
      END_STATE();
    case 8:
      ACCEPT_TOKEN(anon_sym_COMMA);
      END_STATE();
    case 9:
      ACCEPT_TOKEN(anon_sym_DASH);
      END_STATE();
    case 10:
      ACCEPT_TOKEN(anon_sym_SLASH);
      END_STATE();
    case 11:
      ACCEPT_TOKEN(anon_sym_COLON);
      if (lookahead == '=')
        ADVANCE(16);
      END_STATE();
    case 12:
      ACCEPT_TOKEN(anon_sym_EQ);
      END_STATE();
    case 13:
      ACCEPT_TOKEN(sym_NUMBER);
      if (('0' <= lookahead && lookahead <= '9'))
        ADVANCE(13);
      END_STATE();
    case 14:
      ACCEPT_TOKEN(sym_ID);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(14);
      END_STATE();
    case 15:
      ACCEPT_TOKEN(anon_sym_BANG_EQ);
      END_STATE();
    case 16:
      ACCEPT_TOKEN(anon_sym_COLON_EQ);
      END_STATE();
    case 17:
      if (lookahead == 0)
        ADVANCE(1);
      if (lookahead == '!')
        ADVANCE(2);
      if (lookahead == '%')
        ADVANCE(3);
      if (lookahead == '*')
        ADVANCE(6);
      if (lookahead == '+')
        ADVANCE(7);
      if (lookahead == '-')
        ADVANCE(9);
      if (lookahead == '/')
        ADVANCE(10);
      if (lookahead == ':')
        ADVANCE(18);
      if (lookahead == '=')
        ADVANCE(12);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(17);
      if (('0' <= lookahead && lookahead <= '9'))
        ADVANCE(13);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(14);
      END_STATE();
    case 18:
      if (lookahead == '=')
        ADVANCE(16);
      END_STATE();
    case 19:
      if (lookahead == '(')
        ADVANCE(4);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(19);
      END_STATE();
    case 20:
      if (lookahead == ')')
        ADVANCE(5);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(20);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z'))
        ADVANCE(14);
      END_STATE();
    case 21:
      if (lookahead == ':')
        ADVANCE(22);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(21);
      END_STATE();
    case 22:
      ACCEPT_TOKEN(anon_sym_COLON);
      END_STATE();
    case 23:
      if (lookahead == ')')
        ADVANCE(5);
      if (lookahead == ',')
        ADVANCE(8);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(23);
      END_STATE();
    default:
      return false;
  }
}

static bool ts_lex_keywords(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  switch (state) {
    case 0:
      if (lookahead == 'a')
        ADVANCE(1);
      if (lookahead == 'e')
        ADVANCE(2);
      if (lookahead == 'f')
        ADVANCE(3);
      if (lookahead == 'i')
        ADVANCE(4);
      if (lookahead == 'o')
        ADVANCE(5);
      if (lookahead == 't')
        ADVANCE(6);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ')
        SKIP(0);
      END_STATE();
    case 1:
      if (lookahead == 'n')
        ADVANCE(7);
      END_STATE();
    case 2:
      if (lookahead == 'l')
        ADVANCE(8);
      if (lookahead == 'n')
        ADVANCE(9);
      END_STATE();
    case 3:
      if (lookahead == 'u')
        ADVANCE(10);
      END_STATE();
    case 4:
      if (lookahead == 'f')
        ADVANCE(11);
      if (lookahead == 's')
        ADVANCE(12);
      END_STATE();
    case 5:
      if (lookahead == 'r')
        ADVANCE(13);
      END_STATE();
    case 6:
      if (lookahead == 'h')
        ADVANCE(14);
      END_STATE();
    case 7:
      if (lookahead == 'd')
        ADVANCE(15);
      END_STATE();
    case 8:
      if (lookahead == 's')
        ADVANCE(16);
      END_STATE();
    case 9:
      if (lookahead == 'd')
        ADVANCE(17);
      END_STATE();
    case 10:
      if (lookahead == 'n')
        ADVANCE(18);
      END_STATE();
    case 11:
      ACCEPT_TOKEN(anon_sym_if);
      END_STATE();
    case 12:
      ACCEPT_TOKEN(anon_sym_is);
      END_STATE();
    case 13:
      ACCEPT_TOKEN(anon_sym_or);
      END_STATE();
    case 14:
      if (lookahead == 'e')
        ADVANCE(19);
      END_STATE();
    case 15:
      ACCEPT_TOKEN(anon_sym_and);
      END_STATE();
    case 16:
      if (lookahead == 'e')
        ADVANCE(20);
      END_STATE();
    case 17:
      ACCEPT_TOKEN(anon_sym_end);
      END_STATE();
    case 18:
      if (lookahead == 'c')
        ADVANCE(21);
      END_STATE();
    case 19:
      if (lookahead == 'n')
        ADVANCE(22);
      END_STATE();
    case 20:
      ACCEPT_TOKEN(anon_sym_else);
      END_STATE();
    case 21:
      if (lookahead == 't')
        ADVANCE(23);
      END_STATE();
    case 22:
      ACCEPT_TOKEN(anon_sym_then);
      END_STATE();
    case 23:
      if (lookahead == 'i')
        ADVANCE(24);
      END_STATE();
    case 24:
      if (lookahead == 'o')
        ADVANCE(25);
      END_STATE();
    case 25:
      if (lookahead == 'n')
        ADVANCE(26);
      END_STATE();
    case 26:
      ACCEPT_TOKEN(anon_sym_function);
      END_STATE();
    default:
      return false;
  }
}

static TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0},
  [1] = {.lex_state = 17},
  [2] = {.lex_state = 17},
  [3] = {.lex_state = 17},
  [4] = {.lex_state = 0},
  [5] = {.lex_state = 17},
  [6] = {.lex_state = 17},
  [7] = {.lex_state = 19},
  [8] = {.lex_state = 17},
  [9] = {.lex_state = 17},
  [10] = {.lex_state = 17},
  [11] = {.lex_state = 17},
  [12] = {.lex_state = 17},
  [13] = {.lex_state = 17},
  [14] = {.lex_state = 17},
  [15] = {.lex_state = 17},
  [16] = {.lex_state = 20},
  [17] = {.lex_state = 17},
  [18] = {.lex_state = 17},
  [19] = {.lex_state = 17},
  [20] = {.lex_state = 17},
  [21] = {.lex_state = 17},
  [22] = {.lex_state = 17},
  [23] = {.lex_state = 17},
  [24] = {.lex_state = 17},
  [25] = {.lex_state = 17},
  [26] = {.lex_state = 21},
  [27] = {.lex_state = 21},
  [28] = {.lex_state = 23},
  [29] = {.lex_state = 17},
  [30] = {.lex_state = 17},
  [31] = {.lex_state = 17},
  [32] = {.lex_state = 17},
  [33] = {.lex_state = 17},
  [34] = {.lex_state = 17},
  [35] = {.lex_state = 17},
  [36] = {.lex_state = 17},
  [37] = {.lex_state = 17},
  [38] = {.lex_state = 17},
  [39] = {.lex_state = 21},
  [40] = {.lex_state = 23},
  [41] = {.lex_state = 17},
  [42] = {.lex_state = 17},
  [43] = {.lex_state = 17},
  [44] = {.lex_state = 17},
  [45] = {.lex_state = 17},
  [46] = {.lex_state = 17},
  [47] = {.lex_state = 17},
  [48] = {.lex_state = 17},
  [49] = {.lex_state = 23},
  [50] = {.lex_state = 17},
  [51] = {.lex_state = 23},
  [52] = {.lex_state = 17},
  [53] = {.lex_state = 21},
  [54] = {.lex_state = 23},
  [55] = {.lex_state = 17},
  [56] = {.lex_state = 17},
  [57] = {.lex_state = 17},
  [58] = {.lex_state = 17},
  [59] = {.lex_state = 17},
  [60] = {.lex_state = 17},
  [61] = {.lex_state = 17},
  [62] = {.lex_state = 17},
  [63] = {.lex_state = 17},
  [64] = {.lex_state = 17},
  [65] = {.lex_state = 17},
  [66] = {.lex_state = 17},
  [67] = {.lex_state = 17},
  [68] = {.lex_state = 17},
  [69] = {.lex_state = 17},
  [70] = {.lex_state = 17},
  [71] = {.lex_state = 17},
  [72] = {.lex_state = 17},
  [73] = {.lex_state = 17},
  [74] = {.lex_state = 17},
  [75] = {.lex_state = 17},
  [76] = {.lex_state = 17},
};

static uint16_t ts_parse_table[STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [sym_ID] = ACTIONS(1),
    [anon_sym_else] = ACTIONS(1),
    [anon_sym_BANG_EQ] = ACTIONS(1),
    [anon_sym_RPAREN] = ACTIONS(1),
    [anon_sym_function] = ACTIONS(1),
    [anon_sym_is] = ACTIONS(1),
    [anon_sym_STAR] = ACTIONS(1),
    [anon_sym_or] = ACTIONS(1),
    [anon_sym_then] = ACTIONS(1),
    [anon_sym_EQ] = ACTIONS(1),
    [anon_sym_COMMA] = ACTIONS(1),
    [anon_sym_if] = ACTIONS(1),
    [anon_sym_COLON] = ACTIONS(1),
    [ts_builtin_sym_end] = ACTIONS(1),
    [anon_sym_PERCENT] = ACTIONS(1),
    [anon_sym_DASH] = ACTIONS(1),
    [sym_NUMBER] = ACTIONS(1),
    [anon_sym_COLON_EQ] = ACTIONS(1),
    [anon_sym_PLUS] = ACTIONS(1),
    [anon_sym_and] = ACTIONS(1),
    [anon_sym_LPAREN] = ACTIONS(1),
    [anon_sym_end] = ACTIONS(1),
    [anon_sym_SLASH] = ACTIONS(1),
  },
  [1] = {
    [sym_exprstmt] = STATE(6),
    [sym_source_file] = STATE(4),
    [sym_ifstmt] = STATE(6),
    [sym__expr] = STATE(5),
    [sym_binop] = STATE(5),
    [sym__element] = STATE(6),
    [sym_funcdef] = STATE(6),
    [aux_sym_source_file_repeat1] = STATE(6),
    [sym_ID] = ACTIONS(3),
    [ts_builtin_sym_end] = ACTIONS(5),
    [anon_sym_function] = ACTIONS(7),
    [sym_NUMBER] = ACTIONS(9),
    [anon_sym_if] = ACTIONS(11),
  },
  [2] = {
    [sym_ID] = ACTIONS(13),
  },
  [3] = {
    [sym__expr] = STATE(8),
    [sym_binop] = STATE(8),
    [sym_ID] = ACTIONS(15),
    [sym_NUMBER] = ACTIONS(15),
  },
  [4] = {
    [ts_builtin_sym_end] = ACTIONS(17),
  },
  [5] = {
    [sym_ID] = ACTIONS(19),
    [anon_sym_BANG_EQ] = ACTIONS(21),
    [anon_sym_function] = ACTIONS(19),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(25),
    [anon_sym_EQ] = ACTIONS(21),
    [anon_sym_if] = ACTIONS(19),
    [ts_builtin_sym_end] = ACTIONS(27),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(29),
    [sym_NUMBER] = ACTIONS(27),
    [anon_sym_COLON_EQ] = ACTIONS(31),
    [anon_sym_PLUS] = ACTIONS(29),
    [anon_sym_and] = ACTIONS(33),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [6] = {
    [sym_ifstmt] = STATE(15),
    [sym__expr] = STATE(5),
    [sym_exprstmt] = STATE(15),
    [sym_binop] = STATE(5),
    [sym_funcdef] = STATE(15),
    [sym__element] = STATE(15),
    [aux_sym_source_file_repeat1] = STATE(15),
    [sym_ID] = ACTIONS(3),
    [ts_builtin_sym_end] = ACTIONS(35),
    [anon_sym_function] = ACTIONS(7),
    [sym_NUMBER] = ACTIONS(9),
    [anon_sym_if] = ACTIONS(11),
  },
  [7] = {
    [anon_sym_LPAREN] = ACTIONS(37),
  },
  [8] = {
    [sym_thenblock] = STATE(19),
    [anon_sym_BANG_EQ] = ACTIONS(21),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(29),
    [anon_sym_COLON_EQ] = ACTIONS(39),
    [anon_sym_PLUS] = ACTIONS(29),
    [anon_sym_and] = ACTIONS(41),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(43),
    [anon_sym_then] = ACTIONS(45),
    [anon_sym_EQ] = ACTIONS(21),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [9] = {
    [sym__expr] = STATE(20),
    [sym_binop] = STATE(20),
    [sym_ID] = ACTIONS(47),
    [sym_NUMBER] = ACTIONS(47),
  },
  [10] = {
    [sym__expr] = STATE(21),
    [sym_binop] = STATE(21),
    [sym_ID] = ACTIONS(49),
    [sym_NUMBER] = ACTIONS(49),
  },
  [11] = {
    [sym__expr] = STATE(22),
    [sym_binop] = STATE(22),
    [sym_ID] = ACTIONS(51),
    [sym_NUMBER] = ACTIONS(51),
  },
  [12] = {
    [sym__expr] = STATE(23),
    [sym_binop] = STATE(23),
    [sym_ID] = ACTIONS(53),
    [sym_NUMBER] = ACTIONS(53),
  },
  [13] = {
    [sym__expr] = STATE(24),
    [sym_binop] = STATE(24),
    [sym_ID] = ACTIONS(55),
    [sym_NUMBER] = ACTIONS(55),
  },
  [14] = {
    [sym__expr] = STATE(25),
    [sym_binop] = STATE(25),
    [sym_ID] = ACTIONS(57),
    [sym_NUMBER] = ACTIONS(57),
  },
  [15] = {
    [sym_ifstmt] = STATE(15),
    [sym__expr] = STATE(5),
    [sym_exprstmt] = STATE(15),
    [sym_binop] = STATE(5),
    [sym_funcdef] = STATE(15),
    [sym__element] = STATE(15),
    [aux_sym_source_file_repeat1] = STATE(15),
    [sym_ID] = ACTIONS(59),
    [ts_builtin_sym_end] = ACTIONS(62),
    [anon_sym_function] = ACTIONS(64),
    [sym_NUMBER] = ACTIONS(67),
    [anon_sym_if] = ACTIONS(70),
  },
  [16] = {
    [sym_arg] = STATE(28),
    [sym_ID] = ACTIONS(73),
    [anon_sym_RPAREN] = ACTIONS(75),
  },
  [17] = {
    [sym__expr] = STATE(29),
    [sym_binop] = STATE(29),
    [sym_ID] = ACTIONS(77),
    [sym_NUMBER] = ACTIONS(77),
  },
  [18] = {
    [sym_ifstmt] = STATE(30),
    [sym__expr] = STATE(31),
    [sym_exprstmt] = STATE(30),
    [sym_binop] = STATE(31),
    [sym_funcdef] = STATE(30),
    [sym__element] = STATE(30),
    [aux_sym_source_file_repeat1] = STATE(30),
    [sym_ID] = ACTIONS(79),
    [anon_sym_else] = ACTIONS(81),
    [anon_sym_end] = ACTIONS(81),
    [anon_sym_function] = ACTIONS(7),
    [sym_NUMBER] = ACTIONS(83),
    [anon_sym_if] = ACTIONS(11),
  },
  [19] = {
    [sym_elseifblock] = STATE(34),
    [aux_sym_ifstmt_repeat1] = STATE(34),
    [sym_elseblock] = STATE(35),
    [anon_sym_else] = ACTIONS(85),
    [anon_sym_end] = ACTIONS(87),
  },
  [20] = {
    [sym_ID] = ACTIONS(89),
    [anon_sym_else] = ACTIONS(89),
    [anon_sym_BANG_EQ] = ACTIONS(91),
    [anon_sym_function] = ACTIONS(89),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(89),
    [anon_sym_then] = ACTIONS(89),
    [anon_sym_EQ] = ACTIONS(91),
    [anon_sym_if] = ACTIONS(89),
    [ts_builtin_sym_end] = ACTIONS(91),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(29),
    [sym_NUMBER] = ACTIONS(91),
    [anon_sym_COLON_EQ] = ACTIONS(91),
    [anon_sym_PLUS] = ACTIONS(29),
    [anon_sym_and] = ACTIONS(89),
    [anon_sym_end] = ACTIONS(89),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [21] = {
    [sym_ID] = ACTIONS(89),
    [anon_sym_else] = ACTIONS(89),
    [anon_sym_BANG_EQ] = ACTIONS(91),
    [anon_sym_function] = ACTIONS(89),
    [anon_sym_STAR] = ACTIONS(91),
    [anon_sym_or] = ACTIONS(89),
    [anon_sym_then] = ACTIONS(89),
    [anon_sym_EQ] = ACTIONS(91),
    [anon_sym_if] = ACTIONS(89),
    [ts_builtin_sym_end] = ACTIONS(91),
    [anon_sym_PERCENT] = ACTIONS(91),
    [anon_sym_DASH] = ACTIONS(91),
    [sym_NUMBER] = ACTIONS(91),
    [anon_sym_COLON_EQ] = ACTIONS(91),
    [anon_sym_PLUS] = ACTIONS(91),
    [anon_sym_and] = ACTIONS(89),
    [anon_sym_end] = ACTIONS(89),
    [anon_sym_SLASH] = ACTIONS(91),
  },
  [22] = {
    [sym_ID] = ACTIONS(89),
    [anon_sym_else] = ACTIONS(89),
    [anon_sym_BANG_EQ] = ACTIONS(91),
    [anon_sym_function] = ACTIONS(89),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(89),
    [anon_sym_then] = ACTIONS(89),
    [anon_sym_EQ] = ACTIONS(91),
    [anon_sym_if] = ACTIONS(89),
    [ts_builtin_sym_end] = ACTIONS(91),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(91),
    [sym_NUMBER] = ACTIONS(91),
    [anon_sym_COLON_EQ] = ACTIONS(91),
    [anon_sym_PLUS] = ACTIONS(91),
    [anon_sym_and] = ACTIONS(89),
    [anon_sym_end] = ACTIONS(89),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [23] = {
    [sym_ID] = ACTIONS(89),
    [anon_sym_BANG_EQ] = ACTIONS(21),
    [anon_sym_function] = ACTIONS(89),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(25),
    [anon_sym_EQ] = ACTIONS(21),
    [anon_sym_if] = ACTIONS(89),
    [ts_builtin_sym_end] = ACTIONS(91),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(29),
    [sym_NUMBER] = ACTIONS(91),
    [anon_sym_COLON_EQ] = ACTIONS(31),
    [anon_sym_PLUS] = ACTIONS(29),
    [anon_sym_and] = ACTIONS(33),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [24] = {
    [sym_ID] = ACTIONS(89),
    [anon_sym_else] = ACTIONS(89),
    [anon_sym_BANG_EQ] = ACTIONS(21),
    [anon_sym_function] = ACTIONS(89),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(89),
    [anon_sym_then] = ACTIONS(89),
    [anon_sym_EQ] = ACTIONS(21),
    [anon_sym_if] = ACTIONS(89),
    [ts_builtin_sym_end] = ACTIONS(91),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(29),
    [sym_NUMBER] = ACTIONS(91),
    [anon_sym_COLON_EQ] = ACTIONS(91),
    [anon_sym_PLUS] = ACTIONS(29),
    [anon_sym_and] = ACTIONS(89),
    [anon_sym_end] = ACTIONS(89),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [25] = {
    [sym_ID] = ACTIONS(89),
    [anon_sym_else] = ACTIONS(89),
    [anon_sym_BANG_EQ] = ACTIONS(21),
    [anon_sym_function] = ACTIONS(89),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(89),
    [anon_sym_then] = ACTIONS(89),
    [anon_sym_EQ] = ACTIONS(21),
    [anon_sym_if] = ACTIONS(89),
    [ts_builtin_sym_end] = ACTIONS(91),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(29),
    [sym_NUMBER] = ACTIONS(91),
    [anon_sym_COLON_EQ] = ACTIONS(91),
    [anon_sym_PLUS] = ACTIONS(29),
    [anon_sym_and] = ACTIONS(33),
    [anon_sym_end] = ACTIONS(89),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [26] = {
    [anon_sym_COLON] = ACTIONS(93),
  },
  [27] = {
    [anon_sym_COLON] = ACTIONS(95),
  },
  [28] = {
    [aux_sym_funcdef_repeat1] = STATE(40),
    [anon_sym_COMMA] = ACTIONS(97),
    [anon_sym_RPAREN] = ACTIONS(99),
  },
  [29] = {
    [anon_sym_BANG_EQ] = ACTIONS(21),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(29),
    [anon_sym_COLON_EQ] = ACTIONS(39),
    [anon_sym_PLUS] = ACTIONS(29),
    [anon_sym_and] = ACTIONS(41),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(43),
    [anon_sym_then] = ACTIONS(91),
    [anon_sym_EQ] = ACTIONS(21),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [30] = {
    [sym_ifstmt] = STATE(41),
    [sym__expr] = STATE(31),
    [sym_exprstmt] = STATE(41),
    [sym_binop] = STATE(31),
    [sym_funcdef] = STATE(41),
    [sym__element] = STATE(41),
    [aux_sym_source_file_repeat1] = STATE(41),
    [sym_ID] = ACTIONS(79),
    [anon_sym_else] = ACTIONS(101),
    [anon_sym_end] = ACTIONS(101),
    [anon_sym_function] = ACTIONS(7),
    [sym_NUMBER] = ACTIONS(83),
    [anon_sym_if] = ACTIONS(11),
  },
  [31] = {
    [sym_ID] = ACTIONS(19),
    [anon_sym_else] = ACTIONS(19),
    [anon_sym_BANG_EQ] = ACTIONS(21),
    [anon_sym_function] = ACTIONS(19),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(25),
    [anon_sym_EQ] = ACTIONS(21),
    [anon_sym_if] = ACTIONS(19),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(29),
    [sym_NUMBER] = ACTIONS(27),
    [anon_sym_COLON_EQ] = ACTIONS(103),
    [anon_sym_PLUS] = ACTIONS(29),
    [anon_sym_and] = ACTIONS(33),
    [anon_sym_end] = ACTIONS(19),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [32] = {
    [sym_ifstmt] = STATE(44),
    [sym__expr] = STATE(45),
    [sym_exprstmt] = STATE(44),
    [sym_binop] = STATE(45),
    [sym_funcdef] = STATE(44),
    [sym__element] = STATE(44),
    [aux_sym_source_file_repeat1] = STATE(44),
    [sym_ID] = ACTIONS(105),
    [anon_sym_end] = ACTIONS(107),
    [anon_sym_function] = ACTIONS(7),
    [sym_NUMBER] = ACTIONS(109),
    [anon_sym_if] = ACTIONS(111),
  },
  [33] = {
    [sym_ID] = ACTIONS(113),
    [anon_sym_else] = ACTIONS(113),
    [ts_builtin_sym_end] = ACTIONS(115),
    [anon_sym_end] = ACTIONS(113),
    [anon_sym_function] = ACTIONS(113),
    [sym_NUMBER] = ACTIONS(115),
    [anon_sym_if] = ACTIONS(113),
  },
  [34] = {
    [sym_elseifblock] = STATE(47),
    [aux_sym_ifstmt_repeat1] = STATE(47),
    [sym_elseblock] = STATE(48),
    [anon_sym_else] = ACTIONS(85),
    [anon_sym_end] = ACTIONS(117),
  },
  [35] = {
    [anon_sym_end] = ACTIONS(117),
  },
  [36] = {
    [sym_ID] = ACTIONS(119),
  },
  [37] = {
    [sym_ID] = ACTIONS(121),
  },
  [38] = {
    [sym_arg] = STATE(51),
    [sym_ID] = ACTIONS(73),
  },
  [39] = {
    [anon_sym_COLON] = ACTIONS(123),
  },
  [40] = {
    [aux_sym_funcdef_repeat1] = STATE(54),
    [anon_sym_COMMA] = ACTIONS(97),
    [anon_sym_RPAREN] = ACTIONS(125),
  },
  [41] = {
    [sym_ifstmt] = STATE(41),
    [sym__expr] = STATE(31),
    [sym_exprstmt] = STATE(41),
    [sym_binop] = STATE(31),
    [sym_funcdef] = STATE(41),
    [sym__element] = STATE(41),
    [aux_sym_source_file_repeat1] = STATE(41),
    [sym_ID] = ACTIONS(127),
    [anon_sym_else] = ACTIONS(130),
    [anon_sym_end] = ACTIONS(130),
    [anon_sym_function] = ACTIONS(64),
    [sym_NUMBER] = ACTIONS(132),
    [anon_sym_if] = ACTIONS(70),
  },
  [42] = {
    [sym__expr] = STATE(55),
    [sym_binop] = STATE(55),
    [sym_ID] = ACTIONS(135),
    [sym_NUMBER] = ACTIONS(135),
  },
  [43] = {
    [sym__expr] = STATE(56),
    [sym_binop] = STATE(56),
    [sym_ID] = ACTIONS(137),
    [sym_NUMBER] = ACTIONS(137),
  },
  [44] = {
    [sym_ifstmt] = STATE(57),
    [sym__expr] = STATE(45),
    [sym_exprstmt] = STATE(57),
    [sym_binop] = STATE(45),
    [sym_funcdef] = STATE(57),
    [sym__element] = STATE(57),
    [aux_sym_source_file_repeat1] = STATE(57),
    [sym_ID] = ACTIONS(105),
    [anon_sym_end] = ACTIONS(139),
    [anon_sym_function] = ACTIONS(7),
    [sym_NUMBER] = ACTIONS(109),
    [anon_sym_if] = ACTIONS(11),
  },
  [45] = {
    [sym_ID] = ACTIONS(19),
    [anon_sym_BANG_EQ] = ACTIONS(21),
    [anon_sym_function] = ACTIONS(19),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(25),
    [anon_sym_EQ] = ACTIONS(21),
    [anon_sym_if] = ACTIONS(19),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(29),
    [sym_NUMBER] = ACTIONS(27),
    [anon_sym_COLON_EQ] = ACTIONS(141),
    [anon_sym_PLUS] = ACTIONS(29),
    [anon_sym_and] = ACTIONS(33),
    [anon_sym_end] = ACTIONS(19),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [46] = {
    [sym_ID] = ACTIONS(143),
    [anon_sym_else] = ACTIONS(143),
    [ts_builtin_sym_end] = ACTIONS(145),
    [anon_sym_end] = ACTIONS(143),
    [anon_sym_function] = ACTIONS(143),
    [sym_NUMBER] = ACTIONS(145),
    [anon_sym_if] = ACTIONS(143),
  },
  [47] = {
    [sym_elseifblock] = STATE(47),
    [aux_sym_ifstmt_repeat1] = STATE(47),
    [anon_sym_else] = ACTIONS(147),
    [anon_sym_end] = ACTIONS(150),
  },
  [48] = {
    [anon_sym_end] = ACTIONS(152),
  },
  [49] = {
    [anon_sym_COMMA] = ACTIONS(154),
    [anon_sym_RPAREN] = ACTIONS(154),
  },
  [50] = {
    [sym_isblock] = STATE(62),
    [anon_sym_is] = ACTIONS(156),
  },
  [51] = {
    [anon_sym_COMMA] = ACTIONS(158),
    [anon_sym_RPAREN] = ACTIONS(158),
  },
  [52] = {
    [sym_ID] = ACTIONS(160),
  },
  [53] = {
    [anon_sym_COLON] = ACTIONS(162),
  },
  [54] = {
    [aux_sym_funcdef_repeat1] = STATE(54),
    [anon_sym_COMMA] = ACTIONS(164),
    [anon_sym_RPAREN] = ACTIONS(158),
  },
  [55] = {
    [sym_ID] = ACTIONS(89),
    [anon_sym_else] = ACTIONS(89),
    [anon_sym_BANG_EQ] = ACTIONS(21),
    [anon_sym_function] = ACTIONS(89),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(25),
    [anon_sym_EQ] = ACTIONS(21),
    [anon_sym_if] = ACTIONS(89),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(29),
    [sym_NUMBER] = ACTIONS(91),
    [anon_sym_COLON_EQ] = ACTIONS(103),
    [anon_sym_PLUS] = ACTIONS(29),
    [anon_sym_and] = ACTIONS(33),
    [anon_sym_end] = ACTIONS(89),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [56] = {
    [sym_thenblock] = STATE(19),
    [anon_sym_BANG_EQ] = ACTIONS(21),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(29),
    [anon_sym_COLON_EQ] = ACTIONS(39),
    [anon_sym_PLUS] = ACTIONS(29),
    [anon_sym_and] = ACTIONS(41),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(43),
    [anon_sym_then] = ACTIONS(167),
    [anon_sym_EQ] = ACTIONS(21),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [57] = {
    [sym_ifstmt] = STATE(57),
    [sym__expr] = STATE(45),
    [sym_exprstmt] = STATE(57),
    [sym_binop] = STATE(45),
    [sym_funcdef] = STATE(57),
    [sym__element] = STATE(57),
    [aux_sym_source_file_repeat1] = STATE(57),
    [sym_ID] = ACTIONS(169),
    [anon_sym_end] = ACTIONS(130),
    [anon_sym_function] = ACTIONS(64),
    [sym_NUMBER] = ACTIONS(172),
    [anon_sym_if] = ACTIONS(70),
  },
  [58] = {
    [sym__expr] = STATE(66),
    [sym_binop] = STATE(66),
    [sym_ID] = ACTIONS(175),
    [sym_NUMBER] = ACTIONS(175),
  },
  [59] = {
    [anon_sym_if] = ACTIONS(177),
  },
  [60] = {
    [sym_ID] = ACTIONS(179),
    [anon_sym_else] = ACTIONS(179),
    [ts_builtin_sym_end] = ACTIONS(181),
    [anon_sym_end] = ACTIONS(179),
    [anon_sym_function] = ACTIONS(179),
    [sym_NUMBER] = ACTIONS(181),
    [anon_sym_if] = ACTIONS(179),
  },
  [61] = {
    [sym_ifstmt] = STATE(69),
    [sym__expr] = STATE(45),
    [sym_exprstmt] = STATE(69),
    [sym_binop] = STATE(45),
    [sym_funcdef] = STATE(69),
    [sym__element] = STATE(69),
    [aux_sym_source_file_repeat1] = STATE(69),
    [sym_ID] = ACTIONS(105),
    [anon_sym_end] = ACTIONS(183),
    [anon_sym_function] = ACTIONS(7),
    [sym_NUMBER] = ACTIONS(109),
    [anon_sym_if] = ACTIONS(11),
  },
  [62] = {
    [sym_ID] = ACTIONS(185),
    [anon_sym_else] = ACTIONS(185),
    [ts_builtin_sym_end] = ACTIONS(187),
    [anon_sym_end] = ACTIONS(185),
    [anon_sym_function] = ACTIONS(185),
    [sym_NUMBER] = ACTIONS(187),
    [anon_sym_if] = ACTIONS(185),
  },
  [63] = {
    [sym_isblock] = STATE(70),
    [anon_sym_is] = ACTIONS(156),
  },
  [64] = {
    [sym_ID] = ACTIONS(189),
  },
  [65] = {
    [sym_ifstmt] = STATE(72),
    [sym__expr] = STATE(31),
    [sym_exprstmt] = STATE(72),
    [sym_binop] = STATE(31),
    [sym_funcdef] = STATE(72),
    [sym__element] = STATE(72),
    [aux_sym_source_file_repeat1] = STATE(72),
    [sym_ID] = ACTIONS(79),
    [anon_sym_else] = ACTIONS(191),
    [anon_sym_end] = ACTIONS(191),
    [anon_sym_function] = ACTIONS(7),
    [sym_NUMBER] = ACTIONS(83),
    [anon_sym_if] = ACTIONS(11),
  },
  [66] = {
    [sym_ID] = ACTIONS(89),
    [anon_sym_BANG_EQ] = ACTIONS(21),
    [anon_sym_function] = ACTIONS(89),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(25),
    [anon_sym_EQ] = ACTIONS(21),
    [anon_sym_if] = ACTIONS(89),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(29),
    [sym_NUMBER] = ACTIONS(91),
    [anon_sym_COLON_EQ] = ACTIONS(141),
    [anon_sym_PLUS] = ACTIONS(29),
    [anon_sym_and] = ACTIONS(33),
    [anon_sym_end] = ACTIONS(89),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [67] = {
    [sym__expr] = STATE(73),
    [sym_binop] = STATE(73),
    [sym_ID] = ACTIONS(193),
    [sym_NUMBER] = ACTIONS(193),
  },
  [68] = {
    [sym_ID] = ACTIONS(195),
    [anon_sym_else] = ACTIONS(195),
    [ts_builtin_sym_end] = ACTIONS(197),
    [anon_sym_end] = ACTIONS(195),
    [anon_sym_function] = ACTIONS(195),
    [sym_NUMBER] = ACTIONS(197),
    [anon_sym_if] = ACTIONS(195),
  },
  [69] = {
    [sym_ifstmt] = STATE(57),
    [sym__expr] = STATE(45),
    [sym_exprstmt] = STATE(57),
    [sym_binop] = STATE(45),
    [sym_funcdef] = STATE(57),
    [sym__element] = STATE(57),
    [aux_sym_source_file_repeat1] = STATE(57),
    [sym_ID] = ACTIONS(105),
    [anon_sym_end] = ACTIONS(199),
    [anon_sym_function] = ACTIONS(7),
    [sym_NUMBER] = ACTIONS(109),
    [anon_sym_if] = ACTIONS(11),
  },
  [70] = {
    [sym_ID] = ACTIONS(201),
    [anon_sym_else] = ACTIONS(201),
    [ts_builtin_sym_end] = ACTIONS(203),
    [anon_sym_end] = ACTIONS(201),
    [anon_sym_function] = ACTIONS(201),
    [sym_NUMBER] = ACTIONS(203),
    [anon_sym_if] = ACTIONS(201),
  },
  [71] = {
    [sym_isblock] = STATE(75),
    [anon_sym_is] = ACTIONS(156),
  },
  [72] = {
    [sym_ifstmt] = STATE(41),
    [sym__expr] = STATE(31),
    [sym_exprstmt] = STATE(41),
    [sym_binop] = STATE(31),
    [sym_funcdef] = STATE(41),
    [sym__element] = STATE(41),
    [aux_sym_source_file_repeat1] = STATE(41),
    [sym_ID] = ACTIONS(79),
    [anon_sym_else] = ACTIONS(205),
    [anon_sym_end] = ACTIONS(205),
    [anon_sym_function] = ACTIONS(7),
    [sym_NUMBER] = ACTIONS(83),
    [anon_sym_if] = ACTIONS(11),
  },
  [73] = {
    [anon_sym_BANG_EQ] = ACTIONS(21),
    [anon_sym_PERCENT] = ACTIONS(23),
    [anon_sym_DASH] = ACTIONS(29),
    [anon_sym_COLON_EQ] = ACTIONS(39),
    [anon_sym_PLUS] = ACTIONS(29),
    [anon_sym_and] = ACTIONS(41),
    [anon_sym_STAR] = ACTIONS(23),
    [anon_sym_or] = ACTIONS(43),
    [anon_sym_then] = ACTIONS(207),
    [anon_sym_EQ] = ACTIONS(21),
    [anon_sym_SLASH] = ACTIONS(23),
  },
  [74] = {
    [sym_ID] = ACTIONS(209),
    [anon_sym_else] = ACTIONS(209),
    [ts_builtin_sym_end] = ACTIONS(211),
    [anon_sym_end] = ACTIONS(209),
    [anon_sym_function] = ACTIONS(209),
    [sym_NUMBER] = ACTIONS(211),
    [anon_sym_if] = ACTIONS(209),
  },
  [75] = {
    [sym_ID] = ACTIONS(213),
    [anon_sym_else] = ACTIONS(213),
    [ts_builtin_sym_end] = ACTIONS(215),
    [anon_sym_end] = ACTIONS(213),
    [anon_sym_function] = ACTIONS(213),
    [sym_NUMBER] = ACTIONS(215),
    [anon_sym_if] = ACTIONS(213),
  },
  [76] = {
    [sym_ifstmt] = STATE(72),
    [sym__expr] = STATE(31),
    [sym_exprstmt] = STATE(72),
    [sym_binop] = STATE(31),
    [sym_funcdef] = STATE(72),
    [sym__element] = STATE(72),
    [aux_sym_source_file_repeat1] = STATE(72),
    [sym_ID] = ACTIONS(79),
    [anon_sym_else] = ACTIONS(191),
    [anon_sym_end] = ACTIONS(191),
    [anon_sym_function] = ACTIONS(7),
    [sym_NUMBER] = ACTIONS(83),
    [anon_sym_if] = ACTIONS(11),
  },
};

static TSParseActionEntry ts_parse_actions[] = {
  [0] = {.count = 0, .reusable = false},
  [1] = {.count = 1, .reusable = false}, RECOVER(),
  [3] = {.count = 1, .reusable = false}, SHIFT(5),
  [5] = {.count = 1, .reusable = true}, REDUCE(sym_source_file, 0),
  [7] = {.count = 1, .reusable = false}, SHIFT(2),
  [9] = {.count = 1, .reusable = true}, SHIFT(5),
  [11] = {.count = 1, .reusable = false}, SHIFT(3),
  [13] = {.count = 1, .reusable = true}, SHIFT(7),
  [15] = {.count = 1, .reusable = true}, SHIFT(8),
  [17] = {.count = 1, .reusable = true},  ACCEPT_INPUT(),
  [19] = {.count = 1, .reusable = false}, REDUCE(sym_exprstmt, 1),
  [21] = {.count = 1, .reusable = true}, SHIFT(9),
  [23] = {.count = 1, .reusable = true}, SHIFT(10),
  [25] = {.count = 1, .reusable = false}, SHIFT(14),
  [27] = {.count = 1, .reusable = true}, REDUCE(sym_exprstmt, 1),
  [29] = {.count = 1, .reusable = true}, SHIFT(11),
  [31] = {.count = 1, .reusable = true}, SHIFT(12),
  [33] = {.count = 1, .reusable = false}, SHIFT(13),
  [35] = {.count = 1, .reusable = true}, REDUCE(sym_source_file, 1),
  [37] = {.count = 1, .reusable = true}, SHIFT(16),
  [39] = {.count = 1, .reusable = true}, SHIFT(17),
  [41] = {.count = 1, .reusable = true}, SHIFT(13),
  [43] = {.count = 1, .reusable = true}, SHIFT(14),
  [45] = {.count = 1, .reusable = true}, SHIFT(18),
  [47] = {.count = 1, .reusable = true}, SHIFT(20),
  [49] = {.count = 1, .reusable = true}, SHIFT(21),
  [51] = {.count = 1, .reusable = true}, SHIFT(22),
  [53] = {.count = 1, .reusable = true}, SHIFT(23),
  [55] = {.count = 1, .reusable = true}, SHIFT(24),
  [57] = {.count = 1, .reusable = true}, SHIFT(25),
  [59] = {.count = 2, .reusable = false}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(5),
  [62] = {.count = 1, .reusable = true}, REDUCE(aux_sym_source_file_repeat1, 2),
  [64] = {.count = 2, .reusable = false}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(2),
  [67] = {.count = 2, .reusable = true}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(5),
  [70] = {.count = 2, .reusable = false}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(3),
  [73] = {.count = 1, .reusable = true}, SHIFT(26),
  [75] = {.count = 1, .reusable = true}, SHIFT(27),
  [77] = {.count = 1, .reusable = true}, SHIFT(29),
  [79] = {.count = 1, .reusable = false}, SHIFT(31),
  [81] = {.count = 1, .reusable = false}, REDUCE(sym_thenblock, 1),
  [83] = {.count = 1, .reusable = true}, SHIFT(31),
  [85] = {.count = 1, .reusable = true}, SHIFT(32),
  [87] = {.count = 1, .reusable = true}, SHIFT(33),
  [89] = {.count = 1, .reusable = false}, REDUCE(sym_binop, 3),
  [91] = {.count = 1, .reusable = true}, REDUCE(sym_binop, 3),
  [93] = {.count = 1, .reusable = true}, SHIFT(36),
  [95] = {.count = 1, .reusable = true}, SHIFT(37),
  [97] = {.count = 1, .reusable = true}, SHIFT(38),
  [99] = {.count = 1, .reusable = true}, SHIFT(39),
  [101] = {.count = 1, .reusable = false}, REDUCE(sym_thenblock, 2),
  [103] = {.count = 1, .reusable = true}, SHIFT(42),
  [105] = {.count = 1, .reusable = false}, SHIFT(45),
  [107] = {.count = 1, .reusable = false}, REDUCE(sym_elseblock, 1),
  [109] = {.count = 1, .reusable = true}, SHIFT(45),
  [111] = {.count = 1, .reusable = false}, SHIFT(43),
  [113] = {.count = 1, .reusable = false}, REDUCE(sym_ifstmt, 4),
  [115] = {.count = 1, .reusable = true}, REDUCE(sym_ifstmt, 4),
  [117] = {.count = 1, .reusable = true}, SHIFT(46),
  [119] = {.count = 1, .reusable = true}, SHIFT(49),
  [121] = {.count = 1, .reusable = true}, SHIFT(50),
  [123] = {.count = 1, .reusable = true}, SHIFT(52),
  [125] = {.count = 1, .reusable = true}, SHIFT(53),
  [127] = {.count = 2, .reusable = false}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(31),
  [130] = {.count = 1, .reusable = false}, REDUCE(aux_sym_source_file_repeat1, 2),
  [132] = {.count = 2, .reusable = true}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(31),
  [135] = {.count = 1, .reusable = true}, SHIFT(55),
  [137] = {.count = 1, .reusable = true}, SHIFT(56),
  [139] = {.count = 1, .reusable = false}, REDUCE(sym_elseblock, 2),
  [141] = {.count = 1, .reusable = true}, SHIFT(58),
  [143] = {.count = 1, .reusable = false}, REDUCE(sym_ifstmt, 5),
  [145] = {.count = 1, .reusable = true}, REDUCE(sym_ifstmt, 5),
  [147] = {.count = 2, .reusable = true}, REDUCE(aux_sym_ifstmt_repeat1, 2), SHIFT_REPEAT(59),
  [150] = {.count = 1, .reusable = true}, REDUCE(aux_sym_ifstmt_repeat1, 2),
  [152] = {.count = 1, .reusable = true}, SHIFT(60),
  [154] = {.count = 1, .reusable = true}, REDUCE(sym_arg, 3, .production_id = 1),
  [156] = {.count = 1, .reusable = true}, SHIFT(61),
  [158] = {.count = 1, .reusable = true}, REDUCE(aux_sym_funcdef_repeat1, 2),
  [160] = {.count = 1, .reusable = true}, SHIFT(63),
  [162] = {.count = 1, .reusable = true}, SHIFT(64),
  [164] = {.count = 2, .reusable = true}, REDUCE(aux_sym_funcdef_repeat1, 2), SHIFT_REPEAT(38),
  [167] = {.count = 1, .reusable = true}, SHIFT(65),
  [169] = {.count = 2, .reusable = false}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(45),
  [172] = {.count = 2, .reusable = true}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(45),
  [175] = {.count = 1, .reusable = true}, SHIFT(66),
  [177] = {.count = 1, .reusable = true}, SHIFT(67),
  [179] = {.count = 1, .reusable = false}, REDUCE(sym_ifstmt, 6),
  [181] = {.count = 1, .reusable = true}, REDUCE(sym_ifstmt, 6),
  [183] = {.count = 1, .reusable = false}, SHIFT(68),
  [185] = {.count = 1, .reusable = false}, REDUCE(sym_funcdef, 7, .production_id = 2),
  [187] = {.count = 1, .reusable = true}, REDUCE(sym_funcdef, 7, .production_id = 2),
  [189] = {.count = 1, .reusable = true}, SHIFT(71),
  [191] = {.count = 1, .reusable = false}, REDUCE(sym_elseifblock, 4),
  [193] = {.count = 1, .reusable = true}, SHIFT(73),
  [195] = {.count = 1, .reusable = false}, REDUCE(sym_isblock, 2),
  [197] = {.count = 1, .reusable = true}, REDUCE(sym_isblock, 2),
  [199] = {.count = 1, .reusable = false}, SHIFT(74),
  [201] = {.count = 1, .reusable = false}, REDUCE(sym_funcdef, 8, .production_id = 3),
  [203] = {.count = 1, .reusable = true}, REDUCE(sym_funcdef, 8, .production_id = 3),
  [205] = {.count = 1, .reusable = false}, REDUCE(sym_elseifblock, 5),
  [207] = {.count = 1, .reusable = true}, SHIFT(76),
  [209] = {.count = 1, .reusable = false}, REDUCE(sym_isblock, 3),
  [211] = {.count = 1, .reusable = true}, REDUCE(sym_isblock, 3),
  [213] = {.count = 1, .reusable = false}, REDUCE(sym_funcdef, 9, .production_id = 4),
  [215] = {.count = 1, .reusable = true}, REDUCE(sym_funcdef, 9, .production_id = 4),
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
