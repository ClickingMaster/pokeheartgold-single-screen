#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

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

typedef struct Vec {
    S *data;
    u32 len, cap, z, extra;
} Vec;

typedef struct Node {
    void *left, *right;
    u32 parent;
    u32 key;
} Node;

extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
#include "overlay45_string_search.h"
void PPWLobbyPlayerSet_DestroyNodes(void *ctx, Node *p);
u32 LobbyStringVector_RecommendSize(Vec *p, u32 n, ...);

void PPWLobbyPlayerSet_DestroyNodes(void *ctx, Node *p) {
    if (p->left) {
        PPWLobbyPlayerSet_DestroyNodes(ctx, p->left);
    }
    if (p->right) {
        PPWLobbyPlayerSet_DestroyNodes(ctx, p->right);
    }
    if (p) {
        DWC_Free(0, p, 0);
    }
}

u32 LobbyStringVector_RecommendSize(Vec *p, u32 n, ...) {
    u32 cap = p->cap, a, b;
    va_list ap;
    va_start(ap, n);
    va_end(ap);
    if (n > 0x15555555 - cap) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    if (cap < 0x071c71c7) {
        a = (cap + 1) * 3 / 5;
        return cap + *(n > a ? &n : &a);
    }
    if (cap < 0x0e38e38e) {
        b = (cap + 1) / 2;
        return cap + *(n > b ? &n : &b);
    }
    return 0x15555555;
}
