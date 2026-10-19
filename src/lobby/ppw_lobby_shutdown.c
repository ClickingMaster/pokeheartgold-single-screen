#include "global.h"

#include "ppw_lobby_state.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_work.h"

extern int LobbyChat_Shutdown(void);
int PPW_LobbyShutdownAsync(void);

int PPW_LobbyShutdownAsync(void) {
    int result, error;
    volatile u8 unused;
    PPWLobbyWork *work;
    if (!gPPWLobbyWork) {
        return 0;
    }
    if (gPPWLobbyWork->state == PPW_LOBBY_STATE_CLOSED) {
        return 0;
    }
    LobbyTimerMap_Clear(&gPPWLobbyWork->timers);
    gPPWLobbyWork->state = PPW_LOBBY_STATE_CLOSED;
    result = LobbyChat_Shutdown();
    work = gPPWLobbyWork;
    unused = 0;
    if (result != 1) {
        if (result == 2) {
            error = 3;
        } else {
            error = 0;
        }
    } else {
        error = 2;
    }
    if (error) {
        work->error = error;
        work->state = PPW_LOBBY_STATE_ERROR;
    }
    return result;
}
