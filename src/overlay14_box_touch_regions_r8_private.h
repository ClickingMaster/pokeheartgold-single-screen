#ifndef POKEHEARTGOLD_OVERLAY14_BOX_TOUCH_REGIONS_R8_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY14_BOX_TOUCH_REGIONS_R8_PRIVATE_H
#include "global.h"
extern const u8 sPCBoxSlotGridRect[];
extern const u8 sPCBoxTrayRects[];
u32 PCBox_IsPointInRect(s32, s32, const u8 *);
u32 PCBox_GetTouchedBoxSlot(s32, s32);
u32 PCBox_GetTouchedPartySlot(s32, s32, const u8 *);
u32 PCBox_GetTouchedTrayBox(s32, s32);
#endif
