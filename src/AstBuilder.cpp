#include "Lilu.visitors.h"
#include "expr_parser.h"
#include <unordered_set>

struct AstNode
{
    virtual ~AstNode() {}
    virtual string toString() const
    {
        return typeid(this).name();
    }
};

struct FunctionNode : public AstNode
{
    string name, rtype;
    AstNode const *body;
    string toString() const override
    {
        return "function " + name + "(" + "):" + rtype + " is\n" + body->toString() + "end";
    }
};

struct ExprNode : public AstNode
{
    string toString() const override
    {
        return "expr";
    }
};

struct IdExprNode : public ExprNode
{
    string id;
    IdExprNode(string_view sv) : id(sv) {}
    IdExprNode(string const &s) : id(s) {}
    string toString() const override
    {
        return id;
    }
};

struct FuncallExprNode : public ExprNode
{
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

class AstBuilder : public LiluVisitor<AstNode const *>
{
    BlockNode *currentBlock;

  public:
    virtual AstNode const *visit_lilufile(RuleMatch const *rm, void *ctx) override
    {
        currentBlock = nullptr;
        for (auto child : rm->positional)
            visit(child);
        return currentBlock;
    }

    virtual AstNode const *visit_block(RuleMatch const *rm, void *ctx) override
    {
        BlockNode *outer = currentBlock;
        auto block = new BlockNode();
        currentBlock = block;
        for (auto child : rm->positional)
            visit(child);
        if (outer)
            currentBlock = outer;
        return block;
    }

    virtual AstNode const *visit_element(RuleMatch const *rm, void *ctx) override
    {
        AstNode const *ret = nullptr;
        for (auto child : rm->positional)
        {
            ret = visit(child);
            currentBlock->elements.push_back(ret);
        }
        return ret;
    }

    virtual AstNode const *visit_funcdef(RuleMatch const *rm, void *ctx) override
    {
        FunctionNode *fn = new FunctionNode();
        fn->name = rm->get("name")->sv();
        fn->rtype = rm->get("rtype")->sv();
        auto body = rm->get("body");
        if (body)
            fn->body = visit(body);
        return fn;
    }

    virtual AstNode const *visit_primary(RuleMatch const *rm, void *ctx) override
    {
        auto id = rm->get("ID");
        if (id != nullptr)
        {
            return new IdExprNode(((TextMatch const *)id)->sv());
        }
        auto num = rm->get("NUMBER");
        if (num != nullptr)
        {
            return new IdExprNode(((TextMatch const *)num)->sv());
        }
        auto ex = rm->get("expr");
        if (ex != nullptr)
        {
            return visit(ex);
        }
        cout << "unknown primary: ";
        rm->print(0);
        return default_value<AstNode const *>();
    }
    virtual AstNode const *visit_arg(RuleMatch const *, void *ctx) override { return default_value<AstNode const *>(); }
    virtual AstNode const *visit_expr(RuleMatch const *rm, void *ctx) override
    {
        FuncallExprNode *en = new FuncallExprNode();
        for (auto child : rm->positional)
        {
            auto ch = (ExprNode const *)visit(child);
            en->exprs.push_back(ch);
        }
        return en;
    }
    virtual AstNode const *visit_args(RuleMatch const *, void *ctx) override { return default_value<AstNode const *>(); }
    virtual AstNode const *visitTextMatch(TextMatch const *, void *ctx) override { return default_value<AstNode const *>(); }
    virtual AstNode const *visitExprMatch(RuleMatch const *rm, void *ctx) override
    {
        return visit_expr(rm, ctx);
    }
    virtual AstNode const *visitOperatorMatch(Operator const *rm, void *ctx) override
    {
        return new IdExprNode(rm->name);
    }
};

AstNode const *generateAst(Match const *res)
{
    AstBuilder v;
    return v.visit(res);
}
