#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void LobbyChat_OnUserModeChanged(int a, int b, int c, int mode) {
    Str str;
    u32 *w;
    u32 i;
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == 5) {
        return;
    }
    i = 0;
    w = (u32 *)&str;
    for (; i < 3; i++) {
        w[i] = 0;
    }
    switch (mode) {
    case 0:
        LobbyString_AssignCStr(&str, sStr_normal);
        break;
    case 1:
        LobbyString_AssignCStr(&str, sStr_voice);
        break;
    case 2:
        LobbyString_AssignCStr(&str, sStr_ops);
        break;
    case 3:
        LobbyString_AssignCStr(&str, sStr_VoiceOps);
        break;
    default:
        LobbyString_AssignCStr(&str, sStr_invalid);
        break;
    }
    LobbyString_Destroy(&str);
}
