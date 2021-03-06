#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

class Cursor
{
    char const *ptr;

  public:
    Cursor(char const *p) : ptr(p) {}

    bool atEof() const
    {
        return *ptr == 0;
    }

    void skipSpace()
    {
        while (isspace(*ptr))
        {
            ++ptr;
        }
    }

    char la(int ahead = 0) const
    {
        return ptr[ahead];
    }

    void consume()
    {
        ++ptr;
    }

    void unget()
    {
        --ptr;
    }

    string_view stringStartingAt(Cursor const &start) const
    {
        auto sp = start.ptr;
        return string_view(sp, ptr - sp);
    }

    int lengthFrom(Cursor const &start) const
    {
        return ptr - start.ptr;
    }

    void error(string const &text) const
    {
        char const *end = ptr;
        while (*end != '\n' && *end != 0)
            ++end;
        cout << text << " at [" << string_view(ptr, end - ptr) << "]" << endl;
    }

    char const *current() const
    {
        return ptr;
    }

    operator char const *() const
    {
        return ptr;
    }

    bool operator<(Cursor const &other) const
    {
        return ptr < other.ptr;
    }
    bool operator>(Cursor const &other) const
    {
        return ptr > other.ptr;
    }
};

class Match
{
  public:
    int rule_index;
    virtual ~Match() {}
    virtual void print(int indent) const = 0;
    virtual string_view sv() const { return string_view(); }
};

class TextMatch : public Match
{
    Cursor begin, end;

  public:
    TextMatch(Cursor b, Cursor e) : begin(b), end(e) { rule_index = 1; }
    virtual string_view sv() const override { return end.stringStartingAt(begin); }
    virtual void print(int indent) const override
    {
        cout << "'" << sv() << "'" << endl;
    }
};

class RuleMatch : public Match
{
  public:
    multimap<string, Match const *> named;
    vector<Match const *> positional;
    virtual ~RuleMatch() override
    {
        named.clear();
        for (auto m : positional)
        {
            delete m;
        }
    }
    virtual void print(int indent) const override
    {
        cout << "rule" << endl;
        ++indent;
        if (named.empty())
        {
            for (auto e : positional)
            {
                cout << string(indent * 2, ' ');
                e->print(indent);
            }
        }
        else
        {
            for (auto kv : named)
            {
                cout << string(indent * 2, ' ') << kv.first << "=";
                kv.second->print(indent);
            }
        }
    }
    RuleMatch *add(Match const *inner)
    {
        positional.push_back(inner);
        return this;
    }
    Match const *get(string const &key) const
    {
        auto it = named.find(key);
        if (it == named.end())
            return nullptr;
        return it->second;
    }
    vector<Match const *> range(string const &key) const
    {
        vector<Match const *> v;
        auto it = named.find(key);
        while (it != named.end())
        {
            v.push_back(it->second);
            ++it;
        }
        return v;
    }
    Match const *child(int n = 0) const
    {
        if (n >= positional.size())
            return nullptr;
        return positional[n];
    }
};

inline RuleMatch *addToMatch(RuleMatch *outer, char const *name, Match const *inner)
{
    if (!outer)
    {
        outer = new RuleMatch();
    }

    if (inner)
    {
        if (name != nullptr)
        {
            outer->named.insert({name, inner});
        }

        outer->positional.push_back(inner);
    }

    return outer;
}

inline RuleMatch *mergeMatch(RuleMatch *outer, RuleMatch *&submatch)
{
    if (!outer)
    {
        outer = new RuleMatch();
    }

    outer->named.insert(submatch->named.begin(), submatch->named.end());
    outer->positional.insert(outer->positional.end(), submatch->positional.begin(), submatch->positional.end());

    submatch->positional.clear();
    submatch->named.clear();

    delete submatch;
    submatch = nullptr;

    return outer;
}

class DefaultGrammar
{
  public:
    vector<string> keywords;
    TextMatch const *ID(Cursor &c);
    TextMatch const *NUMBER(Cursor &c);
    TextMatch const *STRING(Cursor &c);
    TextMatch const *TEXT(Cursor &c, string text);
};

inline TextMatch const *DefaultGrammar::TEXT(Cursor &c, string text)
{
    Cursor save = c;
    char const *tptr = text.c_str();
    while (*tptr && c.la(0) == *tptr)
    {
        c.consume();
        ++tptr;
    }
    if (*tptr == 0)
    {
        cout << "matched text " << c.stringStartingAt(save) << endl;
        return new TextMatch(save, c);
    }
    c = save;
    c.error("expected text " + text);
    return nullptr;
}

inline TextMatch const *DefaultGrammar::ID(Cursor &c)
{
    Cursor save = c;
    if (isalpha(c.la()))
    {
        c.consume();
        while (isalnum(c.la()))
        {
            c.consume();
        }
        if (find(keywords.begin(), keywords.end(), c.stringStartingAt(save)) != keywords.end())
        {
            c = save;
            c.error("expected id but found keyword");
            return nullptr;
        }
        cout << "matched id " << c.stringStartingAt(save) << endl;
        return new TextMatch(save, c);
    }
    c = save;
    c.error("expected id");
    return nullptr;
}

inline TextMatch const *DefaultGrammar::NUMBER(Cursor &c)
{
    Cursor save = c;
    if (isdigit(c.la()))
    {
        c.consume();
        while (isdigit(c.la()))
        {
            c.consume();
        }
        cout << "matched number " << c.stringStartingAt(save) << endl;
        return new TextMatch(save, c);
    }
    c = save;
    c.error("expected number");
    return nullptr;
}

inline TextMatch const *DefaultGrammar::STRING(Cursor &c)
{
    Cursor save = c;
    if (c.la() == '"')
    {
        c.consume();
        while (c.la() != '"')
        {
            if (c.la() == 0)
            {
                c = save;
                c.error("unterminated string");
                return nullptr;
            }
            c.consume();
        }
        c.consume();
        cout << "matched string " << c.stringStartingAt(save) << endl;
        return new TextMatch(save, c);
    }
    c = save;
    c.error("expected string");
    return nullptr;
}

template <typename V>
inline V default_value() { return {}; }

template <>
inline void default_value<void>() {}

class Operator;