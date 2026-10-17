#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

// The mode name is built and dropped again; nothing is reported to the channel.
void LobbyChat_OnUserModeChanged(int chat, int channel, int nick, int mode) {
    Str modeName;
    u32 *words;
    u32 i;
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == LOBBY_CHAT_STATE_ERROR) {
        return;
    }
    i = 0;
    words = (u32 *)&modeName;
    for (; i < 3; i++) {
        words[i] = 0;
    }
    switch (mode) {
    case 0:
        LobbyString_AssignCStr(&modeName, sStr_normal);
        break;
    case 1:
        LobbyString_AssignCStr(&modeName, sStr_voice);
        break;
    case 2:
        LobbyString_AssignCStr(&modeName, sStr_ops);
        break;
    case 3:
        LobbyString_AssignCStr(&modeName, sStr_VoiceOps);
        break;
    default:
        LobbyString_AssignCStr(&modeName, sStr_invalid);
        break;
    }
    LobbyString_Destroy(&modeName);
}
