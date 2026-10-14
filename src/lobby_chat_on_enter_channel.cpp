#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

union Str {
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

    u32 words[3];

    const char *c_str() const {
        return !l.flag ? s.data : l.data;
    }
};
struct Handler;

struct HandlerVtbl {
    void *f0, *f4, *f8;
    u32 (*getArg)(Handler *h);
};

extern "C" HandlerVtbl ov45_02255238;

struct Base {
    HandlerVtbl *vtbl;
    u32 id;
    u32 unk8;

    Base() {
    }

    Base(u32 i)
        : vtbl(&ov45_02255238)
        , id(i)
        , unk8(0) {
    }
};

struct Handler {
    virtual void f0();
    virtual void f4();
    virtual void f8();
    virtual u32 getArg();
    u32 id;
    u32 unk8;
    void (*callback)(BOOL ok, int error, const char *str, u32 arg);
};

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

    It end() {
        return It(&header);
    }
};

struct Manager {
    void *net;
    u32 state;
    u8 pad2[0x88];
    Map map;
};

extern "C" Manager *ov45_022577DC;
extern "C" It LobbyRequestMap_Find(Map *map, Base *const &key);
extern "C" void LobbyRequestMap_DeleteKey(Map *map, u32 id);
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" void LobbyChat_StripChannelPrefix(Str *dst, Manager *m, Str *src);
extern "C" void LobbyChat_OnChannelModeChanged(void *self, const char *name, u32 x, u32 id);

inline void KeyInit(Base *k, u32 id) {
    k->vtbl = &ov45_02255238;
    k->id = id;
    k->unk8 = 0;
}

inline Handler *Lookup(Manager *m, const It &it) {
    return it.n == m->map.end().n ? 0 : it.n->value;
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

extern "C" void LobbyChatManager_GetNick(Str *dst, Manager *m);
extern "C" void LobbyChat_AddChannelUser(Manager *m, Str *a, Str *b);
extern "C" int LobbyChat_GetChannelUserLimit(Manager *m, Str *s);
extern "C" int chatGetChannelNumUsersA(void *net, const char *name);
extern "C" void chatSetChannelLimitA(void *net, const char *name, int n);
extern "C" void chatEnumUsersA(void *session, const char *name, void *cb, u32 id, u32 z);
extern "C" void LobbyChat_LeaveChannel(const char *s);
extern "C" void LobbyChat_OnEnterChannelEnumUsers(void *self, int result, const char *name, int count, const char **names, u32 unused, u32 id);
extern "C" void LobbyChat_OnEnterChannel(void *session, int ok, int error, const char *name, u32 id);
#pragma push
#pragma opt_propagation off

extern "C" void LobbyChat_OnEnterChannel(void *session, int ok, int error, const char *name, u32 id) {
    if (ov45_022577DC != NULL && ov45_022577DC->state == 5) {
        return;
    }
    if (ok == 1) {
        Str s;
        LobbyString_InitFromCStr(&s, name);
        int found = LobbyChat_GetChannelUserLimit(ov45_022577DC, &s);
        LobbyString_Destroy(&s);
        if (found != 0) {
            int limit = chatGetChannelNumUsersA(ov45_022577DC->net, name);
            if (found == -1) {
                ok = 0;
                error = 7;
            } else if (limit > found || limit == -1) {
                ok = 0;
                error = 2;
            } else {
                chatSetChannelLimitA(ov45_022577DC->net, name, found);
            }
        }
    }
    if (ok == 1) {
        Str a;
        LobbyChatManager_GetNick(&a, ov45_022577DC);
        Str b;
        LobbyString_InitFromCStr(&b, name);
        LobbyChat_AddChannelUser(ov45_022577DC, &a, &b);
        LobbyString_Destroy(&b);
        LobbyString_Destroy(&a);
        chatEnumUsersA(session, name, (void *)LobbyChat_OnEnterChannelEnumUsers, id, 0);
        return;
    }
    {
        Str s;
        LobbyString_InitFromCStr(&s, name);
        Str t;
        LobbyChat_StripChannelPrefix(&t, ov45_022577DC, &s);
        LobbyChat_LeaveChannel(t.c_str());
        LobbyString_Destroy(&t);
        LobbyString_Destroy(&s);
    }
    int sid = id;
    Manager *m = ov45_022577DC;
    It it = FindIt(m, sid);
    Handler *h = it.n == &m->map.header ? 0 : it.n->value;
    Str s;
    LobbyString_InitFromCStr(&s, name);
    Str t;
    LobbyChat_StripChannelPrefix(&t, ov45_022577DC, &s);
    const char *str = t.c_str();
    void (*cb)(BOOL, int, const char *, u32) = h->callback;
    cb(0, error, str, h->getArg());
    LobbyString_Destroy(&t);
    LobbyString_Destroy(&s);
    LobbyRequestMap_DeleteKey(&ov45_022577DC->map, sid);
}

#pragma pop
