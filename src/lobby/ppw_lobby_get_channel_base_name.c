#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "ppw_lobby_internal.h"

#pragma warn_uninitializedvar off

extern void *LobbyString_InitCopy(void *, void *);

void *PPWLobby_GetChannelBaseName(u8 *p, u8 *q) {
    return LobbyString_InitCopy(p, q + 88);
}
