#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u32 unk0; u32 state; } Ov45State;
extern Ov45State *ov45_022577DC;
extern char ov45_022552DC[];
extern char ov45_022552E4[];
extern char ov45_022552EC[];
extern char ov45_022552F0[];
extern char ov45_022552FC[];
typedef struct { u32 w[3]; } String;
void ov45_0223DAD4(String *str, const char *s);
void ov45_02237BC8(String *str);
void ov45_0223F768(int a, int b, int c, int mode);
// @fn ov45_0223F768
void ov45_0223F768(int a, int b, int c, int mode)
{
    String str;
    u32 *w;
    u32 i;
    if (ov45_022577DC != NULL && ov45_022577DC->state == 5) {
        return;
    }
    i = 0;
    w = str.w;
    for (; i < 3; i++) { w[i] = 0; }
    switch (mode) {
    case 0:
        ov45_0223DAD4(&str, ov45_022552DC);
        break;
    case 1:
        ov45_0223DAD4(&str, ov45_022552E4);
        break;
    case 2:
        ov45_0223DAD4(&str, ov45_022552EC);
        break;
    case 3:
        ov45_0223DAD4(&str, ov45_022552F0);
        break;
    default:
        ov45_0223DAD4(&str, ov45_022552FC);
        break;
    }
    ov45_02237BC8(&str);
}
