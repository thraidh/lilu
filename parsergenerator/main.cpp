#include "Grammar.gen.h"
#include "Grammar.visitors.h"
#include "expr_parser.h"

void generateHeader(string const &dir, Match const *res);
void generateImplementation(string const &dir, Match const *res);
void generateVisitors(string const &dir, Match const *res);

int parse(string const &inputname, string const &dir)
{
    ifstream file(inputname);

    if (file.is_open())
    {
        stringstream ss;
        ss << file.rdbuf();
        string text = ss.str();
        cout << "read: " << text << endl;
        Cursor c(text.c_str());

        Grammar grammar;
        Match const *res = grammar.top(c);
        if (res != nullptr)
        {
            res->print(0);
            cout << endl;
            generateHeader(dir, res);
            generateImplementation(dir, res);
            generateVisitors(dir, res);
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

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cerr << "need exactly two arguments (got " << (argc - 1) << ")" << endl;
        return 1;
    }
    parse(argv[1], argv[2]);
    return 0;
}
