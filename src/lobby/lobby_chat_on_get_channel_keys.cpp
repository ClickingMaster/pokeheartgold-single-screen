#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_request.h"

union String {
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

    u32 w[3];

    const char *c_str() const {
        return !l.flag ? s.data : l.data;
    }
};

extern "C" void LobbyString_InitFromCStr(String *s, const char *t);
extern "C" void LobbyString_Destroy(String *s);

struct Manager {
    u8 pad[0x4];
    u32 state;
    u8 pad2[0x88];
    LobbyRequestMap requestMap;
};

extern "C" Manager *gLobbyChatManager;
extern "C" int LobbyChat_FindUserIdByNick(Manager *m, String *s);
extern "C" void LobbyChat_StripChannelPrefix(String *dst, Manager *m, String *s);

inline void KeyInit(LobbyRequest *k, u32 id) {
    k->vtable = &gLobbyRequestVtable;
    k->id = id;
    k->param = 0;
}

inline LobbyRequestMapIterator FindIt(Manager *m, u32 id) {
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
        String s;
        LobbyString_InitFromCStr(&s, user);
        userId = LobbyChat_FindUserIdByNick(gLobbyChatManager, &s);
        LobbyString_Destroy(&s);
    }
    int sid = id;
    u32 key, value;
    LobbyGetChannelKeysRequest *h;
    Manager *m = gLobbyChatManager;
    LobbyRequestMapIterator it = FindIt(m, sid);
    if (it.node == &m->requestMap.header) {
        h = 0;
    } else {
        LobbyRequestMapNode *nd = it.node;
        h = static_cast<LobbyGetChannelKeysRequest *>(nd->request);
    }
    key = 0;
    value = key;
    if (result == 1 && count > 0) {
        key = *keys;
        value = *values;
    }
    String t;
    LobbyString_InitFromCStr(&t, name);
    String k;
    LobbyChat_StripChannelPrefix(&k, gLobbyChatManager, &t);
    const char *str = k.c_str();
    LobbyGetChannelKeysCallback cb = h->callback;
    cb(result == 1, str, userId, key, value, h->getParam());
    LobbyString_Destroy(&k);
    LobbyString_Destroy(&t);
    LobbyRequestMap_DeleteKey(&gLobbyChatManager->requestMap, sid);
}

#pragma pop
