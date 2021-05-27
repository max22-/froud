#include "froud.h"

void Circuit::registerNode(Node* node) {
    nodes.insert(node);
}
void Circuit::unregisterNode(Node* node) {
    nodes.erase(node);
}

void Circuit::tick() {
    for(auto n: nodes)
        if(n->isFireable())
            n->process();
}