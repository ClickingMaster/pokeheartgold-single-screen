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

extern "C" void LobbyString_InitFromCStr(String *s, const char *t);
extern "C" void LobbyString_Destroy(String *s);
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
    void (*callback)(BOOL ok, u32 zero, const char *str, u32 extra);
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

struct Tag {};
struct Vec;
extern "C" void ov45_0223FFE8(Vec *v, u32 *first, u32 *last, Tag t);
extern "C" void ov45_022376BC(Vec *v);
extern "C" void *ov45_0223259C(Vec *v);

struct Vec {
    u32 *ptr;
    u32 size;
    u32 cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    Vec &operator=(const Vec &o) {
        if (this != &o) {
            ov45_0223FFE8(this, o.ptr, o.ptr + o.size, Tag());
        }
        return *this;
    }

    void destroy() {
        ov45_022376BC(this);
    }

    u32 count() const {
        return size;
    }
};

extern "C" Vec ov45_02242EF4(Manager *map, int count, const char **names, const String &key);
extern "C" void ov45_022437CC(Manager *m, String *s);
extern "C" void ov45_022412CC(const char *s);

inline Handler *Get(Manager *m, const It &it) {
    Handler *h = 0;
    if (it.n != &m->map.header) {
        h = it.n->value;
    }
    return h;
}

inline Handler *Get2(Manager *m, It it) {
    if (it.n != &m->map.header) {
        return it.n->value;
    }
    return 0;
}

inline Handler *Get3(Node *n, Node *end) {
    return n == end ? 0 : n->value;
}

extern "C" void ov45_0224005C(void *self, int result, const char *name, int count, const char **names, u32 unused, u32 id);
#pragma push
#pragma opt_propagation off

extern "C" void ov45_0224005C(void *self, int result, const char *name, int count, const char **names, u32 unused, u32 id) {
    if (ov45_022577DC != NULL && ov45_022577DC->state == 5) {
        return;
    }
    if (result == 1) {
        String s;
        LobbyString_InitFromCStr(&s, name);
        Vec tmp = ov45_02242EF4(ov45_022577DC, count, names, s);
        tmp.destroy();
        LobbyString_Destroy(&s);
    }
    {
        String u;
        LobbyString_InitFromCStr(&u, name);
        ov45_022437CC(ov45_022577DC, &u);
        LobbyString_Destroy(&u);
    }
    int sid = id;
    Manager *m = ov45_022577DC;
    It it = FindIt(m, sid);
    Handler *h;
    if (it.n == &m->map.header) {
        h = 0;
    } else {
        Node *nd = it.n;
        h = nd->value;
    }
    String t;
    LobbyString_InitFromCStr(&t, name);
    String k;
    ov45_022428D8(&k, ov45_022577DC, &t);
    void (*cb)(BOOL, u32, const char *, u32) = h->callback;
    const char *str = k.c_str();
    cb(result == 1, 0, str, h->getArg());
    LobbyString_Destroy(&k);
    LobbyString_Destroy(&t);
    ov45_02242510(&ov45_022577DC->map, sid);
    if (result != 0) {
        return;
    }
    String a;
    LobbyString_InitFromCStr(&a, name);
    String b;
    ov45_022428D8(&b, ov45_022577DC, &a);
    ov45_022412CC(b.c_str());
    LobbyString_Destroy(&b);
    LobbyString_Destroy(&a);
}

#pragma pop
