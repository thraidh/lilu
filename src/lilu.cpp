#include "expr_parser.h"
#include "lilu.gen.h"
#include "lilu.visitors.h"

ResultMap<Operator>
    resmap = ResultMap<Operator>()
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

int parse()
{
    ifstream file("first.lilu");

    if (file.is_open())
    {
        stringstream ss;
        ss << file.rdbuf();
        string text = ss.str();
        cout << "read: " << text << endl;
        Cursor c(text.c_str());

        lilu grammar;
        Match const *res = grammar.lilufile(c);
        if (res != nullptr)
        {
            res->print(0);
            cout << endl;
            liluPrintWalker v;
            v.visit(res);
        }
        cout << "rest: [[[" << c << "]]]" << endl;
    }

    return 0;
}
