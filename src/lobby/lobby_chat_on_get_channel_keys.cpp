#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_manager.h"

extern "C" void LobbyString_InitFromCStr(Str *s, const char *t);
extern "C" void LobbyString_Destroy(Str *s);

extern "C" int LobbyChat_FindUserIdByNick(LobbyChatManager *m, Str *s);
extern "C" void LobbyChat_StripChannelPrefix(Str *dst, LobbyChatManager *m, Str *s);

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

extern "C" void LobbyChat_OnGetChannelKeys(void *chat, int result, const char *name, const char *user, int count, u32 *keys, u32 *values, u32 id);
#pragma push
#pragma opt_propagation off

extern "C" void LobbyChat_OnGetChannelKeys(void *chat, int result, const char *name, const char *user, int count, u32 *keys, u32 *values, u32 id) {
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == 5) {
        return;
    }
    if (name == NULL) {
        return;
    }
    int userId;
    if (user == NULL) {
        userId = -1;
    } else {
        Str s;
        LobbyString_InitFromCStr(&s, user);
        userId = LobbyChat_FindUserIdByNick(gLobbyChatManager, &s);
        LobbyString_Destroy(&s);
    }
    int sid = id;
    u32 key, value;
    LobbyGetChannelKeysRequest *h;
    LobbyChatManager *m = gLobbyChatManager;
    LobbyRequestMapIterator it = FindIt(m, sid);
    if (it.node == (LobbyRequestMapNode *)&m->requestMap.root) {
        h = 0;
    } else {
        LobbyRequestMapNode *nd = it.node;
        h = (LobbyGetChannelKeysRequest *)nd->request;
    }
    key = 0;
    value = key;
    if (result == 1 && count > 0) {
        key = *keys;
        value = *values;
    }
    Str t;
    LobbyString_InitFromCStr(&t, name);
    Str k;
    LobbyChat_StripChannelPrefix(&k, gLobbyChatManager, &t);
    const char *str = k.c_str();
    LobbyGetChannelKeysCallback cb = h->callback;
    cb(result == 1, str, userId, key, value, h->getParam());
    LobbyString_Destroy(&k);
    LobbyString_Destroy(&t);
    LobbyRequestMap_DeleteKey(&gLobbyChatManager->requestMap, sid);
}

#pragma pop
