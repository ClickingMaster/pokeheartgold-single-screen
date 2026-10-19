#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_manager.h"


extern "C" int LobbyChat_FindUserIdByNick(LobbyChatManager *m, Str *s);
extern "C" void LobbyChat_StripChannelPrefix(Str *dst, LobbyChatManager *m, Str *s);
extern "C" LobbyChannelCallbacks *LobbyChat_GetChannelCallbacks(LobbyChatManager *manager, Str *key);
extern "C" void LobbyChat_OnChannelModeChanged(void *chat, const char *name, u32 mode, u32 id);

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

struct Tag {};
struct Vec;
extern "C" void LobbyU32Vector_AssignRange(Vec *v, u32 *first, u32 *last, Tag t);
extern "C" void LobbyU32Vector_Destroy(Vec *v);
extern "C" void *LobbyU32Vector_Data2(Vec *v);

struct Vec {
    u32 *ptr;
    u32 size;
    u32 cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    Vec &operator=(const Vec &o) {
        if (this != &o) {
            LobbyU32Vector_AssignRange(this, o.ptr, o.ptr + o.size, Tag());
        }
        return *this;
    }

    void destroy() {
        LobbyU32Vector_Destroy(this);
    }

    u32 count() const {
        return size;
    }
};

extern "C" Vec LobbyChat_AddChannelUsers(LobbyChatManager *map, int count, const char **names, const Str &key);
extern "C" void LobbyChat_SetChannelUsersEnumerated(LobbyChatManager *m, Str *s);
extern "C" void LobbyChat_LeaveChannel(const char *s);
extern "C" void LobbyChatManager_GetNick(Str *dst, LobbyChatManager *m);
extern "C" void LobbyChat_AddChannelUser(LobbyChatManager *m, Str *a, Str *b);
extern "C" int LobbyChat_GetChannelUserLimit(LobbyChatManager *m, Str *s);
extern "C" int chatGetChannelNumUsersA(void *chat, const char *channel);
extern "C" void chatSetChannelLimitA(void *chat, const char *channel, int limit);
extern "C" void chatEnumUsersA(void *chat, const char *channel, void *callback, u32 param, u32 blocking);

extern "C" void LobbyChat_OnEnterChannelEnumUsers(void *chat, int result, const char *name, int count, const char **nicks, u32 modes, u32 id);
extern "C" void LobbyChat_OnEnterChannel(void *chat, int ok, int error, const char *name, u32 id);
extern "C" void LobbyChat_OnGetChannelKeys(void *chat, int result, const char *name, const char *user, int count, const char **keys, const char **values, u32 id);
extern "C" void LobbyChat_OnBroadcastKeyChanged(void *chat, char *channel, char *nick, void *key, void *value);
extern "C" void LobbyChat_OnGetChannelMode(void *chat, int result, const char *name, u32 mode, u32 id);

#pragma push
#pragma opt_propagation off

extern "C" void LobbyChat_OnEnterChannelEnumUsers(void *chat, int result, const char *name, int count, const char **nicks, u32 modes, u32 id) {
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    if (result == 1) {
        Str s;
        LobbyString_InitFromCStr(&s, name);
        Vec tmp = LobbyChat_AddChannelUsers(gLobbyChatManager, count, nicks, s);
        tmp.destroy();
        LobbyString_Destroy(&s);
    }
    {
        Str u;
        LobbyString_InitFromCStr(&u, name);
        LobbyChat_SetChannelUsersEnumerated(gLobbyChatManager, &u);
        LobbyString_Destroy(&u);
    }
    int sid = id;
    LobbyChatManager *m = gLobbyChatManager;
    LobbyRequestMapIterator it = FindIt(m, sid);
    LobbyEnterChannelRequest *h;
    if (it.node == (LobbyRequestMapNode *)&m->requestMap.root) {
        h = 0;
    } else {
        LobbyRequestMapNode *nd = it.node;
        h = (LobbyEnterChannelRequest *)nd->request;
    }
    Str t;
    LobbyString_InitFromCStr(&t, name);
    Str k;
    LobbyChat_StripChannelPrefix(&k, gLobbyChatManager, &t);
    LobbyEnterChannelCallback cb = h->callback;
    const char *str = k.c_str();
    cb(result == 1, 0, str, h->getParam());
    LobbyString_Destroy(&k);
    LobbyString_Destroy(&t);
    LobbyRequestMap_DeleteKey(&gLobbyChatManager->requestMap, sid);
    if (result != 0) {
        return;
    }
    Str a;
    LobbyString_InitFromCStr(&a, name);
    Str b;
    LobbyChat_StripChannelPrefix(&b, gLobbyChatManager, &a);
    LobbyChat_LeaveChannel(b.c_str());
    LobbyString_Destroy(&b);
    LobbyString_Destroy(&a);
}

#pragma pop

#pragma push
#pragma opt_propagation off

extern "C" void LobbyChat_OnEnterChannel(void *chat, int ok, int error, const char *name, u32 id) {
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    if (ok == 1) {
        Str s;
        LobbyString_InitFromCStr(&s, name);
        int userLimit = LobbyChat_GetChannelUserLimit(gLobbyChatManager, &s);
        LobbyString_Destroy(&s);
        if (userLimit != 0) {
            int numUsers = chatGetChannelNumUsersA(gLobbyChatManager->connection, name);
            if (userLimit == -1) {
                ok = 0;
                error = 7;
            } else if (numUsers > userLimit || numUsers == -1) {
                ok = 0;
                error = 2;
            } else {
                chatSetChannelLimitA(gLobbyChatManager->connection, name, userLimit);
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
    LobbyChatManager *m = gLobbyChatManager;
    LobbyRequestMapIterator it = FindIt(m, sid);
    LobbyEnterChannelRequest *h = it.node == (LobbyRequestMapNode *)&m->requestMap.root ? 0 : (LobbyEnterChannelRequest *)it.node->request;
    Str s;
    LobbyString_InitFromCStr(&s, name);
    Str t;
    LobbyChat_StripChannelPrefix(&t, gLobbyChatManager, &s);
    const char *str = t.c_str();
    LobbyEnterChannelCallback cb = h->callback;
    cb(0, error, str, h->getParam());
    LobbyString_Destroy(&t);
    LobbyString_Destroy(&s);
    LobbyRequestMap_DeleteKey(&gLobbyChatManager->requestMap, sid);
}

#pragma pop

#pragma push
#pragma opt_propagation off

extern "C" void LobbyChat_OnGetChannelKeys(void *chat, int result, const char *name, const char *user, int count, const char **keys, const char **values, u32 id) {
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
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
    const char *key, *value;
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

// A key without a user is a channel key; it is reported with user ID -1.
extern "C" void LobbyChat_OnBroadcastKeyChanged(void *chat, char *channel, char *nick, void *key, void *value) {
    Str nickStr;
    Str channelKey;
    Str fullName;
    Str channelName;
    int userId;
    LobbyChannelCallbacks *callbacks;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    if (!nick) {
        userId = -1;
    } else {
        LobbyString_InitFromCStr(&nickStr, nick);
        userId = LobbyChat_FindUserIdByNick(gLobbyChatManager, &nickStr);
        LobbyString_Destroy(&nickStr);
    }
    LobbyString_InitFromCStr(&channelKey, channel);
    callbacks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &channelKey);
    LobbyString_Destroy(&channelKey);
    LobbyString_InitFromCStr(&fullName, channel);
    LobbyChat_StripChannelPrefix(&channelName, gLobbyChatManager, &fullName);
    callbacks->onBroadcastKeyChanged(channelName.c_str(), userId, key, value, callbacks->param);
    LobbyString_Destroy(&channelName);
    LobbyString_Destroy(&fullName);
}

#pragma push
#pragma opt_propagation off

extern "C" void LobbyChat_OnGetChannelMode(void *chat, int result, const char *name, u32 mode, u32 id) {
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
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
