#pragma force_active on
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

static inline BOOL StrIsShort(Str *s) {
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

void ov45_0223E648(InsertResult *out, List *l, Session **key) {
    Node *parent;
    u8 left;
    u8 flag;
    Node *y = ov45_0223E940(l, key, &parent, &left, &flag);
    if (parent == NULL || parent->val->id < (*key)->id) {
        out->current = ov45_0223E758(l, y, left, flag, key);
        out->inserted = ov45_02255200[0];
        return;
    }
    out->current = parent;
    out->inserted = ov45_02255200[1];
}

void ov45_0223E6E8(NodeIter *out, List *l, NodeIter it) {
    Node *n = it.current;
    if (n == l->leftmost) {
        ov45_02236E58(&it);
        l->leftmost = it.current;
    }
    out->current = ov45_022370D0(n, l->root);
    if (n != NULL) {
        DWC_Free(0, n, 0);
    }
    l->count--;
}

Node *ov45_0223E758(List *l, Node *parent, u8 left, u8 flag, Session **key) {
    Node *n;
    Node *tmp;
    Session **pv;
    u32 *pp;

    volatile struct {
        Node *a;
        Node *b;
        void **c;
        Node *d;
    } h;

    if (l->count == (u32)-1) {
        printf(ov45_0225520C);
        abort();
    }
    {
        volatile Tag t = { 0 };
    }
    n = LobbyMem_Alloc(sizeof(Node));
    h.c = &l->root;
    pv = &n->val;
    h.d = n;
    h.a = n;
    if (pv != NULL) {
        *pv = *key;
    }
    n->right = NULL;
    h.d = NULL;
    n->left = NULL;
    pp = &n->parent;
    if (pp != NULL) {
        *pp = (u32)parent;
    }
    if (left) {
        parent->left = n;
    } else {
        parent->right = n;
    }
    l->count++;
    h.b = n;
    ov45_02236EC4(n, l->root);
    if (flag) {
        l->leftmost = n;
    }
    tmp = h.d;
    if (tmp != NULL) {
        DWC_Free(0, tmp, 0);
    }
    return n;
}

void ov45_0223E830(Vec *v, u32 n) {
    Vec b;
    u32 *first;
    int cnt;
    b.ptr = NULL;
    b.size = 0;
    b.cap = 0;
    {
        Tag ta = { 0 };
        ov45_0223EA04(&b, n, ta);
    }
    first = v->ptr;
    cnt = (first + v->size) - v->ptr;
    TouchTag();
    memcpy((u32 *)b.ptr + b.size, first, cnt * 4);
    memset(first, 0, cnt * 4);
    b.size += cnt;
    TouchTag();
    v->size = 0;
    {
        u32 *pcap = &b.cap;
        u32 t = v->cap;
        v->cap = *pcap;
        *pcap = t;
    }
    SwapPtr(&v->ptr, &b.ptr);
    SwapU32(&v->size, &b.size);
    ov45_0223E9B4(&b);
}

void ov45_0223E910(Vec *v, u32 *x, Tag unused) {
    TouchTag();
    ov45_0223EA34(v, 1);
    ov45_0223EB00(v, 1, x);
}

Node *ov45_0223E940(List *l, Session **key, Node **parent, u8 *left, u8 *flag) {
    Node *y;
    Node *x;
    *parent = NULL;
    x = l->root;
    *left = 1;
    *flag = 1;
    y = (Node *)&l->root;
    while (x != NULL) {
        y = x;
        if ((*key)->id < x->val->id) {
            x = x->left;
            *left = 1;
        } else {
            *parent = x;
            x = x->right;
            *left = 0;
            *flag = 0;
        }
    }
    return y;
}

Vec *ov45_0223E9B4(Vec *v) {
    if (v->ptr != NULL) {
        u32 n = *(volatile u32 *)&v->size;
        volatile Tag a = { 0 };
        *(volatile u32 *)&v->size -= n;
        if (v->ptr != NULL) {
            DWC_Free(0, v->ptr, 0);
        }
    }
    return v;
}

void ov45_0223EA04(Vec *v, u32 n, Tag unused) {
    if (n > 0x3FFFFFFF) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    v->ptr = LobbyMem_Alloc(n * 4);
    v->cap = n;
}

u32 ov45_0223EA34(Vec *v, u32 n) {
    u32 cap = v->cap;
    u32 g1;
    u32 g2;
    if (n > 0x3FFFFFFF - cap) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    if (cap < 0x15555555) {
        g1 = (cap + 1) * 3 / 5;
        return cap + *(n > g1 ? &n : &g1);
    }
    if (cap < 0x2AAAAAAA) {
        g2 = (cap + 1) / 2;
        return cap + *(n > g2 ? &n : &g2);
    }
    return 0x3FFFFFFF;
}

void ov45_0223EB00(Vec *v, u32 n, u32 *x) {
    volatile Tag t4;
    SplitBuf b;
    u32 *first;
    int cnt;
    b.ptr = NULL;
    b.size = 0;
    b.cap = 0;
    b.start = 0;
    b.alloc = &v->cap;
    {
        u32 rec = ov45_0223EA34(v, v->size + n - v->cap);
        Tag t3 = { 0 };
        t4.c = 0;
        ov45_0223EC80(&b, rec, t3);
    }
    b.start = v->size;
    TouchTag();
    {
        u32 *p = b.ptr + b.start + b.size;
        u32 i = n;
        while (i != 0) {
            *p++ = *x;
            i--;
        }
    }
    b.size += n;
    first = v->ptr;
    cnt = (first + v->size) - v->ptr;
    TouchTag();
    b.start -= cnt;
    memcpy(b.ptr + b.start, first, cnt * 4);
    memset(first, 0, cnt * 4);
    b.size += cnt;
    TouchTag();
    v->size = 0;
    {
        u32 *pcap = &b.cap;
        u32 t = v->cap;
        v->cap = *pcap;
        *pcap = t;
    }
    SwapPtr(&v->ptr, (void **)&b.ptr);
    SwapU32(&v->size, &b.size);
    ov45_0223EC64(&b);
}
