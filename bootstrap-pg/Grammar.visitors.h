#include "parser_base.h"

enum
{
    no_match=0,
    text_match=1,
    expr_match=2,
    before_rules,
    rule_noncapture,
    rule_capture,
    rule_text,
    rule_special,
    rule_element,
    rule_top,
    rule_rule,
    rule_rep,
    rule_alt,
    rule_seq,
    rule_ref,
    rule_named
};
template<typename RESULT>
class AbstractGrammarVisitor
{
  public:
    virtual ~AbstractGrammarVisitor() {}
    RESULT visit(Match const *m, void *ctx = nullptr)
    {
        switch (m->rule_index)
        {
        case rule_noncapture:
            return visit_noncapture((RuleMatch const *)m, ctx);
        case rule_capture:
            return visit_capture((RuleMatch const *)m, ctx);
        case rule_text:
            return visit_text((RuleMatch const *)m, ctx);
        case rule_special:
            return visit_special((RuleMatch const *)m, ctx);
        case rule_element:
            return visit_element((RuleMatch const *)m, ctx);
        case rule_top:
            return visit_top((RuleMatch const *)m, ctx);
        case rule_rule:
            return visit_rule((RuleMatch const *)m, ctx);
        case rule_rep:
            return visit_rep((RuleMatch const *)m, ctx);
        case rule_alt:
            return visit_alt((RuleMatch const *)m, ctx);
        case rule_seq:
            return visit_seq((RuleMatch const *)m, ctx);
        case rule_ref:
            return visit_ref((RuleMatch const *)m, ctx);
        case rule_named:
            return visit_named((RuleMatch const *)m, ctx);
        }
        return default_value<RESULT>();
    }
    void visitChildren(RuleMatch const *m, void *ctx)
    {
        for (auto c : m->positional)
        {
            visit(c, ctx);
        }
    }
    virtual RESULT visit_noncapture(RuleMatch const *m, void *ctx) = 0;
    virtual RESULT visit_capture(RuleMatch const *m, void *ctx) = 0;
    virtual RESULT visit_text(RuleMatch const *m, void *ctx) = 0;
    virtual RESULT visit_special(RuleMatch const *m, void *ctx) = 0;
    virtual RESULT visit_element(RuleMatch const *m, void *ctx) = 0;
    virtual RESULT visit_top(RuleMatch const *m, void *ctx) = 0;
    virtual RESULT visit_rule(RuleMatch const *m, void *ctx) = 0;
    virtual RESULT visit_rep(RuleMatch const *m, void *ctx) = 0;
    virtual RESULT visit_alt(RuleMatch const *m, void *ctx) = 0;
    virtual RESULT visit_seq(RuleMatch const *m, void *ctx) = 0;
    virtual RESULT visit_ref(RuleMatch const *m, void *ctx) = 0;
    virtual RESULT visit_named(RuleMatch const *m, void *ctx) = 0;
    virtual RESULT visitTextMatch(TextMatch const *, void *ctx) = 0;
};

template<typename RESULT>
class GrammarVisitor : public AbstractGrammarVisitor<RESULT>
{
  public:
    virtual RESULT visit_noncapture(RuleMatch const *, void *ctx) override { return default_value<RESULT>(); }
    virtual RESULT visit_capture(RuleMatch const *, void *ctx) override { return default_value<RESULT>(); }
    virtual RESULT visit_text(RuleMatch const *, void *ctx) override { return default_value<RESULT>(); }
    virtual RESULT visit_special(RuleMatch const *, void *ctx) override { return default_value<RESULT>(); }
    virtual RESULT visit_element(RuleMatch const *, void *ctx) override { return default_value<RESULT>(); }
    virtual RESULT visit_top(RuleMatch const *, void *ctx) override { return default_value<RESULT>(); }
    virtual RESULT visit_rule(RuleMatch const *, void *ctx) override { return default_value<RESULT>(); }
    virtual RESULT visit_rep(RuleMatch const *, void *ctx) override { return default_value<RESULT>(); }
    virtual RESULT visit_alt(RuleMatch const *, void *ctx) override { return default_value<RESULT>(); }
    virtual RESULT visit_seq(RuleMatch const *, void *ctx) override { return default_value<RESULT>(); }
    virtual RESULT visit_ref(RuleMatch const *, void *ctx) override { return default_value<RESULT>(); }
    virtual RESULT visit_named(RuleMatch const *, void *ctx) override { return default_value<RESULT>(); }
    virtual RESULT visitTextMatch(TextMatch const *, void *ctx) override { return default_value<RESULT>(); }
};

template <typename CONTEXT, typename RESULT>
class GrammarTreeWalker : public GrammarVisitor<RESULT>
{
  public:
    struct pre_result
    {
        RESULT result;
        bool success;
    };
    inline pre_result ok() { return {RESULT(), true}; }
    struct result_combiner
    {
        virtual ~result_combiner(){};
        virtual RESULT result() = 0;
        virtual void add(RESULT res) = 0;
    };
    struct last_result_combiner : public result_combiner
    {
        RESULT last;
        virtual RESULT result() override { return last; }
        virtual void add(RESULT res) override { last = res; };
        static result_combiner *combine(result_combiner *rc, RESULT res)
        {
            if (rc == nullptr)
                rc = new last_result_combiner();
            rc->add(res);
            return rc;
        }
    };
    virtual result_combiner *combine_results_noncapture(result_combiner *rc, RESULT res)
    {
        return last_result_combiner::combine(rc, res);
    }
    virtual RESULT visit_noncapture(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        pre_result res=pre_noncapture(r, local, *(CONTEXT *)ctx);
        if (res.success)
        {
            result_combiner *rc = combine_results_element(nullptr, res.result);
            for (auto c : r->positional)
            {
                rc = combine_results_noncapture(rc, this->visit(c, &local));
            }
            RESULT combined = rc ? rc->result() : res.result;
            delete rc;
            return post_noncapture(r, combined, local, *(CONTEXT *)ctx);
        }
        else
        {
            return res.result;
        }
    }
    virtual pre_result pre_noncapture(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return ok();
    }
    virtual RESULT post_noncapture(RuleMatch const *r, RESULT result, CONTEXT &ctx, CONTEXT &parent) { return result; }

    virtual result_combiner *combine_results_capture(result_combiner *rc, RESULT res)
    {
        return last_result_combiner::combine(rc, res);
    }
    virtual RESULT visit_capture(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        pre_result res=pre_capture(r, local, *(CONTEXT *)ctx);
        if (res.success)
        {
            result_combiner *rc = combine_results_element(nullptr, res.result);
            for (auto c : r->positional)
            {
                rc = combine_results_capture(rc, this->visit(c, &local));
            }
            RESULT combined = rc ? rc->result() : res.result;
            delete rc;
            return post_capture(r, combined, local, *(CONTEXT *)ctx);
        }
        else
        {
            return res.result;
        }
    }
    virtual pre_result pre_capture(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return ok();
    }
    virtual RESULT post_capture(RuleMatch const *r, RESULT result, CONTEXT &ctx, CONTEXT &parent) { return result; }

    virtual result_combiner *combine_results_text(result_combiner *rc, RESULT res)
    {
        return last_result_combiner::combine(rc, res);
    }
    virtual RESULT visit_text(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        pre_result res=pre_text(r, local, *(CONTEXT *)ctx);
        if (res.success)
        {
            result_combiner *rc = combine_results_element(nullptr, res.result);
            for (auto c : r->positional)
            {
                rc = combine_results_text(rc, this->visit(c, &local));
            }
            RESULT combined = rc ? rc->result() : res.result;
            delete rc;
            return post_text(r, combined, local, *(CONTEXT *)ctx);
        }
        else
        {
            return res.result;
        }
    }
    virtual pre_result pre_text(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return ok();
    }
    virtual RESULT post_text(RuleMatch const *r, RESULT result, CONTEXT &ctx, CONTEXT &parent) { return result; }

    virtual result_combiner *combine_results_special(result_combiner *rc, RESULT res)
    {
        return last_result_combiner::combine(rc, res);
    }
    virtual RESULT visit_special(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        pre_result res=pre_special(r, local, *(CONTEXT *)ctx);
        if (res.success)
        {
            result_combiner *rc = combine_results_element(nullptr, res.result);
            for (auto c : r->positional)
            {
                rc = combine_results_special(rc, this->visit(c, &local));
            }
            RESULT combined = rc ? rc->result() : res.result;
            delete rc;
            return post_special(r, combined, local, *(CONTEXT *)ctx);
        }
        else
        {
            return res.result;
        }
    }
    virtual pre_result pre_special(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return ok();
    }
    virtual RESULT post_special(RuleMatch const *r, RESULT result, CONTEXT &ctx, CONTEXT &parent) { return result; }

    virtual result_combiner *combine_results_element(result_combiner *rc, RESULT res)
    {
        return last_result_combiner::combine(rc, res);
    }
    virtual RESULT visit_element(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        pre_result res=pre_element(r, local, *(CONTEXT *)ctx);
        if (res.success)
        {
            result_combiner *rc = combine_results_element(nullptr, res.result);
            for (auto c : r->positional)
            {
                rc = combine_results_element(rc, this->visit(c, &local));
            }
            RESULT combined = rc ? rc->result() : res.result;
            delete rc;
            return post_element(r, combined, local, *(CONTEXT *)ctx);
        }
        else
        {
            return res.result;
        }
    }
    virtual pre_result pre_element(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return ok();
    }
    virtual RESULT post_element(RuleMatch const *r, RESULT result, CONTEXT &ctx, CONTEXT &parent) { return result; }

    virtual result_combiner *combine_results_top(result_combiner *rc, RESULT res)
    {
        return last_result_combiner::combine(rc, res);
    }
    virtual RESULT visit_top(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        pre_result res=pre_top(r, local, *(CONTEXT *)ctx);
        if (res.success)
        {
            result_combiner *rc = combine_results_element(nullptr, res.result);
            for (auto c : r->positional)
            {
                rc = combine_results_top(rc, this->visit(c, &local));
            }
            RESULT combined = rc ? rc->result() : res.result;
            delete rc;
            return post_top(r, combined, local, *(CONTEXT *)ctx);
        }
        else
        {
            return res.result;
        }
    }
    virtual pre_result pre_top(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return ok();
    }
    virtual RESULT post_top(RuleMatch const *r, RESULT result, CONTEXT &ctx, CONTEXT &parent) { return result; }

    virtual result_combiner *combine_results_rule(result_combiner *rc, RESULT res)
    {
        return last_result_combiner::combine(rc, res);
    }
    virtual RESULT visit_rule(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        pre_result res=pre_rule(r, local, *(CONTEXT *)ctx);
        if (res.success)
        {
            result_combiner *rc = combine_results_element(nullptr, res.result);
            for (auto c : r->positional)
            {
                rc = combine_results_rule(rc, this->visit(c, &local));
            }
            RESULT combined = rc ? rc->result() : res.result;
            delete rc;
            return post_rule(r, combined, local, *(CONTEXT *)ctx);
        }
        else
        {
            return res.result;
        }
    }
    virtual pre_result pre_rule(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return ok();
    }
    virtual RESULT post_rule(RuleMatch const *r, RESULT result, CONTEXT &ctx, CONTEXT &parent) { return result; }

    virtual result_combiner *combine_results_rep(result_combiner *rc, RESULT res)
    {
        return last_result_combiner::combine(rc, res);
    }
    virtual RESULT visit_rep(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        pre_result res=pre_rep(r, local, *(CONTEXT *)ctx);
        if (res.success)
        {
            result_combiner *rc = combine_results_element(nullptr, res.result);
            for (auto c : r->positional)
            {
                rc = combine_results_rep(rc, this->visit(c, &local));
            }
            RESULT combined = rc ? rc->result() : res.result;
            delete rc;
            return post_rep(r, combined, local, *(CONTEXT *)ctx);
        }
        else
        {
            return res.result;
        }
    }
    virtual pre_result pre_rep(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return ok();
    }
    virtual RESULT post_rep(RuleMatch const *r, RESULT result, CONTEXT &ctx, CONTEXT &parent) { return result; }

    virtual result_combiner *combine_results_alt(result_combiner *rc, RESULT res)
    {
        return last_result_combiner::combine(rc, res);
    }
    virtual RESULT visit_alt(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        pre_result res=pre_alt(r, local, *(CONTEXT *)ctx);
        if (res.success)
        {
            result_combiner *rc = combine_results_element(nullptr, res.result);
            for (auto c : r->positional)
            {
                rc = combine_results_alt(rc, this->visit(c, &local));
            }
            RESULT combined = rc ? rc->result() : res.result;
            delete rc;
            return post_alt(r, combined, local, *(CONTEXT *)ctx);
        }
        else
        {
            return res.result;
        }
    }
    virtual pre_result pre_alt(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return ok();
    }
    virtual RESULT post_alt(RuleMatch const *r, RESULT result, CONTEXT &ctx, CONTEXT &parent) { return result; }

    virtual result_combiner *combine_results_seq(result_combiner *rc, RESULT res)
    {
        return last_result_combiner::combine(rc, res);
    }
    virtual RESULT visit_seq(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        pre_result res=pre_seq(r, local, *(CONTEXT *)ctx);
        if (res.success)
        {
            result_combiner *rc = combine_results_element(nullptr, res.result);
            for (auto c : r->positional)
            {
                rc = combine_results_seq(rc, this->visit(c, &local));
            }
            RESULT combined = rc ? rc->result() : res.result;
            delete rc;
            return post_seq(r, combined, local, *(CONTEXT *)ctx);
        }
        else
        {
            return res.result;
        }
    }
    virtual pre_result pre_seq(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return ok();
    }
    virtual RESULT post_seq(RuleMatch const *r, RESULT result, CONTEXT &ctx, CONTEXT &parent) { return result; }

    virtual result_combiner *combine_results_ref(result_combiner *rc, RESULT res)
    {
        return last_result_combiner::combine(rc, res);
    }
    virtual RESULT visit_ref(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        pre_result res=pre_ref(r, local, *(CONTEXT *)ctx);
        if (res.success)
        {
            result_combiner *rc = combine_results_element(nullptr, res.result);
            for (auto c : r->positional)
            {
                rc = combine_results_ref(rc, this->visit(c, &local));
            }
            RESULT combined = rc ? rc->result() : res.result;
            delete rc;
            return post_ref(r, combined, local, *(CONTEXT *)ctx);
        }
        else
        {
            return res.result;
        }
    }
    virtual pre_result pre_ref(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return ok();
    }
    virtual RESULT post_ref(RuleMatch const *r, RESULT result, CONTEXT &ctx, CONTEXT &parent) { return result; }

    virtual result_combiner *combine_results_named(result_combiner *rc, RESULT res)
    {
        return last_result_combiner::combine(rc, res);
    }
    virtual RESULT visit_named(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        pre_result res=pre_named(r, local, *(CONTEXT *)ctx);
        if (res.success)
        {
            result_combiner *rc = combine_results_element(nullptr, res.result);
            for (auto c : r->positional)
            {
                rc = combine_results_named(rc, this->visit(c, &local));
            }
            RESULT combined = rc ? rc->result() : res.result;
            delete rc;
            return post_named(r, combined, local, *(CONTEXT *)ctx);
        }
        else
        {
            return res.result;
        }
    }
    virtual pre_result pre_named(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return ok();
    }
    virtual RESULT post_named(RuleMatch const *r, RESULT result, CONTEXT &ctx, CONTEXT &parent) { return result; }

};

template <typename CONTEXT>
class GrammarTreeWalker<CONTEXT, void> : public GrammarVisitor<void>
{
  public:
    virtual void visit_noncapture(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_noncapture(r, local, *(CONTEXT *)ctx))
        {
            this->visitChildren(r, &local);
            post_noncapture(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_noncapture(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_noncapture(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent) {}

    virtual void visit_capture(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_capture(r, local, *(CONTEXT *)ctx))
        {
            this->visitChildren(r, &local);
            post_capture(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_capture(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_capture(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent) {}

    virtual void visit_text(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_text(r, local, *(CONTEXT *)ctx))
        {
            this->visitChildren(r, &local);
            post_text(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_text(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_text(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent) {}

    virtual void visit_special(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_special(r, local, *(CONTEXT *)ctx))
        {
            this->visitChildren(r, &local);
            post_special(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_special(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_special(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent) {}

    virtual void visit_element(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_element(r, local, *(CONTEXT *)ctx))
        {
            this->visitChildren(r, &local);
            post_element(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_element(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_element(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent) {}

    virtual void visit_top(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_top(r, local, *(CONTEXT *)ctx))
        {
            this->visitChildren(r, &local);
            post_top(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_top(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_top(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent) {}

    virtual void visit_rule(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_rule(r, local, *(CONTEXT *)ctx))
        {
            this->visitChildren(r, &local);
            post_rule(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_rule(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_rule(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent) {}

    virtual void visit_rep(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_rep(r, local, *(CONTEXT *)ctx))
        {
            this->visitChildren(r, &local);
            post_rep(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_rep(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_rep(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent) {}

    virtual void visit_alt(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_alt(r, local, *(CONTEXT *)ctx))
        {
            this->visitChildren(r, &local);
            post_alt(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_alt(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_alt(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent) {}

    virtual void visit_seq(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_seq(r, local, *(CONTEXT *)ctx))
        {
            this->visitChildren(r, &local);
            post_seq(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_seq(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_seq(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent) {}

    virtual void visit_ref(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_ref(r, local, *(CONTEXT *)ctx))
        {
            this->visitChildren(r, &local);
            post_ref(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_ref(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_ref(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent) {}

    virtual void visit_named(RuleMatch const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_named(r, local, *(CONTEXT *)ctx))
        {
            this->visitChildren(r, &local);
            post_named(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_named(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_named(RuleMatch const *r, CONTEXT &ctx, CONTEXT &parent) {}

};

class GrammarPrintWalker : public GrammarTreeWalker<void*, void>
{
  public:
    virtual bool pre_noncapture(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "enter 'noncapture'" << endl;
        return true;
    }
    virtual void post_noncapture(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "leave 'noncapture'" << endl;
    }

    virtual bool pre_capture(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "enter 'capture'" << endl;
        return true;
    }
    virtual void post_capture(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "leave 'capture'" << endl;
    }

    virtual bool pre_text(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "enter 'text'" << endl;
        return true;
    }
    virtual void post_text(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "leave 'text'" << endl;
    }

    virtual bool pre_special(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "enter 'special'" << endl;
        return true;
    }
    virtual void post_special(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "leave 'special'" << endl;
    }

    virtual bool pre_element(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "enter 'element'" << endl;
        return true;
    }
    virtual void post_element(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "leave 'element'" << endl;
    }

    virtual bool pre_top(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "enter 'top'" << endl;
        return true;
    }
    virtual void post_top(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "leave 'top'" << endl;
    }

    virtual bool pre_rule(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "enter 'rule'" << endl;
        return true;
    }
    virtual void post_rule(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "leave 'rule'" << endl;
    }

    virtual bool pre_rep(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "enter 'rep'" << endl;
        return true;
    }
    virtual void post_rep(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "leave 'rep'" << endl;
    }

    virtual bool pre_alt(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "enter 'alt'" << endl;
        return true;
    }
    virtual void post_alt(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "leave 'alt'" << endl;
    }

    virtual bool pre_seq(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "enter 'seq'" << endl;
        return true;
    }
    virtual void post_seq(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "leave 'seq'" << endl;
    }

    virtual bool pre_ref(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "enter 'ref'" << endl;
        return true;
    }
    virtual void post_ref(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "leave 'ref'" << endl;
    }

    virtual bool pre_named(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "enter 'named'" << endl;
        return true;
    }
    virtual void post_named(RuleMatch const *r, void* &ctx, void* &parent) override
    {
        cout << "leave 'named'" << endl;
    }

};

