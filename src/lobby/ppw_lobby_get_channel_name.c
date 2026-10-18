#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "ppw_lobby_internal.h"

#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))

extern u8 *volatile gPPWLobbyWork;
extern int PPWLobby_SendMessage();
int PPW_LobbySendChannelBinaryMessage(int a, void *b, void *c, void *d);
int PPW_LobbySendPlayerBinaryMessage(int a, void *b, void *c, void *d);
int PPW_LobbyGetMyUserId(void);

void PPWLobby_GetChannelName(Str *p, u8 *q, int x) {
    volatile Str copy;
    if (!x) {
        PPWLobbyChannel_GetName((u8 *)p, q);
        copy = *p;
        return;
    }
    PPWLobbyChannel_GetName((u8 *)p, q + 24);
    copy = *p;
}

int PPW_LobbySendChannelBinaryMessage(int a, void *b, void *c, void *d) {
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
    return PPWLobby_SendMessage(-1, a, 1, 1, b, c, d);
}

int PPW_LobbySendPlayerBinaryMessage(int a, void *b, void *c, void *d) {
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
    return PPWLobby_SendMessage(a, 5, 1, 1, b, c, d);
}

int PPW_LobbyGetMyUserId(void) {
    u8 *p = gPPWLobbyWork;
    if (p) {
        return W(p, 76);
    }
    return -1;
}
