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
extern void LobbyRequestMap_DeleteAll(struct Tree *p);

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
Node *LobbyRequestMap_FindInsertPos(Tree *t, Obj **key, Node **pos, u8 *a, u8 *b);
void LobbyUserIdSet_Find(Iter *out, Tree *t, int *key);

static inline BOOL StrLess(Str *a, Str *b) {
    return LobbyString_Compare(a, b) < 0;
}

typedef struct ObjKey {
    void *vt;
    u32 id;
    u32 param;
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

extern Node *LobbyTree_EraseRebalance(Node *n, void *root);
extern void LobbyRequestMap_Find(Iter *out, Tree *t, void *key);
extern void *gLobbyRequestVtable;
extern void LobbyUserIdSet_Assign(TreeHdr *dst, TreeHdr *src);
void LobbyRequestMap_Erase(Iter *out, Tree *t, Iter pos);

typedef struct Tag {
    u8 x;
} Tag;

IntStrPair *LobbyUserMap_GetOrInsert(Tree *t, int *key);

static inline IntStrPair *MapIndex(Tree *t, int *key, Tag a, Tag b) {
    return LobbyUserMap_GetOrInsert(t, key);
}

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
    int disconnected;
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
extern void chatDisconnect(void *p);
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
    int connection;
    int state;
    int userData;
    Str sC;
    Str s18;
    Str s24;
    Str s30;
    Str s3C;
    Str s48;
    int unk54;
    int lastError;
    int connectedFrames;
    int disconnected;
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
extern VoidFunc gLobbyMemOutOfMemoryHandler;

extern struct {
    u8 *obj;
    void *buf;
} ov45_022577E8;

void LobbyMem_OnOutOfMemoryDisconnect(void);
void LobbyMem_OnOutOfMemory(void);
int LobbyString_FindFirstOfChars(Str *s, const char *chars, u32 pos, u32 n);

BOOL LobbyMem_InstallOutOfMemoryHandler(u8 *obj);
BOOL LobbyRequestMap_DeleteKey(Tree *t, u32 id);

int LobbyString_FindFirstOfChars(Str *s, const char *chars, u32 pos, u32 n) {
    const char *data;
    u32 len;
    const char *end;
    const char *it;
    if (s->l.isLong) {
        data = s->l.ptr;
        len = s->l.size;
    } else {
        len = s->s.len;
        data = s->s.buf;
    }
    if (pos < len && n != 0) {
        end = data + len;
        it = data + pos;
        if (it < end) {
            do {
                const char *c = chars;
                u32 i;
                for (i = 0; i < n; i++, c++) {
                    if (*it == *c) {
                        return it - data;
                    }
                }
                it++;
            } while (it < end);
        }
    }
    return -1;
}

Node *LobbyRequestMap_FindInsertPos(Tree *t, Obj **key, Node **pos, u8 *a, u8 *b) {
    Node *parent;
    Node *n;
    *pos = NULL;
    n = t->head.left;
    *a = 1;
    *b = 1;
    parent = &t->head;
    while (n != NULL) {
        parent = n;
        if ((*key)->id < n->key.obj->id) {
            n = n->left;
            *a = 1;
        } else {
            *pos = n;
            n = n->right;
            *a = 0;
            *b = 0;
        }
    }
    return parent;
}

void *LobbyMem_Alloc(u32 size) {
    void *p;
    if (size == 0) {
        size = 1;
    }
    for (;;) {
        VoidFunc f;
        p = DWC_Alloc(0, size);
        if (p != NULL) {
            return p;
        }
        f = gLobbyMemOutOfMemoryHandler;
        gLobbyMemOutOfMemoryHandler = f;
        if (f == NULL) {
            break;
        }
        f();
    }
    return NULL;
}

void LobbyMem_OnOutOfMemoryDisconnect(void) {
    chatDisconnect(*(void **)ov45_022577E8.obj);
    gLobbyMemOutOfMemoryHandler = NULL;
}

void LobbyMem_OnOutOfMemory(void) {
    u8 *obj;
    if (ov45_022577E8.buf != NULL) {
        DWC_Free(0, ov45_022577E8.buf, 0);
        ov45_022577E8.buf = NULL;
    }
    obj = ov45_022577E8.obj;
    *(int *)(obj + 0x58) = 2;
    *(int *)(obj + 4) = 5;
    gLobbyMemOutOfMemoryHandler = LobbyMem_OnOutOfMemoryDisconnect;
}

BOOL LobbyMem_InstallOutOfMemoryHandler(u8 *obj) {
    void *buf;
    ov45_022577E8.obj = obj;
    if (ov45_022577E8.buf != NULL) {
        DWC_Free(0, ov45_022577E8.buf, 0);
        ov45_022577E8.buf = NULL;
    }
    buf = DWC_Alloc(0, 0x400);
    ov45_022577E8.buf = buf;
    if (buf == NULL) {
        return FALSE;
    }
    gLobbyMemOutOfMemoryHandler = LobbyMem_OnOutOfMemory;
    return TRUE;
}

void LobbyMem_ReleaseReserve(void) {
    if (ov45_022577E8.buf != NULL) {
        DWC_Free(0, ov45_022577E8.buf, 0);
        ov45_022577E8.buf = NULL;
    }
}

BOOL LobbyRequestMap_DeleteKey(Tree *t, u32 id) {
    ObjKey key;
    Iter tmp;
    ObjKey *pk;
    Iter it;
    Iter res;
    key.vt = &gLobbyRequestVtable;
    key.id = id;
    key.param = 0;
    pk = &key;
    LobbyRequestMap_Find(&it, t, &pk);
    tmp = it;
    if (it.current == &t->head) {
        return FALSE;
    }
    if (((Node *)it.current)->key.obj != NULL) {
        ((VObj *)((Node *)it.current)->key.obj)->vt->dtor((VObj *)((Node *)it.current)->key.obj);
    }
    LobbyRequestMap_Erase(&res, t, tmp);
    return TRUE;
}

void LobbyRequestMap_DeleteAll(Tree *t) {
    Iter it;
    Iter tmp;
    Iter res;
    it.current = t->head.right;
    while (it.current != &t->head) {
        VObj *o = (VObj *)((Node *)it.current)->key.obj;
        if (o != NULL) {
            o->vt->dtor(o);
        }
        tmp.current = it.current;
        ov45_02236E58(&it);
        LobbyRequestMap_Erase(&res, t, tmp);
    }
}

void LobbyRequestMap_Erase(Iter *out, Tree *t, Iter pos) {
    Node *n = pos.current;
    if (n == t->head.right) {
        ov45_02236E58(&pos);
        t->head.right = pos.current;
    }
    out->current = LobbyTree_EraseRebalance(n, t->head.left);
    if (n != NULL) {
        DWC_Free(0, n, 0);
    }
    t->count--;
}
