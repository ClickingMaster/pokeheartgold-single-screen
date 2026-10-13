// Overlay 45 (DWC lobby) translation unit PPW_LobbySubmitQuestionnaire..ov45_022383DF.
// One file because the original TU owned the -ipa-file pooled .data block at _02254F58
// (4 one-byte objects + 6 string pointers, MWCC pool order = heapsort by size); 35A0/4D68/2E60/4A88/50FC/6BA8/8104
// address those objects through the shared pool literal. Each former matched fragment keeps its own namespace.
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <size_t.h>
#include <stdarg.h>
#include <string.h>
using std::va_list;
extern "C" {
#include <nitro/os/printf.h>
}
#include <nitro/rtc.h>

#include "overlay45_format_buffer_private.h"
#include "overlay45_string_search.h"

#include "../lib/NitroDWC/src/tree-iterator4.h"
using std::memcmp;
using std::memmove;

inline void *operator new(std::size_t, void *p) {
    return p;
}

extern "C" char ov45_02255048[], ov45_02255024[], ov45_02255070[], ov45_02254FE4[], ov45_02254FC8[], ov45_02255004[];
#pragma explicit_zero_data on
extern "C" u8 ov45_02254F5A = 1;
extern "C" const char *ov45_02254F68 = ov45_02254FE4;
extern "C" const char *ov45_02254F6C = ov45_02254FC8;
extern "C" const char *ov45_02254F60 = ov45_02255024;
extern "C" const char *ov45_02254F5C = ov45_02255048;
extern "C" signed char ov45_02254F5B = 0;
extern "C" const char *ov45_02254F70 = ov45_02255004;
extern "C" const char *ov45_02254F64 = ov45_02255070;
extern "C" signed char ov45_02254F59 = 0;
extern "C" u8 _02254F58 = 0;
#pragma explicit_zero_data reset
// ---- formerly src/op90_to70_ov45_extra04_residual_9_r03.cpp
#pragma push

namespace F00 {
#pragma force_active on
extern "C" void OS_GetMacAddress(u8 *);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);

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
    } raw;

    Str(const char *c) {
        LobbyString_InitFromCStr(this, c);
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }
};

enum ReqMethod {
    REQ_METHOD_GET,
    REQ_METHOD_POST
};

struct TA {};

struct TB {};

struct TD {};

struct Iter {
    const u8 *p;
    u32 extra;

    Iter(const u8 *q)
        : p(q) {
    }
};
struct Vec;
extern "C" void ov45_022327B8(Vec *, Iter, Iter, TD);
extern "C" const u8 *LobbyByteVector_Data(const Vec *);

struct Vec {
    u8 *ptr;
    u32 size, cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void Assign(Iter first, Iter last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(Iter first, Iter last, const TA &, const TB &) {
        ov45_022327B8(this, first, last, TD());
    }
};

struct Info {
    int id;
    u16 one;
    u8 a;
    u8 b;
    u8 mac[6];
    u16 pad;
    int pad2;
};

struct Packet {
    Info info;
    u32 out0, out1;
    int mode;
    u32 d4;
    u32 d38;
    u16 d3c;
    u16 d3e;
};

struct Out {
    u32 a, b;
    u8 rest[0x2d0];
};
struct Op;

struct OpVtbl {
    void (*f0)(Op *);
    void (*del)(Op *);
};

extern "C" const OpVtbl ov45_02254FC0;
extern "C" void ov45_022354AC(void);

struct Op {
    const OpVtbl *vtbl;
    int kind;
    int flag;
    Str a;
    Str b;
    int id;
    Vec vec;
    void (*cb)(void);
    int x38;
    int x3c;

    Op(int f, const char *n, const char *h, int i, const u8 *first, const u8 *last)
        : vtbl(&ov45_02254FC0)
        , kind(3)
        , flag(f)
        , a(n)
        , b(h)
        , id(i)
        , vec()
        , cb(ov45_022354AC)
        , x38(0)
        , x3c(0) {
        vec.Assign(Iter(first), Iter(last));
    }
};

extern "C" void *LobbyMem_Alloc(u32);
extern "C" int ov45_0223AEDC(void *, void *);
extern "C" void StatsUpload_Start(int, const char *);
extern "C" int StatsUpload_UploadWithLastHash(const char *, int, const u8 *, u32, int);
extern "C" int StatsUpload_Upload(const char *, int, const u8 *, u32, int);
extern "C" void StatsUpload_Stop(void);

struct Data {
    u8 pad0[4];
    u32 x4;
    u8 pad8[0x38 - 8];
    u8 x38;
    u8 x39;
    u8 pad3a[2];
    u16 x3c;
    u8 x3e;
    u8 pad3f[3];
    u8 x42;
};

struct Work {
    int state;
    u8 pad4[0x4c - 4];
    int x4c;
    int x50;
    u8 pad54[0x6c - 0x54];
    int x6c;
    u8 pad70[0x110 - 0x70];
    Vec v110;
    u8 pad11c[0x144 - 0x11c];
    u8 x144[4];
};

extern "C" Work *_022577C4;
extern "C" Op *ov45_022577D4;

inline const Data *GetData(Work *w) {
    return (const Data *)LobbyByteVector_Data(&w->v110);
}

#define PNEW(o, a, b, c, d, e, f) new (o) Op(a, b, c, d, e, f)
extern "C" int PPW_LobbySubmitQuestionnaire(int mode);

extern "C" int PPW_LobbySubmitQuestionnaire(int mode) {
    const char *name;
    const char *host;
    Op *o;
    int x;
    ReqMethod flag;
    Work *w = _022577C4;
    if (w == 0) {
        return 4;
    }
    if (w != 0 && w->state == 10) {
        return 7;
    }
    if (mode < 0 || mode > 2) {
        return 3;
    }
    Out out;
    if (!ov45_0223AEDC(w->x144, &out) || _022577C4->x6c != 0) {
        return 4;
    }
    const Data *d = GetData(_022577C4);
    Packet p = { 0 };
    {
        Info info;
        const Data *d2 = GetData(_022577C4);
        info.id = _022577C4->x4c;
        info.one = 1;
        info.a = d2->x42;
        info.b = d2->x39;
        OS_GetMacAddress(info.mac);
        p.info = info;
    }
    p.out0 = out.a;
    p.out1 = out.b;
    p.mode = mode;
    p.d4 = d->x4;
    p.d38 = d->x38;
    p.d3c = d->x3c;
    p.d3e = d->x3e;
    x = _022577C4->x4c;
    host = ov45_02254F5C;
    name = ov45_02254F64;
    flag = _022577C4->x50 == 0 ? REQ_METHOD_GET : REQ_METHOD_POST;
    if (ov45_022577D4 == 0) {
        o = (Op *)LobbyMem_Alloc(sizeof(Op));
        PNEW(o, flag, name, host, x, (const u8 *)&p, (const u8 *)&p + sizeof(p));
        ov45_022577D4 = o;
        StatsUpload_Start(o->flag, o->a.CStr());
        int ret;
        if (o->x3c) {
            u32 n = o->vec.size;
            ret = StatsUpload_UploadWithLastHash(o->b.CStr(), o->id, LobbyByteVector_Data(&o->vec), n, 0);
        } else {
            u32 n = o->vec.size;
            ret = StatsUpload_Upload(o->b.CStr(), o->id, LobbyByteVector_Data(&o->vec), n, 0);
        }
        int ok;
        if (ret == 0) {
            ok = 1;
        } else {
            StatsUpload_Stop();
            ok = 0;
        }
        if (!ok) {
            if (ov45_022577D4 != 0) {
                Op *q = ov45_022577D4;
                if (q != 0) {
                    q->vtbl->del(q);
                }
                ov45_022577D4 = 0;
            }
        }
    }
    _022577C4->x6c = 1;
    return 0;
}

}

#pragma pop
#undef PNEW

// ---- formerly src/to70_ov45_extra04_run10.c
#pragma push

namespace F01 {
extern "C" {
#pragma force_active on
#pragma force_active on
#pragma warn_uninitializedvar off
#define V(p, o) (*(volatile u32 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))

typedef union S {
    struct {
        u32 flag : 1;
        u32 cap : 31;
        u32 len;
        char *ptr;
    } l;

    struct {
        u8 flag : 1;
        u8 len : 7;
        char data[11];
    } s;
} S;

typedef struct Small {
    u8 x;
} Small;

typedef struct Vec {
    char *data;
    u32 len, cap;
} Vec;

typedef struct Node {
    void *left, *right;
    u32 parent;
    u32 key;
} Node;

typedef struct Body {
    u64 a, b;
    u32 c, d;
} Body;

typedef struct Record {
    int type;
    u32 words[6];
} Record;

extern int DWC_CheckUserData(void *);
extern int DWC_CheckHasProfile(void *);
extern u8 *volatile _022577C4;
extern u8 ov45_02254FA0[];
extern void *LobbyString_InitCopy(void *, void *);
extern void *LobbyCharVector_Data(Vec *);
extern Node *LobbyTimerMap_LowerBound(u8 *, u32 *, Node *, Node *);
extern void LobbyTimerMap_Find(Node **, u8 *, u32 *);
extern int DWC_Free();
extern int DWC_GetDateTime();
extern int ov45_02231144();
extern int PPWLobby_SendMessage();
extern int PPWLobby_SetChannelData();
extern int ov45_02236B88();
extern int LobbyTimerMap_Clear();
extern int LobbyCharVector_Grow();
extern int ov45_02237820();
extern int ov45_02237890();
extern int LobbyCharVector_Destroy();
extern int LobbyString_InitFromCStr();
extern int LobbyString_Destroy();
extern int PPWLobbySchedule_Copy();
extern int ov45_0223DF4C();
extern int LobbyChat_ToErrorCode(int);
extern int LobbyChat_IsInChannel();
extern int LobbyChat_Shutdown();
u8 *PPWLobby_GetWork(void);
int PPW_LobbyInitializeAsync(void *a, void *b, void *c, void *d, u8 *user, void *e, void *g);
u8 *ov45_02231448(u8 *p);
u8 *ov45_02231460(u8 *p);
u8 *ov45_02231C38(u8 *p);
void *ov45_02231EB0(u8 *p, u8 *q);
void *ov45_022320B4(u8 *p, u8 *q);
int PPW_LobbyGetTimeInfo(u64 *out);
int PPW_LobbyGetSchedule(void *p, u32 *size);
int PPW_LobbySetChannelData(void *a, void *b, void *c, void *d, void *e);
int PPW_LobbySendChannelBinaryMessage(int a, void *b, void *c, void *d);
int PPW_LobbySendPlayerBinaryMessage(int a, void *b, void *c, void *d);
int PPW_LobbyGetMyUserId(void);
void ov45_02232760(Vec *p, char *first, char *last);
void LobbyTimerMap_MakeEntry(Record *p, int type, Body *q);
void LobbyString_FormatN(u8 *p, int n, char *fmt, ...);
int LobbyTimerMap_EraseKey(u8 *p, u32 *key);
void LobbyTimerMap_Find(Node **out, u8 *p, u32 *key);
Node *LobbyTimerMap_LowerBound(u8 *p, u32 *key, Node *n, Node *best);
int PPW_LobbyGetLastError(void);
int PPW_LobbyToErrorCode(int x);
int ov45_02233220(int x);
int PPW_LobbyShutdownAsync(void);
int PPW_LobbyStopRecruit(void);
int PPW_LobbyGetSubChannelState(void);
void ov45_02232434(S *p, u8 *q, int x);
int PPW_LobbyUpdateRecruitInfo(u8 *p);

int PPW_LobbyGetLastError(void) {
    u8 *p = _022577C4;
    if (p) {
        return W(p, 4);
    }
    return 0;
}

int PPW_LobbyToErrorCode(int x) {
    if (x >= 4) {
        return x + 0x297c;
    }
    return LobbyChat_ToErrorCode(x);
}

int ov45_02233220(int x) {
    switch (x) {
    case 0:
        return 0;
    case 1:
        return 2;
    case 2:
        return 3;
    default:
        return 1;
    }
}
}
}

#pragma pop
#undef V
#undef W
#undef P

// ---- formerly src/op90_to80_extra1435_residual_1_r01.cpp
#pragma push

namespace F02 {
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

    Str() {
        u32 i;
        u32 *p = raw.words;
        for (i = 0; i < 3; i++) {
            p[i] = 0;
        }
    }

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

extern "C" Work *_022577C4;
extern "C" char ov45_022550B4[];
extern "C" Str ov45_02232434(u8 *, int);
extern "C" int PPWLobby_EncodeMessage(int, int, int, int, int, Str *);
extern "C" int LobbyChat_SendUserMessage(int, const char *);
extern "C" int LobbyChat_SendChannelMessage(const char *, const char *);
extern "C" int PPWLobby_SendMessage(int, int, int, int, int, int, int);

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

inline void SetError(int r) {
    Work *g = _022577C4;
    int state = ErrorMap().Map(r);
    if (state != 0) {
        g->sub = state;
        g->state = 10;
    }
}

#pragma force_active on

extern "C" int PPWLobby_SendMessage(int id, int key, int a, int b, int c, int d, int e) {
    int result;
    if (id == -1 && key == 5) {
        return 3;
    }
    Str value;
    if (!PPWLobby_EncodeMessage(a, b, c, d, e, &value)) {
        value.Destroy();
        return 3;
    }
    if (key == 5) {
        result = LobbyChat_SendUserMessage(id, value.CStr());
        SetError(result);
        value.Destroy();
        return result;
    }
    Str name = ov45_02232434(_022577C4->names, key);
    if (name == ov45_022550B4) {
        name.Destroy();
        value.Destroy();
        return 5;
    }
    result = LobbyChat_SendChannelMessage(name.CStr(), value.CStr());
    SetError(result);
    name.Destroy();
    value.Destroy();
    return result;
}

}

#pragma pop

// ---- formerly src/to80_extra1435_run01.c
#pragma push

namespace F03 {
extern "C" {
#pragma force_active on
#pragma force_active on

typedef union {
    u32 words[3];

    struct {
        u32 heap : 1;
        u32 unused : 31;
    } bits;

    u8 bytes[12];
} String;

typedef struct {
    int state, error;
    u8 p[0xb8 - 8];
    u8 names[1];
} Global;

extern Global *_022577C4;
extern char ov45_022550B4[];
extern int PPWLobby_EncodeMessage(void *, void *, void *, void *, void *, String *), LobbyChat_SendUserMessage(int, char *), LobbyChat_SendChannelMessage(char *, char *), LobbyString_CompareCStr(String *, char *), LobbyChat_SetChannelKey(char *, int, int, void *);
extern void LobbyString_Destroy(String *), ov45_02232434(String *, void *, int), LobbyCharVector_Grow(String *, u32), LobbyCharVector_Destroy(String *);
extern void *LobbyCharVector_Data(String *);
extern int DWC_Base64Encode(void *, u32, void *, u32);

static inline void Zero(u32 *p) {
    u32 i, zero = 0;
    for (i = 0; i < 3; i++) {
        p[i] = zero;
    }
}

static inline char *Text(String *s) {
    if (s->bits.heap == 0) {
        return (char *)&s->bytes[1];
    } else {
        return (char *)s->words[2];
    }
}

static inline int Empty(String *s) {
    return LobbyString_CompareCStr(s, ov45_022550B4) == 0;
}

static inline void Status(int result) {
    volatile u8 z = 0;
    Global *g = _022577C4;
    int e = 0;
    (void)&z;
    switch (result) {
    case 1:
        e = 2;
        break;
    case 2:
        e = 3;
        break;
    }
    if (e) {
        g->error = e;
        g->state = 10;
    }
}

int PPWLobby_SendMessage(int, int, void *, void *, void *, void *, void *);
int PPWLobby_SetChannelData(int, int, int, void *, u32);

int PPWLobby_SetChannelData(int id, int key, int mode, void *data, u32 length) {
    String name, encoded;
    int result;
    char *text;
    ov45_02232434(&name, _022577C4->names, key);
    if (Empty(&name)) {
        LobbyString_Destroy(&name);
        return 5;
    }
    if (length > 300) {
        LobbyString_Destroy(&name);
        return 3;
    }
    encoded.words[0] = 0;
    encoded.words[1] = 0;
    encoded.words[2] = 0;
    LobbyCharVector_Grow(&encoded, (length + 2) / 3 * 4 + 1);
    {
        void *out = LobbyCharVector_Data(&encoded);
        int end = DWC_Base64Encode(data, length, out, encoded.words[1]);
        ((char *)encoded.words[0])[end] = 0;
    }
    text = Text(&name);
    result = LobbyChat_SetChannelKey(text, id, mode, LobbyCharVector_Data(&encoded));
    Status(result);
    LobbyCharVector_Destroy(&encoded);
    LobbyString_Destroy(&name);
    return result;
}
}
}

#pragma pop

// ---- formerly lib/NitroDWC/asm/to80_extra1435_residual_2.s
#pragma push

namespace F04 {
extern "C" void OS_GetMacAddress(u8 *);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);

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

    Str(const char *c) {
        LobbyString_InitFromCStr(this, c);
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }
};

enum ReqMethod {
    REQ_METHOD_GET,
    REQ_METHOD_POST
};

struct Al {};

struct TA {};

struct TB {};

struct TC {};

struct TD {};

struct Iter {
    const u8 *p;
    u32 extra;

    Iter(const u8 *q)
        : p(q) {
    }
};
struct Vec;
extern "C" void ov45_022327B8(Vec *, Iter, Iter, TD);
extern "C" const char *LobbyByteVector_Data(const void *);

struct Vec {
    char *ptr;
    u32 size, cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void Assign(Iter first, Iter last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(Iter first, Iter last, const TA &, const TB &) {
        ov45_022327B8(this, first, last, TD());
    }
};

typedef u64 W8;

struct B48 {
    u32 w[12];
};

struct Config {
    u32 a, b;
    u16 h08[8];
    u32 h18, h1C;
    u16 h20[6];
    u8 b2C[6];
    u8 b32[6];
    u8 b38, b39;
    u16 h3A, h3C;
    u8 b3E, b3F, b40, b41, b42, b43;
    W8 b44;
    u8 b4C[12];
    B48 b58;

    struct {
        u16 h88, h8A;
    } t1;

    struct {
        u32 h8C, h90;
    } t2;
};

struct Info {
    u32 x;
    u16 one;
    u8 a;
    u8 b;
    u8 mac[6];
    u8 rest[6];
    Config cfg;
};
struct Obj;

struct ObjVtbl {
    void (*f0)(Obj *);
    void (*del)(Obj *);
};

extern "C" const ObjVtbl ov45_02254FC0;
extern "C" void ov45_02234A88(int, int, int);

struct Obj {
    const ObjVtbl *vtbl;
    int kind;
    int flag;
    Str name;
    Str host;
    u32 x;
    Vec data;
    void (*cb)(int, int, int);
    int x38;
    int x3c;

    Obj(int f, const char *n, const char *h, u32 x_, const Info &info)
        : vtbl(&ov45_02254FC0)
        , kind(3)
        , flag(f)
        , name(n)
        , host(h)
        , x(x_)
        , data()
        , cb(ov45_02234A88)
        , x38(0)
        , x3c(0) {
        data.Assign(Iter((const u8 *)&info), Iter((const u8 *)&info + sizeof(Info)));
    }
};

struct Work {
    u8 pad0[0x4c];
    u32 x4c;
    u32 x50;
    u8 pad54[0x110 - 0x54];
    u8 field[4];
};

extern "C" Work *_022577C4;
extern "C" Obj *ov45_022577D4;
extern "C" void *LobbyMem_Alloc(u32);
extern "C" void StatsUpload_Start(int, const char *);
extern "C" int StatsUpload_UploadWithLastHash(const char *, u32, const char *, u32, int);
extern "C" int StatsUpload_Upload(const char *, u32, const char *, u32, int);
extern "C" void StatsUpload_Stop(int);
extern "C" void PPWLobby_StartCheckProfile(void);
#pragma force_active on

extern "C" void PPWLobby_StartCheckProfile(void) {
    Info info;
    const Config *c = (const Config *)LobbyByteVector_Data(_022577C4->field);
    const u8 *f = (const u8 *)LobbyByteVector_Data(_022577C4->field);
    const char *name;
    const char *host;
    Obj *o;
    u32 x;
    ReqMethod flag;
    info.x = _022577C4->x4c;
    info.one = 1;
    info.a = f[0x42];
    info.b = f[0x39];
    OS_GetMacAddress(info.mac);
    info.cfg = *c;
    x = _022577C4->x4c;
    host = ov45_02254F70;
    name = ov45_02254F64;
    flag = _022577C4->x50 == 0 ? REQ_METHOD_GET : REQ_METHOD_POST;
    if (ov45_022577D4 != 0) {
        return;
    }
    o = (Obj *)LobbyMem_Alloc(sizeof(Obj));
    new (o) Obj(flag, name, host, x, info);
    ov45_022577D4 = o;
    StatsUpload_Start(o->flag, o->name.CStr());
    int ret;
    if (o->x3c) {
        u32 n = o->data.size;
        ret = StatsUpload_UploadWithLastHash(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    } else {
        u32 n = o->data.size;
        ret = StatsUpload_Upload(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    }
    int ok;
    if (ret == 0) {
        ok = 1;
    } else {
        StatsUpload_Stop(ret);
        ok = 0;
    }
    if (ok) {
        return;
    }
    if (ov45_022577D4 != 0) {
        Obj *p = ov45_022577D4;
        if (p != 0) {
            p->vtbl->del(p);
        }
        ov45_022577D4 = 0;
    }
}

}

#pragma pop

// ---- formerly src/to70_ov45_extra16_run01.c
#pragma push

namespace F05 {
extern "C" {
#pragma force_active on
#pragma force_active on
#define I(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
extern void *_022577C4;
extern char *_02254D04[];
extern void ov45_022320B4(Str *, void *), LobbyString_Reserve(Str *, u32);
extern int LobbyString_Compare(Str *, Str *), LobbyChat_IsInChannel(const char *), ov45_0223E4A8(void *), PPWLobby_SetChannelData(u32, u32, const void *, const void *, u32);
extern void *LobbyByteVector_Data(void *);
extern u32 current_time(void);
extern void LobbyString_Format(Str *, const char *, ...);
extern void PPWLobby_SendMessage(u32, u32, u32, u32, u32, const void *, u32), LobbyChat_GetChannelModeAsync(const char *, void (*)(void), s32), LobbyChat_GetChannelKeysAsync(const char *, s32, const char *, void (*)(void), s32);
extern void PPWLobby_OnGetChannelMode(void), PPWLobby_OnGetChannelTime(void), PPWLobby_OnGetLobbyData(void), ov45_0223AF04(void *, void *);
extern Str *LobbyString_ConcatCStr(Str *, Str *, char *);
extern Str *LobbyString_Concat(Str *, Str *, Str *);
extern char ov45_022550B8[];
extern char ov45_022550BC[];
extern char ov45_022550C4[];
extern char ov45_022550D4[];
extern char ov45_022550E4[];
extern char ov45_022550F4[];
void ov45_02233994(Str *out, u32 id, Str *s);
void ov45_02233B74(Str *out);
int LobbyString_Equals(Str *a, Str *b);
int ov45_02234688(void);
int ov45_02234764(void);
int PPWLobby_TimerUploadSystemProfile(void);
int PPWLobby_TimerSendRecruitInfo(void);
int PPWLobby_TimerPollChannelMode(void);
int PPWLobby_TimerRequestChannelTime(void);
int PPWLobby_TimerRequestLobbyData(void);
int ov45_022349CC(u32 type);
int ov45_02234A24(int valid, u8 *data, u32 size, void **output, u32 *length);
void ov45_022353F0(int valid, u8 *data, u32 size);
void ov45_022354AC(int status);

#pragma warn_uninitializedvar off
extern Str *LobbyString_ReplaceRange(Str *, u32, u32, const char *, const char *, u8);

static inline u32 short_length(Str *p) {
    if (p->l.flag) {
        return p->l.size;
    }
    return p->s.size;
}

void ov45_02233994(Str *out, u32 id, Str *s) {
    Str b, a;
    LobbyString_InitFromCStr(&b, _02254D04[id]);
    LobbyString_ConcatCStr(&a, &b, ov45_022550B8);
    LobbyString_Concat(out, &a, s);
    LobbyString_Destroy(&a);
    LobbyString_Destroy(&b);
}
}
}

#pragma pop
#undef I
#undef P

// ---- formerly lib/NitroDWC/src/to91_ov45_string_concat200.cpp
#pragma push

namespace F06 {
#pragma force_active on

namespace DwcStringConcat200 {
    union Str {
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
    };

    class RangeDispatch {};

    typedef char ActualString12[sizeof(Str) == 12 ? 1 : -1];
    typedef char ActualCarrier1[sizeof(RangeDispatch) == 1 ? 1 : -1];
}

using namespace DwcStringConcat200;
extern "C" void LobbyString_Reserve(Str *, u32);
extern "C" Str *LobbyString_ReplaceRange(Str *, u32, u32, char *, char *, RangeDispatch);
extern "C" Str *LobbyString_Concat(Str *, Str *, Str *);

extern "C" Str *LobbyString_Concat(Str *out, Str *left, Str *right) {
    u32 i, n1, n2;
    char *p1, *p2;
    RangeDispatch first, second;
    for (i = 0; i < 3; i++) {
        out->words[i] = 0;
    }
    if (left->l.flag) {
        n1 = left->l.size;
        p1 = left->l.data;
    } else {
        n1 = left->s.size;
        p1 = left->s.data;
    }
    if (right->l.flag) {
        p2 = right->l.data;
        n2 = right->l.size;
    } else {
        p2 = right->s.data;
        n2 = right->s.size;
    }
    LobbyString_Reserve(out, n1 + n2);
    out = LobbyString_ReplaceRange(out, 0, 0, p1, p1 + n1, first);
    return LobbyString_ReplaceRange(out, n1, 0, p2, p2 + n2, second);
}

}

#pragma pop

// ---- formerly lib/NitroDWC/src/to91_ov45_cstring_concat184.cpp
#pragma push

namespace F07 {
#pragma force_active on

namespace DwcStringConcat200 {
    union Str {
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
    };

    class RangeDispatch {};

    typedef char ActualString12[sizeof(Str) == 12 ? 1 : -1];
    typedef char ActualCarrier1[sizeof(RangeDispatch) == 1 ? 1 : -1];
}

using namespace DwcStringConcat200;
extern "C" void LobbyString_Reserve(Str *, u32);
extern "C" Str *LobbyString_ReplaceRange(Str *, u32, u32, char *, char *, RangeDispatch);
extern "C" unsigned strlen(const char *);
extern "C" Str *LobbyString_ConcatCStr(Str *, Str *, char *);

extern "C" Str *LobbyString_ConcatCStr(Str *out, Str *left, char *right) {
    u32 i, n1;
    char *p1;
    RangeDispatch first, second;
    for (i = 0; i < 3; i++) {
        out->words[i] = 0;
    }
    const u32 n2 = strlen(right);
    if (left->l.flag) {
        p1 = left->l.data;
        n1 = left->l.size;
    } else {
        p1 = left->s.data;
        n1 = left->s.size;
    }
    LobbyString_Reserve(out, n1 + n2);
    out = LobbyString_ReplaceRange(out, 0, 0, p1, p1 + n1, first);
    return LobbyString_ReplaceRange(out, n1, 0, right, right + n2, second);
}

}

#pragma pop

// ---- formerly src/to70_ov45_extra16_run02.c
#pragma push

namespace F08 {
extern "C" {
#pragma force_active on
#pragma force_active on
#define I(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
extern void *_022577C4;
extern char *_02254D04[];
extern void ov45_022320B4(Str *, void *), LobbyString_Reserve(Str *, u32);
extern int LobbyString_Compare(Str *, Str *), LobbyChat_IsInChannel(const char *), ov45_0223E4A8(void *), PPWLobby_SetChannelData(u32, u32, const void *, const void *, u32);
extern void *LobbyByteVector_Data(void *);
extern u32 current_time(void);
extern void LobbyString_Format(Str *, const char *, ...);
extern void PPWLobby_SendMessage(u32, u32, u32, u32, u32, const void *, u32), LobbyChat_GetChannelModeAsync(const char *, void (*)(void), s32), LobbyChat_GetChannelKeysAsync(const char *, s32, const char *, void (*)(void), s32);
extern void PPWLobby_OnGetChannelMode(void), PPWLobby_OnGetChannelTime(void), PPWLobby_OnGetLobbyData(void), ov45_0223AF04(void *, void *);
extern Str *LobbyString_ConcatCStr(Str *, Str *, char *);
extern Str *LobbyString_Concat(Str *, Str *, Str *);
extern char ov45_022550B8[];
extern char ov45_022550BC[];
extern char ov45_022550C4[];
extern char ov45_022550D4[];
extern char ov45_022550E4[];
extern char ov45_022550F4[];
void ov45_02233994(Str *out, u32 id, Str *s);
void ov45_02233B74(Str *out);
int LobbyString_Equals(Str *a, Str *b);
int ov45_02234688(void);
int ov45_02234764(void);
int PPWLobby_TimerUploadSystemProfile(void);
int PPWLobby_TimerSendRecruitInfo(void);
int PPWLobby_TimerPollChannelMode(void);
int PPWLobby_TimerRequestChannelTime(void);
int PPWLobby_TimerRequestLobbyData(void);
int ov45_022349CC(u32 type);
int ov45_02234A24(int valid, u8 *data, u32 size, void **output, u32 *length);
void ov45_022353F0(int valid, u8 *data, u32 size);
void ov45_022354AC(int status);

#pragma warn_uninitializedvar off
extern Str *LobbyString_ReplaceRange(Str *, u32, u32, const char *, const char *, u8);

static inline u32 short_length(Str *p) {
    if (p->l.flag) {
        return p->l.size;
    }
    return p->s.size;
}

void ov45_02233B74(Str *out) {
    u32 id = I(_022577C4, 0x4c);
    u32 tick = current_time();
    LobbyString_Format(out, ov45_022550BC, id, tick % 1000);
}
}
}

#pragma pop
#undef I
#undef P

// ---- formerly lib/NitroDWC/src/to91_ov45_format_string216.cpp
#pragma push
using std::va_list;
extern "C" {
}
using namespace DwcFormatted216Native;

namespace DwcFormatted216 {
struct Vector {
    Vec repr;

    Vector() {
        repr.ptr = 0;
        repr.size = 0;
        repr.cap = 0;
    }

    ~Vector() {
        LobbyCharVector_Destroy(&repr);
    }

    void shrink(u32 count, ResizeDispatch) {
        repr.size -= count;
    }

    void resize(u32 count) {
        if (count > repr.size) {
            ov45_0223795C(&repr, count - repr.size, ResizeDispatch());
        } else if (count < repr.size) {
            shrink(repr.size - count, ResizeDispatch());
        }
    }

    char *text() {
        return LobbyCharVector_Data(&repr);
    }
};

typedef char StringSize[sizeof(Str) == 12 ? 1 : -1];
typedef char VectorSize[sizeof(Vector) == 12 ? 1 : -1];
typedef char DispatchSize[sizeof(ResizeDispatch) == 1 ? 1 : -1];
}

using namespace DwcFormatted216;
extern "C" void LobbyString_Format(Str *, const char *, ...);
#pragma force_active on

extern "C" void LobbyString_Format(Str *output, const char *format, ...) {
    Vector vector;
    va_list args;
    u32 count;
    va_start(args, format);
    count = OS_VSNPrintf(0, 0, format, args) + 1;
    vector.resize(count);
    OS_VSNPrintf(vector.text(), count, format, args);
    vector.repr.ptr[count - 1] = 0;
    va_end(args);
    new (output) Str;
    LobbyString_InitFromCStr(output, vector.text());
}

#pragma force_active off

#pragma pop

// ---- formerly src/op90_op90_to91_ov45_format_string216_residual_residual_1_r01.cpp
#pragma push

namespace F10 {
#pragma force_active on
class Str;
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" Str *LobbyString_InitCopy(Str *, const Str *);
extern "C" Str *LobbyString_Assign(Str *, const Str *);

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

    void Destroy() const {
        LobbyString_Destroy(const_cast<Str *>(this));
    }

    Str &operator=(const Str &o) {
        LobbyString_Assign(this, &o);
        return *this;
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }
};

struct Info {
    int id;
    Str name;
    int x;
    int y;

    Info(const Str &n)
        : id(0)
        , name(n)
        , x(0)
        , y(0) {
    }

    void Destroy() const {
        name.Destroy();
    }
};
struct Work;

struct Work {
    u32 state;
    u32 sub;
    u8 pad8[0xb8 - 8];
    Info self;
};

struct Callbacks {
    void (*a)();
    void (*b)();
    void (*c)();
    void (*d)();
    void (*e)();
    u32 f;

    Callbacks(void (*a_)(), void (*b_)(), void (*c_)(), void (*d_)(), void (*e_)())
        : a(a_)
        , b(b_)
        , c(c_)
        , d(d_)
        , e(e_)
        , f(0) {
    }
};

extern "C" Work *_022577C4;
extern "C" void PPWLobby_OnPlayerJoined(), PPWLobby_OnPlayerLeft(), PPWLobby_OnMessage(), PPWLobby_OnChannelDataChanged(), PPWLobby_OnTopicChanged(), PPWLobby_OnEnterMainChannel();
extern "C" Str ov45_02231EB0(Work *, int);
extern "C" Str ov45_02233994(int, const Str &);
extern "C" int LobbyChat_EnterChannelAsync(const char *, int, int, Callbacks *, void (*)(), int);
extern "C" int PPWLobby_EnterMainChannel(void);

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

inline void SetSelf(const Str &n) {
    Info i(n);
    Work *w = _022577C4;
    w->self.id = i.id;
    w->self.name = i.name;
    w->self.x = i.x;
    w->self.y = i.y;
    i.Destroy();
}

inline Str Build(Str t) {
    Str n = ov45_02233994(0, t);
    t.Destroy();
    return n;
}

struct Info2 {
    int id;
    u32 nm[3];
    int x;
    int y;

    Str &name() {
        return *(Str *)nm;
    }

    void Destroy() {
        name().Destroy();
    }
};

struct Work2 {
    u32 state;
    u32 sub;
    u8 pad8[0xb8 - 8];
    Info2 self;
};

inline void SetSelfK(const Str &n) {
    Info2 i;
    Work2 *w;
    Info2 *p = &i;
    i.id = 0;
    LobbyString_InitCopy(&i.name(), &n);
    p->x = 0;
    p->y = 0;
    w = (Work2 *)_022577C4;
    w->self.id = i.id;
    w->self.name() = i.name();
    w->self.x = i.x;
    w->self.y = i.y;
    i.Destroy();
}

#pragma push
#pragma opt_propagation off

extern "C" int PPWLobby_EnterMainChannel(void) {
    Callbacks cb(PPWLobby_OnPlayerJoined, PPWLobby_OnPlayerLeft, PPWLobby_OnMessage, PPWLobby_OnChannelDataChanged, PPWLobby_OnTopicChanged);
    Str name = Build(ov45_02231EB0(_022577C4, 0));
    int result = LobbyChat_EnterChannelAsync(name.CStr(), 0, 0x14, &cb, PPWLobby_OnEnterMainChannel, 0);
    {
        Work *w = _022577C4;
        int state = ErrorMap().Map(result);
        if (state != 0) {
            w->sub = state;
            w->state = 10;
        }
    }
    if (result != 0) {
        name.Destroy();
        return result;
    }
    {
        SetSelfK(name);
    }
    name.Destroy();
    return result;
}

#pragma pop

}

#pragma pop

// ---- formerly src/op90_op90_to91_ov45_format_string216_residual_residual_1_r02.cpp
#pragma push

namespace F11 {
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, char *);
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" Str *LobbyString_InitCopy(Str *, const Str *);

struct Str {
    u32 words[3];

    Str(char *s) {
        LobbyString_InitFromCStr(this, s);
    }

    Str(const Str &o) {
        LobbyString_InitCopy(this, &o);
    }

    void Destroy() {
        LobbyString_Destroy(this);
    }
};
struct Vec;
extern "C" Vec *LobbyCharVector_Destroy(Vec *);

struct Vec {
    char *ptr;
    u32 size, cap;

    void Init() {
        ptr = 0;
        size = 0;
        cap = 0;
    }

    void Destroy() {
        LobbyCharVector_Destroy(this);
    }
};

struct Entry {
    int id;
};

struct Session {
    u8 pad[0x38];
    int active;
};

typedef void (*Handler)(int, int, int, int, int, char *, u32, int);

struct Work {
    u8 pad0[0x1c];
    Handler handler;
    u8 pad20[0xb8 - 0x20];
    Entry a;
    u8 padbc[0xd0 - 0xbc];
    Entry b;
    u8 padd4[0xe8 - 0xd4];
    u8 sessions[4];
};

extern "C" Work *_022577C4;
extern "C" Str ov45_022320B4(Entry *);
extern "C" int LobbyString_Equals(const Str &, const Str &);
extern "C" int PPWLobby_DecodeMessageData(const char *, Vec *);
extern "C" char *LobbyByteVector_Data(Vec *);
extern "C" int PPWLobby_HandleLibraryData(int, int, int, int, int, char *, u32, int);
extern "C" Session *PPWLobbyPlayers_Find(void *, int);
extern "C" void PPWLobby_HandleReceivedData(int, int, char *, int, int, const char *, int);

inline int Find(Work *w, const Str &name) {
    const Str &n = name;
    int eq;
    {
        Str t = ov45_022320B4(&w->a);
        eq = LobbyString_Equals(t, n);
        t.Destroy();
    }
    if (eq) {
        return w->a.id;
    }
    {
        Str t = ov45_022320B4(&w->b);
        eq = LobbyString_Equals(t, n);
        t.Destroy();
    }
    if (eq) {
        return w->b.id;
    }
    return 5;
}

inline int IsActive(Session *s) {
    return s != 0 && s->active != 0;
}

#pragma force_active on
#pragma opt_common_subs off

extern "C" void PPWLobby_HandleReceivedData(int a, int b, char *channel, int id, int arg, const char *text, int extra) {
    int kind;
    Vec data;
    {
        Str name(channel);
        kind = Find(_022577C4, name);
        name.Destroy();
    }
    if (kind == 5) {
        return;
    }
    data.Init();
    if (!PPWLobby_DecodeMessageData(text, &data)) {
        data.Destroy();
        return;
    }
    u32 len = data.size;
    if (len == 0) {
        data.Destroy();
        return;
    }
    if (PPWLobby_HandleLibraryData(a, b, kind, id, arg, LobbyByteVector_Data(&data), len, extra)) {
        data.Destroy();
        return;
    }
    if (id != -1) {
        if (!IsActive(PPWLobbyPlayers_Find(_022577C4->sessions, id))) {
            data.Destroy();
            return;
        }
    }
    Work *w = _022577C4;
    w->handler(a, b, kind, id, arg, LobbyByteVector_Data(&data), data.size, extra);
    data.Destroy();
}

#pragma opt_common_subs reset

}

#pragma pop

// ---- formerly src/op90_to91_ov45_format_string216_residual_r01.cpp
#pragma push

namespace F12 {
extern "C" unsigned strlen(const char *);

class ResizeDispatch {};
struct Vec;

class Decoder {
public:
    int Decode(const char *src, u32 len, Vec *dst) const;
};

struct Vec {
    char *ptr;
    u32 size, cap;
};

extern "C" void ov45_0223795C(Vec *, u32, ResizeDispatch);
extern "C" char *LobbyByteVector_Data(Vec *);
extern "C" int DWC_Base64Decode(const char *, u32, char *, u32);
extern "C" int PPWLobby_DecodeMessageData(const char *, Vec *);

inline void Shrink(Vec *v, u32 count, ResizeDispatch) {
    v->size -= count;
}

inline void Resize(Vec *v, u32 count) {
    u32 size = v->size;
    if (count > size) {
        ov45_0223795C(v, count - size, ResizeDispatch());
    } else if (count < size) {
        Shrink(v, size - count, ResizeDispatch());
    }
}

inline int Decoder::Decode(const char *src, u32 len, Vec *dst) const {
    return DWC_Base64Decode(src, len, LobbyByteVector_Data(dst), dst->size);
}

#pragma force_active on

extern "C" int PPWLobby_DecodeMessageData(const char *text, Vec *out) {
    Decoder codec = Decoder();
    u32 len = strlen(text);
    int n;
    Resize(out, len);
    if (len == 0) {
        return 1;
    }
    n = codec.Decode(text, strlen(text), out);
    if (n == -1) {
        return 0;
    }
    Resize(out, n);
    return 1;
}

}

#pragma pop

// ---- formerly src/op90_to91_ov45_format_string216_residual_r02.cpp
#pragma push

namespace F13 {
class String {
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
    } raw;

    u32 Size() const {
        return !raw.l.flag ? raw.s.size : raw.l.size;
    }
};

extern "C" int LobbyString_Compare(const String *, const String *);
extern "C" int LobbyString_Equals(const String &, const String &);
#pragma force_active on

extern "C" int LobbyString_Equals(const String &a, const String &b) {
    return a.Size() == b.Size() && LobbyString_Compare(&a, &b) == 0;
}

}

#pragma pop

// ---- formerly src/op90_op90_to91_ov45_format_string216_residual_residual_2_r01.cpp
#pragma push

namespace F14 {
extern "C" int strcmp(const char *, const char *);
extern "C" int memcmp(const void *, const void *, unsigned long);

struct TA {};

struct TB {};

struct TD {};

struct TE {};

struct TU {};

struct Iter {
    const u8 *p;
    u32 extra;

    Iter(const u8 *q)
        : p(q) {
    }
};
struct Vec;
extern "C" void ov45_022327B8(Vec *, Iter, Iter, TD);
extern "C" void ov45_022383E0(Vec *, const u8 *, const u8 *, TU);
extern "C" void ov45_02232760(Vec *, const u8 *, const u8 *, TE);
extern "C" const u8 *LobbyByteVector_Data(const Vec *);
extern "C" void LobbyCharVector_Destroy(const Vec *);

struct Vec {
    u8 *ptr;
    u32 size, cap;

    void Init() {
        ptr = 0;
        size = 0;
        cap = 0;
    }

    void Assign(const u8 *first, const u8 *last) {
        Assign(Iter(first), Iter(last));
    }

    void Assign(Iter first, Iter last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(Iter first, Iter last, const TA &, const TB &) {
        ov45_022327B8(this, first, last, TD());
    }

    void InitCopy(const Vec &o, TU al) {
        ptr = 0;
        size = 0;
        cap = 0;
        ov45_022383E0(this, o.ptr, o.ptr + o.size, al);
    }

    void AssignVec(const Vec &o) {
        if (this != &o) {
            ov45_02232760(this, o.ptr, o.ptr + o.size, TE());
        }
    }
};

inline bool operator==(const Vec &a, const Vec &b) {
    return a.size == b.size && (a.size == 0 || memcmp(LobbyByteVector_Data(&a), LobbyByteVector_Data(&b), a.size) == 0);
}

struct Quad {
    u32 a;
    u64 b;
    u32 c;
};

struct Triple {
    s64 b;
    u32 a;
};

struct VecBox {
    Vec v;
    int changed;

    void Init(const Vec &x, TU al) {
        v.InitCopy(x, al);
        changed = 1;
    }

    void InitM(const Vec &x, TU al) {
        v.InitCopy(x, al);
        Mark();
    }

    void Mark() {
        if (this) {
            changed = 1;
        }
    }

    void Update(const VecBox &o) {
        if (!(v == o.v)) {
            v.AssignVec(o.v);
            changed = 1;
        }
    }
};

inline bool operator==(const Quad &a, const Quad &b) {
    return memcmp(&a, &b, 16) == 0;
}

struct QuadBox {
    Quad q;
    int changed;

    void Init(const Quad &x) {
        q = x;
        changed = 1;
    }

    void Update(const QuadBox &o) {
        if (!(q == o.q)) {
            q = o.q;
            changed = 1;
        }
    }
};

struct Rec {
    u32 x0;
    QuadBox q;
    VecBox vec;
    u32 x28, x2c;
    int qRecv;
    int vecRecv;
    int valid;
    void SetVec(const u8 *d, u32 n, TU al);
    void SetQuad(const u8 *d);

    Triple GetTriple() {
        Triple t;
        t.a = q.q.a;
        t.b = q.q.b;
        return t;
    }
};

struct Val {
    u64 v;

    Val(const u8 *d) {
        ((u32 *)&v)[0] = ((const u32 *)d)[0];
        ((u32 *)&v)[1] = ((const u32 *)d)[1];
    }

    Val(const Val &o)
        : v(o.v) {
    }
};

struct Work {
    u8 pad0[0x20];
    void (*onQuad)(int, const Triple *);
    void (*onData)(int, const u8 *, u32);
    u8 pad28[0x4c - 0x28];
    int id;
    u8 pad50[0xe8 - 0x50];
    u8 table[0x144 - 0xe8];
    int status;
    int ready;
    Val pair;

    void SetPair(Val p) {
        ready = 1;
        pair = p;
    }

    void OnPair(const u8 *d) {
        SetPair(Val(d));
    }
};

extern "C" Work *_022577C4;
extern "C" const char ov45_022550C4[], ov45_022550D4[], ov45_022550E4[], ov45_022550F4[];
extern "C" Rec *PPWLobbyPlayers_Find(void *, int);
extern "C" int PPWLobbySchedule_Load(int *, const u8 *, u32);
extern "C" int PPWLobby_HandleLibraryData(int, int, int, int, const char *, const u8 *, u32);

inline void Rec::SetVec(const u8 *d, u32 n, TU al) {
    Vec v;
    VecBox box;
    v.Init();
    v.Assign(Iter(d), Iter(d + n));
    VecBox *pb = &box;
    pb->InitM(v, al);
    VecBox *pc = &box;
    vec.Update(*pc);
    LobbyCharVector_Destroy(&box.v);
    vecRecv = 1;
    LobbyCharVector_Destroy(&v);
}

inline void Rec::SetQuad(const u8 *d) {
    QuadBox qbox;
    QuadBox *pq = &qbox;
    pq->Init(*(const Quad *)d);
    q.Update(*pq);
    qRecv = 1;
}

static inline void OnPairFn(const u8 *d) {
    _022577C4->SetPair(Val(d));
}

#pragma force_active on

extern "C" int PPWLobby_HandleLibraryData(int a0, int a1, int sel, int id, const char *cmd, const u8 *data, u32 len) {
    if (sel) {
        return 0;
    }
    if (strcmp(cmd, ov45_022550C4) == 0) {
        if (id == _022577C4->id) {
            return 1;
        }
        Rec *r = PPWLobbyPlayers_Find(_022577C4->table, id);
        if (!r) {
            return 1;
        }
        TU al;
        r->SetVec(data, len, al);
        if (r->valid) {
            _022577C4->onData(id, data, len);
        }
        return 1;
    }
    if (strcmp(cmd, ov45_022550D4) == 0) {
        if (id == _022577C4->id) {
            return 1;
        }
        if (len != 16) {
            return 1;
        }
        Rec *r = PPWLobbyPlayers_Find(_022577C4->table, id);
        if (!r) {
            return 1;
        }
        r->SetQuad(data);
        if (r->valid && r->q.changed) {
            r->q.changed = 0;
            Triple t = r->GetTriple();
            _022577C4->onQuad(id, &t);
        }
        return 1;
    }
    if (strcmp(cmd, ov45_022550E4) == 0) {
        if (len != 8) {
            return 1;
        }
        OnPairFn(data);
        return 1;
    }
    if (strcmp(cmd, ov45_022550F4) == 0) {
        if (!PPWLobbySchedule_Load(&_022577C4->status, data, len)) {
            return 1;
        }
        _022577C4->status = 4;
        return 1;
    }
    return 0;
}

}

#pragma pop

// ---- formerly src/to80_extra1657_run01.c
#pragma push

namespace F15 {
extern "C" {
#pragma force_active on

typedef struct {
    u32 pad[2];
    int mode;
    u32 gap;
    int selector;
} Obj;

typedef struct {
    u32 pad[14];
    int valid;
} Record;

extern u8 *_022577C4;
Record *PPWLobbyPlayers_Find(void *, int);
void ov45_0223B1C8(void *);

static inline BOOL RecordValid(Record *r) {
    return r && r->valid != 0;
}

int ov45_0223454C(Obj *, int, int, void *, int);

int ov45_0223454C(Obj *obj, int id, int flag, void *payload, int message) {
    Record *r;
    BOOL valid;
    if (flag) {
        return 1;
    }
    switch (obj->selector) {
    case 1:
        if (id == *(int *)(_022577C4 + 0x4c)) {
            break;
        }
        r = PPWLobbyPlayers_Find(_022577C4 + 0xe8, id);
        valid = RecordValid(r);
        switch (valid) {
        case 0:
            goto done;
        default:
            break;
        }
        if (obj->mode == 1 && message == 0x44) {
            (*(void (**)(int, void *))(_022577C4 + 0x28))(id, payload);
        }
        break;
    case 2:
        if (id == *(int *)(_022577C4 + 0x4c)) {
            break;
        }
        r = PPWLobbyPlayers_Find(_022577C4 + 0xe8, id);
        valid = RecordValid(r);
        switch (valid) {
        case 0:
            goto done;
        default:
            break;
        }
        if (obj->mode == 1 && message == 0x44) {
            (*(void (**)(int, void *))(_022577C4 + 0x2c))(id, payload);
        }
        break;
    case 0:
        if (obj->mode == 1 && message == 4) {
            ov45_0223B1C8(_022577C4 + 0x144);
        }
        break;
    }
done:
    return 1;
}
}
}

#pragma pop

// ---- formerly src/to90_extra2330_run01.c
#pragma push

namespace F16 {
extern "C" {
#pragma force_active on
#pragma force_active on

typedef union {
    struct {
        u32 large : 1, other : 31;
        u32 size;
        char *ptr;
    } heap;

    struct {
        u8 header;
        char data[11];
    } small;
} W30615String;

typedef struct {
    u8 pad[0xb8];
    W30615String name;
    u32 unusedC4, a, b;
    u8 padD0[0x18];
    u8 collection[0x5c];
    s32 count;
    u32 active;
} W30615Context;

extern W30615Context *_022577C4;
extern void ov45_022320B4(W30615String *, void *), LobbyString_Destroy(W30615String *);
extern int LobbyChat_IsInChannel(char *), ov45_0223E4A8(void *);
int ov45_02234688(void);

static inline int enough(s32 n) {
    int result;
    if (n >= 4) {
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

#pragma opt_propagation off

int ov45_02234688(void) {
    W30615String local;
    int empty;
    W30615Context *c;
    ov45_022320B4(&local, &_022577C4->name);
    empty = LobbyChat_IsInChannel(!local.heap.large ? local.small.data : local.heap.ptr) == 0;
    LobbyString_Destroy(&local);
    if (empty) {
        return 0;
    }
    c = _022577C4;
    if (c->a == 0) {
        return 0;
    }
    if (c->b == 0) {
        return 0;
    }
    if (c->active == 0) {
        return 0;
    }
    if (!enough(c->count)) {
        return 0;
    }
    return ov45_0223E4A8(c->collection) != 0;
}
}
}

#pragma pop

// ---- formerly src/to90_extra2330_run02.c
#pragma push

namespace F17 {
extern "C" {
#pragma force_active on
#pragma opt_common_subs off
#pragma force_active on
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))

typedef union Str {
    struct {
        u32 large : 1;
        u32 other : 31;
        u32 length;
        char *ptr;
    } f;

    struct {
        u8 bit : 1;
        u8 size : 7;
        u8 rest[11];
    } small;

    struct {
        u8 header;
        char data[11];
    } shortData;

    u8 bytes[12];
} Str;

extern u8 *_022577C4;
extern char ov45_022550C4[];
extern int LobbyString_Compare(Str *, Str *);
extern void *LobbyByteVector_Data(void *);
extern void PPWLobby_SetChannelData(u32, u32, char *, void *, u32);
int LobbyString_Equals(Str *, Str *);
int ov45_02234764(void);

static inline u32 strsize(Str *s) {
    if (s->f.large) {
        return s->f.length;
    }
    return s->small.size;
}

static inline int flush(u8 *c) {
    u8 *peer;
    void *buf;
    if (W(c, 0x11c) == 0) {
        return 1;
    }
    W(c, 0x11c) = 0;
    peer = (u8 *)W(_022577C4, 0x4c);
    buf = LobbyByteVector_Data(c + 0x110);
    PPWLobby_SetChannelData((u32)peer, 0, ov45_022550C4, buf, W(c, 0x114));
    return 1;
}

extern void ov45_022320B4(Str *, void *);
extern int LobbyChat_IsInChannel(char *);
extern void LobbyString_Destroy(Str *);
extern int ov45_0223E4A8(void *);
int ov45_02234688(void);

static inline BOOL ready(int n) {
    return n >= 4;
}

int ov45_02234764(void) {
    return flush(_022577C4);
}
}
}

#pragma pop
#undef W

// ---- formerly src/to70_ov45_extra16_run03.c
#pragma push

namespace F18 {
extern "C" {
#pragma force_active on
#pragma force_active on
#define I(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
extern void *_022577C4;
extern char *_02254D04[];
extern void ov45_022320B4(Str *, void *), LobbyString_Reserve(Str *, u32);
extern int LobbyString_Compare(Str *, Str *), LobbyChat_IsInChannel(const char *), ov45_0223E4A8(void *), PPWLobby_SetChannelData(u32, u32, const void *, const void *, u32);
extern void *LobbyByteVector_Data(void *);
extern u32 current_time(void);
extern void LobbyString_Format(Str *, const char *, ...);
extern void PPWLobby_SendMessage(u32, u32, u32, u32, u32, const void *, u32), LobbyChat_GetChannelModeAsync(const char *, void (*)(void), s32), LobbyChat_GetChannelKeysAsync(const char *, s32, const char *, void (*)(void), s32);
extern void PPWLobby_OnGetChannelMode(void), PPWLobby_OnGetChannelTime(void), PPWLobby_OnGetLobbyData(void), ov45_0223AF04(void *, void *);
extern Str *LobbyString_ConcatCStr(Str *, Str *, char *);
extern Str *LobbyString_Concat(Str *, Str *, Str *);
extern char ov45_022550B8[];
extern char ov45_022550BC[];
extern char ov45_022550C4[];
extern char ov45_022550D4[];
extern char ov45_022550E4[];
extern char ov45_022550F4[];
void ov45_02233994(Str *out, u32 id, Str *s);
void ov45_02233B74(Str *out);
int LobbyString_Equals(Str *a, Str *b);
int ov45_02234688(void);
int ov45_02234764(void);
int PPWLobby_TimerUploadSystemProfile(void);
int PPWLobby_TimerSendRecruitInfo(void);
int PPWLobby_TimerPollChannelMode(void);
int PPWLobby_TimerRequestChannelTime(void);
int PPWLobby_TimerRequestLobbyData(void);
int ov45_022349CC(u32 type);
int ov45_02234A24(int valid, u8 *data, u32 size, void **output, u32 *length);
void ov45_022353F0(int valid, u8 *data, u32 size);
void ov45_022354AC(int status);

#pragma warn_uninitializedvar off
extern Str *LobbyString_ReplaceRange(Str *, u32, u32, const char *, const char *, u8);

static inline u32 short_length(Str *p) {
    if (p->l.flag) {
        return p->l.size;
    }
    return p->s.size;
}

int PPWLobby_TimerUploadSystemProfile(void) {
    void *p = _022577C4;
    if (!I(p, 0x10c)) {
        return 1;
    }
    I(p, 0x10c) = 0;
    PPWLobby_SetChannelData(I(_022577C4, 0x4c), 0, ov45_022550D4, (u8 *)p + 0xfc, 16);
    return 1;
}

int PPWLobby_TimerSendRecruitInfo(void) {
    void *p = _022577C4;
    if (!I(p, 0x68)) {
        return 0;
    }
    PPWLobby_SendMessage(I(p, 0x4c), 0, 1, 0, 1, (u8 *)p + 0x74, 0x44);
    return 1;
}

int PPWLobby_TimerPollChannelMode(void) {
    Str s;
    ov45_022320B4(&s, (u8 *)_022577C4 + 0xb8);
    LobbyChat_GetChannelModeAsync(!s.l.flag ? s.s.data : s.l.data, PPWLobby_OnGetChannelMode, 0);
    LobbyString_Destroy(&s);
    return 1;
}

int PPWLobby_TimerRequestChannelTime(void) {
    Str s;
    if (I(_022577C4, 328) != 0) {
        return 0;
    }
    ov45_022320B4(&s, (u8 *)_022577C4 + 0xb8);
    LobbyChat_GetChannelKeysAsync(!s.l.flag ? s.s.data : s.l.data, -1, ov45_022550E4, PPWLobby_OnGetChannelTime, 0);
    LobbyString_Destroy(&s);
    return 1;
}
}
}

#pragma pop
#undef I
#undef P

// ---- formerly src/to80_extra1916_run01.c
#pragma push

namespace F19 {
extern "C" {
#pragma force_active on
extern u8 *_022577C4;
extern u8 ov45_022550F4[];
extern void PPWLobby_OnGetLobbyData(void), ov45_022320B4(void *, void *), LobbyString_Destroy(void *), LobbyChat_GetChannelKeysAsync(void *, int, void *, void (*)(void), int);

typedef union {
    struct {
        u32 flag : 1, other : 31;
        u32 a;
        void *pointer;
    } longform;

    struct {
        u8 prefix;
        u8 data[11];
    } shortform;
} String;

static inline BOOL limit(int value) {
    return value >= 4;
}

int PPWLobby_TimerRequestLobbyData(void);
#pragma force_active on

int PPWLobby_TimerRequestLobbyData(void) {
    String s;
    switch (limit(*(int *)(_022577C4 + 0x144))) {
    case 0:
        break;
    default:
        return 0;
    }
    ov45_022320B4(&s, _022577C4 + 0xb8);
    LobbyChat_GetChannelKeysAsync(s.longform.flag == 0 ? s.shortform.data : s.longform.pointer, -1, ov45_022550F4, PPWLobby_OnGetLobbyData, 0);
    LobbyString_Destroy(&s);
    return 1;
}
}
}

#pragma pop

// ---- formerly src/to70_ov45_extra16_run04.c
#pragma push

namespace F20 {
extern "C" {
#pragma force_active on
#pragma force_active on
#define I(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
extern void *_022577C4;
extern char *_02254D04[];
extern void ov45_022320B4(Str *, void *), LobbyString_Reserve(Str *, u32);
extern int LobbyString_Compare(Str *, Str *), LobbyChat_IsInChannel(const char *), ov45_0223E4A8(void *), PPWLobby_SetChannelData(u32, u32, const void *, const void *, u32);
extern void *LobbyByteVector_Data(void *);
extern u32 current_time(void);
extern void LobbyString_Format(Str *, const char *, ...);
extern void PPWLobby_SendMessage(u32, u32, u32, u32, u32, const void *, u32), LobbyChat_GetChannelModeAsync(const char *, void (*)(void), s32), LobbyChat_GetChannelKeysAsync(const char *, s32, const char *, void (*)(void), s32);
extern void PPWLobby_OnGetChannelMode(void), PPWLobby_OnGetChannelTime(void), PPWLobby_OnGetLobbyData(void), ov45_0223AF04(void *, void *);
extern Str *LobbyString_ConcatCStr(Str *, Str *, char *);
extern Str *LobbyString_Concat(Str *, Str *, Str *);
extern char ov45_022550B8[];
extern char ov45_022550BC[];
extern char ov45_022550C4[];
extern char ov45_022550D4[];
extern char ov45_022550E4[];
extern char ov45_022550F4[];
void ov45_02233994(Str *out, u32 id, Str *s);
void ov45_02233B74(Str *out);
int LobbyString_Equals(Str *a, Str *b);
int ov45_02234688(void);
int ov45_02234764(void);
int PPWLobby_TimerUploadSystemProfile(void);
int PPWLobby_TimerSendRecruitInfo(void);
int PPWLobby_TimerPollChannelMode(void);
int PPWLobby_TimerRequestChannelTime(void);
int PPWLobby_TimerRequestLobbyData(void);
int ov45_022349CC(u32 type);
int ov45_02234A24(int valid, u8 *data, u32 size, void **output, u32 *length);
void ov45_022353F0(int valid, u8 *data, u32 size);
void ov45_022354AC(int status);

#pragma warn_uninitializedvar off
extern Str *LobbyString_ReplaceRange(Str *, u32, u32, const char *, const char *, u8);

static inline u32 short_length(Str *p) {
    if (p->l.flag) {
        return p->l.size;
    }
    return p->s.size;
}

int ov45_022349CC(u32 type) {
    switch (type) {
    case 0:
        return 0;
    case 6:
        return 1;
    case 7:
    case 8:
    case 9:
    case 10:
        return 2;
    default:
        return 2;
    }
}

int ov45_02234A24(int valid, u8 *data, u32 size, void **output, u32 *length) {
    int e;
    *output = 0;
    *length = 0;
    if (!valid) {
        return 2;
    }
    if (size < 4) {
        return 2;
    }
    e = ov45_022349CC(*data);
    if (e == 2) {
        return e;
    }
    if (e == 1) {
        return e;
    }
    *output = data + 4;
    *length = size - 4;
    return e;
}
}
}

#pragma pop
#undef I
#undef P

// ---- formerly src/op90_op90_to80_extra1436_residual_1_residual_1_r01.cpp
#pragma push

namespace F21 {
extern "C" void OS_GetMacAddress(u8 *);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);

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

    Str(const char *c) {
        LobbyString_InitFromCStr(this, c);
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }
};

enum ReqMethod {
    REQ_METHOD_GET,
    REQ_METHOD_POST
};

struct Al {};

struct TA {};

struct TB {};

struct TC {};

struct TD {};

struct Iter {
    const u8 *p;
    u32 extra;

    Iter(const u8 *q)
        : p(q) {
    }
};
struct Vec;
extern "C" void ov45_022327B8(Vec *, Iter, Iter, TD);
extern "C" const char *LobbyByteVector_Data(const Vec *);

struct Vec {
    char *ptr;
    u32 size, cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void Assign(Iter first, Iter last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(Iter first, Iter last, const TA &, const TB &) {
        ov45_022327B8(this, first, last, TD());
    }

    void Assign(const u8 *first, const u8 *last) {
        Assign(Iter(first), Iter(last));
    }
};

struct Info {
    u32 x;
    u16 one;
    u8 a;
    u8 b;
    u8 mac[6];
    u8 rest[6];
};
struct Obj;

struct ObjVtbl {
    void (*f0)(Obj *);
    void (*del)(Obj *);
};

extern "C" const ObjVtbl ov45_02254FC0;
extern "C" void ov45_02234D68(void);

struct Obj {
    const ObjVtbl *vtbl;
    int kind;
    int flag;
    Str name;
    Str host;
    u32 x;
    Vec data;
    void (*cb)(void);
    int x38;
    int x3c;

    Obj(int f, const char *n, const char *h, u32 x_, const Info &info)
        : vtbl(&ov45_02254FC0)
        , kind(3)
        , flag(f)
        , name(n)
        , host(h)
        , x(x_)
        , data()
        , cb(ov45_02234D68)
        , x38(0)
        , x3c(1) {
        data.Assign(Iter((const u8 *)&info), Iter((const u8 *)&info + sizeof(Info)));
    }
};

struct Work {
    int state;
    int sub;
    u8 pad8[0x34 - 8];
    void (*cb)(int, void *, u32);
    void *cb38;
    void (*cb3c)(int, void *, u32);
    u8 pad40[0x4c - 0x40];
    u32 x4c;
    u32 x50;
    u8 pad54[0x110 - 0x54];
    Vec field;
    u8 pad11c[0x144 - 0x11c];
    u8 x144[4];
};

extern "C" Work *_022577C4;
extern "C" Obj *ov45_022577D4;
extern "C" int ov45_02234A24(int, int, int, void **, u32 *);
extern "C" void *LobbyMem_Alloc(u32);
extern "C" void StatsUpload_Start(int, const char *);
extern "C" int StatsUpload_UploadWithLastHash(const char *, u32, const char *, u32, int);
extern "C" int StatsUpload_Upload(const char *, u32, const char *, u32, int);
extern "C" void StatsUpload_Stop(int);
extern "C" int PPW_LobbyUpdateMyProfile(const void *, u32, int);
extern "C" void ov45_02234A88(int a0, int a1, int a2);
#pragma force_active on

extern "C" void ov45_02234A88(int a0, int a1, int a2) {
    Info info;
    const char *name;
    const char *host;
    Obj *o;
    u32 x;
    ReqMethod flag;
    void *buf;
    u32 len;
    Work *w = _022577C4;
    if (w == 0 || w->state == 9) {
        return;
    }
    buf = 0;
    len = 0;
    int r = ov45_02234A24(a0, a1, a2, &buf, &len);
    if (r == 0 && len != 0x94) {
        r = 2;
    }
    if (r == 0) {
        PPW_LobbyUpdateMyProfile(buf, len, 0);
    }
    _022577C4->cb(r, buf, len);
    if (r != 0) {
        Work *g = _022577C4;
        g->sub = 6;
        g->state = 10;
        return;
    }
    const u8 *f = (const u8 *)LobbyByteVector_Data(&_022577C4->field);
    info.x = _022577C4->x4c;
    info.one = 1;
    info.a = f[0x42];
    info.b = f[0x39];
    OS_GetMacAddress(info.mac);
    x = _022577C4->x4c;
    host = ov45_02254F68;
    name = ov45_02254F64;
    flag = _022577C4->x50 == 0 ? REQ_METHOD_GET : REQ_METHOD_POST;
    if (ov45_022577D4 != 0) {
        return;
    }
    o = (Obj *)LobbyMem_Alloc(sizeof(Obj));
    new (o) Obj(flag, name, host, x, info);
    ov45_022577D4 = o;
    StatsUpload_Start(o->flag, o->name.CStr());
    int ret;
    if (o->x3c) {
        u32 n = o->data.size;
        ret = StatsUpload_UploadWithLastHash(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    } else {
        u32 n = o->data.size;
        ret = StatsUpload_Upload(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    }
    int ok;
    if (ret == 0) {
        ok = 1;
    } else {
        StatsUpload_Stop(ret);
        ok = 0;
    }
    if (ok) {
        return;
    }
    if (ov45_022577D4 != 0) {
        Obj *p = ov45_022577D4;
        if (p != 0) {
            p->vtbl->del(p);
        }
        ov45_022577D4 = 0;
    }
}

}

#pragma pop

// ---- formerly lib/NitroDWC/asm/op90_op90_to80_extra1436_residual_1_residual_1_residual_1.s
#pragma push

namespace F22 {
// @var
extern "C" void OS_GetMacAddress(u8 *);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);

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

    Str(const char *c) {
        LobbyString_InitFromCStr(this, c);
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }
};

enum ReqMethod {
    REQ_METHOD_GET,
    REQ_METHOD_POST
};

struct Al {};

struct TA {};

struct TB {};

struct TC {};

struct TD {};

struct Iter {
    const u8 *p;
    u32 extra;

    Iter(const u8 *q)
        : p(q) {
    }
};
struct Vec;
extern "C" void ov45_022327B8(Vec *, Iter, Iter, TD);
extern "C" const char *LobbyByteVector_Data(const Vec *);

struct Vec {
    char *ptr;
    u32 size, cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void Assign(const Iter &first, const Iter &last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(const Iter &first, const Iter &last, const TA &, const TB &) {
        ov45_022327B8(this, first, last, TD());
    }

    void Assign(const u8 *first, const u8 *last) {
        Assign(Iter(first), Iter(last));
    }
};

struct Info {
    u32 x;
    u16 one;
    u8 a;
    u8 b;
    u8 mac[6];
    u16 r14;
    u32 r16;
};

struct Info2 {
    Info info;
    u32 extra;
};
struct Obj;
extern "C" void ov45_022350FC(int, int, int);

struct ObjVtbl {
    void (*f0)(Obj *);
    void (*del)(Obj *);
};

extern "C" const ObjVtbl ov45_02254FC0;

struct Obj {
    const ObjVtbl *vtbl;
    int kind;
    int flag;
    Str name;
    Str host;
    u32 x;
    Vec data;
    void (*cb)(int, int, int);
    int x38;
    int x3c;

    Obj(int f, const char *n, const char *h, u32 x_, const Info2 &info)
        : vtbl(&ov45_02254FC0)
        , kind(3)
        , flag(f)
        , name(n)
        , host(h)
        , x(x_)
        , data()
        , cb(ov45_022350FC)
        , x38(0)
        , x3c(1) {
        data.Assign(Iter((const u8 *)&info), Iter((const u8 *)&info + sizeof(Info2)));
    }
};

struct Work {
    int state;
    int sub;
    u8 pad8[0x34 - 8];
    void (*cb)(int, void *, u32);
    int (*cb38)(int, void *);
    void (*cb3c)(int, void *, u32);
    u8 pad40[0x4c - 0x40];
    u32 x4c;
    u32 x50;
    u8 pad54[0x110 - 0x54];
    Vec field;
    u8 pad11c[0x144 - 0x11c];
    u8 x144[4];
};

extern "C" Work *_022577C4;
extern "C" Obj *ov45_022577D4;
extern "C" int ov45_02234A24(int, int, int, void **, u32 *);
extern "C" int PPW_LobbyUpdateMyProfile(const void *, u32, int);
extern "C" void *LobbyMem_Alloc(u32);
extern "C" void StatsUpload_Start(int, const char *);
extern "C" int StatsUpload_UploadWithLastHash(const char *, u32, const char *, u32, int);
extern "C" int StatsUpload_Upload(const char *, u32, const char *, u32, int);
extern "C" void StatsUpload_Stop(int);
extern "C" int PPWLobbySchedule_IsSizeValid(void *, void *, u32);
extern "C" void PPWLobbySchedule_Load(void *, void *, u32);
extern "C" void ov45_02234D68(int a0, int a1, int a2);
#pragma force_active on
#pragma opt_dead_assignments off

extern "C" void ov45_02234D68(int a0, int a1, int a2) {
    Info2 i2;
    Info info;
    const char *name;
    const char *host;
    Obj *o;
    u32 x;
    ReqMethod flag;
    void *buf;
    u32 len;
    Work *w = _022577C4;
    if (w == 0 || w->state == 9) {
        return;
    }
    buf = 0;
    len = 0;
    int r = ov45_02234A24(a0, a1, a2, &buf, &len);
    if (r == 0 && !PPWLobbySchedule_IsSizeValid(_022577C4->x144, buf, len)) {
        r = 2;
    }
    int ok2 = _022577C4->cb38(r, buf);
    if (r != 0) {
        Work *g = _022577C4;
        g->sub = 4;
        g->state = 10;
        return;
    }
    if (ok2) {
        PPWLobbySchedule_Load(_022577C4->x144, buf, len);
    }
    const u8 *p = (const u8 *)LobbyByteVector_Data(&_022577C4->field);
    int ret = 0;
    std::memset(&i2, 0, sizeof(i2));
    const u8 *f = (const u8 *)LobbyByteVector_Data(&_022577C4->field);
    info.x = _022577C4->x4c;
    info.one = 1;
    info.a = f[0x42];
    info.b = f[0x39];
    OS_GetMacAddress(info.mac);
    i2.info = info;
    i2.extra = *(const u32 *)(p + 4);
    x = _022577C4->x4c;
    host = ov45_02254F6C;
    name = ov45_02254F64;
    flag = _022577C4->x50 == 0 ? REQ_METHOD_GET : REQ_METHOD_POST;
    if (ov45_022577D4 != 0) {
        return;
    }
    o = (Obj *)LobbyMem_Alloc(sizeof(Obj));
    new (o) Obj(flag, name, host, x, i2);
    ov45_022577D4 = o;
    StatsUpload_Start(o->flag, o->name.CStr());
    if (o->x3c) {
        u32 n = o->data.size;
        ret = StatsUpload_UploadWithLastHash(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    } else {
        u32 n = o->data.size;
        ret = StatsUpload_Upload(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    }
    int ok;
    if (ret == 0) {
        ok = 1;
    } else {
        StatsUpload_Stop(ret);
        ok = 0;
    }
    if (ok) {
        return;
    }
    if (ov45_022577D4 != 0) {
        Obj *q = ov45_022577D4;
        if (q != 0) {
            q->vtbl->del(q);
        }
        ov45_022577D4 = 0;
    }
}

}

#pragma pop

// ---- formerly src/op90_to80_extra1436_residual_1_r01.cpp
#pragma push

namespace F23 {
extern "C" void OS_GetMacAddress(u8 *);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, const char *);

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

    Str(const char *c) {
        LobbyString_InitFromCStr(this, c);
    }

    const char *CStr() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }
};

enum ReqMethod {
    REQ_METHOD_GET,
    REQ_METHOD_POST
};

struct Al {};

struct TA {};

struct TB {};

struct TC {};

struct TD {};

struct Iter {
    const u8 *p;
    u32 extra;

    Iter(const u8 *q)
        : p(q) {
    }
};
struct Vec;
extern "C" void ov45_022327B8(Vec *, Iter, Iter, TD);
extern "C" const char *LobbyByteVector_Data(const Vec *);

struct Vec {
    char *ptr;
    u32 size, cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void Assign(Iter first, Iter last) {
        Assign(first, last, TA(), TB());
    }

    void Assign(Iter first, Iter last, const TA &, const TB &) {
        ov45_022327B8(this, first, last, TD());
    }

    void Assign(const u8 *first, const u8 *last) {
        Assign(Iter(first), Iter(last));
    }
};

struct Info {
    u32 x;
    u16 one;
    u8 a;
    u8 b;
    u8 mac[6];
    u8 rest[6];
};
struct Obj;

struct ObjVtbl {
    void (*f0)(Obj *);
    void (*del)(Obj *);
};

extern "C" const ObjVtbl ov45_02254FC0;
extern "C" void ov45_022353F0(void);

struct Obj {
    const ObjVtbl *vtbl;
    int kind;
    int flag;
    Str name;
    Str host;
    u32 x;
    Vec data;
    void (*cb)(void);
    int x38;
    int x3c;

    Obj(int f, const char *n, const char *h, u32 x_, const Info &info)
        : vtbl(&ov45_02254FC0)
        , kind(3)
        , flag(f)
        , name(n)
        , host(h)
        , x(x_)
        , data()
        , cb(ov45_022353F0)
        , x38(0)
        , x3c(1) {
        data.Assign(Iter((const u8 *)&info), Iter((const u8 *)&info + sizeof(Info)));
    }
};

struct Work {
    int state;
    int sub;
    u8 pad8[0x34 - 8];
    void (*cb)(int, void *, u32);
    void *cb38;
    void (*cb3c)(int, void *, u32);
    u8 pad40[0x4c - 0x40];
    u32 x4c;
    u32 x50;
    u8 pad54[0x110 - 0x54];
    Vec field;
    u8 pad11c[0x144 - 0x11c];
    u8 x144[4];
};

extern "C" Work *_022577C4;
extern "C" Obj *ov45_022577D4;
extern "C" int ov45_02234A24(int, int, int, void **, u32 *);
extern "C" void *LobbyMem_Alloc(u32);
extern "C" void StatsUpload_Start(int, const char *);
extern "C" int StatsUpload_UploadWithLastHash(const char *, u32, const char *, u32, int);
extern "C" int StatsUpload_Upload(const char *, u32, const char *, u32, int);
extern "C" void StatsUpload_Stop(int);
extern "C" int ov45_0223AE84(void *, void *, u32);
extern "C" void ov45_0223ACA4(void *, void *, u32);
extern "C" void ov45_022350FC(int a0, int a1, int a2);
#pragma force_active on

extern "C" void ov45_022350FC(int a0, int a1, int a2) {
    Info info;
    const char *name;
    const char *host;
    Obj *o;
    u32 x;
    ReqMethod flag;
    void *buf;
    u32 len;
    Work *w = _022577C4;
    if (w == 0 || w->state == 9) {
        return;
    }
    buf = 0;
    len = 0;
    int r = ov45_02234A24(a0, a1, a2, &buf, &len);
    if (!ov45_0223AE84(_022577C4->x144, buf, len)) {
        r = 2;
    }
    if (r != 0) {
        Work *g = _022577C4;
        g->sub = 5;
        g->state = 10;
        return;
    }
    ov45_0223ACA4(_022577C4->x144, buf, len);
    _022577C4->cb3c(r, buf, len >> 3);
    const u8 *f = (const u8 *)LobbyByteVector_Data(&_022577C4->field);
    info.x = _022577C4->x4c;
    info.one = 1;
    info.a = f[0x42];
    info.b = f[0x39];
    OS_GetMacAddress(info.mac);
    x = _022577C4->x4c;
    host = ov45_02254F60;
    name = ov45_02254F64;
    flag = _022577C4->x50 == 0 ? REQ_METHOD_GET : REQ_METHOD_POST;
    if (ov45_022577D4 != 0) {
        return;
    }
    o = (Obj *)LobbyMem_Alloc(sizeof(Obj));
    new (o) Obj(flag, name, host, x, info);
    ov45_022577D4 = o;
    StatsUpload_Start(o->flag, o->name.CStr());
    int ret;
    if (o->x3c) {
        u32 n = o->data.size;
        ret = StatsUpload_UploadWithLastHash(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    } else {
        u32 n = o->data.size;
        ret = StatsUpload_Upload(o->host.CStr(), o->x, LobbyByteVector_Data(&o->data), n, 0);
    }
    int ok;
    if (ret == 0) {
        ok = 1;
    } else {
        StatsUpload_Stop(ret);
        ok = 0;
    }
    if (ok) {
        return;
    }
    if (ov45_022577D4 != 0) {
        Obj *p = ov45_022577D4;
        if (p != 0) {
            p->vtbl->del(p);
        }
        ov45_022577D4 = 0;
    }
}

}

#pragma pop

// ---- formerly src/to80_extra1436_run01.c
#pragma push

namespace F24 {
extern "C" {
#pragma force_active on
#pragma force_active on

typedef struct {
    int state, error;
} Status;

typedef struct {
    union {
        struct {
            int state, error;
        };

        Status status;
    };

    u8 p[0x40 - 8];
    void (*callback)(int, void *);
    u8 q[0x144 - 0x44];
    int value;
} Work;

extern Work *_022577C4;
extern int ov45_02234A24(void *, void *, void *, void **, u32 *);
extern void ov45_0223AF04(void *, void *);
void ov45_022353F0(void *, void *, void *);

void ov45_022353F0(void *a, void *b, void *c) {
    void *data;
    u32 size;
    int result;
    Work *w = _022577C4;
    if (!w || w->state == 9) {
        return;
    }
    data = 0;
    size = 0;
    result = ov45_02234A24(a, b, c, &data, &size);
    if (size != 0x2d8) {
        result = 2;
    }
    if (result) {
        {
            Work *target = _022577C4;
            target->error = 7;
            target->state = 10;
        }
        return;
    }
    ov45_0223AF04(&_022577C4->value, data);
    _022577C4->callback(result, data);
    _022577C4->value = 2;
}
}
}

#pragma pop

// ---- formerly src/to70_ov45_extra16_run05.c
#pragma push

namespace F25 {
extern "C" {
#pragma force_active on
#pragma force_active on
#define I(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
extern void *_022577C4;
extern char *_02254D04[];
extern void ov45_022320B4(Str *, void *), LobbyString_Reserve(Str *, u32);
extern int LobbyString_Compare(Str *, Str *), LobbyChat_IsInChannel(const char *), ov45_0223E4A8(void *), PPWLobby_SetChannelData(u32, u32, const void *, const void *, u32);
extern void *LobbyByteVector_Data(void *);
extern u32 current_time(void);
extern void LobbyString_Format(Str *, const char *, ...);
extern void PPWLobby_SendMessage(u32, u32, u32, u32, u32, const void *, u32), LobbyChat_GetChannelModeAsync(const char *, void (*)(void), s32), LobbyChat_GetChannelKeysAsync(const char *, s32, const char *, void (*)(void), s32);
extern void PPWLobby_OnGetChannelMode(void), PPWLobby_OnGetChannelTime(void), PPWLobby_OnGetLobbyData(void), ov45_0223AF04(void *, void *);
extern Str *LobbyString_ConcatCStr(Str *, Str *, char *);
extern Str *LobbyString_Concat(Str *, Str *, Str *);
extern char ov45_022550B8[];
extern char ov45_022550BC[];
extern char ov45_022550C4[];
extern char ov45_022550D4[];
extern char ov45_022550E4[];
extern char ov45_022550F4[];
void ov45_02233994(Str *out, u32 id, Str *s);
void ov45_02233B74(Str *out);
int LobbyString_Equals(Str *a, Str *b);
int ov45_02234688(void);
int ov45_02234764(void);
int PPWLobby_TimerUploadSystemProfile(void);
int PPWLobby_TimerSendRecruitInfo(void);
int PPWLobby_TimerPollChannelMode(void);
int PPWLobby_TimerRequestChannelTime(void);
int PPWLobby_TimerRequestLobbyData(void);
int ov45_022349CC(u32 type);
int ov45_02234A24(int valid, u8 *data, u32 size, void **output, u32 *length);
void ov45_022353F0(int valid, u8 *data, u32 size);
void ov45_022354AC(int status);

#pragma warn_uninitializedvar off
extern Str *LobbyString_ReplaceRange(Str *, u32, u32, const char *, const char *, u8);

static inline u32 short_length(Str *p) {
    if (p->l.flag) {
        return p->l.size;
    }
    return p->s.size;
}

void ov45_022354AC(int status) {
    if (!_022577C4 || I(_022577C4, 0) == 9) {
        return;
    }
    I(_022577C4, 0x6c) = 0;
    ((void (*)(s32))P(_022577C4, 0x44))(status ? 0 : 2);
}
}
}

#pragma pop
#undef I
#undef P

// ---- formerly src/op90_op90_to70_ov45_extra16_residual_6_residual_1_r01.cpp
#pragma push

namespace F26 {
#pragma force_active on
using std::memcmp;

typedef struct RTCDate {
    u32 year, month, day, week;
} RTCDate;

typedef struct RTCTime {
    u32 hour, minute, second;
} RTCTime;

extern "C" s64 RTC_ConvertDateTimeToSecond(const RTCDate *date, const RTCTime *time);
extern "C" void DWC_GetDateTime(RTCDate *date, RTCTime *time);

struct TrueType {};

struct FalseType {};

template <class T, class U>
inline void Assign(T &d, const U &s, TrueType, FalseType) {
    d = s;
}

template <class T, class U>
inline void Assign(T &d, const U &s) {
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

extern "C" void LobbyString_Destroy(Str *s);
extern "C" void LobbyString_Assign(Str *dst, const Str *src);

typedef struct Vec {
    void *ptr;
    u32 size, cap;
} Vec;

extern "C" void *LobbyByteVector_Data(Vec *v);

static inline s64 GetNow(void) {
    RTCDate date;
    RTCTime time;
    DWC_GetDateTime(&date, &time);
    return RTC_ConvertDateTimeToSecond(&date, &time);
}

struct Pair {
    s64 v;

    Pair() {
    }

    Pair(s64 x)
        : v(x) {
    }
};

struct Stamp {
    u32 code;
    Pair pair;
    u32 extra;

    Stamp(u32 c)
        : code(c)
        , pair(0)
        , extra(0) {
        Pair *d = &pair;
        *d = Pair(GetNow());
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

    Tracked(u32 c, int a)
        : stamp(Stamp(c))
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

template <class A, class B>
struct KV {
    A first;
    B second;

    template <class C, class D>
    KV(const KV<C, D> &p) {
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
extern "C" void LobbyTimerMap_Insert(InsertResult *out, Map *m, const KV<u32, Task> *v);

struct Map {
    u32 words[4];

    InsertResult insert(const KV<u32, Task> &v) {
        InsertResult r;
        LobbyTimerMap_Insert(&r, this, &v);
        return r;
    }
};

extern "C" KV<int, Task> LobbyTimerMap_MakeEntry(int k, const Task &t);

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
        LobbyString_Assign(&name, &empty);
        a = 0;
        b = 0;
        LobbyString_Destroy(&empty);
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
extern "C" void LobbyChat_EnumUsersAsync(void *handle, void (*cb)(void), u32 code);
extern "C" void PPWLobby_OnEnumUsers(void);
extern "C" int PPWLobby_TimerUploadSystemProfile(void);
extern "C" int ov45_02234764(void);
extern "C" int PPWLobby_TimerRequestChannelTime(void);
extern "C" int PPWLobby_TimerPollChannelMode(void);
extern "C" int PPWLobby_TimerRequestLobbyData(void);

extern "C" void PPWLobby_OnEnterMainChannel(int success, void *unused, void *handle, u32 code);

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
    KV<int, Task> d = LobbyTimerMap_MakeEntry(kind, task);
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
    cbs->notify(key, args, LobbyByteVector_Data(&sub->vec), sub->vec.size);
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

extern "C" void PPWLobby_OnEnterMainChannel(int success, void *unused, void *handle, u32 code) {
    if (code == 5) {
        return;
    }
    if (success) {
        Lobby_ResetStamp();
        LobbyChat_EnumUsersAsync(handle, PPWLobby_OnEnumUsers, code);
        Lobby_AddTimer(1, 3, PPWLobby_TimerUploadSystemProfile);
        Lobby_AddTimer(2, 4, ov45_02234764);
        Lobby_AddTimer(5, 4, PPWLobby_TimerRequestChannelTime);
        Lobby_AddTimer(4, 3, PPWLobby_TimerPollChannelMode);
        Lobby_AddTimer(6, 4, PPWLobby_TimerRequestLobbyData);
        Arguments args = Lobby_GetArgs();
        Lobby_Notify(&args);
    } else {
        Lobby_OnFail();
    }
}

}

#pragma pop

// ---- formerly src/op90_to70_ov45_extra16_residual_6_r01.cpp
#pragma push

namespace F27 {
extern "C" int memcmp(const void *, const void *, unsigned);
class Str;
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" Str *LobbyString_Assign(Str *, const Str *);

class Str {
public:
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

    Str &operator=(const Str &o) {
        LobbyString_Assign(this, &o);
        return *this;
    }
};

struct InfoHead {
    int id;
};

struct Info : InfoHead {
    u64 mid;
    int z;

    Info() {
    }

    Info(const Info &o)
        : InfoHead(o) {
        *this = o;
    }

    bool operator==(const Info &o) const {
        return memcmp(this, &o, sizeof(Info)) == 0;
    }
};

struct Entry {
    Info info;
    int valid;

    Entry(const Info &i)
        : info(i)
        , valid(1) {
    }

    void Assign(const Info &i) {
        info = i;
        valid = 1;
    }
};

struct Peer {
    int kind;
    Str name;
    int a;
    int b;

    void Reset() {
        Str empty;
        kind = 5;
        name = empty;
        a = 0;
        b = 0;
        empty.Destroy();
    }
};

struct Work {
    int state;
    int sub;
    void (*callback)(int, int);
    u8 padc[0xd0 - 0xc];
    Peer peer;
    u8 pade8[0xfc - 0xe8];
    Entry entry;
    u8 pad110[0x128 - 0x110];
    int dirty;
};

extern "C" Work *_022577C4;
extern "C" char ov45_02254D5C[], ov45_02254D1C[], ov45_02254D3C[];
extern "C" void LobbyChat_SetChannelMode(int, const char *);
extern "C" void ov45_02235A94(int, int, int, int);
#pragma force_active on
#pragma opt_common_subs off

extern "C" void ov45_02235A94(int result, int unused, int arg, int kind) {
    if (kind == 5) {
        return;
    }
    if (result != 0) {
        const char *name;
        switch (kind) {
        case 1:
        case 2:
            name = ov45_02254D5C;
            break;
        case 3:
            name = ov45_02254D1C;
            break;
        case 4:
            name = ov45_02254D3C;
            break;
        default:
            name = 0;
            break;
        }
        if (name == 0) {
            Work *w = _022577C4;
            w->sub = 1;
            w->state = 10;
        }
        LobbyChat_SetChannelMode(arg, name);
        {
            Work *w = _022577C4;
            w->entry.valid = 0;
            Info info;
            info = w->entry.info;
            info.id = kind;
            Entry e(info);
            const Info &ni = e.info;
            if (!(w->entry.info == ni)) {
                w->entry.Assign(ni);
            }
            w->dirty = 1;
        }
    } else {
        _022577C4->peer.Reset();
    }
    _022577C4->callback(result, kind);
}

#pragma opt_common_subs reset

}

#pragma pop

// ---- formerly src/op90_to70_ov45_extra16_residual_6_r02.cpp
#pragma push

namespace F28 {
extern "C" int memcmp(const void *, const void *, unsigned);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, char *);
extern "C" Str *LobbyString_Destroy(Str *);

struct Str {
    u32 words[3];

    void Destroy() {
        LobbyString_Destroy(this);
    }
};

struct InfoHead {
    int id;
};

struct Info : InfoHead {
    u64 mid;
    int z;

    Info() {
    }

    Info(const Info &o)
        : InfoHead(o) {
        *this = o;
    }

    bool operator==(const Info &o) const {
        return memcmp(this, &o, sizeof(Info)) == 0;
    }
};

struct Entry {
    Info info;
    int valid;

    Entry(const Info &i)
        : info(i)
        , valid(1) {
    }

    void Assign(const Info &i) {
        info = i;
        valid = 1;
    }
};

struct SummaryData {
    u64 mid;
    int id;
};

struct Summary {
    SummaryData d;

    Summary(const Info &i) {
        d.id = i.id;
        d.mid = i.mid;
    }

    Summary() {
    }

    Summary(const Summary &o)
        : d(o.d) {
    }
};

struct Session {
    u32 unk0;
    Entry entry;
    u8 pad18[0x30 - 0x18];
    int dirty;
    u8 pad34[4];
    int active;
};

struct Channel {
    int id;
};

struct Work {
    u8 pad0[0x10];
    void (*left)(int);
    u8 pad14[0x20 - 0x14];
    void (*update)(int, const Summary &);
    u8 pad24[0xb8 - 0x24];
    Channel a;
    u8 padbc[0xd0 - 0xbc];
    Channel b;
    u8 padd4[0xe8 - 0xd4];
    u8 sessions[4];
};

extern "C" Work *_022577C4;
extern "C" void ov45_022320B4(Str *, Channel *);
extern "C" int LobbyString_Equals(const Str *, const Str *);
extern "C" Session *PPWLobbyPlayers_Find(void *, int);
extern "C" void PPWLobbyPlayers_Remove(void *, int);
extern "C" void PPWLobby_OnPlayerJoined(char *, int);
extern "C" void PPWLobbyPlayers_Add(void *, int);

inline int Match(Channel *c, const Str &name) {
    Str t;
    int r;
    ov45_022320B4(&t, c);
    r = LobbyString_Equals(&t, &name);
    t.Destroy();
    return r;
}

inline int Find(Work *w, const Str &name) {
    const Str &n = name;
    if (Match(&w->a, n)) {
        return w->a.id;
    }
    if (Match(&w->b, n)) {
        return w->b.id;
    }
    return 5;
}

inline void Refresh2(Session *s, Info &info, Summary &arg, int kind) {
    s->entry.valid = 0;
    info = s->entry.info;
    info.id = kind;
    Entry e(info);
    const Info &ni = e.info;
    if (!(s->entry.info == ni)) {
        s->entry.Assign(ni);
    }
    s->dirty = 1;
    s->entry.valid = 0;
}

inline void MakeSum(Session *s, Summary &arg) {
    Summary sum(s->entry.info);
    arg = sum;
}

#pragma force_active on
#pragma opt_common_subs off

extern "C" void PPWLobby_OnPlayerJoined(char *channel, int id) {
    Info info;
    Summary arg;
    int kind;
    {
        Str name;
        LobbyString_InitFromCStr(&name, channel);
        kind = Find(_022577C4, name);
        name.Destroy();
    }
    if (kind == 5) {
        return;
    }
    if (kind == 0) {
        PPWLobbyPlayers_Add(_022577C4->sessions, id);
        return;
    }
    Session *s = PPWLobbyPlayers_Find(_022577C4->sessions, id);
    if (s == 0) {
        PPWLobbyPlayers_Add(_022577C4->sessions, id);
        s = PPWLobbyPlayers_Find(_022577C4->sessions, id);
    }
    Refresh2(s, info, arg, kind);
    MakeSum(s, arg);
    if (s->active) {
        _022577C4->update(id, arg);
    }
}

#pragma opt_common_subs reset

}

#pragma pop

// ---- formerly src/op90_to70_ov45_extra16_residual_6_r03.cpp
#pragma push

namespace F29 {
extern "C" int memcmp(const void *, const void *, unsigned);
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, char *);
extern "C" Str *LobbyString_Destroy(Str *);

struct Str {
    u32 words[3];

    void Destroy() {
        LobbyString_Destroy(this);
    }
};

struct InfoHead {
    int id;
};

struct Info : InfoHead {
    u64 mid;
    int z;

    Info() {
    }

    Info(const Info &o)
        : InfoHead(o) {
        *this = o;
    }

    bool operator==(const Info &o) const {
        return memcmp(this, &o, sizeof(Info)) == 0;
    }
};

struct Entry {
    Info info;
    int valid;

    Entry(const Info &i)
        : info(i)
        , valid(1) {
    }

    void Assign(const Info &i) {
        info = i;
        valid = 1;
    }
};

struct SummaryData {
    u64 mid;
    int id;
};

struct Summary {
    SummaryData d;

    Summary(const Info &i) {
        d.id = i.id;
        d.mid = i.mid;
    }

    Summary() {
    }

    Summary(const Summary &o)
        : d(o.d) {
    }
};

struct Session {
    u32 unk0;
    Entry entry;
    u8 pad18[0x30 - 0x18];
    int dirty;
    u8 pad34[4];
    int active;
};

struct Channel {
    int id;
};

struct Work {
    u8 pad0[0x10];
    void (*left)(int);
    u8 pad14[0x20 - 0x14];
    void (*update)(int, const Summary &);
    u8 pad24[0xb8 - 0x24];
    Channel a;
    u8 padbc[0xd0 - 0xbc];
    Channel b;
    u8 padd4[0xe8 - 0xd4];
    u8 sessions[4];
};

extern "C" Work *_022577C4;
extern "C" void ov45_022320B4(Str *, Channel *);
extern "C" int LobbyString_Equals(const Str *, const Str *);
extern "C" Session *PPWLobbyPlayers_Find(void *, int);
extern "C" void PPWLobbyPlayers_Remove(void *, int);
extern "C" void PPWLobby_OnPlayerLeft(char *, int);

inline int Match(Channel *c, const Str &name) {
    Str t;
    int r;
    ov45_022320B4(&t, c);
    r = LobbyString_Equals(&t, &name);
    t.Destroy();
    return r;
}

inline int Find(Work *w, const Str &name) {
    const Str &n = name;
    if (Match(&w->a, n)) {
        return w->a.id;
    }
    if (Match(&w->b, n)) {
        return w->b.id;
    }
    return 5;
}

inline void Refresh2(Session *s, Info &info, Summary &arg) {
    s->entry.valid = 0;
    info = s->entry.info;
    info.id = 5;
    Entry e(info);
    const Info &ni = e.info;
    if (!(s->entry.info == ni)) {
        s->entry.Assign(ni);
    }
    s->dirty = 1;
    s->entry.valid = 0;
}

inline void MakeSum(Session *s, Summary &arg) {
    Summary sum(s->entry.info);
    arg = sum;
}

#pragma force_active on
#pragma opt_common_subs off

extern "C" void PPWLobby_OnPlayerLeft(char *channel, int id) {
    Info info;
    Summary arg;
    int kind;
    {
        Str name;
        LobbyString_InitFromCStr(&name, channel);
        kind = Find(_022577C4, name);
        name.Destroy();
    }
    if (kind == 5) {
        return;
    }
    if (kind == 0) {
        Session *s = PPWLobbyPlayers_Find(_022577C4->sessions, id);
        if (s == 0 || s->active == 0) {
            return;
        }
        _022577C4->left(id);
        PPWLobbyPlayers_Remove(_022577C4->sessions, id);
        return;
    }
    Session *s = PPWLobbyPlayers_Find(_022577C4->sessions, id);
    if (s == 0) {
        return;
    }
    Refresh2(s, info, arg);
    MakeSum(s, arg);
    if (s->active) {
        _022577C4->update(id, arg);
    }
}

#pragma opt_common_subs reset

}

#pragma pop

// ---- formerly src/to65_ov45_extra06_run01.c
#pragma push

namespace F30 {
extern "C" {
#pragma force_active on
void ov45_02236E58(Iterator *);
#pragma force_active on
extern int printf(const char *, ...);
extern void abort(void);
#pragma warn_uninitializedvar off

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u32 *_022577C4;
extern u8 ov45_02254FA0[], ov45_022550C4[], ov45_022550D4[], ov45_02254D7C[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void ov45_0223DF4C(void *), ov45_02238508(void *), ov45_02238558(void *, void *);
extern void ov45_022385A4(struct Vec *, u32, int), ov45_02238674(struct Vec *, u32, void *), ov45_022387E4(struct Vec *, u32, struct Alloc), ov45_02238820(struct Vec *, u32);
extern void ov45_02237800(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void ov45_022320B4(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 ov45_02237890(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void ov45_02238508(void *), ov45_02238558(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *ov45_02236AE8(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char ov45_02255120[], ov45_02255104[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void ov45_02238C1C(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char ov45_022550F4[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void PPWLobby_OnPlayerMessage(int a, int b, int c) {
    PPWLobby_OnMessage(0, a, b, c);
}
}
}

#pragma pop

// ---- formerly src/to80_extra1839_run01.c
#pragma push

namespace F31 {
extern "C" {
#pragma force_active on

typedef struct {
    u32 a, b, c;
} String;

typedef struct {
    int a, b, type, kind, arg, spare;
    String text;
} Message;

typedef struct {
    u8 pad[20];
    void (*cb0)(int, int, int, void *);
    void (*cb1)(int, int, int, void *, u32);
    u8 gap[156];
    String first;
    u8 gap2[12];
    String second;
    u8 gap3[12];
    u8 map[4];
} Context;

extern Context *_022577C4;
extern void LobbyString_InitFromCStr(String *, void *), ov45_022320B4(String *, String *), LobbyString_Destroy(String *), LobbyCharVector_Destroy(String *), ov45_0223454C(Message *, int, int, void *, u32);
extern int LobbyString_Equals(String *, String *), ov45_0223D27C(String *, Message *);
extern u8 *PPWLobbyPlayers_Find(void *, int);
extern void *LobbyByteVector_Data(String *);
void PPWLobby_OnMessage(void *, int, void *);

static inline void zeroString(String *s) {
    u32 *p = (u32 *)s;
    *p++ = 0;
    *p++ = 0;
    *p = 0;
}

#pragma force_active on
#pragma opt_lifetimes off
#pragma opt_common_subs off

static inline int validEntry(u8 *p) {
    if (p && *(u32 *)(p + 56)) {
        return 1;
    }
    return 0;
}

void PPWLobby_OnMessage(void *key, int index, void *name) {
    Message m;
    String k, tmp, b, a;
    int language = 5;
    if (key) {
        Context *c;
        String *kp;
        int match;
        LobbyString_InitFromCStr(&k, key);
        c = _022577C4;
        kp = &k;
        ov45_022320B4(&a, &c->first);
        match = LobbyString_Equals(&a, kp);
        LobbyString_Destroy(&a);
        if (match) {
            language = c->first.a;
        } else {
            ov45_022320B4(&b, &c->second);
            match = LobbyString_Equals(&b, kp);
            LobbyString_Destroy(&b);
            if (match) {
                language = c->second.a;
            } else {
                language = 5;
            }
        }
        LobbyString_Destroy(&k);
    }
    if (key && language == 5) {
        return;
    }
    {
        int match;
        int zero = 0;
        m.a = zero;
        m.b = 6;
        m.type = 0;
        m.kind = 0;
        m.arg = 0;
        m.spare = 0;
        zeroString(&m.text);
        LobbyString_InitFromCStr(&tmp, name);
        match = ov45_0223D27C(&tmp, &m) == 0 ? 1 : zero;
        LobbyString_Destroy(&tmp);
        if (match) {
            LobbyCharVector_Destroy(&m.text);
            return;
        }
        switch (m.kind) {
        case 1: {
            u8 *p = PPWLobbyPlayers_Find(_022577C4->map, index);
            if (!validEntry(p)) {
                LobbyCharVector_Destroy(&m.text);
                return;
            }
            switch (m.type) {
            case 1: {
                Context *c = _022577C4;
                void *t = LobbyByteVector_Data(&m.text);
                c->cb1(index, language, m.arg, t, m.text.b);
                break;
            }
            case 0: {
                Context *c = _022577C4;
                void *t = LobbyByteVector_Data(&m.text);
                c->cb0(index, language, m.arg, t);
                break;
            }
            }
            break;
        }
        case 0: {
            void *t = LobbyByteVector_Data(&m.text);
            ov45_0223454C(&m, index, language, t, m.text.b);
            break;
        }
        }
        LobbyCharVector_Destroy(&m.text);
    }
}
}
}

#pragma pop

// ---- formerly src/to65_ov45_extra06_run02.c
#pragma push

namespace F32 {
extern "C" {
#pragma force_active on
void ov45_02236E58(Iterator *);
#pragma force_active on
extern int printf(const char *, ...);
extern void abort(void);
#pragma warn_uninitializedvar off

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u32 *_022577C4;
extern u8 ov45_02254FA0[], ov45_022550C4[], ov45_022550D4[], ov45_02254D7C[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void ov45_0223DF4C(void *), ov45_02238508(void *), ov45_02238558(void *, void *);
extern void ov45_022385A4(struct Vec *, u32, int), ov45_02238674(struct Vec *, u32, void *), ov45_022387E4(struct Vec *, u32, struct Alloc), ov45_02238820(struct Vec *, u32);
extern void ov45_02237800(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void ov45_022320B4(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 ov45_02237890(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void ov45_02238508(void *), ov45_02238558(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *ov45_02236AE8(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char ov45_02255120[], ov45_02255104[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void ov45_02238C1C(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char ov45_022550F4[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void PPWLobby_OnExcessFlood(int a) {
    ((void (*)(int))_022577C4[18])(a);
}

void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d) {
    PPWLobby_HandleReceivedData(1, 1, a, b, c, d, 0);
}

void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f) {
    PPWLobby_HandleReceivedData(a, 0, b, c, d, e, f);
}
}
}

#pragma pop

// ---- formerly src/to70_to70_to65_ov45_extra06_residual_3_extra341_residual_1_extra1156_run01.c
#pragma push

namespace F33 {
extern "C" {
#pragma force_active on

typedef struct {
    u32 allocated : 1;
    u32 rest : 31;
    u32 size;
    void *data;
} String;

typedef struct {
    String *data;
    u32 size, cap;
} Vec;

typedef struct {
    u32 state;
    u8 pad[0x54];
    String message;
    int failed;
} Work;

extern Work *_022577C4;
extern char ov45_022550B8[];
extern void LobbyString_InitFromCStr(String *, char *), LobbyString_Destroy(String *), ov45_02237664(Vec *), LobbyString_Assign(String *, String *), ov45_02233B74(String *);
extern void LobbyString_Split(Vec *, String *, String *, int);
void PPWLobby_OnEnumChannels(int, int, char **, int, int *);

void PPWLobby_OnEnumChannels(int valid, int count, char **names, int unused, int *types) {
    Vec parts;
    String item;
    String delimiter;
    String text;
    String empty;
    int i;
    if (!valid) {
        return;
    }
    for (i = 0; i < count; i++) {
        if (types[i] >= 20) {
            continue;
        }
        LobbyString_InitFromCStr(&item, names[i]);
        LobbyString_InitFromCStr(&delimiter, ov45_022550B8);
        LobbyString_Split(&parts, &item, &delimiter, 0);
        LobbyString_Destroy(&delimiter);
        LobbyString_Destroy(&item);
        if (parts.size < 2) {
            ov45_02237664(&parts);
            continue;
        }
        {
            String *s = parts.data + 1;
            char *p = s->allocated == 0 ? (char *)s + 1 : (char *)s->data;
            LobbyString_InitFromCStr(&text, p);
        }
        LobbyString_Assign(&_022577C4->message, &text);
        LobbyString_Destroy(&text);
        _022577C4->failed = 0;
        _022577C4->state = 6;
        ov45_02237664(&parts);
        return;
    }
    ov45_02233B74(&empty);
    LobbyString_Assign(&_022577C4->message, &empty);
    LobbyString_Destroy(&empty);
    _022577C4->failed = 1;
    _022577C4->state = 6;
}
}
}

#pragma pop

// ---- formerly src/op90_to70_to70_to65_ov45_extra06_residual_3_extra341_residual_1_extra1156_residual_1_r01.cpp
#pragma push

namespace F34 {
struct Str;
extern "C" Str *LobbyString_InitCopy(Str *, const Str *);
extern "C" void LobbyString_Destroy(Str *);
extern "C" void LobbyString_Assign(Str *, const Str *);
extern "C" int LobbyString_Find(const Str *, const Str *, int);
extern "C" void LobbyString_Substr(Str *, const Str *, int, int);

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

    Str(const Str &o) {
        LobbyString_InitCopy(this, &o);
    }

    u32 size() const {
        return !raw.l.flag ? raw.s.size : raw.l.size;
    }

    void Destroy() {
        LobbyString_Destroy(this);
    }
};

class Alloc {};
struct Vec;
extern "C" void ov45_02237634(Vec *, const Str &, Alloc);

struct Vec {
    Str *data;
    u32 count, cap;

    Alloc get_alloc() const {
        return Alloc();
    }

    Vec()
        : data(0)
        , count(0)
        , cap(0) {
    }

    void push_back(const Str &x) {
        Alloc a = get_alloc();
        (void)a;
        if (count < cap) {
            new (data + count) Str(x);
            count++;
        } else {
            ov45_02237634(this, x, Alloc());
        }
    }
};

extern "C" Vec LobbyString_Split(const Str &s, const Str &delim, int max);
#pragma force_active on

extern "C" Vec LobbyString_Split(const Str &s, const Str &delim, int max) {
    Vec out;
    int n = 1;
    Str rest(s);
    int pos;
    while (n != max && (pos = LobbyString_Find(&rest, &delim, 0)) != -1) {
        if (pos != 0) {
            Str piece;
            LobbyString_Substr(&piece, &rest, 0, pos);
            out.push_back(piece);
            piece.Destroy();
            n++;
        }
        Str tmp;
        LobbyString_Substr(&tmp, &rest, pos + delim.size(), -1);
        LobbyString_Assign(&rest, &tmp);
        tmp.Destroy();
    }
    if (rest.size() != 0) {
        out.push_back(rest);
    }
    rest.Destroy();
    return out;
}

}

#pragma pop

// ---- formerly src/to70_to65_ov45_extra06_residual_3_extra341_run01.c
#pragma push

namespace F35 {
extern "C" {
#pragma force_active on
#pragma force_active on
void LobbyString_InitSubstring(void *, void *, int, int);
void LobbyString_Substr(void *, void *, int, int);

void LobbyString_Substr(void *a, void *b, int c, int d) {
    void *volatile copy = b;
    LobbyString_InitSubstring(a, b, c, d);
}
}
}

#pragma pop

// ---- formerly src/op90_to70_to65_ov45_extra06_residual_3_extra341_residual_2_r01.cpp
#pragma push

namespace F36 {
struct Str;
extern "C" Str *LobbyString_InitFromCStr(Str *, char *);
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" Str *LobbyString_InitCopy(Str *, const Str *);

struct Str {
    u32 words[3];

    Str(char *s) {
        LobbyString_InitFromCStr(this, s);
    }

    Str(const Str &o) {
        LobbyString_InitCopy(this, &o);
    }

    void Destroy() {
        LobbyString_Destroy(this);
    }
};

struct Entry {
    int id;
};

struct Work {
    u8 pad0[0x4c];
    int self;
    u8 pad50[0xb8 - 0x50];
    Entry a;
    u8 padbc[0xc8 - 0xbc];
    int flag;
    u8 padcc[0xd0 - 0xcc];
    Entry b;
    u8 padd4[0xe8 - 0xd4];
    u8 sessions[4];
};

extern "C" Work *_022577C4;
extern "C" char ov45_022550C4[], ov45_022550D4[];
extern "C" Str ov45_022320B4(Entry *);
extern "C" int LobbyString_Equals(const Str &, const Str &);
extern "C" void PPWLobbyPlayers_Add(void *, int);
extern "C" void LobbyChat_GetChannelKeysAsync(char *, int, char *, void (*)(), int);
extern "C" void PPWLobby_OnGetChannelKeys();
extern "C" void PPWLobby_OnEnumUsers(int, char *, int, int *);

inline int Find(Work *w, const Str &name) {
    const Str &n = name;
    int eq;
    {
        Str t = ov45_022320B4(&w->a);
        eq = LobbyString_Equals(t, n);
        t.Destroy();
    }
    if (eq) {
        return w->a.id;
    }
    {
        Str t = ov45_022320B4(&w->b);
        eq = LobbyString_Equals(t, n);
        t.Destroy();
    }
    if (eq) {
        return w->b.id;
    }
    return 5;
}

inline bool IsSelf(int id) {
    return id == _022577C4->self;
}

#pragma force_active on
#pragma opt_common_subs off

extern "C" void PPWLobby_OnEnumUsers(int a, char *channel, int count, int *ids) {
    int result, i;
    if (a == 0) {
        return;
    }
    {
        Str name(channel);
        Work *w = _022577C4;
        result = Find(w, name);
        name.Destroy();
    }
    if (result == 5) {
        return;
    }
    if (result != 0) {
        return;
    }
    for (i = 0; i < count; i++) {
        if (!IsSelf(ids[i])) {
            PPWLobbyPlayers_Add(_022577C4->sessions, ids[i]);
            LobbyChat_GetChannelKeysAsync(channel, ids[i], ov45_022550C4, PPWLobby_OnGetChannelKeys, 0);
            LobbyChat_GetChannelKeysAsync(channel, ids[i], ov45_022550D4, PPWLobby_OnGetChannelKeys, 0);
        }
    }
    _022577C4->flag = 1;
}

#pragma opt_common_subs reset

}

#pragma pop

// ---- formerly src/to65_ov45_extra06_run03.c
#pragma push

namespace F37 {
extern "C" {
#pragma force_active on
void ov45_02236E58(Iterator *);
#pragma force_active on
extern int printf(const char *, ...);
extern void abort(void);
#pragma warn_uninitializedvar off

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u32 *_022577C4;
extern u8 ov45_02254FA0[], ov45_022550C4[], ov45_022550D4[], ov45_02254D7C[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void ov45_0223DF4C(void *), ov45_02238508(void *), ov45_02238558(void *, void *);
extern void ov45_022385A4(struct Vec *, u32, int), ov45_02238674(struct Vec *, u32, void *), ov45_022387E4(struct Vec *, u32, struct Alloc), ov45_02238820(struct Vec *, u32);
extern void ov45_02237800(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void ov45_022320B4(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 ov45_02237890(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void ov45_02238508(void *), ov45_02238558(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *ov45_02236AE8(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char ov45_02255120[], ov45_02255104[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void ov45_02238C1C(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char ov45_022550F4[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void PPWLobby_OnGetChannelMode(int a, int b, u32 *c) {
    int v;
    if (!a) {
        return;
    }
    if (c[7] == 20) {
        _022577C4[51] = 1;
        v = 4;
        LobbyTimerMap_EraseKey((u8 *)_022577C4 + 308, &v);
        return;
    }
    LobbyChat_SetChannelMode(b, (char *)ov45_02254D7C);
}
}
}

#pragma pop

// ---- formerly src/op90_to70_to65_ov45_extra06_residual_4_extra1124_residual_1_r01.cpp
#pragma push

namespace F38 {
struct Vec;
extern "C" Vec *LobbyCharVector_Destroy(Vec *);

struct Vec {
    char *ptr;
    u32 size, cap;

    Vec()
        : ptr(0)
        , size(0)
        , cap(0) {
    }

    void Destroy() {
        LobbyCharVector_Destroy(this);
    }
};

struct Date {
    u32 year, month, day, week;
};

struct Time {
    u32 hour, minute, second;
};

struct Stamp {
    s64 v;
};

struct DateTime {
    Date date;
    Time time;
};

struct Work {
    u8 pad[0x134];
    u8 list[0x148 - 0x134];
    int valid;
    Stamp stamp;

    void SetStamp(const Stamp &t) {
        valid = 1;
        stamp = t;
    }
};

extern "C" Work *_022577C4;
extern "C" char ov45_022550E4[];
extern "C" int PPWLobby_DecodeMessageData(const char *, Vec *);
extern "C" void DWC_GetDateTime(Date *, Time *);
extern "C" s64 RTC_ConvertDateTimeToSecond(const Date *, const Time *);
extern "C" void PPWLobby_SetChannelData(int, int, char *, Stamp *, int);
extern "C" char *LobbyByteVector_Data(Vec *);
extern "C" void LobbyTimerMap_EraseKey(void *, int *);
extern "C" void PPWLobby_OnGetChannelTime(int, int, int, int, const char *);
#pragma force_active on

extern "C" void PPWLobby_OnGetChannelTime(int result, int unused1, int unused2, int unused3, const char *value) {
    if (result == 0) {
        return;
    }
    Vec data;
    if (PPWLobby_DecodeMessageData(value, &data) == 0 || data.size != 8) {
        Work *w = _022577C4;
        DateTime now;
        DWC_GetDateTime(&now.date, &now.time);
        w->stamp.v = RTC_ConvertDateTimeToSecond(&now.date, &now.time);
        Stamp s;
        s = w->stamp;
        PPWLobby_SetChannelData(-1, 0, ov45_022550E4, &s, 8);
        data.Destroy();
        return;
    }
    {
        Stamp s = *(Stamp *)LobbyByteVector_Data(&data);
        Work *w = _022577C4;
        w->valid = 1;
        w->stamp = s;
        int state = 5;
        LobbyTimerMap_EraseKey(_022577C4->list, &state);
    }
    data.Destroy();
}

}

#pragma pop

// ---- formerly src/to70_to65_ov45_extra06_residual_4_extra1124_run01.c
#pragma push

namespace F39 {
extern "C" {
#pragma force_active on

typedef struct {
    void *ptr;
    u32 size, capacity;
} Blob;

typedef struct {
    u32 lo, hi;
} Pair;

typedef struct {
    u8 p[0x134];
    int state;
    u8 q[0x144 - 0x138];
    u32 stored, flag;
    Pair time;
} Work;

extern Work *_022577C4;
extern u8 ov45_022550E4[], ov45_022550F4[];
extern int PPWLobby_DecodeMessageData(void *, Blob *), PPWLobbySchedule_Load(u32 *, void *, u32);
extern void *LobbyByteVector_Data(Blob *);
extern void LobbyCharVector_Destroy(Blob *), PPWLobby_SetChannelData(int, int, void *, void *, int), LobbyTimerMap_EraseKey(int *, int *), ov45_02238C1C(Blob *, u32 *), DWC_GetDateTime(RTCDate *, RTCTime *);

static inline int appendBlob(Blob *blob) {
    Work *w = _022577C4;
    return PPWLobbySchedule_Load(&w->stored, LobbyByteVector_Data(blob), blob->size);
}

void PPWLobby_OnGetChannelTime(int, int, int, int, void *), PPWLobby_OnGetLobbyData(int, int, int, int, void *);

void PPWLobby_OnGetLobbyData(int ok, int a, int b, int c, void *data) {
    Blob blob, copy;
    int state;
    if (ok == 0) {
        return;
    }
    blob.ptr = 0;
    blob.size = 0;
    blob.capacity = 0;
    if (!PPWLobby_DecodeMessageData(data, &blob) || blob.size == 0 || !appendBlob(&blob)) {
        ov45_02238C1C(&copy, &_022577C4->stored);
        PPWLobby_SetChannelData(-1, 0, ov45_022550F4, LobbyByteVector_Data(&copy), copy.size);
        LobbyCharVector_Destroy(&copy);
        LobbyCharVector_Destroy(&blob);
        return;
    }
    _022577C4->stored = 4;
    state = 6;
    LobbyTimerMap_EraseKey(&_022577C4->state, &state);
    LobbyCharVector_Destroy(&blob);
}
}
}

#pragma pop

// ---- formerly src/to65_ov45_extra06_run04.c
#pragma push

namespace F40 {
extern "C" {
#pragma force_active on
void ov45_02236E58(Iterator *);
#pragma force_active on
extern int printf(const char *, ...);
extern void abort(void);
#pragma warn_uninitializedvar off

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u32 *_022577C4;
extern u8 ov45_02254FA0[], ov45_022550C4[], ov45_022550D4[], ov45_02254D7C[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void ov45_0223DF4C(void *), ov45_02238508(void *), ov45_02238558(void *, void *);
extern void ov45_022385A4(struct Vec *, u32, int), ov45_02238674(struct Vec *, u32, void *), ov45_022387E4(struct Vec *, u32, struct Alloc), ov45_02238820(struct Vec *, u32);
extern void ov45_02237800(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void ov45_022320B4(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 ov45_02237890(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void ov45_02238508(void *), ov45_02238558(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *ov45_02236AE8(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char ov45_02255120[], ov45_02255104[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void ov45_02238C1C(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char ov45_022550F4[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void PPWLobby_OnTopicChanged(void) {
}

void *ov45_02236A48(u8 *a) {
    LobbyCharVector_Destroy((struct Vec *)(a + 40));
    LobbyString_Destroy((struct Str *)(a + 24));
    LobbyString_Destroy((struct Str *)(a + 12));
    return a;
}

void *ov45_02236A70(u8 *a) {
    *(u8 **)a = ov45_02254FA0;
    ov45_0223DF4C(a);
    LobbyCharVector_Destroy((struct Vec *)(a + 40));
    ov45_02236B88((struct Tree *)(a + 4));
    if (a) {
        DWC_Free(0, a, 0);
    }
    return a;
}

void *ov45_02236AB8(u8 *a) {
    LobbyCharVector_Destroy((struct Vec *)(a + 24));
    if (a) {
        DWC_Free(0, a, 0);
    }
    return a;
}
}
}

#pragma pop

// ---- formerly src/to80_extra1643_run01.c
#pragma push

namespace F41 {
extern "C" {
#pragma force_active on

typedef struct Queue {
    u32 a;
    volatile u32 b;
    void **base;
    u32 start;
    u32 count, capacity;
} Queue;

extern void DWC_Free(int, void *, int), ov45_02238508(void *);
Queue *ov45_02236AE8(Queue *);

Queue *ov45_02236AE8(Queue *p) {
    void **end, **cursor, **base, **limit;
    u32 capacity, start;
    u32 index = 0;
    volatile u8 tag = 0;
    p->b = 0;
    if (p->count) {
        if (p->capacity) {
            index = (p->start + ((volatile Queue *)p)->count) % p->capacity;
        }
        base = p->base;
        start = p->start;
        capacity = ((volatile Queue *)p)->capacity;
        end = base + index;
        cursor = base + start;
        limit = base + capacity;
        while (cursor != end) {
            if (*cursor) {
                DWC_Free(0, *cursor, 0);
            }
            if (++cursor == limit) {
                cursor = base;
            }
        }
    }
    ov45_02238508(&p->base);
    return p;
}
}
}

#pragma pop

// ---- formerly src/to65_ov45_extra06_run05.c
#pragma push

namespace F42 {
extern "C" {
#pragma force_active on
void ov45_02236E58(Iterator *);
#pragma force_active on
extern int printf(const char *, ...);
extern void abort(void);
#pragma warn_uninitializedvar off

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u32 *_022577C4;
extern u8 ov45_02254FA0[], ov45_022550C4[], ov45_022550D4[], ov45_02254D7C[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void ov45_0223DF4C(void *), ov45_02238508(void *), ov45_02238558(void *, void *);
extern void ov45_022385A4(struct Vec *, u32, int), ov45_02238674(struct Vec *, u32, void *), ov45_022387E4(struct Vec *, u32, struct Alloc), ov45_02238820(struct Vec *, u32);
extern void ov45_02237800(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void ov45_022320B4(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 ov45_02237890(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void ov45_02238508(void *), ov45_02238558(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *ov45_02236AE8(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char ov45_02255120[], ov45_02255104[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void ov45_02238C1C(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char ov45_022550F4[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void *ov45_02236B88(struct Tree *a) {
    if (a->root) {
        ov45_02238558(a, a->root);
    }
    return a;
}
}
}

#pragma pop

// ---- formerly src/to80_extra1644_run01.c
#pragma push

namespace F43 {
extern "C" {
#pragma force_active on

typedef struct Node {
    char pad[12];
    u32 value;
} Node;

typedef struct Result {
    Node *node;
    u8 flag;
    char pad[3];
} Result;

extern int LobbyTimerMap_FindInsertPos(void *, u32 *, Node **, u8 *, u8 *);
extern Node *LobbyTimerMap_InsertNode(void *, int, u8, u8, u32 *);
void LobbyTimerMap_Insert(Result *, void *, u32 *);

void LobbyTimerMap_Insert(Result *r, void *p, u32 *v) {
    u8 second, first;
    Node *node;
    int result = LobbyTimerMap_FindInsertPos(p, v, &node, &second, &first);
    if (!node || node->value < *v) {
        r->node = LobbyTimerMap_InsertNode(p, result, second, first, v);
        r->flag = ov45_02254F5A;
    } else {
        r->node = node;
        r->flag = _02254F58;
    }
}
}
}

#pragma pop

// ---- formerly src/to61_overlay_45_arm_residual_1_run01.c
#pragma push

namespace F44 {
extern "C" {
#pragma force_active on
void ov45_02236E58(Iterator *);
#pragma force_active on
extern int printf(const char *, ...);
extern void abort(void);
#pragma warn_uninitializedvar off

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u32 *_022577C4;
extern u8 ov45_02254FA0[], ov45_022550C4[], ov45_022550D4[], ov45_02254D7C[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void ov45_0223DF4C(void *), ov45_02238508(void *), ov45_02238558(void *, void *);
extern void ov45_022385A4(struct Vec *, u32, int), ov45_02238674(struct Vec *, u32, void *), ov45_022387E4(struct Vec *, u32, struct Alloc), ov45_02238820(struct Vec *, u32);
extern void ov45_02237800(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void ov45_022320B4(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 ov45_02237890(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void ov45_02238508(void *), ov45_02238558(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *ov45_02236AE8(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char ov45_02255120[], ov45_02255104[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void ov45_02238C1C(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char ov45_022550F4[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node) {
    struct TreeNode *old = (struct TreeNode *)node.current;
    if (old == a->end) {
        ov45_02236E58(&node);
        a->end = node.current;
    }
    result->current = LobbyTree_EraseRebalance(old, a->root);
    if (old) {
        DWC_Free(0, old, 0);
    }
    a->count--;
}

void LobbyTimerMap_Clear(struct Tree *a) {
    if (!a->root) {
        return;
    }
    LobbyTimerMap_DestroyNodes(a, (struct TreeNode *)a->root);
    a->count = 0;
    a->root = 0;
    a->end = (struct TreeNode *)&a->root;
}
}
}

#pragma pop

// ---- formerly src/op90_to61_overlay_45_arm_residual_1_residual_2_r01.cpp
#pragma push

namespace F45 {
extern "C" int printf(const char *, ...);
extern "C" void abort(void);
extern "C" char ov45_02255104[];
extern "C" void *LobbyMem_Alloc(u32);
extern "C" void DWC_Free(int, void *, int);

class Alloc {};

struct Pair8 {
    u64 v;
};

struct Inner {
    u64 x;
    u64 y;
    u32 p;
    u32 q;
};

template <class A, class B>
struct Pair {
    A first;
    B second;

    Pair(const A &a, const B &b)
        : first(a) {
        B *dst = &second;
        *dst = b;
    }
};

typedef Pair<u32, Inner> Value;

struct Node {
    Node *left, *right, *parent;
    Value val;
};

struct Tree {
    u32 count;
    Node *root;
    u32 unk8;
    Node *leftmost;
};

class Holder {
public:
    Node *a;
    Node *b;
    Node **alloc;
    Node *volatile ptr;

    Holder(Node *n, Node **al)
        : a(n)
        , b(n)
        , alloc(al)
        , ptr(n) {
    }

    void Destroy() {
        Node *p = ptr;
        if (p) {
            DWC_Free(0, p, 0);
        }
    }

    void Release() {
        ptr = 0;
    }
};

inline Node *Allocate(Alloc) {
    return (Node *)LobbyMem_Alloc(sizeof(Node));
}

extern "C" void LobbyTree_InsertRebalance(Node *, Node *);
extern "C" Node *LobbyTimerMap_InsertNode(Tree *, Node *, int, int, Value *);
#pragma force_active on

extern "C" Node *LobbyTimerMap_InsertNode(Tree *t, Node *parent, int left, int leftmost, Value *v) {
    if (t->count == (u32)-1) {
        printf(ov45_02255104);
        abort();
    }
    Node *n = Allocate(Alloc());
    Holder h(n, &t->root);
    new (&n->val) Value(v->first, v->second);
    n->right = 0;
    h.Release();
    n->left = 0;
    new (&n->parent) Node *(parent);
    if (left) {
        parent->left = n;
    } else {
        parent->right = n;
    }
    t->count++;
    LobbyTree_InsertRebalance(n, t->root);
    if (leftmost) {
        t->leftmost = n;
    }
    h.Destroy();
    return n;
}

}

#pragma pop

// ---- formerly src/to61_overlay_45_arm_residual_1_run02.c
#pragma push

namespace F46 {
extern "C" {
#pragma force_active on
void ov45_02236E58(Iterator *);
#pragma force_active on
extern int printf(const char *, ...);
extern void abort(void);
#pragma warn_uninitializedvar off

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u32 *_022577C4;
extern u8 ov45_02254FA0[], ov45_022550C4[], ov45_022550D4[], ov45_02254D7C[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void ov45_0223DF4C(void *), ov45_02238508(void *), ov45_02238558(void *, void *);
extern void ov45_022385A4(struct Vec *, u32, int), ov45_02238674(struct Vec *, u32, void *), ov45_022387E4(struct Vec *, u32, struct Alloc), ov45_02238820(struct Vec *, u32);
extern void ov45_02237800(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void ov45_022320B4(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 ov45_02237890(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void ov45_02238508(void *), ov45_02238558(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *ov45_02236AE8(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char ov45_02255120[], ov45_02255104[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void ov45_02238C1C(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char ov45_022550F4[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b) {
    if (b->left) {
        LobbyTimerMap_DestroyNodes(a, (struct TreeNode *)b->left);
    }
    if (b->right) {
        LobbyTimerMap_DestroyNodes(a, (struct TreeNode *)b->right);
    }
    if (b) {
        DWC_Free(0, b, 0);
    }
}

void *LobbyTimerMap_Destroy(struct Tree *a) {
    if (a->root) {
        LobbyTimerMap_DestroyNodes(a, (struct TreeNode *)a->root);
    }
    return a;
}
}
}

#pragma pop

// ---- formerly lib/NitroDWC/src/to91_ov45_iterator_advance108.cpp
#pragma push

namespace F47 {
#pragma force_active on

static inline void *Left(void *node) {
    return *reinterpret_cast<void **>(static_cast<unsigned char *>(node));
}

static inline void *Right(void *node) {
    return *reinterpret_cast<void **>(static_cast<unsigned char *>(node) + 4);
}

static inline void *Parent(void *node) {
    return reinterpret_cast<void *>(*reinterpret_cast<u32 *>(static_cast<unsigned char *>(node) + 8) & ~1UL);
}

extern "C" void ov45_02236E58(Iterator *it);

extern "C" void ov45_02236E58(Iterator *it) {
    if (Right(it->current)) {
        void *next = Right(it->current);
        void *left;
        while ((left = Left(next)) != 0) {
            next = left;
        }
        it->current = next;
        return;
    }
    void *node = it->current;
    void *parent = Parent(node);
    while (node != Left(parent)) {
        it->current = parent;
        node = it->current;
        parent = Parent(node);
    }
    it->current = parent;
}

}

#pragma pop

// ---- formerly src/to70_to61_overlay_45_arm_residual_1_residual_3_extra1104_run01.c
#pragma push

namespace F48 {
extern "C" {
#pragma force_active on
void ov45_02236E58(Iterator *);
#pragma force_active on

typedef struct Node {
    void *left, *right;
    u32 parent;
} Node;

static inline Node *Parent(Node *n) {
    return (Node *)(n->parent & ~1);
}

static inline void Set(Node **p, Node *n) {
    *p = n;
}

void ov45_02236E58(Iterator *);
extern void LobbyTree_RotateLeft(Node *, void **), LobbyTree_RotateRight(Node *, void **);

static inline int Red(Node *n) {
    return (n->parent & 1) ? 1 : 0;
}

static inline void Black(Node *n) {
    n->parent &= ~1;
}

static inline void Paint(Node *n) {
    n->parent |= 1;
}

void LobbyTree_InsertRebalance(Node *, void *);

void LobbyTree_InsertRebalance(Node *n, void *volatile root) {
    Paint(n);
    while (n != root && Red(Parent(n)) == 1) {
        Node *p = Parent(n), *g = Parent(p);
        if (p == g->left) {
            Node *u = (Node *)g->right;
            if (u && Red(u) == 1) {
                Black(p);
                Black(u);
                n = Parent(Parent(n));
                Paint(n);
            } else {
                if (n == p->right) {
                    n = p;
                    LobbyTree_RotateLeft(n, (void **)&root);
                }
                Black(Parent(n));
                Paint(Parent(Parent(n)));
                LobbyTree_RotateRight(Parent(Parent(n)), (void **)&root);
            }
        } else {
            Node *u = (Node *)g->left;
            if (u && Red(u) == 1) {
                Black(p);
                Black(u);
                n = Parent(Parent(n));
                Paint(n);
            } else {
                if (n == p->left) {
                    n = p;
                    LobbyTree_RotateRight(n, (void **)&root);
                }
                Black(Parent(n));
                Paint(Parent(Parent(n)));
                LobbyTree_RotateLeft(Parent(Parent(n)), (void **)&root);
            }
        }
    }
    Black((Node *)root);
}
}
}

#pragma pop

// ---- formerly src/to80_extra1731_run01.c
#pragma push

namespace F49 {
extern "C" {
#pragma force_active on
void ov45_02236E58(Iterator *);

typedef struct Node {
    struct Node *left, *right;
    u32 tagged;
} Node;

extern void ov45_02236E58(Iterator *), LobbyTree_RotateLeft(Node *, void **), LobbyTree_RotateRight(Node *, void **);
Node *LobbyTree_EraseRebalance(Node *, void *);

static inline Node *parent(Node *n) {
    return (Node *)(n->tagged & ~1);
}

static inline int red(Node *n) {
    if (*(volatile u32 *)&n->tagged & 1) {
        return 1;
    }
    return 0;
}

static inline void setParent(Node *n, Node *p) {
    n->tagged = (u32)p | (n->tagged & 1);
}

static inline void setRed(Node *n, int r) {
    u32 t = n->tagged;
    if (r) {
        t |= 1;
    } else {
        t &= ~1;
    }
    n->tagged = t;
}

static inline int redLoaded(Node *n, u32 *tag) {
    u32 t = *(volatile u32 *)&n->tagged;
    *tag = t;
    if (t & 1) {
        return 1;
    }
    return 0;
}

Node *LobbyTree_EraseRebalance(Node *node, void *root) {
    Iterator iter = { node };
    Node *removed, *child, *up;
    int left, fix;
    ov45_02236E58(&iter);
    removed = node;
    if (node->left && node->right) {
        removed = (Node *)iter.current;
    }
    child = removed->left;
    if (!child) {
        child = removed->right;
    }
    up = parent(removed);
    if (child) {
        setParent(child, up);
    }
    {
        Node *p = parent(removed);
        if (removed == p->left) {
            p->left = child;
            left = 1;
        } else {
            p->right = child;
            left = 0;
        }
    }
    fix = !red(removed);
    if (removed != node) {
        Node *p = parent(node);
        setParent(removed, p);
        p = parent(removed);
        if (node == p->left) {
            p->left = removed;
        } else {
            p->right = removed;
        }
        {
            Node *c = node->left;
            removed->left = c;
            if (!c) {
                up = removed;
            } else {
                setParent(c, removed);
            }
        }
        {
            Node *c = node->right;
            removed->right = c;
            if (!c) {
                up = removed;
            } else {
                setParent(c, removed);
            }
        }
        setRed(removed, red(node));
        if (root == node) {
            root = removed;
        }
    } else if (!child && node == root) {
        fix = 0;
    }
    if (fix) {
        while (child != root && (!child || !red(child))) {
            Node *sibling;
            u32 colorword;
            if (left) {
                sibling = up->right;
                if (redLoaded(sibling, &colorword) == 1) {
                    sibling->tagged = colorword & ~1;
                    setRed(up, 1);
                    LobbyTree_RotateLeft(up, &root);
                    sibling = up->right;
                }
                if ((!sibling->left || !red(sibling->left)) && (!sibling->right || !red(sibling->right))) {
                    setRed(sibling, 1);
                    child = up;
                    up = parent(up);
                    left = (child == up->left);
                } else {
                    if (!sibling->right || !red(sibling->right)) {
                        setRed(sibling->left, 0);
                        setRed(sibling, 1);
                        LobbyTree_RotateRight(sibling, &root);
                        sibling = up->right;
                    }
                    setRed(sibling, red(up));
                    setRed(up, 0);
                    setRed(sibling->right, 0);
                    LobbyTree_RotateLeft(up, &root);
                    child = (Node *)root;
                }
            } else {
                sibling = up->left;
                if (redLoaded(sibling, &colorword) == 1) {
                    sibling->tagged = colorword & ~1;
                    setRed(up, 1);
                    LobbyTree_RotateRight(up, &root);
                    sibling = up->left;
                }
                if ((!sibling->left || !red(sibling->left)) && (!sibling->right || !red(sibling->right))) {
                    setRed(sibling, 1);
                    child = up;
                    up = parent(up);
                    left = (child == up->left);
                } else {
                    if (!sibling->left || !red(sibling->left)) {
                        setRed(sibling->right, 0);
                        setRed(sibling, 1);
                        LobbyTree_RotateLeft(sibling, &root);
                        sibling = up->left;
                    }
                    setRed(sibling, red(up));
                    setRed(up, 0);
                    setRed(sibling->left, 0);
                    LobbyTree_RotateRight(up, &root);
                    child = (Node *)root;
                }
            }
        }
        if (child) {
            setRed(child, 0);
        }
    }
    return (Node *)iter.current;
}
}
}

#pragma pop

// ---- formerly src/to61_overlay_45_arm_residual_1_run03.c
#pragma push

namespace F50 {
extern "C" {
#pragma force_active on
void ov45_02236E58(Iterator *);
#pragma force_active on
extern int printf(const char *, ...);
extern void abort(void);
#pragma warn_uninitializedvar off

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u32 *_022577C4;
extern u8 ov45_02254FA0[], ov45_022550C4[], ov45_022550D4[], ov45_02254D7C[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void ov45_0223DF4C(void *), ov45_02238508(void *), ov45_02238558(void *, void *);
extern void ov45_022385A4(struct Vec *, u32, int), ov45_02238674(struct Vec *, u32, void *), ov45_022387E4(struct Vec *, u32, struct Alloc), ov45_02238820(struct Vec *, u32);
extern void ov45_02237800(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void ov45_022320B4(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 ov45_02237890(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void ov45_02238508(void *), ov45_02238558(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *ov45_02236AE8(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char ov45_02255120[], ov45_02255104[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void ov45_02238C1C(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char ov45_022550F4[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void LobbyTree_RotateLeft(struct TreeNode *a, void **root) {
    struct TreeNode *b = (struct TreeNode *)a->right, *c, *parent;
    if (*root == a) {
        *root = b;
    }
    a->right = b->left;
    c = (struct TreeNode *)b->left;
    if (c) {
        c->parent = (u32)a | (c->parent & 1);
    }
    b->parent = (a->parent & ~1) | (b->parent & 1);
    parent = (struct TreeNode *)(a->parent & ~1);
    if (a == parent->left) {
        parent->left = b;
    } else {
        parent->right = b;
    }
    b->left = a;
    a->parent = (u32)b | (a->parent & 1);
}

void LobbyTree_RotateRight(struct TreeNode *a, void **root) {
    struct TreeNode *b = (struct TreeNode *)a->left, *c, *parent;
    if (*root == a) {
        *root = b;
    }
    a->left = b->right;
    c = (struct TreeNode *)b->right;
    if (c) {
        c->parent = (u32)a | (c->parent & 1);
    }
    b->parent = (a->parent & ~1) | (b->parent & 1);
    parent = (struct TreeNode *)(a->parent & ~1);
    if (a == parent->left) {
        parent->left = b;
    } else {
        parent->right = b;
    }
    b->right = a;
    a->parent = (u32)b | (a->parent & 1);
}

void ov45_02237634(struct Vec *a, void *b, struct Alloc unused) {
    struct Alloc alloc = { 0 };
    ov45_022385A4(a, 1, 0);
    ov45_02238674(a, 1, b);
}
}
}

#pragma pop

// ---- formerly src/to90_extra2321_run01.c
#pragma push

namespace F51 {
extern "C" {
#pragma force_active on
#pragma force_active on

typedef struct Tag {
    u8 value;
} Tag;

void ov45_022387E4(u32 *, u32, Tag, u32);
void DWC_Free(u32, u32, u32);
u32 *ov45_02237664(u32 *);

static inline Tag tag(void) {
    Tag t;
    t.value = 0;
    return t;
}

u32 *ov45_02237664(u32 *p) {
    if (p[0]) {
        u32 current = p[1];
        Tag t = { 0 };
        ov45_022387E4(p, current, t, 0);
        if (p[0]) {
            DWC_Free(0, p[0], 0);
        }
    }
    return p;
}
}
}

#pragma pop

// ---- formerly src/to65_ov45_extra07_run01.c
#pragma push

namespace F52 {
extern "C" {
#pragma force_active on
void ov45_02236E58(Iterator *);
#pragma force_active on
extern int printf(const char *, ...);
extern void abort(void);
#pragma warn_uninitializedvar off

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u32 *_022577C4;
extern u8 ov45_02254FA0[], ov45_022550C4[], ov45_022550D4[], ov45_02254D7C[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void ov45_0223DF4C(void *), ov45_02238508(void *), ov45_02238558(void *, void *);
extern void ov45_022385A4(struct Vec *, u32, int), ov45_02238674(struct Vec *, u32, void *), ov45_022387E4(struct Vec *, u32, struct Alloc), ov45_02238820(struct Vec *, u32);
extern void ov45_02237800(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void ov45_022320B4(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 ov45_02237890(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void ov45_02238508(void *), ov45_02238558(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *ov45_02236AE8(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char ov45_02255120[], ov45_02255104[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void ov45_02238C1C(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char ov45_022550F4[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

struct Vec *ov45_022376BC(struct Vec *a) {
    if (a->ptr) {
        u32 size = a->size;
        struct Alloc alloc = { 0 };
        a->size -= size;
        if (a->ptr) {
            DWC_Free(0, a->ptr, 0);
        }
    }
    return a;
}

struct Vec *ov45_0223770C(struct Vec *a) {
    if (a->ptr) {
        u32 size = a->size;
        struct Alloc alloc = { 0 };
        a->size -= size;
        if (a->ptr) {
            DWC_Free(0, a->ptr, 0);
        }
    }
    return a;
}

struct Vec *ov45_0223775C(struct Vec *a) {
    if (a->ptr) {
        u32 size = a->size;
        struct Alloc alloc = { 0 };
        a->size -= size;
        if (a->ptr) {
            DWC_Free(0, a->ptr, 0);
        }
    }
    return a;
}
}
}

#pragma pop

// ---- formerly src/op90_to65_ov45_extra07_residual_2_r01.cpp
#pragma push

namespace F53 {
#pragma force_active on
extern "C" void *memset(void *, int, unsigned);

class Alloc {};

class TrueType {};

struct Vec {
    char *ptr;
    u32 size, cap;
};

extern "C" void ov45_02237800(Vec *, u32, Alloc);
extern "C" void LobbyCharVector_Grow(Vec *, u32);

inline void FillN(char *p, u32 n, char c, TrueType) {
    memset(p, c, n);
}

extern "C" void LobbyCharVector_Grow(Vec *v, u32 n) {
    if (n) {
        ov45_02237800(v, n, Alloc());
        FillN(v->ptr + v->size, n, char(), TrueType());
        v->size += n;
    }
}

}

#pragma pop

// ---- formerly src/to65_ov45_extra07_run02.c
#pragma push

namespace F54 {
extern "C" {
#pragma force_active on
void ov45_02236E58(Iterator *);
#pragma force_active on
extern int printf(const char *, ...);
extern void abort(void);
#pragma warn_uninitializedvar off

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};

struct Vec {
    void *ptr;
    u32 size, cap;
};

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u32 *_022577C4;
extern u8 ov45_02254FA0[], ov45_022550C4[], ov45_022550D4[], ov45_02254D7C[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern struct Str *LobbyString_Destroy(struct Str *);
extern struct Vec *LobbyCharVector_Destroy(struct Vec *);
extern struct Str *LobbyString_InitSubstring(struct Str *, struct Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void ov45_0223DF4C(void *), ov45_02238508(void *), ov45_02238558(void *, void *);
extern void ov45_022385A4(struct Vec *, u32, int), ov45_02238674(struct Vec *, u32, void *), ov45_022387E4(struct Vec *, u32, struct Alloc), ov45_02238820(struct Vec *, u32);
extern void ov45_02237800(struct Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void ov45_022320B4(struct Str *, void *);
extern struct Str *LobbyString_InitFromCStr(struct Str *, char *);
extern int LobbyString_Equals(struct Str *, struct Str *);
extern void LobbyString_Reserve(struct Str *, u32);
extern u32 ov45_02237890(struct Vec *, u32);
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
extern struct Str *LobbyString_ReplaceRange(struct Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);

extern void ov45_02238508(void *), ov45_02238558(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(struct Str *, u32, u32, struct Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void *ov45_02236AE8(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
extern char ov45_02255120[], ov45_02255104[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, struct Vec *);
extern void *LobbyByteVector_Data(struct Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void ov45_02238C1C(struct Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char ov45_022550F4[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(struct Str *a, struct Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(struct Vec *a, void *b, struct Alloc unused);
struct Vec *ov45_02237664(struct Vec *a);
struct Vec *ov45_022376BC(struct Vec *a);
struct Vec *ov45_0223770C(struct Vec *a);
struct Vec *ov45_0223775C(struct Vec *a);
void LobbyCharVector_Grow(struct Vec *a, u32 n);
void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(struct Vec *a, u32 unused, u32 n);
u32 ov45_02237890(struct Vec *a, u32 n);
struct Vec *LobbyCharVector_Destroy(struct Vec *a);
struct Str *LobbyString_InitCopy(struct Str *a, struct Str *b);
struct Str *LobbyString_InitSubstring(struct Str *a, struct Str *b, u32 offset, u32 length);
struct Str *LobbyString_InitFromCStr(struct Str *a, char *b);
struct Str *LobbyString_Destroy(struct Str *a);
void LobbyString_Reserve(struct Str *a, u32 requested);
struct Str *LobbyString_Assign(struct Str *a, struct Str *b);
struct Str *LobbyString_Replace(struct Str *a, u32 b, u32 c, struct Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

void ov45_02237800(struct Vec *a, u32 b, struct Alloc unused) {
    a->ptr = LobbyMem_Alloc(b);
    a->cap = b;
}
}
}

#pragma pop

// ---- formerly src/to80_extra1913_run01.c
#pragma push

namespace F55 {
extern "C" {
#pragma force_active on
#pragma force_active on
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct {
    u8 byte;
} Allocator;

typedef struct {
    void *data;
    volatile u32 size;
    u32 capacity;
} Buffer;

extern void DWC_Free(int, void *, int), ov45_02237800(Buffer *, u32, Allocator);
void ov45_02237820(Buffer *, Allocator, u32);

void ov45_02237820(Buffer *b, Allocator incoming, u32 n) {
    u32 size = b->size;
    Allocator old = { 0 };
    volatile Allocator copy;
    (void)incoming;
    (void)old;
    b->size -= size;
    if (b->data) {
        if (b->data) {
            DWC_Free(0, b->data, 0);
        }
        b->data = 0;
        b->capacity = 0;
    }
    Allocator fresh = { 0 };
    copy.byte = 0;
    ov45_02237800(b, n, fresh);
}
}
}

#pragma pop
#undef U32
#undef U16
#undef PTR

// ---- formerly src/to61_overlay_45_arm_residual_1_run04.c
#pragma push

namespace F56 {
extern "C" {
#pragma force_active on
void ov45_02236E58(Iterator *);
#pragma force_active on
extern int printf(const char *, ...);
extern void abort(void);
#pragma warn_uninitializedvar off

struct TreeNode {
    void *left, *right;
    u32 parent;
    u32 key;
    u32 data[6];
};

struct Tree {
    u32 count;
    void *root;
    u32 unk;
    void *end;
};

struct Alloc {
    u8 tag;
};

extern u32 *_022577C4;
extern u8 ov45_02254FA0[], ov45_022550C4[], ov45_022550D4[], ov45_02254D7C[];
extern void DWC_Free(int, void *, int);
extern void *LobbyMem_Alloc(u32);
extern Str *LobbyString_Destroy(Str *);
extern Vec *LobbyCharVector_Destroy(Vec *);
extern Str *LobbyString_InitSubstring(Str *, Str *, u32, u32);
extern void PPWLobby_OnMessage(int, int, int, int);
extern void PPWLobby_HandleReceivedData(int, int, int, int, int, int, int);
extern void ov45_0223DF4C(void *), ov45_02238508(void *), ov45_02238558(void *, void *);
extern void ov45_022385A4(Vec *, u32, int), ov45_02238674(Vec *, u32, void *), ov45_022387E4(Vec *, u32, struct Alloc), ov45_02238820(Vec *, u32);
extern void ov45_02237800(Vec *, u32, struct Alloc);
extern void LobbyTree_InsertRebalance(struct TreeNode *, void *), LobbyTimerMap_DestroyNodes(struct Tree *, struct TreeNode *);
extern struct TreeNode *LobbyTree_EraseRebalance(struct TreeNode *, void *);
extern void LobbyTimerMap_EraseKey(void *, int *), LobbyChat_SetChannelMode(int, char *);
extern void PPWLobbyPlayers_Add(void *, int), LobbyChat_GetChannelKeysAsync(int, int, char *, void (*)(), int);
extern void ov45_022320B4(Str *, void *);
extern Str *LobbyString_InitFromCStr(Str *, char *);
extern int LobbyString_Equals(Str *, Str *);
extern void LobbyString_Reserve(Str *, u32);
extern u32 ov45_02237890(Vec *, u32);
extern Str *LobbyString_Replace(Str *, u32, u32, Str *);
extern Str *LobbyString_ReplaceRange(Str *, u32, u32, char *, char *, struct Alloc);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(Str *a, Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(Vec *a, void *b, struct Alloc unused);
Vec *ov45_02237664(Vec *a);
Vec *ov45_022376BC(Vec *a);
Vec *ov45_0223770C(Vec *a);
Vec *ov45_0223775C(Vec *a);
Vec *LobbyCharVector_Destroy(Vec *a);
void ov45_02237800(Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(Vec *a, u32 n);
Str *LobbyString_Destroy(Str *a);
Str *LobbyString_Assign(Str *a, Str *b);
Str *LobbyString_Replace(Str *a, u32 b, u32 c, Str *d);

extern void ov45_02238508(void *), ov45_02238558(void *, void *);
extern struct TreeNode *LobbyTimerMap_FindInsertPos(struct Tree *, u32 *, struct TreeNode **, u8 *, u8 *);
extern struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *, struct TreeNode *, int, int, u32 *);
extern void LobbyString_ReplaceSubstring(Str *, u32, u32, Str *, u32, u32);
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(Str *a, Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(Vec *a, void *b, struct Alloc unused);
Vec *ov45_02237664(Vec *a);
Vec *ov45_022376BC(Vec *a);
Vec *ov45_0223770C(Vec *a);
Vec *ov45_0223775C(Vec *a);
void ov45_02237800(Vec *a, u32 b, struct Alloc unused);
u32 ov45_02237890(Vec *a, u32 n);
Vec *LobbyCharVector_Destroy(Vec *a);
Str *LobbyString_Destroy(Str *a);
Str *LobbyString_Assign(Str *a, Str *b);
Str *LobbyString_Replace(Str *a, u32 b, u32 c, Str *d);
void *ov45_02236AE8(u32 *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyCharVector_Grow(Vec *a, u32 n);
void ov45_02237820(Vec *a, u32 unused, u32 n);
Str *LobbyString_InitCopy(Str *a, Str *b);
Str *LobbyString_InitSubstring(Str *a, Str *b, u32 offset, u32 length);
Str *LobbyString_InitFromCStr(Str *a, char *b);
extern char ov45_02255120[], ov45_02255104[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(Str *a, Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(Vec *a, void *b, struct Alloc unused);
Vec *ov45_02237664(Vec *a);
Vec *ov45_022376BC(Vec *a);
Vec *ov45_0223770C(Vec *a);
Vec *ov45_0223775C(Vec *a);
void LobbyCharVector_Grow(Vec *a, u32 n);
void ov45_02237800(Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(Vec *a, u32 unused, u32 n);
u32 ov45_02237890(Vec *a, u32 n);
Vec *LobbyCharVector_Destroy(Vec *a);
Str *LobbyString_InitCopy(Str *a, Str *b);
Str *LobbyString_InitSubstring(Str *a, Str *b, u32 offset, u32 length);
Str *LobbyString_InitFromCStr(Str *a, char *b);
Str *LobbyString_Destroy(Str *a);
Str *LobbyString_Assign(Str *a, Str *b);
Str *LobbyString_Replace(Str *a, u32 b, u32 c, Str *d);
void LobbyString_Reserve(Str *a, u32 requested);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
extern int PPWLobby_DecodeMessageData(void *, Vec *);
extern void *LobbyByteVector_Data(Vec *);
extern int PPWLobbySchedule_Load(void *, void *, u32);
extern void ov45_02238C1C(Vec *, void *), PPWLobby_SetChannelData(int, int, char *, void *, u32);
extern char ov45_022550F4[];
void PPWLobby_OnPlayerMessage(int a, int b, int c);
void PPWLobby_OnExcessFlood(int a);
void PPWLobby_OnChannelDataChanged(int a, int b, int c, int d);
void PPWLobby_OnGetChannelKeys(int a, int b, int c, int d, int e, int f);
void LobbyString_Substr(Str *a, Str *b, u32 c, u32 d);
void PPWLobby_OnGetChannelMode(int a, int b, u32 *c);
void PPWLobby_OnTopicChanged(void);
void *ov45_02236A48(u8 *a);
void *ov45_02236A70(u8 *a);
void *ov45_02236AB8(u8 *a);
void *ov45_02236AE8(u32 *a);
void *ov45_02236B88(struct Tree *a);
void LobbyTimerMap_Insert(u32 *result, struct Tree *a, u32 *value);
void LobbyTimerMap_Erase(Iterator *result, struct Tree *a, Iterator node);
void LobbyTimerMap_Clear(struct Tree *a);
struct TreeNode *LobbyTimerMap_InsertNode(struct Tree *a, struct TreeNode *parent, int left, int end, u32 *value);
void LobbyTimerMap_DestroyNodes(struct Tree *a, struct TreeNode *b);
void *LobbyTimerMap_Destroy(struct Tree *a);
void ov45_02236E58(Iterator *p);
void LobbyTree_RotateLeft(struct TreeNode *a, void **root);
void LobbyTree_RotateRight(struct TreeNode *a, void **root);
void ov45_02237634(Vec *a, void *b, struct Alloc unused);
Vec *ov45_02237664(Vec *a);
Vec *ov45_022376BC(Vec *a);
Vec *ov45_0223770C(Vec *a);
Vec *ov45_0223775C(Vec *a);
void LobbyCharVector_Grow(Vec *a, u32 n);
void ov45_02237800(Vec *a, u32 b, struct Alloc unused);
void ov45_02237820(Vec *a, u32 unused, u32 n);
u32 ov45_02237890(Vec *a, u32 n);
Vec *LobbyCharVector_Destroy(Vec *a);
Str *LobbyString_InitCopy(Str *a, Str *b);
Str *LobbyString_InitSubstring(Str *a, Str *b, u32 offset, u32 length);
Str *LobbyString_InitFromCStr(Str *a, char *b);
Str *LobbyString_Destroy(Str *a);
void LobbyString_Reserve(Str *a, u32 requested);
Str *LobbyString_Assign(Str *a, Str *b);
Str *LobbyString_Replace(Str *a, u32 b, u32 c, Str *d);
void PPWLobby_OnEnumUsers(char *name, int id, int n, int *list);
void PPWLobby_OnGetLobbyData(int ok, int unused1, int unused2, int unused3, void *value);

u32 ov45_02237890(Vec *a, u32 n) {
    u32 cap = a->cap;
    u32 growth;
    if (n > ~0u - cap) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    if (cap < 0x55555555) {
        growth = (cap + 1) * 3 / 5;
        return cap + *(n > growth ? &n : &growth);
    }
    if (cap < 0xaaaaaaaa) {
        u32 g = (cap + 1) / 2;
        return cap + *(n > g ? &n : &g);
    }
    return ~0u;
}
}
}

#pragma pop

// ---- formerly lib/NitroDWC/src/to91_ov45_vector_grow120.cpp
#pragma push
using std::memset;
using namespace DwcFormatted216Native;
#pragma force_active on

extern "C" void ov45_0223795C(Vec *vector, u32 count, ResizeDispatch) {
    u32 capacity = vector->cap;
    if (count > capacity || vector->size > capacity - count) {
        u32 additional = vector->size + count - capacity;
        Allocator allocator = Allocator();
        allocator.recommend(vector, additional);
    } else {
        memset(vector->ptr + vector->size, 0, count);
        vector->size += count;
        return;
    }
    ov45_02238820(vector, count);
}

#pragma force_active off

#pragma pop

// ---- formerly lib/NitroDWC/src/to91_ov45_vector_destroy_copy204.cpp
#pragma push
using DwcStringConcat200::RangeDispatch;
using namespace DwcFormatted216Native;
extern "C" void DWC_Free(int, void *, u32);
extern "C" Str *LobbyString_InitCopy(Str *, Str *);

namespace DwcFormatted216Native {
struct CharacterAllocator {
    void release(char *storage) {
        if (storage) {
            DWC_Free(0, storage, 0);
        }
    }
};
}

#pragma force_active on

extern "C" Vec *LobbyCharVector_Destroy(Vec *vector) {
    if (vector->ptr) {
        u32 count = vector->size;
        CharacterAllocator allocator = CharacterAllocator();
        vector->size -= count;
        allocator.release(vector->ptr);
    }
    return vector;
}

extern "C" Str *LobbyString_InitCopy(Str *output, Str *input) {
    if (!input->l.flag) {
        *output = *input;
    } else {
        u32 index;
        RangeDispatch dispatch;
        for (index = 0; index < 3; index++) {
            output->words[index] = 0;
        }
        LobbyString_Reserve(output, input->l.size);
        LobbyString_ReplaceRange(output, 0, 0, input->l.data, input->l.data + input->l.size, dispatch);
    }
    return output;
}

#pragma force_active off

#pragma pop

// ---- formerly src/op90_to65_ov45_extra08_residual_2_r01.cpp
#pragma push

namespace F59 {
#pragma force_active on

class String {
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

    u32 Size() const {
        return !raw.l.flag ? raw.s.size : raw.l.size;
    }
};

extern "C" void LobbyString_Reserve(String *, u32);
extern "C" String *LobbyString_ReplaceSubstring(String *, u32, u32, String *, u32, u32);
extern "C" String *LobbyString_InitSubstring(String *, String *, u32, u32);

template <class T>
inline const T &Min(const T &a, const T &b) {
    return a < b ? a : b;
}

extern "C" String *LobbyString_InitSubstring(String *self, String *str, u32 pos, u32 n) {
    u32 i;
    for (i = 0; i < 3; i++) {
        self->raw.words[i] = 0;
    }
    u32 size = str->Size();
    LobbyString_Reserve(self, pos <= size ? Min(size - pos, n) : 0);
    LobbyString_ReplaceSubstring(self, 0, 0, str, pos, n);
    return self;
}

}

#pragma pop

// ---- formerly lib/NitroDWC/src/to91_ov45_string_construct_destroy164.cpp
#pragma push
using DwcStringConcat200::RangeDispatch;
using std::strlen;
#pragma force_active on

extern "C" Str *LobbyString_InitFromCStr(Str *output, char *text) {
    u32 index, length;
    RangeDispatch dispatch;
    for (index = 0; index < 3; index++) {
        output->words[index] = 0;
    }
    length = strlen(text);
    LobbyString_Reserve(output, length);
    LobbyString_ReplaceRange(output, 0, 0, text, text + length, dispatch);
    return output;
}

extern "C" void DWC_Free(int, void *, u32);

extern "C" Str *LobbyString_Destroy(Str *output) {
    if (output->l.flag && output->l.data) {
        DWC_Free(0, output->l.data, 0);
    }
    return output;
}

#pragma force_active off

#pragma pop

// ---- formerly src/to90_extra2117_run01.c
#pragma push

namespace F61 {
extern "C" {
#pragma force_active on
#pragma force_active on
#define B(p, o)  (*((u8 *)(p) + (o)))
#define H(p, o)  (*(u16 *)((u8 *)(p) + (o)))
#define SH(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U(p, o)  (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o)  (*(void **)((u8 *)(p) + (o)))
extern char ov45_02255120[];
int printf(const char *, ...);
void abort(void);
void *LobbyMem_Alloc(u32);
void DWC_Free(int, void *, int);

typedef union {
    u32 raw;

    struct {
        u32 heap : 1;
        u32 capacity : 31;
    } h;

    struct {
        u8 heap : 1;
        u8 smallSize : 7;
        u8 rest[3];
    } small;
} Header;

typedef struct {
    Header header;
    u32 length;
    u8 *ptr;
} Text;

void LobbyString_Reserve(Text *w, u32 size);

void LobbyString_Reserve(Text *w, u32 size) {
    u32 len, capacity;
    u8 *dst, *old;
    int wasHeap, makeHeap;
    if (size > 0x7ffffffe) {
        printf(ov45_02255120);
        abort();
    }
    wasHeap = w->header.h.heap != 0;
    if (wasHeap) {
        capacity = w->header.raw >> 1;
        len = w->length;
    } else {
        capacity = 11;
        len = w->header.small.smallSize;
    }
    if (size < len) {
        size = len;
    }
    if (size + 1 <= 11) {
        size = 11;
    } else {
        size = (size + 16) & ~15;
    }
    if (size == capacity) {
        return;
    }
    if (size == 11) {
        dst = (u8 *)w + 1;
        old = w->ptr;
        makeHeap = 0;
    } else {
        if (size > capacity) {
            dst = (u8 *)LobbyMem_Alloc(size);
        } else {
            dst = (u8 *)LobbyMem_Alloc(size);
            if (!dst) {
                return;
            }
        }
        old = wasHeap ? w->ptr : (u8 *)w + 1;
        makeHeap = 1;
    }
    memmove(dst, old, len);
    dst[len] = 0;
    if (wasHeap && old) {
        DWC_Free(0, old, 0);
    }
    w->header.h.heap = makeHeap;
    if (!makeHeap) {
        w->header.small.smallSize = (u8)len;
    } else {
        w->length = len;
        w->ptr = dst;
        w->header.raw = (w->header.raw & 1) | (size << 1);
    }
}
}
}

#pragma pop
#undef B
#undef H
#undef SH
#undef U
#undef P

// ---- formerly src/to90_w41400_run01.c
#pragma push

namespace F62 {
extern "C" {
#pragma force_active on

struct StrRaw {
    u32 w[3];
};

struct Str {
    union {
        u32 flags;

        struct {
            u32 large : 1;
            u32 cap : 31;
        } bits;
    } u;

    u32 size;
    char *ptr;
};
extern struct Str *LobbyString_Replace(struct Str *, u32, u32, struct Str *);
struct Str *LobbyString_Assign(struct Str *, struct Str *);
#pragma force_active on

struct Str *LobbyString_Assign(struct Str *dst, struct Str *src) {
    BOOL isLong = dst->u.bits.large ? TRUE : FALSE;
    if (!isLong && !src->u.bits.large) {
        *(struct StrRaw *)dst = *(struct StrRaw *)src;
        return dst;
    }
    return LobbyString_Replace(dst, 0, isLong ? dst->size : ((u32)((const u8 *)dst)[0] << 24) >> 25, src);
}
}
}

#pragma pop

// ---- formerly lib/NitroDWC/src/to91_ov45_string_replace80.cpp
#pragma push

namespace F63 {
#pragma force_active on

namespace DwcStringConcat200 {
    union Str {
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
    };

    class RangeDispatch {};

    typedef char ActualString12[sizeof(Str) == 12 ? 1 : -1];
    typedef char ActualCarrier1[sizeof(RangeDispatch) == 1 ? 1 : -1];
}

using namespace DwcStringConcat200;
extern "C" Str *LobbyString_ReplaceRange(Str *, u32, u32, char *, char *, RangeDispatch);
extern "C" Str *LobbyString_Replace(Str *, u32, u32, Str *);

extern "C" Str *LobbyString_Replace(Str *out, u32 pos, u32 remove, Str *input) {
    char *data;
    u32 size;
    RangeDispatch dispatch;
    if (input->l.flag) {
        data = input->l.data;
        size = input->l.size;
    } else {
        data = input->s.data;
        size = input->s.size;
    }
    return LobbyString_ReplaceRange(out, pos, remove, data, data + size, dispatch);
}

}

#pragma pop

// ---- formerly src/op90_to91_ov45_string_between172_r01.cpp
#pragma push

namespace F64 {
#pragma force_active on
extern "C" int printf(const char *, ...);
extern "C" void abort(void);
extern "C" char ov45_02255144[];

class RangeDispatch {};

class String {
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
    } raw;
};

extern "C" String *LobbyString_ReplaceRange(String *, u32, u32, char *, char *, RangeDispatch);
extern "C" String *LobbyString_ReplaceSubstring(String *, u32, u32, String *, u32, u32);

template <class T>
inline const T &Min(const T &a, const T &b) {
    return a < b ? a : b;
}

extern "C" String *LobbyString_ReplaceSubstring(String *self, u32 pos, u32 n1, String *str, u32 pos2, u32 n2) {
    char *data;
    u32 size;
    RangeDispatch dispatch;
    if (str->raw.l.flag) {
        data = str->raw.l.data;
        size = str->raw.l.size;
    } else {
        data = str->raw.s.data;
        size = str->raw.s.size;
    }
    if (pos2 > size) {
        printf(ov45_02255144);
        abort();
    }
    data += pos2;
    n2 = Min(size - pos2, n2);
    return LobbyString_ReplaceRange(self, pos, n1, data, data + n2, dispatch);
}

}

#pragma pop

// ---- formerly lib/NitroDWC/src/to91_ov45_string_search52.cpp
#pragma push
#pragma force_active on

inline StringSearchRange StringSearch::GetRange() const {
    if (raw.longForm.flag) {
        return StringSearchRange(raw.longForm.data, raw.longForm.size);
    }
    return StringSearchRange(raw.shortForm.data, raw.shortForm.size);
}

inline int StringSearch::Find(const StringSearch &other, u32 pos) const {
    StringSearchRange needle = other.GetRange();
    return (int)LobbyString_FindChars(this, needle.data, pos, needle.size);
}

extern "C" int LobbyString_Find(const StringSearch *a, const StringSearch *b, u32 pos) {
    return a->Find(*b, pos);
}

#pragma pop

// ---- formerly src/to90_extra2336_run01.c
#pragma push
extern "C" {
#pragma force_active on
#pragma force_active on

u32 LobbyString_FindChars(const StringSearch *s, const s8 *needle, u32 start, u32 count) {
    u32 len, left;
    const s8 *data, *end, *at, *p, *q;
    if (s->raw.longForm.flag) {
        data = s->raw.longForm.data;
        len = s->raw.longForm.size;
    } else {
        data = s->raw.shortForm.data;
        len = s->raw.shortForm.size;
    }
    if (start > len) {
        goto fail;
    }
    left = len - start;
    end = needle + count;
    at = data + start;
    if (left < count) {
        goto fail;
    }
    do {
        p = at;
        q = needle;
        while (q < end) {
            if (*q != *p) {
                goto next;
            }
            q++;
            p++;
        }
        return at - data;
next:
        left--;
        at++;
    } while (left >= count);
fail:
    return ~0U;
}
}
#pragma pop

// ---- formerly src/op90_to90_extra2336_residual_2_r01.cpp
#pragma push

namespace F67 {
#pragma force_active on
extern "C" unsigned strlen(const char *);

class String {
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
    } raw;

    u32 Size() const {
        return !raw.l.flag ? raw.s.size : raw.l.size;
    }

    const char *Data() const {
        return !raw.l.flag ? raw.s.data : raw.l.data;
    }

    int Compare(const String &other) const;
    int Compare(const char *text) const;
};

extern "C" int LobbyString_CompareChars(const String *, u32, u32, const char *, u32);
extern "C" int LobbyString_Compare(const String *, const String *);
extern "C" int LobbyString_CompareCStr(const String *, const char *);

inline int String::Compare(const String &other) const {
    const char *d;
    u32 n;
    if (!other.raw.l.flag) {
        d = other.raw.s.data;
        n = other.raw.s.size;
    } else {
        d = other.raw.l.data;
        n = other.raw.l.size;
    }
    return LobbyString_CompareChars(this, 0, Size(), d, n);
}

inline int String::Compare(const char *text) const {
    u32 n = strlen(text);
    return LobbyString_CompareChars(this, 0, Size(), text, n);
}

extern "C" int LobbyString_Compare(const String *a, const String *b) {
    return a->Compare(*b);
}

extern "C" int LobbyString_CompareCStr(const String *a, const char *b) {
    return a->Compare(b);
}

}

#pragma pop

// ---- formerly src/to90_extra2336_run02.c
#pragma push

namespace F68 {
extern "C" {
#pragma force_active on
extern int printf(const char *, ...);
extern void abort(void);
#pragma force_active on
extern const char ov45_02255160[];
int LobbyString_CompareChars(const u8 *, u32, u32, const void *, u32);

static inline const u32 *Min(const u32 *a, const u32 *b) {
    return *a < *b ? a : b;
}

int LobbyString_CompareChars(const u8 *s, u32 pos, u32 length, const void *other, u32 otherLength) {
    const u8 *data;
    u32 size, n, remaining;
    int r;
    if (((const struct {u32 flag:1;u32 rest:31; } *)s)->flag) {
        data = *(u8 **)(s + 8);
        size = *(u32 *)(s + 4);
    } else {
        data = s + 1;
        size = ((const struct {u8 flag:1;u8 size:7; } *)s)->size;
    }
    if (pos > size) {
        printf(ov45_02255160);
        abort();
    }
    remaining = size - pos;
    n = *Min(&length, &remaining);
    r = memcmp(data + pos, other, *Min(&otherLength, &n));
    if (r) {
        return r;
    }
    if (n < otherLength) {
        return -1;
    }
    return n != otherLength;
}
}
}

#pragma pop

// ---- formerly src/op90_op90_to90_extra2336_residual_3_residual_1_r01.cpp
#pragma push

namespace F69 {
#pragma force_active on
extern "C" void *memcpy(void *, const void *, unsigned);
extern "C" void *memmove(void *, const void *, unsigned);
extern "C" int printf(const char *, ...);
extern "C" void abort(void);
extern "C" void *LobbyMem_Alloc(u32);
extern "C" void DWC_Free(int, void *, int);
extern "C" const char ov45_02255144[], ov45_0225517C[];

class Tag {};
struct Str;
extern "C" Str *LobbyString_Destroy(Str *);
extern "C" Str *LobbyString_AssignRange(Str *, const char *, const char *, int);

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
        u32 i;
        u32 *p = raw.words;
        for (i = 0; i < 3; i++) {
            p[i] = 0;
        }
    }

    void Destroy() {
        LobbyString_Destroy(this);
    }

    const char *CStr() const {
        const Str *t = this;
        return !t->raw.l.flag ? t->raw.s.data : t->raw.l.data;
    }
};

inline const u32 &Min(const u32 &a, const u32 &b) {
    return b < a ? b : a;
}

extern "C" Str *LobbyString_ReplaceRange(Str *s, u32 pos, u32 n1, const char *first, const char *last, Tag);

#pragma opt_common_subs off

extern "C" Str *LobbyString_ReplaceRange(Str *s, u32 pos, u32 n1, const char *first, const char *last, Tag) {
    bool isLong = s->raw.l.flag;
    u32 cap;
    char *data;
    u32 size;
    if (!isLong) {
        cap = 11;
        data = s->raw.s.data;
        size = s->raw.s.size;
    } else {
        cap = s->raw.l.capacity;
        size = s->raw.l.size;
        data = s->raw.l.data;
    }
    if (pos > size) {
        printf(ov45_02255144);
        abort();
    }
    u32 rem = size - pos;
    u32 n = Min(n1, rem);
    u32 n2 = last - first;
    u32 len;
    if (n2 > 0x7ffffffe || (len = size - n) > 0x7ffffffe - n2) {
        printf(ov45_0225517C);
        abort();
    }
    u32 newSize = n2 + len;
    u32 tail = size - (pos + n);
    if (newSize >= cap) {
        u32 newCap = (cap + 15) & ~15;
        while (newCap < newSize + 1) {
            newCap = (newCap * 2 + 15) & ~15;
        }
        char *p = (char *)LobbyMem_Alloc(newCap);
        if (pos) {
            memcpy(p, data, pos);
        }
        char *q = p + pos;
        memmove(q, first, last - first);
        if (tail) {
            memcpy(q + n2, data + pos + n, tail);
        }
        p[newSize] = ov45_02254F5B;
        if (isLong) {
            if (data) {
                DWC_Free(0, data, 0);
            }
        } else {
            s->raw.l.flag = 1;
        }
        s->raw.l.data = p;
        s->raw.l.size = newSize;
        s->raw.l.capacity = newCap;
    } else {
        Str tmp;
        if (tail && data + pos + n2 < last && last <= data + size) {
            LobbyString_AssignRange(&tmp, first, last, 0);
            first = tmp.CStr();
        }
        if (tail) {
            memmove(data + pos + n2, data + pos + n, tail);
        }
        memmove(data + pos, first, n2);
        data[newSize] = ov45_02254F59;
        if (isLong) {
            s->raw.l.size = newSize;
        } else {
            s->raw.s.size = newSize;
        }
        tmp.Destroy();
    }
    return s;
}

#pragma opt_common_subs reset

}

#pragma pop
