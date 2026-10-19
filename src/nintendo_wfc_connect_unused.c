#include "nintendo_wfc_connect_unused.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "record_mixing/record_mixing.h"

#include "bg_window.h"
#include "comm_wireless.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "network_overlay_loader.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"
#include "sprite_transfer.h"
#include "system.h"
#include "vram_transfer_manager.h"
#include "wireless_status_icon.h"

// An older version of the Nintendo WFC connection app, with a scrolling number instead of the touch number pad and a
// Yes/No prompt instead of the touch YES/NO menu. Nothing in the game launches it.

extern const u16 sNintendoWfcUnusedArrowPositions[2][2];

enum NintendoWfcUnusedInitState {
    NINTENDO_WFC_UNUSED_INIT_SETUP,
    NINTENDO_WFC_UNUSED_INIT_START_WIRELESS,
};

static void NintendoWfcUnused_VBlankCB(void *arg);
static void NintendoWfcUnused_SetGfxBanks(void);
static void NintendoWfcUnused_InitArgs(NintendoWfcUnused *wfc, OverlayManager *man);
static void NintendoWfcUnused_ExitDummy(NintendoWfcUnused *wfc);
static void NintendoWfcUnused_InitObjTransfer(void);
static void NintendoWfcUnused_InitSpriteResources(NintendoWfcUnused *wfc);
static void NintendoWfcUnused_InitSpriteTemplate(SpriteTemplate *template, NintendoWfcUnused *wfc, SpriteResourcesHeader *header, u32 whichScreen);
static void NintendoWfcUnused_CreateSprites(NintendoWfcUnused *wfc);
static void *NintendoWfcUnused_Alloc(void *unused, u32 size, int alignment);
static void NintendoWfcUnused_Free(void *unused, void *ptr);
static void NintendoWfcUnused_InitGraphics(NintendoWfcUnused *wfc);
static void NintendoWfcUnused_FreeGraphics(NintendoWfcUnused *wfc);

static const ObjCharTransferTemplate sNintendoWfcUnusedObjCharTransferTemplate = {
    .maxTasks = 20,
    .sizeMain = 0x800,
    .sizeSub = 0x800,
    .heapID = HEAP_ID_150,
};

static const GraphicsModes sNintendoWfcUnusedGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

static const GraphicsBanks sNintendoWfcUnusedGraphicsBanks = {
    .bg = GX_VRAM_BG_128_A,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_64_E,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_0_B,
    .texpltt = GX_VRAM_TEXPLTT_01_FG,
};

static NintendoWfcUnusedModeRow sNintendoWfcUnusedModes[1] = {
    {
     .init = NintendoWfcUnused_SessionInit,
     .main = NintendoWfcUnused_SessionMain,
     .exit = NintendoWfcUnused_SessionExit,
     },
};

static NintendoWfcUnusedGlobal sNintendoWfcUnusedGlobal;

BOOL NintendoWfcUnused_Init(OverlayManager *man, int *state) {
    NintendoWfcUnused *wfc;
    GraphicsModes modes;
    void *heapMem;

    switch (*state) {
    case NINTENDO_WFC_UNUSED_INIT_SETUP:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        Heap_Create(HEAP_ID_3, HEAP_ID_150, 0x50000);
        wfc = OverlayManager_CreateAndGetData(man, sizeof(NintendoWfcUnused), HEAP_ID_150);
        memset(wfc, 0, sizeof(NintendoWfcUnused));
        wfc->bgConfig = BgConfig_Alloc(HEAP_ID_150);
        sNintendoWfcUnusedGlobal.wfc = wfc;
        modes = sNintendoWfcUnusedGraphicsModes;
        SetBothScreensModesAndDisable(&modes);
        wfc->msgFormat = MessageFormat_New_Custom(11, 32, HEAP_ID_150);
        wfc->msgDataTower = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0799_bin, HEAP_ID_150);
        wfc->msgDataWfc = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0778_bin, HEAP_ID_150);
        wfc->msgDataError = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0800_bin, HEAP_ID_150);
        SetKeyRepeatTimers(4, 8);
        NintendoWfcUnused_InitArgs(wfc, man);
        NintendoWfcUnused_InitGraphics(wfc);
        Sound_SetSceneAndPlayBGM(52, 0, 0);
        heapMem = Heap_Alloc(HEAP_ID_150, 0x20020);
        wfc->heapMem = heapMem;
        wfc->expHeap = NNS_FndCreateExpHeapEx((void *)(((u32)heapMem + 31) & ~31), 0x20000, 0);
        Sound_SetSceneAndPlayBGM(11, SEQ_GS_WIFI_ACCESS, 1);
        reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
        *state = NINTENDO_WFC_UNUSED_INIT_START_WIRELESS;
        break;
    case NINTENDO_WFC_UNUSED_INIT_START_WIRELESS:
        LoadDwcOverlay();
        LoadOVY38();
        CommWireless_StartUp();
        *state = NINTENDO_WFC_UNUSED_INIT_SETUP;
        return TRUE;
    default:
        break;
    }
    return FALSE;
}

BOOL NintendoWfcUnused_Main(OverlayManager *man, int *state) {
    NintendoWfcUnused *wfc = OverlayManager_GetData(man);
    DWC_UpdateConnection();
    NintendoWfcHttp_Update();
    DWC_UpdateConnection();
    switch (*state) {
    case NINTENDO_WFC_UNUSED_MAIN_WAIT_STARTUP:
        if (CommWireless_IsStartedUp()) {
            sNintendoWfcUnusedGlobal.heap = wfc->expHeap;
            DWC_SetMemFunc(NintendoWfcUnused_Alloc, NintendoWfcUnused_Free);
            *state = NINTENDO_WFC_UNUSED_MAIN_SESSION_INIT;
        }
        break;
    case NINTENDO_WFC_UNUSED_MAIN_SESSION_INIT:
        *state = sNintendoWfcUnusedModes[wfc->mode].init(wfc, *state);
        break;
    case NINTENDO_WFC_UNUSED_MAIN_WAIT_FADE_IN:
        if (IsPaletteFadeFinished()) {
            *state = NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN;
        }
        break;
    case NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN:
        *state = sNintendoWfcUnusedModes[wfc->mode].main(wfc, *state);
        break;
    case NINTENDO_WFC_UNUSED_MAIN_SESSION_EXIT:
        if (IsPaletteFadeFinished()) {
            *state = sNintendoWfcUnusedModes[wfc->mode].exit(wfc, *state);
        }
        break;
    case NINTENDO_WFC_UNUSED_MAIN_DONE:
        return TRUE;
    }
    if (wfc->spriteList) {
        SpriteList_RenderAndAnimateSprites(wfc->spriteList);
    }
    return FALSE;
}

BOOL NintendoWfcUnused_Exit(OverlayManager *man, int *state) {
    NintendoWfcUnused *wfc = OverlayManager_GetData(man);

    Heap_Free(wfc->heapMem);
    UnloadOVY38();
    UnloadDwcOverlay();
    NintendoWfcUnused_FreeGraphics(wfc);
    DestroyMsgData(wfc->msgDataError);
    DestroyMsgData(wfc->msgDataWfc);
    DestroyMsgData(wfc->msgDataTower);
    MessageFormat_Delete(wfc->msgFormat);
    NintendoWfcUnused_ExitDummy(wfc);
    CommWireless_Terminate();
    Heap_Free(wfc->bgConfig);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_150);
    return TRUE;
}

static void NintendoWfcUnused_VBlankCB(void *arg) {
    NintendoWfcUnused *wfc = arg;

    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(wfc->bgConfig);
    OamManager_ApplyAndResetBuffers();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

static void NintendoWfcUnused_SetGfxBanks(void) {
    GraphicsBanks banks = sNintendoWfcUnusedGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void NintendoWfcUnused_InitArgs(NintendoWfcUnused *wfc, OverlayManager *man) {
    wfc->args = OverlayManager_GetArgs(man);
    wfc->mode = 0;
    NintendoWfcUnused_SetNextMode(wfc, 0, 0);
}

static void NintendoWfcUnused_ExitDummy(NintendoWfcUnused *wfc) {
}

static void NintendoWfcUnused_InitObjTransfer(void) {
    ObjCharTransferTemplate template = sNintendoWfcUnusedObjCharTransferTemplate;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(20, HEAP_ID_150);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

static void NintendoWfcUnused_InitSpriteResources(NintendoWfcUnused *wfc) {
    NARC *narc = NARC_New(NARC_a_1_0_0, HEAP_ID_150);
    int i;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_150);
    wfc->spriteList = G2dRenderer_Init(10, &wfc->renderer, HEAP_ID_150);
    G2dRenderer_SetSubSurfaceCoords(&wfc->renderer, 0, 256 << FX32_SHIFT);
    for (i = 0; i < 4; i++) {
        wfc->resMan[i] = Create2DGfxResObjMan(2, (GfGfxResType)i, HEAP_ID_150);
    }
    wfc->resObjs[0] = AddCharResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_CHAR], narc, 46, TRUE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_150);
    wfc->resObjs[1] = AddPlttResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_PLTT], narc, 10, FALSE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 3, HEAP_ID_150);
    wfc->resObjs[2] = AddCellOrAnimResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_CELL], narc, 47, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_150);
    wfc->resObjs[3] = AddCellOrAnimResObjFromOpenNarc(wfc->resMan[GF_GFX_RES_TYPE_ANIM], narc, 48, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_150);
    SpriteTransfer_CreateCharTransferTask(wfc->resObjs[0]);
    SpriteTransfer_CreateExtPlttTransferTask(wfc->resObjs[1]);
    NARC_Delete(narc);
}

static void NintendoWfcUnused_InitSpriteTemplate(SpriteTemplate *template, NintendoWfcUnused *wfc, SpriteResourcesHeader *header, u32 whichScreen) {
    template->spriteList = wfc->spriteList;
    template->header = header;
    template->position.z = 0;
    template->scale.x = FX32_ONE;
    template->scale.y = FX32_ONE;
    template->scale.z = FX32_ONE;
    template->rotation = 0;
    template->drawPriority = 1;
    template->whichScreen = (NNS_G2D_VRAM_TYPE)whichScreen;
    template->heapID = HEAP_ID_150;
}

static void NintendoWfcUnused_CreateSprites(NintendoWfcUnused *wfc) {
    SpriteTemplate template;
    int i;
    CreateSpriteResourcesHeader(&wfc->spriteHeader, 0, 0, 0, 0, -1, -1, 0, 0, wfc->resMan[0], wfc->resMan[1], wfc->resMan[2], wfc->resMan[3], NULL, NULL);
    NintendoWfcUnused_InitSpriteTemplate(&template, wfc, &wfc->spriteHeader, NNS_G2D_VRAM_TYPE_2DMAIN);
    for (i = 0; i < 2; i++) {
        template.position.x = sNintendoWfcUnusedArrowPositions[i][0] << FX32_SHIFT;
        template.position.y = sNintendoWfcUnusedArrowPositions[i][1] << FX32_SHIFT;
        wfc->arrows[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(wfc->arrows[i], TRUE);
        Sprite_SetAnimCtrlSeq(wfc->arrows[i], i);
        Sprite_SetDrawFlag(wfc->arrows[i], FALSE);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    WirelessStatusIcon_Create();
}

YesNoPrompt *NintendoWfcUnused_CreateYesNoPrompt(BgConfig *bgConfig, u8 y, int tileStart) {
    YesNoPromptTemplate template;
    YesNoPrompt *prompt = YesNoPrompt_Create(HEAP_ID_150);
    template.bgConfig = bgConfig;
    template.bgId = GF_BG_LYR_MAIN_0;
    template.tileStart = tileStart;
    template.plttSlot = 8;
    template.x = 23;
    template.y = y;
    template.ignoreTouchFlag = 0;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(prompt, &template);
    return prompt;
}

int NintendoWfcUnused_HandleYesNoPrompt(YesNoPrompt *prompt) {
    int response = YesNoPrompt_HandleInput(prompt);
    if (response) {
        YesNoPrompt_Destroy(prompt);
    }
    return response;
}

void NintendoWfcUnused_SetState(NintendoWfcUnused *wfc, int state, int nextState) {
    wfc->state = state;
    wfc->nextState = nextState;
}

static void *NintendoWfcUnused_Alloc(void *unused, u32 size, int alignment) {
    OSIntrMode enabled = OS_DisableInterrupts();
    void *result = NNS_FndAllocFromExpHeapEx(sNintendoWfcUnusedGlobal.heap, size, alignment);
    OS_RestoreInterrupts(enabled);
    return result;
}

static void NintendoWfcUnused_Free(void *unused, void *ptr) {
    if (ptr) {
        OSIntrMode enabled = OS_DisableInterrupts();
        NNS_FndFreeToExpHeap(sNintendoWfcUnusedGlobal.heap, ptr);
        OS_RestoreInterrupts(enabled);
    }
}

int NintendoWfcUnused_GetWifiIconLevel(NintendoWfcUnused *wfc) {
    return 3 - DWC_GetLinkLevel();
}

void NintendoWfcUnused_SetNextMode(NintendoWfcUnused *wfc, int mode, int a2) {
    wfc->nextMode = mode;
    wfc->unk_0018 = a2;
}

static void NintendoWfcUnused_InitGraphics(NintendoWfcUnused *wfc) {
    NintendoWfcUnused_SetGfxBanks();
    NintendoWfcUnused_InitObjTransfer();
    NintendoWfcUnused_InitSpriteResources(wfc);
    NintendoWfcUnused_CreateSprites(wfc);
    Main_SetVBlankIntrCB(NintendoWfcUnused_VBlankCB, wfc);
}

static void NintendoWfcUnused_FreeGraphics(NintendoWfcUnused *wfc) {
    int i;

    SpriteTransfer_DeleteCharTransferTask(wfc->resObjs[0]);
    SpriteTransfer_DeletePlttTransferTask(wfc->resObjs[1]);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(wfc->resMan[i]);
    }
    SpriteList_Delete(wfc->spriteList);
    wfc->spriteList = NULL;
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}
