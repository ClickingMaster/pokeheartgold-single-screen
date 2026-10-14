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
void LobbyEventDequeMap_LengthError(void);
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

void LobbyCharVectorBuffer_Allocate(CharSplitBuf *b, u32 cap, Tag unused);
void LobbyCharVectorBuffer_Destroy(CharSplitBuf *b);

u32 LobbyCharVector_RecommendSize(CharVec *, u32);
void LobbyCharVector_AppendFillSlow(CharVec *, u32, const char *);
u32 LobbyU32Vector_RecommendSize(Vec *, u32);
void LobbyU32Vector_AppendFillSlow(Vec *, u32, u32 *);
void ov45_02236E58(NodeIter *);
Node *LobbyTree_EraseRebalance(Node *, void *);

extern char sStr_TreeInsertLengthError_2[];
void printf(const char *, ...);
void abort(void);
void LobbyTree_InsertRebalance(Node *, void *);

typedef struct InsertResult {
    Node *current;
    u8 inserted;
} InsertResult;

extern u8 ov45_02255200[];
Node *PPWLobbyPlayerSet_FindInsertPos(List *l, Session **key, Node **parent, u8 *left, u8 *flag);
Node *PPWLobbyPlayerSet_InsertNode(List *l, Node *parent, u8 left, u8 flag, Session **key);

typedef struct SessionMgr {
    u32 unk0;
    List list;
    Session self;
} SessionMgr;

void PPWLobbyPlayerSet_Erase(NodeIter *out, List *l, NodeIter it);

typedef struct FindState {
    int id;
    Node *end;
    NodeIter it;
} FindState;

void LobbyU32Vector_Reserve(Vec *v, u32 n);
void LobbyU32Vector_PushBackSlow(Vec *v, u32 *x, Tag unused);

static inline void VecReserve(Vec *v, u32 n) {
    if (n > v->cap) {
        LobbyU32Vector_Reserve(v, n);
    }
}

void PPWLobbyPlayers_GetEnteredIds(Vec *out, SessionMgr *m);
void LobbyU32Vector_Destroy(Vec *v);

void LobbyU32Vector_Allocate(Vec *v, u32 n, Tag unused);
Vec *LobbyU32Vector_Destroy2(Vec *v);

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
const char *LobbyByteVector_Data(void *);
void PPWLobbyPlayer_UpdateState(Session *s);

extern VTable gPPWLobbyPlayerVtable;
void DWC_GetDateTime(RTCDate *date, RTCTime *time);
void PPWLobbyPlayerSet_Insert(InsertResult *out, List *l, Session **key);

typedef struct PtrDeque {
    u32 unk0;
    u32 count;
    DequeBuf buf;
} PtrDeque;

void ov45_02238508(DequeBuf *);

extern char sStr_SpaceSpace[];
extern char sStr_Space_2[];
extern char sStr_B[];
extern char sStr_S[];
extern char sStr_Empty_16[];
extern char sFmt_Hex_3[];
extern char sStr_A[];
extern char sStr__[];
extern char sFmt_Str_3[];
void LobbyString_FormatN(Str *out, u32 width, const char *fmt, ...);
void LobbyString_Destroy(Str *s);
void LobbyString_Append(Str *s, void *arg);
void LobbyString_AppendCStr(Str *s, const char *arg);
void LobbyString_AssignCStr(Str *s, const char *arg);
int DWC_Base64Encode(const void *src, u32 len, char *dst, u32 dstLen);
void MI_CpuFill8(void *dst, u8 val, u32 size);

void LobbyEventDequeMap_Allocate(DequeBuf *d, u32 n, Tag unused);
int LobbyEventDequeMapIterator_Distance2(Iter *a, Iter *b);
void LobbyEventDequeMap_AppendRange(DequeBuf *d, Iter *a, Iter *b, Tag unused);

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

void LobbyU32VectorBuffer_Allocate(SplitBuf *b, u32 cap, Tag unused);
void LobbyU32VectorBuffer_Destroy(SplitBuf *b);

BOOL PPWLobbyPlayers_IsFull(SessionMgr *m);

BOOL PPWLobbyPlayers_IsFull(SessionMgr *m) {
    Vec v;
    BOOL made = FALSE;
    BOOL res = FALSE;
    if (m->list.count + 1 == 20) {
        PPWLobbyPlayers_GetEnteredIds(&v, m);
        made = TRUE;
        if (v.size + 1 == 20) {
            res = TRUE;
        }
    }
    if (made) {
        LobbyU32Vector_Destroy(&v);
    }
    return res;
}

void PPWLobbyPlayers_GetEnteredIds(Vec *out, SessionMgr *m) {
    NodeIter it;
    u32 id;
    Node *hdr;
    out->ptr = NULL;
    out->size = 0;
    out->cap = 0;
    VecReserve(out, m->list.count);
    hdr = (Node *)&m->list.root;
    it.current = m->list.leftmost;
    while (it.current != hdr) {
        Session *s = ((Node *)it.current)->val;
        if (s->unk38 != 0) {
            id = s->id;
            TouchTag();
            if (out->size < out->cap) {
                out->size++;
                ((u32 *)out->ptr)[out->size - 1] = id;
            } else {
                Tag t2 = { 0 };
                LobbyU32Vector_PushBackSlow(out, &id, t2);
            }
        }
        ov45_02236E58(&it);
    }
}
