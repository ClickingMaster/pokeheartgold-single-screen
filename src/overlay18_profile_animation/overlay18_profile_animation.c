#include "overlay18_profile_animation_gap2_private.h"

void ov18_021F4D64(PokedexAppData *pokedexApp) {
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
    ov18_021F14FC(pokedexApp, pokedexApp->curSpecies, form, 2);
    ov18_021F1534(pokedexApp, pokedexApp->curSpecies, form, 3);
    ManagedSprite_SetAffineOverwriteMode(*(ManagedSprite **)((u8 *)pokedexApp + 0x68C), 2);
    ManagedSprite_SetAffineZRotation(*(ManagedSprite **)((u8 *)pokedexApp + 0x68C), 0);
    ManagedSprite_SetAffineTranslation(*(ManagedSprite **)((u8 *)pokedexApp + 0x68C), 0, -4);
}

void ov18_021F4DDC(PokedexAppData *pokedexApp) {
    u8 b, a;
    PokedexAppData_UnkSub18CC *state;
    u16 species;

    ov18_021F3CA8(pokedexApp, 0, &b, &a);
    state = &pokedexApp->unk_18CC;
    ov18_021F69E8(pokedexApp, pokedexApp->curSpecies, b, a, 2);
    species = pokedexApp->curSpecies;
    ov18_021F6AB0(pokedexApp, state->mon_ypos[species], state->mon_scale[species]);
}

void ov18_021F4E28(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub18CC *state = &pokedexApp->unk_18CC;
    float scale = (float)FX_Div(1 << 20, state->player_scale[pokedexApp->curSpecies] << 12) / 4096.0f;
    s16 x;
    s16 y;

    ManagedSprite_SetAffineOverwriteMode(*(ManagedSprite **)((u8 *)pokedexApp + 0x674), 2);
    ManagedSprite_SetAffineScale(*(ManagedSprite **)((u8 *)pokedexApp + 0x674), scale, scale);
    ov18_021F12C8(pokedexApp, 1, &x, &y, 2);
    ov18_021F1294(pokedexApp, 1, x, y + state->player_ypos[pokedexApp->curSpecies], 2);
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
