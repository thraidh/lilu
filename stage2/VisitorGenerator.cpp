#include "Grammar.visitors.h"
#include <unordered_set>

class VisitorGenerator : public GrammarTreeWalker<void *, void>
{
    ofstream file;
    string cname;
    unordered_set<string> rule_seen;
    string dir;

  public:
    VisitorGenerator(string const &d) : dir(d) {}
    virtual bool pre_top(RuleMatch const *r, void *&, void *&) override
    {
        cname = r->get("ID")->sv();
        string fname = dir + "/" + cname + ".visitors.h";
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
            << endl;

        return true;
    }

    virtual void post_top(RuleMatch const *r, void *&, void *&) override
    {
        file
            << "enum" << endl
            << "{" << endl
            << "    no_match=0," << endl
            << "    text_match=1," << endl
            << "    expr_match=2," << endl
            << "    operator_match=3," << endl
            << "    before_rules";
        for (auto n : rule_seen)
        {
            file << "," << endl
                 << "    rule_" << n;
        }
        file << endl
             << "};" << endl;

        file
            << "template<typename RESULT>" << endl
            << "class Abstract" << cname << "Visitor" << endl
            << "{" << endl
            << "  public:" << endl
            << "    virtual ~Abstract" << cname << "Visitor() {}" << endl
            << "    RESULT visit(Match const *m, void *ctx = nullptr)" << endl
            << "    {" << endl
            << "        switch (m->rule_index)" << endl
            << "        {" << endl;
        for (auto n : rule_seen)
        {
            file
                << "        case rule_" << n << ":" << endl
                << "            return visit_" << n << "((RuleMatch const *)m, ctx);" << endl;
        }
        file
            << "        case text_match:" << endl
            << "            return visitTextMatch((TextMatch const *)m, ctx);" << endl
            << "        case expr_match:" << endl
            << "            return visitExprMatch((RuleMatch const *)m, ctx);" << endl
            << "        case operator_match:" << endl
            << "            return visitOperatorMatch((Operator const *)m, ctx);" << endl
            << "        }" << endl
            << "        return default_value<RESULT>();" << endl
            << "    }" << endl
            << "    void visitChildren(RuleMatch const *m, void *ctx)" << endl
            << "    {" << endl
            << "        for (auto c : m->positional)" << endl
            << "        {" << endl
            << "            visit(c, ctx);" << endl
            << "        }" << endl
            << "    }" << endl;

        for (auto n : rule_seen)
        {
            file
                << "    virtual RESULT visit_" << n << "(RuleMatch const *m, void *ctx) = 0;" << endl;
        }
        file
            << "    virtual RESULT visitTextMatch(TextMatch const *m, void *ctx) = 0;" << endl
            << "    virtual RESULT visitExprMatch(RuleMatch const *m, void *ctx) = 0;" << endl
            << "    virtual RESULT visitOperatorMatch(Operator const *m, void *ctx) = 0;" << endl
            << "};" << endl
            << endl;

        file
            << "template<typename RESULT>" << endl
            << "class " << cname << "Visitor : public Abstract" << cname << "Visitor<RESULT>" << endl
            << "{" << endl
            << "  public:" << endl;
        for (auto n : rule_seen)
        {
            file
                << "    virtual RESULT visit_" << n << "(RuleMatch const *m, void *ctx) override { return default_value<RESULT>(); }" << endl;
        }
        file
            << "    virtual RESULT visitTextMatch(TextMatch const *m, void *ctx) override { return default_value<RESULT>(); }" << endl
            << "    virtual RESULT visitExprMatch(RuleMatch const *m, void *ctx) override { return default_value<RESULT>(); }" << endl
            << "    virtual RESULT visitOperatorMatch(Operator const *m, void *ctx) override { return default_value<RESULT>(); }" << endl
            << "};" << endl
            << endl;

        file
            << "template <typename CONTEXT, typename RESULT>" << endl
            << "class " << cname << "TreeWalker : public " << cname << "Visitor<RESULT>" << endl
            << "{" << endl
            << "  public:" << endl
            << "    struct pre_result" << endl
            << "    {" << endl
            << "        RESULT result;" << endl
            << "        bool success;" << endl
            << "    };" << endl
            << "    inline pre_result ok() { return {RESULT(), true}; }" << endl
            << "    struct result_combiner" << endl
            << "    {" << endl
            << "        virtual ~result_combiner(){};" << endl
            << "        virtual RESULT result() = 0;" << endl
            << "        virtual void add(RESULT res) = 0;" << endl
            << "    };" << endl
            << "    struct last_result_combiner : public result_combiner" << endl
            << "    {" << endl
            << "        RESULT last;" << endl
            << "        virtual RESULT result() override { return last; }" << endl
            << "        virtual void add(RESULT res) override { last = res; };" << endl
            << "        static result_combiner *combine(result_combiner *rc, RESULT res)" << endl
            << "        {" << endl
            << "            if (rc == nullptr)" << endl
            << "                rc = new last_result_combiner();" << endl
            << "            rc->add(res);" << endl
            << "            return rc;" << endl
            << "        }" << endl
            << "    };" << endl;
        for (auto n : rule_seen)
        {
            file
                << "    virtual result_combiner *combine_results_" << n << "(result_combiner *rc, RESULT res)" << endl
                << "    {" << endl
                << "        return last_result_combiner::combine(rc, res);" << endl
                << "    }" << endl
                << "    virtual RESULT visit_" << n << "(RuleMatch const *r, void *ctx) override" << endl
                << "    {" << endl
                << "        CONTEXT local;" << endl
                << "        pre_result res=pre_" << n << "(r, local, *(CONTEXT *)ctx);" << endl
                << "        if (res.success)" << endl
                << "        {" << endl
                << "            result_combiner *rc = combine_results_element(nullptr, res.result);" << endl
                << "            for (auto c : r->positional)" << endl
                << "            {" << endl
                << "                rc = combine_results_" << n << "(rc, this->visit(c, &local));" << endl
                << "            }" << endl
                << "            RESULT combined = rc ? rc->result() : res.result;" << endl
                << "            delete rc;" << endl
                << "            return post_" << n << "(r, combined, local, *(CONTEXT *)ctx);" << endl
                << "        }" << endl
                << "        else" << endl
                << "        {" << endl
                << "            return res.result;" << endl
                << "        }" << endl
                << "    }" << endl
                << "    virtual pre_result pre_" << n << "(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)" << endl
                << "    {" << endl
                << "        return ok();" << endl
                << "    }" << endl
                << "    virtual RESULT post_" << n << "(RuleMatch const *r, RESULT result, CONTEXT &ctx, CONTEXT &parent) { return result; }" << endl
                << endl;
        }
        file
            << "};" << endl
            << endl;

        file
            << "template <typename CONTEXT>" << endl
            << "class " << cname << "TreeWalker<CONTEXT, void> : public " << cname << "Visitor<void>" << endl
            << "{" << endl
            << "  public:" << endl;
        for (auto n : rule_seen)
        {
            file
                << "    virtual void visit_" << n << "(RuleMatch const *r, void *ctx) override" << endl
                << "    {" << endl
                << "        CONTEXT local;" << endl
                << "        if (pre_" << n << "(r, local, *(CONTEXT *)ctx))" << endl
                << "        {" << endl
                << "            this->visitChildren(r, &local);" << endl
                << "            post_" << n << "(r, local, *(CONTEXT *)ctx);" << endl
                << "        }" << endl
                << "    }" << endl
                << "    virtual bool pre_" << n << "(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)" << endl
                << "    {" << endl
                << "        return true;" << endl
                << "    }" << endl
                << "    virtual void post_" << n << "(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent) {}" << endl
                << endl;
        }
        file
            << "};" << endl
            << endl;

        file
            << "class " << cname << "PrintWalker : public " << cname << "TreeWalker<void*, void>" << endl
            << "{" << endl
            << "  public:" << endl;
        for (auto n : rule_seen)
        {
            file
                << "    virtual bool pre_" << n << "(RuleMatch const *r, void* &ctx, void* &parent) override" << endl
                << "    {" << endl
                << "        cout << \"enter '" << n << "'\" << endl;" << endl
                << "        return true;" << endl
                << "    }" << endl
                << "    virtual void post_" << n << "(RuleMatch const *r, void* &ctx, void* &parent) override" << endl
                << "    {" << endl
                << "        cout << \"leave '" << n << "'\" << endl;" << endl
                << "    }" << endl
                << endl;
        }
        file
            << "};" << endl
            << endl;

        file.close();
    }

    virtual void post_rule(RuleMatch const *r, void *&, void *&) override
    {
        string rname;
        rname = r->get("ID")->sv();
        rule_seen.insert(rname);
    }
};

void generateVisitors(string const &dir, Match const *res)
{
    VisitorGenerator v(dir);
    v.visit(res);
}
