#include "Grammar.ast.h"
#include "Grammar.visitors.h"
#include <unordered_set>

static string empty_string;

struct ImplData
{
    vector<string> childFuncNames;

    void addChildFunc(string const &name)
    {
        childFuncNames.push_back(name);
    }

    void addChildFunc(ImplData const &other)
    {
        childFuncNames.push_back(other.childFuncName());
    }

    string const &childFuncName() const
    {
        if (childFuncNames.empty())
            return empty_string;
        return childFuncNames.back();
    }
};

class ImplementationGenerator : public GrammarTreeWalker<ImplData, void>
{
    ofstream file;
    string cname;
    unordered_set<string> rule_seen;
    vector<stringstream *> streams;
    stringstream *cur;
    bool have_name;
    int helperId;
    string dir;

    string startHelper(string const &name)
    {
        ++helperId;
        string fname = name + "_" + to_string(helperId);
        file << "static inline bool " << fname << "(" << cname << " &g, Match const *&result, RuleMatch *&submatch, Cursor &c)" << endl
             << "{" << endl;
        return fname;
    }
    void endHelper()
    {
        file << "}" << endl
             << endl;
    }

  public:
    ImplementationGenerator(string const &d) : dir(d) {}
    virtual bool pre_top(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        cname = r->get("ID")->sv();
        string fname = dir + "/" + cname + ".gen.cpp";
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
            << "#include \"" << cname << ".gen.h\"" << endl
            << "#include \"" << cname << ".visitors.h\"" << endl
            << endl;

        helperId = 0;
        return true;
    }
    virtual void post_top(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        file.close();
    }

    virtual bool pre_rule(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        if (r->get("extern") != nullptr)
            return false;
        cur = new stringstream();
        have_name = false;
        return true;
    }

    virtual bool pre_named(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        if (r->get("name") != nullptr)
        {
            have_name = true;
        }
        return true;
    }

    virtual void post_named(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        have_name = false;
        auto n = r->get("name");
        if (!n)
        {
            parent.addChildFunc(local);
        }
        else
        {
            string name(n->sv());
            parent.addChildFunc(startHelper("named"));
            file << "    if (" << local.childFuncName() << "(g, result, submatch, c))" << endl
                 << "    {" << endl
                 << "        submatch = addToMatch(submatch, \"" << name << "\", result);" << endl
                 << "        return true;" << endl
                 << "    }" << endl
                 << "    return false;" << endl;
            endHelper();
        }
    }

    virtual void post_ref(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        string name(r->child()->sv());
        parent.addChildFunc(startHelper("ref"));
        file
            << "    result = g." << name << "(c);" << endl
            << "    if (result)" << endl
            << "    {" << endl
            << "        c.skipSpace();" << endl;
        if (!have_name)
            file << "        submatch = addToMatch(submatch, \"" << name << "\", result);" << endl;
        file
            << "        return true;" << endl
            << "    }" << endl
            << "    return false;" << endl;
        endHelper();
    }

    virtual void post_special(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        parent.addChildFunc(startHelper("special"));
        file
            << "    return true;" << endl;
        endHelper();
    }

    virtual void post_element(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        parent.addChildFunc(local);
    }

    virtual void post_capture(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        parent.addChildFunc(local);
    }

    virtual void post_noncapture(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        parent.addChildFunc(local);
    }

    virtual void post_rep(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        if (auto m = r->get("op"))
        {
            string op(m->sv());
            parent.addChildFunc(startHelper("rep"));
            if (op == "*")
            {
                file
                    << "    while (true)" << endl
                    << "    {" << endl
                    << "        RuleMatch *subsubmatch = nullptr;" << endl
                    << "        if (" << local.childFuncName() << "(g, result, subsubmatch, c))" << endl
                    << "        {" << endl
                    << "            submatch = mergeMatch(submatch, subsubmatch);" << endl
                    << "        }" << endl
                    << "        else" << endl
                    << "        {" << endl
                    << "            delete subsubmatch;" << endl
                    << "            return true;" << endl
                    << "        }" << endl
                    << "    }" << endl;
            }
            else if (op == "?")
            {
                file
                    << "    RuleMatch *subsubmatch = nullptr;" << endl
                    << "    if (" << local.childFuncName() << "(g, result, subsubmatch, c))" << endl
                    << "    {" << endl
                    << "        submatch = mergeMatch(submatch, subsubmatch);" << endl
                    << "    }" << endl
                    << "    return true;" << endl;
            }
            endHelper();
        }
        else
        {
            parent.addChildFunc(local);
        }
    }

    virtual bool pre_seq(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        have_name = false;
        return true;
    }
    virtual void post_seq(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        if (local.childFuncNames.size() <= 1)
        {
            parent.addChildFunc(local);
            return;
        }

        parent.addChildFunc(startHelper("seq"));
        file
            << "    RuleMatch *subsubmatch = nullptr;" << endl
            << "    Cursor start = c;" << endl
            << "" << endl;
        for (auto fn : local.childFuncNames)
        {
            file
                << "    if (!" << fn << "(g, result, subsubmatch, c))" << endl
                << "    {" << endl
                << "        c = start;" << endl
                << "        delete subsubmatch;" << endl
                << "        return false;" << endl
                << "    }" << endl
                << "" << endl;
        }
        file
            << "    submatch = mergeMatch(submatch, subsubmatch);" << endl
            << "    return true;" << endl;
        endHelper();
    }
    virtual void post_alt(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        if (local.childFuncNames.size() <= 1)
        {
            parent.addChildFunc(local);
            return;
        }

        parent.addChildFunc(startHelper("alt"));
        for (auto fn : local.childFuncNames)
        {
            file
                << "    if (" << fn << "(g, result, submatch, c))" << endl
                << "    {" << endl
                << "        return true;" << endl
                << "    }" << endl
                << "" << endl;
        }
        file
            << "    return false;" << endl;
        endHelper();
    }
    virtual void post_text(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        string text(r->child()->sv());
        parent.addChildFunc(startHelper("text"));
        file
            << "    result = g.TEXT(c, " << text << ");" << endl
            << "    if (result)" << endl
            << "    {" << endl
            << "        c.skipSpace();" << endl
            << "        return true;" << endl
            << "    }" << endl
            << "    return false;" << endl;
        endHelper();
    }
    virtual void post_rule(RuleMatch const *r, ImplData &local, ImplData &parent) override
    {
        string rname(r->get("ID")->sv());

        file << "Match const *" << cname << "::" << rname << "(Cursor &c)" << endl
             << "{" << endl
             << "    Match const *inner;" << endl
             << "    RuleMatch *m = nullptr;" << endl
             << "" << endl
             << "    if (" << local.childFuncName() << "(*this, inner, m, c))" << endl
             << "    {" << endl
             << "        if (!m) m = addToMatch(m, nullptr, inner);" << endl
             << "        m->rule_index = rule_" << rname << ";" << endl
             << "        return m;" << endl
             << "    }" << endl
             << "    return nullptr;" << endl
             << "}" << endl
             << endl;
        delete cur;
    }
};

void generateImplementation(string const &dir, Match const *res)
{
    ImplementationGenerator v(dir);
    v.visit(res);
}
