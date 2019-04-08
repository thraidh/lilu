#include "Grammar.ast.h"
#include "Grammar.visitors.h"
#include "common.h"
#include <map>
#include <set>

map<string, AstNodeInfo *> astNodes;
int currentNodeTypeId = 0;

class GrammarAnalyzer : public GrammarVisitor<AstNode *> {
  public:
    AstNodeInfo *ani;
    string *namectx;
    AstNode *visit_noncapture(RuleMatch const *m, void *ctx) override {
        return visit(m->child(), ctx);
    }
    AstNode *visit_capture(RuleMatch const *m, void *ctx) override {
        return visit(m->child(), ctx);
    }
    AstNode *visit_text(RuleMatch const *m, void *ctx) override {
        auto ret = new text_Node();
        ret->_children.push_back(visit(m->child()));
        return ret;
    }
    AstNode *visit_special(RuleMatch const *m, void *ctx) override {
        return nullptr;
    }
    AstNode *visit_element(RuleMatch const *m, void *ctx) override {
        return visit(m->child(), ctx);
    }
    AstNode *visit_top(RuleMatch const *m, void *ctx) override {
        auto ret = new top_Node();
        for (auto c : m->range("rule")) {
            auto a = visit(c);
            ret->_children.push_back(a);
        }
        return ret;
    }
    AstNode *visit_rule(RuleMatch const *m, void *ctx) override {
        auto ID = visit(m->get("ID"));
        string name(ID->_text);
        AstNodeInfo *cur = astNodes[name];
        if (!cur) {
            cur = new AstNodeInfo(ID->_text);
            astNodes[name] = cur;
        }
        auto ext = m->get("extern");
        auto ret = new rule_Node();
        ret->_children.push_back(ID);
        if (ext) {
        } else {
            ani = cur;
            namectx = nullptr;
            auto child = m->get("alt");
            if (child) {
                auto c = visit(child);
                ret->_children.push_back(c);
                cur->rule = c;
            }
        }
        return ret;
    }
    AstNode *visit_rep(RuleMatch const *m, void *ctx) override {
        auto op = m->get("op");
        if (!op)
            return visit(m->child(), ctx);
        auto ret = new rep_Node();
        auto c = visit(m->child(), ctx);
        ret->_children.push_back(c);
        return ret;
    }
    AstNode *visit_alt(RuleMatch const *m, void *ctx) override {
        auto &v = m->positional;
        if (v.size() == 1)
            return visit(v[0], ctx);
        auto ret = new alt_Node();
        for (auto c : v) {
            ret->_children.push_back(visit(c, ctx));
        }
        return ret;
    }
    AstNode *visit_seq(RuleMatch const *m, void *ctx) override {
        auto &v = m->positional;
        if (v.size() == 1)
            return visit(v[0], ctx);
        auto ret = new seq_Node();
        for (auto c : v) {
            ret->_children.push_back(visit(c, ctx));
        }
        return ret;
    }
    AstNode *visit_ref(RuleMatch const *m, void *ctx) override {
        auto ret = new ref_Node();
        auto ID = visit(m->get("ID"));
        ret->_children.push_back(ID);
        if (!namectx) {
            auto &nameInfo = ani->names[string(ID->_text)];
            nameInfo.isMulti = true;
            nameInfo.type = "AstNode*";
        }
        // if !namectx add ref name as implicit
        return ret;
    }
    AstNode *visit_named(RuleMatch const *m, void *ctx) override {
        auto name = m->get("name");
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
    AstNode *visitTextMatch(TextMatch const *m, void *ctx) override {
        auto ret = new ID_Node();
        ret->_text = m->sv();
        cout << "TextMatch: " << m->sv() << endl;
        return ret;
    }
    AstNode *visitExprMatch(RuleMatch const *m, void *ctx) override {
        return default_value<AstNode *>();
    }
    AstNode *visitOperatorMatch(Operator const *m, void *ctx) override {
        return default_value<AstNode *>();
    }
};

auto addAstNodeInfo(char const *name, char const *parent,
                    char const *type = nullptr) {
    auto ret = new AstNodeInfo(name, parent, type);
    astNodes[name] = ret;
    return ret;
}

void printANI(AstNodeInfo const *ani) {
    cout << "ANI: " << ani->name << endl;
    for (auto n : ani->names) {
        cout << "  " << n.first << ": ";
        if (n.second.isMulti)
            cout << "vector<" << n.second.type << ">";
        else
            cout << n.second.type;
        cout << endl;
    }
}

void addAniAttr(AstNodeInfo *ani, char const *n, char const *t,
                bool multi = false) {
    auto &ni = ani->names[n];
    ni.type = t;
    ni.isMulti = multi;
}
class AstNodeToString : public GrammarAstVisitor<string> {
  public:
    string default_result(AstNode *n) override {
        return "<unknown " + to_string(n->_nodeTypeId) + ">";
    }
    string visit_AstNode(AstNode *n) override { return "AstNode"; }
    string visit_TerminalNode(TerminalNode *n) override {
        return "TerminalNode";
    }
    string visit_ID_Node(ID_Node *n) override {
        string ret = "ID_Node ";
        ret += n->_text;
        return ret;
    }
    string visit_InnerNode(InnerNode *n) override { return "InnerNode"; }
    string visit_KeywordNode(KeywordNode *n) override { return "KeywordNode"; }
    string visit_NUMBER_Node(NUMBER_Node *n) override { return "NUMBER_Node"; }
    string visit_STRING_Node(STRING_Node *n) override { return "STRING_Node"; }
    string visit_alt_Node(alt_Node *n) override { return "alt_Node"; }
    string visit_capture_Node(capture_Node *n) override {
        return "capture_Node";
    }
    string visit_element_Node(element_Node *n) override {
        return "element_Node";
    }
    string visit_named_Node(named_Node *n) override { return "named_Node"; }
    string visit_noncapture_Node(noncapture_Node *n) override {
        return "noncapture_Node";
    }
    string visit_ref_Node(ref_Node *n) override { return "ref_Node"; }
    string visit_rep_Node(rep_Node *n) override { return "rep_Node"; }
    string visit_rule_Node(rule_Node *n) override { return "rule_Node"; }
    string visit_seq_Node(seq_Node *n) override { return "seq_Node"; }
    string visit_special_Node(special_Node *n) override {
        return "special_Node";
    }
    string visit_text_Node(text_Node *n) override { return "text_Node"; }
    string visit_top_Node(top_Node *n) override { return "top_Node"; }
};

string astNodeToString(AstNode *n) {
    AstNodeToString v;
    return v.visit(n);
}

class PrintAst : public GrammarAstVisitor<void, int> {
  public:
    void default_result(AstNode *n, int indent) override {
        cout << string(indent * 2, ' ') << astNodeToString(n) << endl;
    }
    void visit_InnerNode(InnerNode *n, int indent) override {
        default_result(n, indent);
        for (auto c : n->_children)
            visit(c, indent + 1);
    }
};

void analyzeGrammar(Match const *res) {
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
    auto ast = v.visit(res);

    for (auto ani : astNodes) {
        printANI(ani.second);
    }

    PrintAst printAst;
    printAst.visit(ast, 0);
}
