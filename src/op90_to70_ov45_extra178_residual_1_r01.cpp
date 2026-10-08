#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
union Str {
    struct L { u32 flag:1; u32 capacity:31; u32 size; char *data; } l;
    struct S { u8 flag:1; u8 size:7; char data[11]; } s;
    u32 words[3];
    const char *c_str() const { return !l.flag ? s.data : l.data; }
};
struct Handler;
struct HandlerVtbl { void *f0, *f4, *f8; u32 (*getArg)(Handler *h); };
extern "C" HandlerVtbl ov45_02255238;
struct Base {
    HandlerVtbl *vtbl;
    u32 id;
    u32 unk8;
    Base() {}
    Base(u32 i) : vtbl(&ov45_02255238), id(i), unk8(0) {}
};
struct Handler {
    virtual void f0(); virtual void f4(); virtual void f8(); virtual u32 getArg();
    u32 id;
    u32 unk8;
    void (*callback)(BOOL ok, const char *str, u32 x, u32 arg);
};
struct Node { Node *l, *r, *p; Handler *value; };
struct It { Node *n; It() {} It(Node *q) : n(q) {} };
struct Map { u32 size; Node header; It end() { return It(&header); } };
struct Manager { u8 pad[0x4]; u32 state; u8 pad2[0x88]; Map map; };
extern "C" Manager *ov45_022577DC;
extern "C" It ov45_0223FD8C(Map *map, Base *const &key);
extern "C" void ov45_02242510(Map *map, u32 id);
extern "C" Str *ov45_02237B58(Str *, const char *);
extern "C" Str *ov45_02237BC8(Str *);
extern "C" void ov45_022428D8(Str *dst, Manager *m, Str *src);
extern "C" void ov45_0223F8F4(void *self, const char *name, u32 x, u32 id);
inline void KeyInit(Base *k, u32 id) { k->vtbl = &ov45_02255238; k->id = id; k->unk8 = 0; }
inline Handler *Lookup(Manager *m, const It &it) { return it.n == m->map.end().n ? 0 : it.n->value; }

extern "C" void ov45_02240718(void *self, int result, const char *name, u32 x, u32 id);
inline It FindIt(Manager *m, u32 id) {
    Base key;
    KeyInit(&key, id);
    Base *kp = &key;
    return ov45_0223FD8C(&m->map, kp);
}
inline Handler *Find(Manager *m, u32 id) {
    Base key;
    KeyInit(&key, id);
    Base *kp = &key;
    return Lookup(m, ov45_0223FD8C(&m->map, kp));
}
#pragma push
#pragma opt_propagation off
// @fn ov45_02240718
extern "C" void ov45_02240718(void *self, int result, const char *name, u32 x, u32 id) {
    if (ov45_022577DC != NULL && ov45_022577DC->state == 5) return;
    if (result == 0) return;
    ov45_0223F8F4(self, name, x, id);
    int sid = id;
    Manager *m = ov45_022577DC;
    It it = FindIt(m, sid);
    Handler *h = it.n == &m->map.header ? 0 : it.n->value;
    Str s;
    ov45_02237B58(&s, name);
    Str t;
    ov45_022428D8(&t, ov45_022577DC, &s);
    const char *str = t.c_str();
    void (*cb)(BOOL, const char *, u32, u32) = h->callback;
    cb(result == 1, str, x, h->getArg());
    ov45_02237BC8(&t);
    ov45_02237BC8(&s);
    ov45_02242510(&ov45_022577DC->map, sid);
}
#pragma pop
