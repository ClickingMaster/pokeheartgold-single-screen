#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "ppw_lobby_internal.h"
#include "ppw_lobby_state.h"

extern int PPWLobby_SetChannelData();
int PPW_LobbySetChannelData(void *a, void *b, void *c, void *d, void *e);

int PPW_LobbySetChannelData(void *a, void *b, void *c, void *d, void *e) {
    PPWLobbyWork *work = gPPWLobbyWork;
    if (!work) {
        return 4;
    }
    if (work) {
        if (work->state == PPW_LOBBY_STATE_ERROR) {
            return 7;
        }
    }
    if (work) {
        if (PPWLobby_GetState() != PPW_LOBBY_STATE_READY) {
            return 4;
        }
    }
    return PPWLobby_SetChannelData(a, b, c, d, e);
}
