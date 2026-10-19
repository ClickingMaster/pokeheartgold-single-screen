#include "global.h"

#include "ppw_lobby_internal.h"
#include "ppw_lobby_state.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern int LobbyChat_IsInChannel(char *);

static inline BOOL active(PPWLobbyWork *w) {
    return w->subChannel.kind == 5 ? FALSE : TRUE;
}

int PPW_LobbyGetSubChannelState(void);
#pragma opt_propagation off

int PPW_LobbyGetSubChannelState(void) {
    Str text;
    PPWLobbyWork *w = gPPWLobbyWork;
    int result;
    if (!w) {
        return PPW_LOBBY_CHANNEL_STATE_ERROR;
    }
    if (w && w->state == PPW_LOBBY_STATE_ERROR) {
        return PPW_LOBBY_CHANNEL_STATE_ERROR;
    }
    if (w && PPWLobby_GetState() != PPW_LOBBY_STATE_READY) {
        return PPW_LOBBY_CHANNEL_STATE_ERROR;
    }
    {
        BOOL enabled = active(w);
        if (enabled == 0) {
            return PPW_LOBBY_CHANNEL_STATE_NONE;
        }
    }
    PPWLobbyChannel_GetName(&text, &w->subChannel);
    result = LobbyChat_IsInChannel(!text.l.isLong ? text.s.buf : text.l.ptr);
    LobbyString_Destroy(&text);
    if (result) {
        return PPW_LOBBY_CHANNEL_STATE_JOINED;
    }
    return PPW_LOBBY_CHANNEL_STATE_JOINING;
}
