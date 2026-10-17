#include <string.h>

#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChat_OnTopicChanged(void *chat, char *channel, void *topic) {
    Str channelKey;
    Str fullName;
    Str channelName;
    LobbyChannelCallbacks *callbacks;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    LobbyString_InitFromCStr(&channelKey, channel);
    callbacks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &channelKey);
    LobbyString_Destroy(&channelKey);
    LobbyString_InitFromCStr(&fullName, channel);
    LobbyChat_StripChannelPrefix(&channelName, gLobbyChatManager, &fullName);
    callbacks->onTopicChanged(StrData(&channelName), topic, callbacks->param);
    LobbyString_Destroy(&channelName);
    LobbyString_Destroy(&fullName);
}

void LobbyChat_OnChannelModeChanged(void) {
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
}

void LobbyChat_OnUserListUpdated(void) {
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
}

void LobbyChat_OnConnected(void *chat, int success) {
    int timerId;
    LobbyChatManager *manager;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    // stop the connect timeout
    timerId = 0;
    LobbyTimerMap_EraseKey(gLobbyChatManager->timerMap, &timerId, 0);
    if (!success) {
        manager = gLobbyChatManager;
        manager->lastError = 3;
        manager->state = LOBBY_CHAT_STATE_ERROR;
        gLobbyChatManager->disconnected = 1;
        return;
    }
    if (gLobbyChatManager->state != LOBBY_CHAT_STATE_ERROR) {
        gLobbyChatManager->state = LOBBY_CHAT_STATE_CONNECTED;
    }
}

void LobbyChat_OnFillInUser(void *chat, void *param, char *user) {
    Str userName;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    LobbyChatManager_GetUser(&userName, gLobbyChatManager);
    strncpy(user, StrData(&userName), 127);
    LobbyString_Destroy(&userName);
}

void LobbyChatManager_GetUser(Str *out, LobbyChatManager *manager) {
    LobbyString_InitCopy(out, &manager->user);
}
