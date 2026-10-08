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
    void (*callback)(BOOL ok, int error, const char *str, u32 arg);
};
struct Node { Node *l, *r, *p; Handler *value; };
struct It { Node *n; It() {} It(Node *q) : n(q) {} };
struct Map { u32 size; Node header; It end() { return It(&header); } };
struct Manager { void *net; u32 state; u8 pad2[0x88]; Map map; };
extern "C" Manager *ov45_022577DC;
extern "C" It ov45_0223FD8C(Map *map, Base *const &key);
extern "C" void ov45_02242510(Map *map, u32 id);
extern "C" Str *ov45_02237B58(Str *, const char *);
extern "C" Str *ov45_02237BC8(Str *);
extern "C" void ov45_022428D8(Str *dst, Manager *m, Str *src);
extern "C" void ov45_0223F8F4(void *self, const char *name, u32 x, u32 id);
inline void KeyInit(Base *k, u32 id) { k->vtbl = &ov45_02255238; k->id = id; k->unk8 = 0; }
inline Handler *Lookup(Manager *m, const It &it) { return it.n == m->map.end().n ? 0 : it.n->value; }

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
extern "C" void ov45_0223FC5C(Str *dst, Manager *m);
extern "C" void ov45_02242D30(Manager *m, Str *a, Str *b);
extern "C" int ov45_02243808(Manager *m, Str *s);
extern "C" int ov45_02250F5C(void *net, const char *name);
extern "C" void ov45_02250E6C(void *net, const char *name, int n);
extern "C" void ov45_02251208(void *session, const char *name, void *cb, u32 id, u32 z);
extern "C" void ov45_022412CC(const char *s);
extern "C" void ov45_0224005C(void *self, int result, const char *name, int count, const char **names, u32 unused, u32 id);
extern "C" void ov45_02240234(void *session, int ok, int error, const char *name, u32 id);
#pragma push
#pragma opt_propagation off
// @fn ov45_02240234
extern "C" void ov45_02240234(void *session, int ok, int error, const char *name, u32 id) {
    if (ov45_022577DC != NULL && ov45_022577DC->state == 5) return;
    if (ok == 1) {
        Str s;
        ov45_02237B58(&s, name);
        int found = ov45_02243808(ov45_022577DC, &s);
        ov45_02237BC8(&s);
        if (found != 0) {
            int limit = ov45_02250F5C(ov45_022577DC->net, name);
            if (found == -1) {
                ok = 0;
                error = 7;
            } else if (limit > found || limit == -1) {
                ok = 0;
                error = 2;
            } else {
                ov45_02250E6C(ov45_022577DC->net, name, found);
            }
        }
    }
    if (ok == 1) {
        Str a;
        ov45_0223FC5C(&a, ov45_022577DC);
        Str b;
        ov45_02237B58(&b, name);
        ov45_02242D30(ov45_022577DC, &a, &b);
        ov45_02237BC8(&b);
        ov45_02237BC8(&a);
        ov45_02251208(session, name, (void *)ov45_0224005C, id, 0);
        return;
    }
    {
        Str s;
        ov45_02237B58(&s, name);
        Str t;
        ov45_022428D8(&t, ov45_022577DC, &s);
        ov45_022412CC(t.c_str());
        ov45_02237BC8(&t);
        ov45_02237BC8(&s);
    }
    int sid = id;
    Manager *m = ov45_022577DC;
    It it = FindIt(m, sid);
    Handler *h = it.n == &m->map.header ? 0 : it.n->value;
    Str s;
    ov45_02237B58(&s, name);
    Str t;
    ov45_022428D8(&t, ov45_022577DC, &s);
    const char *str = t.c_str();
    void (*cb)(BOOL, int, const char *, u32) = h->callback;
    cb(0, error, str, h->getArg());
    ov45_02237BC8(&t);
    ov45_02237BC8(&s);
    ov45_02242510(&ov45_022577DC->map, sid);
}
#pragma pop
