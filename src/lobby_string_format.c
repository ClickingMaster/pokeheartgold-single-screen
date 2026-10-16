#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>
#pragma warn_uninitializedvar off
#define V(p, o) (*(volatile u32 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))

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

typedef struct Small {
    u8 x;
} Small;

typedef struct Vec {
    char *data;
    u32 len, cap;
} Vec;

typedef struct Node {
    void *left, *right;
    u32 parent;
    u32 key;
} Node;

typedef struct Body {
    u64 a, b;
    u32 c, d;
} Body;

typedef struct Record {
    int type;
    u32 words[6];
} Record;

extern void *LobbyCharVector_Data(Vec *);
extern Node *LobbyTimerMap_LowerBound(u8 *, u32 *, Node *, Node *);
extern void LobbyTimerMap_Find(Node **, u8 *, u32 *);
extern int LobbyCharVector_Grow();
extern int LobbyCharVector_Destroy();
extern int LobbyString_InitFromCStr();
void LobbyString_FormatN(u8 *p, int n, char *fmt, ...);
void LobbyTimerMap_Find(Node **out, u8 *p, u32 *key);
Node *LobbyTimerMap_LowerBound(u8 *p, u32 *key, Node *n, Node *best);

void LobbyString_FormatN(u8 *p, int n, char *fmt, ...) {
    va_list ap;
    Vec v;
    v.data = 0;
    v.len = 0;
    v.cap = 0;
    va_start(ap, fmt);
    LobbyCharVector_Grow(&v, n);
    OS_VSNPrintf(LobbyCharVector_Data(&v), n, fmt, ap);
    LobbyString_InitFromCStr(p, LobbyCharVector_Data(&v));
    LobbyCharVector_Destroy(&v);
    va_end(ap);
}
