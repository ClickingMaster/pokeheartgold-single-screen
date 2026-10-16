#include <string.h>

#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChat_OnTopicChanged(void *a, char *s, void *b) {
    Str x, y, z;
    LobbyChannelCallbacks *callbacks;
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
    LobbyString_InitFromCStr(&x, s);
    callbacks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &x);
    LobbyString_Destroy(&x);
    LobbyString_InitFromCStr(&y, s);
    LobbyChat_StripChannelPrefix(&z, gLobbyChatManager, &y);
    callbacks->onTopicChanged(StrData(&z), b, callbacks->param);
    LobbyString_Destroy(&z);
    LobbyString_Destroy(&y);
}

void LobbyChat_OnChannelModeChanged(void) {
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
}

void LobbyChat_OnUserListUpdated(void) {
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
}

void LobbyChat_OnConnected(void *a, int n) {
    int zero;
    LobbyChatManager *manager;
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
    zero = 0;
    LobbyTimerMap_EraseKey(gLobbyChatManager->timerMap, &zero, 0);
    if (!n) {
        manager = gLobbyChatManager;
        manager->lastError = 3;
        manager->state = 5;
        gLobbyChatManager->disconnected = 1;
        return;
    }
    if (gLobbyChatManager->state != 5) {
        gLobbyChatManager->state = 2;
    }
}

void LobbyChat_OnFillInUser(void *a, void *b, char *out) {
    Str t;
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
    LobbyChatManager_GetUser(&t, gLobbyChatManager);
    strncpy(out, StrData(&t), 127);
    LobbyString_Destroy(&t);
}

void LobbyChatManager_GetUser(Str *out, LobbyChatManager *manager) {
    LobbyString_InitCopy(out, &manager->user);
}
