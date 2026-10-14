#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Value {
    virtual void f0();
    virtual void Release();
};

struct Node {
    Node *a, *b, *c;
    Value *value;
};
struct It;
extern "C" void ov45_02236E58(It *it);

struct It {
    Node *n;

    It() {
    }

    It(Node *p)
        : n(p) {
    }

    It &operator++() {
        ov45_02236E58(this);
        return *this;
    }

    It operator++(int) {
        It t = *this;
        ++*this;
        return t;
    }

    bool operator!=(const It &o) const {
        return n != o.n;
    }
};

struct Tree {
    u32 size;
    Node header;
};

extern "C" It PPWLobbyPlayerSet_Erase(Tree *t, It pos);

struct Map {
    u32 unk0;
    Tree tree;

    It begin() {
        return It(tree.header.b);
    }

    It end() {
        return It(&tree.header);
    }

    void erase(It pos) {
        PPWLobbyPlayerSet_Erase(&tree, pos);
    }
};

extern "C" void PPWLobbyPlayers_Clear(Map *m);

extern "C" void PPWLobbyPlayers_Clear(Map *m) {
    for (It it = m->begin(); it != m->end();) {
        Value *v = it.n->value;
        if (v != 0) {
            v->Release();
        }
        m->erase(it++);
    }
}
