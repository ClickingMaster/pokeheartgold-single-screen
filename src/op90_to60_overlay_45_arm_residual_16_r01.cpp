#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
struct Key { u32 v; };
struct Node;
struct It;
extern "C" void ov45_02236E58(It *it);
struct It {
    Node *n;
    It() {}
    It(Node *p) : n(p) {}
    It &operator++() { ov45_02236E58(this); return *this; }
    bool operator==(const It &o) const { return n == o.n; }
    bool operator!=(const It &o) const { return n != o.n; }
};
struct Set {
    u32 size;
    struct { Node *a, *b, *c; } header;
    It end() { return It((Node *)&header); }
};
struct Pair { u8 first[0x10]; Set second; };
struct Node { Node *a, *b, *c; Pair value; };
struct Map {
    struct { Node *a, *b, *c; } header;
    It begin() { return It(header.c); }
    It end() { return It((Node *)&header); }
    inline It FindIf(struct HasKey pred);
};
struct Obj { u8 pad[0x70]; u8 list[0x14]; Map map; };
extern "C" It ov45_02243128(Set *s, const Key &k);
extern "C" void ov45_022431AC(void *list, const Key &k);
struct HasKey {
    Key k;
    HasKey(Key q) : k(q) {}
    bool operator()(Pair &p) const { It r = ov45_02243128(&p.second, k); return r != p.second.end(); }
};
inline It Map::FindIf(HasKey pred) {
    It it = begin();
    It e = end();
    while (it != e && !pred(it.n->value)) ++it;
    return it;
}
extern "C" BOOL ov45_0224307C(Obj *o, Key k);
// @fn ov45_0224307C
extern "C" BOOL ov45_0224307C(Obj *o, Key k) {
    It it = o->map.FindIf(HasKey(k));
    if (it != o->map.end()) return 0;
    ov45_022431AC(o->list, k);
    return 1;
}
