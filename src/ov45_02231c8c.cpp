#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
struct Str;
extern "C" void LobbyString_Destroy(Str *);
extern "C" void LobbyString_InitCopy(Str *, const Str *);
extern "C" void LobbyString_Assign(Str *, const Str *);

struct Str {
    union {
        struct {
            u32 flag : 1;
            u32 len : 31;
        };

        u8 raw[4];
    };

    u32 cap;
    char *ptr;

    const char *c_str() const {
        return (flag == 0) ? (const char *)&raw[1] : ptr;
    }

    Str() {
    }

    Str(const Str &o) {
        LobbyString_InitCopy(this, &o);
    }

    void Destroy() const {
        LobbyString_Destroy((Str *)this);
    }
};
struct Work;
extern "C" Str PPWLobby_GetChannelBaseName(Work *);
extern "C" Str PPWLobby_MakeChannelName(int, const Str &);

struct Result {
    int mode;
    Str text;
    int a, b;

    Result(int m, const Str &s)
        : mode(m) {
        Result *r = (Result *)(void *)this;
        LobbyString_InitCopy(&text, &s);
        r->a = 0;
        r->b = 0;
    }
};

struct Work {
    int state, error;
    u8 pad8[0xd0 - 8];
    int mode;
    Str text;
    int a, b;
};

extern "C" Work *_022577C4;

struct Table {
    u8 pad[0x1c];
    void *ctx;
};

extern "C" Table ov45_02254D5C, ov45_02254D1C, ov45_02254D3C;
extern "C" void PPWLobby_OnPlayerJoined(void), PPWLobby_OnPlayerLeft(void), PPWLobby_OnMessage(void), PPWLobby_OnChannelDataChanged(void), PPWLobby_OnTopicChanged(void), PPWLobby_OnEnterSubChannel(void);

struct Callbacks {
    void (*f0)(void), (*f1)(void), (*f2)(void), (*f3)(void), (*f4)(void);
    int mode;
};

extern "C" int LobbyChat_EnterChannelAsync(const char *, int, void *, Callbacks *, void (*)(void), int);

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

inline void SetResult(const Result &r) {
    Work *g = _022577C4;
    g->mode = r.mode;
    LobbyString_Assign(&g->text, &r.text);
    g->a = r.a;
    g->b = r.b;
    r.text.Destroy();
}

inline Str Build(int mode, const Str &t) {
    Str n = PPWLobby_MakeChannelName(mode, t);
    t.Destroy();
    return n;
}

extern "C" int PPW_LobbyJoinSubChannelAsync(int mode);
#pragma opt_common_subs off

extern "C" int PPW_LobbyJoinSubChannelAsync(int mode) {
    Work *w = _022577C4;
    if (w == 0) {
        return 4;
    }
    if (w != 0 && w->state == 10) {
        return 7;
    }
    if (w != 0 && _022577C4->state != 8) {
        return 4;
    }
    if (w != 0 && (w->mode != 5 ? 1 : 0) != 0) {
        return 4;
    }
    Callbacks cb;
    cb.f0 = PPWLobby_OnPlayerJoined;
    cb.f1 = PPWLobby_OnPlayerLeft;
    cb.f2 = PPWLobby_OnMessage;
    cb.f3 = PPWLobby_OnChannelDataChanged;
    cb.f4 = PPWLobby_OnTopicChanged;
    cb.mode = mode;
    Str name = Build(mode, PPWLobby_GetChannelBaseName(w));
    Table *t;
    switch (mode) {
    case 1:
    case 2:
        t = &ov45_02254D5C;
        break;
    case 3:
        t = &ov45_02254D1C;
        break;
    case 4:
        t = &ov45_02254D3C;
        break;
    default:
        t = 0;
        break;
    }
    int result = LobbyChat_EnterChannelAsync(name.c_str(), 0, t->ctx, &cb, PPWLobby_OnEnterSubChannel, mode);
    {
        Work *g = _022577C4;
        int state = ErrorMap().Map(result);
        if (state != 0) {
            g->error = state;
            g->state = 10;
        }
    }
    if (result != 0) {
        name.Destroy();
        return result;
    }
    SetResult(Result(mode, name));
    name.Destroy();
    return 0;
}

#pragma opt_common_subs reset
