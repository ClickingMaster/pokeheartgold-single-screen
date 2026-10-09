#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
struct String;
struct TreeHdr;
extern "C" void ov45_02237A24(String *dst, const String *src);
extern "C" void ov45_02237BC8(String *s);
extern "C" void ov45_02243BC0(TreeHdr *dst, const TreeHdr *src);
extern "C" void ov45_02243F1C(TreeHdr *t);
struct Blob24 { u32 w[6]; };
struct Alloc {};
struct String {
    u32 w[3];
    String(const String &s) { ov45_02237A24(this, &s); }
    String(const String &s, int) { String *const d = this; Alloc al = Alloc(); (void)al; ov45_02237A24(d, &s); }
};
struct TreeHdr {
    int count; void *root; void *leftmost;
    void init() { TreeHdr *t = this; t->count = 0; t->leftmost = &t->root; t->root = 0; }
    void copyFrom(const TreeHdr &s, const Alloc &) { TreeHdr *t = this; t->count = 0; t->leftmost = &t->root; t->root = 0; ov45_02243BC0(t, &s); }
};
struct Entry {
    u32 a; TreeHdr t; u32 x10; Blob24 b;
    Entry() { a = 0; t.init(); }
    Entry(const Entry &s) { Alloc al = Alloc(); Entry *d; const Entry *const p = &s; d = this; d->a = p->a; d->t.copyFrom(p->t, al); d->x10 = p->x10; d->b = p->b; }
};
struct NamedEntry {
    String name; Entry e;
    NamedEntry(const NamedEntry &s) : name(s.name, 0), e(s.e) {}
};
struct InsertResult { void *it; u8 inserted; };
struct Obj { u8 pad[0x80]; TreeHdr tree; };
extern "C" InsertResult ov45_02243840(TreeHdr *t, NamedEntry *e);
extern "C" NamedEntry ov45_02242BE0(String *name, Entry *src);
extern "C" int ov45_02242ACC(Obj *o, const String *name, const Blob24 *b, u32 x);
// @fn ov45_02242ACC
#pragma opt_propagation off
extern "C" int ov45_02242ACC(Obj *o, const String *name, const Blob24 *b, u32 x) {
    String a(*name);
    Entry e;
    e.x10 = x;
    e.b = *b;
    NamedEntry ne = ov45_02242BE0(&a, &e);
    NamedEntry cp(ne);
    u8 ins = ov45_02243840(&o->tree, &cp).inserted;
    ov45_02243F1C(&cp.e.t);
    ov45_02237BC8(&cp.name);
    ov45_02243F1C(&ne.e.t);
    ov45_02237BC8(&ne.name);
    ov45_02243F1C(&e.t);
    ov45_02237BC8(&a);
    return ins;
}
#pragma opt_propagation reset
