#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_mem.h"

#include "lobby_chat_manager.h"

extern "C" unsigned long strlen(const char *);
extern "C" void MI_CpuFill8(void *, u8, u32);

extern "C" const u32 gLobbyChatNextRequestId;


extern "C" Str LobbyChat_GetFullChannelName(LobbyChatManager *m, const Str &s);

extern "C" void LobbyChat_AddChannel(LobbyChatManager *m, const Str &s, LobbyChannelCallbacks *callbacks, u32 limit);

struct Callbacks {
    void (*message)();
    void (*kicked)();
    void (*userJoined)();
    void (*userLeft)();
    void (*nickChanged)();
    void (*topicChanged)();
    void (*modeChanged)();
    void (*userModeChanged)();
    void (*usersChanged)();
    void (*unk_24)();
    void (*broadcastKeyChanged)();
    void *param;
};

extern "C" void chatEnterChannelA(void *chat, const char *channel, u32 password, Callbacks *callbacks, void (*callback)(), u32 param, u32 blocking);
extern "C" void LobbyChat_OnChannelMessage();
extern "C" void LobbyChat_OnKicked();
extern "C" void LobbyChat_OnUserJoined();
extern "C" void LobbyChat_OnUserParted();
extern "C" void LobbyChat_OnUserChangedNick();
extern "C" void LobbyChat_OnUserModeChanged();
extern "C" void LobbyChat_OnTopicChanged();
extern "C" void LobbyChat_OnChannelModeChanged();
extern "C" void LobbyChat_OnUserListUpdated();
extern "C" void LobbyChat_OnEnterChannel();
extern "C" void LobbyChat_OnBroadcastKeyChanged();

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
        p->vtable = &gLobbyEnterChannelRequestVtable;
        p->callback = callback;
    }
    return p;
}

struct IdTag {};

inline u32 GetId(LobbyCallbackRequest *h, IdTag, IdTag) {
    return h->vtable->getId(h);
}

extern "C" int LobbyChat_EnterChannelAsync(const char *name, u32 password, u32 limit, LobbyChannelCallbacks *channelCallbacks, u32 callback, u32 param);
#pragma opt_common_subs off

extern "C" int LobbyChat_EnterChannelAsync(const char *name, u32 password, u32 limit, LobbyChannelCallbacks *channelCallbacks, u32 callback, u32 param) {
    Callbacks cb;
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
    strlen(name);
    LobbyRequestMapInsertResult res;
    LobbyCallbackRequest *h = NewHandler(callback, param);
    LobbyRequestMap_Insert(&res, &m->requestMap, &h);
    u32 id = GetId(h, IdTag(), IdTag());
    {
        Str s;
        LobbyString_InitFromCStr(&s, name);
        Str t = LobbyChat_GetFullChannelName(GetMgr(), s);
        Str u;
        LobbyString_InitFromCStr(&u, t.c_str());
        LobbyChat_AddChannel(GetMgr(), u, channelCallbacks, limit);
        LobbyString_Destroy(&u);
        LobbyString_Destroy(&t);
        LobbyString_Destroy(&s);
    }
    MI_CpuFill8(&cb, 0, sizeof(cb));
    cb.message = LobbyChat_OnChannelMessage;
    cb.modeChanged = LobbyChat_OnChannelModeChanged;
    cb.kicked = LobbyChat_OnKicked;
    cb.topicChanged = LobbyChat_OnTopicChanged;
    cb.userLeft = LobbyChat_OnUserParted;
    cb.userJoined = LobbyChat_OnUserJoined;
    cb.usersChanged = LobbyChat_OnUserListUpdated;
    cb.userModeChanged = LobbyChat_OnUserModeChanged;
    cb.nickChanged = LobbyChat_OnUserChangedNick;
    cb.broadcastKeyChanged = LobbyChat_OnBroadcastKeyChanged;
    cb.param = channelCallbacks->param;
    Str s2;
    LobbyString_InitFromCStr(&s2, name);
    Str t2 = LobbyChat_GetFullChannelName(GetMgr(), s2);
    chatEnterChannelA(GetMgr()->connection, t2.c_str(), password, &cb, LobbyChat_OnEnterChannel, id, 0);
    LobbyString_Destroy(&t2);
    LobbyString_Destroy(&s2);
    return 0;
}

#pragma opt_common_subs reset
