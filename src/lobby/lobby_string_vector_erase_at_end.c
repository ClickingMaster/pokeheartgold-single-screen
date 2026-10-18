#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>
#pragma warn_uninitializedvar off

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

typedef struct Small {
    u8 x;
} Small;

extern int LobbyString_Destroy();
#include "overlay45_string_search.h"
void LobbyStringVector_EraseAtEnd(Vec *p, u32 n, Small tag);

#pragma optimization_level 3

void LobbyStringVector_EraseAtEnd(Vec *p, u32 n, Small tag) {
    S *end = p->data + p->len;
    p->len -= n;
    if (!n) {
        return;
    }
    do {
        end--;
        LobbyString_Destroy(end);
    } while (--n);
}
