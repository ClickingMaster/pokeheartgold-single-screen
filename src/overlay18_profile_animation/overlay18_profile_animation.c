#include "overlay18_profile_animation_gap2_private.h"

void PokedexApp_SetSizeCheckMonIcons(PokedexAppData *pokedexApp) {
    u8 raw = pokedexApp->seenForms[0];
    u8 form;

    if (raw & 0x80) {
        form = raw ^ 0x80;
        if (pokedexApp->curSpecies == 0xAC) {
            if (form == 2) {
                form = 1;
            } else {
                form = 0;
            }
        }
    } else {
        form = 0;
    }
    PokedexApp_SetMonIcon(pokedexApp, pokedexApp->curSpecies, form, 2);
    PokedexApp_SetMonIconSub(pokedexApp, pokedexApp->curSpecies, form, 3);
    ManagedSprite_SetAffineOverwriteMode(*(ManagedSprite **)((u8 *)pokedexApp + 0x68C), 2);
    ManagedSprite_SetAffineZRotation(*(ManagedSprite **)((u8 *)pokedexApp + 0x68C), 0);
    ManagedSprite_SetAffineTranslation(*(ManagedSprite **)((u8 *)pokedexApp + 0x68C), 0, -4);
}

void PokedexApp_DrawSizeCheckMonPic(PokedexAppData *pokedexApp) {
    u8 b, a;
    PokedexAppData_UnkSub18CC *state;
    u16 species;

    PokedexApp_GetSeenFormAndGender(pokedexApp, 0, &b, &a);
    state = &pokedexApp->unk_18CC;
    PokedexApp_LoadMonPicToBg(pokedexApp, pokedexApp->curSpecies, b, a, 2);
    species = pokedexApp->curSpecies;
    PokedexApp_SetMonPicBgAffine(pokedexApp, state->mon_ypos[species], state->mon_scale[species]);
}

void PokedexApp_ScaleSizeCheckTrainer(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub18CC *state = &pokedexApp->unk_18CC;
    float scale = (float)FX_Div(1 << 20, state->player_scale[pokedexApp->curSpecies] << 12) / 4096.0f;
    s16 x;
    s16 y;

    ManagedSprite_SetAffineOverwriteMode(*(ManagedSprite **)((u8 *)pokedexApp + 0x674), 2);
    ManagedSprite_SetAffineScale(*(ManagedSprite **)((u8 *)pokedexApp + 0x674), scale, scale);
    PokedexApp_GetSpritePos(pokedexApp, 1, &x, &y, 2);
    PokedexApp_SetSpritePos(pokedexApp, 1, x, y + state->player_ypos[pokedexApp->curSpecies], 2);
}

void ov18_021F4EB0(u16 angle, s16 *x, s16 *y) {
    s32 sinValue = FX_SinCosTable_[(angle >> 4) * 2];
    s32 cosValue;
    float rounded;

    if ((float)sinValue / 4096.0f > 0.0f) {
        rounded = ((float)sinValue / 4096.0f) * 4096.0f + 0.5f;
    } else {
        rounded = ((float)sinValue / 4096.0f) * 4096.0f - 0.5f;
    }
    sinValue = (s32)rounded;
    cosValue = FX_SinCosTable_[(angle >> 4) * 2 + 1];
    if ((float)cosValue / 4096.0f > 0.0f) {
        rounded = ((float)cosValue / 4096.0f) * 4096.0f + 0.5f;
    } else {
        rounded = ((float)cosValue / 4096.0f) * 4096.0f - 0.5f;
    }
    cosValue = (s32)rounded;
    *x += sinValue * 56 >> 12;
    *y += cosValue * 56 >> 12;
}
