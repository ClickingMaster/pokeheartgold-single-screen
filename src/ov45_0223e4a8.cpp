#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
struct Value;

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

    Value *&operator*() const {
        return n->value;
    }

    bool operator==(const It &o) const {
        return n == o.n;
    }

    bool operator!=(const It &o) const {
        return n != o.n;
    }
};

struct Value {
    virtual void f0();
};

typedef bool (Value::*Pmf)();

struct Holder {
    u32 unk0;
    Pmf pmf;
};

extern "C" Holder ov45_02255200;

struct Pred {
    Pmf f;

    Pred(Pmf q)
        : f(q) {
    }

    bool operator()(Value *v) const {
        return (v->*f)();
    }
};

struct Tree {
    u32 size;
    Node header;
};

struct Map {
    u32 unk0;
    Tree tree;

    It begin() {
        return It(tree.header.b);
    }

    It end() {
        return It(&tree.header);
    }
};

inline It FindIfNot(It first, It last, Pred pred) {
    while (first != last && pred(*first)) {
        ++first;
    }
    return first;
}

extern "C" BOOL ov45_0223E4A8(Map *m);

extern "C" BOOL ov45_0223E4A8(Map *m) {
    return FindIfNot(m->begin(), m->end(), Pred(ov45_02255200.pmf)) == m->end();
}
