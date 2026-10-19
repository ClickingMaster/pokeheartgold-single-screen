#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_request.h"

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

extern "C" const u32 gLobbyChatNextRequestId;

struct Mgr {
    void *connection;
    u32 state;
    u8 pad[0x88];
    LobbyRequestMap requestMap;
};

extern "C" Mgr *gLobbyChatManager;
extern "C" void *LobbyMem_Alloc(u32);

extern "C" Str LobbyChat_GetFullChannelName(Mgr *m, const Str &s);
extern "C" void chatGetChannelModeA(void *chat, const char *channel, void (*callback)(), u32 param, u32 blocking);
extern "C" void LobbyChat_OnGetChannelMode();

inline Mgr *GetMgr() {
    return gLobbyChatManager;
}

inline void BaseInit(LobbyRequest *h, u32 param) {
    h->vtable = &gLobbyRequestVtable;
    h->id = 0;
    h->param = param;
}

// The id counter is only changed here; it is declared const so that stores through the new handler cannot alias it.
inline u32 NextHandlerId() {
    u32 n = gLobbyChatNextRequestId;
    *(u32 *)&gLobbyChatNextRequestId = n + 1;
    return n;
}

inline LobbyCallbackRequest *NewHandler(u32 callback, u32 param) {
    LobbyCallbackRequest *p = (LobbyCallbackRequest *)LobbyMem_Alloc(sizeof(LobbyCallbackRequest));
    if (p) {
        BaseInit(p, param);
        p->id = NextHandlerId();
        p->vtable = &gLobbyGetChannelModeRequestVtable;
        p->callback = callback;
    }
    return p;
}

extern "C" int LobbyChat_GetChannelModeAsync(const char *name, u32 callback, u32 param);

struct IdTag {};

inline u32 GetId(LobbyCallbackRequest *h, IdTag, IdTag) {
    return h->vtable->getId(h);
}

#pragma opt_common_subs off

extern "C" int LobbyChat_GetChannelModeAsync(const char *name, u32 callback, u32 param) {
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
    LobbyRequestMapInsertResult res;
    LobbyCallbackRequest *h;
    h = NewHandler(callback, param);
    LobbyRequestMap_Insert(&res, &m->requestMap, &h);
    u32 id = GetId(h, IdTag(), IdTag());
    Str s(name);
    Str t = LobbyChat_GetFullChannelName(GetMgr(), s);
    chatGetChannelModeA(GetMgr()->connection, t.c_str(), LobbyChat_OnGetChannelMode, id, 0);
    t.destroy();
    s.destroy();
    return 0;
}

#pragma opt_common_subs reset
