#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

int LobbyChat_TimerConnectTimeout(void) {
    LobbyChatManager *manager = gLobbyChatManager;
    manager->lastError = 3;
    manager->state = LOBBY_CHAT_STATE_ERROR;
    return 0;
}
