#include "expr_parser.h"

enum
{
    idAstNode,
    idArgNode,
    idFunctionNode,
    idExprNode,
    idIdExprNode,
    idNumberExprNode,
    idFuncallExprNode,
    idBlockNode,
};

struct AstNode
{
    virtual int nodeTypeId() const { return idAstNode; }
    virtual ~AstNode() {}
    virtual string toString() const
    {
        return typeid(this).name();
    }
};

struct ArgNode : public AstNode
{
    int nodeTypeId() const override { return idArgNode; }
    string name, type;
    string toString() const override
    {
        return name + ":" + type;
    }
};

struct FunctionNode : public AstNode
{
    int nodeTypeId() const override { return idFunctionNode; }
    string name, rtype;
    AstNode const *body;
    vector<ArgNode const *> args;
    string toString() const override
    {
        string ret = "function " + name + "(";
        auto it = args.begin();
        if (it != args.end())
        {
            ret += (*it)->toString();
            it++;
            while (it != args.end())
            {
                ret += ", ";
                ret += (*it)->toString();
                it++;
            }
        }
        ret += "):" + rtype + " is\n" + body->toString() + "end";
        return ret;
    }
};

struct ExprNode : public AstNode
{
    int nodeTypeId() const override { return idExprNode; }
    string toString() const override
    {
        return "expr";
    }
};

struct IdExprNode : public ExprNode
{
    int nodeTypeId() const override { return idIdExprNode; }
    string id;
    IdExprNode(string_view sv) : id(sv) {}
    IdExprNode(string const &s) : id(s) {}
    string toString() const override
    {
        return id;
    }
};

struct NumberExprNode : public ExprNode
{
    int nodeTypeId() const override { return idNumberExprNode; }
    int num;
    NumberExprNode(string_view sv)
    {
        num = stoi(string(sv));
    }
    NumberExprNode(string const &s)
    {
        num = stoi(s);
    }
    string toString() const override
    {
        return to_string(num);
    }
};

struct FuncallExprNode : public ExprNode
{
    int nodeTypeId() const override { return idFuncallExprNode; }
    vector<ExprNode const *> exprs;
    string toString() const override
    {
        if (exprs.size() == 0)
            return "<empty funcall>";
        auto it = exprs.begin();
        string ret = (*it)->toString() + "(";
        ++it;
        if (it != exprs.end())
        {
            ret += (*it)->toString();
            ++it;
        }
        while (it != exprs.end())
        {
            ret += ", " + (*it)->toString();
            ++it;
        }
        ret += ")";
        return ret;
    }
};

struct BlockNode : public AstNode
{
    int nodeTypeId() const override { return idBlockNode; }
    vector<AstNode const *> elements;
    string toString() const override
    {
        string ret = "[\n";
        for (auto element : elements)
        {
            ret += element->toString() + "\n";
        }
        ret += "]\n";
        return ret;
    }
};

AstNode const *generateAst(Match const *res);

template <typename RESULT>
class AstVisitor
{
  public:
    virtual ~AstVisitor() {}
    RESULT visit(AstNode const *n, void *ctx = nullptr)
    {
        switch (n->nodeTypeId())
        {
        case idAstNode:
            return visit_AstNode((AstNode const *)n, ctx);
        case idArgNode:
            return visit_ArgNode((ArgNode const *)n, ctx);
        case idFunctionNode:
            return visit_FunctionNode((FunctionNode const *)n, ctx);
        case idExprNode:
            return visit_ExprNode((ExprNode const *)n, ctx);
        case idIdExprNode:
            return visit_IdExprNode((IdExprNode const *)n, ctx);
        case idNumberExprNode:
            return visit_NumberExprNode((NumberExprNode const *)n, ctx);
        case idFuncallExprNode:
            return visit_FuncallExprNode((FuncallExprNode const *)n, ctx);
        case idBlockNode:
            return visit_BlockNode((BlockNode const *)n, ctx);
        }
        return default_value<RESULT>();
    }
    virtual RESULT visit_AstNode(AstNode const *n, void *ctx) { return default_value<RESULT>(); };
    virtual RESULT visit_ArgNode(ArgNode const *n, void *ctx) { return default_value<RESULT>(); };
    virtual RESULT visit_FunctionNode(FunctionNode const *n, void *ctx) { return default_value<RESULT>(); };
    virtual RESULT visit_BlockNode(BlockNode const *n, void *ctx) { return default_value<RESULT>(); };
    virtual RESULT visit_ExprNode(ExprNode const *n, void *ctx) { return default_value<RESULT>(); };
    virtual RESULT visit_IdExprNode(IdExprNode const *n, void *ctx) { return default_value<RESULT>(); };
    virtual RESULT visit_NumberExprNode(NumberExprNode const *n, void *ctx) { return default_value<RESULT>(); };
    virtual RESULT visit_FuncallExprNode(FuncallExprNode const *n, void *ctx) { return default_value<RESULT>(); };
};
/*
AstNode
ArgNode
FunctionNode
ExprNode
IdExprNode
NumberExprNode
FuncallExprNode
BlockNode
*/
