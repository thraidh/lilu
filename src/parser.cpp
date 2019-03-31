#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

class MapResult;
class TextResult;

class visitor
{
  public:
    virtual ~visitor() {}
    virtual void visit_top(MapResult const *, void *ctx) = 0;
    virtual void visit_rule(MapResult const *, void *ctx) = 0;
    virtual void visit_alt(MapResult const *, void *ctx) = 0;
    virtual void visit_seq(MapResult const *, void *ctx) = 0;
    virtual void visit_repe(MapResult const *, void *ctx) = 0;
    virtual void visit_named(MapResult const *, void *ctx) = 0;
    virtual void visit_element(MapResult const *, void *ctx) = 0;
    virtual void visit_special(MapResult const *, void *ctx) = 0;
    virtual void visit_ref(MapResult const *, void *ctx) = 0;
    virtual void visit_capture(MapResult const *, void *ctx) = 0;
    virtual void visit_noncapture(MapResult const *, void *ctx) = 0;
    virtual void visit_txt(MapResult const *, void *ctx) = 0;
    virtual void visitTextResult(TextResult const *) {}
};

template <typename T>
using cps = std::shared_ptr<T const>;

template <typename T>
using ps = std::shared_ptr<T>;

using VisitCallback = void (visitor::*)(MapResult const *, void *ctx);

class Result
{
  public:
    virtual void print(int indent) const = 0;
    virtual void visit(visitor &v, void *ctx = nullptr) const {};
    virtual void addToMap(class MapResult const *map) const {};
    virtual std::string_view text() const { return std::string_view(); }
    virtual std::string str() const { return std::string(text()); }
    virtual ~Result() {}
};

class NullResult : public Result
{
  public:
    virtual void print(int indent) const override {}
};

NullResult const *nullresult = new NullResult();

class TextResult : public Result
{
    char const *b;
    char const *e;

  public:
    TextResult(char const *begin, char const *end) : b(begin), e(end) {}
    virtual void print(int indent) const override
    {
        std::cout << "Text[" << text() << "]" << std::endl;
    }
    virtual void visit(visitor &v, void *ctx = nullptr) const override
    {
        v.visitTextResult(this);
    };
    virtual std::string_view text() const override
    {
        return std::string_view(b, e - b);
    }
};

class NamedResult : public Result
{
  protected:
    Result const *e;
    std::string name;

  public:
    NamedResult(std::string n, Result const *expr) : e(expr), name(n) {}
    virtual void print(int indent) const override
    {
        std::cout << name;
        std::cout << "=(";
        e->print(indent + 1);
        std::cout << ")";
    }
    virtual void addToMap(class MapResult const *map) const override;
};

class MapResult : public NamedResult
{
    mutable std::multimap<std::string, Result const *> map;
    VisitCallback visitcb;

  public:
    MapResult(std::string n, Result const *expr, VisitCallback cb) : NamedResult(n, expr), visitcb(cb) {}
    virtual void print(int indent) const override
    {
        //std::cout << "map> " << name << std::endl;
        e->addToMap(this);
        //std::cout << "<map " << name << std::endl;
        std::cout << "rule " << name << ":" << std::endl;
        ++indent;
        if (map.empty())
        {
            std::cout << std::string(indent * 2, ' ');
            e->print(indent);
        }
        else
        {
            for (auto kv : map)
            {
                std::cout << std::string(indent * 2, ' ') << kv.first << "=";
                kv.second->print(indent);
            }
        }
    }
    virtual void addToMap(MapResult const *map) const override
    {
        //std::cout << "map+> " << name << std::endl;
        map->add(name, this);
        //std::cout << "<map+ " << name << std::endl;
    };
    void add(std::string k, Result const *v) const
    {
        //std::cout << "adding " << k << " to " << name << std::endl;
        map.insert({k, v});
    }
    Result const *get(std::string const &key) const
    {
        auto it = map.find(key);
        if (it == map.end())
            return nullresult;
        return it->second;
    }
    Result const *child() const
    {
        return e;
    }
    virtual void visit(visitor &v, void *ctx) const override
    {
        if (visitcb != nullptr)
        {
            //std::cout << "visit " << name << std::endl;
            (v.*visitcb)(this, ctx);
            //std::cout << "leave " << name << std::endl;
        }
    };
    void visitChildren(visitor &v, void *ctx) const
    {
        if (map.empty())
        {
            e->visit(v);
        }
        else
        {
            for (auto kv : map)
            {
                kv.second->visit(v, ctx);
            }
        }
    }
};

void NamedResult::addToMap(MapResult const *map) const
{
    //std::cout << "named> " << name << std::endl;
    map->add(name, e);
    //e->addToMap(map);
    //std::cout << "<named " << name << std::endl;
};

class MultiResult : public Result
{
    std::vector<Result const *> l;

  public:
    virtual void print(int indent) const override
    {
        std::cout << "[" << std::endl;
        ++indent;
        for (auto e : l)
        {
            std::cout << std::string(indent * 2, ' ');
            e->print(indent);
        }
        --indent;
        std::cout << std::string(indent * 2, ' ') << "]" << std::endl;
    }
    void add(Result const *r)
    {
        l.push_back(r);
    }
    virtual void addToMap(MapResult const *map) const override
    {
        //std::cout << "multi>" << std::endl;
        for (auto r : l)
        {
            r->addToMap(map);
        }
        //std::cout << "<multi" << std::endl;
    };
};

class LexerState
{
  public:
    char const *ptr;
    void error(std::string const &desc)
    {
        char const *end = ptr;
        while (*end != '\n' && *end != 0)
            ++end;
        std::cout << desc << " at [" << std::string_view(ptr, end - ptr) << "]" << std::endl;
    }
};

class GrammarElement
{
  public:
    virtual Result const *match(LexerState &ls) const = 0;
    virtual ~GrammarElement() {}
};

class visitor;

class RuleImpl : public GrammarElement
{
    std::string name;
    VisitCallback visit;

  public:
    cps<GrammarElement> element;
    RuleImpl(std::string n, VisitCallback cb) : name(n), visit(cb) {}
    virtual Result const *match(LexerState &ls) const override
    {
        std::cout << "enter rule " << name << std::endl;
        auto ret = element->match(ls);
        if (ret)
        {
            std::cout << "store in rule " << name << std::endl;
            return new MapResult(name, ret, visit);
        }
        std::cout << "fail rule " << name << std::endl;
        return ret;
    }
};

class Named : public GrammarElement
{
    std::string name;

  public:
    cps<GrammarElement> element;
    Named(std::string n, cps<GrammarElement> e) : name(n), element(e) {}
    virtual Result const *match(LexerState &ls) const override
    {
        auto ret = element->match(ls);
        if (ret)
        {
            std::cout << "store in " << name << std::endl;
            return new NamedResult(name, ret);
        }
        return ret;
    }
};

class visitor;

class Rule
{
    std::shared_ptr<RuleImpl> rule;

  public:
    Rule(std::string n) : rule(std::make_shared<RuleImpl>(n, nullptr)) {}
    Rule(std::string n, VisitCallback cb) : rule(std::make_shared<RuleImpl>(n, cb)) {}
    operator std::shared_ptr<GrammarElement const>() const
    {
        return rule;
    }
    Rule &operator=(std::shared_ptr<GrammarElement const> ge)
    {
        rule->element = ge;
        return *this;
    }
    Result const *match(LexerState &ls) const
    {
        return rule->match(ls);
    }
};

class Repetition : public GrammarElement
{
    std::shared_ptr<GrammarElement const> element;
    int min, max;

  public:
    Repetition(std::shared_ptr<GrammarElement const> ge, int min_, int max_) : element(ge), min(min_), max(max_) {}
    virtual Result const *match(LexerState &ls) const override
    {
        LexerState save = ls;
        auto result = new MultiResult();
        std::cout << "rep(" << std::endl;
        int i = 0;
        while (i != max)
        {
            Result const *ret = element->match(ls);
            if (!ret)
            {
                if (i < min)
                {
                    ls = save;
                    ls.error("rep failed (too few)");
                    return nullptr;
                }
                std::cout << "rep)" << std::endl;
                return result;
            }
            result->add(ret);
            std::cout << "rep+" << std::endl;
            ++i;
        }
        std::cout << "rep max)" << std::endl;
        return result;
    }
};

std::shared_ptr<GrammarElement const> rep(std::shared_ptr<GrammarElement const> ge) { return std::make_shared<Repetition const>(ge, 0, -1); }
std::shared_ptr<GrammarElement const> rep1(std::shared_ptr<GrammarElement const> ge) { return std::make_shared<Repetition const>(ge, 1, -1); }
std::shared_ptr<GrammarElement const> opt(std::shared_ptr<GrammarElement const> ge) { return std::make_shared<Repetition const>(ge, 0, 1); }

class Alternation : public GrammarElement
{
    std::vector<std::shared_ptr<GrammarElement const>> elements;

  public:
    Alternation() {}

    void add(std::shared_ptr<GrammarElement const> ge) { elements.push_back(ge); }

    virtual Result const *match(LexerState &ls) const override
    {
        std::cout << "alt(" << std::endl;
        LexerState save = ls;
        for (auto ge : elements)
        {
            auto ret = ge->match(ls);
            if (ret)
            {
                std::cout << "alt)" << std::endl;
                return ret;
            }
            ls = save;
            std::cout << "alt+" << std::endl;
        }
        ls.error("alt failed");
        return nullptr;
    }
};

ps<Alternation> operator|(ps<Alternation> left, cps<GrammarElement> right)
{
    left->add(right);
    return left;
}

ps<Alternation> operator|(cps<GrammarElement> left, cps<GrammarElement> right)
{
    ps<Alternation> ret = std::make_shared<Alternation>();
    ret->add(left);
    ret->add(right);
    return ret;
}

class Sequence : public GrammarElement
{
    std::vector<std::shared_ptr<GrammarElement const>> elements;

  public:
    Sequence() {}

    void add(std::shared_ptr<GrammarElement const> ge) { elements.push_back(ge); }

    virtual Result const *match(LexerState &ls) const override
    {
        LexerState save = ls;
        auto result = new MultiResult();
        std::cout << "seq(" << std::endl;
        for (auto ge : elements)
        {
            auto ret = ge->match(ls);
            if (!ret)
            {
                ls.error("seq failed");
                ls = save;
                return nullptr;
            }
            result->add(ret);
            std::cout << "seq+" << std::endl;
        }
        std::cout << "seq)" << std::endl;
        return result;
    }
};

ps<Sequence> operator+(ps<Sequence> left, cps<GrammarElement> right)
{
    left->add(right);
    return left;
}

ps<Sequence> operator+(cps<GrammarElement> left, cps<GrammarElement> right)
{
    ps<Sequence> ret = std::make_shared<Sequence>();
    ret->add(left);
    ret->add(right);
    return ret;
}

void skip_space(LexerState &ls)
{
    while (isspace(*ls.ptr))
    {
        ls.ptr++;
    }
}

class Id : public GrammarElement
{
  public:
    virtual Result const *match(LexerState &ls) const override
    {
        LexerState save = ls;
        skip_space(ls);
        char const *start = ls.ptr;
        if (isalpha(*ls.ptr))
        {
            ls.ptr++;
            while (isalnum(*ls.ptr))
            {
                ls.ptr++;
            }
            std::cout << "matched id " << std::string_view(start, ls.ptr - start) << std::endl;
            return new TextResult(start, ls.ptr);
        }
        ls = save;
        ls.error("expected id");
        return nullptr;
    }
};

std::shared_ptr<GrammarElement const> id() { return std::shared_ptr<GrammarElement const>(new Id()); }

class StringToken : public GrammarElement
{
  public:
    virtual Result const *match(LexerState &ls) const override
    {
        LexerState save = ls;
        skip_space(ls);
        char const *start = ls.ptr;
        if (*ls.ptr == '"')
        {
            ls.ptr++;
            while (*ls.ptr != '"')
            {
                if (!*ls.ptr)
                {
                    ls = save;
                    ls.error("unterminated string");
                    return nullptr;
                }
                ls.ptr++;
            }
            ls.ptr++;
            std::cout << "matched string " << std::string_view(start, ls.ptr - start) << std::endl;
            return new TextResult(start, ls.ptr);
        }
        ls = save;
        ls.error("expected string");
        return nullptr;
    }
};

std::shared_ptr<GrammarElement const> stringtoken() { return std::shared_ptr<GrammarElement const>(new StringToken()); }

class Text : public GrammarElement
{
    std::string t;

  public:
    Text(std::string text) : t(text) {}
    virtual Result const *match(LexerState &ls) const override
    {
        LexerState save = ls;
        skip_space(ls);
        char const *start = ls.ptr;
        char const *tptr = t.c_str();
        while (*tptr && *ls.ptr == *tptr)
        {
            ++ls.ptr;
            ++tptr;
        }
        if (*tptr == 0)
        {
            std::cout << "matched text " << std::string_view(start, ls.ptr - start) << std::endl;
            return new TextResult(start, ls.ptr);
        }
        ls = save;
        ls.error("expected text " + t);
        return nullptr;
    }
};

std::shared_ptr<GrammarElement const> text(std::string text) { return std::shared_ptr<GrammarElement const>(new Text(text)); }

ps<Sequence> operator+(ps<Sequence> left, std::string right)
{
    left->add(text(right));
    return left;
}

ps<Sequence> operator+(cps<GrammarElement> left, std::string right)
{
    ps<Sequence> ret = std::make_shared<Sequence>();
    ret->add(left);
    ret->add(text(right));
    return ret;
}

cps<Named> operator/(cps<GrammarElement> left, std::string right)
{
    cps<Named> ret = std::make_shared<Named>(right, left);
    return ret;
}

ps<Sequence> operator+(std::string left, cps<GrammarElement> right)
{
    ps<Sequence> ret = std::make_shared<Sequence>();
    ret->add(text(left));
    ret->add(right);
    return ret;
}

ps<Alternation> operator|(ps<Alternation> left, std::string right)
{
    left->add(text(right));
    return left;
}

ps<Alternation> operator|(cps<GrammarElement> left, std::string right)
{
    ps<Alternation> ret = std::make_shared<Alternation>();
    ret->add(left);
    ret->add(text(right));
    return ret;
}

ps<Alternation> operator|(std::string left, cps<GrammarElement> right)
{
    ps<Alternation> ret = std::make_shared<Alternation>();
    ret->add(text(left));
    ret->add(right);
    return ret;
}

class Number : public GrammarElement
{
  public:
    virtual Result const *match(LexerState &ls) const override
    {
        LexerState save = ls;
        skip_space(ls);
        char const *start = ls.ptr;
        if (isdigit(*ls.ptr))
        {
            ls.ptr++;
            while (isdigit(*ls.ptr))
            {
                ls.ptr++;
            }
            std::cout << "matched number " << std::string_view(start, ls.ptr - start) << std::endl;
            return new TextResult(start, ls.ptr);
        }
        ls = save;
        ls.error("expected number");
        return nullptr;
    }
};

std::shared_ptr<GrammarElement const> number() { return std::shared_ptr<GrammarElement const>(new Number()); }

class Punctuation : public GrammarElement
{
  public:
    virtual Result const *match(LexerState &ls) const override
    {
        LexerState save = ls;
        skip_space(ls);
        char const *start = ls.ptr;
        if (ispunct(*ls.ptr))
        {
            ls.ptr++;
            std::cout << "matched punctuation " << std::string_view(start, ls.ptr - start) << std::endl;
            return new TextResult(start, ls.ptr);
        }
        ls = save;
        ls.error("expected punctuation");
        return nullptr;
    }
};

std::shared_ptr<GrammarElement const> punct() { return std::shared_ptr<GrammarElement const>(new Punctuation()); }

template <typename V>
std::string extractKeyFrom(V const *v);

template <typename V>
class ResultMap : public GrammarElement
{
    int maxlen;
    std::map<std::string, V const *> resmap;

  public:
    virtual V const *match(LexerState &ls) const override
    {
        LexerState save = ls;
        skip_space(ls);
        char const *start = ls.ptr;
        while (*ls.ptr && ls.ptr - start < maxlen)
        {
            ++ls.ptr;
        }
        while (ls.ptr > start)
        {
            auto f = resmap.find(std::string(start, ls.ptr - start));
            if (f != resmap.end())
            {
                std::cout << "matched resmap value " << f->first << std::endl;
                return f->second;
            }
            --ls.ptr;
        }
        ls = save;
        ls.error("expected value from resmap");
        return nullptr;
    }

    ResultMap *add(V const *v)
    {
        std::string k = extractKeyFrom(v);
        int len = k.length();
        if (len > maxlen)
        {
            maxlen = len;
        }
        resmap.insert({k, v});
        return this;
    }
};

template <typename V>
std::shared_ptr<ResultMap<V>> resmap() { return std::shared_ptr<ResultMap<V>>(new ResultMap<V>()); }

class Expr : public Result
{
    std::string op;
    std::vector<Result const *> args;

  public:
    Expr(std::string o) : op(o) {}
    Expr *add(Result const *e)
    {
        args.push_back(e);
        return this;
    }

    virtual void print(int indent) const override
    {
        std::cout << "expr " << op << ":" << std::endl;
        ++indent;
        for (auto arg : args)
        {
            std::cout << std::string(indent * 2, ' ');
            arg->print(indent);
        }
    }
};

template <typename C>
typename C::value_type pop(C &container)
{
    typename C::value_type ret = container.back();
    container.pop_back();
    return ret;
}

class Operator : public Result
{
  public:
    int prec;
    std::string name;

    Operator(int p, std::string n, int type) : prec(p), name(n) {}

    bool tighterThan(int other_prec) const
    {
        std::cout << "is " << name << "(" << prec << ") tighter than " << other_prec << std::endl;
        return prec <= other_prec;
    }

    virtual void print(int indent) const override
    {
        std::cout << "operator " << name << std::endl;
    }
};

std::string extractKeyFrom(Operator const *op)
{
    return op->name;
}

static Operator const *sentinel = new Operator(std::numeric_limits<int>::max(), "  sentinel  ", -1);

class ExpressionParser : public GrammarElement
{
    struct Data
    {
        std::vector<Result const *> exprs;
        std::vector<Operator const *> ops;

        void resolveStack(int prec)
        {
            std::cout << "resolveStack " << prec << std::endl;
            while (ops.back()->tighterThan(prec))
            {
                auto op = ::pop(ops);
                auto rhs = pop();
                auto lhs = pop();
                auto ex = new Expr(op->name);
                exprs.push_back(ex->add(lhs)->add(rhs));
                std::cout << "build expr " << op->name << "(" << lhs << "," << rhs << ")" << std::endl;
            }
        }

        void push(Result const *ex)
        {
            exprs.push_back(ex);
        }

        void push(Operator const *ex)
        {
            ops.push_back(ex);
        }

        Result const *pop()
        {
            return ::pop(exprs);
        }
    };

    cps<GrammarElement> primary;
    cps<ResultMap<Operator>> opmap;

  public:
    ExpressionParser(cps<GrammarElement> p, cps<ResultMap<Operator>> o) : primary(p), opmap(o) {}

    virtual Result const *match(LexerState &ls) const override
    {
        LexerState save = ls;

        Data data;
        // prec=0 is tightest

        data.ops.push_back(sentinel);

        while (true)
        {
            skip_space(ls);
            Result const *prim = primary->match(ls);
            if (!prim)
            {
                ls = save;
                ls.error("expected expression");
                return nullptr;
            }
            data.push(prim);

            Operator const *op = opmap->match(ls);
            if (!op)
            {
                data.resolveStack(std::numeric_limits<int>::max() - 1);
                return data.pop();
            }

            data.resolveStack(op->prec);

            data.push(op);
        }
    }

    virtual ~ExpressionParser()
    {
        delete sentinel;
    }
};

/*
function abc(x:int, y:int):int is
    x+y
end

*/

template <typename CONTEXT>
class treewalker : public visitor
{
  public:
    virtual void visit_top(MapResult const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_top(r, local, *(CONTEXT *)ctx))
        {
            r->visitChildren(*this, &local);
            post_top(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_top(MapResult const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_top(MapResult const *r, CONTEXT &ctx, CONTEXT &parent) {}
    virtual void visit_rule(MapResult const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_rule(r, local, *(CONTEXT *)ctx))
        {
            r->visitChildren(*this, &local);
            post_rule(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_rule(MapResult const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_rule(MapResult const *r, CONTEXT &ctx, CONTEXT &parent) {}
    virtual void visit_alt(MapResult const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_alt(r, local, *(CONTEXT *)ctx))
        {
            r->visitChildren(*this, &local);
            post_alt(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_alt(MapResult const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_alt(MapResult const *r, CONTEXT &ctx, CONTEXT &parent) {}
    virtual void visit_seq(MapResult const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_seq(r, local, *(CONTEXT *)ctx))
        {
            r->visitChildren(*this, &local);
            post_seq(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_seq(MapResult const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_seq(MapResult const *r, CONTEXT &ctx, CONTEXT &parent) {}
    virtual void visit_repe(MapResult const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_repe(r, local, *(CONTEXT *)ctx))
        {
            r->visitChildren(*this, &local);
            post_repe(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_repe(MapResult const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_repe(MapResult const *r, CONTEXT &ctx, CONTEXT &parent) {}
    virtual void visit_named(MapResult const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_named(r, local, *(CONTEXT *)ctx))
        {
            r->visitChildren(*this, &local);
            post_named(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_named(MapResult const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_named(MapResult const *r, CONTEXT &ctx, CONTEXT &parent) {}
    virtual void visit_element(MapResult const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_element(r, local, *(CONTEXT *)ctx))
        {
            r->visitChildren(*this, &local);
            post_element(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_element(MapResult const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_element(MapResult const *r, CONTEXT &ctx, CONTEXT &parent) {}
    virtual void visit_special(MapResult const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_special(r, local, *(CONTEXT *)ctx))
        {
            r->visitChildren(*this, &local);
            post_special(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_special(MapResult const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_special(MapResult const *r, CONTEXT &ctx, CONTEXT &parent) {}
    virtual void visit_ref(MapResult const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_ref(r, local, *(CONTEXT *)ctx))
        {
            r->visitChildren(*this, &local);
            post_ref(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_ref(MapResult const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_ref(MapResult const *r, CONTEXT &ctx, CONTEXT &parent) {}
    virtual void visit_capture(MapResult const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_capture(r, local, *(CONTEXT *)ctx))
        {
            r->visitChildren(*this, &local);
            post_capture(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_capture(MapResult const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_capture(MapResult const *r, CONTEXT &ctx, CONTEXT &parent) {}
    virtual void visit_noncapture(MapResult const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_noncapture(r, local, *(CONTEXT *)ctx))
        {
            r->visitChildren(*this, &local);
            post_noncapture(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_noncapture(MapResult const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_noncapture(MapResult const *r, CONTEXT &ctx, CONTEXT &parent) {}
    virtual void visit_txt(MapResult const *r, void *ctx) override
    {
        CONTEXT local;
        if (pre_txt(r, local, *(CONTEXT *)ctx))
        {
            r->visitChildren(*this, &local);
            post_txt(r, local, *(CONTEXT *)ctx);
        }
    }
    virtual bool pre_txt(MapResult const *r, CONTEXT &ctx, CONTEXT &parent)
    {
        return true;
    }
    virtual void post_txt(MapResult const *r, CONTEXT &ctx, CONTEXT &parent) {}
};

using namespace std;

class writeheader : public treewalker<void *>
{
    ofstream file;
    string cname;
    unordered_set<string> rule_seen;
    string dir;

  public:
    writeheader(string const &d) : dir(d) {}
    virtual bool pre_top(MapResult const *r, void *&, void *&) override
    {
        cname = r->get("name")->str();
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
    virtual void post_top(MapResult const *r, void *&, void *&) override
    {
        file << "};" << endl;
        file.close();
    }

    virtual void post_rule(MapResult const *r, void *&, void *&) override
    {
        string rname = r->get("name")->str();
        if (rule_seen.insert(rname).second)
            file << "    virtual Match const *" << rname << "(Cursor &c);" << endl;
    }
};

class writevisitor : public treewalker<void *>
{
    ofstream file;
    string cname;
    unordered_set<string> rule_seen;
    string dir;

  public:
    writevisitor(string const &d) : dir(d) {}
    virtual bool pre_top(MapResult const *r, void *&, void *&) override
    {
        cname = r->get("name")->str();
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

    virtual void post_top(MapResult const *r, void *&, void *&) override
    {
        file
            << "enum" << endl
            << "{" << endl
            << "    no_rule=0";
        for (auto n : rule_seen)
        {
            file << "," << endl
                 << "    rule_" << n;
        }
        file << endl
             << "};" << endl;

        file
            << "class Abstract" << cname << "Visitor : public VisitorBase" << endl
            << "{" << endl
            << "  public:" << endl
            << "    virtual void dispatch(RuleMatch const *m, void *ctx) override" << endl
            << "    {" << endl
            << "        switch (m->rule_index)" << endl
            << "        {" << endl;
        for (auto n : rule_seen)
        {
            file
                << "        case rule_" << n << ":" << endl
                << "            visit_" << n << "(m, ctx);" << endl
                << "            return;" << endl;
        }
        file
            << "        }" << endl
            << "    }" << endl;
        for (auto n : rule_seen)
        {
            file
                << "    virtual void visit_" << n << "(RuleMatch const *, void *ctx) = 0;" << endl;
        }
        file
            << "};" << endl
            << endl;

        file
            << "class " << cname << "Visitor : public Abstract" << cname << "Visitor" << endl
            << "{" << endl
            << "  public:" << endl;
        for (auto n : rule_seen)
        {
            file
                << "    virtual void visit_" << n << "(RuleMatch const *, void *ctx) override {}" << endl;
        }
        file
            << "    virtual void visitTextResult(TextMatch const *) override {}" << endl
            << "};" << endl
            << endl;

        file
            << "template <typename CONTEXT>" << endl
            << "class " << cname << "TreeWalker : public " << cname << "Visitor" << endl
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
                << "            r->visitChildren(*this, &local);" << endl
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
            << "class " << cname << "PrintWalker : public " << cname << "TreeWalker<void*>" << endl
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

    virtual void post_rule(MapResult const *r, void *&, void *&) override
    {
        string rname = r->get("name")->str();
        rule_seen.insert(rname);
    }
};

static string empty_string;

struct writeimpldata
{
    vector<string> childFuncNames;

    void addChildFunc(string const &name)
    {
        childFuncNames.push_back(name);
    }

    void addChildFunc(writeimpldata const &other)
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

class writeimpl : public treewalker<writeimpldata>
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
    writeimpl(string const &d) : dir(d) {}
    virtual bool pre_top(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        cname = r->get("name")->str();
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
    virtual void post_top(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        file.close();
    }

    virtual bool pre_rule(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        if (r->get("extern") != nullresult)
            return false;
        cur = new stringstream();
        have_name = false;
        return true;
    }

    virtual bool pre_named(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        if (r->get("name") != nullresult)
        {
            have_name = true;
        }
        return true;
    }

    virtual void post_named(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        have_name = false;
        string name = r->get("name")->str();
        if (name.empty())
        {
            parent.addChildFunc(local);
        }
        else
        {
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

    virtual void post_ref(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        string name = r->child()->str();
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

    virtual void post_special(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        parent.addChildFunc(startHelper("special"));
        file
            << "    return true;" << endl;
        endHelper();
    }

    virtual void post_element(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        parent.addChildFunc(local);
    }

    virtual void post_capture(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        parent.addChildFunc(local);
    }

    virtual void post_noncapture(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        parent.addChildFunc(local);
    }

    virtual void post_repe(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        string op = r->get("op")->str();
        if (op.empty())
        {
            parent.addChildFunc(local);
        }
        else
        {
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
    }

    virtual bool pre_seq(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        have_name = false;
        return true;
    }
    virtual void post_seq(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
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
    virtual void post_alt(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
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
    virtual void post_txt(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        string text = r->child()->str();
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
    virtual void post_rule(MapResult const *r, writeimpldata &local, writeimpldata &parent) override
    {
        string rname = r->get("name")->str();

        file << "Match const *" << cname << "::" << rname << "(Cursor &c)" << endl
             << "{" << endl
             << "    Match const *inner;" << endl
             << "    RuleMatch *m = nullptr;" << endl
             << "" << endl
             << "    if (" << local.childFuncName() << "(*this, inner, m, c))" << endl
             << "    {" << endl
             << "        m->rule_index = rule_" << rname << ";" << endl
             << "        return m;" << endl
             << "    }" << endl
             << "    return nullptr;" << endl
             << "}" << endl
             << endl;
        delete cur;
    }
};

#define RULE(name) Rule name(#name, &visitor::visit_##name)

int parse(string const &inputname, string const &dir)
{
    RULE(top);
    RULE(rule);
    RULE(alt);
    RULE(seq);
    RULE(repe);
    RULE(named);
    RULE(element);
    RULE(special);
    RULE(ref);
    RULE(capture);
    RULE(noncapture);
    RULE(txt);

    top = text("grammar") + id() / "name" + text(";") + rep(rule);
    rule = (text("extern") / "extern" + id() / "name" + text(";")) | (id() / "name" + text("::=") + alt + text(";"));
    alt = opt(text("|")) + seq + rep(text("|") + seq);
    seq = rep(repe);
    repe = named + opt((text("*") | text("+") | text("?")) / "op");
    named = opt(id() / "name" + text("=")) + element;
    element = special | ref | capture | noncapture | txt;
    special = text("^") | text("$");
    ref = id();
    capture = text("(") + alt + text(")");
    noncapture = text("[") + alt + text("]");
    txt = stringtoken();

    std::ifstream file(inputname);

    if (file.is_open())
    {
        std::stringstream ss;
        ss << file.rdbuf();
        std::string text = ss.str();
        std::cout << "read: " << text << std::endl;
        LexerState ls;
        ls.ptr = text.c_str();
        auto res = top.match(ls);
        if (res != nullptr)
        {
            res->print(0);
            std::cout << std::endl;
            {
                writeheader v(dir);
                res->visit(v);
            }
            {
                writeimpl v(dir);
                res->visit(v);
            }
            {
                writevisitor v(dir);
                res->visit(v);
            }
        }
        std::cout << "rest: " << ls.ptr << std::endl;
    }

    return 0;
}

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