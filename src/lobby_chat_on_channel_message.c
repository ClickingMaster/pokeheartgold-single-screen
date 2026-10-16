#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChat_OnInvited(void) {
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
}

void LobbyChat_OnChannelMessage(void *a, char *s, char *name, void *arg, int mode) {
    Str x, y, z, t;
    int id;
    LobbyChannelCallbacks *callbacks;
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
    if (mode != 3) {
        return;
    }
    LobbyString_InitFromCStr(&x, name);
    id = LobbyChat_FindUserIdByNick(gLobbyChatManager, &x);
    LobbyString_Destroy(&x);
    LobbyString_InitFromCStr(&y, s);
    callbacks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &y);
    LobbyString_Destroy(&y);
    LobbyString_InitFromCStr(&z, s);
    LobbyChat_StripChannelPrefix(&t, gLobbyChatManager, &z);
    callbacks->onChannelMessage(StrData(&t), id, arg, callbacks->param);
    LobbyString_Destroy(&t);
    LobbyString_Destroy(&z);
}

void LobbyChat_OnKicked(void *a, char *s) {
    Str t;
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
    LobbyString_InitFromCStr(&t, s);
    LobbyChat_RemoveChannel(gLobbyChatManager, &t);
    LobbyString_Destroy(&t);
}
