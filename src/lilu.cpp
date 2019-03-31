#include "expr_parser.h"
#include "lilu.gen.cpp"

ResultMap<Operator> resmap = ResultMap<Operator>()
                                 .add(new Operator(2, "+", 0))
                                 .add(new Operator(2, "-", 0))
                                 .add(new Operator(1, "*", 0))
                                 .add(new Operator(1, "/", 0));

Match const *primary_proxy(lilu *g, Cursor &c)
{
    return g->primary(c);
}

Match const *lilu::expr(Cursor &c)
{
    return expr_parser(c, this, primary_proxy, resmap);
}