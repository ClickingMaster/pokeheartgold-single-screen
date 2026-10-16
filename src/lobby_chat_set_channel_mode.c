#pragma force_active on
#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

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

typedef struct IntStrPair {
    int key;
    Str val;
} IntStrPair;

extern void LobbyString_InitCopy(void *dst, void *src);

static inline void StrInit(Str *s) {
    u32 i;
    u32 *p;
    for (i = 0, p = (u32 *)s; i < 3; i++) {
        p[i] = 0;
    }
}

static inline void PairInit(IntStrPair *p, int *k, Str *v) {
    p->key = *k;
    LobbyString_InitCopy(&p->val, v);
}

static inline BOOL StrIsShort(Str *s) {
    return !s->l.isLong;
}

static inline u32 StrSize(Str *s) {
    return StrIsShort(s) ? s->s.len : s->l.size;
}

static inline const char *StrData(Str *s) {
    return StrIsShort(s) ? s->s.buf : s->l.ptr;
}

extern void LobbyString_InitCopy(void *dst, void *src);
struct Tree;
extern void LobbyString_Destroy(void *p);

typedef struct Obj {
    void *vt;
    u32 id;
} Obj;

typedef struct Node {
    void *left;
    void *right;
    u32 parent;

    union {
        int i;
        Obj *obj;
        Str str;
    } key;
} Node;

typedef struct Tree {
    int count;
    Node head;
} Tree;

#include "../lib/NitroDWC/src/tree-iterator4.h"
typedef Iterator Iter;

typedef struct InsertResult {
    Node *node;
    u8 inserted;
} InsertResult;

extern int LobbyString_Compare(Str *a, Str *b);
Node *LobbyRequestMap_InsertNode(Tree *t, Node *parent, u8 a, u8 b, Obj **val);
Node *LobbyChannelMap_LowerBound(Tree *t, Str *key, Node *n, Node *res);
Node *LobbyRequestMap_FindInsertPos(Tree *t, Obj **key, Node **pos, u8 *a, u8 *b);
void LobbyUserIdSet_Find(Iter *out, Tree *t, int *key);
void LobbyUserMap_Find(Iter *out, Tree *t, int *key);
void LobbyChannelMap_Find(Iter *out, Tree *t, Str *key);

static inline BOOL StrLess(Str *a, Str *b) {
    return LobbyString_Compare(a, b) < 0;
}

typedef struct ObjKey {
    void *vt;
    u32 id;
    u32 unk8;
} ObjKey;

static inline Iter IterPostInc(Iter *it) {
    Iter tmp = *it;
    ov45_02236E58(it);
    return tmp;
}

typedef struct VObj VObj;

typedef struct VTable {
    void (*f0)(VObj *);
    void (*dtor)(VObj *);
    void *(*f8)(VObj *);
} VTable;

struct VObj {
    VTable *vt;
};

typedef struct Vec {
    void *ptr;
    u32 size;
    u32 cap;
} Vec;

typedef struct Blob24 {
    u32 w[6];
} Blob24;

typedef struct TreeHdr {
    int count;
    void *root;
    void *leftmost;
} TreeHdr;

typedef struct Entry {
    u32 a;
    TreeHdr t;
    u32 x10;
    Blob24 b;
} Entry;

typedef struct NamedEntry {
    Str name;
    Entry e;
} NamedEntry;

typedef struct Empty2 {
    u8 a;
    u8 b;
} Empty2;

extern void *gLobbyRequestVtable;
extern void LobbyString_InitFromCStr(Str *s, const char *cstr);
extern void LobbyUserIdSet_Assign(TreeHdr *dst, TreeHdr *src);
int LobbyChat_GetUserIdFromNick(Str *s);
void LobbyRequestMap_Erase(Iter *out, Tree *t, Iter pos);

typedef struct Tag {
    u8 x;
} Tag;

IntStrPair *LobbyUserMap_GetOrInsert(Tree *t, int *key);

static inline IntStrPair *MapIndex(Tree *t, int *key, Tag a, Tag b) {
    return LobbyUserMap_GetOrInsert(t, key);
}

Node *LobbyUserMap_FindInsertPos2(Tree *t, int *key, Node **pos, u8 *a, u8 *b);
BOOL LobbyChat_RemoveUserIfUnused(u8 *obj, int id);
BOOL LobbyUserMap_EraseKey(Tree *t, int *key);

int LobbyChat_GetFullChannelName(void *a, void *b0, void *c);

typedef struct Pred {
    int id;
} Pred;

static inline Pred MakePred(int id) {
    Pred p;
    p.id = id;
    return p;
}

static inline BOOL PredCallV(Pred p, Node *n) {
    Iter r;
    LobbyUserIdSet_Find(&r, (Tree *)((u8 *)n + 0x1c), &p.id);
    return r.current != (Node *)((u8 *)n + 0x20);
}

static inline BOOL PredCall(Pred *p, Node *n) {
    Iter r;
    LobbyUserIdSet_Find(&r, (Tree *)((u8 *)n + 0x1c), &p->id);
    return r.current != (Node *)((u8 *)n + 0x20);
}

static inline Iter FindIf(Iter first, Node *last, Pred p) {
    while (first.current != last && !PredCall(&p, first.current)) {
        ov45_02236E58(&first);
    }
    return first;
}

static inline void FindIfP(Iter *first, Node *last, Pred p) {
    while (first->current != last && !PredCall(&p, first->current)) {
        ov45_02236E58(first);
    }
}

typedef struct Ctx {
    void *connection;
    int state;
    u8 unk8[0x58];
    int unk60;
    u8 unk64[0x2c];
    u8 map[0xc];
} Ctx;

typedef struct CbObj {
    VTable *vt;
    u32 id;
    void *x8;
    int xc;
} CbObj;

extern int gLobbyChatNextRequestId;
extern void *LobbyMem_Alloc(u32 size);

static inline void CbObjBaseInit(CbObj *o, void *x8) {
    o->vt = (VTable *)&gLobbyRequestVtable;
    o->id = 0;
    o->x8 = x8;
}

static inline void CbObjSetVt(CbObj *o) {
    o->vt = (VTable *)&gLobbyRequestVtable;
}

static inline void CbObjBaseInit2(CbObj *o, void *x8) {
    o->id = 0;
    o->x8 = x8;
}

static inline void CbObjDerivedInit(CbObj *o, void *vt, int xc, void *x8, int id) {
    CbObjBaseInit(o, x8);
    o->vt = vt;
    o->id = id;
    o->xc = xc;
}

static inline CbObj *CbObjCreate(void *vt, int xc, void *x8) {
    CbObj *o = LobbyMem_Alloc(sizeof(CbObj));
    if (o != NULL) {
        CbObjBaseInit(o, x8);
        o->vt = vt;
        o->id = gLobbyChatNextRequestId++;
        o->xc = xc;
    }
    return o;
}

extern int LobbyString_CompareCStr(Str *a, const char *b);
void *LobbyChatManager_Destroy(u8 *p);
void LobbyMem_ReleaseReserve(void);

#define CTX_CHECK(c)                      \
    if ((c) == NULL) {                    \
        return 4;                         \
    }                                     \
    if ((c) != NULL && (c)->state == 5) { \
        return 7;                         \
    }                                     \
    if ((c) != NULL && CtxState() != 3) { \
        return 4;                         \
    }

static inline void OpDelete(void *p) {
    if (p != NULL) {
        DWC_Free(0, p, 0);
    }
}

void LobbyChannelMap_MakeEntry(NamedEntry *dst, Str *name, Entry *src);

static inline void TreeHdrCopy(TreeHdr *d, TreeHdr *s) {
    Tag t;
    d->count = 0;
    d->root = NULL;
    d->leftmost = (Node *)&d->root;
    t = (Tag) { 0 };
    LobbyUserIdSet_Assign(d, s);
}

static inline void TagTemp(void) {
    Tag t;
    t = (Tag) { 0 };
}

static inline void TreeHdrInit(TreeHdr *t) {
    t->count = 0;
    t->root = NULL;
    t->leftmost = (Node *)&t->root;
}

static inline void EntryCopy(Entry *d, Entry *s) {
    d->a = s->a;
    TreeHdrCopy(&d->t, &s->t);
    d->x10 = s->x10;
    d->b = s->b;
}

static inline void NamedEntryCopy(NamedEntry *d, NamedEntry *s) {
    LobbyString_InitCopy(&d->name, &s->name);
    EntryCopy(&d->e, &s->e);
}

typedef struct Tree16 {
    int count;
    void *root;
    Node *unk8;
    void *leftmost;
} Tree16;

typedef struct Blob12 {
    u32 w[3];
} Blob12;

typedef struct NameTable {
    const char *names[12];
} NameTable;

typedef struct CtxFull {
    int unk0;
    int unk4;
    int unk8;
    Str sC;
    Str s18;
    Str s24;
    Str s30;
    Str s3C;
    Str s48;
    int unk54;
    int unk58;
    int unk5C;
    int unk60;
    Blob12 b64;
    Tree16 t70;
    Tree16 t80;
    TreeHdr t90;
    Tree16 t9C;
} CtxFull;

typedef struct NodeHolder {
    Node **hdr;
    Node *p;
} NodeHolder;

static inline void IterTemp(Node *n) {
    Iter it;
    it = (Iter) { n };
}

static inline BOOL StrEqC(Str *a, const char *b) {
    return LobbyString_CompareCStr(a, b) == 0;
}

extern Ctx *gLobbyChatManager;
extern void chatSetChannelModeA(void *h, const char *name, int arg);

static inline int CtxState(void) {
    return (*(Ctx *volatile *)&gLobbyChatManager)->state;
}

static inline BOOL CtxInited(void) {
    return gLobbyChatManager != NULL;
}

static inline BOOL CtxIsState5(void) {
    return gLobbyChatManager != NULL && gLobbyChatManager->state == 5;
}

static inline BOOL CtxIsState3(void) {
    return gLobbyChatManager != NULL && gLobbyChatManager->state == 3;
}

typedef void (*VoidFunc)(void);

extern struct {
    u8 *obj;
    void *buf;
} ov45_022577E8;

void LobbyMem_OnOutOfMemoryDisconnect(void);
void LobbyMem_OnOutOfMemory(void);
int LobbyString_FindFirstOfChars(Str *s, const char *chars, u32 pos, u32 n);
Node *LobbyUserIdSet_LowerBound(Tree *t, int *key, Node *node, Node *res);

Node *LobbyUserMap_LowerBound(Tree *t, int *key, Node *node, Node *res);

int LobbyChat_SetChannelMode(const char *name, int arg);

int LobbyChat_SetChannelMode(const char *name, int arg) {
    Str s1;
    Str s2;
    Ctx *c = gLobbyChatManager;
    if (c == NULL) {
        return 4;
    }
    if (c != NULL && c->state == 5) {
        return 7;
    }
    if (c != NULL && CtxState() != 3) {
        return 4;
    }
    LobbyString_InitFromCStr(&s1, name);
    LobbyChat_GetFullChannelName(&s2, gLobbyChatManager, &s1);
    chatSetChannelModeA(gLobbyChatManager->connection, StrData(&s2), arg);
    LobbyString_Destroy(&s2);
    LobbyString_Destroy(&s1);
    return 0;
}
