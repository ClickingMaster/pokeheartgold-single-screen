#include "global.h"

#include "chat_user_mode.h"

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
    case CHAT_USER_MODE_NORMAL:
        LobbyString_AssignCStr(&modeName, sStr_normal);
        break;
    case CHAT_USER_MODE_VOICE:
        LobbyString_AssignCStr(&modeName, sStr_voice);
        break;
    case CHAT_USER_MODE_OP:
        LobbyString_AssignCStr(&modeName, sStr_ops);
        break;
    case CHAT_USER_MODE_VOICE_OP:
        LobbyString_AssignCStr(&modeName, sStr_VoiceOps);
        break;
    default:
        LobbyString_AssignCStr(&modeName, sStr_invalid);
        break;
    }
    LobbyString_Destroy(&modeName);
}
