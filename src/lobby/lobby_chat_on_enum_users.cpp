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

extern "C" Vec LobbyChat_AddChannelUsers(Manager *map, int count, const char **names, const String &key);
extern "C" void LobbyChat_OnEnumUsers(void *chat, int result, const char *name, int count, const char **nicks, u32 modes, u32 id);
#pragma push
#pragma opt_propagation off

extern "C" void LobbyChat_OnEnumUsers(void *chat, int result, const char *name, int count, const char **nicks, u32 modes, u32 id) {
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == 5) {
        return;
    }
    Vec ids;
    if (count != 0) {
        String s;
        LobbyString_InitFromCStr(&s, name);
        Vec tmp = LobbyChat_AddChannelUsers(gLobbyChatManager, count, nicks, s);
        ids = tmp;
        tmp.destroy();
        LobbyString_Destroy(&s);
    }
    int sid = id;
    Manager *m = gLobbyChatManager;
    LobbyRequestMapIterator it = FindIt(m, sid);
    LobbyEnumUsersRequest *h;
    if (it.node == &m->requestMap.header) {
        h = 0;
    } else {
        LobbyRequestMapNode *nd = it.node;
        h = static_cast<LobbyEnumUsersRequest *>(nd->request);
    }
    String t;
    String k;
    LobbyString_InitFromCStr(&t, name);
    LobbyChat_StripChannelPrefix(&k, gLobbyChatManager, &t);
    u32 n = ids.count();
    const char *str = k.c_str();
    LobbyEnumUsersCallback cb = h->callback;
    cb(result == 1, str, n, LobbyU32Vector_Data2(&ids), h->getParam());
    LobbyString_Destroy(&k);
    LobbyString_Destroy(&t);
    LobbyRequestMap_DeleteKey(&gLobbyChatManager->requestMap, sid);
    ids.destroy();
}

#pragma pop
