#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct Triple {
    u32 a, b, c;
};

struct Obj {
    u32 unk0, unk4, unk8, unkC, unk10, unk14;
    u8 unk18[4];
    u32 unk1C, unk20, unk24, state, unk2C, unk30, unk34, unk38;
};

struct Node {
    Node *a, *b, *c;
    Obj *value;
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

    Obj *&operator*() const {
        return n->value;
    }

    Obj *operator->() const {
        return n->value;
    }

    bool operator==(const It &o) const {
        return n == o.n;
    }

    bool operator!=(const It &o) const {
        return n != o.n;
    }
};

struct Handler {
    void *unk0, *unk4, *unk8;
    void (*func)(u32, Triple *, void *, u32);
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

extern "C" void ov45_0223DCD0(Obj *obj);
extern "C" Handler *ov45_022310B0(void);
extern "C" void *ov45_02231C78(void *p);

inline Triple MakeAddr(Obj *o) {
    Triple t;
    t.c = o->unk4;
    t.a = o->unk8;
    t.b = o->unkC;
    return t;
}

extern "C" void ov45_0223E558(Map *m);
#pragma opt_propagation off

extern "C" void ov45_0223E558(Map *m) {
    Obj *o2;
    u32 zero;
    u32 x;
    u32 y;
    It it;
    Node *end;
    it.n = m->tree.header.b;
    end = &m->tree.header;
    if (it.n != end) {
        zero = 0;
        do {
            ov45_0223DCD0(it.n->value);
            switch (it.n->value->state) {
            case 0:
            case 1:
            case 3:
                break;
            case 2: {
                Obj *obj = it.n->value;
                obj->unk14 = zero;
                o2 = it.n->value;
                o2->unk24 = zero;
                const Triple &t = (Triple) { .c = obj->unk4, .a = obj->unk8, .b = obj->unkC };
                Triple b = t;
                x = o2->unk1C;
                y = it.n->value->unk2C;
                Handler *h = ov45_022310B0();
                h->func(y, &b, ov45_02231C78(o2->unk18), x);
                it.n->value->unk38 = 1;
                break;
            }
            }
            ov45_02236E58(&it);
        } while (it.n != end);
    }
}

#pragma opt_propagation reset
