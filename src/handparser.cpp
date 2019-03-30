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

template <typename T>
using scp = shared_ptr<T const>;

template <typename T>
using sp = shared_ptr<T>;

/*
lilufile = rep(element);
element = funcdef | expr;
expr = expa;
args = opt(arg + rep("," + arg));
arg = id() / "name" + text(":") + id() / "type";
funcdef = "function" + id() / "name" + "(" + args + ")" + ":" + id() / "rtype" + "is" + element / "body" + "end";
auto parenthesized = "(" + expr + ")";
primary = id() | number() | parenthesized;
*/

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
    virtual ~Match() {}
    virtual void print(int indent) const = 0;
};

class TextMatch : public Match
{
    Cursor begin, end;

  public:
    TextMatch(Cursor b, Cursor e) : begin(b), end(e) {}
    virtual void print(int indent) const override
    {
        cout << "'" << end.stringStartingAt(begin) << "'" << endl;
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
};

RuleMatch *addToMatch(RuleMatch *outer, char const *name, Match const *inner)
{
    if (!outer)
    {
        outer = new RuleMatch();
    }

    if (name != nullptr)
    {
        outer->named.insert({name, inner});
    }

    outer->positional.push_back(inner);

    return outer;
}

RuleMatch const *lilufile(Cursor &c);
RuleMatch const *element(Cursor &c);
Match const *expr(Cursor &c);
RuleMatch const *args(Cursor &c);
RuleMatch const *arg(Cursor &c);
RuleMatch const *funcdef(Cursor &c);
RuleMatch const *primary(Cursor &c);

RuleMatch const *lilufile(Cursor &c)
{
    c.skipSpace();
    RuleMatch *m = new RuleMatch();
    while (auto ret = element(c))
    {
        addToMatch(m, "element", ret);
        c.skipSpace();
    }
    if (c.atEof())
    {
        return m;
    }
    else
    {
        delete m;
        return nullptr;
    }
}

RuleMatch const *element(Cursor &c)
{
    Match const *inner;
    inner = funcdef(c);
    if (inner)
    {
        return addToMatch(nullptr, "funcdef", inner);
    }
    inner = expr(c);
    if (inner)
    {
        return addToMatch(nullptr, "expr", inner);
    }
    return nullptr;
}

TextMatch const *text(Cursor &c, string text);
TextMatch const *id(Cursor &c);
TextMatch const *number(Cursor &c);

RuleMatch const *args(Cursor &c)
{
    Match const *inner;
    inner = arg(c);
    if (!inner)
    {
        return new RuleMatch();
    }
    RuleMatch *m = addToMatch(nullptr, "arg", inner);
    c.skipSpace();
    while (true)
    {
        Cursor save = c;
        inner = text(c, ",");
        if (!inner)
        {
            // can fail at this point
            break;
        }
        // don't care for the literals
        delete inner;

        c.skipSpace();

        inner = arg(c);
        if (!inner)
        {
            // fail repetition, so return  cursor to save point
            c = save;
            break;
        }
        addToMatch(m, "arg", inner);

        c.skipSpace();
    }
    return m;
}

RuleMatch const *arg(Cursor &c)
{
    Match const *inner;
    Cursor save = c;
    inner = id(c);
    if (!inner)
    {
        return nullptr;
    }
    RuleMatch *m = addToMatch(nullptr, "name", inner);

    c.skipSpace();

    inner = text(c, ":");
    if (!inner)
    {
        c = save;
        delete m;
        return nullptr;
    }
    delete inner;

    c.skipSpace();

    inner = id(c);
    if (!inner)
    {
        c = save;
        delete m;
        return nullptr;
    }
    addToMatch(m, "type", inner);

    c.skipSpace();
    return m;
}

RuleMatch const *funcdef(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    Cursor save = c;

    inner = text(c, "function");
    if (!inner)
    {
        return nullptr;
    }
    delete inner;

    c.skipSpace();

    inner = id(c);
    if (!inner)
    {
        c = save;
        delete m;
        return nullptr;
    }
    m = addToMatch(m, "name", inner);

    c.skipSpace();

    inner = text(c, "(");
    if (!inner)
    {
        c = save;
        delete m;
        return nullptr;
    }
    delete inner;

    c.skipSpace();

    inner = args(c);
    if (!inner)
    {
        c = save;
        delete m;
        return nullptr;
    }
    addToMatch(m, "args", inner);

    c.skipSpace();

    inner = text(c, ")");
    if (!inner)
    {
        c = save;
        delete m;
        return nullptr;
    }
    delete inner;

    c.skipSpace();

    inner = text(c, ":");
    if (!inner)
    {
        c = save;
        delete m;
        return nullptr;
    }
    delete inner;

    c.skipSpace();

    inner = id(c);
    if (!inner)
    {
        c = save;
        delete m;
        return nullptr;
    }
    addToMatch(m, "rtype", inner);

    c.skipSpace();

    inner = text(c, "is");
    if (!inner)
    {
        c = save;
        delete m;
        return nullptr;
    }
    delete inner;

    c.skipSpace();

    inner = element(c);
    if (!inner)
    {
        c = save;
        delete m;
        return nullptr;
    }
    addToMatch(m, "body", inner);

    c.skipSpace();

    inner = text(c, "end");
    if (!inner)
    {
        c = save;
        delete m;
        return nullptr;
    }
    delete inner;

    c.skipSpace();

    return m;
}

// fragment
inline bool parenthesized(Cursor &c, RuleMatch *&m)
{
    Match const *inner;

    Cursor save = c;

    inner = text(c, "(");
    if (!inner)
    {
        return false;
    }
    delete inner;

    c.skipSpace();

    inner = expr(c);
    if (!inner)
    {
        c = save;
        return false;
    }
    m = addToMatch(m, "expr", inner);

    c.skipSpace();

    inner = text(c, ")");
    if (!inner)
    {
        c = save;
        return false;
    }
    delete inner;

    c.skipSpace();

    return true;
}

RuleMatch const *primary(Cursor &c)
{
    Match const *inner;
    RuleMatch *m = nullptr;

    inner = id(c);
    if (inner)
    {
        c.skipSpace();

        m = addToMatch(m, "id", inner);
        return m;
    }

    inner = number(c);
    if (inner)
    {
        c.skipSpace();

        m = addToMatch(m, "id", inner);
        return m;
    }

    if (parenthesized(c, m))
    {
        c.skipSpace();
        return m;
    }
    delete m;

    return nullptr;
}

/*
lilufile = rep(element);
element = funcdef | expr;
expr = expa;
args = opt(arg + rep("," + arg));
arg = id() / "name" + text(":") + id() / "type";
funcdef = "function" + id() / "name" + "(" + args + ")" + ":" + id() / "rtype" + "is" + element / "body" + "end";
auto parenthesized = "(" + expr + ")";
primary = id() | number() | parenthesized;
*/

TextMatch const *text(Cursor &c, string text)
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

TextMatch const *id(Cursor &c)
{
    Cursor save = c;
    if (isalpha(c.la()))
    {
        c.consume();
        while (isalnum(c.la()))
        {
            c.consume();
        }
        cout << "matched id " << c.stringStartingAt(save) << endl;
        return new TextMatch(save, c);
    }
    c = save;
    c.error("expected id");
    return nullptr;
}

TextMatch const *number(Cursor &c)
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

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

template <typename V>
string getKeyFrom(V const *v);

template <typename V>
class ResultMap
{
    int maxlen;
    map<string, V const *> resmap;

  public:
    ResultMap *add(V const *v)
    {
        string k = getKeyFrom(v);
        int len = k.length();
        if (len > maxlen)
        {
            maxlen = len;
        }
        resmap.insert({k, v});
        return this;
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
V const *mapped(Cursor &c, ResultMap<V> const &resmap)
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

    Operator(int p, string n, int type) : prec(p), name(n) {}

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

string getKeyFrom(Operator const *op)
{
    return op->name;
}

static Operator const *sentinel = new Operator(numeric_limits<int>::max(), "  sentinel  ", -1);

template <typename PRIMARY, typename OPMAP>
Match const *expr_(Cursor &c, PRIMARY primary, OPMAP opmap)
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
        Match const *prim = primary(c);
        if (!prim)
        {
            c = save;
            c.error("expected expression");
            return nullptr;
        }
        data.push(prim);

        Operator const *op = opmap(c);
        if (!op)
        {
            data.resolveStack(numeric_limits<int>::max() - 1);
            return data.pop();
        }

        data.resolveStack(op->prec);

        data.push(op);
    }
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

ResultMap<Operator> resmap;

Match const *expr(Cursor &c)
{
    return expr_(c, primary, [&](Cursor &c) { return mapped(c, resmap); });
}

int handparse()
{
    resmap.add(new Operator(2, "+", 0));
    resmap.add(new Operator(2, "-", 0));
    resmap.add(new Operator(1, "*", 0));
    resmap.add(new Operator(1, "/", 0));

    ifstream file("first.lilu");

    if (file.is_open())
    {
        stringstream ss;
        ss << file.rdbuf();
        string text = ss.str();
        cout << "read: " << text << endl;
        Cursor c(text.c_str());
        auto res = lilufile(c);
        if (res != nullptr)
        {
            res->print(0);
            cout << endl;
        }
        cout << "rest: [[[" << c << "]]]" << endl;
    }

    return 0;
}
