#include "code_entry.h"

#include "global.h"

#include "bg_window.h"
#include "code_entry_internal.h"
#include "error_handling.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "palette.h"
#include "sprite_system.h"
#include "system.h"
#include "touch_hitbox_controller.h"
#include "touchpad.h"
#include "vram_transfer_manager.h"

typedef struct CodeEntryMainBgTemplates {
    BgTemplate templates[3];
} CodeEntryMainBgTemplates;

typedef struct CodeEntrySubBgTemplates {
    BgTemplate templates[4];
} CodeEntrySubBgTemplates;

void WirelessStatusIcon_ShowOnBothScreens(int topFirst, enum HeapID heapID);
void WirelessStatusIcon_SetLevel(int level);
void WirelessStatusIcon_Create(void);
void WirelessStatusIcon_Destroy(void);
int DWC_GetLinkLevel(void);

static CodeEntryArgs *CodeEntryArgs_Alloc(enum HeapID heapID, u32 length, const int *groupLengths, Options *options, int promptMsg, u32 showWirelessIcon);
static void CodeEntry_SetupBgs(BgConfig *bgConfig);
static void CodeEntry_VBlank(void *data);
static void CodeEntry_InitLayout(CodeEntryApp *app);

const OverlayManagerTemplate gOverlayTemplate_CodeEntry = { CodeEntry_Init, CodeEntry_Main, CodeEntry_Exit, FS_OVERLAY_ID_NONE };

static const GraphicsBanks sCodeEntryGraphicsBanks = {
    .bg = GX_VRAM_BG_128_A,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_32_H,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_64_E,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_01_BC,
    .texpltt = GX_VRAM_TEXPLTT_01_FG,
};

static const CodeEntryMainBgTemplates sCodeEntryMainBgTemplates = {
    .templates = {
                  {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0000,
            .charBase = GX_BG_CHARBASE_0x04000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 1,
            .areaOver = GX_BG_AREAOVER_XLU,
            .dummy = 0,
            .mosaic = FALSE,
        },
                  {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x512_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x1000,
            .charBase = GX_BG_CHARBASE_0x0c000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 2,
            .areaOver = GX_BG_AREAOVER_XLU,
            .dummy = 0,
            .mosaic = FALSE,
        },
                  {
            .x = 0,
            .y = 0,
            .bufferSize = 0x1800,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x3000,
            .charBase = GX_BG_CHARBASE_0x10000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 3,
            .areaOver = GX_BG_AREAOVER_XLU,
            .dummy = 0,
            .mosaic = FALSE,
        },
                  },
};

static const CodeEntrySubBgTemplates sCodeEntrySubBgTemplates = {
    .templates = {
                  {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x6800,
            .charBase = GX_BG_CHARBASE_0x00000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 0,
            .areaOver = GX_BG_AREAOVER_XLU,
            .dummy = 0,
            .mosaic = FALSE,
        },
                  {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x7000,
            .charBase = GX_BG_CHARBASE_0x04000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 1,
            .areaOver = GX_BG_AREAOVER_XLU,
            .dummy = 0,
            .mosaic = FALSE,
        },
                  {
            .x = 0,
            .y = 0,
            .bufferSize = 0,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_128x128,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0000,
            .charBase = GX_BG_CHARBASE_0x00000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 0,
            .areaOver = GX_BG_AREAOVER_XLU,
            .dummy = 0,
            .mosaic = FALSE,
        },
                  {
            .x = 0,
            .y = 0,
            .bufferSize = 0,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_128x128,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0000,
            .charBase = GX_BG_CHARBASE_0x00000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 0,
            .areaOver = GX_BG_AREAOVER_XLU,
            .dummy = 0,
            .mosaic = FALSE,
        },
                  },
};

BOOL CodeEntry_Init(OverlayManager *manager, int *state) {
    CodeEntryApp *app;

    Heap_Create(HEAP_ID_3, HEAP_ID_108, 0x40000);
    app = OverlayManager_CreateAndGetData(manager, sizeof(CodeEntryApp), HEAP_ID_108);
    memset(app, 0, sizeof(CodeEntryApp));
    app->args = *(CodeEntryArgs *)OverlayManager_GetArgs(manager);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    *(vu32 *)0x04000000 &= 0xFFFFE0FF;
    *(vu32 *)0x04001000 &= 0xFFFFE0FF;
    app->narc = NARC_New(NARC_a_1_9_0, HEAP_ID_108);
    app->bgConfig = BgConfig_Alloc(HEAP_ID_108);
    app->paletteData = PaletteData_Init(HEAP_ID_108);
    PaletteData_SetAutoTransparent(app->paletteData, TRUE);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_108);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_SUB_BG, 0x200, HEAP_ID_108);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_MAIN_OBJ, 0x200, HEAP_ID_108);
    PaletteData_AllocBuffers(app->paletteData, PLTTBUF_SUB_OBJ, 0x200, HEAP_ID_108);
    CodeEntry_SetupBgs(app->bgConfig);
    CodeEntry_InitSpriteSystem(app);
    CodeEntry_InitLayout(app);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    CodeEntry_InitTouch(app);
    if (app->args.showWirelessIcon) {
        WirelessStatusIcon_Create();
        WirelessStatusIcon_ShowOnBothScreens(1, HEAP_ID_108);
    }
    G2x_SetBlendAlpha_(REG_BLDCNT_ADDR, GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2, 0xF, 7);
    G2x_SetBlendAlpha_(REG_DB_BLDCNT_ADDR, GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, 7, 8);
    Main_SetVBlankIntrCB(CodeEntry_VBlank, app);
    return TRUE;
}

BOOL CodeEntry_Main(OverlayManager *manager, int *state) {
    CodeEntryApp *app = OverlayManager_GetData(manager);
    BOOL done = CodeEntry_RunState(app);

    if (app->args.showWirelessIcon) {
        WirelessStatusIcon_SetLevel(3 - DWC_GetLinkLevel());
    }
    if (done) {
        return TRUE;
    }
    return FALSE;
}

BOOL CodeEntry_Exit(OverlayManager *manager, int *state) {
    CodeEntryApp *app = OverlayManager_GetData(manager);

    if (app->args.showWirelessIcon) {
        WirelessStatusIcon_Destroy();
    }
    CodeEntry_FreeGraphics(app);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_SUB_3);
    Heap_Free(app->bgConfig);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(app->paletteData, PLTTBUF_SUB_OBJ);
    PaletteData_Free(app->paletteData);
    NARC_Delete(app->narc);
    GF_TouchpadStopSampling();
    SpriteSystem_FreeResourcesAndManager(app->spriteSystem, app->spriteManager);
    SpriteSystem_Free(app->spriteSystem);
    TouchHitboxController_Destroy(app->touchController);
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_108);
    return TRUE;
}

static CodeEntryArgs *CodeEntryArgs_Alloc(enum HeapID heapID, u32 length, const int *groupLengths, Options *options, int promptMsg, u32 showWirelessIcon) {
    CodeEntryArgs *args = Heap_Alloc(heapID, sizeof(CodeEntryArgs));
    int i;

    args->length = length;
    args->text = String_New(length + 1, heapID);
    args->options = options;
    for (i = 0; i < 4; i++) {
        args->groupLengths[i] = groupLengths[i];
    }
    args->groupLengths[i] = groupLengths[i - 1];
    args->promptMsg = promptMsg;
    args->showWirelessIcon = showWirelessIcon;
    return args;
}

CodeEntryArgs *CodeEntryArgs_New(enum HeapID heapID, u32 length, const int *groupLengths, Options *options, int promptMsg, u32 showWirelessIcon) {
    CodeEntryArgs *args = CodeEntryArgs_Alloc(heapID, length, groupLengths, options, promptMsg, showWirelessIcon);
    args->fixedGroups = 0;
    args->fixedValue = 0;
    return args;
}

CodeEntryArgs *CodeEntryArgs_NewWithFixedDigits(enum HeapID heapID, u32 length, const int *groupLengths, Options *options, int promptMsg, u32 showWirelessIcon, int fixedGroups, u32 fixedValue) {
    CodeEntryArgs *args = CodeEntryArgs_Alloc(heapID, length, groupLengths, options, promptMsg, showWirelessIcon);
    args->fixedGroups = fixedGroups;
    args->fixedValue = fixedValue;
    return args;
}

void CodeEntryArgs_Delete(CodeEntryArgs *args) {
    if (args->text == NULL) {
        GF_AssertFail();
    }
    if (args == NULL) {
        GF_AssertFail();
    }
    String_Delete(args->text);
    Heap_Free(args);
}

static void CodeEntry_SetupBgs(BgConfig *bgConfig) {
    GraphicsBanks banks;
    CodeEntryMainBgTemplates mainBgs;
    CodeEntrySubBgTemplates subBgs;

    GfGfx_DisableEngineAPlanes();
    banks = sCodeEntryGraphicsBanks;
    GfGfx_SetBanks(&banks);
    MIi_CpuClear32(0, (u32 *)HW_BG_VRAM, 0x80000);
    MIi_CpuClear32(0, (u32 *)HW_DB_BG_VRAM, 0x20000);
    MIi_CpuClear32(0, (u32 *)HW_OBJ_VRAM, 0x40000);
    MIi_CpuClear32(0, (u32 *)HW_DB_OBJ_VRAM, 0x20000);
    mainBgs = sCodeEntryMainBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &mainBgs.templates[0], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &mainBgs.templates[1], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &mainBgs.templates[2], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    subBgs = sCodeEntrySubBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &subBgs.templates[0], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &subBgs.templates[1], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &subBgs.templates[2], GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &subBgs.templates[3], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void CodeEntry_VBlank(void *data) {
    CodeEntryApp *app = data;

    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(app->paletteData);
    DoScheduledBgGpuUpdates(app->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void CodeEntry_SelectGroup(CodeEntryApp *app, int group) {
    app->previousGroup = app->selectedGroup;
    app->selectedGroup = group;
    app->selectedStart = 0;
    app->selectedEnd = 0;
    app->previousStart = 0;
    app->previousEnd = 0;
    if (app->selectedGroup != 0) {
        int index = app->selectedGroup - 1;
        app->selectedStart = app->groupRanges[index].start;
        index = app->selectedGroup - 1;
        app->selectedEnd = app->groupRanges[index].end;
    }
    if (app->previousGroup != 0) {
        int index = app->previousGroup - 1;
        app->previousStart = app->groupRanges[index].start;
        index = app->previousGroup - 1;
        app->previousEnd = app->groupRanges[index].end;
    }
}

static void CodeEntry_InitLayout(CodeEntryApp *app) {
    int i, group, j, end;
    u16 start;

    app->touchActive = TRUE;
    start = 0;
    for (i = 0; i < 5; i++) {
        app->groupRanges[i].start = start;
        start += app->args.groupLengths[i];
        app->groupRanges[i].end = start;
    }
    CodeEntry_SelectGroup(app, app->args.fixedGroups + 1);
    for (i = 0; i < 4; i++) {
        if (app->args.groupLengths[i] == 0) {
            break;
        }
        app->digitCount += app->args.groupLengths[i];
        app->separatorCount++;
    }
    app->separatorCount--;
    app->groupX[0] = 0x70 - ((app->digitCount + app->separatorCount) * 8) / 2;
    for (i = 0; i < 4; i++) {
        app->groupX[i + 1] = 0x70 - (app->separatorCount * 8 + ((app->digitCount - app->args.groupLengths[i]) * 8 + app->args.groupLengths[i] * 32)) / 2;
    }
    app->groupX[1] += 12;
    end = 0;
    for (i = 0; i < app->separatorCount; i++) {
        end += app->args.groupLengths[i];
        app->separators[i].value = end - 1;
    }
    i = 0;
    group = 0;
    do {
        for (j = 0; j < app->args.groupLengths[group]; j++) {
            app->digits[i].group = group + 1;
            i++;
        }
        group++;
    } while (i < app->digitCount);
    for (i = 0; i < app->args.fixedGroups; i++) {
        app->firstEditable += app->args.groupLengths[i];
    }
}
