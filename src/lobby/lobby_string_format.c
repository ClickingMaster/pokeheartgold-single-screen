#include "global.h"

#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

void LobbyString_FormatN(Str *out, u32 n, const char *fmt, ...) {
    va_list ap;
    LobbyCharVector v;
    v.data = 0;
    v.size = 0;
    v.capacity = 0;
    va_start(ap, fmt);
    LobbyCharVector_Grow(&v, n);
    OS_VSNPrintf(LobbyCharVector_Data(&v), n, fmt, ap);
    LobbyString_InitFromCStr(out, LobbyCharVector_Data(&v));
    LobbyCharVector_Destroy(&v);
    va_end(ap);
}
