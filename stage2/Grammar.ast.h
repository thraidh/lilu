#pragma once
#include "parser_base.h"

enum
{
    __no_node__ = 0,
    AstNodeTypeId = 1,
    TerminalNodeTypeId = 2,
    ID_NodeTypeId = 3,
    InnerNodeTypeId = 7,
    KeywordNodeTypeId = 6,
    NUMBER_NodeTypeId = 4,
    STRING_NodeTypeId = 5,
    alt_NodeTypeId = 10,
    capture_NodeTypeId = 17,
    element_NodeTypeId = 14,
    named_NodeTypeId = 13,
    noncapture_NodeTypeId = 18,
    ref_NodeTypeId = 16,
    rep_NodeTypeId = 12,
    rule_NodeTypeId = 9,
    seq_NodeTypeId = 11,
    special_NodeTypeId = 15,
    text_NodeTypeId = 19,
    top_NodeTypeId = 8
};

struct AstNode
{
    AstNode() { _nodeTypeId = AstNodeTypeId; }
    int _nodeTypeId;
    string_view _text;
};

struct TerminalNode : public AstNode
{
    TerminalNode() { _nodeTypeId = TerminalNodeTypeId; }
};

struct ID_Node : public TerminalNode
{
    ID_Node() { _nodeTypeId = ID_NodeTypeId; }
};

struct InnerNode : public AstNode
{
    InnerNode() { _nodeTypeId = InnerNodeTypeId; }
    vector<AstNode *> _children;
};

struct KeywordNode : public TerminalNode
{
    KeywordNode() { _nodeTypeId = KeywordNodeTypeId; }
};

struct NUMBER_Node : public TerminalNode
{
    NUMBER_Node() { _nodeTypeId = NUMBER_NodeTypeId; }
    int _num;
};

struct STRING_Node : public TerminalNode
{
    STRING_Node() { _nodeTypeId = STRING_NodeTypeId; }
    string _str;
};

struct alt_Node : public InnerNode
{
    alt_Node() { _nodeTypeId = alt_NodeTypeId; }
    vector<AstNode *> seq;
};

struct capture_Node : public InnerNode
{
    capture_Node() { _nodeTypeId = capture_NodeTypeId; }
    vector<AstNode *> alt;
};

struct element_Node : public InnerNode
{
    element_Node() { _nodeTypeId = element_NodeTypeId; }
    vector<AstNode *> capture;
    vector<AstNode *> noncapture;
    vector<AstNode *> ref;
    vector<AstNode *> special;
    vector<AstNode *> text;
};

struct named_Node : public InnerNode
{
    named_Node() { _nodeTypeId = named_NodeTypeId; }
    vector<AstNode *> element;
};

struct noncapture_Node : public InnerNode
{
    noncapture_Node() { _nodeTypeId = noncapture_NodeTypeId; }
    vector<AstNode *> alt;
};

struct ref_Node : public InnerNode
{
    ref_Node() { _nodeTypeId = ref_NodeTypeId; }
    vector<AstNode *> ID;
};

struct rep_Node : public InnerNode
{
    rep_Node() { _nodeTypeId = rep_NodeTypeId; }
    vector<AstNode *> named;
};

struct rule_Node : public InnerNode
{
    rule_Node() { _nodeTypeId = rule_NodeTypeId; }
    vector<AstNode *> ID;
    vector<AstNode *> alt;
};

struct seq_Node : public InnerNode
{
    seq_Node() { _nodeTypeId = seq_NodeTypeId; }
    vector<AstNode *> rep;
};

struct special_Node : public InnerNode
{
    special_Node() { _nodeTypeId = special_NodeTypeId; }
};

struct text_Node : public InnerNode
{
    text_Node() { _nodeTypeId = text_NodeTypeId; }
    vector<AstNode *> STRING;
};

struct top_Node : public InnerNode
{
    top_Node() { _nodeTypeId = top_NodeTypeId; }
    vector<AstNode *> ID;
    vector<AstNode *> rule;
};

template <typename RESULT, typename... ARGS>
class AbstractGrammarAstVisitor
{
  public:
    virtual ~AbstractGrammarAstVisitor() {}
    RESULT visit(AstNode *n, ARGS... args)
    {
        if (!n)
            return default_value<RESULT>();
        switch (n->_nodeTypeId)
        {
        case AstNodeTypeId:
            return visit_AstNode((AstNode *)n, args...);
        case TerminalNodeTypeId:
            return visit_TerminalNode((TerminalNode *)n, args...);
        case ID_NodeTypeId:
            return visit_ID_Node((ID_Node *)n, args...);
        case InnerNodeTypeId:
            return visit_InnerNode((InnerNode *)n, args...);
        case KeywordNodeTypeId:
            return visit_KeywordNode((KeywordNode *)n, args...);
        case NUMBER_NodeTypeId:
            return visit_NUMBER_Node((NUMBER_Node *)n, args...);
        case STRING_NodeTypeId:
            return visit_STRING_Node((STRING_Node *)n, args...);
        case alt_NodeTypeId:
            return visit_alt_Node((alt_Node *)n, args...);
        case capture_NodeTypeId:
            return visit_capture_Node((capture_Node *)n, args...);
        case element_NodeTypeId:
            return visit_element_Node((element_Node *)n, args...);
        case named_NodeTypeId:
            return visit_named_Node((named_Node *)n, args...);
        case noncapture_NodeTypeId:
            return visit_noncapture_Node((noncapture_Node *)n, args...);
        case ref_NodeTypeId:
            return visit_ref_Node((ref_Node *)n, args...);
        case rep_NodeTypeId:
            return visit_rep_Node((rep_Node *)n, args...);
        case rule_NodeTypeId:
            return visit_rule_Node((rule_Node *)n, args...);
        case seq_NodeTypeId:
            return visit_seq_Node((seq_Node *)n, args...);
        case special_NodeTypeId:
            return visit_special_Node((special_Node *)n, args...);
        case text_NodeTypeId:
            return visit_text_Node((text_Node *)n, args...);
        case top_NodeTypeId:
            return visit_top_Node((top_Node *)n, args...);
        }
        return default_value<RESULT>();
    }
    void visitChildren(InnerNode *n, ARGS... args)
    {
        for (auto c : n->_children)
        {
            visit(c, args...);
        }
    }
    virtual RESULT visit_AstNode(AstNode *n, ARGS... args) = 0;
    virtual RESULT visit_TerminalNode(TerminalNode *n, ARGS... args) = 0;
    virtual RESULT visit_ID_Node(ID_Node *n, ARGS... args) = 0;
    virtual RESULT visit_InnerNode(InnerNode *n, ARGS... args) = 0;
    virtual RESULT visit_KeywordNode(KeywordNode *n, ARGS... args) = 0;
    virtual RESULT visit_NUMBER_Node(NUMBER_Node *n, ARGS... args) = 0;
    virtual RESULT visit_STRING_Node(STRING_Node *n, ARGS... args) = 0;
    virtual RESULT visit_alt_Node(alt_Node *n, ARGS... args) = 0;
    virtual RESULT visit_capture_Node(capture_Node *n, ARGS... args) = 0;
    virtual RESULT visit_element_Node(element_Node *n, ARGS... args) = 0;
    virtual RESULT visit_named_Node(named_Node *n, ARGS... args) = 0;
    virtual RESULT visit_noncapture_Node(noncapture_Node *n, ARGS... args) = 0;
    virtual RESULT visit_ref_Node(ref_Node *n, ARGS... args) = 0;
    virtual RESULT visit_rep_Node(rep_Node *n, ARGS... args) = 0;
    virtual RESULT visit_rule_Node(rule_Node *n, ARGS... args) = 0;
    virtual RESULT visit_seq_Node(seq_Node *n, ARGS... args) = 0;
    virtual RESULT visit_special_Node(special_Node *n, ARGS... args) = 0;
    virtual RESULT visit_text_Node(text_Node *n, ARGS... args) = 0;
    virtual RESULT visit_top_Node(top_Node *n, ARGS... args) = 0;
};

template <typename RESULT, typename... ARGS>
class GrammarAstVisitor : public AbstractGrammarAstVisitor<RESULT, ARGS...>
{
  public:
    virtual RESULT default_result(AstNode *n, ARGS... args) { return default_value<RESULT>(); }
    RESULT visit_AstNode(AstNode *n, ARGS... args) override { return default_result(n, args...); }
    RESULT visit_TerminalNode(TerminalNode *n, ARGS... args) override { return visit_AstNode(n, args...); }
    RESULT visit_ID_Node(ID_Node *n, ARGS... args) override { return visit_TerminalNode(n, args...); }
    RESULT visit_InnerNode(InnerNode *n, ARGS... args) override { return visit_AstNode(n, args...); }
    RESULT visit_KeywordNode(KeywordNode *n, ARGS... args) override { return visit_TerminalNode(n, args...); }
    RESULT visit_NUMBER_Node(NUMBER_Node *n, ARGS... args) override { return visit_TerminalNode(n, args...); }
    RESULT visit_STRING_Node(STRING_Node *n, ARGS... args) override { return visit_TerminalNode(n, args...); }
    RESULT visit_alt_Node(alt_Node *n, ARGS... args) override { return visit_InnerNode(n, args...); }
    RESULT visit_capture_Node(capture_Node *n, ARGS... args) override { return visit_InnerNode(n, args...); }
    RESULT visit_element_Node(element_Node *n, ARGS... args) override { return visit_InnerNode(n, args...); }
    RESULT visit_named_Node(named_Node *n, ARGS... args) override { return visit_InnerNode(n, args...); }
    RESULT visit_noncapture_Node(noncapture_Node *n, ARGS... args) override { return visit_InnerNode(n, args...); }
    RESULT visit_ref_Node(ref_Node *n, ARGS... args) override { return visit_InnerNode(n, args...); }
    RESULT visit_rep_Node(rep_Node *n, ARGS... args) override { return visit_InnerNode(n, args...); }
    RESULT visit_rule_Node(rule_Node *n, ARGS... args) override { return visit_InnerNode(n, args...); }
    RESULT visit_seq_Node(seq_Node *n, ARGS... args) override { return visit_InnerNode(n, args...); }
    RESULT visit_special_Node(special_Node *n, ARGS... args) override { return visit_InnerNode(n, args...); }
    RESULT visit_text_Node(text_Node *n, ARGS... args) override { return visit_InnerNode(n, args...); }
    RESULT visit_top_Node(top_Node *n, ARGS... args) override { return visit_InnerNode(n, args...); }
};
