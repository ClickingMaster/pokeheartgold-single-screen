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

void PokedexApp_UpdateSliderSprites(PokedexAppData *pokedexApp, u32 first, u32 second, int weight) {
    if (weight == FALSE) {
        PokedexApp_SetHeightSliderX(pokedexApp, PokedexApp_SliderValueToX(pokedexApp, second), 5, TRUE);
        PokedexApp_SetHeightSliderX(pokedexApp, PokedexApp_SliderValueToX(pokedexApp, first), 11, FALSE);
        PokedexApp_DrawHeightDigits(pokedexApp, 6, pokedexApp->unk_1850[second].unk_0);
        PokedexApp_DrawHeightDigits(pokedexApp, 12, pokedexApp->unk_1850[first].unk_0);
    } else {
        PokedexApp_SetWeightSliderX(pokedexApp, PokedexApp_SliderValueToX(pokedexApp, second), 5, TRUE);
        PokedexApp_SetWeightSliderX(pokedexApp, PokedexApp_SliderValueToX(pokedexApp, first), 11, FALSE);
        PokedexApp_DrawWeightDigits(pokedexApp, 6, pokedexApp->unk_1850[second].unk_2);
        PokedexApp_DrawWeightDigits(pokedexApp, 12, pokedexApp->unk_1850[first].unk_2);
    }
    PokedexApp_UpdateSliderLimitSprites(pokedexApp, second, 1);
    PokedexApp_UpdateSliderLimitSprites(pokedexApp, first, 3);
}

void PokedexApp_UpdateSearchSprites(PokedexAppData *pokedexApp, int state) {
    switch (state) {
    case 0:
        ov18_021F34EC(pokedexApp, TRUE);
        PokedexApp_SetSliderSpritesVisible(pokedexApp, FALSE);
        break;
    case 1:
        ov18_021F34EC(pokedexApp, FALSE);
        PokedexApp_SetSliderSpritesVisible(pokedexApp, FALSE);
        break;
    case 2:
        ov18_021F34C4(pokedexApp, TRUE);
        PokedexApp_SetSliderSpritesVisible(pokedexApp, TRUE);
        PokedexApp_SetSpriteAnim(pokedexApp, 5, 0x43);
        PokedexApp_SetSpriteAnim(pokedexApp, 11, 0x44);
        PokedexApp_UpdateSliderSprites(pokedexApp, pokedexApp->dexSearchCriteria[4], pokedexApp->dexSearchCriteria[5], FALSE);
        break;
    case 3:
        ov18_021F34C4(pokedexApp, TRUE);
        PokedexApp_SetSliderSpritesVisible(pokedexApp, TRUE);
        PokedexApp_SetSpriteAnim(pokedexApp, 5, 0x29);
        PokedexApp_SetSpriteAnim(pokedexApp, 11, 0x2A);
        PokedexApp_UpdateSliderSprites(pokedexApp, pokedexApp->dexSearchCriteria[6], pokedexApp->dexSearchCriteria[7], TRUE);
        break;
    default:
        ov18_021F34C4(pokedexApp, TRUE);
        PokedexApp_SetSliderSpritesVisible(pokedexApp, FALSE);
        break;
    }
}

void PokedexApp_SetHeightSliderX(PokedexAppData *pokedexApp, int xPos, int index, int moveCursor) {
    s16 x;
    s16 y;

    ManagedSprite_GetPositionXY(SPRITE_AT(pokedexApp, 0x670, index), &x, &y);
    if (xPos == 0) {
        xPos = x;
    } else if ((u32)xPos < 0x34) {
        xPos = 0x34;
    } else if ((u32)xPos > 0xCC) {
        xPos = 0xCC;
    }

    ManagedSprite_GetPositionXY(SPRITE_AT(pokedexApp, 0x670, index), &x, &y);
    ManagedSprite_SetPositionXY(SPRITE_AT(pokedexApp, 0x670, index), xPos, y);
    ManagedSprite_GetPositionXY(SPRITE_AT(pokedexApp, 0x674, index), &x, &y);
    ManagedSprite_SetPositionXY(SPRITE_AT(pokedexApp, 0x674, index), xPos - 0x14, y);
    ManagedSprite_SetPositionXY(SPRITE_AT(pokedexApp, 0x678, index), xPos - 0xC, y);
    ManagedSprite_SetPositionXY(SPRITE_AT(pokedexApp, 0x67C, index), xPos + 4, y);
    ManagedSprite_SetPositionXY(SPRITE_AT(pokedexApp, 0x680, index), xPos + 0xC, y);

    if (moveCursor == TRUE) {
        ManagedSprite_GetPositionXY(SPRITE_AT(pokedexApp, 0x670, 0), &x, &y);
        ManagedSprite_SetPositionXY(SPRITE_AT(pokedexApp, 0x670, 0), xPos, y);
    }
}

void PokedexApp_SetWeightSliderX(PokedexAppData *pokedexApp, int xPos, int index, int moveCursor) {
    s16 x;
    s16 y;

    ManagedSprite_GetPositionXY(SPRITE_AT(pokedexApp, 0x670, index), &x, &y);
    if (xPos == 0) {
        xPos = x;
    } else if ((u32)xPos < 0x34) {
        xPos = 0x34;
    } else if ((u32)xPos > 0xCC) {
        xPos = 0xCC;
    }

    ManagedSprite_GetPositionXY(SPRITE_AT(pokedexApp, 0x670, index), &x, &y);
    ManagedSprite_SetPositionXY(SPRITE_AT(pokedexApp, 0x670, index), xPos, y);
    ManagedSprite_GetPositionXY(SPRITE_AT(pokedexApp, 0x674, index), &x, &y);
    ManagedSprite_SetPositionXY(SPRITE_AT(pokedexApp, 0x674, index), xPos - 0x14, y);
    ManagedSprite_SetPositionXY(SPRITE_AT(pokedexApp, 0x678, index), xPos - 0xC, y);
    ManagedSprite_SetPositionXY(SPRITE_AT(pokedexApp, 0x67C, index), xPos - 4, y);
    ManagedSprite_SetPositionXY(SPRITE_AT(pokedexApp, 0x680, index), xPos + 4, y);
    ManagedSprite_SetPositionXY(SPRITE_AT(pokedexApp, 0x684, index), xPos + 0x14, y);

    if (moveCursor == TRUE) {
        ManagedSprite_GetPositionXY(SPRITE_AT(pokedexApp, 0x670, 0), &x, &y);
        ManagedSprite_SetPositionXY(SPRITE_AT(pokedexApp, 0x670, 0), xPos, y);
    }
}

void PokedexApp_DrawHeightDigits(PokedexAppData *pokedexApp, int index, u16 value) {
    u32 display;
    u32 integral;
    u32 fractional;

    if (value == 999) {
        display = 1188;
    } else {
        display = (((u32)10000 * value) / 254 + 5) / 10;
    }

    integral = display / 12;
    fractional = display % 12;
    if (integral >= 10) {
        PokedexApp_SetSpriteAnim(pokedexApp, index, integral / 10 + 0x2B);
        PokedexApp_SetSpriteVisible(pokedexApp, index, TRUE);
    } else {
        PokedexApp_SetSpriteVisible(pokedexApp, index, FALSE);
    }
    PokedexApp_SetSpriteAnim(pokedexApp, index + 1, integral % 10 + 0x2B);
    PokedexApp_SetSpriteVisible(pokedexApp, index + 1, TRUE);
    PokedexApp_SetSpriteAnim(pokedexApp, index + 2, fractional / 10 + 0x2B);
    PokedexApp_SetSpriteVisible(pokedexApp, index + 2, TRUE);
    PokedexApp_SetSpriteAnim(pokedexApp, index + 3, fractional % 10 + 0x2B);
    PokedexApp_SetSpriteVisible(pokedexApp, index + 3, TRUE);
    PokedexApp_SetSpriteVisible(pokedexApp, index + 4, FALSE);
}
