#pragma once

#include "parser_base.h"

template <typename V>
string getKeyFrom(V const *v);

template <typename V>
class ResultMap
{
    int maxlen;
    map<string, V const *> resmap;

  public:
    ResultMap &add(V const *v)
    {
        string k = getKeyFrom(v);
        int len = k.length();
        if (len > maxlen)
        {
            maxlen = len;
        }
        resmap.insert({k, v});
        return *this;
    }

    auto find(string const &key) const
    {
        return resmap.find(key);
    }

    auto end() const
    {
        return resmap.end();
    }

    int maxLength() const
    {
        return maxlen;
    }
};

template <typename V>
V const *resmapper(Cursor &c, ResultMap<V> const &resmap)
{
    Cursor save = c;
    while (!c.atEof() && c.lengthFrom(save) < resmap.maxLength())
    {
        c.consume();
    }
    cout << "check for '" << c.stringStartingAt(save) << "' in resmap" << endl;
    while (c > save)
    {
        auto f = resmap.find(string(c.stringStartingAt(save)));
        if (f != resmap.end())
        {
            cout << "matched resmap value " << f->first << endl;
            return new V(*(f->second));
        }
        c.unget();
    }
    c = save;
    c.error("expected value from resmap");
    return nullptr;
}

template <typename C>
typename C::value_type pop(C &container)
{
    typename C::value_type ret = container.back();
    container.pop_back();
    return ret;
}

class Operator : public Match
{
  public:
    int prec;
    string name;

    Operator(int p, string n, int type) : prec(p), name(n)
    {
        rule_index = 3;
    }

    bool tighterThan(int other_prec) const
    {
        cout << "is " << name << "(" << prec << ") tighter than " << other_prec << endl;
        return prec <= other_prec;
    }

    virtual void print(int indent) const override
    {
        cout << "operator " << name << endl;
    }
};

inline string getKeyFrom(Operator const *op)
{
    return op->name;
}

static Operator const *sentinel = new Operator(numeric_limits<int>::max(), "  sentinel  ", -1);

template <typename GRAMMAR, typename PRIMARY>
Match const *expr_parser(Cursor &c, GRAMMAR *g, PRIMARY primary, ResultMap<Operator> const &resmap)
{
    struct Data
    {
        vector<Match const *> exprs;
        vector<Operator const *> ops;

        void resolveStack(int prec)
        {
            cout << "resolveStack " << prec << endl;
            while (ops.back()->tighterThan(prec))
            {
                auto op = ::pop(ops);
                auto rhs = pop();
                auto lhs = pop();
                auto ex = new RuleMatch();
                ex->rule_index = 2;
                exprs.push_back(ex->add(op)->add(lhs)->add(rhs));
                cout << "build expr " << op->name << "(" << lhs << "," << rhs << ")" << endl;
            }
        }

        void push(Match const *ex)
        {
            exprs.push_back(ex);
        }

        void push(Operator const *ex)
        {
            ops.push_back(ex);
        }

        Match const *pop()
        {
            return ::pop(exprs);
        }

        ~Data()
        {
            for (auto e : exprs)
            {
                delete e;
            }
        }
    };

    Cursor save = c;

    Data data;
    // prec=0 is tightest

    data.ops.push_back(sentinel);

    while (true)
    {
        Match const *prim = primary(g, c);
        if (!prim)
        {
            c = save;
            c.error("expected expression");
            return nullptr;
        }
        data.push(prim);

        Operator const *op = resmapper(c, resmap);
        if (!op)
        {
            data.resolveStack(numeric_limits<int>::max() - 1);
            return data.pop();
        }

        data.resolveStack(op->prec);

        data.push(op);
    }
}
