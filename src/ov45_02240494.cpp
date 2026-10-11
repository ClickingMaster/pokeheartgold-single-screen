#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

union String {
    struct L {
        u32 flag : 1;
        u32 capacity : 31;
        u32 size;
        char *data;
    } l;

    struct S {
        u8 flag : 1;
        u8 size : 7;
        char data[11];
    } s;

    u32 w[3];

    const char *c_str() const {
        return !l.flag ? s.data : l.data;
    }
};

extern "C" void ov45_02237B58(String *s, const char *t);
extern "C" void ov45_02237BC8(String *s);
struct Handler;

struct HandlerVtbl {
    void *f0, *f4, *f8;
    u32 (*getArg)(Handler *h);
};

struct Base {
    HandlerVtbl *vtbl;
    u32 id;
    u32 unk8;
};

struct Handler {
    virtual void f0();
    virtual void f4();
    virtual void f8();
    virtual u32 getArg();
    u32 id;
    u32 unk8;
    void (*callback)(BOOL ok, const char *str, int x, u32 a, u32 b, u32 extra);
};

extern "C" HandlerVtbl ov45_02255238;

struct Node {
    Node *l, *r, *p;
    Handler *value;
};

struct It {
    Node *n;

    It() {
    }

    It(Node *q)
        : n(q) {
    }
};

struct Map {
    u32 size;
    Node header;
};

struct Manager {
    u8 pad[0x4];
    u32 state;
    u8 pad2[0x88];
    Map map;
};

extern "C" Manager *ov45_022577DC;
extern "C" It ov45_0223FD8C(Map *map, Base *const &key);
extern "C" void ov45_02242510(Map *map, u32 id);
extern "C" int ov45_022435F8(Manager *m, String *s);
extern "C" void ov45_022428D8(String *dst, Manager *m, String *s);

inline void KeyInit(Base *k, u32 id) {
    k->vtbl = &ov45_02255238;
    k->id = id;
    k->unk8 = 0;
}

inline Handler *Lookup(Manager *m, const It &it) {
    return it.n == &m->map.header ? 0 : it.n->value;
}

inline Handler *Find(Manager *m, u32 id) {
    Base key;
    KeyInit(&key, id);
    Base *kp = &key;
    return Lookup(m, ov45_0223FD8C(&m->map, kp));
}

inline It FindIt(Manager *m, u32 id) {
    Base key;
    KeyInit(&key, id);
    Base *kp = &key;
    return ov45_0223FD8C(&m->map, kp);
}

extern "C" void ov45_02240494(void *self, int result, const char *name, const char *other, int n, u32 *pa, u32 *pb, u32 id);
#pragma push
#pragma opt_propagation off

extern "C" void ov45_02240494(void *self, int result, const char *name, const char *other, int n, u32 *pa, u32 *pb, u32 id) {
    if (ov45_022577DC != NULL && ov45_022577DC->state == 5) {
        return;
    }
    if (name == NULL) {
        return;
    }
    int x;
    if (other == NULL) {
        x = -1;
    } else {
        String s;
        ov45_02237B58(&s, other);
        x = ov45_022435F8(ov45_022577DC, &s);
        ov45_02237BC8(&s);
    }
    int sid = id;
    u32 a, b;
    Handler *h;
    Manager *m = ov45_022577DC;
    It it = FindIt(m, sid);
    if (it.n == &m->map.header) {
        h = 0;
    } else {
        Node *nd = it.n;
        h = nd->value;
    }
    a = 0;
    b = a;
    if (result == 1 && n > 0) {
        a = *pa;
        b = *pb;
    }
    String t;
    ov45_02237B58(&t, name);
    String k;
    ov45_022428D8(&k, ov45_022577DC, &t);
    const char *str = k.c_str();
    void (*cb)(BOOL, const char *, int, u32, u32, u32) = h->callback;
    cb(result == 1, str, x, a, b, h->getArg());
    ov45_02237BC8(&k);
    ov45_02237BC8(&t);
    ov45_02242510(&ov45_022577DC->map, sid);
}

#pragma pop
