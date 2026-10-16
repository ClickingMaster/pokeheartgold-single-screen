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
extern int LobbyString_Destroy();
#include "overlay45_string_search.h"
Vec *LobbyStringVectorBuffer_Destroy(Vec *p);
Vec *LobbyCharVectorBuffer_Destroy(Vec *p);
void LobbyCharVectorBuffer_Allocate(Vec *p, u32 n, Small tag);
u8 *LobbyCharVectorBuffer_Deallocate(u8 *p);
Vec *LobbyStringVectorBuffer_Deallocate(Vec *p);

#pragma optimization_level 3

Vec *LobbyStringVectorBuffer_Destroy(Vec *p) {
    S *first = p->data + p->extra, *end = first + p->len;
    while (end > first) {
        end--;
        LobbyString_Destroy(end);
    }
    p->len = 0;
    LobbyStringVectorBuffer_Deallocate(p);
    return p;
}

Vec *LobbyCharVectorBuffer_Destroy(Vec *p) {
    p->len = 0;
    LobbyCharVectorBuffer_Deallocate((u8 *)p);
    return p;
}

void LobbyCharVectorBuffer_Allocate(Vec *p, u32 n, Small tag) {
    p->data = LobbyMem_Alloc(n);
    p->cap = n;
}
