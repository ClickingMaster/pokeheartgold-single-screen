#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

inline void *operator new(unsigned long, void *p) {
    return p;
}

extern "C" void ov00_021EC2EC(int, void *, u32);
extern "C" void *ov45_02242394(u32 size);
extern "C" int printf(const char *, ...);
extern "C" void abort(void);
extern "C" char ov45_022553AC[];
struct String;
extern "C" String *ov45_02237A24(String *dst, const String *src);

struct String {
    u32 w[3];

    String(const String &o) {
        ov45_02237A24(this, &o);
    }
};
struct Set;
extern "C" void ov45_02243BC0(Set *dst, const Set *src);

struct Set {
    u32 size;
    void *root;
    void **first;

    Set(const Set &o)
        : size(0)
        , root(0)
        , first(&root) {
        ov45_02243BC0(this, &o);
    }
};

struct Blob {
    u32 w[6];
};

struct Value {
    String s;
    u32 a;
    Set set;
    u32 b;
    Blob c;
};

struct Node {
    Node *left, *right, *parent;
    Value value;
};

struct Alloc {
    Node *allocate() {
        return (Node *)ov45_02242394(sizeof(Node));
    }
};

struct Tree {
    u32 size;
    Node *root;
    Node *x8;
    Node *xC;
};

extern "C" void ov45_02236EC4(Node *n, Node *root);

struct Holder {
    Alloc na;
    bool constructed;
    Node *a, *b;
    Alloc *alloc;
    Node *volatile ptr;

    Holder(Alloc *al)
        : constructed(false)
        , a(na.allocate())
        , b(a)
        , alloc(al)
        , ptr(a) {
    }

    void Destroy() {
        Node *q = ptr;
        if (q) {
            ov00_021EC2EC(0, q, 0);
        }
    }

    Node *release() {
        Node *r = ptr;
        ptr = 0;
        return r;
    }
};

extern "C" Node *ov45_02243970(Tree *t, Node *parent, BOOL left, BOOL rightmost, const Value &v);

extern "C" Node *ov45_02243970(Tree *t, Node *parent, BOOL left, BOOL rightmost, const Value &v) {
    if (t->size == 0xFFFFFFFF) {
        printf(ov45_022553AC);
        abort();
    }
    Holder h((Alloc *)&t->root);
    Node *p = h.a;
    new (&p->value) Value(v);
    Node *n = h.ptr;
    n->right = 0;
    h.ptr = 0;
    n->left = 0;
    new (&n->parent) Node *(parent);
    if (left) {
        parent->left = n;
    } else {
        parent->right = n;
    }
    ++t->size;
    ov45_02236EC4(n, t->root);
    if (rightmost) {
        t->xC = n;
    }
    h.Destroy();
    return n;
}
