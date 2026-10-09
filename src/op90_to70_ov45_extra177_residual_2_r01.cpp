#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
struct String { u32 w[3]; };
struct Handler;
struct HandlerVtbl { void *f0, *f4, *f8; u32 (*getArg)(Handler *h); };
extern "C" HandlerVtbl ov45_02255238;
struct Base {
    HandlerVtbl *vtbl;
    u32 id;
    u32 unk8;
};
struct Handler {
    virtual void f0(); virtual void f4(); virtual void f8(); virtual u32 getArg();
    u32 id;
    u32 unk8;
    void (*callback)(BOOL ok, u32 a, const char *str, u32 b, u32 c, u32 extra);
};
struct Node { Node *l, *r, *p; Handler *value; };
struct It { Node *n; It() {} It(Node *q) : n(q) {} };
struct Map { u32 size; Node header; It end() { return It(&header); } };
struct Manager { u8 pad[0x4]; u32 state; u8 pad2[0x88]; Map map; };
extern "C" Manager *ov45_022577DC;
extern "C" void ov45_02242910(String *dst, Manager *mgr, u32 a, u32 b);
extern "C" It ov45_0223FD8C(Map *map, Base *const &key);
extern "C" const char *ov45_0223FD74(String *s);
extern "C" void ov45_02242510(Map *map, u32 id);
extern "C" void ov45_022376BC(String *s);
inline void KeyInit(Base *k, u32 id) { k->vtbl = &ov45_02255238; k->id = id; k->unk8 = 0; }
inline It FindIt(Manager *m, u32 id) {
    Base key;
    KeyInit(&key, id);
    Base *kp = &key;
    return ov45_0223FD8C(&m->map, kp);
}
extern "C" void ov45_0223FC6C(void *self, int result, u32 a, u32 d, u32 b, u32 c, u32 id);
#pragma push
#pragma opt_propagation off
// @fn ov45_0223FC6C
extern "C" void ov45_0223FC6C(void *self, int result, u32 a, u32 d, u32 b, u32 c, u32 id) {
    if (ov45_022577DC != NULL && ov45_022577DC->state == 5) return;
    String str;
    ov45_02242910(&str, ov45_022577DC, a, d);
    int sid = id;
    Manager *m = ov45_022577DC;
    It it = FindIt(m, sid);
    Handler *h;
    if (it.n == &m->map.header)
    {
        h = 0;
    }
    else
    {
        Node *nd = it.n;
        h = nd->value;
    }
    void (*cb)(BOOL, u32, const char *, u32, u32, u32) = h->callback;
    cb(result == 1, a, ov45_0223FD74(&str), b, c, h->getArg());
    ov45_02242510(&ov45_022577DC->map, sid);
    ov45_022376BC(&str);
}
#pragma pop
