#include "global.h"

#include "ppw_lobby_state.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef union {
    struct {
        u32 flag : 1, rest : 31;
        u8 raw[8];
    };

    char bytes[12];
} String;

typedef struct {
    int state;
    u8 p[0xd0 - 4];
    int mode;
} Work;

extern Work *volatile gPPWLobbyWork;
extern void PPWLobbyChannel_GetName(String *, void *);
extern void LobbyString_Destroy(String *);
extern int LobbyChat_IsInChannel(char *);

static inline BOOL active(Work *w) {
    return w->mode == 5 ? FALSE : TRUE;
}

int PPW_LobbyGetSubChannelState(void);
#pragma opt_propagation off

int PPW_LobbyGetSubChannelState(void) {
    String text;
    Work *w = gPPWLobbyWork;
    int result;
    if (!w) {
        return PPW_LOBBY_CHANNEL_STATE_ERROR;
    }
    if (w && w->state == PPW_LOBBY_STATE_ERROR) {
        return PPW_LOBBY_CHANNEL_STATE_ERROR;
    }
    if (w && gPPWLobbyWork->state != PPW_LOBBY_STATE_READY) {
        return PPW_LOBBY_CHANNEL_STATE_ERROR;
    }
    {
        BOOL enabled = active(w);
        if (enabled == 0) {
            return PPW_LOBBY_CHANNEL_STATE_NONE;
        }
    }
    PPWLobbyChannel_GetName(&text, &w->mode);
    result = LobbyChat_IsInChannel(!text.flag ? &text.bytes[1] : *(char **)&text.raw[4]);
    LobbyString_Destroy(&text);
    if (result) {
        return PPW_LOBBY_CHANNEL_STATE_JOINED;
    }
    return PPW_LOBBY_CHANNEL_STATE_JOINING;
}
