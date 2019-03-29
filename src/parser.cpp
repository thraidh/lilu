#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

template <typename T>
using cps = std::shared_ptr<T const>;

template <typename T>
using ps = std::shared_ptr<T>;

class Result
{
  public:
    virtual void print(int indent) const = 0;
    virtual void addToMap(class MapResult const *map) const {};
    virtual ~Result() {}
};

class NullResult : public Result
{
  public:
    virtual void print(int indent) const override {}
};

class TextResult : public Result
{
    char const *b;
    char const *e;

  public:
    TextResult(char const *begin, char const *end) : b(begin), e(end) {}
    virtual void print(int indent) const override
    {
        std::cout << "Text[" << std::string_view(b, e - b) << "]" << std::endl;
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

  public:
    MapResult(std::string n, Result const *expr) : NamedResult(n, expr) {}
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

class RuleImpl : public GrammarElement
{
    std::string name;

  public:
    cps<GrammarElement> element;
    RuleImpl(std::string n) : name(n) {}
    virtual Result const *match(LexerState &ls) const override
    {
        auto ret = element->match(ls);
        if (ret)
        {
            std::cout << "store in rule " << name << std::endl;
            return new MapResult(name, ret);
        }
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

class Rule
{
    std::shared_ptr<RuleImpl> rule;

  public:
    Rule(std::string n) : rule(std::make_shared<RuleImpl>(n)) {}
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

class ExpressionParser : public GrammarElement
{
    Operator const *sentinel = new Operator(std::numeric_limits<int>::max(), "sentinel", -1);

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
                std::cout << "build expr " << op->name << "(" << lhs << "," << rhs << std::endl;
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

int parse()
{
    Rule lilufile("lilufile"), element("element"), funcdef("funcdef"), expr("expr"), args("args"), arg("arg"), primary("primary");

    ps<ResultMap<Operator>> rmap = resmap<Operator>();
    cps<ExpressionParser> expa = std::make_shared<ExpressionParser const>(primary, rmap);
    rmap->add(new Operator(2, "+", 0));

    lilufile = rep(element);
    element = funcdef | expr;
    expr = expa;
    args = opt(arg + rep("," + arg));
    arg = id() / "name" + text(":") + id() / "type";
    funcdef = "function" + id() / "name" + "(" + args + ")" + ":" + id() / "rtype" + "is" + element / "body" + "end";
    primary = id();

    std::ifstream file("first.lilu");

    if (file.is_open())
    {
        std::stringstream ss;
        ss << file.rdbuf();
        std::string text = ss.str();
        std::cout << "read: " << text << std::endl;
        LexerState ls;
        ls.ptr = text.c_str();
        auto res = lilufile.match(ls);
        if (res != nullptr)
        {
            res->print(0);
            std::cout << std::endl;
        }
        std::cout << "rest: " << ls.ptr << std::endl;
    }

    return 0;
}
