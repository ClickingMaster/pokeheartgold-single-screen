#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChatManager_GetGameKey(Str *out, LobbyChatManager *manager) {
    LobbyString_InitCopy(out, &manager->gameKey);
}

// After a lost connection the session is freed at once; otherwise LobbyChat_OnDisconnected frees it.
int LobbyChat_Shutdown(void) {
    if (gLobbyChatManager == NULL) {
        return 0;
    }
    if (gLobbyChatManager->state == LOBBY_CHAT_STATE_DISCONNECTING) {
        return 0;
    }
    LobbyMem_ReleaseReserve();
    if (gLobbyChatManager->disconnected != 0) {
        chatDisconnect(gLobbyChatManager->connection);
        if (gLobbyChatManager != NULL) {
            LobbyChatManager *manager = gLobbyChatManager;
            if (manager != NULL) {
                LobbyChatManager_Destroy(manager);
                OpDelete(manager);
            }
            gLobbyChatManager = NULL;
        }
    } else {
        gLobbyChatManager->state = LOBBY_CHAT_STATE_DISCONNECTING;
        chatDisconnect(gLobbyChatManager->connection);
    }
    return 0;
}
