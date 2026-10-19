#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "ppw_lobby_internal.h"

PPWLobbyWork *PPWLobby_GetWork(void) {
    return gPPWLobbyWork;
}
