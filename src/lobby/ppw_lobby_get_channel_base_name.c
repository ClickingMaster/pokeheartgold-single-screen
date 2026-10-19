#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "ppw_lobby_internal.h"

Str *PPWLobby_GetChannelBaseName(Str *out, PPWLobbyWork *work) {
    return LobbyString_InitCopy(out, &work->channelBaseName);
}
