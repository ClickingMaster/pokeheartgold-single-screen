#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>
#pragma warn_uninitializedvar off

typedef struct Vec {
    char *data;
    u32 len, cap;
} Vec;

extern void *LobbyCharVector_Data(Vec *);
extern int LobbyCharVector_Grow();
extern int LobbyCharVector_Destroy();
extern int LobbyString_InitFromCStr();
void LobbyString_FormatN(u8 *p, int n, char *fmt, ...);

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
