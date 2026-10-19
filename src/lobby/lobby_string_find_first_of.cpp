#include <nitro/types.h>

#include "lobby_string.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern "C" void LobbyString_FindFirstOf(Str *self, const Str &str, int pos);

extern "C" void LobbyString_FindFirstOf(Str *self, const Str &str, int pos) {
    const char *p;
    u32 n;
    if (!str.l.isLong) {
        p = str.s.buf;
        n = str.s.len;
    } else {
        p = str.l.ptr;
        n = str.l.size;
    }
    LobbyString_FindFirstOfChars(self, p, pos, n);
}
