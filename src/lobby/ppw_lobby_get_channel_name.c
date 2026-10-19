#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "ppw_lobby_internal.h"
#include "ppw_lobby_state.h"

extern int PPWLobby_SendMessage();
int PPW_LobbySendChannelBinaryMessage(int a, void *b, void *c, void *d);
int PPW_LobbySendPlayerBinaryMessage(int a, void *b, void *c, void *d);
int PPW_LobbyGetMyUserId(void);

void PPWLobby_GetChannelName(Str *out, PPWLobbyChannel *channels, int subChannel) {
    volatile Str copy;
    if (!subChannel) {
        PPWLobbyChannel_GetName(out, &channels[0]);
        copy = *out;
        return;
    }
    PPWLobbyChannel_GetName(out, &channels[1]);
    copy = *out;
}

int PPW_LobbySendChannelBinaryMessage(int a, void *b, void *c, void *d) {
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
    return PPWLobby_SendMessage(-1, a, 1, 1, b, c, d);
}

int PPW_LobbySendPlayerBinaryMessage(int a, void *b, void *c, void *d) {
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
    return PPWLobby_SendMessage(a, 5, 1, 1, b, c, d);
}

int PPW_LobbyGetMyUserId(void) {
    PPWLobbyWork *work = gPPWLobbyWork;
    if (work) {
        return work->userId;
    }
    return -1;
}
