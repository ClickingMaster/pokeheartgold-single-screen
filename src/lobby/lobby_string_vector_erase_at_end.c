#include "global.h"

#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

extern int LobbyString_Destroy();
#include "overlay45_string_search.h"

void LobbyStringVector_EraseAtEnd(LobbyStringVector *p, u32 n, LobbyTag tag) {
    Str *end = p->data + p->size;
    p->size -= n;
    if (!n) {
        return;
    }
    do {
        end--;
        LobbyString_Destroy(end);
    } while (--n);
}
