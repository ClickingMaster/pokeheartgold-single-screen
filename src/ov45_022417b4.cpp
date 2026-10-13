#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
union Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" void LobbyString_AssignCStr(Str *s, const char *p);
extern "C" void LobbyString_Assign(Str *dst, const Str *src);
extern "C" int LobbyString_CompareCStr(const Str *s, const char *p);

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

    void assign(const char *p) {
        LobbyString_AssignCStr(this, p);
    }

    void assign(const Str &o) {
        LobbyString_Assign(this, &o);
    }

    bool equals(const char *p) const {
        return LobbyString_CompareCStr(this, p) == 0;
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

extern "C" HandlerVtbl ov45_02255238, ov45_02255268;
extern "C" const u32 ov45_022577E0;

struct Mgr {
    void *unk0;
    u32 state;
    u8 pad[0x88];
    u8 list[1];
};

extern "C" Mgr *ov45_022577DC;
extern "C" void *LobbyMem_Alloc(u32);

struct Tag {};

struct InsertResult {
    void *it;
    bool ok;

    InsertResult() {
    }
};

extern "C" void ov45_0224206C(InsertResult *out, void *list, Handler **h);
extern "C" Str ov45_022428C8(Mgr *m, const Str &s);
extern "C" void chatGetChannelKeysA(void *ctx, const char *name, const char *msg, int x, const char **desc, void (*cb)(), u32 id, u32 z);
extern "C" void ov45_02240494();
extern "C" unsigned long strlen(const char *);
extern "C" const char ov45_02255304[];
extern "C" Str ov45_0224367C(Mgr *m, int idx);

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
        p->vtbl = &ov45_02255268;
        p->a = a;
    }
    return p;
}

extern "C" int ov45_022417B4(const char *name, int idx, const char *desc, u32 a, u32 b);

struct IdTag {};

inline u32 GetId(Handler *h, IdTag, IdTag) {
    return h->vtbl->getId(h);
}

#pragma opt_common_subs off

extern "C" int ov45_022417B4(const char *name, int idx, const char *desc, u32 a, u32 b) {
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
    if (strlen(desc) + 1 > 20) {
        return 3;
    }
    Str msg(ov45_02255304);
    if (idx == -1) {
        msg.assign(ov45_02255304);
    } else {
        Str tmp = ov45_0224367C(GetMgr(), idx);
        msg.assign(tmp);
        tmp.destroy();
        if (msg.equals(ov45_02255304)) {
            msg.destroy();
            return 3;
        }
    }
    m = GetMgr();
    InsertResult res;
    Handler *h = NewHandler(a, b);
    ov45_0224206C(&res, m->list, &h);
    u32 id = GetId(h, IdTag(), IdTag());
    Str s(name);
    Str t = ov45_022428C8(GetMgr(), s);
    chatGetChannelKeysA(GetMgr()->unk0, t.c_str(), msg.c_str(), 1, &desc, ov45_02240494, id, 0);
    t.destroy();
    s.destroy();
    msg.destroy();
    return 0;
}

#pragma opt_common_subs reset
