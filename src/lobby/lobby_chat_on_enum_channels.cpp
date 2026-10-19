#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_manager.h"
#include "lobby_vector.h"

extern "C" void LobbyChat_StripChannelPrefixes(LobbyU32Vector *dst, LobbyChatManager *mgr, u32 a, u32 b);
extern "C" const char *LobbyU32Vector_Data(LobbyU32Vector *s);
extern "C" void LobbyU32Vector_Destroy(LobbyU32Vector *s);

inline void KeyInit(LobbyRequest *k, u32 id) {
    k->vtable = &gLobbyRequestVtable;
    k->id = id;
    k->param = 0;
}

inline LobbyRequestMapIterator FindIt(LobbyChatManager *m, u32 id) {
    LobbyRequest key;
    KeyInit(&key, id);
    LobbyRequest *kp = &key;
    return LobbyRequestMap_Find(&m->requestMap, kp);
}

extern "C" void LobbyChat_OnEnumChannels(void *chat, int result, u32 count, u32 channels, u32 topics, u32 userCounts, u32 id);
#pragma push
#pragma opt_propagation off

extern "C" void LobbyChat_OnEnumChannels(void *chat, int result, u32 count, u32 channels, u32 topics, u32 userCounts, u32 id) {
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == 5) {
        return;
    }
    LobbyU32Vector str;
    LobbyChat_StripChannelPrefixes(&str, gLobbyChatManager, count, channels);
    int sid = id;
    LobbyChatManager *m = gLobbyChatManager;
    LobbyRequestMapIterator it = FindIt(m, sid);
    LobbyEnumChannelsRequest *h;
    if (it.node == (LobbyRequestMapNode *)&m->requestMap.root) {
        h = 0;
    } else {
        LobbyRequestMapNode *nd = it.node;
        h = (LobbyEnumChannelsRequest *)nd->request;
    }
    LobbyEnumChannelsCallback cb = h->callback;
    cb(result == 1, count, LobbyU32Vector_Data(&str), topics, userCounts, h->getParam());
    LobbyRequestMap_DeleteKey(&gLobbyChatManager->requestMap, sid);
    LobbyU32Vector_Destroy(&str);
}

#pragma pop
