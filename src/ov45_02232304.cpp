#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
class Str;
extern "C" Str *ov45_02237BC8(Str *);
extern "C" Str *ov45_02237A24(Str *, const Str *);
extern "C" int ov45_02237FD8(const Str *, const char *);

class Str {
public:
    union {
        struct {
            u32 flag : 1;
            u32 capacity : 31;
            u32 size;
            char *data;
        } l;

        struct {
            u8 flag : 1;
            u8 size : 7;
            char data[11];
        } s;

        u32 words[3];
    } raw;

    Str(const Str &o) {
        ov45_02237A24(this, &o);
    }

    void Destroy() {
        ov45_02237BC8(this);
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }

    bool operator==(const char *t) const {
        return ov45_02237FD8(this, t) == 0;
    }
};

struct Work {
    int state;
    int sub;
    u8 pad8[0xb8 - 8];
    u8 names[4];
};

extern "C" Work *_022577C4;
extern "C" char ov45_022550B4[];
extern "C" Str ov45_02232434(u8 *, int);
extern "C" int ov45_022417B4(const char *, int, int, void (*)(), int);
extern "C" void ov45_02236308();
extern "C" int ov45_02232304(int, int, int, int);

inline bool IsState(int s) {
    return _022577C4->state == s;
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

#pragma force_active on
#pragma opt_common_subs off

extern "C" int ov45_02232304(int id, int key, int mode, int arg) {
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
    {
        Str name = ov45_02232434(w->names, key);
        if (name == ov45_022550B4) {
            name.Destroy();
            return 5;
        }
        int result = ov45_022417B4(name.CStr(), id, mode, ov45_02236308, arg);
        {
            Work *g = _022577C4;
            int state = ErrorMap().Map(result);
            if (state != 0) {
                g->sub = state;
                g->state = 10;
            }
        }
        name.Destroy();
        return result;
    }
}

#pragma opt_common_subs reset
