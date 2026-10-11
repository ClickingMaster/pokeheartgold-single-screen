#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

inline void *operator new(unsigned long, void *p) {
    return p;
}

extern "C" void ov00_021EC2EC(int, void *, u32);
extern "C" void *ov45_02242394(u32 size);

struct Node {
    Node *left, *right;
    u32 pc;
    u32 value;

    void set_parent(Node *p) {
        pc = (u32)p | (pc & 1);
    }

    enum Color {
        black,
        red_
    };

    Color color() const {
        return (pc & 1) ? red_ : black;
    }

    void set_color(Color c) {
        if (c) {
            pc |= 1;
        } else {
            pc &= ~1;
        }
    }
};

struct Alloc {
    Node *allocate() {
        return (Node *)ov45_02242394(sizeof(Node));
    }
};

struct Tree {
    u32 size;
    Alloc alloc;
    Node *leftmost;
};

struct Holder {
    Node *a, *b;
    Alloc *alloc;
    Node *ptr;

    Holder(Node *p, Alloc *al)
        : a(p)
        , b(p)
        , alloc(al)
        , ptr(p) {
    }

    Node *release() {
        Node *r = ptr;
        ptr = 0;
        return r;
    }
};

inline void Destroy(Holder *h) {
    if (h->ptr) {
        ov00_021EC2EC(0, h->ptr, 0);
    }
}

inline void Construct(u32 *p, const u32 &v) {
    new (p) u32(v);
}

extern "C" void ov45_02243CF8(Tree *t, Node *parent, Node *src, BOOL left);
#pragma opt_scalarize off
#pragma opt_common_subs off

extern "C" void ov45_02243CF8(Tree *t, Node *parent, Node *src, BOOL left) {
    Alloc a = Alloc();
    Node *p = a.allocate();
    Holder h(p, &t->alloc);
    p->right = 0;
    p->left = 0;
    p->set_parent(parent);
    if (left) {
        Construct(&h.ptr->value, src->left->value);
        t->leftmost = h.ptr;
        parent->left = h.ptr;
        parent = h.release();
        src = src->left;
    } else {
        Construct(&h.ptr->value, src->right->value);
        parent->right = h.ptr;
        parent = h.release();
        src = src->right;
    }
    parent->set_color(src->color());
    ++t->size;
    if (src->right) {
        ov45_02243CF8(t, parent, src, FALSE);
    }
    if (src->left) {
        ov45_02243CF8(t, parent, src, TRUE);
    }
    Destroy(&h);
}
