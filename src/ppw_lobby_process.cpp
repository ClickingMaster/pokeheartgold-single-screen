#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
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
extern "C" void MI_CpuCopy8(const void *src, void *dst, u32 size);

typedef union Str {
    struct {
        u32 large : 1;
        u32 cap : 31;
    } bits;

    struct {
        u8 tag;
        char data[11];
    } s;

    struct {
        u32 cap;
        u32 size;
        char *ptr;
    } l;
} Str;

typedef struct Vec {
    void *ptr;
    u32 size, cap;
} Vec;

typedef struct Iterator {
    void *current;
} Iterator;

typedef struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *first;
} Tree;

typedef struct Timer {
    u32 link[4];
    s64 last;
    s64 interval;
    void *arg;
    int (*fn)(void *);
} Timer;

typedef struct Pair {
    s64 v;
} Pair;

struct Stamp {
    u32 code;
    Pair pair;
    u32 extra;

    Stamp(u32 c, const Pair &p, u32 e)
        : code(c) {
        Pair *d = &pair;
        *d = p;
        extra = e;
    }

    Stamp(const Stamp &o)
        : code(o.code) {
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

    Tracked(u32 c, const Pair &p, u32 e, int a)
        : stamp(Stamp(c, p, e))
        , active(a) {
    }

    void Set(const Tracked &t) {
        const Stamp *s = &t.stamp;
        if (!(stamp == *s)) {
            stamp = *s;
            active = 1;
        }
    }
};

typedef struct Request {
    u32 unk0;
    int retries;
    int mode;
    Str url;
    Str post;
    int len24;
    Vec body;
    void (*fn)(int, void *, u32, void *);
    void *arg;
    int alt;
} Request;

typedef struct Deletable Deletable;

typedef struct DeletableVtbl {
    void *unk0;
    void (*destroy)(Deletable *);
} DeletableVtbl;

struct Deletable {
    DeletableVtbl *vt;
};

struct Sub {
    void *vt;
    Tree tree;
    Tracked tracked;
    Vec vec;
    u8 pad34[0x40 - 0x34];
    int flag;
};

typedef struct Lobby {
    int state;
    int err;
    void (*cb)(int, int);
    u8 pad0c[0x58 - 0xc];
    Str s58;
    u8 pad64[0xbc - 0x64];
    Str sbc;
    u8 padc8[0xd4 - 0xc8];
    Str sd4;
    u8 pade0[0xe8 - 0xe0];
    Sub sub;
    u8 pad12c[0x134 - 0x12c];
    Tree timers;
    int x144;
    u8 pad148[0x170 - 0x148];
    Vec vec170;
    u8 pad17c[0x194 - 0x17c];
    Vec vec194;

    int GetState() const {
        return state;
    }
} Lobby;

extern "C" Request *ov45_022577D4;
extern "C" Deletable *ov45_022577D8;
extern "C" Lobby *_022577C4;
extern "C" u8 ov45_02254FA0[];
extern "C" char ov45_022550B0[];

extern "C" int StatsUpload_Process(void);
extern "C" void *StatsUpload_GetResponseBody(u32 *len);
extern "C" void *DWC_Alloc(int heap, u32 size);
extern "C" void DWC_Free(int heap, void *p, int unk);
extern "C" void DWC_ClearError(void);
extern "C" void StatsUpload_Stop(void);
extern "C" void StatsUpload_Start(int mode, char *s);
extern "C" void *ov45_02231C78(Vec *v);
extern "C" int StatsUpload_UploadWithLastHash(char *a, int b, void *c, u32 d, int e);
extern "C" int StatsUpload_Upload(char *a, int b, void *c, u32 d, int e);
extern "C" int ov45_02240874(void);
extern "C" int PPW_LobbyShutdownAsync(void);
extern "C" Vec *ov45_0223770C(Vec *);
extern "C" void *ov45_02236AE8(void *);
extern "C" Vec *ov45_0223775C(Vec *);
extern "C" void ov45_02236CB0(Tree *);
extern "C" void *ov45_02236E38(Tree *);
extern "C" void ov45_0223DF4C(void *);
extern "C" Vec *ov45_022379D4(Vec *);
extern "C" void *ov45_02236B88(Tree *);
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" int ov45_02240A58(void);
extern "C" int DWC_GetDateTime(RTCDate *date, RTCTime *time);
extern "C" void ov45_02236E58(Iterator *it);
extern "C" void ov45_02236C40(Iterator *result, Tree *t, Iterator node);
extern "C" void ov45_0223E558(void *);
extern "C" void ov45_0223AF28(void *);
extern "C" Str *LobbyString_InitFromCStr(Str *s, char *text);
extern "C" void ov45_02233994(Str *out, u32 id, Str *s);
extern "C" void ov45_0223633C(void);
extern "C" void ov45_02241A20(char *s, void (*cb)(void), int x);
extern "C" int ov45_02233CA0(void);
extern "C" int ov45_02234688(void);

extern "C" int PPW_LobbyProcess(void);

static inline char *Str_CStr(Str *s) {
    return !s->bits.large ? s->s.data : s->l.ptr;
}

static inline void Request_Finish(Request *r, int ok, void *buf, u32 len) {
    void *arg = r->arg;
    Request *tmp;
    StatsUpload_Stop();
    tmp = (Request *)ov45_022577D8;
    ov45_022577D8 = (Deletable *)ov45_022577D4;
    ov45_022577D4 = tmp;
    r->fn(ok, buf, len, arg);
}

static inline void Request_Succeed(Request *r, void *buf, const u32 &size) {
    void *arg = r->arg;
    u32 len = size;
    Request *tmp;
    StatsUpload_Stop();
    tmp = (Request *)ov45_022577D8;
    ov45_022577D8 = (Deletable *)ov45_022577D4;
    ov45_022577D4 = tmp;
    r->fn(1, buf, len, arg);
}

static inline int Request_Send(Request *r) {
    int ret;
    StatsUpload_Stop();
    StatsUpload_Start(r->mode, Str_CStr(&r->url));
    if (r->alt) {
        u32 n = r->body.size;
        ret = StatsUpload_UploadWithLastHash(Str_CStr(&r->post), r->len24, ov45_02231C78(&r->body), n, 0);
    } else {
        u32 n = r->body.size;
        ret = StatsUpload_Upload(Str_CStr(&r->post), r->len24, ov45_02231C78(&r->body), n, 0);
    }
    if (ret == 0) {
        return 1;
    }
    StatsUpload_Stop();
    return 0;
}

static inline int Request_Poll(Request *r) {
    u32 len = 0;
    switch (StatsUpload_Process()) {
    case 8: {
        void *data = StatsUpload_GetResponseBody(&len);
        void *buf = DWC_Alloc(0, len);
        if (buf != NULL) {
            MI_CpuCopy8(data, buf, len);
        }
        if (buf != NULL) {
            Request_Succeed(r, buf, len);
        } else {
            Request_Finish(r, 0, NULL, 0);
        }
        if (buf != NULL) {
            DWC_Free(0, buf, 0);
        }
        return 1;
    }
    case 1:
        DWC_ClearError();
        if (--r->retries != 0) {
            if (Request_Send(r)) {
                return 0;
            }
        }
        Request_Finish(r, 0, NULL, 0);
        return 1;
    case 0:
        DWC_ClearError();
        Request_Finish(r, 0, NULL, 0);
        return 1;
    }
    return 0;
}

static inline void Deletable_Delete(Deletable *d) {
    if (d != NULL) {
        d->vt->destroy(d);
    }
}

static inline int Request_Update(void) {
    Request *r = ov45_022577D4;
    if (r == NULL) {
        return 0;
    }
    if (Request_Poll(r) && ov45_022577D8 != NULL) {
        Deletable_Delete(ov45_022577D8);
        ov45_022577D8 = NULL;
    }
    return 1;
}

static inline void Lobby_Destroy(Lobby *w) {
    ov45_0223770C(&w->vec194);
    ov45_02236AE8((u8 *)w + 0x17c);
    ov45_0223775C(&w->vec170);
    ov45_02236CB0(&w->timers);
    ov45_02236E38(&w->timers);
    w->sub.vt = ov45_02254FA0;
    ov45_0223DF4C(&w->sub.vt);
    ov45_022379D4(&w->sub.vec);
    ov45_02236B88(&w->sub.tree);
    LobbyString_Destroy(&w->sd4);
    LobbyString_Destroy(&w->sbc);
    LobbyString_Destroy(&w->s58);
}

static inline void Lobby_Delete(Lobby *w) {
    if (w != NULL) {
        Lobby_Destroy(w);
        if (w != NULL) {
            DWC_Free(0, w, 0);
        }
    }
}

static inline void Lobby_SetError(int code) {
    Lobby *w = _022577C4;
    if (code != 0) {
        w->err = code;
        w->state = 10;
    }
}

static inline bool Lobby_IsError(void) {
    return _022577C4 != NULL && _022577C4->GetState() == 10;
}

static inline int Timer_Update(Timer *t, s64 now) {
    if (now - t->last > t->interval) {
        t->last = now;
        if (!t->fn(t->arg)) {
            return 0;
        }
    }
    return 1;
}

static inline Iterator Iterator_Make(void *p) {
    Iterator t;
    t.current = p;
    return t;
}

static inline void Lobby_UpdateTimers(void) {
    Lobby *w = _022577C4;
    RTCTime time;
    RTCDate date;
    s64 now;
    void *end;
    Iterator it;
    DWC_GetDateTime(&date, &time);
    now = RTC_ConvertDateTimeToSecond(&date, &time);
    it.current = w->timers.first;
    end = &w->timers.root;
    while (it.current != end) {
        if (!Timer_Update((Timer *)it.current, now)) {
            void *saved = it.current;
            Iterator res;
            ov45_02236E58(&it);
            ov45_02236C40(&res, &w->timers, Iterator_Make(saved));
        } else {
            ov45_02236E58(&it);
        }
    }
    ov45_0223E558(&w->sub.vt);
    ov45_0223AF28(&w->x144);
}

static inline int Lobby_IsReady(Lobby *w) {
    if (w->x144 >= 2) {
        return 1;
    }
    return 0;
}

static inline void Lobby_Login(void) {
    Str name;
    Str query;
    LobbyString_InitFromCStr(&name, ov45_022550B0);
    ov45_02233994(&query, 0, &name);
    ov45_02241A20(Str_CStr(&query), ov45_0223633C, 0);
    LobbyString_Destroy(&query);
    LobbyString_Destroy(&name);
}

static inline void Lobby_Fail(int r) {
    Lobby *w = _022577C4;
    int code;
    switch (r) {
    case 1:
        code = 2;
        break;
    case 2:
        code = 3;
        break;
    default:
        code = 0;
        break;
    }
    if (code != 0) {
        w->err = code;
        w->state = 10;
    }
}

static inline int Lobby_Process(void) {
    switch (ov45_02240874()) {
    case 0:
        PPW_LobbyShutdownAsync();
        if (_022577C4 != NULL) {
            Lobby_Delete(_022577C4);
            _022577C4 = NULL;
        }
        return 9;
    case 3:
        if (_022577C4->state == 1) {
            _022577C4->state = 2;
        }
        break;
    case 5:
        Lobby_SetError(ov45_02240A58());
        break;
    case 1:
    case 2:
    case 4:
        break;
    }
    Lobby_UpdateTimers();
    switch (_022577C4->state) {
    case 0:
        return 9;
    case 2:
        _022577C4->state = 3;
        break;
    case 3:
        if (Lobby_IsReady(_022577C4)) {
            _022577C4->state = 4;
        }
        break;
    case 4: {
        Lobby_Login();
        _022577C4->state = 5;
        break;
    }
    case 6: {
        int r = ov45_02233CA0();
        if (r != 0) {
            Lobby_Fail(r);
        } else {
            _022577C4->state = 7;
        }
        break;
    }
    case 7:
        if (ov45_02234688()) {
            Sub *sub = &_022577C4->sub;
            sub->tracked.active = 0;
            Tracked t(sub->tracked.stamp.code, sub->tracked.stamp.pair, 1, 1);
            sub->tracked.Set(t);
            sub->flag = 1;
            _022577C4->state = 8;
            _022577C4->cb(1, 0);
        }
        break;
    case 1:
    case 5:
    case 8:
    case 9:
    case 10:
        break;
    }
    return _022577C4->state;
}

int PPW_LobbyProcess(void) {
    int polled = Request_Update();
    if (polled) {
        if (_022577C4 == NULL) {
            return 9;
        }
    } else if (_022577C4 == NULL) {
        return 0;
    }
    if (Lobby_IsError()) {
        return 10;
    }
    return Lobby_Process();
}
