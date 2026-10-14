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

extern "C" HandlerVtbl gLobbyRequestVtable;

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

extern "C" Manager *gLobbyChatManager;
extern "C" It LobbyRequestMap_Find(Map *map, Base *const &key);
extern "C" void LobbyRequestMap_DeleteKey(Map *map, u32 id);
extern "C" int LobbyChat_FindUserIdByNick(Manager *m, String *s);
extern "C" void LobbyChat_StripChannelPrefix(String *dst, Manager *m, String *s);

inline void KeyInit(Base *k, u32 id) {
    k->vtbl = &gLobbyRequestVtable;
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
    return LobbyRequestMap_Find(&m->map, kp);
}

inline Handler *Find(Manager *m, u32 id) {
    Base key;
    KeyInit(&key, id);
    Base *kp = &key;
    return Lookup(m, LobbyRequestMap_Find(&m->map, kp));
}

struct Tag {};
struct Vec;
extern "C" void LobbyU32Vector_AssignRange(Vec *v, u32 *first, u32 *last, Tag t);
extern "C" void LobbyU32Vector_Destroy(Vec *v);
extern "C" void *LobbyU32Vector_Data2(Vec *v);

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
            LobbyU32Vector_AssignRange(this, o.ptr, o.ptr + o.size, Tag());
        }
        return *this;
    }

    void destroy() {
        LobbyU32Vector_Destroy(this);
    }

    u32 count() const {
        return size;
    }
};

extern "C" Vec LobbyChat_AddChannelUsers(Manager *map, int count, const char **names, const String &key);
extern "C" void LobbyChat_SetChannelUsersEnumerated(Manager *m, String *s);
extern "C" void LobbyChat_LeaveChannel(const char *s);

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

extern "C" void LobbyChat_OnEnterChannelEnumUsers(void *self, int result, const char *name, int count, const char **names, u32 unused, u32 id);
#pragma push
#pragma opt_propagation off

extern "C" void LobbyChat_OnEnterChannelEnumUsers(void *self, int result, const char *name, int count, const char **names, u32 unused, u32 id) {
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == 5) {
        return;
    }
    if (result == 1) {
        String s;
        LobbyString_InitFromCStr(&s, name);
        Vec tmp = LobbyChat_AddChannelUsers(gLobbyChatManager, count, names, s);
        tmp.destroy();
        LobbyString_Destroy(&s);
    }
    {
        String u;
        LobbyString_InitFromCStr(&u, name);
        LobbyChat_SetChannelUsersEnumerated(gLobbyChatManager, &u);
        LobbyString_Destroy(&u);
    }
    int sid = id;
    Manager *m = gLobbyChatManager;
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
    LobbyChat_StripChannelPrefix(&k, gLobbyChatManager, &t);
    void (*cb)(BOOL, u32, const char *, u32) = h->callback;
    const char *str = k.c_str();
    cb(result == 1, 0, str, h->getArg());
    LobbyString_Destroy(&k);
    LobbyString_Destroy(&t);
    LobbyRequestMap_DeleteKey(&gLobbyChatManager->map, sid);
    if (result != 0) {
        return;
    }
    String a;
    LobbyString_InitFromCStr(&a, name);
    String b;
    LobbyChat_StripChannelPrefix(&b, gLobbyChatManager, &a);
    LobbyChat_LeaveChannel(b.c_str());
    LobbyString_Destroy(&b);
    LobbyString_Destroy(&a);
}

#pragma pop
