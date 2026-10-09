#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#include <string.h>
using std::memcmp;
typedef struct RTCDate {
    u32 year, month, day, week;
} RTCDate;
typedef struct RTCTime {
    u32 hour, minute, second;
} RTCTime;
extern "C" s64 RTC_ConvertDateTimeToSecond(const RTCDate *date, const RTCTime *time);
extern "C" void ov00_021ECB94(RTCDate *date, RTCTime *time);

struct TrueType {};
struct FalseType {};
template <class T, class U> inline void Assign(T &d, const U &s, TrueType, FalseType) {
    d = s;
}
template <class T, class U> inline void Assign(T &d, const U &s) {
    Assign(d, s, TrueType(), FalseType());
}

struct Str {
    u32 words[3];
    Str() {
        u32 *p = words;
        u32 i;
        for (i = 0; i < 3; i++) {
            p[i] = 0;
        }
    }
};
extern "C" void ov45_02237BC8(Str *s);
extern "C" void ov45_02237D48(Str *dst, const Str *src);

typedef struct Vec {
    void *ptr;
    u32 size, cap;
} Vec;
extern "C" void *ov45_02231C78(Vec *v);

static inline s64 GetNow(void) {
    RTCDate date;
    RTCTime time;
    ov00_021ECB94(&date, &time);
    return RTC_ConvertDateTimeToSecond(&date, &time);
}

struct Pair {
    s64 v;
    Pair() {
    }
    Pair(s64 x) : v(x) {
    }
};
struct Stamp {
    u32 code;
    Pair pair;
    u32 extra;
    Stamp(u32 c) : code(c), pair(0), extra(0) {
        Pair *d = &pair;
        *d = Pair(GetNow());
    }
    Stamp(const Stamp &o) : code(o.code) {
        Pair *d = &pair;
        *d = o.pair;
        extra = o.extra;
    }
    Stamp &operator=(const Stamp &o) {
        code = o.code;
        pair = o.pair;
        extra = o.extra;
        return *this;
    }
    bool operator==(const Stamp &o) const {
        return memcmp(this, &o, sizeof(Stamp)) == 0;
    }
};
struct Tracked {
    Stamp stamp;
    int active;
    Tracked(u32 c, int a) : stamp(Stamp(c)), active(a) {
    }
    void Set(const Tracked &t) {
        const Stamp *s = &t.stamp;
        if (!(stamp == *s)) {
            stamp = *s;
            active = 1;
        }
    }
};

struct Task {
    s64 time;
    s64 period;
    void *arg;
    int (*func)(void);
    Task() {
    }
    Task(s64 t, u32 p, int (*f)(void)) {
        time = t;
        period = p;
        arg = NULL;
        func = f;
        time -= period;
    }
};
template <class A, class B> struct KV {
    A first;
    B second;
    template <class C, class D> KV(const KV<C, D> &p) {
        Assign(first, p.first);
        const D *src = &p.second;
        B *dst = &second;
        *dst = *src;
    }
};
struct InsertResult {
    void *node;
    bool inserted;
};
struct Map;
extern "C" void ov45_02236BA8(InsertResult *out, Map *m, const KV<u32, Task> *v);
struct Map {
    u32 words[4];
    InsertResult insert(const KV<u32, Task> &v) {
        InsertResult r;
        ov45_02236BA8(&r, this, &v);
        return r;
    }
};
extern "C" KV<int, Task> ov45_02232AE8(int k, const Task &t);

struct Arguments {
    s64 time;
    u32 code;
};

struct Sub {
    void *vt;
    u32 tree[4];
    Tracked tracked;
    Vec vec;
    u8 pad34[0x3c - 0x34];
    u32 key;
    int flag;
    void SetFlag(int f) {
        flag = 0;
        if (f) {
            flag = 1;
        }
    }
};

struct Callbacks {
    void *cb8;
    void (*notify)(u32 key, Arguments *args, void *data, u32 size);
};


struct Peer {
    int kind;
    Str name;
    int a;
    int b;
    void Reset() {
        Str empty;
        kind = 5;
        ov45_02237D48(&name, &empty);
        a = 0;
        b = 0;
        ov45_02237BC8(&empty);
    }
};

typedef struct Lobby {
    int state;
    int err;
    Callbacks cbs;
    u8 pad10[0x70 - 0x10];
    int retry;
    u8 pad74[0xb8 - 0x74];
    Peer peer;
    u8 padd0[0xe8 - 0xd0];
    Sub sub;
    u8 pad12c[0x134 - 0x12c];
    Map timers;
} Lobby;

extern "C" Lobby *_022577C4;
extern "C" void ov45_02241B84(void *handle, void (*cb)(void), u32 code);
extern "C" void ov45_02236690(void);
extern "C" int ov45_022347C4(void);
extern "C" int ov45_02234764(void);
extern "C" int ov45_022348C8(void);
extern "C" int ov45_02234870(void);
extern "C" int ov45_02234944(void);

extern "C" void ov45_022354F4(int success, void *unused, void *handle, u32 code);

static inline void Lobby_ResetStamp(void) {
    _022577C4->retry = 0;
    Tracked t(5, 1);
    Sub *sub = &_022577C4->sub;
    sub->tracked.Set(t);
    sub->SetFlag(1);
}

static inline void Lobby_AddTimer(int kind, u32 delay, int (*cb)(void)) {
    Map *m = &_022577C4->timers;
    Task task(GetNow(), delay, cb);
    KV<int, Task> d = ov45_02232AE8(kind, task);
    m->insert(d);
}

static inline Arguments Lobby_GetArgs(void) {
    const Stamp &s = _022577C4->sub.tracked.stamp;
    Arguments a;
    a.code = s.code;
    a.time = s.pair.v;
    return a;
}

static inline void Lobby_Notify(Arguments *args) {
    Callbacks *cbs = &_022577C4->cbs;
    Sub *sub = &_022577C4->sub;
    u32 key = sub->key;
    cbs->notify(key, args, ov45_02231C78(&sub->vec), sub->vec.size);
}

static inline int Lobby_Retry(Lobby *w) {
    if (w->retry >= 3) {
        return 0;
    }
    w->retry++;
    return 1;
}

static inline void Lobby_SetError(int code) {
    Lobby *w = _022577C4;
    if (code != 0) {
        w->err = code;
        w->state = 10;
    }
}

static inline void Lobby_OnFail(void) {
    _022577C4->peer.Reset();
    if (Lobby_Retry(_022577C4)) {
        _022577C4->state = 4;
    } else {
        Lobby_SetError(3);
    }
}

// @fn ov45_022354F4
extern "C" void ov45_022354F4(int success, void *unused, void *handle, u32 code) {
    if (code == 5) {
        return;
    }
    if (success) {
        Lobby_ResetStamp();
        ov45_02241B84(handle, ov45_02236690, code);
        Lobby_AddTimer(1, 3, ov45_022347C4);
        Lobby_AddTimer(2, 4, ov45_02234764);
        Lobby_AddTimer(5, 4, ov45_022348C8);
        Lobby_AddTimer(4, 3, ov45_02234870);
        Lobby_AddTimer(6, 4, ov45_02234944);
        Arguments args = Lobby_GetArgs();
        Lobby_Notify(&args);
    } else {
        Lobby_OnFail();
    }
}
