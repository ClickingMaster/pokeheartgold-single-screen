#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>
#pragma force_active on
#pragma warn_uninitializedvar off
#define V(p, o) (*(volatile u32 *)((p) + (o)))
#define W(p, o) (*(u32 *)((p) + (o)))
#define H(p, o) (*(u16 *)((p) + (o)))

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

typedef struct Small {
    u8 x;
} Small;

typedef struct E {
    int key, val;
} E;

extern void *LobbyMem_Alloc(u32);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
#include "overlay45_string_search.h"
void LobbyStringVectorBuffer_Allocate(Vec *p, u32 n, Small tag);

#pragma optimization_level 3

void LobbyStringVectorBuffer_Allocate(Vec *p, u32 n, Small tag) {
    if (n > 0x15555555) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    p->data = LobbyMem_Alloc(n * 12);
    p->cap = n;
}
