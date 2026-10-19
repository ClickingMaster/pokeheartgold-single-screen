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
extern "C" void LobbyChat_OnEnumUsers(void *chat, int result, const char *name, int count, const char **nicks, u32 modes, u32 id);
#pragma push
#pragma opt_propagation off

extern "C" void LobbyChat_OnEnumUsers(void *chat, int result, const char *name, int count, const char **nicks, u32 modes, u32 id) {
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == 5) {
        return;
    }
    Vec ids;
    if (count != 0) {
        Str s;
        LobbyString_InitFromCStr(&s, name);
        Vec tmp = LobbyChat_AddChannelUsers(gLobbyChatManager, count, nicks, s);
        ids = tmp;
        tmp.destroy();
        LobbyString_Destroy(&s);
    }
    int sid = id;
    LobbyChatManager *m = gLobbyChatManager;
    LobbyRequestMapIterator it = FindIt(m, sid);
    LobbyEnumUsersRequest *h;
    if (it.node == (LobbyRequestMapNode *)&m->requestMap.root) {
        h = 0;
    } else {
        LobbyRequestMapNode *nd = it.node;
        h = (LobbyEnumUsersRequest *)nd->request;
    }
    Str t;
    Str k;
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
