#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChat_OnUserParted(int unused, char *name, char *value) {
    Str a, b, c, d, e, f;
    int id;
    LobbyChannelCallbacks *callbacks;
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
    LobbyString_InitFromCStr(&a, value);
    id = LobbyChat_FindUserIdByNick(gLobbyChatManager, &a);
    LobbyString_Destroy(&a);
    if (id == -1) {
        return;
    }
    LobbyString_InitFromCStr(&b, name);
    callbacks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &b);
    LobbyString_Destroy(&b);
    LobbyString_InitFromCStr(&c, name);
    LobbyChat_StripChannelPrefix(&d, gLobbyChatManager, &c);
    callbacks->onUserParted(StrData(&d), id, callbacks->param);
    LobbyString_Destroy(&d);
    LobbyString_Destroy(&c);
    LobbyString_InitFromCStr(&e, value);
    LobbyString_InitFromCStr(&f, name);
    LobbyChat_RemoveChannelUser(gLobbyChatManager, &e, &f);
    LobbyString_Destroy(&f);
    LobbyString_Destroy(&e);
}
