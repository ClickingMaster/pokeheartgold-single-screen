#pragma force_active on
#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChatManager_GetGameKey(Str *out, LobbyChatManager *manager) {
    LobbyString_InitCopy(out, &manager->gameKey);
}

int LobbyChat_Shutdown(void) {
    if (gLobbyChatManager == NULL) {
        return 0;
    }
    if (gLobbyChatManager->state == 4) {
        return 0;
    }
    LobbyMem_ReleaseReserve();
    if (gLobbyChatManager->disconnected != 0) {
        chatDisconnect(gLobbyChatManager->connection);
        if (gLobbyChatManager != NULL) {
            LobbyChatManager *d = gLobbyChatManager;
            if (d != NULL) {
                LobbyChatManager_Destroy(d);
                OpDelete(d);
            }
            gLobbyChatManager = NULL;
        }
    } else {
        gLobbyChatManager->state = 4;
        chatDisconnect(gLobbyChatManager->connection);
    }
    return 0;
}
