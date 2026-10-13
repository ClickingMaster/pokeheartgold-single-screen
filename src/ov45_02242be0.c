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

extern void ov45_02237A24(void *dst, void *src);

static inline void StrInit(Str *s) {
    u32 i;
    u32 *p;
    for (i = 0, p = (u32 *)s; i < 3; i++) {
        p[i] = 0;
    }
}

static inline void PairInit(IntStrPair *p, int *k, Str *v) {
    p->key = *k;
    ov45_02237A24(&p->val, v);
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

extern void ov45_02237A24(void *dst, void *src);
extern void ov00_021EC2EC(int a, void *p, int b);
extern void *ov00_021EC2A8(int a, u32 size);
extern void ov45_02236680(void *a, void *b, u32 len, int d);
extern void ov45_02236CB0(void *p);
extern void ov45_02236E38(void *p);
struct Tree;
extern void ov45_022425A0(struct Tree *p);
extern void ov45_02244150(void *p);
extern void ov45_02243A98(void *p);
extern void ov45_02244130(void *p);
extern void ov45_02237BC8(void *p);

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

extern int ov45_02237F7C(Str *a, Str *b);
extern void ov45_02243FD4(Iter *out, Tree *t, Iter it);
extern u8 ov45_02255228[];
Node *ov45_0224210C(Tree *t, Node *parent, u8 a, u8 b, Obj **val);
Node *ov45_02242CDC(Tree *t, Str *key, Node *n, Node *res);
Node *ov45_02242320(Tree *t, Obj **key, Node **pos, u8 *a, u8 *b);
void ov45_02243128(Iter *out, Tree *t, int *key);
void ov45_022431FC(Iter *out, Tree *t, int *key);
void ov45_02242C74(Iter *out, Tree *t, Str *key);

static inline BOOL StrLess(Str *a, Str *b) {
    return ov45_02237F7C(a, b) < 0;
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

extern Node *ov45_022370D0(Node *n, void *root);
extern void ov45_0223FD8C(Iter *out, Tree *t, void *key);
extern void *ov45_02255238;
extern u32 ov00_021FF368(void);
extern void ov45_02232B24(char *buf, u32 size, const char *fmt, ...);
extern char ov45_0225539C[];
extern char ov45_022553A4[];
extern void ov45_02237B58(Str *s, const char *cstr);
extern void ov45_0223F000(Vec *out, Str *s, Str *sep, int n);
extern void ov45_02237664(Vec *v);
extern long atol(const char *s);
extern void ov45_02243BC0(TreeHdr *dst, TreeHdr *src);
extern void ov45_02243AB8(InsertResult *out, Tree *t, int *key);
extern void ov45_02237D48(Str *dst, Str *src);
extern Node *ov45_0224404C(Tree *t, Node *parent, u8 a, u8 b, IntStrPair *val);
extern BOOL ov45_0224330C(Tree *t, int *id);
int ov45_02242A2C(Str *s);
void ov45_0224261C(Iter *out, Tree *t, Iter pos);

typedef struct Tag {
    u8 x;
} Tag;

IntStrPair *ov45_02242DCC(Tree *t, int *key);

static inline IntStrPair *MapIndex(Tree *t, int *key, Tag a, Tag b) {
    return ov45_02242DCC(t, key);
}

Node *ov45_02242E88(Tree *t, int *key, Node **pos, u8 *a, u8 *b);
BOOL ov45_0224307C(u8 *obj, int id);
BOOL ov45_022431AC(Tree *t, int *key);

int ov45_022428C8(void *a, void *b0, void *c);

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
    ov45_02243128(&r, (Tree *)((u8 *)n + 0x1c), &p.id);
    return r.current != (Node *)((u8 *)n + 0x20);
}

static inline BOOL PredCall(Pred *p, Node *n) {
    Iter r;
    ov45_02243128(&r, (Tree *)((u8 *)n + 0x1c), &p->id);
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
extern void *ov45_02242394(u32 size);
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
    CbObj *o = ov45_02242394(sizeof(CbObj));
    if (o != NULL) {
        CbObjBaseInit(o, x8);
        o->vt = vt;
        o->id = ov45_022577E0++;
        o->xc = xc;
    }
    return o;
}

extern int ov45_02237FD8(Str *a, const char *b);
extern void chatLeaveChannelA(void *h, const char *name, int x);
extern void ov45_0224335C(Ctx *c, Str *s);
extern void chatSendChannelMessageA(void *h, const char *name, const char *msg, int x);
extern void ov45_0224367C(Str *out, Ctx *c, int a);
extern char ov45_02255304[];
extern void chatSendUserMessageA(void *h, const char *name, const char *msg, int x);
extern void chatEnumChannelsA(void *h, const char *name, void *f1, void *f2, int h2, int z);
extern void ov45_0223FE20(void);
extern void ov45_0223FC6C(void);
extern void ov45_0223FE28(void);
extern void ov45_02240718(void);
extern int chatInChannelA(void *h, const char *name);
extern void chatEnumUsersA(void *h, const char *name, void *f, int h2, int z);
extern void chatGetChannelModeA(void *h, const char *name, void *f, int h2, int z);
extern void *ov45_022552B0;
extern void *ov45_02255298;
extern void *ov45_02255250;
void *ov45_02242858(u8 *p);
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
        ov00_021EC2EC(0, p, 0);
    }
}

extern void ov45_0223DAD4(Str *s, const char *cstr);
extern void chatSetChannelKeysA(void *h, const char *name, const char *who, int x, const char **key, const char **val);

extern void ov45_02243840(InsertResult *out, Tree *t, NamedEntry *e);
extern void ov45_02243F1C(TreeHdr *t);
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
    ov45_02237A24(&d->name, &s->name);
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
extern Str *ov45_02233ABC(Str *out, Str *a, char *b);
extern Str *ov45_022339F4(Str *out, Str *a, Str *b);

extern char ov45_0225533C[];
extern int printf(const char *fmt, ...);
extern void abort(void);
extern void ov45_02236EC4(Node *n, void *root);

typedef struct NodeHolder {
    Node **hdr;
    Node *p;
} NodeHolder;

static inline void IterTemp(Node *n) {
    Iter it;
    it = (Iter) { n };
}

static inline BOOL StrEqC(Str *a, const char *b) {
    return ov45_02237FD8(a, b) == 0;
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
Node *ov45_0224317C(Tree *t, int *key, Node *node, Node *res);

Node *ov45_02243250(Tree *t, int *key, Node *node, Node *res);

void *ov45_02242C3C(u8 *obj, Str *key);
int ov45_02242D30(u8 *obj, Str *name, Str *key2);

void ov45_02242BE0(NamedEntry *dst, Str *name, Entry *src) {
    ov45_02237A24(&dst->name, name);
    dst->e.a = src->a;
    dst->e.t.count = 0;
    dst->e.t.root = NULL;
    dst->e.t.leftmost = (Node *)&dst->e.t.root;
    ov45_02243BC0(&dst->e.t, &src->t);
    dst->e.x10 = src->x10;
    dst->e.b = src->b;
}

void *ov45_02242C3C(u8 *obj, Str *key) {
    Iter it;
    ov45_02242C74(&it, (Tree *)(obj + 0x80), key);
    return it.current == (Node *)(obj + 0x84) ? NULL : (u8 *)it.current + 0x2c;
}

void ov45_02242C74(Iter *out, Tree *t, Str *key) {
    Node *n = ov45_02242CDC(t, key, t->head.left, &t->head);
    if (n == &t->head || StrLess(key, &n->key.str)) {
        out->current = &t->head;
        return;
    }
    out->current = n;
}

Node *ov45_02242CDC(Tree *t, Str *key, Node *n, Node *res) {
    while (n != NULL) {
        if (!StrLess(&n->key.str, key)) {
            res = n;
            n = n->left;
        } else {
            n = n->right;
        }
    }
    return res;
}

int ov45_02242D30(u8 *obj, Str *name, Str *key2) {
    int id;
    Iter it;
    InsertResult ir;
    Tag t1;
    Tag t0;
    id = ov45_02242A2C(name);
    if (id == -1) {
        return -1;
    }
    ov45_02242C74(&it, (Tree *)(obj + 0x80), key2);
    if (it.current == (Node *)(obj + 0x84)) {
        return -1;
    }
    ov45_02243AB8(&ir, (Tree *)((u8 *)it.current + 0x1c), &id);
    t1 = (Tag) { 0 };
    t0 = (Tag) { 0 };
    ov45_02237D48(&ov45_02242DCC((Tree *)(obj + 0x70), &id)->val, name);
    return id;
}

IntStrPair *ov45_02242DCC(Tree *t, int *key) {
    Node *pos;
    u8 a;
    u8 b;
    Node *parent = ov45_02242E88(t, key, &pos, &a, &b);
    if (pos == NULL || pos->key.i < *key) {
        Str empty;
        IntStrPair pair;
        Node *n;
        StrInit(&empty);
        PairInit(&pair, key, &empty);
        n = ov45_0224404C(t, parent, a, b, &pair);
        ov45_02237BC8(&pair.val);
        ov45_02237BC8(&empty);
        return (IntStrPair *)&n->key;
    }
    return (IntStrPair *)&pos->key;
}

Node *ov45_02242E88(Tree *t, int *key, Node **pos, u8 *a, u8 *b) {
    Node *parent;
    Node *n;
    *pos = NULL;
    n = t->head.left;
    *a = 1;
    *b = 1;
    parent = &t->head;
    while (n != NULL) {
        parent = n;
        if (*key < n->key.i) {
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
