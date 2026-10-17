#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChatManager_GetNick(Str *out, LobbyChatManager *manager) {
    LobbyString_InitCopy(out, &manager->nick);
}
