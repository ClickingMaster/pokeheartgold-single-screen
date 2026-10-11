#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0314.h"
#include "pokeathlon/pokeathlon_record_screens.h"

#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "poke_overlay.h"
#include "sound_playback.h"
#include "system.h"

FS_EXTERN_OVERLAY(OVY_98);

extern const SpriteResourceCountsListUnion ov99_021E95A4;
extern const u8 ov99_021E9600[][4];
extern const u8 _021E954C[8];
extern const GraphicsModes ov99_021E9564;
extern const PokeathlonCourseRecordBgTemplates ov99_021E9760;
extern const s32 ov99_021E95D8[][2];
extern const s32 ov99_021E95DC[][2];
extern const PokeathlonCourseRecordSpritePos ov99_021E9690[6];
extern const u32 ov99_021E958C[];
extern const int ov99_021E9554[];
extern const int ov99_021E9650[3][4];
extern const ManagedSpriteTemplate ov99_021E9840[26];
extern const u32 ov99_021E9628[];
extern const TouchscreenHitbox ov99_021E95BC[];
extern const s16 ov99_021E9574[];
extern const s16 ov99_021E9576[];
extern const WindowTemplate ov99_021E96D8[17];
extern const char _021EA5C0[]; // "seq error!"

// The first 0xDC bytes of PokeathlonSave hold the five course records.
static inline PokeathlonCourseRecordEntry *PokeathlonCourseRecord_GetRecords(PokeathlonCourseRecordArgs *args) {
    return (PokeathlonCourseRecordEntry *)args->pokeathlon;
}

BOOL PokeathlonCourseRecord_Init(OverlayManager *man, int *state) {
    PokeathlonCourseRecord *work;
    PokeathlonCourseRecordArgs *args;
    SpriteResourceCountsListUnion counts;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_98), OVY_LOAD_ASYNC);
    args = OverlayManager_GetArgs(man);
    GF_ASSERT(args != NULL);
    Heap_Create(HEAP_ID_3, HEAP_ID_132, 0x30000);
    work = OverlayManager_CreateAndGetData(man, sizeof(PokeathlonCourseRecord), HEAP_ID_132);
    MI_CpuFill8(work, 0, sizeof(PokeathlonCourseRecord));
    work->heapId = HEAP_ID_132;
    work->selection.scrolling = FALSE;
    work->selection.pageChanged = TRUE;
    work->selection.scrollForward = TRUE;
    PokeathlonCourseRecord_CopyArgs(work, args);
    PokeathlonUtil_ResetDisplay();
    PokeathlonCourseRecord_InitBgs(work);
    PokeathlonCourseRecord_LoadBgGraphics(work);
    work->textContext = PokeathlonUtil_CreateTextContext(work->heapId, work->bgConfig, 17, ov99_021E96D8, NARC_msg_msg_0314_bin);
    counts = ov99_021E95A4;
    work->spriteContext = PokeathlonUtil_CreateSpriteContext(work->heapId, &counts, 26);
    PokeathlonCourseRecord_LoadSpriteResources(work);
    PokeathlonCourseRecord_CreateSprites(work);
    PokeathlonCourseRecord_SetMonSpritesVisible(work, FALSE);
    PokeathlonCourseRecord_ShowCoursePanel(work, 0);
    PokeathlonCourseRecord_PrintPage(work, 0);
    PokeathlonCourseRecord_UpdatePageSprites(work, 0);
    PokeathlonCourseRecord_PrintStaticText(work);
    SetKeyRepeatTimers(2, 4);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    Main_SetVBlankIntrCB((GFIntrCB)PokeathlonCourseRecord_VBlankCallback, work);
    return TRUE;
}

BOOL PokeathlonCourseRecord_Exit(OverlayManager *man, int *state) {
    PokeathlonCourseRecord *work = OverlayManager_GetData(man);

    SetKeyRepeatTimers(4, 8);
    PokeathlonCourseRecord_FreeArgs(work->args);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_132);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    return TRUE;
}

BOOL PokeathlonCourseRecord_Main(OverlayManager *man, int *state) {
    PokeathlonCourseRecord *work = OverlayManager_GetData(man);

    PokeathlonCourseRecord_UpdatePageScroll(work);
    if (work->selection.pageChanged == TRUE) {
        PokeathlonCourseRecord_ShowCoursePanel(work, work->selection.page);
        PokeathlonCourseRecord_PrintPage(work, work->selection.page);
        PokeathlonCourseRecord_UpdatePageSprites(work, work->selection.page);
        work->selection.pageChanged = FALSE;
        work->selection.scrolling = TRUE;
    }
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, work->heapId);
        *state = 1;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            *state = 2;
        }
        break;
    case 2:
        if (PokeathlonCourseRecord_HandleInput(work) && work->selection.scrolling == FALSE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, work->heapId);
            *state = 3;
        }
        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            *state = 4;
        }
        break;
    case 4:
        PokeathlonUtil_FreeTextContext(work->textContext, 17);
        PokeathlonCourseRecord_FreeBgs(work);
        PokeathlonUtil_FreeSpriteContext(work->spriteContext, work->sprites, 26);
        PokeathlonUtil_DisableDisplay();
        return TRUE;
    default:
        GF_ASSERT(!_021EA5C0);
        break;
    }
    PokeathlonUtil_DrawSprites(work->spriteContext);
    return FALSE;
}

void PokeathlonCourseRecord_CopyArgs(PokeathlonCourseRecord *work, const PokeathlonCourseRecordArgs *args) {
    PokeathlonCourseRecordArgs *copy = Heap_Alloc(work->heapId, sizeof(PokeathlonCourseRecordArgs));
    copy->pokeathlon = args->pokeathlon;
    copy->unk4 = args->unk4;
    work->args = copy;
}

BOOL ov99_021E5B70(PokeathlonCourseRecordArgs *args) {
    return args->unk4;
}

void PokeathlonCourseRecord_FreeArgs(PokeathlonCourseRecordArgs *args) {
    Heap_Free(args);
}

PokeathlonRecordMon *PokeathlonCourseRecord_GetMon(PokeathlonCourseRecordArgs *args, int course, int slot) {
    return &PokeathlonCourseRecord_GetRecords(args)[course].mons[slot];
}

u16 PokeathlonCourseRecord_GetTotalScore(PokeathlonCourseRecordArgs *args, int course) {
    GF_ASSERT(args != NULL);
    GF_ASSERT(course < 5);
    return PokeathlonCourseRecord_GetRecords(args)[course].totalScore;
}

BOOL PokeathlonCourseRecord_HasRecord(PokeathlonCourseRecordArgs *args, int course) {
    GF_ASSERT(args != NULL);
    return PokeathlonCourseRecord_GetTotalScore(args, course) != 0;
}

#pragma force_active on

u16 PokeathlonCourseRecord_GetScore(PokeathlonCourseRecordArgs *args, int course, int event) {
    GF_ASSERT(args != NULL);
    if (event == 3) {
        u16 bonus = PokeathlonCourseRecord_GetRecords(args)[course].totalScore - PokeathlonCourseRecord_GetRecords(args)[course].eventScores[0];
        bonus -= PokeathlonCourseRecord_GetRecords(args)[course].eventScores[1];
        bonus -= PokeathlonCourseRecord_GetRecords(args)[course].eventScores[2];
        return bonus;
    }
    return PokeathlonCourseRecord_GetRecords(args)[course].eventScores[event];
}

#pragma force_active reset

int PokeathlonCourseRecord_GetEventId(PokeathlonCourseRecordArgs *args, s32 course, u32 event) {
    const u8 *events;

    GF_ASSERT(args != NULL);
    GF_ASSERT(course < 10);
    GF_ASSERT(event < 4);
    events = ov99_021E9600[course];
    return events[event];
}

u16 PokeathlonCourseRecord_GetMonSpecies(PokeathlonCourseRecordArgs *args, u32 course, u32 slot) {
    GF_ASSERT(args != NULL);
    GF_ASSERT(slot < 3);
    GF_ASSERT(course < 5);
    return PokeathlonCourseRecord_GetRecords(args)[course].mons[slot].info.species;
}

#pragma force_active on

void PokeathlonCourseRecord_InitBgs(PokeathlonCourseRecord *work) {
    GraphicsModes modes;
    PokeathlonCourseRecordBgTemplates templates;
    int i;

    work->bgConfig = BgConfig_Alloc(work->heapId);
    modes = ov99_021E9564;
    SetBothScreensModesAndDisable(&modes);
    templates = ov99_021E9760;
    for (i = 0; i < 8; i++) {
        InitBgFromTemplate(work->bgConfig, _021E954C[i], &templates.templates[i], GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(work->bgConfig, _021E954C[i]);
        BG_ClearCharDataRange(_021E954C[i], 32, 0, work->heapId);
    }
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG2, 15, 0);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG1, 15, 0);
    work->narc = NARC_New(NARC_a_1_7_7, work->heapId);
}

#pragma force_active reset

void PokeathlonCourseRecord_FreeBgs(PokeathlonCourseRecord *work) {
    u8 i;

    NARC_Delete(work->narc);
    for (i = 0; i < 8; i++) {
        FreeBgTilemapBuffer(work->bgConfig, _021E954C[i]);
    }
    Heap_Free(work->bgConfig);
}

void PokeathlonCourseRecord_LoadBgGraphics(PokeathlonCourseRecord *work) {
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 30, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 31, work->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 32, work->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 62, work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 36, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x1E0, 0x20, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 37, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 38, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 33, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 34, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 35, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 68, work->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 39, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x1E0, 0x20, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 40, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 41, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, work->heapId);
}

#pragma force_active on

void PokeathlonCourseRecord_StartPageScroll(PokeathlonCourseRecord *work, int course, BOOL forward) {
    s16 offset;
    u8 i;

    if (forward) {
        offset = -256;
    } else {
        offset = 256;
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, ov99_021E95D8[course][0], work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, ov99_021E95DC[course][0], work->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, work->heapId);
    BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, offset);
    BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, offset);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    for (i = 0; i < 6; i++) {
        ManagedSprite_OffsetPositionXY(work->sprites[ov99_021E9690[i].sprite], -offset, 0);
    }
}

#pragma force_active reset

void PokeathlonCourseRecord_UpdatePageScroll(PokeathlonCourseRecord *work) {
    s32 xpos;
    s32 x;
    u8 i;

    if (work->selection.scrolling == FALSE) {
        return;
    }
    xpos = Bg_GetXpos(work->bgConfig, GF_BG_LYR_MAIN_2);
    x = xpos >> 2;
    if (xpos == x) {
        x = 0;
    }
    ScheduleSetBgPosText(work->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, x);
    ScheduleSetBgPosText(work->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, x);
    for (i = 0; i < 6; i++) {
        const PokeathlonCourseRecordSpritePos *pos = &ov99_021E9690[i];
        ManagedSprite_SetPositionXY(work->sprites[pos->sprite], pos->x - x, pos->y);
    }
    if (x == 0) {
        work->selection.scrolling = FALSE;
    }
}

void PokeathlonCourseRecord_ShowCoursePanel(PokeathlonCourseRecord *work, u32 course) {
    if (PokeathlonCourseRecord_HasRecord(work->args, course)) {
        PokeathlonCourseRecord_StartPageScroll(work, course, work->selection.scrollForward);
    } else {
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_1);
    }
}

void PokeathlonCourseRecord_PrintStaticText(PokeathlonCourseRecord *work) {
    ov98_0221EC08(work->textContext, 0, msg_0314_00002, 1, 0);
    ov98_0221EBEC(work->textContext, 2, msg_0314_00000, TRUE, 0, 4);
    ov98_0221EBD8(work->textContext, 11, msg_0314_00007, FALSE);
}

void PokeathlonCourseRecord_PrintPage(PokeathlonCourseRecord *work, u32 course) {
    PokeathlonCourseRecord_PrintCourseName(work, course);
    PokeathlonCourseRecord_PrintEventScores(work, course);
    PokeathlonCourseRecord_PrintMonNames(work, PokeathlonCourseRecord_HasRecord(work->args, course));
    PokeathlonCourseRecord_PrintTotalLabel(work);
}

void PokeathlonCourseRecord_PrintEventScores(PokeathlonCourseRecord *work, int course) {
    MessageFormat *msgFormat = PokeathlonUtil_GetMessageFormat(work->textContext);
    int i;
    const u32 *windows = ov99_021E958C;

    for (i = 0; i < 3; i++, windows += 2) {
        BufferPokeathlonEventName(msgFormat, 1, (u8)PokeathlonCourseRecord_GetEventId(work->args, course, (u8)i));
        PokeathlonUtil_PrintMessageWithNumber(work->textContext, windows[0], msg_0314_00010, i + 1, 1, 0);
        if (PokeathlonCourseRecord_HasRecord(work->args, course)) {
            PokeathlonUtil_PrintMessageWithNumber(work->textContext, windows[1], msg_0314_00008, PokeathlonCourseRecord_GetScore(work->args, course, (u16)i), 3, 0);
        } else {
            ov98_0221EBD8(work->textContext, windows[1], msg_0314_00009, FALSE);
        }
    }
    if (PokeathlonCourseRecord_HasRecord(work->args, course)) {
        PokeathlonUtil_PrintMessageWithNumber(work->textContext, 12, msg_0314_00008, PokeathlonCourseRecord_GetScore(work->args, course, 3), 3, 0);
    } else {
        ov98_0221EBD8(work->textContext, 12, msg_0314_00009, FALSE);
    }
}

void PokeathlonCourseRecord_PrintMonNames(PokeathlonCourseRecord *work, BOOL show) {
    s32 i;
    PokeathlonTextContext *textContext = work->textContext;

    for (i = 0; i < 3; i++) {
        if (show) {
            PokeathlonUtil_PrintSpeciesName(textContext, i + 14, PokeathlonCourseRecord_GetMonSpecies(work->args, (u8)work->selection.page, (u8)i));
        } else {
            PokeathlonUtil_ClearAndCopyWindow(textContext, i + 14);
        }
    }
}

#pragma force_active on

void PokeathlonCourseRecord_PrintCourseName(PokeathlonCourseRecord *work, int course) {
    PokeathlonTextContext *textContext = work->textContext;
    MessageFormat *msgFormat = PokeathlonUtil_GetMessageFormat(textContext);
    int medal;

    BufferPokeathlonMedalName(msgFormat, 0, (u8)course);
    ov98_0221EC08(work->textContext, 1, msg_0314_00004, 1, 0);
    BufferPokeathlonCourseName(msgFormat, 0, course & 0xFF);
    ov98_0221EBD8(work->textContext, 3, msg_0314_00003, TRUE);
    if (ov99_021E5B70(work->args)) {
        medal = ov98_0221EF64(PokeathlonCourseRecord_GetTotalScore(work->args, course));
        if (medal == 0) {
            ov98_0221EBD8(work->textContext, 4, msg_0314_00005, TRUE);
        } else if (medal == 1) {
            ov98_0221EBD8(work->textContext, 4, msg_0314_00006, TRUE);
        } else {
            FillWindowPixelBuffer(&textContext->windows[4], 0);
            ScheduleWindowCopyToVram(&textContext->windows[4]);
        }
    }
}

#pragma force_active reset

void PokeathlonCourseRecord_PrintTotalLabel(PokeathlonCourseRecord *work) {
    PokeathlonTextContext *textContext = work->textContext;

    if (PokeathlonCourseRecord_HasRecord(work->args, work->selection.page)) {
        ov98_0221EBD8(work->textContext, 13, msg_0314_00001, FALSE);
    } else {
        FillWindowPixelBuffer(&textContext->windows[13], 0);
        ScheduleWindowCopyToVram(&textContext->windows[13]);
    }
}

void PokeathlonCourseRecord_VBlankCallback(PokeathlonCourseRecord *work) {
    vu8 *dtcm = (vu8 *)0x027E0000;

    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(work->bgConfig);
    *(vu32 *)(dtcm + 0x3FF8) |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK), DTCM address as a literal
}

void PokeathlonCourseRecord_LoadSpriteResources(PokeathlonCourseRecord *work) {
    SpriteSystem *spriteSystem = PokeathlonUtil_GetSpriteSystem(work->spriteContext);
    SpriteManager *spriteManager = PokeathlonUtil_GetSpriteManager(work->spriteContext);

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 21, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 20, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 22, TRUE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 23, TRUE, 0);

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 1, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 1);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 0, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 2, TRUE, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 3, TRUE, 1);

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 5, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 2);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 4, FALSE, 5, NNS_G2D_VRAM_TYPE_2DMAIN, 2);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 6, TRUE, 2);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 7, TRUE, 2);

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 9, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 6);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 8, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 3);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 10, TRUE, 3);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 11, TRUE, 3);

    PokeathlonCourseRecord_LoadMonPicResources(work);
    PokeathlonUtil_LoadMonIconResources(work->spriteContext, ov99_021E9554, 3, 0, TRUE);
}

void PokeathlonCourseRecord_LoadMonPicResources(PokeathlonCourseRecord *work) {
    u8 i;
    SpriteSystem *spriteSystem = PokeathlonUtil_GetSpriteSystem(work->spriteContext);
    SpriteManager *spriteManager = PokeathlonUtil_GetSpriteManager(work->spriteContext);

    for (i = 0; i < 3; i++) {
        ov98_0221E784(spriteSystem, spriteManager, ov99_021E9650[i], NNS_G2D_VRAM_TYPE_2DMAIN);
    }
}

#pragma force_active on

void PokeathlonCourseRecord_CreateSprites(PokeathlonCourseRecord *work) {
    int i;
    s32 course;
    const ManagedSpriteTemplate *spriteTemplate = ov99_021E9840;

    for (i = 0; i < 26; i++) {
        work->sprites[i] = PokeathlonUtil_NewSprite(work->spriteContext, spriteTemplate);
        ManagedSprite_SetAnimateFlag(work->sprites[i], TRUE);
        spriteTemplate++;
    }
    for (course = 0; course < 5; course++) {
        int medal;
        if (!PokeathlonCourseRecord_HasRecord(work->args, course)) {
            ManagedSprite_SetDrawFlag(work->sprites[1 + course], FALSE);
        }
        medal = ov98_0221EF64(PokeathlonCourseRecord_GetTotalScore(work->args, course));
        if (medal == 2) {
            if (course % 2 == 0) {
                ManagedSprite_SetAnim(work->sprites[6 + course], 5);
            } else {
                ManagedSprite_SetAnim(work->sprites[6 + course], 6);
            }
        } else if (medal == 1) {
            if (course % 2 == 0) {
                ManagedSprite_SetAnim(work->sprites[6 + course], 2);
            } else {
                ManagedSprite_SetAnim(work->sprites[6 + course], 3);
            }
        } else {
            ManagedSprite_SetDrawFlag(work->sprites[6 + course], FALSE);
        }
    }
}

#pragma force_active reset

void PokeathlonCourseRecord_SetMonSpritesVisible(PokeathlonCourseRecord *work, BOOL visible) {
    ManagedSprite_SetDrawFlag(work->sprites[14], visible);
    ManagedSprite_SetDrawFlag(work->sprites[15], visible);
    ManagedSprite_SetDrawFlag(work->sprites[16], visible);
    ManagedSprite_SetDrawFlag(work->sprites[11], visible);
    ManagedSprite_SetDrawFlag(work->sprites[12], visible);
    ManagedSprite_SetDrawFlag(work->sprites[13], visible);
    ManagedSprite_SetDrawFlag(work->sprites[17], visible);
    ManagedSprite_SetDrawFlag(work->sprites[18], visible);
    ManagedSprite_SetDrawFlag(work->sprites[19], visible);
}

#pragma force_active on

void PokeathlonCourseRecord_UpdatePageSprites(PokeathlonCourseRecord *work, int course) {
    u32 totalScore = PokeathlonCourseRecord_GetTotalScore(work->args, course);
    u8 i;

    for (i = 0; i < 3; i++) {
        if (PokeathlonCourseRecord_HasRecord(work->args, course) == TRUE) {
            u16 digit = PokeathlonUtil_GetDecimalDigit(totalScore, (u8)(i + 1));
            ManagedSprite_SetAnim(work->sprites[20 + i], digit);
            ManagedSprite_SetAnim(work->sprites[14 + i], ov99_021E9628[digit]);
            ManagedSprite_SetDrawFlag(work->sprites[14 + i], TRUE);
            ManagedSprite_SetDrawFlag(work->sprites[20 + i], TRUE);
        } else {
            ManagedSprite_SetDrawFlag(work->sprites[14 + i], FALSE);
            ManagedSprite_SetDrawFlag(work->sprites[20 + i], FALSE);
        }
    }
    for (i = 0; i < 3; i++) {
        PokeathlonRecordMon *mon = PokeathlonCourseRecord_GetMon(work->args, course, i);
        if (PokeathlonCourseRecord_HasRecord(work->args, course) == TRUE) {
            PokeathlonUtil_LoadMonPicSprite(work->sprites[11 + i], &mon->info, TRUE, FALSE, work->heapId);
            ManagedSprite_SetDrawFlag(work->sprites[11 + i], TRUE);
            PokeathlonUtil_SetMonIconSprite(work->spriteContext, work->sprites[23 + i], &mon->info, FALSE, TRUE);
            ManagedSprite_SetDrawFlag(work->sprites[23 + i], TRUE);
        } else {
            ManagedSprite_SetDrawFlag(work->sprites[23 + i], FALSE);
        }
    }
    for (i = 0; i < 3; i++) {
        ManagedSprite_SetDrawFlag(work->sprites[17 + i], PokeathlonCourseRecord_HasRecord(work->args, course));
    }
}

BOOL PokeathlonCourseRecord_HandleInput(PokeathlonCourseRecord *work) {
    PokeathlonCourseRecordSelection *selection = &work->selection;
    int oldCursor = selection->cursor;
    int cursor;
    BOOL exit = FALSE;

    if (gSystem.newKeys & PAD_BUTTON_B) {
        return TRUE;
    }
    if (gSystem.touchNew) {
        int hit = TouchscreenHitbox_FindHitboxAtTouchNew(ov99_021E95BC);
        if (hit != -1) {
            if (hit == 5) {
                exit = TRUE;
            } else {
                selection->scrollForward = selection->cursor < hit;
                ManagedSprite_SetAnim(work->sprites[0], 9);
                ManagedSprite_SetAnimateFlag(work->sprites[0], TRUE);
                PlaySE(SEQ_SE_DP_SELECT);
            }
            selection->cursor = hit;
        }
    } else if ((gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) || (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN)) {
        if (oldCursor < 5) {
            selection->cursor++;
            selection->scrollForward = TRUE;
            PlaySE(SEQ_SE_DP_SELECT);
        }
    } else if ((gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) || (gSystem.newAndRepeatedKeys & PAD_KEY_UP)) {
        if (oldCursor > 0) {
            selection->cursor--;
            selection->scrollForward = FALSE;
            PlaySE(SEQ_SE_DP_SELECT);
        }
    } else if ((gSystem.newKeys & PAD_BUTTON_A) && oldCursor == 5) {
        return TRUE;
    }
    cursor = selection->cursor;
    if (cursor > 5) {
        cursor = 5;
    } else if (cursor < 0) {
        cursor = 0;
    }
    selection->cursor = cursor;
    work->selection.page = selection->cursor;
    if (oldCursor != selection->cursor) {
        ManagedSprite_SetPositionXY(work->sprites[0], ov99_021E9574[selection->cursor * 2], ov99_021E9576[selection->cursor * 2]);
        if (selection->cursor == 5) {
            ManagedSprite_SetAnim(work->sprites[0], 12);
        } else {
            if (ManagedSprite_GetActiveAnim(work->sprites[0]) != 9) {
                ManagedSprite_SetAnim(work->sprites[0], 8);
            }
            if (oldCursor != 5 || selection->cursor != 4) {
                PokeathlonCourseRecord_SetMonSpritesVisible(work, FALSE);
                work->selection.pageChanged = TRUE;
            }
        }
    }
    return exit;
}

#pragma force_active reset
