#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "ppw_lobby_internal.h"

extern u8 *gPPWLobbyWork;

u8 *PPWLobby_GetWork(void) {
    return gPPWLobbyWork;
}
