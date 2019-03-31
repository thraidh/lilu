#include "Grammar.visitors.h"
#include <unordered_set>

class HeaderGenerator : public GrammarTreeWalker<void *, void>
{
    ofstream file;
    string cname;
    unordered_set<string> rule_seen;
    string dir;

  public:
    HeaderGenerator(string const &d) : dir(d) {}
    virtual bool pre_top(RuleMatch const *r, void *&, void *&) override
    {
        cname = r->get("ID")->sv();
        string fname = dir + "/" + cname + ".gen.h";
        file.open(fname.c_str());
        if (file.is_open())
        {
            cout << "opened " << fname << endl;
        }
        else
        {
            cout << "could not open " << fname << " " << endl;
            return false;
        }

        file
            << "#include \"parser_base.h\"" << endl
            << endl
            << "class " << cname << " : public DefaultGrammar" << endl
            << "{" << endl
            << "  public:" << endl
            << "    virtual ~" << cname << "() {}" << endl;
        return true;
    }
    virtual void post_top(RuleMatch const *r, void *&, void *&) override
    {
        file << "};" << endl;
        file.close();
    }

    virtual void post_rule(RuleMatch const *r, void *&, void *&) override
    {
        string rname;
        rname = r->get("ID")->sv();
        if (rule_seen.insert(rname).second)
            file << "    virtual Match const *" << rname << "(Cursor &c);" << endl;
    }
};

void generateHeader(string const &dir, Match const *res)
{
    HeaderGenerator v(dir);
    v.visit(res);
}
