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
extern "C" Str ov45_02231EB0(Work *);
extern "C" Str ov45_02233994(int, const Str &);

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
extern "C" void ov45_02235C40(void), ov45_02235E4C(void), ov45_02236090(void), ov45_022362D0(void), ov45_02236A44(void), ov45_02235A94(void);

struct Callbacks {
    void (*f0)(void), (*f1)(void), (*f2)(void), (*f3)(void), (*f4)(void);
    int mode;
};

extern "C" int ov45_02241048(const char *, int, void *, Callbacks *, void (*)(void), int);

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
    Str n = ov45_02233994(mode, t);
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
    cb.f0 = ov45_02235C40;
    cb.f1 = ov45_02235E4C;
    cb.f2 = ov45_02236090;
    cb.f3 = ov45_022362D0;
    cb.f4 = ov45_02236A44;
    cb.mode = mode;
    Str name = Build(mode, ov45_02231EB0(w));
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
    int result = ov45_02241048(name.c_str(), 0, t->ctx, &cb, ov45_02235A94, mode);
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
