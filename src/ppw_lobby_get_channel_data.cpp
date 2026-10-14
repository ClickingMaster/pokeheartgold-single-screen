#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
class Str;
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" Str *LobbyString_InitCopy(Str *, const Str *);
extern "C" int LobbyString_CompareCStr(const Str *, const char *);

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
        LobbyString_InitCopy(this, &o);
    }

    void Destroy() {
        LobbyString_Destroy(this);
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }

    bool operator==(const char *t) const {
        return LobbyString_CompareCStr(this, t) == 0;
    }
};

struct Work {
    int state;
    int sub;
    u8 pad8[0xb8 - 8];
    u8 names[4];
};

extern "C" Work *gPPWLobbyWork;
extern "C" char sStr_Empty_15[];
extern "C" Str PPWLobby_GetChannelName(u8 *, int);
extern "C" int LobbyChat_GetChannelKeysAsync(const char *, int, int, void (*)(), int);
extern "C" void PPWLobby_OnGetChannelKeys();
extern "C" int PPW_LobbyGetChannelDataAsync(int, int, int, int);

inline bool IsState(int s) {
    return gPPWLobbyWork->state == s;
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

extern "C" int PPW_LobbyGetChannelDataAsync(int id, int key, int mode, int arg) {
    Work *w = gPPWLobbyWork;
    if (w == 0) {
        return 4;
    }
    if (w != 0 && w->state == 10) {
        return 7;
    }
    if (w != 0 && gPPWLobbyWork->state != 8) {
        return 4;
    }
    {
        Str name = PPWLobby_GetChannelName(w->names, key);
        if (name == sStr_Empty_15) {
            name.Destroy();
            return 5;
        }
        int result = LobbyChat_GetChannelKeysAsync(name.CStr(), id, mode, PPWLobby_OnGetChannelKeys, arg);
        {
            Work *g = gPPWLobbyWork;
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
