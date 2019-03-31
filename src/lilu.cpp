#include "Lilu.gen.h"
#include "Lilu.visitors.h"
#include "expr_parser.h"

ResultMap<Operator>
    resmap = ResultMap<Operator>()
                 .add(new Operator(2, "+", 0))
                 .add(new Operator(2, "-", 0))
                 .add(new Operator(1, "*", 0))
                 .add(new Operator(1, "/", 0));

Match const *primary_proxy(Lilu *g, Cursor &c)
{
    return g->primary(c);
}

Match const *Lilu::expr(Cursor &c)
{
    return expr_parser(c, this, primary_proxy, resmap);
}

int parse(string const &inputname)
{
    ifstream file(inputname);

    if (file.is_open())
    {
        stringstream ss;
        ss << file.rdbuf();
        string text = ss.str();
        cout << "read: " << text << endl;
        Cursor c(text.c_str());

        Lilu grammar;
        Match const *res = grammar.lilufile(c);
        if (res != nullptr)
        {
            res->print(0);
            cout << endl;
            LiluPrintWalker v;
            v.visit(res);
            LiluTreeWalker<int, int> vv;
            vv.visit(res);
        }
        cout << "rest: [[[" << c << "]]]" << endl;
    }

    return 0;
}

// these should be their own cpp file
template <typename V>
V default_value() { return {}; }

template <>
void default_value<void>() { return; }
