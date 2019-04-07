#include "Grammar.visitors.h"
#include <algorithm>
#include <common.h>
#include <set>
#include <unordered_set>
#include <vector>

template <typename T>
static inline bool contains(vector<T> const &vec, T const &item)
{
    return find(vec.begin(), vec.end(), item) != vec.end();
}

static void sortAnisAdd(vector<AstNodeInfo *> &anis, AstNodeInfo *add)
{
    if (contains(anis, add))
        return;
    if (add->parentType.empty())
    {
        anis.push_back(add);
        return;
    }
    sortAnisAdd(anis, astNodes[add->parentType]);
    anis.push_back(add);
}

static void sortAnis(vector<AstNodeInfo *> &anis)
{
    for (auto a : astNodes)
    {
        sortAnisAdd(anis, a.second);
    }
}

class AstHeaderGenerator : public GrammarTreeWalker<void *, void>
{
    ofstream file;
    string cname;
    string dir;
    vector<AstNodeInfo *> anis;

  public:
    AstHeaderGenerator(string const &d) : dir(d) {}
    virtual bool pre_top(RuleMatch const *r, void *&, void *&) override
    {
        cname = r->get("ID")->sv();
        string fname = dir + "/" + cname + ".ast.h";
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
            << "#pragma once" << endl
            << "#include \"parser_base.h\"" << endl
            << endl;

        sortAnis(anis);
        return true;
    }

    virtual void post_top(RuleMatch const *r, void *&, void *&) override
    {
        file
            << "enum" << endl
            << "{" << endl
            << "    __no_node__ = 0";
        for (auto n : anis)
        {
            file << "," << endl
                 << "    " << n->type << "TypeId = " << n->nodeTypeId;
        }
        file << endl
             << "};" << endl
             << endl;

        for (auto node : anis)
        {
            string &n = node->type;
            file
                << "struct " << n;
            string &pn = node->parentType;
            if (!pn.empty())
                file << " : public " << pn;
            file
                << endl
                << "{" << endl
                << "    " << n << "() { _nodeTypeId = " << n << "TypeId; }" << endl;
            for (auto nipair : node->names)
            {
                auto &ni = nipair.second;
                file << "    ";
                if (ni.isMulti)
                {
                    file << "vector<" << ni.type << "> ";
                }
                else
                {
                    file << ni.type << " ";
                }
                file << nipair.first << ";" << endl;
            }
            file
                << "};" << endl
                << endl;
        }

        file
            << "template <typename RESULT, typename... ARGS>" << endl
            << "class Abstract" << cname << "AstVisitor" << endl
            << "{" << endl
            << "  public:" << endl
            << "    virtual ~Abstract" << cname << "AstVisitor() {}" << endl
            << "    RESULT visit(AstNode *n, ARGS... args)" << endl
            << "    {" << endl
            << "        switch (n->_nodeTypeId)" << endl
            << "        {" << endl;
        for (auto node : anis)
        {
            string &n = node->type;
            file
                << "        case " << n << "TypeId:" << endl
                << "            return visit_" << n << "((" << n << " *)n, args...);" << endl;
        }
        file
            << "        }" << endl
            << "        return default_value<RESULT>();" << endl
            << "    }" << endl
            << "    void visitChildren(InnerNode *n, ARGS... args)" << endl
            << "    {" << endl
            << "        for (auto c : n->_children)" << endl
            << "        {" << endl
            << "            visit(c, args...);" << endl
            << "        }" << endl
            << "    }" << endl;

        for (auto node : anis)
        {
            string &n = node->type;
            file
                << "    virtual RESULT visit_" << n << "(" << n << " *n, ARGS... args) = 0;" << endl;
        }
        file
            << "};" << endl
            << endl;

        file
            << "template <typename RESULT, typename... ARGS>" << endl
            << "class " << cname << "AstVisitor : public Abstract" << cname << "AstVisitor<RESULT, ARGS...>" << endl
            << "{" << endl
            << "  public:" << endl;

        for (auto node : anis)
        {
            string &n = node->type;
            file
                << "    virtual RESULT visit_" << n << "(" << n << " *n, ARGS... args) { return default_value<RESULT>(); }" << endl;
        }
        file
            << "};" << endl
            << endl;

        file.close();
    }
};

void generateAstHeader(string const &dir, Match const *res)
{
    AstHeaderGenerator v(dir);
    v.visit(res);
}
