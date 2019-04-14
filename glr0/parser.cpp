#include "death_handler.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

using namespace std;

struct Element {
    string_view name;
    bool isTerminal;
    int index;

    Element(bool t) : name(), isTerminal(t) {}
};

struct Rule {
    string_view name;
    vector<Element *> elements;
    int index;

    Rule() : name(), elements() {}
    void print() {
        cout << name << " =";
        for (auto e : elements) {
            cout << " " << e->name;
        }
        cout << ";" << endl;
    }
};

struct Terminal : public Element {
    Terminal() : Element(true) {}

    void print() { cout << name << endl; }
};

struct Symbol : public Element {
    vector<Rule *> rules;
    bool hasEmptyRule;
    vector<Terminal *> first;

    Symbol() : Element(false), rules() {}
    void print() {
        for (auto p : rules)
            p->print();
    }
    void add(Rule *r) { rules.push_back(r); }
    void calcFirst(unordered_set<Symbol *> &seen, vector<Terminal *> &first) {
        if (seen.insert(this).second) {
            for (auto r : rules) {
                auto it = r->elements.begin();
                auto end = r->elements.end();
                while (it != end) {
                    auto e = *it;
                    if (e->isTerminal) {
                        first.push_back(static_cast<Terminal *>(e));
                        break;
                    } else {
                        auto s = static_cast<Symbol *>(e);
                        s->calcFirst(seen, first);
                        if (s->hasEmptyRule) {
                            ++it;
                        } else {
                            break;
                        }
                    }
                }
                if (it == end)
                    hasEmptyRule = true;
            }
        }
    }
    void calcFirst() {
        unordered_set<Symbol *> seen;
        calcFirst(seen, first);
    }
};

map<string, Symbol *> symbols;
map<string, Terminal *> terminals;
vector<Rule *> rules;

char const *skipSpace(char const *ptr) {
    while (isspace(*ptr))
        ++ptr;
    return ptr;
}

char const *ID(char const *ptr) {
    if (isalpha(*ptr)) {
        ++ptr;
        while (isalnum(*ptr))
            ++ptr;
        return ptr;
    }
    return nullptr;
}

char const *literal(char const *ptr, char const *lit) {
    while (*ptr == *lit) {
        if (*lit == 0)
            return ptr;
        ++lit;
        ++ptr;
    }
    if (*lit == 0)
        return ptr;
    return nullptr;
}

char const *STRING(char const *ptr) {
    if (*ptr == '\'')
        ++ptr;
    else
        return nullptr;

    while (*ptr != '\'') {
        if (!*ptr)
            return nullptr;
        ++ptr;
    }
    ++ptr;

    return ptr;
}

inline auto symbol(string const &str) {
    auto &ret = symbols[str];
    if (!ret) {
        ret = new Symbol();
        ret->name = str;
        ret->index = -symbols.size();
    }
    return ret;
}

inline auto symbol(char const *str) {
    auto &ret = symbols[str];
    if (!ret) {
        ret = new Symbol();
        ret->name = str;
        ret->index = -symbols.size();
    }
    return ret;
}

inline auto symbol(string_view const &str) {
    auto &ret = symbols[string(str)];
    if (!ret) {
        ret = new Symbol();
        ret->name = str;
        ret->index = -symbols.size();
    }
    return ret;
}

inline auto terminal(char const *str) {
    auto &ret = terminals[str];
    if (!ret) {
        ret = new Terminal();
        ret->name = str;
        ret->index = terminals.size();
    }
    return ret;
}

inline auto terminal(string const &str) {
    auto &ret = terminals[str];
    if (!ret) {
        ret = new Terminal();
        ret->name = str;
        ret->index = terminals.size();
    }
    return ret;
}

inline auto terminal(string_view const &str) {
    auto &ret = terminals[string(str)];
    if (!ret) {
        ret = new Terminal();
        ret->name = str;
        ret->index = terminals.size();
    }
    return ret;
}

char const *readGrammar(char const *ptr) {
    ptr = skipSpace(ptr);
    while (*ptr) {
        auto m = ID(ptr);
        auto rule = new Rule();
        if (m) {
            rule->name = string_view(ptr, m - ptr);
        } else {
            delete rule;
            return ptr;
        }

        ptr = skipSpace(m);
        m = literal(ptr, "=");
        if (!m) {
            delete rule;
            return ptr;
        }

        while (true) {
            ptr = skipSpace(m);

            m = ID(ptr);
            if (m) {
                string_view sv = string_view(ptr, m - ptr);
                if (isupper(sv[0])) {
                    Terminal *term = terminal(sv);
                    rule->elements.push_back(term);
                } else {
                    Symbol *sym = symbol(sv);
                    rule->elements.push_back(sym);
                }
                ptr = m;
                continue;
            }

            m = STRING(ptr);
            if (m) {
                string_view sv = string_view(ptr, m - ptr);
                Terminal *term = terminal(sv);
                rule->elements.push_back(term);
                ptr = m;
                continue;
            }

            m = literal(ptr, ";");
            if (m) {
                ptr = m;
                break;
            }

            delete rule;
            return ptr;
        }

        Symbol *sym = symbol(rule->name);
        sym->name = rule->name;
        sym->add(rule);
        rule->index = rules.size();
        rules.push_back(rule);
        ptr = skipSpace(ptr);
    }
    return ptr;
}

struct State;

struct StateElement {
    Rule *r;
    int pos;
    Terminal *lookahead;
    bool kernel;

    void print() {
        cout << "[ " << r->name << " -> ";
        int len = r->elements.size();
        int i;
        for (i = 0; i < len; i++) {
            if (i == pos)
                cout << ". ";
            cout << r->elements[i]->name << " ";
        }
        if (i == pos)
            cout << ". ";
        cout << "| " << lookahead->name << " ]";
        if (kernel)
            cout << "*";
        cout << endl;
    }

    void buildClosure(State *state);

    bool operator==(StateElement const &other) {
        return r == other.r && pos == other.pos && lookahead == other.lookahead;
    }
};

template <typename V, typename T> bool contains(V const &range, T const *el) {
    auto end = range.end();
    for (auto it = range.begin(); it != end; ++it) {
        if (*(*it) == *el)
            return true;
    }
    return false;
}

enum ParseTreeNodeType {
    parseTreeNodeType,
    parseTreeInnerNodeType,
    parseTreeLeafNodeType,
};
struct ParseTreeNode {
    ParseTreeNodeType type;
    ParseTreeNode(ParseTreeNodeType t) : type(t) {}
    virtual ~ParseTreeNode() {}
    virtual void print(int indent) = 0;
};
struct ParseTreeInnerNode : public ParseTreeNode {
    Rule *r;
    vector<ParseTreeNode *> children;
    ParseTreeInnerNode() : ParseTreeNode(parseTreeInnerNodeType) {}
    void print(int indent) override {
        cout << string(indent * 2, ' ');
        r->print();
        for (auto c : children)
            c->print(indent + 1);
    }
};
struct ParseTreeLeafNode : public ParseTreeNode {
    ParseTreeLeafNode() : ParseTreeNode(parseTreeLeafNodeType) {}
    void print(int indent) override {
        cout << string(indent * 2, ' ') << "leaf" << endl;
    }
};

struct State;

State *newState();

struct State {
    vector<StateElement *> elements;
    map<Element *, State *> trans;
    int index;
    map<Element *, variant<bool, State *, Rule *>> parseTable;

    void add(Rule *r, Terminal *lookahead, int pos, bool kernel = false) {
        auto e = new StateElement();
        e->r = r;
        e->lookahead = lookahead;
        e->pos = pos;
        e->kernel = kernel;
        if (!contains(elements, e)) {
            elements.push_back(e);
            if (pos < r->elements.size()) {
                auto e = r->elements[pos];
                State *&nextState = trans[e];
                if (!nextState) {
                    nextState = newState();
                    parseTable[e] = nextState;
                }
                nextState->add(r, lookahead, pos + 1, true);
            } else {
                parseTable[lookahead] = r;
            }
        }
    }

    void buildClosure() {
        for (int i = 0; i < elements.size(); i++) {
            elements[i]->buildClosure(this);
        }
        for (auto [e, s] : trans) {
            s->buildClosure();
        }
    }

    void print() {
        for (auto e : elements) {
            e->print();
        }
    }

    template <typename Iter>
    void parse(Iter &iter, const Iter &end, vector<State *> &pstate,
               vector<ParseTreeNode *> &nodes) {
        cout << "in state" << endl;
        print();
        Terminal *t = iter == end ? terminal("$") : *iter;
        cout << "looking at ";
        t->print();
        auto &v = parseTable[t];
        if (auto statePtr = get_if<State *>(&v)) {
            auto state = *statePtr;
            pstate.push_back(state);
            ++iter;
            cout << "go to state" << endl;
            state->print();
            ParseTreeLeafNode *node = new ParseTreeLeafNode();
            nodes.push_back(node);
        } else if (auto rulePtr = get_if<Rule *>(&v)) {
            vector<ParseTreeNode *> values;
            auto r = *rulePtr;
            cout << "reduce rule ";
            r->print();
            int len = r->elements.size();
            ParseTreeInnerNode *node = new ParseTreeInnerNode();
            node->r = r;
            node->children.resize(len);
            for (int i = len; i--;) {
                node->children[i] = nodes.back();
                nodes.pop_back();
            }
            nodes.push_back(node);
            auto s = symbol(r->name);
            while (!pstate.empty()) {
                auto state = pstate.back();
                auto &v = state->parseTable[s];
                if (auto statePtr = get_if<State *>(&v)) {
                    pstate.push_back(*statePtr);
                    break;
                }
                pstate.pop_back();
            }
        } else {
            cout << "no transition" << endl;
        }
        cout << "======" << endl;
    }
};

inline void StateElement::buildClosure(State *state) {
    int len = r->elements.size();
    int p = pos;
    vector<Terminal *> follow;
    Symbol *first = nullptr;
    while (p < len) {
        auto e = r->elements[p];
        if (e->isTerminal) {
            follow.push_back(static_cast<Terminal *>(e));
            break;
        } else {
            Symbol *s = static_cast<Symbol *>(e);
            if (!first) {
                first = s;
                // if (s->hasEmptyRule)
                //     state->add(r, lookahead, p, false);
            } else {
                follow.insert(follow.end(), s->first.begin(), s->first.end());
                if (!s->hasEmptyRule) {
                    break;
                }
                // else
                //     state->add(r, lookahead, p, false);
            }
            ++p;
        }
    }
    if (p == len)
        follow.push_back(lookahead);
    if (first) {
        for (auto t : follow) {
            for (auto add : symbol(first->name)->rules) {
                state->add(add, t, 0, false);
            }
        }
    }
}

vector<State *> states;
State *newState() {
    auto ret = new State();
    ret->index = states.size();
    states.push_back(ret);
    return ret;
}

void buildStates() {
    State *s = newState();
    Terminal *t = terminal("$");
    for (auto r : symbol(rules[0]->name)->rules)
        s->add(r, t, 0, true);
    s->buildClosure();
}

void writeOutput(string const &cname, string const &dir) {
    ofstream file;
    string fname = dir + "/" + cname + ".gen.h";
    file.open(fname.c_str());
    if (file.is_open()) {
        cout << "opened " << fname << endl;
    } else {
        cout << "could not open " << fname << " " << endl;
        return;
    }
    file << "enum TokenId {" << endl << "    _NULL=0," << endl;
    for (auto [k, v] : symbols)
        file << "    s_" << v->name << "=" << v->index << "," << endl;
    for (auto [k, v] : terminals) {
        string_view n = v->name;
        string_view raw = n;
        string s;
        char const *prefix;
        if (n == "$") {
            n = "EOF";
            prefix = "_";
        } else if (n[0] == '\'') {
            prefix = "k_";
            s = to_string(v->index);
            n = s;
        } else {
            prefix = "t_";
        }
        file << "    " << prefix << n << "=" << v->index << ", // " << raw
             << endl;
    }
    file << "};" << endl;
    file.close();

    fname = dir + "/" + cname + ".gen.cpp";
    file.open(fname.c_str());
    if (file.is_open()) {
        cout << "opened " << fname << endl;
    } else {
        cout << "could not open " << fname << " " << endl;
        return;
    }

    file << "#include \"glr_parser_base.h\"" << endl;
    file << "#include \"" << cname << ".gen.h\"" << endl << endl;
    file << "something parseTable = {" << endl;
    for (State *s : states) {
        file << "{";
        for (auto [k, v] : s->parseTable) {
            if (v.index() == 0)
                continue;
            file << "trans(" << k->index << ",";
            if (auto statePtr = get_if<State *>(&v)) {
                State *s = *statePtr;
                file << s->index;
            } else if (auto rulePtr = get_if<Rule *>(&v)) {
                Rule *r = *rulePtr;
                file << -(r->index + 1);
            } else
                file << 0;
            file << "),";
        }
        file << "}," << endl;
    }
    file << "};" << endl;
    file.close();
}

int parse(string const &inputname, string const &dir) {

    std::ifstream file(inputname);

    if (file.is_open()) {
        std::stringstream ss;
        ss << file.rdbuf();
        std::string text = ss.str();
        cout << "read: " << text << endl << "======" << endl;
        auto ptr = text.c_str();
        auto res = readGrammar(ptr);
        if (res != nullptr) {
            for (auto e : rules)
                e->print();
            for (auto e : terminals)
                e.second->print();
            for (auto e : symbols) {
                e.second->calcFirst();
            }
            cout << "======" << endl;
            buildStates();
            for (auto e : states) {
                cout << "State:" << endl;
                e->print();
            }
            cout << "======" << endl;
            vector<Terminal *> tokens{terminal("ID"), terminal("ID"),
                                      terminal("'nix'"), terminal("ID"),
                                      terminal("'nix'")};
            vector<State *> pstate;
            vector<ParseTreeNode *> nodes;
            pstate.push_back(states[0]);
            auto iter = tokens.begin();
            auto end = tokens.end();
            do {
                pstate.back()->parse(iter, end, pstate, nodes);
            } while (iter != end || pstate.size() > 0);
            cout << nodes.size() << endl;
            nodes[0]->print(0);
            cout << "======" << endl;
            writeOutput("out", dir);
        }
        cout << "======" << endl;
        std::cout << "rest: [[[" << res << "]]]" << std::endl;
    }

    return 0;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "need exactly two arguments (got " << (argc - 1) << ")" << endl;
        return 1;
    }
    Debug::DeathHandler dh;
    parse(argv[1], argv[2]);
    return 0;
}