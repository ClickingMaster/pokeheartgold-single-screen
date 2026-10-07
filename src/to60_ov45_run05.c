#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on

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

void ov00_021EC2EC(int, void *, int);
void *ov45_02242394(u32);
void ov45_0223BD0C(void);
void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
Str *ov45_02237DB4(Str *, u32, u32, void *);
void ov45_0223DB08(Str *, u32, u32, const char *);
typedef struct Tag {
    u8 c;
} Tag;
void ov45_02238104(Str *, u32, u32, const char *, const char *, Tag);
int ov45_02238024(const u8 *, u32, u32, const void *, u32);

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
Node *ov45_022370D0(Node *,void *);

extern char ov45_0225520C[];
void printf(const char *, ...);
void abort(void);
void ov45_02236EC4(Node *,void *);

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
        fn = *(BOOL (**)(void *))(*(u8 **)obj + p->fn);
    } else {
        fn = (BOOL (*)(void *))p->fn;
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

Callbacks *ov45_022310B0(void);
const char *ov45_02231C78(void *);
void ov45_0223DCD0(Session *s);

extern VTable ov45_02254FB0;
void ov00_021ECB94(RTCDate *date, RTCTime *time);
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
void ov45_02237BC8(Str *s);
void ov45_0223DA6C(Str *s, void *arg);
void ov45_0223DAA0(Str *s, const char *arg);
void ov45_0223DAD4(Str *s, const char *arg);
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



BOOL ov45_0223DD3C(SessionMgr *m, int id);
BOOL ov45_0223DE38(SessionMgr *m, int id);
Session *ov45_0223DEC4(SessionMgr *m, int id);

// @fn ov45_0223DCD0
void ov45_0223DCD0(Session *s) {
    switch (s->state) {
    case 0:
        s->state = (s->unk30 != 0 && s->unk34 != 0) ? 1 : 0;
        if (s->state == 0) {
            return;
        }
    case 1:
        if (s->unk10 != 0) {
            s->state = 2;
        }
        return;
    case 2:
        if (s->unk38 != 0) {
            s->state = 3;
        }
        return;
    case 3:
        return;
    }
}

// @fn ov45_0223DD3C
BOOL ov45_0223DD3C(SessionMgr *m, int id) {
    RTCDate date;
    RTCTime time;
    Session *s;
    volatile InsertResult res2;
    InsertResult res;
    volatile Tag t1;
    volatile Tag t0;
    Session *n = ov45_02242394(sizeof(Session));
    if (n != NULL) {
        n->vt = &ov45_02254FB0;
        n->unk4 = 5;
        n->time = 0;
        n->unk10 = 0;
        ov00_021ECB94(&date, &time);
        n->time = RTC_ConvertDateTimeToSecond(&date, &time);
        n->unk14 = 1;
        n->unk18 = 0;
        n->unk1C = 0;
        n->unk20 = 0;
        n->unk24 = 1;
        n->state = 0;
        n->id = id;
        n->unk30 = 0;
        n->unk34 = 0;
        n->unk38 = 0;
    }
    s = n;
    ov45_0223E648(&res, &m->list, &s);
    t1.c = 0;
    res2.current = res.current;
    t0.c = 0;
    res2.inserted = res.inserted;
    if ((res.inserted & 0xFF) == 0) {
        if (s != NULL) {
            s->vt->dtor((VObj *)s);
        }
        return TRUE;
    }
    return TRUE;
}

// @fn ov45_0223DE38
BOOL ov45_0223DE38(SessionMgr *m, int id) {
    NodeIter it;
    Node *hdr = (Node *)&m->list.root;
    it.current = m->list.leftmost;
    while (it.current != hdr) {
        Session *s = ((Node*)it.current)->val;
        if (id == s->id) {
            NodeIter tmp;
            if (s != NULL) {
                s->vt->dtor((VObj *)s);
            }
            ov45_0223E6E8(&tmp, &m->list, it);
            return TRUE;
        }
        ov45_02236E58(&it);
    }
    return FALSE;
}

// @fn ov45_0223DEC4
Session *ov45_0223DEC4(SessionMgr *m, int id) {
    volatile FindState a;
    volatile FindState b;
    Node *r;
    BOOL e;
    Node *hdr;
    Node *first;
    if (id == m->self.id) {
        return &m->self;
    }
    first = m->list.leftmost;
    hdr = (Node *)&m->list.root;
    a.id = id;
    b.id = id;
    a.end = hdr;
    b.end = hdr;
    a.it.current = first;
    b.it.current = first;
    while (((NodeIter *)&b.it)->current != hdr && id != ((Node*)((NodeIter *)&b.it)->current)->val->id) {
        ov45_02236E58((NodeIter *)&b.it);
    }
    e = ((NodeIter *)&b.it)->current == (Node *)&m->list.root;
    r = b.it.current;
    if (e) {
        return NULL;
    }
    return r->val;
}
