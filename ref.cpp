#include "out.gen.h"
//#include "glr_parser_base.h"
#include <vector>

int parse(int state, int tokenId, std::vector<int> &stateStack,
          std::vector<int> &nodeStack) {
    while (true) {
        if (tokenId == 0)
            tokenId = LA();
        switch (state) {
        case 0:
            switch (tokenId) {
            case -1: // goto
                stateStack.push_back(1);
                continue;
            case 1: { // reduce
                InnerNode *node = new InnerNode();
                node->children.resize(0);
                // node->children[0]=pop(nodeStack);
                node->rule = 3;
                nodeStack.push_back(node);
                tokenId = -1;
                continue;
            }
            default:
                if (tokenId < 0) { // goto up stack
                    stateStack.pop_back();
                    state = stateStack.back();
                    continue;
                }
                return -1;
            }
        case 1:
            switch (tokenId) {
            case 1: { // shift
                stateStack.push_back(3);
                LeafNode *node = new LeafNode();
                node->tokenId = tokenId;
                // node->text=string_view
                nodeStack.push_back(node);
                // TODO: consume token
                tokenId = 0;
                continue;
            }
            }
        }
    }
}