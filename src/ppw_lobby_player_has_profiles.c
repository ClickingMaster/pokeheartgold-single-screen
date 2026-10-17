#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_internal.h"

BOOL PPWLobbyPlayer_HasProfiles(Session *s) {
    return s->state >= 1;
}
