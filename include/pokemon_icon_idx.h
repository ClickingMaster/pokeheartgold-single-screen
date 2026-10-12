#ifndef POKEHEARTGOLD_POKEMON_ICON_IDX_H
#define POKEHEARTGOLD_POKEMON_ICON_IDX_H

#include "pokemon_types_def.h"

u32 Boxmon_GetIconNaix(BoxPokemon *boxMon);
u32 Pokemon_GetIconNaix(Pokemon *mon);
u32 GetMonIconNaixEx(u32 species, BOOL isEgg, u32 form);
u32 GetBattleMonIconNaixEx(u32 species, BOOL isEgg, u32 form);
const u8 GetMonIconPaletteEx(u32 species, u32 form, u32 isEgg);
const u8 GetBattleMonIconPaletteEx(u32 species, u32 form, BOOL isEgg);
const u8 Boxmon_GetIconPalette(BoxPokemon *boxMon);
const u8 Pokemon_GetIconPalette(Pokemon *mon);
u32 GetMonIconPlttNaix(void);
u32 GetMonIconStaticCellNaix(void);
u32 GetMonIconCell32KNaix(void);
u32 GetMonIconCell64KNaix(void);
u32 GetMonIconStaticAnimNaix(void);
u32 GetMonIconAnim32KNaix(void);
u32 GetMonIconAnim64KNaix(void);

#endif // POKEHEARTGOLD_POKEMON_ICON_IDX_H
