#include "Lilu.visitors.h"
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
    string toString() const override
    {
        return "function " + name + "(" + "):" + rtype;
    }
};

struct ExprNode : public AstNode
{
    string toString() const override
    {
        return "expr";
    }
};

struct BlockNode : public AstNode
{
    vector<FunctionNode const *> functions;
    vector<ExprNode const *> exprs;
    string toString() const override
    {
        return "block[funcs:" + to_string(functions.size()) + " exprs:" + to_string(functions.size()) + "]";
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
            ret = visit(child);
        return ret;
    }

    virtual AstNode const *visit_funcdef(RuleMatch const *rm, void *ctx) override
    {
        FunctionNode *fn = new FunctionNode();
        fn->name = rm->get("name")->sv();
        fn->rtype = rm->get("rtype")->sv();
        currentBlock->functions.push_back(fn);
        auto body = rm->get("body");
        if (body)
            visit(body);
        return fn;
    }

    virtual AstNode const *visit_primary(RuleMatch const *, void *ctx) override { return default_value<AstNode const *>(); }
    virtual AstNode const *visit_arg(RuleMatch const *, void *ctx) override { return default_value<AstNode const *>(); }
    virtual AstNode const *visit_expr(RuleMatch const *rm, void *ctx) override
    {
        ExprNode *en = new ExprNode();
        currentBlock->exprs.push_back(en);
        for (auto child : rm->positional)
            visit(child);
        return en;
    }
    virtual AstNode const *visit_args(RuleMatch const *, void *ctx) override { return default_value<AstNode const *>(); }
    virtual AstNode const *visitTextMatch(TextMatch const *, void *ctx) override { return default_value<AstNode const *>(); }
};

AstNode const *generateAst(Match const *res)
{
    AstBuilder v;
    return v.visit(res);
}
