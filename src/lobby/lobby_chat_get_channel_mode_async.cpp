#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_manager.h"


extern "C" const u32 gLobbyChatNextRequestId;

extern "C" void *LobbyMem_Alloc(u32);

extern "C" Str LobbyChat_GetFullChannelName(LobbyChatManager *m, const Str &s);
extern "C" void chatGetChannelModeA(void *chat, const char *channel, void (*callback)(), u32 param, u32 blocking);
extern "C" void LobbyChat_OnGetChannelMode();

inline LobbyChatManager *GetMgr() {
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
    LobbyChatManager *m = gLobbyChatManager;
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
    Str s;
    LobbyString_InitFromCStr(&s, name);
    Str t = LobbyChat_GetFullChannelName(GetMgr(), s);
    chatGetChannelModeA(GetMgr()->connection, t.c_str(), LobbyChat_OnGetChannelMode, id, 0);
    LobbyString_Destroy(&t);
    LobbyString_Destroy(&s);
    return 0;
}

#pragma opt_common_subs reset
