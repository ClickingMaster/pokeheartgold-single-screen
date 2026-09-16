#ifndef POKEHEARTGOLD_SLOP_R1_OVERLAY83_M10_PRIVATE_H
#define POKEHEARTGOLD_SLOP_R1_OVERLAY83_M10_PRIVATE_H

#include "heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "system.h"
#include "touchscreen.h"
#include "touchscreen_list_menu.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

extern const TouchscreenHitbox ov83_022484F4[];
extern const GridCallbacks ov83_02248500;
extern const GridCallbacks ov83_02248510;
extern const TouchscreenHitbox ov83_02248530[];
extern const int ov83_02248544[];
extern const TouchscreenHitbox ov83_02248558[];
extern const int ov83_0224858C[];
extern const DpadMenuBox ov83_022485A8[];
extern const DpadMenuBox ov83_022485E8[];

void ov83_02247998(void *param0, String *str, int param2, int param3, int param4, int param5, int param6);
void ov83_022479E4(void *param0, MsgData *msgData, s32 msgNo, int param3, int param4, int param5, int param6, int param7);
GridInputHandler *ov83_02247A24(void *param0, int param1, int param2);
GridInputHandler *ov83_02247A7C(void *param0, int param1, int param2);
int ov83_02247AD4(GridInputHandler *inputHandler);
int ov83_02247BC4(GridInputHandler *inputHandler);
TouchscreenListMenu *ov83_02247CCC(TouchscreenListMenuSpawner *spawner, TouchscreenListMenuHeader *header, u8 x, u8 y, u8 width);

#endif
