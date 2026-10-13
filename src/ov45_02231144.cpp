#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <size_t.h>

struct Date {
    u32 year, month, day, week;
};

struct Time {
    u32 hour, minute, second;
};

extern "C" void DWC_GetDateTime(Date *, Time *);
extern "C" s64 RTC_ConvertDateTimeToSecond(const Date *, const Time *);
extern "C" void *LobbyMem_Alloc(u32);
extern "C" void DWC_Free(int, void *, int);
extern "C" char ov45_02254FA0[], ov45_02254FB0[];
extern "C" void PPWLobby_OnPlayerMessage();
extern "C" void PPWLobby_OnExcessFlood();
extern "C" void ov45_02238B08(void *);
extern "C" void ov45_0223770C(void *);
extern "C" void ov45_02236AE8(void *);
extern "C" void ov45_0223775C(void *);
extern "C" void LobbyTimerMap_Clear(void *);
extern "C" void LobbyTimerMap_Destroy(void *);
extern "C" void ov45_0223DF4C(void *);
extern "C" void LobbyCharVector_Destroy(void *);
extern "C" void ov45_02236B88(void *);
extern "C" void LobbyString_Destroy(void *);
extern "C" void ov45_0223E100(void *, u32, u32, u32);
extern "C" void PPWLobby_StartCheckProfile(void);

struct Callbacks {
    void (*connected)();
    void (*error)();
    int kind;

    Callbacks(void (*c)(), void (*e)(), int k)
        : connected(c)
        , error(e)
        , kind(k) {
    }
};

extern "C" int LobbyChat_Initialize(u32, u32, u32, Callbacks *, u32);

inline void *operator new(std::size_t, void *p) {
    return p;
}

struct Config {
    u32 words[17];
};

struct Str {
    u32 words[3];

    Str() {
        u32 i;
        u32 *p = words;
        for (i = 0; i < 3; i++) {
            p[i] = 0;
        }
    }

    void Destroy() {
        LobbyString_Destroy(this);
    }
};

struct Peer {
    int kind;
    Str name;
    u32 a, b;

    Peer()
        : kind(5) {
    }
};

struct List {
    u32 size;
    void *head;
    void **tail;

    List()
        : size(0)
        , head(0)
        , tail(&head) {
    }
};

struct Info {
    int id;
    s64 mid;
    int z;

    Info()
        : id(5)
        , mid(0)
        , z(0) {
        Time time;
        Date date;
        DWC_GetDateTime(&date, &time);
        mid = RTC_ConvertDateTimeToSecond(&date, &time);
    }
};

struct Vec {
    char *ptr;
    u32 size, cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }
};

struct Sessions {
    const void *vtbl;
    List list;
    const void *alloc;
    Info info;
    int valid;
    Vec field;
    int dirty;
    int pad120;
    int last;

    Sessions()
        : vtbl(ov45_02254FA0)
        , alloc(ov45_02254FB0)
        , valid(1)
        , dirty(1)
        , last(-1) {
    }

    void Destroy() {
        vtbl = ov45_02254FA0;
        ov45_0223DF4C(this);
        LobbyCharVector_Destroy(&field);
        ov45_02236B88(&list);
    }
};

struct Tree {
    u32 size;
    void *root;
    void *unk;
    void **leftmost;

    Tree()
        : size(0)
        , root(0)
        , leftmost(&root) {
    }
};

struct Peers {
    Peer a;
    Peer b;
};

struct Work {
    int state;
    int sub;
    Config cfg;
    u32 arg4;
    u32 arg2;
    u32 arg7;
    Str s58;
    u32 w64, w68, w6c, w70;
    u8 pad74[0xb8 - 0x74];
    Peers peers;
    Sessions sessions;
    u8 pad128[0x134 - 0x128];
    Tree tree;
    u8 b144[0x170 - 0x144];
    u8 b170[0x17c - 0x170];
    u8 b17c[0x194 - 0x17c];
    u8 b194[0x47c - 0x194];

    Work(u32 a2, const Config &c, u32 a4, u32 a7)
        : state(0)
        , sub(0)
        , cfg(c)
        , arg4(a4)
        , arg2(a2)
        , arg7(a7)
        , w64(0)
        , w68(0)
        , w6c(0)
        , w70(0) {
        ov45_02238B08(b144);
    }

    void Destroy() {
        ov45_0223770C(b194);
        ov45_02236AE8(b17c);
        ov45_0223775C(b170);
        LobbyTimerMap_Clear(&tree);
        LobbyTimerMap_Destroy(&tree);
        sessions.Destroy();
        peers.b.name.Destroy();
        peers.a.name.Destroy();
        s58.Destroy();
    }
};

extern "C" Work *_022577C4;

inline void Free(void *p) {
    if (p) {
        DWC_Free(0, p, 0);
    }
}

inline void Delete(Work *w) {
    if (w) {
        w->Destroy();
        Free(w);
    }
}

class ErrorMap {
public:
    int Map(int r) const;
};

inline int ErrorMap::Map(int r) const {
    int state = 0;
    switch (r) {
    case 1:
        state = 2;
        break;
    case 2:
        state = 3;
        break;
    }
    return state;
}

extern "C" int ov45_02231144(u32 a0, u32 a1, u32 a2, const Config *cfg, u32 a4, u32 a5, u32 a6, u32 a7);

extern "C" int ov45_02231144(u32 a0, u32 a1, u32 a2, const Config *cfg, u32 a4, u32 a5, u32 a6, u32 a7) {
    if (_022577C4 != 0) {
        return 4;
    }
    Callbacks cbs(PPWLobby_OnPlayerMessage, PPWLobby_OnExcessFlood, 5);
    _022577C4 = new (LobbyMem_Alloc(sizeof(Work))) Work(a2, *cfg, a4, a7);
    if (_022577C4 == 0) {
        return 1;
    }
    int result = LobbyChat_Initialize(a0, a1, a2, &cbs, a4);
    {
        Work *g = _022577C4;
        int state = ErrorMap().Map(result);
        if (state != 0) {
            g->sub = state;
            g->state = 10;
        }
    }
    if (result != 0) {
        Work *w = _022577C4;
        if (w != 0) {
            Delete(w);
            _022577C4 = 0;
        }
        return result;
    }
    ov45_0223E100(&_022577C4->sessions, a4, a5, a6);
    PPWLobby_StartCheckProfile();
    _022577C4->state = 1;
    return 0;
}
