#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0314.h"
#include "pokeathlon/pokeathlon_record_screens.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "sound_playback.h"
#include "system.h"

FS_EXTERN_OVERLAY(OVY_98);

// work->sprites[index], addressed as (work + index) first: with plain array indexing the compiler
// folds the index differently than the original code.
#define EVENT_RECORD_SPRITE(work, index) (((PokeathlonEventRecord *)((u8 *)(work) + (index) * sizeof(ManagedSprite *)))->sprites[0])

extern const WindowTemplate ov99_021EA45C[19];
extern const char _021EA5E4[]; // "seq error!"
extern const GraphicsModes ov99_021EA374;
extern const PokeathlonEventRecordBgTemplates ov99_021EA4F4;
extern const u8 ov99_021EA59C[6];
extern const u8 ov99_021EA35A[5][2];
extern const u32 ov99_021EA384[2];
extern const u32 ov99_021EA38C[2];
extern const SpriteResourceCountsListUnion ov99_021EA3AC;
extern const int ov99_021EA364[];
extern const ManagedSpriteTemplate ov99_021EA3F4[2];
extern const int ov99_021EA394[GF_GFX_RES_TYPE_MAX];
extern const TouchscreenHitbox ov99_021EA3C4[];
extern const PokeathlonEventRecordDefaultEntry ov99_021EA350;

u16 PokeathlonEventRecord_GetEventTotal(PokeathlonEventRecordData *data, u32 event) {
    GF_ASSERT(data != NULL);
    GF_ASSERT(event < 10);
    return ((u16 *)data->args->unk8)[event];
}

BOOL PokeathlonEventRecord_Init(OverlayManager *man, int *state) {
    PokeathlonEventRecord *work;
    PokeathlonEventRecordArgs *args;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_98), OVY_LOAD_ASYNC);
    args = OverlayManager_GetArgs(man);
    GF_ASSERT(args != NULL);
    Heap_Create(HEAP_ID_3, HEAP_ID_132, 0x30000);
    work = OverlayManager_CreateAndGetData(man, sizeof(PokeathlonEventRecord), HEAP_ID_132);
    MI_CpuFill8(work, 0, sizeof(PokeathlonEventRecord));
    work->heapId = HEAP_ID_132;
    PokeathlonEventRecord_CreateRecordData(work, args);
    PokeathlonUtil_ResetDisplay();
    PokeathlonEventRecord_InitBgs(work);
    PokeathlonEventRecord_LoadBgGraphics(work);
    work->textContext = PokeathlonUtil_CreateTextContext(work->heapId, work->bgConfig, 19, ov99_021EA45C, NARC_msg_msg_0314_bin);
    PokeathlonEventRecord_PrintReturnButton(work);
    PokeathlonEventRecord_ShowEvent(work);
    PokeathlonEventRecord_CreateSprites(work);
    PokeathlonEventRecord_DrawModeScreen(work);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    Main_SetVBlankIntrCB((GFIntrCB)PokeathlonEventRecord_VBlankCallback, work);
    return TRUE;
}

BOOL PokeathlonEventRecord_Exit(OverlayManager *man, int *state) {
    PokeathlonEventRecord *work = OverlayManager_GetData(man);

    PokeathlonEventRecord_FreeRecordData(work->data);
    PokeathlonUtil_FreeSpriteContext(work->spriteContext, work->sprites, 37);
    PokeathlonUtil_FreeTextContext(work->textContext, 19);
    PokeathlonEventRecord_FreeBgs(work);
    PokeathlonUtil_DisableDisplay();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_132);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    return TRUE;
}

BOOL PokeathlonEventRecord_Main(OverlayManager *man, int *state) {
    PokeathlonEventRecord *work = OverlayManager_GetData(man);

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
        if (work->modeSwitchRequested) {
            work->linkMode ^= 1;
            PokeathlonEventRecord_SwitchMode(work);
        }
        if (work->eventChanged) {
            PokeathlonEventRecord_ShowEvent(work);
            PokeathlonEventRecord_ShowEventRecordMons(work);
            work->eventChanged = FALSE;
        }
        if (PokeathlonUtil_CheckExitInput() || PokeathlonEventRecord_HandleInput(work)) {
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
        return TRUE;
    default:
        GF_ASSERT(!_021EA5E4);
        break;
    }
    PokeathlonUtil_DrawSprites(work->spriteContext);
    return FALSE;
}

BOOL PokeathlonEventRecord_RecordReachesThreshold(PokeathlonEventRecord *work, int event) {
    u16 score = PokeathlonEventRecord_GetRecordScore(work->data, event, 0, work->linkMode);
    return PokeathlonUtil_ReachesEventThreshold((u8)event, ov99_021E945C(work->data), score);
}

BOOL PokeathlonEventRecord_SoloRecordReachesThreshold(PokeathlonEventRecord *work, s8 event, u32 column) {
    u16 score = PokeathlonEventRecord_GetRecordScore(work->data, event, 0, FALSE);
    if (work->linkMode) {
        return FALSE;
    }
    return PokeathlonUtil_ReachesEventThreshold((u8)event, column, score);
}

BOOL PokeathlonEventRecord_IsRankEmpty(PokeathlonEventRecord *work, u8 rank) {
    if (PokeathlonEventRecord_GetRecordScore(work->data, work->cursor, rank, work->linkMode) == 0xFFFF) {
        return TRUE;
    }
    return FALSE;
}

void PokeathlonEventRecord_VBlankCallback(PokeathlonEventRecord *work) {
    vu8 *dtcm = (vu8 *)0x027E0000;

    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(work->bgConfig);
    *(vu32 *)(dtcm + 0x3FF8) |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK), DTCM address as a literal
}

void PokeathlonEventRecord_InitBgs(PokeathlonEventRecord *work) {
    work->bgConfig = BgConfig_Alloc(work->heapId);
    {
        GraphicsModes modes = ov99_021EA374;
        SetBothScreensModesAndDisable(&modes);
    }
    {
        PokeathlonEventRecordBgTemplates templates = ov99_021EA4F4;
        int i;
        for (i = 0; i < 6; i++) {
            InitBgFromTemplate(work->bgConfig, ov99_021EA59C[i], &templates.templates[i], GF_BG_TYPE_TEXT);
            BgClearTilemapBufferAndCommit(work->bgConfig, ov99_021EA59C[i]);
            BG_ClearCharDataRange(ov99_021EA59C[i], 32, 0, work->heapId);
        }
    }
    work->narc = NARC_New(NARC_a_1_7_7, work->heapId);
}

void PokeathlonEventRecord_LoadBgGraphics(PokeathlonEventRecord *work) {
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 51, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 52, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 53, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 55, work->bgConfig, GF_BG_LYR_MAIN_2, 1, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 51, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 52, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 54, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 55, work->bgConfig, GF_BG_LYR_SUB_2, 1, 0, TRUE, work->heapId);
}

#pragma force_active on

void PokeathlonEventRecord_DrawEventIcon(PokeathlonEventRecord *work, u8 event, s16 x, s16 y, BOOL grayed, BOOL subScreen) {
    int row;
    u8 bgId = subScreen ? GF_BG_LYR_SUB_2 : GF_BG_LYR_MAIN_2;
    int col;
    int ypos = y;

    for (row = 0; row < 6; row++) {
        for (col = 0; col < 6; col++) {
            u16 tile = grayed * 360 + (col + 1 + event * 6 + row * 60);
            LoadRectToBgTilemapRect(work->bgConfig, bgId, &tile, x + col, ypos, 1, 1);
        }
        ypos++;
    }
    BgCommitTilemapBufferToVram(work->bgConfig, bgId);
}

#pragma force_active reset

void PokeathlonEventRecord_FreeBgs(PokeathlonEventRecord *work) {
    u8 i;

    NARC_Delete(work->narc);
    for (i = 0; i < 6; i++) {
        FreeBgTilemapBuffer(work->bgConfig, ov99_021EA59C[i]);
    }
    Heap_Free(work->bgConfig);
}

void PokeathlonEventRecord_ShowEvent(PokeathlonEventRecord *work) {
    BOOL grayed;

    if (work->linkMode) {
        grayed = TRUE;
    } else if (work->noRecord[work->cursor] == FALSE) {
        grayed = TRUE;
    } else {
        grayed = FALSE;
    }
    PokeathlonEventRecord_PrintEventRecord(work);
    PokeathlonEventRecord_DrawEventIcon(work, (u8)work->cursor, 26, 0, grayed, TRUE);
}

void PokeathlonEventRecord_LoadModePalette(PokeathlonEventRecord *work) {
    u8 palette;
    u32 srcOffset;

    if (work->linkMode) {
        palette = 2;
    } else {
        palette = 1;
    }
    srcOffset = (u8)(palette << 5);
    GfGfxLoader_GXLoadPalWithSrcOffsetFromOpenNarc(work->narc, 51, GF_PAL_LOCATION_MAIN_BG, srcOffset, (enum GFPalSlotOffset)0x20, 0x20, work->heapId);
    GfGfxLoader_GXLoadPalWithSrcOffsetFromOpenNarc(work->narc, 51, GF_PAL_LOCATION_SUB_BG, srcOffset, (enum GFPalSlotOffset)0x20, 0x20, work->heapId);
}

void PokeathlonEventRecord_DrawModeScreen(PokeathlonEventRecord *work) {
    PokeathlonEventRecord_PrintModeTitle(work);
    PokeathlonEventRecord_UpdateEventGrid(work);
}

void PokeathlonEventRecord_SwitchMode(PokeathlonEventRecord *work) {
    int i;
    const u8 *windowY;

    PokeathlonEventRecord_PrintModeTitle(work);
    PokeathlonEventRecord_UpdateEventGrid(work);
    PokeathlonEventRecord_LoadModePalette(work);
    windowY = ov99_021EA35A[0];
    for (i = 0; i < 5; i++, windowY += 2) {
        PokeathlonUtil_ClearAndCopyWindow(work->textContext, i + 7);
        PokeathlonUtil_ClearWindowTilemap(work->textContext, i + 7);
        PokeathlonUtil_SetWindowY(work->textContext, i + 7, windowY[work->linkMode]);
    }
    PokeathlonEventRecord_SetMonIconsVisible(work, FALSE);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
    ManagedSprite_SetDrawFlag(work->sprites[1], FALSE);
    PokeathlonUtil_ClearAndCopyWindow(work->textContext, 3);
    PokeathlonUtil_ClearAndCopyWindow(work->textContext, 6);
    PokeathlonUtil_ClearAndCopyWindow(work->textContext, 17);
    PokeathlonUtil_ClearAndCopyWindow(work->textContext, 18);
    PokeathlonEventRecord_ClearNameWindows(work);
    PokeathlonUtil_PrintMessageAtXY(work->textContext, 2, msg_0314_00063, 1, 1);
    PokeathlonUtil_PrintMessageAtXY(work->textContext, 5, msg_0314_00064, 0, 1);
    work->modeSwitchRequested = FALSE;
}

void PokeathlonEventRecord_PrintReturnButton(PokeathlonEventRecord *work) {
    PokeathlonUtil_PrintAlignedMessageAtY(work->textContext, 4, msg_0314_00000, TRUE, 0, 4);
}

#pragma force_active on

void PokeathlonEventRecord_PrintRecordValue(PokeathlonEventRecord *work, int value, int window, int unused, int y) {
    GF_ASSERT(value != 0xFFFF);
    if (work->cursor == 0) {
        PokeathlonUtil_BufferNumber(work->textContext, (u8)(value / 30), 3, 0);
        PokeathlonUtil_BufferNumber(work->textContext, (u8)((value % 30) * 10 / 30), 1, 1);
    } else if (work->cursor == 6) {
        PokeathlonUtil_BufferNumber(work->textContext, (u32)value >> 10, 3, 0);
        PokeathlonUtil_BufferNumber(work->textContext, ((value % 1024) * 10) / 1024, 1, 1);
    } else {
        PokeathlonUtil_BufferNumber(work->textContext, value, 3, 0);
    }
    PokeathlonUtil_PrintMessageAt(work->textContext, window, work->cursor + msg_0314_00086, 0, (u8)y);
}

#pragma force_active reset

void PokeathlonEventRecord_PrintModeTitle(PokeathlonEventRecord *work) {
    PokeathlonUtil_PrintMessageAtXY(work->textContext, 0, ov99_021EA384[work->linkMode], 1, 0);
    PokeathlonUtil_PrintAlignedMessageAtY(work->textContext, 1, ov99_021EA38C[work->linkMode], TRUE, 0, 4);
}

#pragma force_active on

void PokeathlonEventRecord_PrintEventRecord(PokeathlonEventRecord *work) {
    int i;
    int highlight;
    MessageFormat *msgFormat;

    BufferPokeathlonEventName(PokeathlonUtil_GetMessageFormat(work->textContext), 0, (u8)work->cursor);
    PokeathlonUtil_PrintMessageAtXY(work->textContext, 2, msg_0314_00062, 1, 1);
    PokeathlonUtil_PrintAlignedMessageAtY(work->textContext, 5, work->cursor + msg_0314_00065, FALSE, 1, 0);
    msgFormat = PokeathlonUtil_GetMessageFormat(work->textContext);
    BufferIntegerAsString(msgFormat, 0, PokeathlonEventRecord_GetTimesJoined(work->data, (u8)work->cursor, work->linkMode), 7, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    PokeathlonUtil_PrintMessageAtXY(work->textContext, 3, msg_0314_00054, 1, 0);
    for (i = 0; i < 5; i++) {
        if (i == 0 && work->linkMode == FALSE) {
            highlight = TRUE;
        } else {
            highlight = FALSE;
        }
        if (!PokeathlonEventRecord_IsRankEmpty(work, i)) {
            u32 score = PokeathlonEventRecord_GetRecordScore(work->data, work->cursor, i, work->linkMode);
            PokeathlonEventRecord_PrintRecordValue(work, score, i + 7, work->cursor + 86, highlight);
        } else {
            PokeathlonUtil_PrintAlignedMessageAtY(work->textContext, i + 7, work->cursor + msg_0314_00096, FALSE, (u8)highlight, 0);
        }
    }
    if (work->linkMode) {
        PokeathlonUtil_PrintAlignedMessage(work->textContext, 6, msg_0314_00075, FALSE);
        PokeathlonEventRecord_ClearNameWindows(work);
        for (i = 0; i < 5; i++) {
            if (!PokeathlonEventRecord_IsRankEmpty(work, i)) {
                String *name = PokeathlonEventRecord_GetLinkPlayerName(work->data, (u8)work->cursor, (u8)i);
                PokeathlonUtil_PrintString(work->textContext, i + 12, name, 0, 1);
            }
        }
        return;
    }
    if (work->noRecord[work->cursor]) {
        PokeathlonUtil_ClearAndCopyWindow(work->textContext, 17);
        PokeathlonUtil_ClearAndCopyWindow(work->textContext, 18);
    } else {
        int total = PokeathlonEventRecord_GetEventTotal(work->data, (u8)work->cursor);
        PokeathlonUtil_PrintMessageWithNumber(work->textContext, 17, msg_0314_00107, total, 3, 0);
        PokeathlonUtil_PrintAlignedMessage(work->textContext, 18, msg_0314_00106, FALSE);
    }
    if (work->cursor < 0) {
        PokeathlonUtil_ClearAndCopyWindow(work->textContext, 6);
    }
    if (!ov99_021E945C(work->data)) {
        if (work->noRecord[work->cursor]) {
            PokeathlonUtil_PrintAlignedMessage(work->textContext, 6, msg_0314_00109, FALSE);
        } else {
            PokeathlonUtil_PrintMessageWithPlayerName(work->textContext, 6, msg_0314_00108, 0, PokeathlonEventRecord_GetProfile(work->data));
        }
    } else if (PokeathlonEventRecord_RecordReachesThreshold(work, work->cursor)) {
        PokeathlonUtil_PrintMessageWithPlayerName(work->textContext, 6, msg_0314_00108, 0, PokeathlonEventRecord_GetProfile(work->data));
    } else {
        PokeathlonUtil_PrintAlignedMessage(work->textContext, 6, work->cursor + msg_0314_00076, FALSE);
    }
}

void PokeathlonEventRecord_CreateSprites(PokeathlonEventRecord *work) {
    SpriteResourceCountsListUnion counts = ov99_021EA3AC;

    work->spriteContext = PokeathlonUtil_CreateSpriteContext(work->heapId, &counts, 37);
    PokeathlonEventRecord_LoadSpriteResources(work);
    PokeathlonEventRecord_CreateUiSprites(work);
    PokeathlonEventRecord_CreateEventSprites(work);
    PokeathlonEventRecord_CreateMonIconSprites(work);
    PokeathlonEventRecord_ShowEventRecordMons(work);
}

#pragma force_active reset

void PokeathlonEventRecord_ShowEventRecordMons(PokeathlonEventRecord *work) {
    BOOL showMarker;

    if (work->linkMode) {
        showMarker = FALSE;
    } else if (work->noRecord[work->cursor] == FALSE) {
        showMarker = TRUE;
    } else {
        showMarker = FALSE;
    }
    PokeathlonEventRecord_ShowRecordMonIcons(work);
    ManagedSprite_SetDrawFlag(work->sprites[1], showMarker);
}

void PokeathlonEventRecord_LoadSpriteResources(PokeathlonEventRecord *work) {
    SpriteSystem *spriteSystem = PokeathlonUtil_GetSpriteSystem(work->spriteContext);
    SpriteManager *spriteManager = PokeathlonUtil_GetSpriteManager(work->spriteContext);

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 1, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 0, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 2, TRUE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 3, TRUE, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 12, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 13, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 14, TRUE, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 15, TRUE, 1);
    PokeathlonUtil_LoadMonIconResources(work->spriteContext, ov99_021EA364, 15, 0, TRUE);
}

void PokeathlonEventRecord_CreateUiSprites(PokeathlonEventRecord *work) {
    const ManagedSpriteTemplate *spriteTemplate = ov99_021EA3F4;
    int i = 0;

    do {
        ManagedSprite *sprite = PokeathlonUtil_NewSprite(work->spriteContext, spriteTemplate);
        work->sprites[i] = sprite;
        ManagedSprite_SetAnimateFlag(sprite, TRUE);
        spriteTemplate++;
        i++;
    } while (i < 2);
}

#pragma force_active on

void PokeathlonEventRecord_CreateEventSprites(PokeathlonEventRecord *work) {
    ManagedSpriteTemplate spriteTemplate = { 0 };
    u32 row;
    u32 col;
    u8 event;
    int index;

    for (row = 0; row < 2; row++) {
        for (col = 0; col < 5; col++) {
            event = col + row * 5;
            MI_CpuCopy8(ov99_021EA394, spriteTemplate.resIdList, sizeof(spriteTemplate.resIdList));
            spriteTemplate.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
            spriteTemplate.x = col * 48 + 32;
            spriteTemplate.y = row * 64 + 72;
            spriteTemplate.drawPriority = 0;
            spriteTemplate.animation = 16;
            index = event + 12;
            work->sprites[index] = PokeathlonUtil_NewSprite(work->spriteContext, &spriteTemplate);
            ManagedSprite_SetDrawFlag(work->sprites[index], work->noRecord[event] == FALSE);
            spriteTemplate.drawPriority = 1;
            spriteTemplate.animation = 4;
            index = event + 2;
            work->sprites[index] = PokeathlonUtil_NewSprite(work->spriteContext, &spriteTemplate);
            ManagedSprite_SetDrawFlag(work->sprites[index], PokeathlonEventRecord_SoloRecordReachesThreshold(work, event, 1));
            ManagedSprite_SetAnimateFlag(work->sprites[index], TRUE);
            PokeathlonEventRecord_DrawEventIcon(work, event, (u16)(col * 6 + 1), (u16)(row * 8 + 6), work->noRecord[event] == FALSE, FALSE);
        }
    }
}

void PokeathlonEventRecord_CreateMonIconSprites(PokeathlonEventRecord *work) {
    s16 ypos;
    ManagedSpriteTemplate spriteTemplate = { 0 };
    u32 row = 0;
    u32 col;
    u32 base = 0;
    int xoffset = 0;
    int y = 44;

    for (; row < 5; base += 3, xoffset += 8, y += 32, row++) {
        int x;
        col = 0;
        x = 60;
        ypos = y;
        for (; col < 3; col++, x += 24) {
            u32 index = col + base;
            spriteTemplate.resIdList[GF_GFX_RES_TYPE_CHAR] = index + 2;
            spriteTemplate.resIdList[GF_GFX_RES_TYPE_PLTT] = 2;
            spriteTemplate.resIdList[GF_GFX_RES_TYPE_CELL] = 2;
            spriteTemplate.resIdList[GF_GFX_RES_TYPE_ANIM] = 2;
            spriteTemplate.vram = NNS_G2D_VRAM_TYPE_2DSUB;
            spriteTemplate.x = x + xoffset;
            spriteTemplate.y = ypos;
            spriteTemplate.drawPriority = 3 - col;
            EVENT_RECORD_SPRITE(work, index + 22) = PokeathlonUtil_NewSprite(work->spriteContext, &spriteTemplate);
            ManagedSprite_SetAnimateFlag(EVENT_RECORD_SPRITE(work, index + 22), TRUE);
        }
    }
}

void PokeathlonEventRecord_ShowRecordMonIcons(PokeathlonEventRecord *work) {
    int rank = 0;
    int slot;
    int base = 0;

    for (; rank < 5; rank++, base += 3) {
        if (PokeathlonEventRecord_GetRecordScore(work->data, work->cursor, rank, work->linkMode) != 0xFFFF) {
            for (slot = 0; slot < 3; slot++) {
                PokeathlonRecordMon mon = { 0 };
                int index = slot + base + 22;
                PokeathlonEventRecord_GetRecordMon(work->data, &mon.info, work->cursor, rank, slot, work->linkMode);
                if (mon.info.species) {
                    PokeathlonUtil_SetMonIconSprite(work->spriteContext, work->sprites[index], &mon.info, FALSE, TRUE);
                    ManagedSprite_SetDrawFlag(work->sprites[index], TRUE);
                    if (rank == 0) {
                        ManagedSprite_SetAnimSpeed(work->sprites[index], FX32_CONST(1.5));
                    }
                }
            }
        }
    }
}

#pragma force_active reset

void PokeathlonEventRecord_SetMonIconsVisible(PokeathlonEventRecord *work, BOOL visible) {
    int rank = 0;
    int base = 0;

    do {
        int slot = 0;
        // steps through work->sprites[22 + base + slot] by offsetting the work pointer
        PokeathlonEventRecord *row = (PokeathlonEventRecord *)((u8 *)work + base * sizeof(ManagedSprite *));
        do {
            ManagedSprite_SetDrawFlag(row->sprites[22], visible);
            slot++;
            row = (PokeathlonEventRecord *)((u8 *)row + sizeof(ManagedSprite *));
        } while (slot < 3);
        rank++;
        base += 3;
    } while (rank < 5);
}

void PokeathlonEventRecord_ClearNameWindows(PokeathlonEventRecord *work) {
    int i;

    for (i = 0; i < 5; i++) {
        PokeathlonUtil_ClearAndCopyWindow(work->textContext, i + 12);
    }
}

void PokeathlonEventRecord_UpdateEventGrid(PokeathlonEventRecord *work) {
    u32 row = 0;
    u32 col;
    u32 base = 0;
    s32 y = 6;

    do {
        s32 x = 1;
        s16 ypos;
        col = 0;
        ypos = (u16)y;
        do {
            u8 event = col + base;
            s32 grayed;
            s32 index;
            if (work->linkMode) {
                grayed = TRUE;
            } else if (work->noRecord[event] == FALSE) {
                grayed = TRUE;
            } else {
                grayed = FALSE;
            }
            index = event + 12;
            ManagedSprite_SetDrawFlag(work->sprites[index], work->linkMode ? FALSE : grayed);
            ManagedSprite_SetDrawFlag(work->sprites[2 + event], PokeathlonEventRecord_SoloRecordReachesThreshold(work, event, 1));
            PokeathlonEventRecord_DrawEventIcon(work, event, (u16)x, ypos, grayed, FALSE);
            x += 6;
            col++;
        } while (col < 5);
        base += 5;
        y += 8;
        row++;
    } while (row < 2);
}

#pragma force_active on

BOOL PokeathlonEventRecord_HandleInput(PokeathlonEventRecord *work) {
    int oldCursor = work->cursor;

    if (gSystem.touchNew) {
        int hit = TouchscreenHitbox_FindHitboxAtTouchNew(ov99_021EA3C4);
        if (hit != -1) {
            work->cursor = hit - 1;
            ManagedSprite_SetAnim(work->sprites[0], 11);
            PlaySE(SEQ_SE_DP_SELECT);
            if (work->cursor == -1) {
                work->modeSwitchRequested = TRUE;
                ManagedSprite_SetAnim(work->sprites[0], 13);
                PlaySE(SEQ_SE_DP_SELECT);
            }
            ManagedSprite_SetAnimateFlag(work->sprites[0], TRUE);
        }
    } else {
        if (gSystem.newKeys & PAD_BUTTON_A) {
            if (oldCursor == 10) {
                return TRUE;
            }
            if (oldCursor == -1) {
                work->modeSwitchRequested = TRUE;
                PlaySE(SEQ_SE_DP_SELECT);
            }
        }
        if (work->cursor == -1) {
            if (gSystem.newAndRepeatedKeysRaw & PAD_KEY_DOWN) {
                work->cursor = work->lastColumn;
            }
        } else if (work->cursor == 10) {
            if (gSystem.newAndRepeatedKeysRaw & PAD_KEY_UP) {
                work->cursor = work->lastColumn + 5;
            }
        } else {
            if (gSystem.newAndRepeatedKeysRaw & PAD_KEY_RIGHT) {
                work->cursor++;
            } else if (gSystem.newAndRepeatedKeysRaw & PAD_KEY_LEFT) {
                work->cursor--;
            } else if (gSystem.newAndRepeatedKeysRaw & PAD_KEY_DOWN) {
                work->cursor += 5;
            } else if (gSystem.newAndRepeatedKeysRaw & PAD_KEY_UP) {
                int step = work->cursor == 10 ? 1 : 5;
                work->cursor -= step;
            }
            {
                int cursor = work->cursor;
                if (cursor > 10) {
                    cursor = 10;
                } else if (cursor < -1) {
                    cursor = -1;
                }
                work->cursor = cursor;
            }
            if (work->cursor != -1 && work->cursor != 10) {
                work->lastColumn = work->cursor % 5;
            }
        }
    }
    if (oldCursor != work->cursor) {
        PlaySE(SEQ_SE_DP_SELECT);
        if (work->cursor == -1) {
            ManagedSprite_SetPositionXY(work->sprites[0], 224, 16);
            ManagedSprite_SetAnim(work->sprites[0], 12);
        } else if (work->cursor == 10) {
            ManagedSprite_SetPositionXY(work->sprites[0], 224, 176);
            ManagedSprite_SetAnim(work->sprites[0], 12);
        } else {
            int row = work->cursor >= 5 ? 1 : 0;
            s8 col = work->cursor;
            if (row) {
                col -= 5;
            }
            ManagedSprite_SetPositionXY(work->sprites[0], col * 48 + 32, row * 64 + 72);
            if (ManagedSprite_GetActiveAnim(work->sprites[0]) != 11) {
                ManagedSprite_SetAnim(work->sprites[0], 10);
            }
            if (oldCursor != 10 || work->cursor != 9) {
                PokeathlonEventRecord_SetMonIconsVisible(work, FALSE);
                work->eventChanged = TRUE;
            }
        }
    }
    return FALSE;
}

void PokeathlonEventRecord_CopyRecords(PokeathlonEventRecord *work, const PokeathlonEventRecords *src, PokeathlonEventRecords *dst) {
    int event;
    int rank;
    int i;
    PokeathlonEventRecordTeam team;
    PokeathlonEventRecordTeam defaultTeam;

    MI_CpuCopy8(src, dst, sizeof(PokeathlonEventRecords) * 10);
    event = 0;
    defaultTeam = ov99_021EA350.team;
    for (; event < 10; event++) {
        for (rank = 0; rank < 5; rank++) {
            if (!PokeathlonUtil_BeatsEventThreshold((u8)event, 0, src[event].entries[rank].score)) {
                break;
            }
        }
        if (rank == 0) {
            work->noRecord[event] = TRUE;
        }
        for (i = 0; i < 5; i++) {
            if (i == rank) {
                team = defaultTeam;
                MI_CpuCopy8(&team, &dst[event].entries[i].team, sizeof(PokeathlonEventRecordTeam));
                // called with a u16 event index (pokeathlon_util.h declares it u32), truncated at the call
                dst[event].entries[i].score = ((u16(*)(u16, u32))PokeathlonUtil_GetEventThreshold)(event, 0);
            } else if (i > rank) {
                dst[event].entries[i] = src[event].entries[i - 1];
            }
        }
    }
}

#pragma force_active reset

void PokeathlonEventRecord_CreateRecordData(PokeathlonEventRecord *work, PokeathlonEventRecordArgs *args) {
    PokeathlonEventRecordData *data = Heap_Alloc(work->heapId, sizeof(PokeathlonEventRecordData));

    MI_CpuFill8(data, 0, sizeof(PokeathlonEventRecordData));
    data->args = args;
    PokeathlonEventRecord_CopyRecords(work, (const PokeathlonEventRecords *)args->recordsSolo, data->records);
    data->string = String_New(8, work->heapId);
    work->data = data;
}

void PokeathlonEventRecord_FreeRecordData(PokeathlonEventRecordData *data) {
    String_Delete(data->string);
    Heap_Free(data);
}

PokeathlonEventRecords *PokeathlonEventRecord_GetRecords(PokeathlonEventRecordData *data, u32 event, BOOL link) {
    GF_ASSERT(event < 10);
    if (link) {
        return (PokeathlonEventRecords *)&data->args->recordsLink[event];
    }
    return &data->records[event];
}

BOOL ov99_021E945C(PokeathlonEventRecordData *data) {
    return data->args->unk10;
}

#pragma force_active on

void PokeathlonEventRecord_GetRecordMon(PokeathlonEventRecordData *data, PokeathlonMonInfo *mon, int event, int rank, int slot, BOOL link) {
    int value;
    PokeathlonEventRecordEntry *records;
    PokeathlonEventRecordEntry *entry;

    GF_ASSERT(data != NULL);
    GF_ASSERT(rank < 5);
    GF_ASSERT(slot < 3);
    records = PokeathlonEventRecord_GetRecords(data, (u8)event, link)->entries;
    entry = &records[rank];
    value = entry->team.mons[slot];
    mon->species = value & 0x3FF;
    mon->form = value >> 10;
}

#pragma force_active reset

u16 PokeathlonEventRecord_GetRecordScore(PokeathlonEventRecordData *data, int event, u8 rank, BOOL link) {
    GF_ASSERT(data != NULL);
    GF_ASSERT((u32)rank < 5);
    return PokeathlonUtil_GetRecordScore(PokeathlonEventRecord_GetRecords(data, (u8)event, link), rank);
}

u32 PokeathlonEventRecord_GetTimesJoined(PokeathlonEventRecordData *data, u32 event, BOOL link) {
    return PokeathlonEventRecord_GetRecords(data, event, link)->timesJoined;
}

PlayerProfile *PokeathlonEventRecord_GetProfile(PokeathlonEventRecordData *data) {
    GF_ASSERT(data != NULL);
    return data->args->profile;
}

String *PokeathlonEventRecord_GetLinkPlayerName(PokeathlonEventRecordData *data, int event, int rank) {
    u8 *records;
    String *string;

    GF_ASSERT(data != NULL);
    records = (u8 *)&data->args->recordsLink[event];
    string = data->string;
    // link records store each rank's player name 0x18 bytes apart, from offset 0x30
    CopyU16ArrayToString(string, (const u16 *)(records + 0x30 + rank * 0x18));
    return data->string;
}
