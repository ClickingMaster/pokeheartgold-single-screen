#pragma force_active on
#include "global.h"

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
extern void DWC_Free(int a, void *p, int b);
extern void *DWC_Alloc(int a, u32 size);
extern void LobbyString_Substr(void *a, void *b, u32 len, int d);
extern void LobbyTimerMap_Clear(void *p);
extern void LobbyTimerMap_Destroy(void *p);
struct Tree;
extern void ov45_022425A0(struct Tree *p);
extern void ov45_02244150(void *p);
extern void LobbyChannelMap_Destroy(void *p);
extern void LobbyUserMap_Destroy(void *p);
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

extern void ov45_02236E58(Iter *it);

typedef struct InsertResult {
    Node *node;
    u8 inserted;
} InsertResult;

extern int LobbyString_Compare(Str *a, Str *b);
extern void LobbyUserMap_Erase(Iter *out, Tree *t, Iter it);
extern u8 ov45_02255228[];
Node *ov45_0224210C(Tree *t, Node *parent, u8 a, u8 b, Obj **val);
Node *LobbyChannelMap_LowerBound(Tree *t, Str *key, Node *n, Node *res);
Node *ov45_02242320(Tree *t, Obj **key, Node **pos, u8 *a, u8 *b);
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

extern Node *LobbyTree_EraseRebalance(Node *n, void *root);
extern void ov45_0223FD8C(Iter *out, Tree *t, void *key);
extern void *ov45_02255238;
extern u32 current_time(void);
extern void LobbyString_FormatN(char *buf, u32 size, const char *fmt, ...);
extern char ov45_0225539C[];
extern char ov45_022553A4[];
extern void LobbyString_InitFromCStr(Str *s, const char *cstr);
extern void LobbyChat_SplitString(Vec *out, Str *s, Str *sep, int n);
extern void ov45_02237664(Vec *v);
extern long atol(const char *s);
extern void ov45_02243BC0(TreeHdr *dst, TreeHdr *src);
extern void LobbyUserIdSet_Insert(InsertResult *out, Tree *t, int *key);
extern void LobbyString_Assign(Str *dst, Str *src);
extern Node *ov45_0224404C(Tree *t, Node *parent, u8 a, u8 b, IntStrPair *val);
extern BOOL LobbyUserIdSet_EraseKey(Tree *t, int *id);
int LobbyChat_GetUserIdFromNick(Str *s);
void ov45_0224261C(Iter *out, Tree *t, Iter pos);

typedef struct Tag {
    u8 x;
} Tag;

IntStrPair *LobbyUserMap_GetOrInsert(Tree *t, int *key);

static inline IntStrPair *MapIndex(Tree *t, int *key, Tag a, Tag b) {
    return LobbyUserMap_GetOrInsert(t, key);
}

Node *ov45_02242E88(Tree *t, int *key, Node **pos, u8 *a, u8 *b);
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
    void *unk0;
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

extern int ov45_022577E0;
extern void chatDisconnect(void *p);
extern void *LobbyMem_Alloc(u32 size);
extern void ov45_0224206C(InsertResult *out, Tree *t, Obj **val);

static inline void CbObjBaseInit(CbObj *o, void *x8) {
    o->vt = (VTable *)&ov45_02255238;
    o->id = 0;
    o->x8 = x8;
}

static inline void CbObjSetVt(CbObj *o) {
    o->vt = (VTable *)&ov45_02255238;
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
        o->id = ov45_022577E0++;
        o->xc = xc;
    }
    return o;
}

extern int LobbyString_CompareCStr(Str *a, const char *b);
extern void chatLeaveChannelA(void *h, const char *name, int x);
extern void LobbyChat_RemoveChannel(Ctx *c, Str *s);
extern void chatSendChannelMessageA(void *h, const char *name, const char *msg, int x);
extern void LobbyChat_GetUserNick(Str *out, Ctx *c, int a);
extern char ov45_02255304[];
extern void chatSendUserMessageA(void *h, const char *name, const char *msg, int x);
extern void chatEnumChannelsA(void *h, const char *name, void *f1, void *f2, int h2, int z);
extern void LobbyChat_OnEnumChannelsEach(void);
extern void LobbyChat_OnEnumChannels(void);
extern void LobbyChat_OnEnumUsers(void);
extern void LobbyChat_OnGetChannelMode(void);
extern int chatInChannelA(void *h, const char *name);
extern void chatEnumUsersA(void *h, const char *name, void *f, int h2, int z);
extern void chatGetChannelModeA(void *h, const char *name, void *f, int h2, int z);
extern void *ov45_022552B0;
extern void *ov45_02255298;
extern void *ov45_02255250;
void *LobbyChatManager_Destroy(u8 *p);
void ov45_022424DC(void);

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

extern void LobbyString_AssignCStr(Str *s, const char *cstr);
extern void chatSetChannelKeysA(void *h, const char *name, const char *who, int x, const char **key, const char **val);

extern void LobbyChannelMap_Insert(InsertResult *out, Tree *t, NamedEntry *e);
extern void LobbyUserIdSet_Destroy(TreeHdr *t);
void ov45_02242BE0(NamedEntry *dst, Str *name, Entry *src);

static inline void TreeHdrCopy(TreeHdr *d, TreeHdr *s) {
    Tag t;
    d->count = 0;
    d->root = NULL;
    d->leftmost = (Node *)&d->root;
    t = (Tag) { 0 };
    ov45_02243BC0(d, s);
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

extern void ov45_0223EA04(Vec *v, u32 n, Tag t);

extern void ov45_0223E910(Vec *v, u32 *val, Tag t);

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

extern NameTable ov45_02254DC4;
extern char ov45_02254DBC[];
extern char ov45_02255398[];
extern Str *LobbyString_ConcatCStr(Str *out, Str *a, char *b);
extern Str *LobbyString_Concat(Str *out, Str *a, Str *b);

extern char ov45_0225533C[];
extern int printf(const char *fmt, ...);
extern void abort(void);
extern void LobbyTree_InsertRebalance(Node *n, void *root);

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

extern Ctx *ov45_022577DC;
extern void chatSetChannelModeA(void *h, const char *name, int arg);

static inline int CtxState(void) {
    return (*(Ctx *volatile *)&ov45_022577DC)->state;
}

static inline BOOL CtxInited(void) {
    return ov45_022577DC != NULL;
}

static inline BOOL CtxIsState5(void) {
    return ov45_022577DC != NULL && ov45_022577DC->state == 5;
}

static inline BOOL CtxIsState3(void) {
    return ov45_022577DC != NULL && ov45_022577DC->state == 3;
}

typedef void (*VoidFunc)(void);
extern VoidFunc ov45_022577E4;

extern struct {
    u8 *obj;
    void *buf;
} ov45_022577E8;

void ov45_022423E0(void);
void ov45_0224240C(void);
int ov45_02242288(Str *s, const char *chars, u32 pos, u32 n);
Node *LobbyUserIdSet_LowerBound(Tree *t, int *key, Node *node, Node *res);

Node *LobbyUserMap_LowerBound(Tree *t, int *key, Node *node, Node *res);

BOOL LobbyChat_RemoveChannelUser(u8 *obj, Str *name, Str *key2);

void LobbyUserIdSet_Find(Iter *out, Tree *t, int *key) {
    Node *n = LobbyUserIdSet_LowerBound(t, key, t->head.left, &t->head);
    if (n == &t->head || *key < n->key.i) {
        out->current = &t->head;
        return;
    }
    out->current = n;
}

Node *LobbyUserIdSet_LowerBound(Tree *t, int *key, Node *node, Node *res) {
    while (node != NULL) {
        if (node->key.i >= *key) {
            res = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return res;
}

BOOL LobbyUserMap_EraseKey(Tree *t, int *key) {
    Iter it;
    Iter res;
    Iter tmp;
    LobbyUserMap_Find(&it, t, key);
    if (it.current == &t->head) {
        return FALSE;
    }
    tmp.current = it.current;
    LobbyUserMap_Erase(&res, t, tmp);
    return TRUE;
}

void LobbyUserMap_Find(Iter *out, Tree *t, int *key) {
    Node *n = LobbyUserMap_LowerBound(t, key, t->head.left, &t->head);
    if (n == &t->head || *key < n->key.i) {
        out->current = &t->head;
        return;
    }
    out->current = n;
}

Node *LobbyUserMap_LowerBound(Tree *t, int *key, Node *node, Node *res) {
    while (node != NULL) {
        if (node->key.i >= *key) {
            res = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return res;
}

BOOL LobbyChat_RemoveChannelUser(u8 *obj, Str *name, Str *key2) {
    int id;
    Iter it;
    id = LobbyChat_GetUserIdFromNick(name);
    if (id == -1) {
        return FALSE;
    }
    LobbyChannelMap_Find(&it, (Tree *)(obj + 0x80), key2);
    if (it.current == (Node *)(obj + 0x84)) {
        return FALSE;
    }
    if (!LobbyUserIdSet_EraseKey((Tree *)((u8 *)it.current + 0x1c), &id)) {
        return FALSE;
    }
    LobbyChat_RemoveUserIfUnused(obj, id);
    return TRUE;
}
