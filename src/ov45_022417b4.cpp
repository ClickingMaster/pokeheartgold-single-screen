#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
union Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" void LobbyString_AssignCStr(Str *s, const char *p);
extern "C" void LobbyString_Assign(Str *dst, const Str *src);
extern "C" int LobbyString_CompareCStr(const Str *s, const char *p);

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

    Str(const char *p) {
        LobbyString_InitFromCStr(this, p);
    }

    void destroy() {
        LobbyString_Destroy(this);
    }

    const char *c_str() const {
        return !l.flag ? s.data : l.data;
    }

    void assign(const char *p) {
        LobbyString_AssignCStr(this, p);
    }

    void assign(const Str &o) {
        LobbyString_Assign(this, &o);
    }

    bool equals(const char *p) const {
        return LobbyString_CompareCStr(this, p) == 0;
    }
};
struct Handler;

struct HandlerVtbl {
    void *f0, *f4;
    u32 (*getId)(Handler *);
};

struct Handler {
    HandlerVtbl *vtbl;
    u32 id;
    u32 b;
    u32 a;
};

extern "C" HandlerVtbl gLobbyRequestVtable, gLobbyGetChannelKeysRequestVtable;
extern "C" const u32 gLobbyChatNextRequestId;

struct Mgr {
    void *connection;
    u32 state;
    u8 pad[0x88];
    u8 list[1];
};

extern "C" Mgr *gLobbyChatManager;
extern "C" void *LobbyMem_Alloc(u32);

struct Tag {};

struct InsertResult {
    void *it;
    bool ok;

    InsertResult() {
    }
};

extern "C" void LobbyRequestMap_Insert(InsertResult *out, void *list, Handler **h);
extern "C" Str LobbyChat_GetFullChannelName(Mgr *m, const Str &s);
extern "C" void chatGetChannelKeysA(void *chat, const char *channel, const char *nick, int count, const char **keys, void (*callback)(), u32 param, u32 blocking);
extern "C" void LobbyChat_OnGetChannelKeys();
extern "C" unsigned long strlen(const char *);
extern "C" const char sStr_Empty_17[];
extern "C" Str LobbyChat_GetUserNick(Mgr *m, int idx);

inline Mgr *GetMgr() {
    return gLobbyChatManager;
}

inline void BaseInit(Handler *h, u32 param) {
    h->vtbl = &gLobbyRequestVtable;
    h->id = 0;
    h->b = param;
}

// The id counter is only changed here; it is declared const so that stores through the new handler cannot alias it.
inline u32 NextHandlerId() {
    u32 n = gLobbyChatNextRequestId;
    *(u32 *)&gLobbyChatNextRequestId = n + 1;
    return n;
}

inline Handler *NewHandler(u32 callback, u32 param) {
    Handler *p = (Handler *)LobbyMem_Alloc(sizeof(Handler));
    if (p) {
        BaseInit(p, param);
        p->id = NextHandlerId();
        p->vtbl = &gLobbyGetChannelKeysRequestVtable;
        p->a = callback;
    }
    return p;
}

extern "C" int LobbyChat_GetChannelKeysAsync(const char *name, int userId, const char *key, u32 callback, u32 param);

struct IdTag {};

inline u32 GetId(Handler *h, IdTag, IdTag) {
    return h->vtbl->getId(h);
}

#pragma opt_common_subs off

extern "C" int LobbyChat_GetChannelKeysAsync(const char *name, int userId, const char *key, u32 callback, u32 param) {
    Mgr *m = gLobbyChatManager;
    if (m == NULL) {
        return 4;
    }
    if (m != NULL && m->state == 5) {
        return 7;
    }
    if (m != NULL && GetMgr()->state != 3) {
        return 4;
    }
    if (strlen(key) + 1 > 20) {
        return 3;
    }
    Str nick(sStr_Empty_17);
    if (userId == -1) {
        nick.assign(sStr_Empty_17);
    } else {
        Str tmp = LobbyChat_GetUserNick(GetMgr(), userId);
        nick.assign(tmp);
        tmp.destroy();
        if (nick.equals(sStr_Empty_17)) {
            nick.destroy();
            return 3;
        }
    }
    m = GetMgr();
    InsertResult res;
    Handler *h = NewHandler(callback, param);
    LobbyRequestMap_Insert(&res, m->list, &h);
    u32 id = GetId(h, IdTag(), IdTag());
    Str s(name);
    Str t = LobbyChat_GetFullChannelName(GetMgr(), s);
    chatGetChannelKeysA(GetMgr()->connection, t.c_str(), nick.c_str(), 1, &key, LobbyChat_OnGetChannelKeys, id, 0);
    t.destroy();
    s.destroy();
    nick.destroy();
    return 0;
}

#pragma opt_common_subs reset
