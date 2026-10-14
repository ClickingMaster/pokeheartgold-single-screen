#include "pokeathlon/pokeathlon_circle_push_display.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg/msg_0309.h"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "math_util.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "text.h"
#include "char_tiles.h"

// Sprite scene of the event and the course helpers (OVY_96, not yet tidied).
void PokeathlonCourse_GetMonInfo(PokeathlonCourseData *course, int participant, int member, PokeathlonCirclePushMonView *view);
void PokeathlonCourseSpritePool_CreateSprite(void *scene, int priority, int whichScreen, int resId, int kind);
void *PokeathlonCourseSpritePool_Take(void *scene, int resId, int kind);
void PokeathlonCourseSpritePoolSprite_Show(void *sprite, int shown, int draw);
Sprite *PokeathlonCourseSpritePoolSprite_GetSprite(void *sprite);
SpriteResource **PokeathlonCourseSpritePool_GetResourceSlot(void *scene, int id, int type);

// The resource ids of the two picture sprites (102, 103) lie 0x20 bytes after the coordinates in
// the read-only data of their defining file.
extern PokeathlonCirclePushNameplateCoords gPokeathlonCirclePushNameplateCoords;
extern const WindowTemplate ov96_0221C028[5];
extern const PokeathlonCirclePushPicKeys ov96_0221C01C;
extern const VecFx32 ov96_0221C050[4];
extern const VecFx32 ov96_0221C080[4];
extern const u32 ov96_0221DC04[3];

#pragma force_active on

void PokeathlonCirclePushDisplay_CreateSprites(void *scene, PokeathlonCirclePushDisplay *display) {
    VecFx32 pos = { 0, 0, 0 };
    u8 x[4] = { gPokeathlonCirclePushNameplateCoords.x[0], gPokeathlonCirclePushNameplateCoords.x[1], gPokeathlonCirclePushNameplateCoords.x[2], gPokeathlonCirclePushNameplateCoords.x[3] };
    u8 y[4] = { gPokeathlonCirclePushNameplateCoords.y[0], gPokeathlonCirclePushNameplateCoords.y[1], gPokeathlonCirclePushNameplateCoords.y[2], gPokeathlonCirclePushNameplateCoords.y[3] };
    u8 i;
    Sprite *sprite;
    Sprite *scoreboardSprite;
    Sprite *nameplateSprite;
    Sprite *eventSprite;
    PokeathlonCirclePushDisplayAnim *anim = &display->anim;

    PokeathlonCourseSpritePool_CreateSprite(scene, 0, 2, 105, 8);
    PokeathlonCourseSpritePool_CreateSprite(scene, 0, 2, 105, 9);
    PokeathlonCourseSpritePool_CreateSprite(scene, 1, 2, 105, 10);
    for (i = 0; i < 2; i++) {
        PokeathlonCourseSpritePool_CreateSprite(scene, 0, 2, 105, 11);
    }
    for (i = 0; i < 4; i++) {
        PokeathlonCourseSpritePool_CreateSprite(scene, 1, 2, 105, 12);
    }
    for (i = 0; i < 4; i++) {
        PokeathlonCourseSpritePool_CreateSprite(scene, 1, 2, 105, 13);
    }

    anim->unkB4 = PokeathlonCourseSpritePool_Take(scene, 105, 8);
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(anim->unkB4);
    Sprite_SetDrawPriority(sprite, 4);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    pos.x = FX32_CONST(128);
    pos.y = FX32_CONST(544);
    Sprite_SetMatrix(sprite, &pos);
    PokeathlonCourseSpritePoolSprite_Show(anim->unkB4, 1, 1);

    anim->roundSprite = PokeathlonCourseSpritePool_Take(scene, 105, 9);
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(anim->roundSprite);
    Sprite_SetDrawPriority(sprite, 3);
    Sprite_SetAnimCtrlSeq(sprite, 1);
    pos.x = FX32_CONST(152);
    pos.y = FX32_CONST(544);
    Sprite_SetMatrix(sprite, &pos);
    PokeathlonCourseSpritePoolSprite_Show(anim->roundSprite, 1, 1);

    anim->zoomSprite = PokeathlonCourseSpritePool_Take(scene, 105, 10);
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(anim->zoomSprite);
    Sprite_SetDrawPriority(sprite, 5);
    Sprite_SetAnimCtrlSeq(sprite, 13);
    pos.x = 0;
    pos.y = FX32_CONST(512);
    Sprite_SetMatrix(sprite, &pos);
    PokeathlonCourseSpritePoolSprite_Show(anim->zoomSprite, 1, 0);

    for (i = 0; i < 2; i++) {
        anim->scoreboardSprites[i] = PokeathlonCourseSpritePool_Take(scene, 105, 11);
        scoreboardSprite = PokeathlonCourseSpritePoolSprite_GetSprite(anim->scoreboardSprites[i]);
        Sprite_SetDrawPriority(scoreboardSprite, 6);
        Sprite_SetAnimCtrlSeq(scoreboardSprite, i + 11);
        pos.x = (160 * i + 48) << FX32_SHIFT;
        pos.y = FX32_CONST(536);
        Sprite_SetMatrix(scoreboardSprite, &pos);
        PokeathlonCourseSpritePoolSprite_Show(anim->scoreboardSprites[i], 1, 1);
    }
    for (i = 0; i < 4; i++) {
        anim->nameplateSprites[i] = PokeathlonCourseSpritePool_Take(scene, 105, 12);
        nameplateSprite = PokeathlonCourseSpritePoolSprite_GetSprite(anim->nameplateSprites[i]);
        Sprite_SetDrawPriority(nameplateSprite, 8);
        Sprite_SetAnimCtrlSeq(nameplateSprite, 14);
        pos.x = x[i] << FX32_SHIFT;
        pos.y = (y[i] << FX32_SHIFT) + FX32_CONST(512);
        Sprite_SetMatrix(nameplateSprite, &pos);
        PokeathlonCourseSpritePoolSprite_Show(anim->nameplateSprites[i], 1, 1);
    }
    for (i = 0; i < 4; i++) {
        anim->playerEventSprites[i] = PokeathlonCourseSpritePool_Take(scene, 105, 13);
        eventSprite = PokeathlonCourseSpritePoolSprite_GetSprite(anim->playerEventSprites[i]);
        Sprite_SetDrawPriority(eventSprite, 7);
        Sprite_SetAnimCtrlSeq(eventSprite, 18);
        pos.x = x[i] << FX32_SHIFT;
        pos.y = (y[i] << FX32_SHIFT) + FX32_CONST(512);
        Sprite_SetMatrix(eventSprite, &pos);
        PokeathlonCourseSpritePoolSprite_Show(anim->playerEventSprites[i], 1, 0);
    }
}

#pragma force_active reset

void PokeathlonCirclePushDisplay_PrintPlayerNames(PokeathlonCirclePushDisplay *display) {
    u8 i;
    u32 self;
    String *name;
    u8 window;

    for (i = 0; i < 5; i++) {
        FillWindowPixelBuffer(&display->windows[i], 0);
    }

    self = (u8)PokeathlonCourse_GetNetId(display->course);
    name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(display->course, self), display->heapId);
    AddTextPrinterParameterizedWithColor(&display->windows[0], 0, name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(name);
    Sprite_SetAnimCtrlSeq(PokeathlonCourseSpritePoolSprite_GetSprite(display->anim.nameplateSprites[0]), self + 14);
    display->windowOfParticipant[self] = 0;

    window = 1;
    for (i = 0; i < 4; i++) {
        if (i != self) {
            name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(display->course, i), display->heapId);
            AddTextPrinterParameterizedWithColor(&display->windows[window], 0, name, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
            String_Delete(name);
            Sprite_SetAnimCtrlSeq(PokeathlonCourseSpritePoolSprite_GetSprite(display->anim.nameplateSprites[window]), i + 14);
            display->windowOfParticipant[i] = window;
            window++;
        }
    }
    PokeathlonCirclePushDisplay_PrintRound(display, 1);
    for (i = 0; i < 5; i++) {
        CopyWindowToVram(&display->windows[i]);
    }
}

void PokeathlonCirclePushDisplay_SetRound(PokeathlonCirclePushDisplay *display, u32 round) {
    if (display->round != round) {
        display->round = round;
        PokeathlonCirclePushDisplay_PrintRound(display, round);
        Sprite_SetAnimCtrlSeq(PokeathlonCourseSpritePoolSprite_GetSprite(display->anim.roundSprite), round);
        CopyWindowToVram(&display->windows[4]);
    }
}

void PokeathlonCirclePushDisplay_ShowPlayerEvent(PokeathlonCirclePushDisplay *display, u32 participant, u32 kind) {
    u32 seq;
    void **sprites;
    u32 window;

    switch (kind) {
    case 1:
        seq = 18;
        break;
    case 2:
        seq = 19;
        break;
    case 3:
        seq = 20;
        break;
    case 4:
        seq = 21;
        break;
    default:
        GF_AssertFail();
        seq = 18;
        break;
    }
    sprites = display->anim.playerEventSprites;
    window = display->windowOfParticipant[participant];
    PokeathlonCourseSpritePoolSprite_Show(sprites[window], 1, 1);
    Sprite_SetAnimCtrlSeq(PokeathlonCourseSpritePoolSprite_GetSprite(sprites[window]), seq);
}

void PokeathlonCirclePushDisplay_ResetAnim(PokeathlonCirclePushDisplay *display) {
    u8 i;

    for (i = 0; i < 12; i++) {
        display->anim.monShown[i] = FALSE;
    }
    display->anim.scoreboardShown = FALSE;
    PokeathlonCirclePushDisplayAnim_Stop(&display->anim);
}

void PokeathlonCirclePushDisplay_GetPicVramLocations(void *scene, PokeathlonCirclePushDisplay *display) {
    u32 ids[2] = { ((const u32 *)&gPokeathlonCirclePushNameplateCoords)[8], ((const u32 *)&gPokeathlonCirclePushNameplateCoords)[9] };
    u8 i;

    for (i = 0; i < 2; i++) {
        NNSG2dImagePaletteProxy *volatile paletteProxy;
        SpriteResource *volatile charResource;
        SpriteResource *volatile paletteResource;
        NNSG2dImageProxy *imageProxy;
        u32 id = ids[i];

        charResource = *PokeathlonCourseSpritePool_GetResourceSlot(scene, id, 0);
        paletteResource = *PokeathlonCourseSpritePool_GetResourceSlot(scene, id, 1);
        imageProxy = SpriteTransfer_GetCharProxy(charResource);
        paletteProxy = SpriteTransfer_GetPaletteProxy(paletteResource, imageProxy);
        display->anim.picImageLocations[i] = NNS_G2dGetImageLocation(imageProxy, NNS_G2D_VRAM_TYPE_2DSUB);
        display->anim.picPaletteLocations[i] = NNS_G2dGetImagePaletteLocation(paletteProxy, NNS_G2D_VRAM_TYPE_2DSUB);
    }
}

void PokeathlonCirclePushDisplay_LoadMonPics(PokeathlonCirclePushDisplay *display) {
    int i;
    PokepicTemplate pokepic;
    PokeathlonCirclePushMonView view;

    for (i = 0; i < 12; i++) {
        PokeathlonCourse_GetMonInfo(display->course, i / 3, i % 3, &view);
        GetMonSpriteCharAndPlttNarcIdsEx(&pokepic, view.species, view.gender, 2, view.shiny, (u8)view.form, view.personality);
        display->anim.monPics[i].charData = CharTiles_AllocAndLoadMonPicAsObjFromNarc((NarcId)pokepic.narcID, pokepic.charDataID, display->heapId, view.personality, FALSE, 2, view.species);
        display->anim.monPics[i].paletteData = AllocAndLoadPltt16FromNarc((NarcId)pokepic.narcID, pokepic.palDataID, display->heapId);
    }
}

void PokeathlonCirclePushDisplayAnim_LoadMonPic(PokeathlonCirclePushDisplayAnim *anim, u32 slot, u32 mon) {
    GF_ASSERT(slot < 2);
    GF_ASSERT(mon < 12);
    DC_FlushRange(anim->monPics[mon].charData, 0xC80);
    GXS_LoadOBJ(anim->monPics[mon].charData, anim->picImageLocations[slot], 0xC80);
    DC_FlushRange(anim->monPics[mon].paletteData, 0x20);
    GXS_LoadOBJPltt(anim->monPics[mon].paletteData, anim->picPaletteLocations[slot], 0x20);
}

void PokeathlonCirclePushDisplay_CreateWindows(PokeathlonCirclePushDisplay *display) {
    u8 i;

    for (i = 0; i < 5; i++) {
        AddWindow(display->bgConfig, &display->windows[i], &ov96_0221C028[i]);
    }
    BG_FillCharDataRange(display->bgConfig, GF_BG_LYR_SUB_0, 0, 1, 0);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, display->heapId);
}

void PokeathlonCirclePushDisplay_PrintRound(PokeathlonCirclePushDisplay *display, u32 round) {
    String *string;

    FillWindowPixelBuffer(&display->windows[4], 0);
    BufferIntegerAsString(display->msgFormat, 0, round, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(display->msgFormat, display->msgData, msg_0309_00152, display->heapId);
    AddTextPrinterParameterizedWithColor(&display->windows[4], 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
}

void PokeathlonCirclePushDisplay_Update(PokeathlonCirclePushDisplay *display, BOOL showScoreboard, const u8 *monScored) {
    PokeathlonCirclePushDisplayAnim *anim = &display->anim;
    u32 mon = (u8)PokeathlonCirclePushDisplayAnim_TakeNewMon(anim, monScored);

    if (!showScoreboard && anim->scoreboardShown) {
        PokeathlonCirclePushDisplay_ResetAnim(display);
    }
    if (anim->active) {
        if (showScoreboard) {
            if (!anim->scoreboardShown) {
                anim->scoreboardShown = TRUE;
                if (PokeathlonCirclePushDisplayAnim_TryStart(anim, CIRCLE_PUSH_DISPLAY_SCOREBOARD) != TRUE) {
                    GF_AssertFail();
                }
                PokeathlonCirclePushDisplayAnim_StartScoreboard(anim, display->bgConfig);
            }
        } else if (mon != 12 && PokeathlonCirclePushDisplayAnim_TryStart(anim, CIRCLE_PUSH_DISPLAY_MON_PIC) != FALSE) {
            PlaySE(SEQ_SE_GS_PUSH02);
            PokeathlonCirclePushDisplayAnim_StartMonPic(anim, display->bgConfig, mon);
        }
    } else {
        PokeathlonCirclePushDisplayAnim_StartIdle(anim, display->bgConfig);
    }
    if (anim->update != NULL) {
        anim->update(display);
    } else {
        GF_AssertFail();
    }
}

void PokeathlonCirclePushDisplay_SetScore(PokeathlonCirclePushDisplay *display, u32 score, u32 participant) {
    u32 self = PokeathlonCourse_GetNetId(display->course);

    if (participant == self && display->anim.score != score) {
        GF_ASSERT(display->anim.score < score);
        display->anim.score = score;
    }
}

u8 PokeathlonCirclePushDisplay_GetScore(PokeathlonCirclePushDisplay *display) {
    return display->anim.score;
}

int PokeathlonCirclePushDisplayAnim_TakeNewMon(PokeathlonCirclePushDisplayAnim *anim, const u8 *monScored) {
    u8 isNew[12];
    int i;

    for (i = 0; i < 12; i++) {
        if (monScored[i] != FALSE && anim->monShown[i] == FALSE) {
            anim->monShown[i] = TRUE;
            isNew[i] = TRUE;
        } else {
            isNew[i] = FALSE;
        }
    }
    for (i = 0; i < 12; i++) {
        if (isNew[i] != FALSE) {
            return i;
        }
    }
    return 12;
}

BOOL PokeathlonCirclePushDisplayAnim_CanStart(PokeathlonCirclePushDisplayAnim *anim, int phase) {
    if (anim->phase == phase) {
        return FALSE;
    }
    if (phase == CIRCLE_PUSH_DISPLAY_SCOREBOARD) {
        return TRUE;
    }
    if (phase == CIRCLE_PUSH_DISPLAY_MON_PIC && anim->phase != CIRCLE_PUSH_DISPLAY_SCOREBOARD) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

void PokeathlonCirclePushDisplayAnim_Stop(PokeathlonCirclePushDisplayAnim *anim) {
    int i;

    anim->active = FALSE;
    anim->frame = 0;
    for (i = 0; i < 2; i++) {
        PokeathlonCourseSpritePoolSprite_Show(anim->picSprites[i], 1, 0);
    }
    G2S_BlendNone();
    PokeathlonCirclePushDisplayAnim_HideSprites(anim);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    anim->update = NULL;
    anim->scroll = 0;
    G2S_SetBG2Offset(0, anim->scroll);
}

#pragma force_active reset

void PokeathlonCirclePushDisplayAnim_HideSprites(PokeathlonCirclePushDisplayAnim *anim) {
    u8 i;

    for (i = 0; i < 2; i++) {
        PokeathlonCourseSpritePoolSprite_Show(anim->scoreboardSprites[i], 1, 0);
    }
    for (i = 0; i < 4; i++) {
        PokeathlonCourseSpritePoolSprite_Show(anim->nameplateSprites[i], 1, 0);
        PokeathlonCourseSpritePoolSprite_Show(anim->playerEventSprites[i], 1, 0);
    }
    PokeathlonCourseSpritePoolSprite_Show(anim->zoomSprite, 1, 0);
}

BOOL PokeathlonCirclePushDisplayAnim_TryStart(PokeathlonCirclePushDisplayAnim *anim, int phase) {
    if (PokeathlonCirclePushDisplayAnim_CanStart(anim, phase)) {
        PokeathlonCirclePushDisplayAnim_Stop(anim);
        return TRUE;
    }
    return FALSE;
}

void PokeathlonCirclePushDisplayAnim_StartScoreboard(PokeathlonCirclePushDisplayAnim *anim, BgConfig *bgConfig) {
    u8 i;

    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    for (i = 0; i < 2; i++) {
        PokeathlonCourseSpritePoolSprite_Show(anim->scoreboardSprites[i], TRUE, TRUE);
    }
    for (i = 0; i < 4; i++) {
        PokeathlonCourseSpritePoolSprite_Show(anim->nameplateSprites[i], TRUE, TRUE);
    }
    BG_LoadScreenTilemapData(bgConfig, GF_BG_LYR_SUB_2, anim->scoreboardScreen->rawData, anim->scoreboardScreen->szByte);
    PokeathlonCirclePushDisplayAnim_PrintScore(anim, bgConfig);
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_SUB_2, 0, 0, 32, 24, 3);
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_SUB_2);
    anim->update = PokeathlonCirclePushDisplay_UpdateScoreboard;
    anim->phase = CIRCLE_PUSH_DISPLAY_SCOREBOARD;
    anim->active = TRUE;
}

void PokeathlonCirclePushDisplayAnim_StartMonPic(PokeathlonCirclePushDisplayAnim *anim, BgConfig *bgConfig, int mon) {
    PokeathlonCirclePushDisplayAnim_SetupMonPic(anim, mon);
    BG_LoadScreenTilemapData(bgConfig, GF_BG_LYR_SUB_2, anim->monPicScreen->rawData, anim->monPicScreen->szByte);
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_SUB_2, 0, 0, 32, 32, 3);
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_SUB_2);
    anim->update = PokeathlonCirclePushDisplay_UpdateMonPic;
    anim->phase = CIRCLE_PUSH_DISPLAY_MON_PIC;
    anim->active = TRUE;
}

void PokeathlonCirclePushDisplayAnim_StartMatchup(PokeathlonCirclePushDisplayAnim *anim, BgConfig *bgConfig) {
    MtxFx22 matrix;

    PokeathlonCirclePushDisplayAnim_SetupMatchup(anim);
    matrix._00 = FX32_ONE;
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = FX32_ONE;
    OS_WaitVBlankIntr();
    SetBgAffine(bgConfig, GF_BG_LYR_SUB_3, &matrix, 128, 96);
    BG_LoadScreenTilemapData(bgConfig, GF_BG_LYR_SUB_2, anim->matchupBg2Screen->rawData, anim->matchupBg2Screen->szByte);
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_SUB_2, 0, 0, 32, 24, 3);
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_SUB_2);
    BG_LoadScreenTilemapData(bgConfig, GF_BG_LYR_SUB_3, anim->matchupBg3Screen->rawData, anim->matchupBg3Screen->szByte);
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_SUB_3);
    anim->update = PokeathlonCirclePushDisplay_UpdateMatchup;
    anim->phase = CIRCLE_PUSH_DISPLAY_MATCHUP;
    anim->active = TRUE;
}

#pragma force_active on

void PokeathlonCirclePushDisplayAnim_StartZoom(PokeathlonCirclePushDisplayAnim *anim, BgConfig *bgConfig) {
    VecFx32 pos = { 0, 0, 0 };
    MtxFx22 matrix;
    f32 scale;

    pos.x = FX32_CONST(56);
    pos.y = FX32_CONST(656);
    Sprite_SetMatrix(PokeathlonCourseSpritePoolSprite_GetSprite(anim->zoomSprite), &pos);
    // takes the size from the matchup screen, which has the same size
    BG_LoadScreenTilemapData(bgConfig, GF_BG_LYR_SUB_2, anim->zoomBg2Screen->rawData, anim->matchupBg2Screen->szByte);
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_SUB_2, 0, 0, 32, 24, 3);
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_SUB_2);
    BG_LoadScreenTilemapData(bgConfig, GF_BG_LYR_SUB_3, anim->zoomBg3Screen->rawData, anim->zoomBg3Screen->szByte);
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_SUB_3);
    scale = 0.025f;
    matrix._00 = 4096.0f * scale;
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = matrix._00;
    OS_WaitVBlankIntr();
    SetBgAffine(bgConfig, GF_BG_LYR_SUB_3, &matrix, 128, 96);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    anim->update = PokeathlonCirclePushDisplay_UpdateZoom;
    anim->phase = CIRCLE_PUSH_DISPLAY_ZOOM;
    anim->active = TRUE;
}

#pragma force_active reset

void PokeathlonCirclePushDisplayAnim_StartIdle(PokeathlonCirclePushDisplayAnim *anim, BgConfig *bgConfig) {
    PokeathlonCirclePushDisplayAnim_Stop(anim);
    if (ov96_0221DC04[anim->idleCount] == CIRCLE_PUSH_DISPLAY_MATCHUP) {
        PokeathlonCirclePushDisplayAnim_StartMatchup(anim, bgConfig);
    } else {
        PokeathlonCirclePushDisplayAnim_StartZoom(anim, bgConfig);
    }
    anim->idleCount = (anim->idleCount + 1) % 3;
}

void PokeathlonCirclePushDisplay_UpdateScoreboard(PokeathlonCirclePushDisplay *display) {
    if (display->anim.shownScore != display->anim.score) {
        PokeathlonCirclePushDisplayAnim_PrintScore(&display->anim, display->bgConfig);
        BgTilemapRectChangePalette(display->bgConfig, GF_BG_LYR_SUB_2, 3, 7, 12, 8, 3);
        ScheduleBgTilemapBufferTransfer(display->bgConfig, GF_BG_LYR_SUB_2);
    }
}

void PokeathlonCirclePushDisplay_UpdateMonPic(PokeathlonCirclePushDisplay *display) {
    PokeathlonCirclePushDisplayAnim *anim = &display->anim;

    anim->frame++;
    if (anim->frame > 70) {
        PokeathlonCirclePushDisplayAnim_Stop(anim);
        return;
    }
    PokeathlonCirclePushDisplayAnim_MovePicVertically(anim->frame, &ov96_0221C01C, anim->picSprites[0]);
    anim->scroll += 32;
    G2S_SetBG2Offset(0, anim->scroll);
}

void PokeathlonCirclePushDisplay_UpdateMatchup(PokeathlonCirclePushDisplay *display) {
    PokeathlonCirclePushDisplayAnim *anim = &display->anim;
    u16 frame;
    u16 alpha;

    anim->frame++;
    frame = anim->frame;
    if (frame > 50) {
        PokeathlonCirclePushDisplayAnim_Stop(anim);
        return;
    }
    if (frame == 5) {
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    } else if (frame > 15 && (frame - 15) <= 4) {
        alpha = ((frame - 15) * 16) / 4;
        G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 16 - alpha, alpha);
    }
    PokeathlonCirclePushDisplayAnim_MovePicAlongPath(anim->frame, ov96_0221C050, anim->picSprites[0]);
    PokeathlonCirclePushDisplayAnim_MovePicAlongPath(anim->frame, ov96_0221C080, anim->picSprites[1]);
}

void PokeathlonCirclePushDisplay_UpdateZoom(PokeathlonCirclePushDisplay *display) {
    PokeathlonCirclePushDisplayAnim *anim = &display->anim;
    MtxFx22 matrix;
    VecFx32 pos;
    u16 alpha;
    u16 frame;

    anim->frame++;
    if (anim->frame > 80) {
        PokeathlonCirclePushDisplayAnim_Stop(anim);
        return;
    }
    if (anim->frame == 41) {
        PokeathlonCourseSpritePoolSprite_Show(anim->zoomSprite, 1, 1);
    } else if (anim->frame == 61) {
        PokeathlonCourseSpritePoolSprite_Show(anim->zoomSprite, 1, 0);
    }
    frame = anim->frame;
    if (frame <= 40) {
        matrix._00 = (f32)frame / 40.0f * 4096.0f;
        matrix._01 = 0;
        matrix._10 = 0;
        matrix._11 = matrix._00;
        OS_WaitVBlankIntr();
        SetBgAffine(display->bgConfig, GF_BG_LYR_SUB_3, &matrix, 128, 96);
        return;
    }
    if (frame > 40 && frame <= 60) {
        pos.z = 0;
        pos.x = (((frame - 40) * 144) / 20 + 56) << FX32_SHIFT;
        pos.y = FX32_CONST(656);
        Sprite_SetMatrix(PokeathlonCourseSpritePoolSprite_GetSprite(anim->zoomSprite), &pos);
        return;
    }
    PokeathlonCourseSpritePoolSprite_Show(anim->zoomSprite, 1, 0);
    alpha = ((anim->frame - 60) << 4) / 20;
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BD, 16 - alpha, alpha);
}

void PokeathlonCirclePushDisplayAnim_SetupMatchup(PokeathlonCirclePushDisplayAnim *anim) {
    u8 tries;
    int first;
    int second;
    VecFx32 pos;
    Sprite *sprite;

    // two Pokemon of different teams
    first = (s32)LCRandom() % 4;
    tries = 0;
    do {
        second = (s32)LCRandom() % 4;
        if (first != second) {
            break;
        }
        tries++;
    } while (tries < 10);
    if (tries == 10) {
        second = (first + 1) % 4;
    }
    first = first * 3 + (s32)LCRandom() % 3;
    second = second * 3 + (s32)LCRandom() % 3;
    PokeathlonCirclePushDisplayAnim_LoadMonPic(anim, 0, (u8)first);
    PokeathlonCirclePushDisplayAnim_LoadMonPic(anim, 1, (u8)second);

    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(anim->picSprites[0]);
    pos = ov96_0221C050[0];
    pos.y += FX32_CONST(512);
    Sprite_SetMatrix(sprite, &pos);
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(anim->picSprites[1]);
    pos = ov96_0221C080[0];
    pos.y += FX32_CONST(512);
    Sprite_SetMatrix(sprite, &pos);
    PokeathlonCourseSpritePoolSprite_Show(anim->picSprites[0], 1, 1);
    PokeathlonCourseSpritePoolSprite_Show(anim->picSprites[1], 1, 1);
}

// Moves a picture along path[0] -> path[1] (frames 0-5), -> path[2] (5-45), -> path[3] (45-50).
#pragma force_active on

void PokeathlonCirclePushDisplayAnim_MovePicAlongPath(u8 frame, const VecFx32 *path, void *picSprite) {
    fx32 startX;
    fx32 startY;
    fx32 x;
    fx32 y;
    VecFx32 pos;
    Sprite *sprite;

    if (frame <= 5) {
        startX = path[0].x;
        startY = path[0].y;
        x = path[1].x - startX;
        x = startX + x * frame / 5;
        y = path[1].y - startY;
        y = startY + y * frame / 5;
    } else if (frame > 5 && frame <= 45) {
        frame -= 5;
        startX = path[1].x;
        startY = path[1].y;
        x = path[2].x - startX;
        x = startX + x * frame / 40;
        y = path[2].y - startY;
        y = startY + y * frame / 40;
    } else {
        frame -= 45;
        startX = path[2].x;
        startY = path[2].y;
        x = path[3].x - startX;
        x = startX + x * frame / 5;
        y = path[3].y - startY;
        y = startY + y * frame / 5;
    }
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(picSprite);
    pos.x = x;
    pos.y = y + FX32_CONST(512);
    pos.z = 0;
    Sprite_SetMatrix(sprite, &pos);
}

#pragma force_active reset

void PokeathlonCirclePushDisplayAnim_SetupMonPic(PokeathlonCirclePushDisplayAnim *anim, int mon) {
    VecFx32 pos;
    Sprite *sprite;

    PokeathlonCirclePushDisplayAnim_LoadMonPic(anim, 0, mon);
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(anim->picSprites[0]);
    pos.x = FX32_CONST(128);
    pos.y = FX32_CONST(472);
    pos.z = 0;
    Sprite_SetMatrix(sprite, &pos);
    PokeathlonCourseSpritePoolSprite_Show(anim->picSprites[0], 1, 1);
}

// Slides a picture in (frames 0-15), holds it (15-60) and slides it out (60-70).
#pragma force_active on

void PokeathlonCirclePushDisplayAnim_MovePicVertically(u8 frame, const PokeathlonCirclePushPicKeys *keys, void *picSprite) {
    s32 y;
    VecFx32 pos;
    Sprite *sprite;

    if (frame <= 15) {
        s32 delta = keys->hold - keys->start;
        y = keys->start + delta * frame / 15;
    } else if (frame > 15 && frame <= 60) {
        y = keys->hold;
    } else {
        u8 t = frame - 60;
        s32 delta = keys->end - keys->hold;
        y = keys->hold + delta * t / 10;
    }
    sprite = PokeathlonCourseSpritePoolSprite_GetSprite(picSprite);
    pos.x = FX32_CONST(128);
    pos.y = y + FX32_CONST(512);
    pos.z = 0;
    Sprite_SetMatrix(sprite, &pos);
}

#pragma force_active reset

void PokeathlonCirclePushDisplayAnim_PrintScore(PokeathlonCirclePushDisplayAnim *anim, BgConfig *bgConfig) {
    u8 srcX[3];
    u8 srcY[3];
    u8 score;
    u8 digit;
    u8 i;

    score = anim->score;
    digit = score / 100;
    srcX[0] = digit % 5 * 4;
    srcY[0] = digit / 5 * 8;
    digit = (u8)(score / 10 - 10 * digit);
    srcX[1] = digit % 5 * 4;
    srcY[1] = digit / 5 * 8;
    digit = score % 10;
    srcX[2] = digit % 5 * 4;
    srcY[2] = digit / 5 * 8;

    for (i = 0; i < 3; i++) {
        CopyRectToBgTilemapRect(bgConfig, GF_BG_LYR_SUB_2, i * 4 + 3, 7, 4, 8, anim->digitsScreen->rawData, srcX[i], srcY[i], 20, 16);
    }
    anim->shownScore = anim->score;
}

// Adds offset to the tile number of every tilemap entry (the palette bits are dropped).
void PokeathlonCirclePushDisplay_OffsetScreenTiles(int offset, NNSG2dScreenData *screen) {
    u16 count = screen->szByte / 2;
    u16 *tiles = (u16 *)screen->rawData;
    int i;

    for (i = 0; i < count; i++) {
        u16 tile = *tiles;
        u16 tileNum = tile & 0xFFF;
        u8 high = tile - tileNum;
        *tiles++ = high + (u16)(tileNum + offset);
    }
}
