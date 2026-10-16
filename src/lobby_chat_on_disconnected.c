#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChat_OnRaw(void) {
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
}

void LobbyChat_OnDisconnected(void) {
    int zero;
    LobbyChatManager *manager;
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
    zero = 0;
    LobbyTimerMap_EraseKey(gLobbyChatManager->timerMap, &zero, 0);
    manager = gLobbyChatManager;
    if (manager->state == 4) {
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
    manager->state = 5;
    gLobbyChatManager->disconnected = 1;
}
