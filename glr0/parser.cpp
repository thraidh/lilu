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
#include <vector>

using namespace std;

struct Element {
    string_view name;
    Element() : name() {}
};

struct Rule {
    string_view name;
    vector<Element *> elements;

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
    Terminal() : Element() {}

    void print() { cout << name << endl; }
};

struct Symbol : public Element {
    vector<Rule *> rules;

    Symbol() : Element(), rules() {}
    void print() {
        for (auto p : rules)
            p->print();
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
                    Terminal *&term = terminals[string(sv)];
                    if (!term) {
                        term = new Terminal();
                        term->name = sv;
                    }
                    rule->elements.push_back(term);
                } else {
                    Symbol *&symbol = symbols[string(sv)];
                    if (!symbol)
                        symbol = new Symbol();
                    rule->elements.push_back(symbol);
                }
                ptr = m;
                continue;
            }

            m = STRING(ptr);
            if (m) {
                string_view sv = string_view(ptr, m - ptr);
                Terminal *&term = terminals[string(sv)];
                if (!term) {
                    term = new Terminal();
                    term->name = sv;
                }
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

        Symbol *&symbol = symbols[string(rule->name)];
        if (!symbol)
            symbol = new Symbol();
        symbol->name = rule->name;
        symbol->rules.push_back(rule);
        rules.push_back(rule);
        ptr = skipSpace(ptr);
    }
    return ptr;
}

inline auto symbol(string const &str) {
    auto &ret = symbols[str];
    if (!ret) {
        ret = new Symbol();
        ret->name = str;
    }
    return ret;
}

inline auto symbol(char const *str) {
    auto &ret = symbols[str];
    if (!ret) {
        ret = new Symbol();
        ret->name = str;
    }
    return ret;
}

inline auto symbol(string_view const &str) {
    auto &ret = symbols[string(str)];
    if (!ret) {
        ret = new Symbol();
        ret->name = str;
    }
    return ret;
}

inline auto terminal(char const *str) {
    auto &ret = terminals[str];
    if (!ret) {
        ret = new Terminal();
        ret->name = str;
    }
    return ret;
}

inline auto terminal(string const &str) {
    auto &ret = terminals[str];
    if (!ret) {
        ret = new Terminal();
        ret->name = str;
    }
    return ret;
}

inline auto terminal(string_view const &str) {
    auto &ret = terminals[string(str)];
    if (!ret) {
        ret = new Terminal();
        ret->name = str;
    }
    return ret;
}

int parse(string const &inputname, string const &dir) {

    std::ifstream file(inputname);

    if (file.is_open()) {
        std::stringstream ss;
        ss << file.rdbuf();
        std::string text = ss.str();
        cout << "read: " << text << endl << "======" << endl;
        rules.push_back(new Rule());
        auto ptr = text.c_str();
        auto res = readGrammar(ptr);
        rules[0]->name = "0";
        rules[0]->elements.push_back(symbol(rules[1]->name));
        rules[0]->elements.push_back(terminal("$"));
        if (res != nullptr) {
            for (auto e : rules)
                e->print();
            for (auto e : terminals)
                e.second->print();
        }
        std::cout << "rest: [[[" << res << "]]]" << std::endl;
    }

    return 0;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "need exactly two arguments (got " << (argc - 1) << ")" << endl;
        return 1;
    }
    parse(argv[1], argv[2]);
    return 0;
}