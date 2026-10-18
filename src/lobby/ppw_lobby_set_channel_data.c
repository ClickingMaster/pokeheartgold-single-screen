#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>
#pragma warn_uninitializedvar off
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))

extern u8 *volatile gPPWLobbyWork;
extern int PPWLobby_SetChannelData();
int PPW_LobbySetChannelData(void *a, void *b, void *c, void *d, void *e);

int PPW_LobbySetChannelData(void *a, void *b, void *c, void *d, void *e) {
    u8 *root = gPPWLobbyWork;
    if (!root) {
        return 4;
    }
    if (root) {
        if (W(root, 0) == 10) {
            return 7;
        }
    }
    if (root) {
        if (W(gPPWLobbyWork, 0) != 8) {
            return 4;
        }
    }
    return PPWLobby_SetChannelData(a, b, c, d, e);
}
