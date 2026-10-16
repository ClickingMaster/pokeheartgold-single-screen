#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChat_OnUserChangedNick(void *chat, void *channel, char *oldNick, char *newNick) {
    Str oldNickStr;
    Str newNickStr;
    if (gLobbyChatManager && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    LobbyString_InitFromCStr(&oldNickStr, oldNick);
    LobbyString_InitFromCStr(&newNickStr, newNick);
    LobbyChat_RenameUser(gLobbyChatManager, &oldNickStr, &newNickStr);
    LobbyString_Destroy(&newNickStr);
    LobbyString_Destroy(&oldNickStr);
}
