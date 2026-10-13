#include "pokeathlon/pokeathlon_event_intro.h"

#include "global.h"

#include "constants/flags.h"
#include "constants/sndseq.h"

#include "pokeathlon/pokeathlon_save.h"

#include "assert.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "palette.h"
#include "poke_overlay.h"
#include "save_vars_flags.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "system.h"
#include "touchscreen.h"
#include "wireless_status_icon.h"

FS_EXTERN_OVERLAY(OVY_98);

// Not declared by a header yet.
void SetBothScreensBrightnessByColor(u16 color);
void GF_SndReadSeqVariable(u16 seqNo, u32 variable, s16 *value);

// Trainer class -> overworld model of the NPC trainer, as two parallel columns of one table.
extern const struct {
    int trainerClass;
    int unused;
} ov96_0221B1D4[];

extern const struct {
    int fileId;
    int unused;
} ov96_0221B1D8[];

extern const GraphicsBanks ov96_0221B9E8;
extern const GraphicsModes ov96_0221B9BC;
extern const BgTemplate ov96_0221B9CC;
extern const GraphicsBanks ov96_0221BAC0;
extern const u8 ov96_0221BA18[7];
extern const u8 ov96_0221BA10[7];
extern const GraphicsModes ov96_0221BA44;

// Copied as a whole (one block copy).
typedef struct EventIntroBgTemplates {
    BgTemplate templates[7];
} EventIntroBgTemplates;

extern const EventIntroBgTemplates ov96_0221BB98;
extern const WindowTemplate ov96_0221BB38[12];
extern const SpriteResourceCountsListUnion ov96_0221BA68;
extern const OamManagerParam ov96_0221BAA0;
extern const OamCharTransferParam ov96_0221BA54;
extern ManagedSpriteTemplate ov96_0221DA80[4];
extern const s16 ov96_0221BA80[8][2];
extern const int ov96_0221BAE8[][2];
extern const TouchscreenHitbox ov96_0221BA20[];
extern const TouchscreenHitbox ov96_0221BA2C[];
extern const TouchscreenHitbox ov96_0221BA38[];

void PokeathlonCourse_LoadSpritePalette(ManagedSprite *sprite, NarcId narcId, int fileId, BOOL mainScreen, BOOL grayScale, enum HeapID heapId) {
    int vramType;
    int location;
    u32 offset;

    if (mainScreen) {
        vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
        location = GF_PAL_LOCATION_MAIN_OBJ;
    } else {
        vramType = NNS_G2D_VRAM_TYPE_2DSUB;
        location = GF_PAL_LOCATION_SUB_OBJ;
    }
    offset = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(sprite->sprite), (NNS_G2D_VRAM_TYPE)vramType);
    GfGfxLoader_GXLoadPal(narcId, fileId, (enum GFPalLoadLocation)location, (enum GFPalSlotOffset)offset, 0x20, heapId);
    if (grayScale) {
        void *palette;
        if (mainScreen) {
            palette = (void *)HW_OBJ_PLTT;
        } else {
            palette = (void *)HW_DB_OBJ_PLTT;
        }
        TintPalette_GrayScale((u16 *)(offset + (u32)palette), 0x20);
    }
}

int PokeathlonCourse_GetNpcTrainerPicFileId(int trainerClass) {
    int i;

    for (i = 0; i < 9999; i++) {
        if (i == 0xFFFF) {
            return 0;
        }
        if (trainerClass == ov96_0221B1D4[i].trainerClass) {
            return ov96_0221B1D8[i].fileId;
        }
    }
    GF_AssertFail();
    return 0;
}

BOOL PokeathlonCourseWait_Init(PokeathlonCourseData *course) {
    PokeathlonCourseWait *wait = PokeathlonCourse_GetHeapAllocPtr4(course);

    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0:
        PokeathlonCourseWait_Create(course);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 1:
        WirelessStatusIcon_CreateOnScreenIfConnected(1);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 4, 1, wait->heapId);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            PokeathlonCourseMessageBox_Print(wait->messageBox, 310);
            PokeathlonCourse_IncrementField1ED(course);
        }
        break;
    case 3:
        if (!PokeathlonCourseMessageBox_Update(wait->messageBox)) {
            PokeathlonCourseMessageBox_ShowWaitingIcon(wait->messageBox);
            PokeathlonCourse_SetStateTransitionType(course, 0x11);
            return TRUE;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonCourseWait_Main(void) {
    return TRUE;
}

BOOL PokeathlonCourseWait_Exit(PokeathlonCourseData *course) {
    PokeathlonCourseWait_Free(course);
    return TRUE;
}

void PokeathlonCourseWait_SetBanks(void) {
    GraphicsBanks banks = ov96_0221B9E8;
    GfGfx_SetBanks(&banks);
}

void PokeathlonCourseWait_VBlankCallback(void *data) {
    PokeathlonCourseWait *wait = data;
    vu32 *dtcm = (vu32 *)0x027E0000;
    DoScheduledBgGpuUpdates(wait->bgConfig);
    dtcm[0x3FF8 / sizeof(u32)] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag
}

void PokeathlonCourseWait_InitBgs(BgConfig *bgConfig, enum HeapID heapId) {
    GraphicsModes modes = ov96_0221B9BC;
    SetBothScreensModesAndDisable(&modes);
    {
        BgTemplate template = ov96_0221B9CC;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x40, 0, heapId);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
        BG_SetMaskColor(GF_BG_LYR_MAIN_0, 0);
        BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);
    }
}

void PokeathlonCourseWait_FreeBgs(PokeathlonCourseWait *wait) {
    FreeBgTilemapBuffer(wait->bgConfig, GF_BG_LYR_MAIN_0);
    Heap_Free(wait->bgConfig);
}

#pragma opt_propagation off

#pragma force_active on

void PokeathlonCourseWait_Create(PokeathlonCourseData *course) {
    PokeathlonCourseWait *wait;
    enum HeapID heapId = HEAP_ID_159;
    Heap_Create(HEAP_ID_POKEATHLON, heapId, 0x40000);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= ~0x1F00;
    reg_GXS_DB_DISPCNT &= ~0x1F00;
    PokeathlonCourseWait_SetBanks();
    wait = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(PokeathlonCourseWait));
    MI_CpuFill8(wait, 0, sizeof(PokeathlonCourseWait));
    wait->heapId = HEAP_ID_159;
    wait->bgConfig = BgConfig_Alloc(HEAP_ID_159);
    PokeathlonCourseWait_InitBgs(wait->bgConfig, wait->heapId);
    wait->messageBox = PokeathlonCourseMessageBox_New(wait->heapId);
    PokeathlonCourseMessageBox_Init(wait->messageBox, wait->bgConfig, GF_BG_LYR_MAIN_0, PokeathlonCourse_GetMode(course), PokeathlonCourse_GetSaveData(course));
    Main_SetVBlankIntrCB(PokeathlonCourseWait_VBlankCallback, wait);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
}

#pragma force_active reset

#pragma opt_propagation reset

void PokeathlonCourseWait_Free(PokeathlonCourseData *course) {
    PokeathlonCourseWait *wait = PokeathlonCourse_GetHeapAllocPtr4(course);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    WirelessStatusIcon_Destroy();
    PokeathlonCourseMessageBox_Free(wait->messageBox);
    PokeathlonCourseWait_FreeBgs(wait);
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    Heap_Destroy(HEAP_ID_159);
}

BOOL PokeathlonCourseWait_FadeOut(PokeathlonCourseData *course, u8 *state) {
    PokeathlonCourseWait *wait = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (*state) {
    case 0:
        if (++wait->timer > 15) {
            PokeathlonCourseMessageBox_HideWaitingIcon(wait->messageBox);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 4, 1, wait->heapId);
            (*state)++;
        }
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            PokeathlonCourseMessageBox_Close(wait->messageBox);
            return TRUE;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonEventIntro_Init(PokeathlonCourseData *course) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0:
        PokeathlonEventIntro_Create(course);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 1:
        PokeathlonEventIntro_InitSprites(intro);
        PokeathlonEventIntro_InitBgs(intro->bgConfig, intro->heapId);
        intro->banner = PokeathlonEventBanner_New(intro->event, intro->heapId);
        PokeathlonEventBanner_AddWindows(intro->banner, intro->bgConfig);
        PokeathlonEventIntro_LoadGraphics(intro);
        PokeathlonEventBanner_LoadGraphics(intro->banner);
        PokeathlonEventIntro_HideZoomBgs(intro);
        WirelessStatusIcon_CreateOnScreenIfConnected(2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        PokeathlonCourse_SetField1F4(course, 1);
        PokeathlonCourse_SetField1ED(course, 0);
        return TRUE;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonEventIntro_Main(PokeathlonCourseData *course) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetHeapAllocPtr4(course);
    SpriteSystem_DrawSprites(intro->spriteManager);
    return TRUE;
}

BOOL PokeathlonEventIntro_Exit(PokeathlonCourseData *course) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i;

    NARC_Delete(intro->narc);
    WirelessStatusIcon_Destroy();
    PokeathlonEventBanner_Free(intro->banner);
    PokeathlonUtil_FreeTextContext(intro->text, 12);
    for (i = 0; i < 7; i++) {
        FreeBgTilemapBuffer(intro->bgConfig, ov96_0221BA18[i]);
    }
    Heap_Free(intro->bgConfig);
    for (i = 0; i < 4; i++) {
        if (intro->sprites[i] != NULL) {
            Sprite_DeleteAndFreeResources(intro->sprites[i]);
            intro->sprites[i] = NULL;
        }
    }
    SpriteSystem_FreeResourcesAndManager(intro->spriteSystem, intro->spriteManager);
    SpriteSystem_Free(intro->spriteSystem);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    Heap_Destroy(HEAP_ID_136);
    return TRUE;
}

void PokeathlonEventIntro_VBlankCallback(PokeathlonEventIntro *intro) {
    vu32 *dtcm = (vu32 *)0x027E0000;

    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(intro->bgConfig);
    dtcm[0x3FF8 / sizeof(u32)] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag
}

void PokeathlonEventIntro_SetBanks(void) {
    GraphicsBanks banks = ov96_0221BAC0;
    GfGfx_SetBanks(&banks);
}

// Whether the sequence variable of the playing BGM has reached value (the screen follows the music).
BOOL PokeathlonEventIntro_IsBgmAt(u32 variable, s16 value) {
    s16 current;
    GF_SndReadSeqVariable(GF_GetCurrentPlayingBGM(), variable, &current);
    return current >= value;
}

u8 PokeathlonEventIntro_GetEvent(PokeathlonCourseData *course) {
    return PokeathlonCourse_GetField3D8_ForCurrentParticipant(course);
}

BOOL PokeathlonEventIntro_HasFlag0EF(PokeathlonCourseData *course) {
    SaveData *saveData = PokeathlonCourse_GetSaveData(course);
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(saveData);
    return Save_VarsFlags_CheckFlagInArray(varsFlags, FLAG_UNK_0EF);
}

#pragma opt_propagation off

#pragma force_active on

void PokeathlonEventIntro_Create(PokeathlonCourseData *course) {
    PokeathlonEventIntro *intro;
    enum HeapID heapId;
    PokeathlonCourse_GetHeapAllocPtr4(course);
    Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_136, 0x40000);
    heapId = HEAP_ID_136;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_98), OVY_LOAD_ASYNC);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    PokeathlonEventIntro_SetBanks();
    intro = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(PokeathlonEventIntro));
    MI_CpuFill8(intro, 0, sizeof(PokeathlonEventIntro));
    intro->narc = NARC_New(NARC_a_2_1_9, heapId);
    intro->bgConfig = BgConfig_Alloc(heapId);
    intro->event = PokeathlonEventIntro_GetEvent(course);
    intro->round = PokeathlonCourse_GetCurrentParticipantIndex(course);
    intro->linkMode = PokeathlonCourse_GetMode(course) == 1 ? TRUE : FALSE;
    intro->course = course;
    intro->heapId = HEAP_ID_136;
    Main_SetVBlankIntrCB((GFIntrCB)PokeathlonEventIntro_VBlankCallback, intro);
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    SetBothScreensBrightnessByColor(0x7FFF);
    if (intro->linkMode) {
        intro->showRecord = FALSE;
    } else {
        intro->showRecord = PokeathlonEventIntro_HasFlag0EF(course);
    }
}

#pragma force_active reset

#pragma opt_propagation reset

void PokeathlonEventIntro_LoadDetailsBg(PokeathlonEventIntro *intro) {
    LoadFontPal0(GF_PAL_LOCATION_SUB_BGEXT, (enum GFPalSlotOffset)0x4000, intro->heapId);
    PokeathlonEventBanner_LoadGraphics(intro->banner);
    GfGfxLoader_GXLoadPalFromOpenNarc(intro->narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, intro->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(intro->narc, 5, intro->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, intro->heapId);
    if (intro->showRecord != FALSE) {
        GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 10, intro->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, intro->heapId);
    } else {
        GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 9, intro->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, intro->heapId);
    }
}

// Picture of a rules / controls page of the event on sub BG 3.
void PokeathlonEventIntro_LoadPagePicture(PokeathlonEventIntro *intro, int event) {
    MtxFx22 matrix;

    GF_ASSERT(event < 10);
    GfGfxLoader_GXLoadPalFromOpenNarc(intro->narc, event + 32, GF_PAL_LOCATION_SUB_BGEXT, (enum GFPalSlotOffset)0x6000, 0, intro->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(intro->narc, event + 42, intro->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, intro->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, event + 52, intro->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, intro->heapId);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);
    matrix._00 = FX32_ONE;
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = FX32_ONE;
    OS_WaitVBlankIntr();
    SetBgAffine(intro->bgConfig, GF_BG_LYR_SUB_3, &matrix, 0, 0);
    Bg_SetTextDimAndAffineParams(intro->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 0, &matrix, 0, 0);
}

void PokeathlonEventIntro_InitBgs(BgConfig *bgConfig, enum HeapID heapId) {
    EventIntroBgTemplates templates;
    GraphicsModes modes;
    int i;

    modes = ov96_0221BA44;
    SetBothScreensModesAndDisable(&modes);
    templates = ov96_0221BB98;
    for (i = 0; i < 7; i++) {
        InitBgFromTemplate(bgConfig, ov96_0221BA18[i], &templates.templates[i], ov96_0221BA10[i]);
        BgClearTilemapBufferAndCommit(bgConfig, ov96_0221BA18[i]);
        BG_ClearCharDataRange(ov96_0221BA18[i], 0x40, 0, heapId);
    }
}

void PokeathlonEventIntro_LoadGraphics(PokeathlonEventIntro *intro) {
    GfGfxLoader_GXLoadPalFromOpenNarc(intro->narc, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, intro->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(intro->narc, 5, intro->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, intro->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 6, intro->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, intro->heapId);
    if (intro->linkMode == FALSE) {
        GfGfxLoader_LoadCharDataFromOpenNarc(intro->narc, 5, intro->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, intro->heapId);
        GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 8, intro->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, intro->heapId);
    }
    PokeathlonEventIntro_LoadDetailsBg(intro);
    intro->text = PokeathlonUtil_CreateTextContext(intro->heapId, intro->bgConfig, 12, ov96_0221BB38, 309);
    PokeathlonEventIntro_PrintDetails(intro);
}

// Best solo record of the event.
u16 PokeathlonEventIntro_GetRecord(PokeathlonEventIntro *intro, int event) {
    Pokeathlon_RecordsSolo *records = PokeathlonSave_GetRecordsSolo2(Save_Pokeathlon_Get(PokeathlonCourse_GetSaveData(intro->course)));
    return records[event].entries[0].unk_0;
}

#pragma force_active on

void PokeathlonEventIntro_PrintDetails(PokeathlonEventIntro *intro) {
    int event = intro->event;
    u8 number = intro->round + 1;
    if (!intro->showRecord) {
        PokeathlonUtil_SetWindowY(intro->text, 5, 5);
    }
    PokeathlonUtil_PrintAlignedMessage(intro->text, 0, 197, FALSE);
    PokeathlonUtil_PrintAlignedMessage(intro->text, 2, 208, FALSE);
    PokeathlonUtil_PrintMessageAtXY(intro->text, 1, event + 198, 0, 0);
    PokeathlonUtil_PrintMessageAtXY(intro->text, 3, event * 3 + 209, 0, 0);
    PokeathlonUtil_PrintMessageWithNumber(intro->text, 5, 239, number, 1, 0);
    if (!intro->linkMode) {
        PokeathlonUtil_PrintAlignedMessageAtY(intro->text, 4, 240, FALSE, 0, 4);
    }
    PokeathlonEventBanner_PrintTexts(intro->banner);
    if (intro->showRecord) {
        int record = PokeathlonEventIntro_GetRecord(intro, intro->event);
        if (record == 0xFFFF) {
            record = 0;
        }
        if (intro->event == 0) {
            // seconds and tenths (30 frames a second)
            MessageFormat *msgFormat = PokeathlonUtil_GetMessageFormat(intro->text);
            BufferIntegerAsString(msgFormat, 0, record / 30, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
            BufferIntegerAsString(msgFormat, 1, (record % 30) * 10 / 30, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
        } else if (intro->event == 6) {
            // fx22.10
            PokeathlonUtil_BufferNumber(intro->text, (u32)record >> 10, 2, 0);
            PokeathlonUtil_BufferNumber(intro->text, (record % 1024) * 10 / 1024, 1, 1);
        } else {
            PokeathlonUtil_BufferNumber(intro->text, record, 3, 0);
        }
        PokeathlonUtil_PrintAlignedMessage(intro->text, 6, 176, FALSE);
        PokeathlonUtil_PrintMessage(intro->text, 7, event + 177);
    }
}

#pragma force_active reset

void PokeathlonEventIntro_PrintRules(PokeathlonEventIntro *intro) {
    int msgId = intro->event * 3 + 210;
    PokeathlonUtil_PrintAlignedMessage(intro->text, 8, 244, FALSE);
    PokeathlonUtil_PrintAlignedMessageAtY(intro->text, 9, 242, FALSE, 0, 4);
    PokeathlonUtil_PrintAlignedMessageAtY(intro->text, 10, 243, FALSE, 0, 4);
    PokeathlonUtil_PrintMessageAtXY(intro->text, 11, msgId, 0, 0);
}

void PokeathlonEventIntro_PrintControls(PokeathlonEventIntro *intro) {
    int msgId = intro->event * 3 + 211;
    PokeathlonUtil_PrintAlignedMessage(intro->text, 8, 245, FALSE);
    PokeathlonUtil_PrintAlignedMessageAtY(intro->text, 9, 241, FALSE, 0, 4);
    PokeathlonUtil_PrintAlignedMessageAtY(intro->text, 10, 243, FALSE, 0, 4);
    PokeathlonUtil_PrintMessageAtXY(intro->text, 11, msgId, 0, 0);
}

#pragma force_active on

void PokeathlonEventIntro_InitSprites(PokeathlonEventIntro *intro) {
    SpriteResourceCountsListUnion counts = ov96_0221BA68;
    OamManagerParam oamManagerParam = ov96_0221BAA0;
    OamCharTransferParam charTransferParam = ov96_0221BA54;
    charTransferParam.maxTasks = 4;
    intro->spriteSystem = SpriteSystem_Alloc(intro->heapId);
    intro->spriteManager = SpriteManager_New(intro->spriteSystem);
    SpriteSystem_Init(intro->spriteSystem, &oamManagerParam, &charTransferParam, 32);
    SpriteSystem_InitSprites(intro->spriteSystem, intro->spriteManager, 4);
    SpriteSystem_InitManagerWithCapacities(intro->spriteSystem, intro->spriteManager, &counts);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(intro->spriteSystem), 0, 2 << 20);
    PokeathlonEventIntro_LoadSpriteResources(intro);
    PokeathlonEventIntro_CreateSprites(intro);
}

#pragma force_active reset

void PokeathlonEventIntro_LoadSpriteResources(PokeathlonEventIntro *intro) {
    SpriteSystem *spriteSystem = intro->spriteSystem;
    SpriteManager *spriteManager = intro->spriteManager;

    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, intro->narc, 0, FALSE, 16, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, intro->narc, 1, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, intro->narc, 2, TRUE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, intro->narc, 3, TRUE, 0);
}

void PokeathlonEventIntro_UpdateMascot(PokeathlonEventIntro *intro) {
    if (!ManagedSprite_IsAnimated(intro->sprites[2])) {
        intro->animStep++;
        if (intro->animStep >= 2) {
            intro->animStep = 0;
        }
        PokeathlonEventIntro_SetMascotAnim(intro);
    }
}

#pragma force_active on

void PokeathlonEventIntro_PlaceSprites(PokeathlonEventIntro *intro, int layout) {
    int i = 0;
    const s16(*position)[2] = ov96_0221BA80 + layout * 4;
    do {
        ManagedSprite_SetPositionXYWithSubscreenOffset(intro->sprites[i], (*position)[0], (*position)[1], 2 << 20);
        position++;
    } while (++i < 4);
}

#pragma force_active reset

void PokeathlonEventIntro_ResetMascot(PokeathlonEventIntro *intro) {
    intro->animStep = 0;
    PokeathlonEventIntro_SetMascotAnim(intro);
}

// Places the templates (in .data) at the positions of layout 0 and creates the sprites. The
// template is written through one pointer and read through another, as the original did.
#pragma force_active on

void PokeathlonEventIntro_CreateSprites(PokeathlonEventIntro *intro) {
    const s16(*position)[2] = ov96_0221BA80;
    int i = 0;
    s16 *coords = &ov96_0221DA80[0].x;
    PokeathlonEventIntro *cursor = intro;
    ManagedSpriteTemplate *template = ov96_0221DA80;
    for (; i < 4; i++) {
        SpriteSystem *spriteSystem = intro->spriteSystem;
        SpriteManager *spriteManager = intro->spriteManager;
        coords[0] = (*position)[0];
        coords[1] = (*position)[1];
        cursor->sprites[0] = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, template, 2 << 20);
        ManagedSprite_SetAnimateFlag(cursor->sprites[0], TRUE);
        position++;
        coords += sizeof(ManagedSpriteTemplate) / sizeof(s16);
        template ++;
        cursor = (PokeathlonEventIntro *)((u8 *)cursor + sizeof(ManagedSprite *));
    }
    ManagedSprite_SetAnimateFlag(intro->sprites[2], FALSE);
    PokeathlonEventIntro_SetMascotAnim(intro);
}

void PokeathlonEventIntro_SetMascotAnim(PokeathlonEventIntro *intro) {
    ManagedSprite_SetAnim(intro->sprites[2], ov96_0221BAE8[intro->event][intro->animStep]);
}

#pragma force_active reset

void PokeathlonEventIntro_HideZoomBgs(PokeathlonEventIntro *intro) {
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

void PokeathlonEventIntro_StartZoom(PokeathlonEventIntro *intro) {
    int offset = 0;
    MtxFx22 matrix;

    if (intro->showRecord == FALSE) {
        offset = 32;
    }
    matrix._00 = 0x125;
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = 0x125;
    OS_WaitVBlankIntr();
    SetBgAffine(intro->bgConfig, GF_BG_LYR_SUB_3, &matrix, 128, 96);
    SetBgAffine(intro->bgConfig, GF_BG_LYR_SUB_2, &matrix, 128, 96);
    Bg_SetTextDimAndAffineParams(intro->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SUB_Y, offset, &matrix, 128, 96);
    Bg_SetTextDimAndAffineParams(intro->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SUB_Y, offset, &matrix, 128, 96);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    intro->timer = 0;
    intro->zooming = TRUE;
}

void PokeathlonEventIntro_UpdateZoom(PokeathlonEventIntro *intro) {
    MtxFx22 matrix;
    fx32 scale = (++intro->timer << FX32_SHIFT) / 14;

    matrix._00 = scale;
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = scale;
    OS_WaitVBlankIntr();
    SetBgAffine(intro->bgConfig, GF_BG_LYR_SUB_3, &matrix, 128, 96);
    SetBgAffine(intro->bgConfig, GF_BG_LYR_SUB_2, &matrix, 128, 96);
    if (intro->timer >= 14) {
        intro->timer = 0;
        intro->zooming = FALSE;
    }
}

void PokeathlonEventIntro_FadeToPage(PokeathlonCourseData *course, int page) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetHeapAllocPtr4(course);

    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFF, 4, 1, intro->heapId);
    intro->page = page;
}

BOOL PokeathlonEventIntro_IsFadeFinished(PokeathlonCourseData *course) {
    return IsPaletteFadeFinished();
}

void PokeathlonEventIntro_ShowDetails(PokeathlonEventIntro *intro) {
    MtxFx22 matrix;
    int y;

    GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 6, intro->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, intro->heapId);
    PokeathlonEventIntro_LoadDetailsBg(intro);
    PokeathlonEventIntro_PlaceSprites(intro, 0);
    ManagedSprite_SetDrawFlag(intro->sprites[0], TRUE);
    ManagedSprite_SetDrawFlag(intro->sprites[1], TRUE);
    ManagedSprite_SetDrawFlag(intro->sprites[2], TRUE);
    ManagedSprite_SetDrawFlag(intro->sprites[3], TRUE);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_SUB_0, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_ON);
    PokeathlonEventIntro_PrintDetails(intro);
    y = 0;
    if (intro->showRecord == FALSE) {
        y = -32;
    }
    matrix._00 = FX32_ONE;
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = FX32_ONE;
    OS_WaitVBlankIntr();
    SetBgAffine(intro->bgConfig, GF_BG_LYR_SUB_3, &matrix, 0, 0);
    Bg_SetTextDimAndAffineParams(intro->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, y, &matrix, 0, 0);
}

void PokeathlonEventIntro_ShowRules(PokeathlonEventIntro *intro) {
    PokeathlonEventIntro_LoadPagePicture(intro, intro->event);
    GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 7, intro->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, intro->heapId);
    PokeathlonEventIntro_PlaceSprites(intro, 1);
    ManagedSprite_SetDrawFlag(intro->sprites[0], TRUE);
    ManagedSprite_SetDrawFlag(intro->sprites[1], FALSE);
    ManagedSprite_SetDrawFlag(intro->sprites[2], FALSE);
    ManagedSprite_SetDrawFlag(intro->sprites[3], FALSE);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_SUB_0, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_OFF);
    PokeathlonEventIntro_PrintRules(intro);
}

void PokeathlonEventIntro_ShowControls(PokeathlonEventIntro *intro) {
    PokeathlonEventIntro_LoadPagePicture(intro, intro->event);
    GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 7, intro->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, intro->heapId);
    PokeathlonEventIntro_PlaceSprites(intro, 1);
    ManagedSprite_SetDrawFlag(intro->sprites[0], FALSE);
    ManagedSprite_SetDrawFlag(intro->sprites[1], TRUE);
    ManagedSprite_SetDrawFlag(intro->sprites[2], TRUE);
    ManagedSprite_SetDrawFlag(intro->sprites[3], FALSE);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_SUB_0, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_OFF);
    PokeathlonEventIntro_PrintControls(intro);
}

void PokeathlonEventIntro_ShowPage(PokeathlonCourseData *course) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetHeapAllocPtr4(course);

    BgClearTilemapBufferAndCommit(intro->bgConfig, GF_BG_LYR_MAIN_0);
    BgClearTilemapBufferAndCommit(intro->bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(intro->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(intro->bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(intro->bgConfig, GF_BG_LYR_SUB_2);
    BgClearTilemapBufferAndCommit(intro->bgConfig, GF_BG_LYR_SUB_3);
    PokeathlonEventIntro_ResetMascot(intro);
    switch (intro->page) {
    case EVENT_INTRO_PAGE_DETAILS:
        PokeathlonEventIntro_ShowDetails(intro);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFF, 4, 1, intro->heapId);
        break;
    case EVENT_INTRO_PAGE_RULES:
        PokeathlonEventIntro_ShowRules(intro);
        break;
    case EVENT_INTRO_PAGE_CONTROLS:
        PokeathlonEventIntro_ShowControls(intro);
        break;
    default:
        GF_AssertFail();
        break;
    }
    // pokeathlon.h declares the state as u8; the page is passed as it is.
    ((void (*)(PokeathlonCourseData *, u32))PokeathlonCourse_SetStateField07)(course, intro->page);
}

// The banner zooms in with the music; in link mode the screen goes on by itself after 3 seconds.
BOOL PokeathlonEventIntro_Appear(PokeathlonCourseData *course, u8 *state) {
    u8 *step = state;
    PokeathlonCourseData *data = course;
    PokeathlonEventIntro *intro = PokeathlonCourse_GetHeapAllocPtr4(data);
    PokeathlonEventIntro_UpdateMascot(intro);
    switch (*step) {
    case 0:
        PlayBGM(SEQ_GS_PT_TITLE);
        (*step)++;
        break;
    case 1:
        if (PokeathlonEventIntro_IsBgmAt(0, 0)) {
            BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFF, 1, 1, intro->heapId);
            (*step)++;
        }
        break;
    case 2:
        if (PokeathlonEventIntro_IsBgmAt(0, 1)) {
            PokeathlonEventIntro_StartZoom(intro);
            (*step)++;
        }
        break;
    case 3:
        if (intro->zooming != FALSE) {
            PokeathlonEventIntro_UpdateZoom(intro);
        } else if (PokeathlonEventIntro_IsBgmAt(0, 3)) {
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFF, 12, 1, intro->heapId);
            ManagedSprite_SetAnimateFlag(intro->sprites[2], TRUE);
            ManagedSprite_SetDrawFlag(intro->sprites[3], intro->linkMode == FALSE);
            (*step)++;
        }
        break;
    case 4:
        if (IsPaletteFadeFinished()) {
            if (intro->linkMode != FALSE) {
                intro->timer = 0;
                (*step)++;
            } else {
                PokeathlonCourse_SetStateField07(data, 1);
            }
        }
        break;
    case 5:
        intro->timer++;
        if (intro->timer >= 180) {
            PokeathlonCourse_SetStateField07(data, 4);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonEventIntro_DetailsPage(PokeathlonCourseData *course, u8 *state) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonEventIntro_UpdateMascot(intro);
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            int button = TouchscreenHitbox_FindRectAtTouchNew(ov96_0221BA20);
            if (button != -1) {
                switch (button) {
                case 0:
                    PlaySE(SEQ_SE_DP_SELECT);
                    PokeathlonEventIntro_FadeToPage(course, EVENT_INTRO_PAGE_RULES);
                    break;
                case 1:
                    PokeathlonCourse_SetStateField07(course, 4);
                    break;
                default:
                    GF_AssertFail();
                    break;
                }
                (*state)++;
            }
        }
        break;
    case 1:
        if (PokeathlonEventIntro_IsFadeFinished(course)) {
            PokeathlonEventIntro_ShowPage(course);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonEventIntro_RulesPage(PokeathlonCourseData *course, u8 *state) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonEventIntro_UpdateMascot(intro);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFF, 4, 1, intro->heapId);
        (*state)++;
        // fallthrough
    case 1:
        if (IsPaletteFadeFinished()) {
            int button = TouchscreenHitbox_FindRectAtTouchNew(ov96_0221BA2C);
            if (button == 0) {
                PlaySE(SEQ_SE_DP_SELECT);
                PokeathlonEventIntro_FadeToPage(course, EVENT_INTRO_PAGE_CONTROLS);
                (*state)++;
            } else if (button == 1) {
                PlaySE(SEQ_SE_DP_SELECT);
                PokeathlonEventIntro_FadeToPage(course, EVENT_INTRO_PAGE_DETAILS);
                (*state)++;
            }
        }
        break;
    case 2:
        if (PokeathlonEventIntro_IsFadeFinished(course)) {
            PokeathlonEventIntro_ShowPage(course);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonEventIntro_ControlsPage(PokeathlonCourseData *course, u8 *state) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonEventIntro_UpdateMascot(intro);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFF, 4, 1, intro->heapId);
        (*state)++;
        // fallthrough
    case 1:
        if (IsPaletteFadeFinished()) {
            int button = TouchscreenHitbox_FindRectAtTouchNew(ov96_0221BA38);
            if (button == 0) {
                PlaySE(SEQ_SE_DP_SELECT);
                PokeathlonEventIntro_FadeToPage(course, EVENT_INTRO_PAGE_RULES);
                (*state)++;
            } else if (button == 1) {
                PlaySE(SEQ_SE_DP_SELECT);
                PokeathlonEventIntro_FadeToPage(course, EVENT_INTRO_PAGE_DETAILS);
                (*state)++;
            }
        }
        break;
    case 2:
        if (PokeathlonEventIntro_IsFadeFinished(course)) {
            PokeathlonEventIntro_ShowPage(course);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonEventIntro_FadeOut(PokeathlonCourseData *course, u8 *state) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonEventIntro_UpdateMascot(intro);
    switch (*state) {
    case 0:
        if (PokeathlonCourse_GetCurrentParticipantIndex(course) == 0) {
            PlaySE(SEQ_SE_GS_GAMENFADEOPED);
        } else {
            PlaySE(SEQ_SE_GS_GAMENFADE);
        }
        GF_SndStartFadeOutBGM(0, 24);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFF, 24, 1, intro->heapId);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}
