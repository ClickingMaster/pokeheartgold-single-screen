#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChat_OnBroadcastKeyChanged(void *a, char *s, char *name, void *arg, void *extra) {
    Str x, y, z, t;
    int id;
    LobbyChannelCallbacks *callbacks;
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
    if (!name) {
        id = -1;
    } else {
        LobbyString_InitFromCStr(&x, name);
        id = LobbyChat_FindUserIdByNick(gLobbyChatManager, &x);
        LobbyString_Destroy(&x);
    }
    LobbyString_InitFromCStr(&y, s);
    callbacks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &y);
    LobbyString_Destroy(&y);
    LobbyString_InitFromCStr(&z, s);
    LobbyChat_StripChannelPrefix(&t, gLobbyChatManager, &z);
    callbacks->onBroadcastKeyChanged(StrData(&t), id, arg, extra, callbacks->param);
    LobbyString_Destroy(&t);
    LobbyString_Destroy(&z);
}
