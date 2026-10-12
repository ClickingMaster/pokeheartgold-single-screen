#include "global.h"

#include "constants/sndseq.h"
#include "constants/species.h"

#include "application/pokedex/pokedex_internal.h"

#include "player_data.h"
#include "pokemon_icon_idx.h"
#include "sound_playback.h"
#include "sprite_system.h"

typedef struct PokedexSpritePos {
    s16 y;
    s16 x;
} PokedexSpritePos;

typedef struct PokedexWeightRange {
    u16 min;
    u16 max;
} PokedexWeightRange;

extern const ManagedSpriteTemplate ov18_021FA7B0[];
extern PokedexWeightRange sWeightCheckDiffRanges[];
extern BOOL (*const sWeightCheckAnimFuncs[])(PokedexAppData_UnkSub0868_State77_Sub4 *anim);

#pragma force_active on

void PokedexApp_SetupSizeCheckSprites(PokedexAppData *pokedexApp) {
    u32 i;

    PokedexApp_LoadSizeCheckResources(pokedexApp);
    for (i = 1; i < 10; i++) {
        pokedexApp->sprites[i] = PokedexApp_NewSprite(pokedexApp, &ov18_021FA7B0[i - 1]);
    }
    PokedexApp_SetSizeCheckMonIcons(pokedexApp);
    PokedexApp_DrawSizeCheckMonPic(pokedexApp);
    PokedexApp_ScaleSizeCheckTrainer(pokedexApp);
    PokedexApp_SetSpriteVisible(pokedexApp, 3, FALSE);
    PokedexApp_SetSpriteVisible(pokedexApp, 5, FALSE);
}

#pragma force_active reset

void PokedexApp_FreeSizeCheckSprites(PokedexAppData *pokedexApp) {
    u32 i;

    for (i = 1; i < 10; i++) {
        PokedexApp_DeleteSprite(pokedexApp, i);
    }
    PokedexApp_UnloadSizeCheckResources(pokedexApp);
}

#pragma force_active on

// The trainer sprites (0xC59B-0xC59D) depend on the player's gender.
void PokedexApp_LoadSizeCheckResources(PokedexAppData *pokedexApp) {
    int palette;

    PokedexApp_LoadMonIconResources(pokedexApp, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 76, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 0xC551);
    palette = sub_02074490();
    SpriteSystem_LoadPaletteBufferFromOpenNarc(pokedexApp->paletteData, PLTTBUF_SUB_OBJ, pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->iconNarc, palette, FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 0xC551);
    SpriteSystem_LoadCellResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 106, TRUE, 0xC55A);
    SpriteSystem_LoadAnimResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 107, TRUE, 0xC55A);
    SpriteSystem_LoadCellResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 112, TRUE, 0xC55B);
    SpriteSystem_LoadAnimResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 113, TRUE, 0xC55B);
    if (PlayerProfile_GetTrainerGender(pokedexApp->args->playerProfile) == PLAYER_GENDER_MALE) {
        SpriteSystem_LoadCharResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 105, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0xC59B);
        SpriteSystem_LoadCharResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 105, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 0xC59C);
        SpriteSystem_LoadCharResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 111, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 0xC59D);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ, pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 108, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0xC55D);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(pokedexApp->paletteData, PLTTBUF_SUB_OBJ, pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 108, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0xC55E);
    } else {
        SpriteSystem_LoadCharResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 109, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0xC59B);
        SpriteSystem_LoadCharResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 109, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 0xC59C);
        SpriteSystem_LoadCharResObjFromOpenNarc(pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 114, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 0xC59D);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ, pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 110, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0xC55D);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(pokedexApp->paletteData, PLTTBUF_SUB_OBJ, pokedexApp->spriteSystem, pokedexApp->spriteManager, pokedexApp->gfxNarc, 110, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 0xC55E);
    }
}

#pragma force_active reset

void PokedexApp_UnloadSizeCheckResources(PokedexAppData *pokedexApp) {
    PokedexApp_UnloadMonIconResources(pokedexApp, 1);
    SpriteManager_UnloadCharObjById(pokedexApp->spriteManager, 0xC551);
    SpriteManager_UnloadPlttObjById(pokedexApp->spriteManager, 0xC551);
    SpriteManager_UnloadCellObjById(pokedexApp->spriteManager, 0xC55A);
    SpriteManager_UnloadAnimObjById(pokedexApp->spriteManager, 0xC55A);
    SpriteManager_UnloadCellObjById(pokedexApp->spriteManager, 0xC55B);
    SpriteManager_UnloadAnimObjById(pokedexApp->spriteManager, 0xC55B);
    SpriteManager_UnloadCharObjById(pokedexApp->spriteManager, 0xC59B);
    SpriteManager_UnloadCharObjById(pokedexApp->spriteManager, 0xC59C);
    SpriteManager_UnloadCharObjById(pokedexApp->spriteManager, 0xC59D);
    SpriteManager_UnloadPlttObjById(pokedexApp->spriteManager, 0xC55D);
    SpriteManager_UnloadPlttObjById(pokedexApp->spriteManager, 0xC55E);
}

// The scale's icons; seenForms entries carry the form in the low bits and bit 7 when a form is set.
void PokedexApp_SetSizeCheckMonIcons(PokedexAppData *pokedexApp) {
    u8 raw = pokedexApp->seenForms[0];
    u8 form;

    if (raw & 0x80) {
        form = raw ^ 0x80;
        if (pokedexApp->curSpecies == SPECIES_PICHU) {
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
    ManagedSprite_SetAffineOverwriteMode(pokedexApp->sprites[7], 2);
    ManagedSprite_SetAffineZRotation(pokedexApp->sprites[7], 0);
    ManagedSprite_SetAffineTranslation(pokedexApp->sprites[7], 0, -4);
}

void PokedexApp_DrawSizeCheckMonPic(PokedexAppData *pokedexApp) {
    u8 form;
    u8 gender;
    PokedexAppData_UnkSub18CC *measures;
    u16 species;

    PokedexApp_GetSeenFormAndGender(pokedexApp, 0, &form, &gender);
    measures = &pokedexApp->sizeCheckData;
    PokedexApp_LoadMonPicToBg(pokedexApp, pokedexApp->curSpecies, form, gender, 2);
    species = pokedexApp->curSpecies;
    PokedexApp_SetMonPicBgAffine(pokedexApp, measures->mon_ypos[species], measures->mon_scale[species]);
}

void PokedexApp_ScaleSizeCheckTrainer(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub18CC *measures = &pokedexApp->sizeCheckData;
    float scale = (float)FX_Div(1 << 20, measures->player_scale[pokedexApp->curSpecies] << 12) / 4096.0f;
    s16 x;
    s16 y;

    ManagedSprite_SetAffineOverwriteMode(pokedexApp->sprites[1], 2);
    ManagedSprite_SetAffineScale(pokedexApp->sprites[1], scale, scale);
    PokedexApp_GetSpritePos(pokedexApp, 1, &x, &y, 2);
    PokedexApp_SetSpritePos(pokedexApp, 1, x, y + measures->player_ypos[pokedexApp->curSpecies], 2);
}

// Moves (x, y) 56 pixels along the angle (sine and cosine rounded to whole fx32 units first).
void PokedexApp_RotatePoint(u16 angle, s16 *x, s16 *y) {
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

void PokedexApp_SetPanSpritePos(PokedexAppData *pokedexApp, int index, u16 angle, s16 x, s16 y) {
    PokedexApp_RotatePoint(angle, &x, &y);
    PokedexApp_SetSpritePos(pokedexApp, index, x, y, 1);
}

#pragma force_active on

// Puts sprite index 16 pixels above sprite from.
void PokedexApp_PlaceFigureOnPan(PokedexAppData *pokedexApp, int index, int from) {
    s16 x, y;

    PokedexApp_GetSpritePos(pokedexApp, from, &x, &y, 1);
    y -= 16;
    PokedexApp_SetSpritePos(pokedexApp, index, x, y, 1);
}

#pragma force_active reset

// Swings the two pans (sprites 8 and 9, with the trainer and the Pokemon icon on top) around the scale's
// pivot for the beam angle.
void PokedexApp_UpdateScalePans(PokedexAppData *pokedexApp, u32 angle) {
    PokedexApp_SetPanSpritePos(pokedexApp, 8, (u16)(-0x4000 - angle), 0x80, 0x68);
    PokedexApp_SetPanSpritePos(pokedexApp, 9, (u16)(0x4000 - angle), 0x80, 0x68);
    PokedexApp_PlaceFigureOnPan(pokedexApp, 2, 8);
    PokedexApp_PlaceFigureOnPan(pokedexApp, 4, 9);
}

#pragma force_active on

// Picks the animation from the weight difference between the Pokemon and the player (40.0 or 41.0 kg).
void PokedexApp_InitWeightCheckAnim(PokedexAppData *pokedexApp, PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    u32 heavier, weight, lighter, diff;
    u8 monLighter;
    u32 i;

    anim->pokedexApp = pokedexApp;
    anim->dropOffset = 0;
    anim->step = 0;
    if (PlayerProfile_GetTrainerGender(pokedexApp->args->playerProfile) == PLAYER_GENDER_MALE) {
        heavier = 400;
    } else {
        heavier = 410;
    }
    weight = ((u32 *)pokedexApp->weights)[pokedexApp->curSpecies];
    if (weight >= heavier) {
        lighter = heavier;
        heavier = weight;
        monLighter = FALSE;
    } else {
        lighter = weight;
        monLighter = TRUE;
    }
    anim->monLighter = monLighter;
    diff = (heavier - lighter) / 10;
    for (i = 0; i < 17; i++) {
        if (diff >= sWeightCheckDiffRanges[i].min && diff <= sWeightCheckDiffRanges[i].max) {
            anim->animIndex = i;
            return;
        }
    }
}

// Lowers the trainer and the Pokemon icon onto the pans; FALSE once they rest at y 88.
BOOL PokedexApp_DropFiguresOntoPans(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    PokedexSpritePos trainer, mon;

    PokedexApp_GetSpritePos(anim->pokedexApp, 2, &trainer.x, &trainer.y, 1);
    PokedexApp_GetSpritePos(anim->pokedexApp, 4, &mon.x, &mon.y, 1);
    anim->dropOffset += 0x400;
    trainer.y += anim->dropOffset >> 12;
    mon.y += anim->dropOffset >> 12;
    if (trainer.y >= 88) {
        PokedexApp_SetSpritePos(anim->pokedexApp, 2, trainer.x, 88, 1);
        PokedexApp_SetSpritePos(anim->pokedexApp, 4, mon.x, 88, 1);
        return FALSE;
    }
    PokedexApp_SetSpritePos(anim->pokedexApp, 2, trainer.x, trainer.y, 1);
    PokedexApp_SetSpritePos(anim->pokedexApp, 4, mon.x, mon.y, 1);
    return TRUE;
}

#pragma force_active reset

BOOL PokedexApp_RunWeightCheckAnim(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    return sWeightCheckAnimFuncs[anim->animIndex](anim);
}

// Rotation speed and acceleration, signed by which side is heavier.
void PokedexApp_SetBeamRotation(PokedexAppData_UnkSub0868_State77_Sub4 *anim, s32 speed, s32 accel) {
    if (anim->monLighter == FALSE) {
        anim->rotationSpeed = -speed;
        anim->rotationAccel = accel;
    } else {
        anim->rotationSpeed = speed;
        anim->rotationAccel = -accel;
    }
}

// Rotation speed, acceleration and the speed at which the swing stops, signed by which side is heavier.
void PokedexApp_SetBeamSwing(PokedexAppData_UnkSub0868_State77_Sub4 *anim, s32 speed, s32 accel, s32 endSpeed) {
    if (anim->monLighter == FALSE) {
        anim->rotationSpeed = -speed;
        anim->rotationAccel = -accel;
        anim->rotationEndSpeed = -endSpeed;
    } else {
        anim->rotationSpeed = speed;
        anim->rotationAccel = accel;
        anim->rotationEndSpeed = endSpeed;
    }
}

// Same weight: no swing.
BOOL PokedexApp_WeightAnim_Even(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    PlaySE(SEQ_SE_GS_ZUKAN01);
    return FALSE;
}

// Small differences: the beam swings and settles level again.
BOOL PokedexApp_WeightAnim_Tip1(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN01);
        PokedexApp_SetBeamSwing(anim, 0x100, -32, -224);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        PokedexApp_UpdateScalePans(anim->pokedexApp, ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]));
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->rotationSpeed == anim->rotationEndSpeed) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL PokedexApp_WeightAnim_Tip2(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN01);
        PokedexApp_SetBeamSwing(anim, 0x120, -32, -256);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        PokedexApp_UpdateScalePans(anim->pokedexApp, ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]));
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->rotationSpeed == anim->rotationEndSpeed) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL PokedexApp_WeightAnim_Tip3(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN01);
        PokedexApp_SetBeamSwing(anim, 0x140, -32, -288);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        PokedexApp_UpdateScalePans(anim->pokedexApp, ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]));
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->rotationSpeed == anim->rotationEndSpeed) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL PokedexApp_WeightAnim_Tip4(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN01);
        PokedexApp_SetBeamSwing(anim, 0x160, -32, -320);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        PokedexApp_UpdateScalePans(anim->pokedexApp, ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]));
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->rotationSpeed == anim->rotationEndSpeed) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL PokedexApp_WeightAnim_Tip5(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN01);
        PokedexApp_SetBeamSwing(anim, 0x180, -32, -352);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        PokedexApp_UpdateScalePans(anim->pokedexApp, ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]));
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->rotationSpeed == anim->rotationEndSpeed) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL PokedexApp_WeightAnim_Tip6(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN01);
        PokedexApp_SetBeamSwing(anim, 0x1C0, -64, -320);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        PokedexApp_UpdateScalePans(anim->pokedexApp, ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]));
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->rotationSpeed == anim->rotationEndSpeed) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL PokedexApp_WeightAnim_Tip7(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN01);
        PokedexApp_SetBeamSwing(anim, 0x200, -64, -384);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        PokedexApp_UpdateScalePans(anim->pokedexApp, ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]));
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->rotationSpeed == anim->rotationEndSpeed) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

// Starts dropping the lighter side's figure (sprite 2 trainer / 4 Pokemon) towards its pan, with its
// lower-screen copy (3 / 5) shown 192 pixels further down.
void PokedexApp_StartFigureThrow(PokedexAppData_UnkSub0868_State77_Sub4 *anim, s16 speed, s16 accel) {
    PokedexSpritePos trainer;
    PokedexSpritePos mon;

    anim->dropSpeed = speed;
    anim->dropAccel = accel;
    PokedexApp_GetSpritePos(anim->pokedexApp, 2, &trainer.x, &trainer.y, 1);
    PokedexApp_GetSpritePos(anim->pokedexApp, 4, &mon.x, &mon.y, 1);
    if (trainer.y < mon.y) {
        anim->dropSprite = 2;
        anim->dropTargetSprite = 8;
        PokedexApp_SetSpriteVisible(anim->pokedexApp, 3, TRUE);
        PokedexApp_SetSpritePos(anim->pokedexApp, 3, trainer.x, trainer.y + 192, 2);
    } else {
        anim->dropSprite = 4;
        anim->dropTargetSprite = 9;
        PokedexApp_SetSpriteVisible(anim->pokedexApp, 5, TRUE);
        PokedexApp_SetSpritePos(anim->pokedexApp, 5, mon.x, mon.y + 192, 2);
    }
}

void PokedexApp_MoveThrownFigure(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    PokedexSpritePos pos;

    PokedexApp_GetSpritePos(anim->pokedexApp, anim->dropSprite, &pos.x, &pos.y, 1);
    pos.y = pos.y + anim->dropSpeed;
    PokedexApp_SetSpritePos(anim->pokedexApp, anim->dropSprite, pos.x, pos.y, 1);
    PokedexApp_GetSpritePos(anim->pokedexApp, anim->dropSprite + 1, &pos.x, &pos.y, 2);
    pos.y = pos.y + anim->dropSpeed;
    PokedexApp_SetSpritePos(anim->pokedexApp, anim->dropSprite + 1, pos.x, pos.y, 2);
    anim->dropSpeed = anim->dropSpeed + anim->dropAccel;
}

// FALSE once the dropping figure is back on its pan.
BOOL PokedexApp_UpdateThrownFigure(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    PokedexSpritePos figure;
    PokedexSpritePos pan;

    PokedexApp_MoveThrownFigure(anim);
    PokedexApp_GetSpritePos(anim->pokedexApp, anim->dropSprite, &figure.x, &figure.y, 1);
    PokedexApp_GetSpritePos(anim->pokedexApp, anim->dropTargetSprite, &pan.x, &pan.y, 1);
    if (figure.y >= pan.y - 16) {
        PokedexApp_SetSpritePos(anim->pokedexApp, anim->dropSprite, pan.x, pan.y - 16, 1);
        return FALSE;
    }
    return TRUE;
}

// Larger differences: the beam tips to its stop (0xF600 / 0xA00); from here on the lighter figure is
// thrown up and falls back.
BOOL PokedexApp_WeightAnim_TipToStop(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    u32 rotation;

    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN01);
        PokedexApp_SetBeamRotation(anim, 0x300, 0);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        rotation = ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]);
        PokedexApp_UpdateScalePans(anim->pokedexApp, rotation);
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->monLighter == FALSE) {
            if (rotation > 0xF600) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xF600);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xF600);
            return FALSE;
        } else {
            if (rotation < 0xA00) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xA00);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xA00);
            return FALSE;
        }
    }
    return TRUE;
}

BOOL PokedexApp_WeightAnim_Throw1(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    u32 rotation;

    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN02);
        PokedexApp_SetBeamRotation(anim, 0x300, 0);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        rotation = ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]);
        PokedexApp_UpdateScalePans(anim->pokedexApp, rotation);
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->monLighter == FALSE) {
            if (rotation > 0xF600) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xF600);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xF600);
            anim->step++;
        } else {
            if (rotation < 0xA00) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xA00);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xA00);
            anim->step++;
        }
    case 2:
        PokedexApp_StartFigureThrow(anim, -4, 1);
        anim->step++;
    case 3:
        if (PokedexApp_UpdateThrownFigure(anim) == FALSE) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL PokedexApp_WeightAnim_Throw2(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    u32 rotation;

    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN02);
        PokedexApp_SetBeamRotation(anim, 0x400, 0);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        rotation = ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]);
        PokedexApp_UpdateScalePans(anim->pokedexApp, rotation);
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->monLighter == FALSE) {
            if (rotation > 0xF600) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xF600);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xF600);
            anim->step++;
        } else {
            if (rotation < 0xA00) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xA00);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xA00);
            anim->step++;
        }
    case 2:
        PokedexApp_StartFigureThrow(anim, -8, 1);
        anim->step++;
    case 3:
        if (PokedexApp_UpdateThrownFigure(anim) == FALSE) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL PokedexApp_WeightAnim_Throw3(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    u32 rotation;

    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN02);
        PokedexApp_SetBeamRotation(anim, 0x400, 0);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        rotation = ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]);
        PokedexApp_UpdateScalePans(anim->pokedexApp, rotation);
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->monLighter == FALSE) {
            if (rotation > 0xF600) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xF600);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xF600);
            anim->step++;
        } else {
            if (rotation < 0xA00) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xA00);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xA00);
            anim->step++;
        }
    case 2:
        PokedexApp_StartFigureThrow(anim, -12, 1);
        anim->step++;
    case 3:
        if (PokedexApp_UpdateThrownFigure(anim) == FALSE) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL PokedexApp_WeightAnim_Throw4(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    u32 rotation;

    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN03);
        PokedexApp_SetBeamRotation(anim, 0x500, 0);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        rotation = ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]);
        PokedexApp_UpdateScalePans(anim->pokedexApp, rotation);
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->monLighter == FALSE) {
            if (rotation > 0xF600) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xF600);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xF600);
            anim->step++;
        } else {
            if (rotation < 0xA00) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xA00);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xA00);
            anim->step++;
        }
    case 2:
        PokedexApp_StartFigureThrow(anim, -14, 1);
        anim->step++;
    case 3:
        if (PokedexApp_UpdateThrownFigure(anim) == FALSE) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL PokedexApp_WeightAnim_Throw5(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    u32 rotation;

    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN03);
        PokedexApp_SetBeamRotation(anim, 0x500, 0);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        rotation = ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]);
        PokedexApp_UpdateScalePans(anim->pokedexApp, rotation);
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->monLighter == FALSE) {
            if (rotation > 0xF600) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xF600);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xF600);
            anim->step++;
        } else {
            if (rotation < 0xA00) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xA00);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xA00);
            anim->step++;
        }
    case 2:
        PokedexApp_StartFigureThrow(anim, -18, 1);
        anim->step++;
    case 3:
        if (PokedexApp_UpdateThrownFigure(anim) == FALSE) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL PokedexApp_WeightAnim_Throw6(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    u32 rotation;

    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN03);
        PokedexApp_SetBeamRotation(anim, 0x600, 0);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        rotation = ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]);
        PokedexApp_UpdateScalePans(anim->pokedexApp, rotation);
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->monLighter == FALSE) {
            if (rotation > 0xF600) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xF600);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xF600);
            anim->step++;
        } else {
            if (rotation < 0xA00) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xA00);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xA00);
            anim->step++;
        }
    case 2:
        PokedexApp_StartFigureThrow(anim, -21, 1);
        anim->step++;
    case 3:
        if (PokedexApp_UpdateThrownFigure(anim) == FALSE) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL PokedexApp_WeightAnim_Throw7(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    u32 rotation;

    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN03);
        PokedexApp_SetBeamRotation(anim, 0x700, 0);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        rotation = ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]);
        PokedexApp_UpdateScalePans(anim->pokedexApp, rotation);
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->monLighter == FALSE) {
            if (rotation > 0xF600) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xF600);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xF600);
            anim->step++;
        } else {
            if (rotation < 0xA00) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xA00);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xA00);
            anim->step++;
        }
    case 2:
        PokedexApp_StartFigureThrow(anim, -24, 1);
        anim->step++;
    case 3:
        if (PokedexApp_UpdateThrownFigure(anim) == FALSE) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

// The heaviest class throws the figure off the top of the screen.
BOOL PokedexApp_WeightAnim_Throw8(PokedexAppData_UnkSub0868_State77_Sub4 *anim) {
    u32 rotation;
    PokedexSpritePos pos;

    switch (anim->step) {
    case 0:
        PlaySE(SEQ_SE_GS_ZUKAN04);
        PokedexApp_SetBeamRotation(anim, 0x800, 0);
        anim->step++;
    case 1:
        ManagedSprite_OffsetAffineZRotation(anim->pokedexApp->sprites[7], anim->rotationSpeed);
        rotation = ManagedSprite_GetRotation(anim->pokedexApp->sprites[7]);
        PokedexApp_UpdateScalePans(anim->pokedexApp, rotation);
        anim->rotationSpeed += anim->rotationAccel;
        if (anim->monLighter == FALSE) {
            if (rotation > 0xF600) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xF600);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xF600);
            anim->step++;
        } else {
            if (rotation < 0xA00) {
                break;
            }
            ManagedSprite_SetAffineZRotation(anim->pokedexApp->sprites[7], 0xA00);
            PokedexApp_UpdateScalePans(anim->pokedexApp, 0xA00);
            anim->step++;
        }
    case 2:
        PokedexApp_StartFigureThrow(anim, -24, 0);
        anim->step++;
    case 3:
        if (PokedexApp_UpdateThrownFigure(anim) == FALSE) {
            return FALSE;
        }
        PokedexApp_GetSpritePos(anim->pokedexApp, anim->dropSprite, &pos.x, &pos.y, 1);
        if (pos.y <= -256) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}
