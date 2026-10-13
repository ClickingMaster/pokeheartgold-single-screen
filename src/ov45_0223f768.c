#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 unk0;
    u32 state;
} Ov45State;

extern Ov45State *ov45_022577DC;
extern char ov45_022552DC[];
extern char ov45_022552E4[];
extern char ov45_022552EC[];
extern char ov45_022552F0[];
extern char ov45_022552FC[];

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
    if (ov45_022577DC != NULL && ov45_022577DC->state == 5) {
        return;
    }
    i = 0;
    w = str.w;
    for (; i < 3; i++) {
        w[i] = 0;
    }
    switch (mode) {
    case 0:
        LobbyString_AssignCStr(&str, ov45_022552DC);
        break;
    case 1:
        LobbyString_AssignCStr(&str, ov45_022552E4);
        break;
    case 2:
        LobbyString_AssignCStr(&str, ov45_022552EC);
        break;
    case 3:
        LobbyString_AssignCStr(&str, ov45_022552F0);
        break;
    default:
        LobbyString_AssignCStr(&str, ov45_022552FC);
        break;
    }
    LobbyString_Destroy(&str);
}
