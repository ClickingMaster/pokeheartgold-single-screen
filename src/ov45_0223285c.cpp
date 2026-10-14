#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" char *strncpy(char *, const char *, unsigned);
struct Str;
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" void LobbyString_Assign(Str *, const Str *);

struct Str {
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

    Str() {
    }

    Str(const Str &o);

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }

    Str &operator=(const Str &o) {
        LobbyString_Assign(this, &o);
        return *this;
    }
};

struct Date {
    u32 year, month, day, week;
};

struct Time {
    u32 hour, minute, second;
};

struct User {
    u32 a, b, c, d;
    u8 key[32];
    char name[20];
};

struct Info {
    s64 time;
    s64 period;
    u32 z;
    void (*cb)();

    Info() {
    }

    Info(s64 now, s64 d, void (*c)())
        : time(now)
        , period(d)
        , z(0)
        , cb(c) {
        time -= d;
    }
};

template <class A, class B>
struct Pair {
    A first;
    B second;

    Pair(const A &a, const B &b)
        : first(a)
        , second(b) {
    }

    template <class C, class D>
    Pair(const Pair<C, D> &o)
        : first(o.first) {
        const Info *s = &o.second;
        Info *d = &second;
        *d = *s;
    }
};

typedef Pair<u32, Info> RawEntry;
typedef Pair<const u32, Info> Entry;

struct Result {
    void *it;
    int ok;
};

struct Container {
    u8 pad[4];
};

struct Work {
    int state;
    u8 pad04[0x4c - 4];
    u32 code;
    u8 pad50[0x68 - 0x50];
    int busy;
    u8 pad6c[0x74 - 0x6c];
    User user;
    u8 padb8[0x134 - 0xb8];
    Container entries;
};

extern "C" Work *gPPWLobbyWork;
extern "C" char sFmt_M_IntUnderscoreUint[];
extern "C" void PPWLobby_TimerSendRecruitInfo();
extern "C" u32 current_time(void);
extern "C" Str LobbyString_FormatN(int, const char *, ...);
extern "C" void DWC_GetDateTime(Date *, Time *);
extern "C" s64 RTC_ConvertDateTimeToSecond(const Date *, const Time *);
extern "C" RawEntry LobbyTimerMap_MakeEntry(int, const Info &);
extern "C" Result LobbyTimerMap_Insert(Container *, const Entry &);
extern "C" int PPW_LobbyStartRecruit(User *u);

struct Tag {};

inline Entry Conv2(Tag, const RawEntry &r) {
    return Entry(r);
}

inline Entry Conv(Tag, const RawEntry &r) {
    return Conv2(Tag(), r);
}

inline Result Insert2(Container *c, const Entry &e) {
    return LobbyTimerMap_Insert(c, e);
}

#pragma force_active on
#pragma opt_common_subs off

extern "C" int PPW_LobbyStartRecruit(User *u) {
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
    Str value;
    u32 *p = value.raw.words;
    for (u32 i = 0; i < 3; i++) {
        p[i] = 0;
    }
    int ok = 0;
    Work *g = gPPWLobbyWork;
    if (g->busy == 0) {
        g->busy = 1;
        g->user = *u;
        Str tmp = LobbyString_FormatN(20, sFmt_M_IntUnderscoreUint, g->code, current_time() % 1000);
        value = tmp;
        LobbyString_Destroy(&tmp);
        strncpy(g->user.name, value.CStr(), 19);
        ok = 1;
    }
    if (!ok) {
        LobbyString_Destroy(&value);
        return 4;
    }
    strncpy(u->name, value.CStr(), 19);
    Work *h = gPPWLobbyWork;
    Date date;
    Time time;
    DWC_GetDateTime(&date, &time);
    Insert2(&h->entries, Conv(Tag(), LobbyTimerMap_MakeEntry(3, Info(RTC_ConvertDateTimeToSecond(&date, &time), 21, PPWLobby_TimerSendRecruitInfo))));
    LobbyString_Destroy(&value);
    return 0;
}

#pragma opt_common_subs reset
