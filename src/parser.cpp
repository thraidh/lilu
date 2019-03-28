#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class Result
{
  public:
    virtual void print() const = 0;
    virtual ~Result() {}
};

class NullResult : public Result
{
  public:
    virtual void print() const override {}
};

class TextResult : public Result
{
    char const *b;
    char const *e;

  public:
    TextResult(char const *begin, char const *end) : b(begin), e(end) {}
    virtual void print() const override
    {
        std::cout << "Text[" << std::string_view(b, e - b) << "]";
    }
};

class ExprResult : public Result
{
    Result const *e;

  public:
    ExprResult(Result const *expr) : e(expr) {}
    virtual void print() const override
    {
        std::cout << "(";
        e->print();
        std::cout << ")";
    }
};

class MultiResult : public Result
{
    std::vector<Result const *> l;

  public:
    virtual void print() const override
    {
        std::cout << "[";
        auto i = l.begin();
        if (i != l.end())
        {
            (*i)->print();
            ++i;
            while (i != l.end())
            {
                std::cout << ", ";
                (*i)->print();
                ++i;
            }
        }
        std::cout << "]";
    }
    void add(Result const *r)
    {
        l.push_back(r);
    }
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
  public:
    std::shared_ptr<GrammarElement const> element;

    virtual Result const *match(LexerState &ls) const override
    {
        return element->match(ls);
    }
};

class Rule
{
    std::shared_ptr<RuleImpl> rule;

  public:
    Rule() : rule(std::make_shared<RuleImpl>()) {}
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

void alt(Alternation *a) {}

template <typename... Args>
void alt(Alternation *a, std::shared_ptr<GrammarElement const> ge, Args... args)
{
    a->add(ge);
    alt(a, args...);
}

template <typename... Args>
void alt(Alternation *a, Rule const &r, Args... args)
{
    a->add(r);
    alt(a, args...);
}

template <typename... Args>
std::shared_ptr<GrammarElement const> alt(Args... args)
{
    Alternation *ret = new Alternation();
    alt(ret, args...);
    return std::shared_ptr<GrammarElement const>(ret);
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

void seq(Sequence *a) {}

template <typename... Args>
void seq(Sequence *a, std::shared_ptr<GrammarElement const> ge, Args... args)
{
    a->add(ge);
    seq(a, args...);
}

template <typename... Args>
void seq(Sequence *a, Rule const &r, Args... args)
{
    a->add(r);
    seq(a, args...);
}

template <typename... Args>
std::shared_ptr<GrammarElement const> seq(Args... args)
{
    Sequence *ret = new Sequence();
    seq(ret, args...);
    return std::shared_ptr<GrammarElement const>(ret);
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

/*
function abc(x:int, y:int):int is
    x+y
end

*/

int parse()
{
    Rule lilufile, element, funcdef, expr, args, arg;

    lilufile = rep(element);
    element = alt(funcdef, expr);
    expr = seq(id(), punct(), id());
    args = opt(seq(arg, rep(seq(text(","), arg))));
    arg = seq(id(), text(":"), id());
    funcdef = seq(text("function"), id(), text("("), args, text(")"), text(":"), id(), text("is"), element, text("end"));

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
            res->print();
            std::cout << std::endl;
        }
        std::cout << "rest: " << ls.ptr << std::endl;
    }

    return 0;
}
