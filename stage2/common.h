#pragma once
#include "Grammar.ast.h"
#include <map>

struct NameInfo
{
    bool isMulti;
    string type;
};

extern int currentNodeTypeId;

struct AstNodeInfo
{
    string name;
    map<string, NameInfo> names;
    AstNode *rule;
    bool isNecessary;
    int nodeTypeId;
    string type;
    string parentType;
    AstNodeInfo(string_view sv) : name(sv)
    {
        type = name + "_Node";
        parentType = "InnerNode";
        nodeTypeId = ++currentNodeTypeId;
        isNecessary = true;
    }
    AstNodeInfo(char const *n, char const *p, char const *t = nullptr) : name(n), parentType(p)
    {
        if (t)
            type = t;
        else
            type = name + "_Node";
        nodeTypeId = ++currentNodeTypeId;
        isNecessary = true;
    }
};

extern map<string, AstNodeInfo *> astNodes;
