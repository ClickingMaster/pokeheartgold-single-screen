#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChat_OnUserChangedNick(void *a, void *b, char *c, char *d) {
    Str x, y;
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
    LobbyString_InitFromCStr(&x, c);
    LobbyString_InitFromCStr(&y, d);
    LobbyChat_RenameUser(gLobbyChatManager, &x, &y);
    LobbyString_Destroy(&y);
    LobbyString_Destroy(&x);
}
