#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u8 *gPPWLobbyWork;
extern void LobbyTimerMap_Clear(void *);
extern int LobbyChat_Shutdown(void);
int PPW_LobbyShutdownAsync(void);

int PPW_LobbyShutdownAsync(void) {
    int result, error;
    volatile u8 unused;
    u8 *p;
    if (!gPPWLobbyWork) {
        return 0;
    }
    if (*(int *)gPPWLobbyWork == 9) {
        return 0;
    }
    LobbyTimerMap_Clear(gPPWLobbyWork + 0x134);
    *(int *)gPPWLobbyWork = 9;
    result = LobbyChat_Shutdown();
    p = gPPWLobbyWork;
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
        *(int *)(p + 4) = error;
        *(int *)p = 10;
    }
    return result;
}
