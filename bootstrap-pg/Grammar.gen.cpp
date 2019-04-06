#include "Grammar.gen.h"
#include "Grammar.visitors.h"

static inline bool special_1(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    return true;
}

static inline bool text_2(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "grammar");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool ref_3(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.ID(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "ID", result);
        return true;
    }
    return false;
}

static inline bool text_4(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, ";");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool ref_5(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.rule(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "rule", result);
        return true;
    }
    return false;
}

static inline bool rep_6(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    while (true)
    {
        RuleMatch *subsubmatch = nullptr;
        if (ref_5(g, result, subsubmatch, c))
        {
            submatch = mergeMatch(submatch, subsubmatch);
        }
        else
        {
            delete subsubmatch;
            return true;
        }
    }
}

static inline bool special_7(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    return true;
}

static inline bool seq_8(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    RuleMatch *subsubmatch = nullptr;
    Cursor start = c;

    if (!special_1(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!text_2(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!ref_3(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!text_4(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!rep_6(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!special_7(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    submatch = mergeMatch(submatch, subsubmatch);
    return true;
}

Match const *Grammar::top(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    if (seq_8(*this, inner, m, c))
    {
        if (!m) m = addToMatch(m, nullptr, inner);
        m->rule_index = rule_top;
        return m;
    }
    return nullptr;
}

static inline bool text_9(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "extern");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool named_10(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    if (text_9(g, result, submatch, c))
    {
        submatch = addToMatch(submatch, "extern", result);
        return true;
    }
    return false;
}

static inline bool ref_11(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.ID(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "ID", result);
        return true;
    }
    return false;
}

static inline bool text_12(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, ";");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool seq_13(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    RuleMatch *subsubmatch = nullptr;
    Cursor start = c;

    if (!named_10(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!ref_11(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!text_12(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    submatch = mergeMatch(submatch, subsubmatch);
    return true;
}

static inline bool ref_14(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.ID(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "ID", result);
        return true;
    }
    return false;
}

static inline bool text_15(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "::=");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool ref_16(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.alt(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "alt", result);
        return true;
    }
    return false;
}

static inline bool text_17(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, ";");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool seq_18(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    RuleMatch *subsubmatch = nullptr;
    Cursor start = c;

    if (!ref_14(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!text_15(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!ref_16(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!text_17(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    submatch = mergeMatch(submatch, subsubmatch);
    return true;
}

static inline bool alt_19(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    if (seq_13(g, result, submatch, c))
    {
        return true;
    }

    if (seq_18(g, result, submatch, c))
    {
        return true;
    }

    return false;
}

Match const *Grammar::rule(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    if (alt_19(*this, inner, m, c))
    {
        if (!m) m = addToMatch(m, nullptr, inner);
        m->rule_index = rule_rule;
        return m;
    }
    return nullptr;
}

static inline bool text_20(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "|");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool rep_21(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    RuleMatch *subsubmatch = nullptr;
    if (text_20(g, result, subsubmatch, c))
    {
        submatch = mergeMatch(submatch, subsubmatch);
    }
    return true;
}

static inline bool ref_22(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.seq(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "seq", result);
        return true;
    }
    return false;
}

static inline bool text_23(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "|");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool ref_24(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.seq(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "seq", result);
        return true;
    }
    return false;
}

static inline bool seq_25(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    RuleMatch *subsubmatch = nullptr;
    Cursor start = c;

    if (!text_23(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!ref_24(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    submatch = mergeMatch(submatch, subsubmatch);
    return true;
}

static inline bool rep_26(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    while (true)
    {
        RuleMatch *subsubmatch = nullptr;
        if (seq_25(g, result, subsubmatch, c))
        {
            submatch = mergeMatch(submatch, subsubmatch);
        }
        else
        {
            delete subsubmatch;
            return true;
        }
    }
}

static inline bool seq_27(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    RuleMatch *subsubmatch = nullptr;
    Cursor start = c;

    if (!rep_21(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!ref_22(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!rep_26(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    submatch = mergeMatch(submatch, subsubmatch);
    return true;
}

Match const *Grammar::alt(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    if (seq_27(*this, inner, m, c))
    {
        if (!m) m = addToMatch(m, nullptr, inner);
        m->rule_index = rule_alt;
        return m;
    }
    return nullptr;
}

static inline bool ref_28(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.rep(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "rep", result);
        return true;
    }
    return false;
}

static inline bool rep_29(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    while (true)
    {
        RuleMatch *subsubmatch = nullptr;
        if (ref_28(g, result, subsubmatch, c))
        {
            submatch = mergeMatch(submatch, subsubmatch);
        }
        else
        {
            delete subsubmatch;
            return true;
        }
    }
}

Match const *Grammar::seq(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    if (rep_29(*this, inner, m, c))
    {
        if (!m) m = addToMatch(m, nullptr, inner);
        m->rule_index = rule_seq;
        return m;
    }
    return nullptr;
}

static inline bool ref_30(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.named(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "named", result);
        return true;
    }
    return false;
}

static inline bool text_31(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "*");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool text_32(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "+");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool text_33(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "?");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool alt_34(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    if (text_31(g, result, submatch, c))
    {
        return true;
    }

    if (text_32(g, result, submatch, c))
    {
        return true;
    }

    if (text_33(g, result, submatch, c))
    {
        return true;
    }

    return false;
}

static inline bool named_35(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    if (alt_34(g, result, submatch, c))
    {
        submatch = addToMatch(submatch, "op", result);
        return true;
    }
    return false;
}

static inline bool rep_36(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    RuleMatch *subsubmatch = nullptr;
    if (named_35(g, result, subsubmatch, c))
    {
        submatch = mergeMatch(submatch, subsubmatch);
    }
    return true;
}

static inline bool seq_37(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    RuleMatch *subsubmatch = nullptr;
    Cursor start = c;

    if (!ref_30(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!rep_36(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    submatch = mergeMatch(submatch, subsubmatch);
    return true;
}

Match const *Grammar::rep(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    if (seq_37(*this, inner, m, c))
    {
        if (!m) m = addToMatch(m, nullptr, inner);
        m->rule_index = rule_rep;
        return m;
    }
    return nullptr;
}

static inline bool ref_38(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.ID(c);
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool named_39(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    if (ref_38(g, result, submatch, c))
    {
        submatch = addToMatch(submatch, "name", result);
        return true;
    }
    return false;
}

static inline bool text_40(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "=");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool seq_41(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    RuleMatch *subsubmatch = nullptr;
    Cursor start = c;

    if (!named_39(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!text_40(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    submatch = mergeMatch(submatch, subsubmatch);
    return true;
}

static inline bool rep_42(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    RuleMatch *subsubmatch = nullptr;
    if (seq_41(g, result, subsubmatch, c))
    {
        submatch = mergeMatch(submatch, subsubmatch);
    }
    return true;
}

static inline bool ref_43(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.element(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "element", result);
        return true;
    }
    return false;
}

static inline bool seq_44(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    RuleMatch *subsubmatch = nullptr;
    Cursor start = c;

    if (!rep_42(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!ref_43(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    submatch = mergeMatch(submatch, subsubmatch);
    return true;
}

Match const *Grammar::named(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    if (seq_44(*this, inner, m, c))
    {
        if (!m) m = addToMatch(m, nullptr, inner);
        m->rule_index = rule_named;
        return m;
    }
    return nullptr;
}

static inline bool ref_45(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.special(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "special", result);
        return true;
    }
    return false;
}

static inline bool ref_46(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.ref(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "ref", result);
        return true;
    }
    return false;
}

static inline bool ref_47(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.capture(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "capture", result);
        return true;
    }
    return false;
}

static inline bool ref_48(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.noncapture(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "noncapture", result);
        return true;
    }
    return false;
}

static inline bool ref_49(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.text(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "text", result);
        return true;
    }
    return false;
}

static inline bool alt_50(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    if (ref_45(g, result, submatch, c))
    {
        return true;
    }

    if (ref_46(g, result, submatch, c))
    {
        return true;
    }

    if (ref_47(g, result, submatch, c))
    {
        return true;
    }

    if (ref_48(g, result, submatch, c))
    {
        return true;
    }

    if (ref_49(g, result, submatch, c))
    {
        return true;
    }

    return false;
}

Match const *Grammar::element(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    if (alt_50(*this, inner, m, c))
    {
        if (!m) m = addToMatch(m, nullptr, inner);
        m->rule_index = rule_element;
        return m;
    }
    return nullptr;
}

static inline bool text_51(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "^");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool text_52(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "$");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool alt_53(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    if (text_51(g, result, submatch, c))
    {
        return true;
    }

    if (text_52(g, result, submatch, c))
    {
        return true;
    }

    return false;
}

Match const *Grammar::special(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    if (alt_53(*this, inner, m, c))
    {
        if (!m) m = addToMatch(m, nullptr, inner);
        m->rule_index = rule_special;
        return m;
    }
    return nullptr;
}

static inline bool ref_54(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.ID(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "ID", result);
        return true;
    }
    return false;
}

Match const *Grammar::ref(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    if (ref_54(*this, inner, m, c))
    {
        if (!m) m = addToMatch(m, nullptr, inner);
        m->rule_index = rule_ref;
        return m;
    }
    return nullptr;
}

static inline bool text_55(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "(");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool ref_56(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.alt(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "alt", result);
        return true;
    }
    return false;
}

static inline bool text_57(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, ")");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool seq_58(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    RuleMatch *subsubmatch = nullptr;
    Cursor start = c;

    if (!text_55(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!ref_56(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!text_57(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    submatch = mergeMatch(submatch, subsubmatch);
    return true;
}

Match const *Grammar::capture(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    if (seq_58(*this, inner, m, c))
    {
        if (!m) m = addToMatch(m, nullptr, inner);
        m->rule_index = rule_capture;
        return m;
    }
    return nullptr;
}

static inline bool text_59(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "[");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool ref_60(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.alt(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "alt", result);
        return true;
    }
    return false;
}

static inline bool text_61(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.TEXT(c, "]");
    if (result)
    {
        c.skipSpace();
        return true;
    }
    return false;
}

static inline bool seq_62(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    RuleMatch *subsubmatch = nullptr;
    Cursor start = c;

    if (!text_59(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!ref_60(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    if (!text_61(g, result, subsubmatch, c))
    {
        c = start;
        delete subsubmatch;
        return false;
    }

    submatch = mergeMatch(submatch, subsubmatch);
    return true;
}

Match const *Grammar::noncapture(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    if (seq_62(*this, inner, m, c))
    {
        if (!m) m = addToMatch(m, nullptr, inner);
        m->rule_index = rule_noncapture;
        return m;
    }
    return nullptr;
}

static inline bool ref_63(Grammar &g, Match const *&result, RuleMatch *&submatch, Cursor &c)
{
    result = g.STRING(c);
    if (result)
    {
        c.skipSpace();
        submatch = addToMatch(submatch, "STRING", result);
        return true;
    }
    return false;
}

Match const *Grammar::text(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    if (ref_63(*this, inner, m, c))
    {
        if (!m) m = addToMatch(m, nullptr, inner);
        m->rule_index = rule_text;
        return m;
    }
    return nullptr;
}

