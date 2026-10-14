#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct String {
    u32 w[3];
};
struct Handler;

struct HandlerVtbl {
    void *f0, *f4, *f8;
    u32 (*getArg)(Handler *h);
};

extern "C" HandlerVtbl gLobbyRequestVtable;

struct Base {
    HandlerVtbl *vtbl;
    u32 id;
    u32 unk8;
};

struct Handler {
    virtual void f0();
    virtual void f4();
    virtual void f8();
    virtual u32 getArg();
    u32 id;
    u32 unk8;
    void (*callback)(BOOL ok, u32 a, const char *str, u32 b, u32 c, u32 extra);
};

struct Node {
    Node *l, *r, *p;
    Handler *value;
};

struct It {
    Node *n;

    It() {
    }

    It(Node *q)
        : n(q) {
    }
};

struct Map {
    u32 size;
    Node header;

    It end() {
        return It(&header);
    }
};

struct Manager {
    u8 pad[0x4];
    u32 state;
    u8 pad2[0x88];
    Map map;
};

extern "C" Manager *gLobbyChatManager;
extern "C" void LobbyChat_StripChannelPrefixes(String *dst, Manager *mgr, u32 a, u32 b);
extern "C" It LobbyRequestMap_Find(Map *map, Base *const &key);
extern "C" const char *LobbyU32Vector_Data(String *s);
extern "C" void LobbyRequestMap_DeleteKey(Map *map, u32 id);
extern "C" void LobbyU32Vector_Destroy(String *s);

inline void KeyInit(Base *k, u32 id) {
    k->vtbl = &gLobbyRequestVtable;
    k->id = id;
    k->unk8 = 0;
}

inline It FindIt(Manager *m, u32 id) {
    Base key;
    KeyInit(&key, id);
    Base *kp = &key;
    return LobbyRequestMap_Find(&m->map, kp);
}

extern "C" void LobbyChat_OnEnumChannels(void *self, int result, u32 a, u32 d, u32 b, u32 c, u32 id);
#pragma push
#pragma opt_propagation off

extern "C" void LobbyChat_OnEnumChannels(void *self, int result, u32 a, u32 d, u32 b, u32 c, u32 id) {
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == 5) {
        return;
    }
    String str;
    LobbyChat_StripChannelPrefixes(&str, gLobbyChatManager, a, d);
    int sid = id;
    Manager *m = gLobbyChatManager;
    It it = FindIt(m, sid);
    Handler *h;
    if (it.n == &m->map.header) {
        h = 0;
    } else {
        Node *nd = it.n;
        h = nd->value;
    }
    void (*cb)(BOOL, u32, const char *, u32, u32, u32) = h->callback;
    cb(result == 1, a, LobbyU32Vector_Data(&str), b, c, h->getArg());
    LobbyRequestMap_DeleteKey(&gLobbyChatManager->map, sid);
    LobbyU32Vector_Destroy(&str);
}

#pragma pop
