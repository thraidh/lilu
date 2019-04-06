#include "AstBuilder.h"
#include "Lilu.gen.h"
#include "Lilu.visitors.h"
#include "expr_parser.h"

ResultMap<Operator> resmap =
    ResultMap<Operator>()
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

void buildIRFrom(AstNode const *n);

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
        grammar.keywords.push_back("end");
        Match const *res = grammar.lilufile(c);
        if (res != nullptr)
        {
            res->print(0);
            cout << endl;
            AstNode const *ast = generateAst(res);
            cout << "AST: " << ast->toString() << endl;
            buildIRFrom(ast);
        }
        cout << "rest: [[[" << c << "]]]" << endl;
    }

    return 0;
}

// these should be their own cpp file
// template <typename V>
// V default_value() { return {}; }

// template <>
// void default_value<void>() { return; }
