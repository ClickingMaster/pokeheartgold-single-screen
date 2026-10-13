#pragma force_active on
#pragma warn_uninitializedvar off
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct DequeBuf {
    u32 **ptr;
    u32 begin;
    u32 size;
    u32 cap;
} DequeBuf;

typedef struct Iter {
    u32 *cur;
    u32 *first;
    u32 *mid;
    u32 *last;
} Iter;

typedef struct Vec {
    void *ptr;
    u32 size;
    u32 cap;
} Vec;

typedef union Str {
    struct {
        u32 isLong : 1;
        u32 pad : 31;
        u32 size;
        char *ptr;
    } l;

    struct {
        u8 isLong : 1;
        u8 len : 7;
        char buf[11];
    } s;
} Str;

typedef struct VObj VObj;

typedef struct VTable {
    void (*f0)(VObj *);
    void (*dtor)(VObj *);
} VTable;

struct VObj {
    VTable *vt;
};

extern u32 ov45_022577CC;
extern u32 ov45_022551A0;
extern u32 ov45_022577D0;
extern u32 ov45_022551A4;
extern unsigned char __lower_mapC[];

void DWC_Free(int, void *, int);
void *LobbyMem_Alloc(u32);
void ov45_0223BD0C(void);
void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
Str *LobbyString_Replace(Str *, u32, u32, void *);
void LobbyString_ReplaceCStr(Str *, u32, u32, const char *);

typedef struct Tag {
    u8 c;
} Tag;

void LobbyString_ReplaceRange(Str *, u32, u32, const char *, const char *, Tag);
int LobbyString_CompareChars(const u8 *, u32, u32, const void *, u32);

static inline BOOL StrIsShort(const Str *s) {
    return !s->l.isLong;
}

static inline u32 StrSize(Str *s) {
    return StrIsShort(s) ? s->s.len : s->l.size;
}

static inline const char *StrData(Str *s) {
    return StrIsShort(s) ? s->s.buf : s->l.ptr;
}

typedef struct Session {
    VTable *vt;
    u32 unk4;
    s64 time;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    int state;
    int id;
    u32 unk30;
    u32 unk34;
    u32 unk38;
} Session;

typedef struct Node {
    void *left;
    void *right;
    u32 parent;
    Session *val;
} Node;

typedef struct List {
    u32 count;
    void *root;
    void *leftmost;
} List;

#include "../lib/NitroDWC/src/tree-iterator4.h"
typedef Iterator NodeIter;

typedef struct CharVec {
    char *ptr;
    u32 size;
    u32 cap;
} CharVec;

typedef struct CharSplitBuf {
    char *ptr;
    u32 size;
    u32 cap;
    u32 *alloc;
    u32 start;
} CharSplitBuf;

void ov45_022389CC(CharSplitBuf *b, u32 cap, Tag unused);
void ov45_022389B0(CharSplitBuf *b);

u32 ov45_02237890(CharVec *, u32);
void ov45_0223DB8C(CharVec *, u32, const char *);
u32 ov45_0223EA34(Vec *, u32);
void ov45_0223EB00(Vec *, u32, u32 *);
void ov45_02236E58(NodeIter *);
Node *ov45_022370D0(Node *, void *);

extern char ov45_0225520C[];
void printf(const char *, ...);
void abort(void);
void ov45_02236EC4(Node *, void *);

typedef struct InsertResult {
    Node *current;
    u8 inserted;
} InsertResult;

extern u8 ov45_02255200[];
Node *ov45_0223E940(List *l, Session **key, Node **parent, u8 *left, u8 *flag);
Node *ov45_0223E758(List *l, Node *parent, u8 left, u8 flag, Session **key);

typedef struct SessionMgr {
    u32 unk0;
    List list;
    Session self;
} SessionMgr;

void ov45_0223E6E8(NodeIter *out, List *l, NodeIter it);

typedef struct FindState {
    int id;
    Node *end;
    NodeIter it;
} FindState;

void ov45_0223E830(Vec *v, u32 n);
void ov45_0223E910(Vec *v, u32 *x, Tag unused);

static inline void VecReserve(Vec *v, u32 n) {
    if (n > v->cap) {
        ov45_0223E830(v, n);
    }
}

void ov45_0223E024(Vec *out, SessionMgr *m);
void ov45_022376BC(Vec *v);

void ov45_0223EA04(Vec *v, u32 n, Tag unused);
Vec *ov45_0223E9B4(Vec *v);

static inline void SwapU32(u32 *a, u32 *b) {
    u32 t = *a;
    *a = *b;
    *b = t;
}

static inline void SwapPtr(void **a, void **b) {
    void *t = *a;
    *a = *b;
    *b = t;
}

static inline int Distance(u32 *a, u32 *b) {
    return b - a;
}

typedef struct PMF {
    int fn;
    int adj;
} PMF;

typedef struct PmfFind {
    PMF pmf;
    Node *end;
    NodeIter it;
} PmfFind;

static inline BOOL CallPmf(PMF *p, int adj, void *o) {
    u8 *obj = (u8 *)o + (adj >> 1);
    BOOL (*fn)(void *);
    if (adj & 1) {
        fn = *(BOOL(**)(void *))(*(u8 **)obj + p->fn);
    } else {
        fn = (BOOL(*)(void *))p->fn;
    }
    return fn(obj);
}

typedef struct SessionInfo {
    u32 timeLo;
    u32 timeHi;
    u32 kind;
} SessionInfo;

typedef struct Callbacks {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    void (*onData)(int id, SessionInfo *info, const char *data, u32 size);
} Callbacks;

Callbacks *PPWLobby_GetWork(void);
const char *ov45_02231C78(void *);
void ov45_0223DCD0(Session *s);

extern VTable ov45_02254FB0;
void DWC_GetDateTime(RTCDate *date, RTCTime *time);
void ov45_0223E648(InsertResult *out, List *l, Session **key);

typedef struct PtrDeque {
    u32 unk0;
    u32 count;
    DequeBuf buf;
} PtrDeque;

void ov45_02238508(DequeBuf *);

extern char ov45_022551DC[];
extern char ov45_022551E0[];
extern char ov45_022551E4[];
extern char ov45_022551E8[];
extern char ov45_022551EC[];
extern char ov45_022551F0[];
extern char ov45_022551F4[];
extern char ov45_022551F8[];
extern char ov45_022551FC[];
void ov45_02232B24(Str *out, u32 width, const char *fmt, ...);
void LobbyString_Destroy(Str *s);
void LobbyString_Append(Str *s, void *arg);
void LobbyString_AppendCStr(Str *s, const char *arg);
void LobbyString_AssignCStr(Str *s, const char *arg);
int ov00_021EFF30(const void *src, u32 len, char *dst, u32 dstLen);
void MI_CpuFill8(void *dst, u8 val, u32 size);

void ov45_0223D15C(DequeBuf *d, u32 n, Tag unused);
int ov45_0223D208(Iter *a, Iter *b);
void ov45_0223CF30(DequeBuf *d, Iter *a, Iter *b, Tag unused);

static inline void TouchTag(void) {
    volatile Tag t = { 0 };
}

typedef struct SplitBuf {
    u32 *ptr;
    u32 size;
    u32 cap;
    u32 *alloc;
    u32 start;
} SplitBuf;

void ov45_0223EC80(SplitBuf *b, u32 cap, Tag unused);
void ov45_0223EC64(SplitBuf *b);

int ov45_0223D7B4(int c);
BOOL ov45_0223D7D4(int type, int mode, u32 value, const void *data, u32 len, Str *out);
void ov45_0223DA3C(CharVec *s, const char *c);
int LobbyString_CompareSubstring(const u8 *s, u32 a, u32 b, const u8 *o);

int ov45_0223D7B4(int c) {
    if (c < 0 || c >= 0x80) {
        return c;
    }
    return __lower_mapC[c];
}

BOOL ov45_0223D7D4(int type, int mode, u32 value, const void *data, u32 len, Str *out) {
    char buf[0x191];
    Str s0;
    Str s1;
    Str s2;
    Str s3;
    if (type == 0 && len > 400) {
        return FALSE;
    }
    if (type == 1 && len > 300) {
        return FALSE;
    }
    LobbyString_AssignCStr(out, ov45_022551EC);
    ov45_02232B24(&s0, 3, ov45_022551F0, 0);
    LobbyString_Append(out, &s0);
    LobbyString_Destroy(&s0);
    LobbyString_AppendCStr(out, ov45_022551E0);
    ov45_02232B24(&s1, 2, ov45_022551F0, 6);
    LobbyString_Append(out, &s1);
    LobbyString_Destroy(&s1);
    LobbyString_AppendCStr(out, ov45_022551E0);
    switch (type) {
    case 0:
        LobbyString_AppendCStr(out, ov45_022551E8);
        break;
    case 1:
        LobbyString_AppendCStr(out, ov45_022551E4);
        break;
    default:
        return FALSE;
    }
    LobbyString_AppendCStr(out, ov45_022551E0);
    switch (mode) {
    case 1:
        LobbyString_AppendCStr(out, ov45_022551F4);
        break;
    case 0:
        LobbyString_AppendCStr(out, ov45_022551E8);
        break;
    default:
        return FALSE;
    }
    LobbyString_AppendCStr(out, ov45_022551E0);
    ov45_02232B24(&s2, 8, ov45_022551F0, value);
    LobbyString_Append(out, &s2);
    LobbyString_Destroy(&s2);
    LobbyString_AppendCStr(out, ov45_022551E0);
    LobbyString_AppendCStr(out, ov45_022551F8);
    LobbyString_AppendCStr(out, ov45_022551DC);
    switch (type) {
    case 0:
        ov45_02232B24(&s3, len, ov45_022551FC, data);
        LobbyString_Append(out, &s3);
        LobbyString_Destroy(&s3);
        break;
    case 1:
        MI_CpuFill8(buf, 0, sizeof(buf));
        if (ov00_021EFF30(data, len, buf, sizeof(buf)) == -1) {
            return FALSE;
        }
        LobbyString_AppendCStr(out, buf);
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

void ov45_0223DA3C(CharVec *s, const char *c) {
    TouchTag();
    ov45_02237890(s, 1);
    ov45_0223DB8C(s, 1, c);
}

void LobbyString_Append(Str *s, void *arg) {
    LobbyString_Replace(s, StrSize(s), 0, arg);
}

void LobbyString_AppendCStr(Str *s, const char *arg) {
    LobbyString_ReplaceCStr(s, StrSize(s), 0, arg);
}

void LobbyString_AssignCStr(Str *s, const char *arg) {
    LobbyString_ReplaceCStr(s, 0, StrSize(s), arg);
}

void LobbyString_ReplaceCStr(Str *s, u32 a, u32 b, const char *str) {
    Tag x;
    LobbyString_ReplaceRange(s, a, b, str, str + strlen(str), x);
}

int LobbyString_CompareSubstring(const u8 *s, u32 a, u32 b, const u8 *input) {
    const Str *o = (const Str *)input;
    u32 n;
    const char *p;
    if (StrIsShort(o)) {
        p = o->s.buf;
        n = o->s.len;
    } else {
        p = o->l.ptr;
        n = o->l.size;
    }
    return LobbyString_CompareChars(s, a, b, p, n);
}
