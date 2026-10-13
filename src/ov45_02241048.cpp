#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern "C" unsigned long strlen(const char *);
extern "C" void MI_CpuFill8(void *, u8, u32);
union Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);
extern "C" Str *LobbyString_Destroy(Str *);

union Str {
    struct L {
        u32 flag : 1;
        u32 capacity : 31;
        u32 size;
        char *data;
    } l;

    struct S {
        u8 flag : 1;
        u8 size : 7;
        char data[11];
    } s;

    u32 words[3];

    Str(const char *p) {
        LobbyString_InitFromCStr(this, p);
    }

    void destroy() {
        LobbyString_Destroy(this);
    }

    const char *c_str() const {
        return !l.flag ? s.data : l.data;
    }
};
struct Handler;

struct HandlerVtbl {
    void *f0, *f4;
    u32 (*getId)(Handler *);
};

struct Handler {
    HandlerVtbl *vtbl;
    u32 id;
    u32 b;
    u32 a;
};

extern "C" HandlerVtbl ov45_02255238, ov45_02255280;
extern "C" const u32 ov45_022577E0;

struct Mgr {
    void *unk0;
    u32 state;
    u8 pad[0x88];
    u8 list[1];
};

extern "C" Mgr *ov45_022577DC;
extern "C" void *LobbyMem_Alloc(u32);

struct InsertResult {
    void *it;
    bool ok;

    InsertResult() {
    }
};

extern "C" void ov45_0224206C(InsertResult *res, void *list, Handler **h);
extern "C" Str ov45_022428C8(Mgr *m, const Str &s);

struct Arg3 {
    u8 pad[0x14];
    u32 unk14;
};

extern "C" void ov45_02242ACC(Mgr *m, const Str &s, Arg3 *c, u32 d);

struct Callbacks {
    void (*f0)();
    void (*f4)();
    void (*f8)();
    void (*fC)();
    void (*f10)();
    void (*f14)();
    void (*f18)();
    void (*f1C)();
    void (*f20)();
    void (*f24)();
    void (*f28)();
    u32 f2C;
};

extern "C" void chatEnterChannelA(void *ctx, const char *name, u32 b, Callbacks *cb, void (*cb2)(), u32 id, u32 z);
extern "C" void ov45_0223F1E4();
extern "C" void ov45_0223F2D8();
extern "C" void ov45_0223F32C();
extern "C" void ov45_0223F5C8();
extern "C" void ov45_0223F6F4();
extern "C" void ov45_0223F768();
extern "C" void ov45_0223F83C();
extern "C" void ov45_0223F8F4();
extern "C" void ov45_0223F910();
extern "C" void ov45_02240234();
extern "C" void ov45_02240620();

inline Mgr *GetMgr() {
    return ov45_022577DC;
}

inline void BaseInit(Handler *h, u32 b) {
    h->vtbl = &ov45_02255238;
    h->id = 0;
    h->b = b;
}

// The id counter is only ever read/written here; it is declared const so that
// stores through `this` are known not to alias it (native schedules the load
// above the base-part stores).
inline u32 NextHandlerId() {
    u32 n = ov45_022577E0;
    *(u32 *)&ov45_022577E0 = n + 1;
    return n;
}

inline Handler *NewHandler(u32 a, u32 b) {
    Handler *p = (Handler *)LobbyMem_Alloc(sizeof(Handler));
    if (p) {
        BaseInit(p, b);
        p->id = NextHandlerId();
        p->vtbl = &ov45_02255280;
        p->a = a;
    }
    return p;
}

struct IdTag {};

inline u32 GetId(Handler *h, IdTag, IdTag) {
    return h->vtbl->getId(h);
}

extern "C" int ov45_02241048(const char *name, u32 p1, u32 p2, Arg3 *p3, u32 a, u32 b);
#pragma opt_common_subs off

extern "C" int ov45_02241048(const char *name, u32 p1, u32 p2, Arg3 *p3, u32 a, u32 b) {
    Callbacks cb;
    Mgr *m = ov45_022577DC;
    if (m == NULL) {
        return 4;
    }
    if (m != NULL && m->state == 5) {
        return 7;
    }
    if (m != NULL && GetMgr()->state != 3) {
        return 4;
    }
    strlen(name);
    InsertResult res;
    Handler *h = NewHandler(a, b);
    ov45_0224206C(&res, m->list, &h);
    u32 id = GetId(h, IdTag(), IdTag());
    {
        Str s(name);
        Str t = ov45_022428C8(GetMgr(), s);
        Str u(t.c_str());
        ov45_02242ACC(GetMgr(), u, p3, p2);
        u.destroy();
        t.destroy();
        s.destroy();
    }
    MI_CpuFill8(&cb, 0, sizeof(cb));
    cb.f0 = ov45_0223F1E4;
    cb.f18 = ov45_0223F8F4;
    cb.f4 = ov45_0223F2D8;
    cb.f14 = ov45_0223F83C;
    cb.fC = ov45_0223F5C8;
    cb.f8 = ov45_0223F32C;
    cb.f20 = ov45_0223F910;
    cb.f1C = ov45_0223F768;
    cb.f10 = ov45_0223F6F4;
    cb.f28 = ov45_02240620;
    cb.f2C = p3->unk14;
    Str s2(name);
    Str t2 = ov45_022428C8(GetMgr(), s2);
    chatEnterChannelA(GetMgr()->unk0, t2.c_str(), p1, &cb, ov45_02240234, id, 0);
    t2.destroy();
    s2.destroy();
    return 0;
}

#pragma opt_common_subs reset
