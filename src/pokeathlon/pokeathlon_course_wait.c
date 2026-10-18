#include "global.h"

#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_event_intro.h"

#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "palette.h"
#include "screen_fade.h"
#include "system.h"
#include "wireless_status_icon.h"

static void PokeathlonCourseWait_Create(PokeathlonCourseData *course);
static void PokeathlonCourseWait_Free(PokeathlonCourseData *course);

enum PokeathlonCourseWaitInitState {
    COURSE_WAIT_INIT_CREATE,
    COURSE_WAIT_INIT_FADE_IN,
    COURSE_WAIT_INIT_PRINT,
    COURSE_WAIT_INIT_WAIT_PRINT,
};

enum PokeathlonCourseWaitFadeOutState {
    COURSE_WAIT_FADE_OUT_START,
    COURSE_WAIT_FADE_OUT_WAIT,
};

static const GraphicsModes sPokeathlonCourseWaitGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

static const BgTemplate sPokeathlonCourseWaitBgTemplate = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x0000,
    .charBase = GX_BG_CHARBASE_0x04000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const GraphicsBanks sPokeathlonCourseWaitGraphicsBanks = {
    .bg = GX_VRAM_BG_128_A,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_0123_H,
    .obj = GX_VRAM_OBJ_128_B,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_NONE,
    .texpltt = GX_VRAM_TEXPLTT_NONE,
};

PokeathlonCourseStepFunc gPokeathlonCourseWaitSteps[1] = {
    PokeathlonCourseWait_FadeOut,
};

BOOL PokeathlonCourseWait_Init(PokeathlonCourseData *course) {
    PokeathlonCourseWait *wait = PokeathlonCourse_GetWork(course);

    switch (PokeathlonCourse_GetEventInitState(course)) {
    case COURSE_WAIT_INIT_CREATE:
        PokeathlonCourseWait_Create(course);
        PokeathlonCourse_AdvanceEventInitState(course);
        break;
    case COURSE_WAIT_INIT_FADE_IN:
        WirelessStatusIcon_CreateOnScreenIfConnected(1);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 4, 1, wait->heapId);
        PokeathlonCourse_AdvanceEventInitState(course);
        break;
    case COURSE_WAIT_INIT_PRINT:
        if (IsPaletteFadeFinished()) {
            PokeathlonCourseMessageBox_Print(wait->messageBox, msg_0309_00310);
            PokeathlonCourse_AdvanceEventInitState(course);
        }
        break;
    case COURSE_WAIT_INIT_WAIT_PRINT:
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

static void PokeathlonCourseWait_SetBanks(void) {
    GraphicsBanks banks = sPokeathlonCourseWaitGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void PokeathlonCourseWait_VBlankCallback(void *data) {
    PokeathlonCourseWait *wait = data;
    vu32 *dtcm = (vu32 *)0x027E0000;
    DoScheduledBgGpuUpdates(wait->bgConfig);
    dtcm[0x3FF8 / sizeof(u32)] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag
}

static void PokeathlonCourseWait_InitBgs(BgConfig *bgConfig, enum HeapID heapId) {
    GraphicsModes modes = sPokeathlonCourseWaitGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    {
        BgTemplate template = sPokeathlonCourseWaitBgTemplate;
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

static void PokeathlonCourseWait_FreeBgs(PokeathlonCourseWait *wait) {
    FreeBgTilemapBuffer(wait->bgConfig, GF_BG_LYR_MAIN_0);
    Heap_Free(wait->bgConfig);
}

#pragma opt_propagation off

static void PokeathlonCourseWait_Create(PokeathlonCourseData *course) {
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
    wait = PokeathlonCourse_AllocWork(course, sizeof(PokeathlonCourseWait));
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

#pragma opt_propagation reset

static void PokeathlonCourseWait_Free(PokeathlonCourseData *course) {
    PokeathlonCourseWait *wait = PokeathlonCourse_GetWork(course);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    WirelessStatusIcon_Destroy();
    PokeathlonCourseMessageBox_Free(wait->messageBox);
    PokeathlonCourseWait_FreeBgs(wait);
    PokeathlonCourse_FreeWork(course);
    Heap_Destroy(HEAP_ID_159);
}

BOOL PokeathlonCourseWait_FadeOut(PokeathlonCourseData *course, u8 *state) {
    PokeathlonCourseWait *wait = PokeathlonCourse_GetWork(course);
    switch (*state) {
    case COURSE_WAIT_FADE_OUT_START:
        if (++wait->timer > 15) {
            PokeathlonCourseMessageBox_HideWaitingIcon(wait->messageBox);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 4, 1, wait->heapId);
            (*state)++;
        }
        break;
    case COURSE_WAIT_FADE_OUT_WAIT:
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
