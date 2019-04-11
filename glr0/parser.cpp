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

struct Production {
    string_view name;
    vector<Element *> elements;

    Production() : name(), elements() {}
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

struct Rule : public Element {
    vector<Production *> productions;

    Rule() : Element(), productions() {}
    void print() {
        for (auto p : productions)
            p->print();
    }
};

map<string, Rule *> rules;
map<string, Terminal *> terminals;

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
    auto production = new Production;
    while (*ptr) {
        auto m = ID(ptr);
        if (m) {
            production->name = string_view(ptr, m - ptr);
        } else {
            delete production;
            return ptr;
        }

        ptr = skipSpace(m);
        m = literal(ptr, "=");
        if (!m) {
            delete production;
            return ptr;
        }

        while (true) {
            ptr = skipSpace(m);

            m = ID(ptr);
            if (m) {
                string_view sv = string_view(ptr, m - ptr);
                cout << "found " << sv << endl;
                if (isupper(sv[0])) {
                    Terminal *&term = terminals[string(sv)];
                    if (!term) {
                        term = new Terminal();
                        term->name = sv;
                    }
                    production->elements.push_back(term);
                } else {
                    Rule *&rule = rules[string(sv)];
                    if (!rule)
                        rule = new Rule();
                    production->elements.push_back(rule);
                }
                ptr = m;
                continue;
            }

            m = STRING(ptr);
            if (m) {
                string_view sv = string_view(ptr, m - ptr);
                cout << "found " << sv << endl;
                Terminal *&term = terminals[string(sv)];
                if (!term) {
                    term = new Terminal();
                    term->name = sv;
                }
                production->elements.push_back(term);
                cout << "got " << production->elements.back()->name << endl;
                ptr = m;
                continue;
            }

            m = literal(ptr, ";");
            if (m) {
                ptr = m;
                break;
            }

            delete production;
            return ptr;
        }

        Rule *&rule = rules[string(production->name)];
        if (!rule)
            rule = new Rule();
        rule->name = production->name;
        rule->productions.push_back(production);
        production = new Production;
        ptr = skipSpace(ptr);
    }
    return ptr;
}

int parse(string const &inputname, string const &dir) {

    std::ifstream file(inputname);

    if (file.is_open()) {
        std::stringstream ss;
        ss << file.rdbuf();
        std::string text = ss.str();
        std::cout << "read: " << text << std::endl;
        auto ptr = text.c_str();
        auto res = readGrammar(ptr);
        if (res != nullptr) {
            for (auto e : rules)
                e.second->print();
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