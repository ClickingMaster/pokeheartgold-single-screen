#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
union Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);
extern "C" Str *LobbyString_Destroy(Str *);

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

extern "C" HandlerVtbl gLobbyRequestVtable, gLobbyEnumUsersRequestVtable;
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
extern "C" void chatEnumUsersA(void *ctx, const char *name, void (*cb)(), u32 id, u32 z);
extern "C" int chatInChannelA(void *ctx, const char *name);
extern "C" void LobbyChat_OnEnumUsers();

inline Mgr *GetMgr() {
    return gLobbyChatManager;
}

inline void BaseInit(Handler *h, u32 b) {
    h->vtbl = &gLobbyRequestVtable;
    h->id = 0;
    h->b = b;
}

// The id counter is only ever read/written here; it is declared const so that
// stores through `this` are known not to alias it (native schedules the load
// above the base-part stores).
inline u32 NextHandlerId() {
    u32 n = gLobbyChatNextRequestId;
    *(u32 *)&gLobbyChatNextRequestId = n + 1;
    return n;
}

inline Handler *NewHandler(u32 a, u32 b) {
    Handler *p = (Handler *)LobbyMem_Alloc(sizeof(Handler));
    if (p) {
        BaseInit(p, b);
        p->id = NextHandlerId();
        p->vtbl = &gLobbyEnumUsersRequestVtable;
        p->a = a;
    }
    return p;
}

extern "C" int LobbyChat_EnumUsersAsync(const char *name, u32 a, u32 b);

struct IdTag {};

inline u32 GetId(Handler *h, IdTag, IdTag) {
    return h->vtbl->getId(h);
}

#pragma opt_common_subs off

extern "C" int LobbyChat_EnumUsersAsync(const char *name, u32 a, u32 b) {
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
    {
        Str s(name);
        Str t = LobbyChat_GetFullChannelName(GetMgr(), s);
        BOOL exists = chatInChannelA(GetMgr()->connection, t.c_str()) == 0;
        t.destroy();
        s.destroy();
        if (exists) {
            return 5;
        }
    }
    m = GetMgr();
    InsertResult res;
    Handler *h = NewHandler(a, b);
    LobbyRequestMap_Insert(&res, m->list, &h);
    u32 id = GetId(h, IdTag(), IdTag());
    Str s(name);
    Str t = LobbyChat_GetFullChannelName(GetMgr(), s);
    chatEnumUsersA(GetMgr()->connection, t.c_str(), LobbyChat_OnEnumUsers, id, 0);
    t.destroy();
    s.destroy();
    return 0;
}

#pragma opt_common_subs reset
