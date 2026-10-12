#ifndef POKEHEARTGOLD_OVERLAY_2_FASHION_CASE_GIFTS_H
#define POKEHEARTGOLD_OVERLAY_2_FASHION_CASE_GIFTS_H

#include "field_system.h"

BOOL FashionCase_GiftAccessoriesFull(SaveData *saveData);
u8 FashionCase_GetRandomGiftAccessory(void);
BOOL FashionCase_HasAllBackdrops(SaveData *saveData);
u8 FashionCase_GetRandomMissingBackdrop(SaveData *saveData);

#endif // POKEHEARTGOLD_OVERLAY_2_FASHION_CASE_GIFTS_H
