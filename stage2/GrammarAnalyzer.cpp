#include "Grammar.ast.h"
#include "Grammar.visitors.h"
#include "common.h"
#include <map>
#include <set>

map<string, AstNodeInfo *> astNodes;
int currentNodeTypeId = 0;

class GrammarAnalyzer : public GrammarVisitor<AstNode *>
{
  public:
    AstNodeInfo *ani;
    string *namectx;
    AstNode *visit_noncapture(RuleMatch const *m, void *ctx) override
    {
        return visit(m->child(), ctx);
    }
    AstNode *visit_capture(RuleMatch const *m, void *ctx) override
    {
        return visit(m->child(), ctx);
    }
    AstNode *visit_text(RuleMatch const *m, void *ctx) override
    {
        return new text_Node();
    }
    AstNode *visit_special(RuleMatch const *m, void *ctx) override
    {
        return nullptr;
    }
    AstNode *visit_element(RuleMatch const *m, void *ctx) override
    {
        return visit(m->child(), ctx);
    }
    AstNode *visit_top(RuleMatch const *m, void *ctx) override
    {
        for (auto c : m->range("rule"))
        {
            visit(c);
        }
        return new top_Node();
    }
    AstNode *visit_rule(RuleMatch const *m, void *ctx) override
    {
        string name(m->get("ID")->sv());
        AstNodeInfo *cur = astNodes[name];
        if (!cur)
        {
            cur = new AstNodeInfo(m->get("ID")->sv());
            astNodes[name] = cur;
        }
        auto ext = m->get("extern");
        if (ext)
        {
        }
        else
        {
            cur->rule = new rule_Node();
            ani = cur;
            namectx = nullptr;
            auto child = m->get("alt");
            if (child)
                visit(child);
        }
        return cur->rule;
    }
    AstNode *visit_rep(RuleMatch const *m, void *ctx) override
    {
        auto op = m->get("op");
        if (!op)
            return visit(m->child(), ctx);
        visit(m->child(), ctx);
        return new rep_Node();
    }
    AstNode *visit_alt(RuleMatch const *m, void *ctx) override
    {
        auto &v = m->positional;
        if (v.size() == 1)
            return visit(v[0], ctx);
        for (auto c : v)
        {
            visit(c, ctx);
        }
        return new alt_Node();
    }
    AstNode *visit_seq(RuleMatch const *m, void *ctx) override
    {
        auto &v = m->positional;
        if (v.size() == 1)
            return visit(v[0], ctx);
        for (auto c : v)
        {
            visit(c, ctx);
        }
        return new seq_Node();
    }
    AstNode *visit_ref(RuleMatch const *m, void *ctx) override
    {
        if (!namectx)
        {
            auto &nameInfo = ani->names[string(m->get("ID")->sv())];
            nameInfo.isMulti = true;
            nameInfo.type = "AstNode*";
        }
        // if !namectx add ref name as implicit
        return new ref_Node();
    }
    AstNode *visit_named(RuleMatch const *m, void *ctx) override
    {
        auto name = m->get("op");
        if (!name)
            return visit(m->child(), ctx);

        string *oldname = namectx;
        namectx = new string(name->sv());

        auto &nameInfo = ani->names[*namectx];
        nameInfo.isMulti = true;
        nameInfo.type = "AstNode*";

        auto ret = visit(m->get("element"), ctx);

        delete namectx;
        namectx = oldname;

        return ret;
    }
    AstNode *visitTextMatch(TextMatch const *m, void *ctx) override
    {
        auto ret = new ID_Node();
        ret->_text = m->sv();
        cout << "TextMatch: " << m->sv() << endl;
        return ret;
    }
    AstNode *visitExprMatch(RuleMatch const *m, void *ctx) override { return default_value<AstNode *>(); }
    AstNode *visitOperatorMatch(Operator const *m, void *ctx) override { return default_value<AstNode *>(); }
};

auto addAstNodeInfo(char const *name, char const *parent, char const *type = nullptr)
{
    auto ret = new AstNodeInfo(name, parent, type);
    astNodes[name] = ret;
    return ret;
}

void printANI(AstNodeInfo const *ani)
{
    cout << "ANI: " << ani->name << endl;
    for (auto n : ani->names)
    {
        cout << "  " << n.first << ": ";
        if (n.second.isMulti)
            cout << "vector<" << n.second.type << ">";
        else
            cout << n.second.type;
        cout << endl;
    }
}

void addAniAttr(AstNodeInfo *ani, char const *n, char const *t, bool multi = false)
{
    auto &ni = ani->names[n];
    ni.type = t;
    ni.isMulti = multi;
}

void analyzeGrammar(Match const *res)
{
    auto ani = addAstNodeInfo("AstNode", "", "AstNode");
    addAniAttr(ani, "_nodeTypeId", "int");
    addAniAttr(ani, "_text", "string_view");
    addAstNodeInfo("TerminalNode", "AstNode", "TerminalNode");
    addAstNodeInfo("ID", "TerminalNode");
    ani = addAstNodeInfo("NUMBER", "TerminalNode");
    addAniAttr(ani, "_num", "int");
    ani = addAstNodeInfo("STRING", "TerminalNode");
    addAniAttr(ani, "_str", "string");
    addAstNodeInfo("KeywordNode", "TerminalNode", "KeywordNode");
    ani = addAstNodeInfo("InnerNode", "AstNode", "InnerNode");
    addAniAttr(ani, "_children", "AstNode*", true);

    GrammarAnalyzer v;
    v.visit(res);

    for (auto ani : astNodes)
    {
        printANI(ani.second);
    }
}
