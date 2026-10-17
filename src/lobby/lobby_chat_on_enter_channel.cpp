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

extern "C" HandlerVtbl gLobbyRequestVtable;

struct Base {
    HandlerVtbl *vtbl;
    u32 id;
    u32 unk8;

    Base() {
    }

    Base(u32 i)
        : vtbl(&gLobbyRequestVtable)
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

extern "C" Manager *gLobbyChatManager;
extern "C" It LobbyRequestMap_Find(Map *map, Base *const &key);
extern "C" void LobbyRequestMap_DeleteKey(Map *map, u32 id);
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" void LobbyChat_StripChannelPrefix(Str *dst, Manager *m, Str *src);
extern "C" void LobbyChat_OnChannelModeChanged(void *self, const char *name, u32 x, u32 id);

inline void KeyInit(Base *k, u32 id) {
    k->vtbl = &gLobbyRequestVtable;
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
extern "C" int chatGetChannelNumUsersA(void *chat, const char *channel);
extern "C" void chatSetChannelLimitA(void *chat, const char *channel, int limit);
extern "C" void chatEnumUsersA(void *chat, const char *channel, void *callback, u32 param, u32 blocking);
extern "C" void LobbyChat_LeaveChannel(const char *s);
extern "C" void LobbyChat_OnEnterChannelEnumUsers(void *self, int result, const char *name, int count, const char **names, u32 unused, u32 id);
extern "C" void LobbyChat_OnEnterChannel(void *chat, int ok, int error, const char *name, u32 id);
#pragma push
#pragma opt_propagation off

extern "C" void LobbyChat_OnEnterChannel(void *chat, int ok, int error, const char *name, u32 id) {
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == 5) {
        return;
    }
    if (ok == 1) {
        Str s;
        LobbyString_InitFromCStr(&s, name);
        int userLimit = LobbyChat_GetChannelUserLimit(gLobbyChatManager, &s);
        LobbyString_Destroy(&s);
        if (userLimit != 0) {
            int numUsers = chatGetChannelNumUsersA(gLobbyChatManager->net, name);
            if (userLimit == -1) {
                ok = 0;
                error = 7;
            } else if (numUsers > userLimit || numUsers == -1) {
                ok = 0;
                error = 2;
            } else {
                chatSetChannelLimitA(gLobbyChatManager->net, name, userLimit);
            }
        }
    }
    if (ok == 1) {
        Str nick;
        LobbyChatManager_GetNick(&nick, gLobbyChatManager);
        Str channelName;
        LobbyString_InitFromCStr(&channelName, name);
        LobbyChat_AddChannelUser(gLobbyChatManager, &nick, &channelName);
        LobbyString_Destroy(&channelName);
        LobbyString_Destroy(&nick);
        chatEnumUsersA(chat, name, (void *)LobbyChat_OnEnterChannelEnumUsers, id, 0);
        return;
    }
    {
        Str s;
        LobbyString_InitFromCStr(&s, name);
        Str t;
        LobbyChat_StripChannelPrefix(&t, gLobbyChatManager, &s);
        LobbyChat_LeaveChannel(t.c_str());
        LobbyString_Destroy(&t);
        LobbyString_Destroy(&s);
    }
    int sid = id;
    Manager *m = gLobbyChatManager;
    It it = FindIt(m, sid);
    Handler *h = it.n == &m->map.header ? 0 : it.n->value;
    Str s;
    LobbyString_InitFromCStr(&s, name);
    Str t;
    LobbyChat_StripChannelPrefix(&t, gLobbyChatManager, &s);
    const char *str = t.c_str();
    void (*cb)(BOOL, int, const char *, u32) = h->callback;
    cb(0, error, str, h->getArg());
    LobbyString_Destroy(&t);
    LobbyString_Destroy(&s);
    LobbyRequestMap_DeleteKey(&gLobbyChatManager->map, sid);
}

#pragma pop
