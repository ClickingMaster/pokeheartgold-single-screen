#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_internal.h"

void PPWLobbyPlayerSet_Insert(InsertResult *out, List *l, Session **key) {
    Node *parent;
    u8 left;
    u8 flag;
    Node *y = PPWLobbyPlayerSet_FindInsertPos(l, key, &parent, &left, &flag);
    if (parent == NULL || parent->val->id < (*key)->id) {
        out->current = PPWLobbyPlayerSet_InsertNode(l, y, left, flag, key);
        out->inserted = ov45_02255200[0];
        return;
    }
    out->current = parent;
    out->inserted = ov45_02255200[1];
}

void PPWLobbyPlayerSet_Erase(NodeIter *out, List *l, NodeIter it) {
    Node *n = it.current;
    if (n == l->leftmost) {
        ov45_02236E58(&it);
        l->leftmost = it.current;
    }
    out->current = LobbyTree_EraseRebalance(n, l->root);
    if (n != NULL) {
        DWC_Free(0, n, 0);
    }
    l->count--;
}

Node *PPWLobbyPlayerSet_InsertNode(List *l, Node *parent, u8 left, u8 flag, Session **key) {
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
        printf(sStr_TreeInsertLengthError_2);
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
    LobbyTree_InsertRebalance(n, l->root);
    if (flag) {
        l->leftmost = n;
    }
    tmp = h.d;
    if (tmp != NULL) {
        DWC_Free(0, tmp, 0);
    }
    return n;
}

void LobbyU32Vector_Reserve(Vec *v, u32 n) {
    Vec b;
    u32 *first;
    int cnt;
    b.ptr = NULL;
    b.size = 0;
    b.cap = 0;
    {
        Tag ta = { 0 };
        LobbyU32Vector_Allocate(&b, n, ta);
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
    LobbyU32Vector_Destroy2(&b);
}

void LobbyU32Vector_PushBackSlow(Vec *v, u32 *x, Tag unused) {
    TouchTag();
    LobbyU32Vector_RecommendSize(v, 1);
    LobbyU32Vector_AppendFillSlow(v, 1, x);
}

Node *PPWLobbyPlayerSet_FindInsertPos(List *l, Session **key, Node **parent, u8 *left, u8 *flag) {
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

Vec *LobbyU32Vector_Destroy2(Vec *v) {
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

void LobbyU32Vector_Allocate(Vec *v, u32 n, Tag unused) {
    if (n > 0x3FFFFFFF) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    v->ptr = LobbyMem_Alloc(n * 4);
    v->cap = n;
}

u32 LobbyU32Vector_RecommendSize(Vec *v, u32 n) {
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

void LobbyU32Vector_AppendFillSlow(Vec *v, u32 n, u32 *x) {
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
        u32 rec = LobbyU32Vector_RecommendSize(v, v->size + n - v->cap);
        Tag t3 = { 0 };
        t4.c = 0;
        LobbyU32VectorBuffer_Allocate(&b, rec, t3);
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
    LobbyU32VectorBuffer_Destroy(&b);
}
