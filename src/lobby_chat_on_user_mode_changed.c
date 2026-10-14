#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 unk0;
    u32 state;
} Ov45State;

extern Ov45State *gLobbyChatManager;
extern char sStr_normal[];
extern char sStr_voice[];
extern char sStr_ops[];
extern char sStr_VoiceOps[];
extern char sStr_invalid[];

typedef struct {
    u32 w[3];
} String;

void LobbyString_AssignCStr(String *str, const char *s);
void LobbyString_Destroy(String *str);
void LobbyChat_OnUserModeChanged(int a, int b, int c, int mode);

void LobbyChat_OnUserModeChanged(int a, int b, int c, int mode) {
    String str;
    u32 *w;
    u32 i;
    if (gLobbyChatManager != NULL && gLobbyChatManager->state == 5) {
        return;
    }
    i = 0;
    w = str.w;
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
