#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_manager.h"
#include "lobby_string.h"

extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" void LobbyChat_StripChannelPrefix(Str *dst, LobbyChatManager *m, Str *src);
extern "C" void LobbyChat_OnChannelModeChanged(void *self, const char *name, u32 x, u32 id);

inline void KeyInit(LobbyRequest *k, u32 id) {
    k->vtable = &gLobbyRequestVtable;
    k->id = id;
    k->param = 0;
}

extern "C" void LobbyChat_OnGetChannelMode(void *chat, int result, const char *name, u32 mode, u32 id);

inline LobbyRequestMapIterator FindIt(LobbyChatManager *m, u32 id) {
    LobbyRequest key;
    KeyInit(&key, id);
    LobbyRequest *kp = &key;
    return LobbyRequestMap_Find(&m->requestMap, kp);
}

#pragma push
#pragma opt_propagation off

extern "C" void LobbyChat_OnGetChannelMode(void *chat, int result, const char *name, u32 mode, u32 id) {
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == 5) {
        return;
    }
    if (result == 0) {
        return;
    }
    LobbyChat_OnChannelModeChanged(chat, name, mode, id);
    int sid = id;
    LobbyChatManager *m = gLobbyChatManager;
    LobbyRequestMapIterator it = FindIt(m, sid);
    LobbyGetChannelModeRequest *h = it.node == (LobbyRequestMapNode *)&m->requestMap.root ? 0 : (LobbyGetChannelModeRequest *)it.node->request;
    Str s;
    LobbyString_InitFromCStr(&s, name);
    Str t;
    LobbyChat_StripChannelPrefix(&t, gLobbyChatManager, &s);
    const char *str = t.c_str();
    LobbyGetChannelModeCallback cb = h->callback;
    cb(result == 1, str, mode, h->getParam());
    LobbyString_Destroy(&t);
    LobbyString_Destroy(&s);
    LobbyRequestMap_DeleteKey(&gLobbyChatManager->requestMap, sid);
}

#pragma pop
