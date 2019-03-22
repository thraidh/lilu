#include <fstream>
#include <string>
#include <iostream>
#include <cstdarg>
#include <vector>
#include <sstream>

class GrammarElement
{
  public:
    virtual char const *match(char const *ptr) const = 0;
    virtual ~GrammarElement() {}
};

class RuleImpl : public GrammarElement
{
    int refcnt = 0;

  public:
    GrammarElement const *element;

    int ref() { return ++refcnt; }
    int deref()
    {
        int nrc = --refcnt;
        if (nrc)
        {
            delete this;
        }
        return nrc;
    }
    virtual char const *match(char const *ptr) const override
    {
        return element->match(ptr);
    }
};

class Rule
{
    RuleImpl *rule;

  public:
    Rule();
    Rule(Rule &other)
    {
        rule = other.rule;
        rule->ref();
    }
    operator RuleImpl *()
    {
        rule->ref();
        return rule;
    }
    Rule &operator=(GrammarElement const *ge)
    {
        rule->element = ge;
        return *this;
    }
    ~Rule()
    {
        rule->deref();
    }
    char const *match(char const *ptr) const
    {
        return rule->match(ptr);
    }
};

Rule::Rule() : rule(new RuleImpl()) {}

Rule lilufile, statement;

class Repetition : public GrammarElement
{
    GrammarElement const *element;

  public:
    Repetition(GrammarElement const *ge) : element(ge) {}
    virtual char const *match(char const *ptr) const override
    {
        while (true)
        {
            char const *ret = element->match(ptr);
            if (!ret)
                return ptr;
            ptr = ret;
        }
    }
};

Repetition const *rep(GrammarElement const *ge)
{
    return new Repetition(ge);
}

class Alternation : public GrammarElement
{
    std::vector<GrammarElement const *> elements;

  public:
    Alternation()
    {
    }

    void add(GrammarElement const *ge)
    {
        elements.push_back(ge);
    }

    virtual char const *match(char const *ptr) const override
    {
        for (auto ge : elements)
        {
            auto ret = ge->match(ptr);
            if (ret)
                return ret;
        }
        return nullptr;
    }
};

void alt(Alternation *a)
{
}

template <typename... Args>
void alt(Alternation *a, GrammarElement const *ge, Args... args)
{
    a->add(ge);
    alt(a, args...);
}

template <typename... Args>
Alternation const *alt(Args... args)
{
    Alternation *ret = new Alternation();
    alt(ret, args...);
    return ret;
}

class Id : public GrammarElement
{
  public:
    virtual char const *match(char const *ptr) const override
    {
        while (isblank(*ptr))
            ptr++;
        if (isalpha(*ptr))
        {
            ptr++;
            while (isalnum(*ptr))
                ptr++;
            std::cout << "matched id\n";
            return ptr;
        }
        return nullptr;
    }
};

GrammarElement const *id()
{
    return new Id();
}

class Number : public GrammarElement
{
  public:
    virtual char const *match(char const *ptr) const override
    {
        while (isblank(*ptr))
            ptr++;
        if (isdigit(*ptr))
        {
            ptr++;
            while (isdigit(*ptr))
                ptr++;
            std::cout << "matched number\n";
            return ptr;
        }
        return nullptr;
    }
};

GrammarElement const *number()
{
    return new Number();
}

class Punctuation : public GrammarElement
{
  public:
    virtual char const *match(char const *ptr) const override
    {
        while (isblank(*ptr))
            ptr++;
        if (ispunct(*ptr))
        {
            ptr++;
            std::cout << "matched punctuation\n";
            return ptr;
        }
        return nullptr;
    }
};

GrammarElement const *punct()
{
    return new Punctuation();
}



int parse()
{
    lilufile = rep(statement);
    statement = alt(id(), number(), punct());

    std::ifstream file("first.lilu");

    if (file.is_open())
    {
        std::stringstream ss;
        ss << file.rdbuf();
        std::string text = ss.str();
        std::cout << "read: " << text << std::endl;
        lilufile.match(text.c_str());
    }

    return 0;
}