#ifndef POKEHEARTGOLD_FIELD_FIELD_FLASH_DARKNESS_H
#define POKEHEARTGOLD_FIELD_FIELD_FLASH_DARKNESS_H

#include "field/field_effect_manager.h"

#include "map_object.h"

FieldEffect *FlashDarkness_Show(LocalMapObject *mapObject);
void FlashDarkness_SetScale(FieldEffect *effect, float scale);

#endif // POKEHEARTGOLD_FIELD_FIELD_FLASH_DARKNESS_H
