#ifndef POKEHEARTGOLD_SEAL_DATA_H
#define POKEHEARTGOLD_SEAL_DATA_H

#include "global.h"

u8 Seal_GetGfxCharId(u32 idx);
u8 Seal_GetNameMsgId(u8 idx);
u8 Seal_GetBattleAnimResId(u32 idx);
BOOL Seal_IsLetterSeal(u32 idx);
u32 Seal_GetDescriptionMsgId(u32 idx);

#endif // POKEHEARTGOLD_SEAL_DATA_H
