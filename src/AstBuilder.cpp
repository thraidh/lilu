#include "AstBuilder.h"
#include "Lilu.visitors.h"
#include "expr_parser.h"

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
        visit(rm->get("args"), fn);
        auto body = rm->get("body");
        if (body)
            fn->body = visit(body);
        return fn;
    }

    virtual AstNode const *visit_primary(RuleMatch const *rm, void *ctx) override
    {
        return visit(rm->child());
    }
    virtual AstNode const *visit_id(RuleMatch const *rm, void *ctx) override
    {
        return new IdExprNode(((TextMatch const *)rm->child())->sv());
    }
    virtual AstNode const *visit_number(RuleMatch const *rm, void *ctx) override
    {
        return new IdExprNode(((TextMatch const *)rm->child())->sv());
    }
    virtual AstNode const *visit_arg(RuleMatch const *rm, void *ctx) override
    {
        FunctionNode *fn = (FunctionNode *)ctx;
        ArgNode *n = new ArgNode();
        n->name = rm->get("name")->sv();
        n->type = rm->get("type")->sv();
        fn->args.push_back(n);
        return n;
    }
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
    virtual AstNode const *visit_args(RuleMatch const *rm, void *ctx) override
    {
        for (auto child : rm->positional)
        {
            visit(child, ctx);
        }
        return nullptr;
    }
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
