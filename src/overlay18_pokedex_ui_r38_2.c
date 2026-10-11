#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite_system.h"

void PokedexApp_SetSpriteAnim(PokedexAppData *pokedexApp, int index, int animation);
void PokedexApp_SetSpriteVisible(PokedexAppData *pokedexApp, int index, int draw);
void PokedexApp_DrawHeightDigits(PokedexAppData *pokedexApp, int index, u16 value);
void PokedexApp_DrawWeightDigits(PokedexAppData *pokedexApp, int index, u16 value);
void PokedexApp_UpdateSliderLimitSprites(PokedexAppData *pokedexApp, u32 value, int index);
u32 PokedexApp_GetSliderValue(PokedexAppData *pokedexApp, int index);
void PokedexApp_UpdateSliderSprites(PokedexAppData *pokedexApp, u32 first, u32 second, int weight);
void PokedexApp_UpdateSearchSprites(PokedexAppData *pokedexApp, int state);
void ov18_021F34C4(PokedexAppData *pokedexApp, int state);
void ov18_021F34EC(PokedexAppData *pokedexApp, int state);
void PokedexApp_SetSliderSpritesVisible(PokedexAppData *pokedexApp, int draw);

#define SPRITE_AT(app, offset, index) (*(ManagedSprite **)((u8 *)(app) + (offset) + (index) * 4))

void PokedexApp_UpdateSliderLimitSprites(PokedexAppData *pokedexApp, u32 value, int index) {
    if (value == 0) {
        PokedexApp_SetSpriteAnim(pokedexApp, index, 0x3A);
        PokedexApp_SetSpriteAnim(pokedexApp, index + 1, 0x35);
    } else if (value == 0x98) {
        PokedexApp_SetSpriteAnim(pokedexApp, index, 0x38);
        PokedexApp_SetSpriteAnim(pokedexApp, index + 1, 0x37);
    } else {
        PokedexApp_SetSpriteAnim(pokedexApp, index, 0x38);
        PokedexApp_SetSpriteAnim(pokedexApp, index + 1, 0x35);
    }
}

u32 PokedexApp_GetSliderValue(PokedexAppData *pokedexApp, int index) {
    s16 x;
    s16 y;
    ManagedSprite *sprite = *(ManagedSprite **)((u8 *)pokedexApp + 0x670 + index * 4);

    ManagedSprite_GetPositionXY(sprite, &x, &y);
    return x - 0x34;
}
