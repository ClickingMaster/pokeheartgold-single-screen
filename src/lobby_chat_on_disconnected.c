#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChat_OnRaw(void) {
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
}

// A disconnect requested by LobbyChat_Shutdown frees the session; any other one is an error.
void LobbyChat_OnDisconnected(void) {
    int timerId;
    LobbyChatManager *manager;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    // stop the connect timeout
    timerId = 0;
    LobbyTimerMap_EraseKey(gLobbyChatManager->timerMap, &timerId, 0);
    manager = gLobbyChatManager;
    if (manager->state == LOBBY_CHAT_STATE_DISCONNECTING) {
        if (!manager) {
            return;
        }
        if (manager) {
            LobbyChatManager_Destroy(manager);
            if (manager) {
                DWC_Free(0, manager, 0);
            }
        }
        gLobbyChatManager = 0;
        return;
    }
    manager->lastError = 3;
    manager->state = LOBBY_CHAT_STATE_ERROR;
    gLobbyChatManager->disconnected = 1;
}
