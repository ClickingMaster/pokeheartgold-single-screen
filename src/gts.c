#include "gts.h"

#include <string.h>

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "game_stats_request.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "gts_internal.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pokemon_icon_idx.h"
#include "pokemon_storage_system.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "touchscreen_list_menu.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02034B0C.h"
#include "unk_02037C94.h"
#include "unk_020915B0.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

// One screen of the app: init, main and exit, each returns the next Gts_Main state.
typedef struct GtsScreen {
    int (*init)(GtsApp *app, int state);
    int (*main)(GtsApp *app, int state);
    int (*exit)(GtsApp *app, int state);
} GtsScreen;

extern const char sGtsPostUrl[];
extern const char sGtsPostFinishUrl[];
extern const char sGtsGetUrl[];
extern const char sGtsResultUrl[];
extern const char sGtsDeleteUrl[];
extern const char sGtsReturnUrl[];
extern const char sGtsSearchUrl[];
extern const char sGtsExchangeUrl[];
extern const char sGtsExchangeFinishUrl[];
extern const char sGtsInfoUrl[];
extern const char sGtsSetProfileUrl[];
extern const GraphicsModes sGtsGraphicsModes;
extern const GraphicsBanks sGtsGraphicsBanks;
extern const ObjCharTransferTemplate sGtsObjCharTransferTemplate;
extern const TouchscreenListMenuTemplate sGtsListMenuTemplate;
extern const GtsScreen sGtsScreens[];

// Network library (OVY_0) and loaders without a header declaring them.
int WCM_GetPhase(void);
void ov00_021EC210(void);
void ov00_021ECB40(void);
void ov00_021EC294(void *(*alloc)(void *, u32, int), void (*free)(void *, void *));
int ov00_021EC9D4(void);
BOOL LoadOVY38(void);
void UnloadOVY38(void);
void sub_02034DE0(void);

// Other GTS screens.
void ov70_02239C6C(GtsApp *app);
void ov70_02239CF8(GtsApp *app);
void ov70_02240D74(GtsApp *app, u32 gender);
void ov70_02241184(GtsApp *app, u32 arg1, int arg2);
void ov70_02241308(GtsApp *app);

BOOL GtsServer_StartRequest(const char *url, void *payload, int payloadSize, void *response, int responseCapacity);
void Gts_VBlankCB(void *arg);
void Gts_SetGfxBanks(void);
void Gts_InitData(GtsApp *app, OverlayManager *man);
void ov70_0223887C(GtsApp *app);
void Gts_ListMenuCallback(TouchscreenListMenu *menu, u8 cursorPos, void *callbackArg, int event);
void *Gts_Alloc(void *unused, u32 size, int alignment);
void Gts_Free(void *unused, void *ptr);
void Gts_DecrementTimer(GtsApp *app);
void Gts_CountBoxMonsStep(GtsApp *app);

void GtsServer_Exit(void) {
    gGtsServer.state = GTS_SERVER_STATE_NONE;
}

BOOL GtsServer_IsRequestDone(void) {
    switch (gGtsServer.state) {
    case GTS_SERVER_STATE_IDLE:
        return TRUE;
    case GTS_SERVER_STATE_DONE:
        gGtsServer.state = GTS_SERVER_STATE_IDLE;
        return TRUE;
    default:
        return FALSE;
    }
}

int GtsServer_GetResult(void) {
    return gGtsServer.result;
}

void GtsServer_RequestPost(const void *record) {
    memcpy(gGtsServerRequest, record, GTS_RECORD_SIZE);
    GameStats_Start();
    if (GtsServer_StartRequest(sGtsPostUrl, gGtsServerRequest, GTS_RECORD_SIZE, gGtsServerShortResponse, 2)) {
        gGtsServer.state = GTS_SERVER_STATE_POST;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

#pragma force_active on
// The copy goes through two pointers (opt_propagation off keeps them).
#pragma opt_propagation off

void GtsServer_RequestPostFinish(void) {
    GtsFriendKey *src = &gGtsServerFriendKey;
    GtsFriendKey *dest = (GtsFriendKey *)gGtsServerRequest;
    *dest = *src;
    GameStats_Start();
    if (GtsServer_StartRequest(sGtsPostFinishUrl, gGtsServerRequest, sizeof(GtsFriendKey), gGtsServerShortResponse, 2)) {
        gGtsServer.state = GTS_SERVER_STATE_POST_FINISH;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

#pragma opt_propagation reset
#pragma force_active reset

void GtsServer_RequestGet(void *response) {
    ov70_02246900.response = response;
    GameStats_Start();
    if (GtsServer_StartRequest(sGtsGetUrl, gGtsServerRequest, 0, response, GTS_RECORD_SIZE)) {
        gGtsServer.state = GTS_SERVER_STATE_GET;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

void GtsServer_RequestResult(void *response) {
    ov70_02246900.response = response;
    GameStats_Start();
    if (GtsServer_StartRequest(sGtsResultUrl, gGtsServerRequest, 0, response, GTS_RECORD_SIZE)) {
        gGtsServer.state = GTS_SERVER_STATE_RESULT;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

void GtsServer_RequestDelete(void) {
    GameStats_Start();
    if (GtsServer_StartRequest(sGtsDeleteUrl, gGtsServerRequest, 0, gGtsServerShortResponse, 2)) {
        gGtsServer.state = GTS_SERVER_STATE_DELETE;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

void GtsServer_RequestReturn(void) {
    GameStats_Start();
    if (GtsServer_StartRequest(sGtsReturnUrl, gGtsServerRequest, 0, gGtsServerShortResponse, 2)) {
        gGtsServer.state = GTS_SERVER_STATE_RETURN;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

void GtsServer_RequestSearch(const void *criteria, int count, void *response) {
    ov70_02246900.response = response;
    GameStats_Start();
    *(GtsSearchCriteria *)gGtsServerRequest = *(const GtsSearchCriteria *)criteria;
    gGtsServer.request.search.count = count;
    if (GtsServer_StartRequest(sGtsSearchUrl, gGtsServerRequest, 7, response, GTS_RECORD_SIZE * count)) {
        gGtsServer.state = GTS_SERVER_STATE_SEARCH;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

#pragma force_active on

void GtsServer_RequestSearchEx(const void *criteria, void *response) {
    u32 i;
    const u8 *src;
    u8 *dest;

    ov70_02246900.response = response;
    GameStats_Start();
    src = criteria;
    dest = gGtsServerRequest;
    i = sizeof(GtsSearchCriteriaEx);
    do {
        *dest++ = *src++;
        i--;
    } while (i != 0);
    if (GtsServer_StartRequest(sGtsSearchUrl, gGtsServerRequest, sizeof(GtsSearchCriteriaEx), response, ((const GtsSearchCriteriaEx *)criteria)->count * GTS_RECORD_SIZE)) {
        gGtsServer.state = GTS_SERVER_STATE_SEARCH;
        return;
    }
    gGtsServer.state = GTS_SERVER_STATE_DONE;
    gGtsServer.result = -13;
    GameStats_Shutdown();
}

#pragma force_active reset

void GtsServer_RequestExchange(void *arg, const void *record, void *response) {
    ov70_02246900.response = response;
    GameStats_Start();
    memcpy(gGtsServerRequest, record, GTS_RECORD_SIZE);
    ov70_02246900.exchangeArg = arg;
    if (GtsServer_StartRequest(sGtsExchangeUrl, gGtsServerRequest, GTS_RECORD_SIZE + 4, response, GTS_RECORD_SIZE)) {
        gGtsServer.state = GTS_SERVER_STATE_EXCHANGE;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

#pragma force_active on
// The copy goes through two pointers (opt_propagation off keeps them).
#pragma opt_propagation off

void GtsServer_RequestExchangeFinish(void) {
    GtsFriendKey *src = &gGtsServerFriendKey;
    GtsFriendKey *dest = (GtsFriendKey *)gGtsServerRequest;
    *dest = *src;
    GameStats_Start();
    if (GtsServer_StartRequest(sGtsExchangeFinishUrl, gGtsServerRequest, sizeof(GtsFriendKey), gGtsServerShortResponse, 2)) {
        gGtsServer.state = GTS_SERVER_STATE_EXCHANGE_FINISH;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

#pragma opt_propagation reset
#pragma force_active reset

void GtsServer_RequestInfo(void) {
    GameStats_Start();
    if (GtsServer_StartRequest(sGtsInfoUrl, gGtsServerRequest, 0, gGtsServerShortResponse, 2)) {
        gGtsServer.state = GTS_SERVER_STATE_INFO;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

// profile: 0x64 bytes (Unk020317F4), the console's MAC address is filled in at 0x1C.
void GtsServer_RequestSetProfile(void *profile, void *response) {
    OS_GetMacAddress((u8 *)profile + 0x1C);
    memcpy(gGtsServerRequest, profile, 0x64);
    ov70_02246900.response = response;
    GameStats_Start();
    if (GtsServer_StartRequest(sGtsSetProfileUrl, gGtsServerRequest, 0x64, ov70_02246900.response, 8)) {
        gGtsServer.state = GTS_SERVER_STATE_SET_PROFILE;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

BOOL GtsServer_StartRequest(const char *url, void *payload, int payloadSize, void *response, int responseCapacity) {
    switch (GameStats_PrepareRequest((char *)url, gGtsServer.pid, payload, payloadSize, response, responseCapacity)) {
    case 0:
        return TRUE;
    case 1:
        break;
    case 2:
        break;
    }
    return FALSE;
}

// Maps a failed request's error to a result: -13 (failed), -15, -2; on a connection
// error (not -13), -14 when the wireless connection is no longer up.
int GtsServer_GetErrorResult(int error) {
    int result;

    switch (error) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 9:
    case 10:
    case 14:
    case 20:
    case 21:
    case 22:
    case 23:
    case 32:
        result = -13;
        break;
    case 2:
    case 8:
    case 25:
        result = -13;
        break;
    case 11:
        result = -15;
        break;
    case 12:
    case 13:
        result = -15;
        break;
    case 16:
    case 17:
    case 18:
    case 19:
    case 26:
    case 27:
    case 28:
    case 29:
    case 31:
        result = -2;
        break;
    case 15:
    case 30:
        result = -2;
        break;
    default:
        result = -13;
        break;
    }
    if (result != -13) {
        if (WCM_GetPhase() != 9) {
            result = -14;
        }
        ov00_021EC210();
    }
    return result;
}

BOOL Gts_Init(OverlayManager *man, int *state) {
    GraphicsModes graphicsModes;
    GtsApp *app;

    switch (*state) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= 0xFFFFE0FF;
        reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
        Gts_SetupWindow0();
        Heap_Create(HEAP_ID_3, HEAP_ID_61, 0x70000);
        LoadDwcOverlay();
        LoadOVY38();
        sub_02039FD8(HEAP_ID_61);
        app = OverlayManager_CreateAndGetData(man, sizeof(GtsApp), HEAP_ID_61);
        memset(app, 0, sizeof(GtsApp));
        app->bgConfig = BgConfig_Alloc(HEAP_ID_61);
        gGtsGlobals.app = app;
        graphicsModes = sGtsGraphicsModes;
        SetBothScreensModesAndDisable(&graphicsModes);
        FontID_Alloc(4, HEAP_ID_61);
        app->msgFormat = MessageFormat_New_Custom(11, 64, HEAP_ID_61);
        app->gtsMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0775_bin, HEAP_ID_61);
        app->wifiMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0778_bin, HEAP_ID_61);
        app->wifiErrorMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0800_bin, HEAP_ID_61);
        app->speciesNames = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0237_bin, HEAP_ID_61);
        app->countryNames = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0798_bin, HEAP_ID_61);
        SetKeyRepeatTimers(4, 8);
        Gts_InitData(app, man);
        Gts_InitGraphics(app);
        Sound_SetSceneAndPlayBGM(11, SEQ_GS_WIFI_ACCESS, 1);
        app->heapMemory = Heap_Alloc(HEAP_ID_61, 0x20020);
        app->heap = NNS_FndCreateExpHeapEx((void *)(((u32)app->heapMemory + 0x1F) & ~0x1F), 0x20000, 0);
        *state = 1;
        break;
    case 1:
        sub_02034D8C();
        TextFlags_SetCanTouchSpeedUpPrint(TRUE);
        *state = 0;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

BOOL Gts_Main(OverlayManager *man, int *state) {
    GtsApp *app = OverlayManager_GetData(man);

    ov00_021ECB40();
    GtsServer_Update();
    switch (*state) {
    case 0:
        if (sub_02034DB8()) {
            gGtsGlobals.heap = app->heap;
            ov00_021EC294(Gts_Alloc, Gts_Free);
            *state = 1;
        }
        break;
    case 1:
        *state = sGtsScreens[app->screen].init(app, *state);
        Gts_SetupWindow0();
        if (app->reloadGraphics) {
            Gts_FreeGraphics(app);
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            *state = 3;
        }
        break;
    case 3:
        *state = sGtsScreens[app->screen].main(app, *state);
        break;
    case 4:
        if (IsPaletteFadeFinished()) {
            if (app->reloadGraphics) {
                Gts_InitGraphics(app);
                ov70_02240D74(app, PlayerProfile_GetTrainerGender(app->args->profile));
                ov70_02241184(app, app->unk_0128, 0);
                ov70_02239C6C(app);
                ov70_02239CF8(app);
                app->reloadGraphics = FALSE;
            }
            *state = sGtsScreens[app->screen].exit(app, *state);
        }
        break;
    case 5:
        return TRUE;
    }
    Gts_DecrementTimer(app);
    Gts_CountBoxMonsStep(app);
    if (app->spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(app->spriteList);
    }
    return FALSE;
}

#pragma force_active reset

BOOL Gts_Exit(OverlayManager *man, int *state) {
    GtsApp *app = OverlayManager_GetData(man);

    Heap_Free(app->heapMemory);
    UnloadOVY38();
    UnloadDwcOverlay();
    Gts_FreeGraphics(app);
    DestroyMsgData(app->speciesNames);
    DestroyMsgData(app->wifiErrorMsgData);
    DestroyMsgData(app->wifiMsgData);
    DestroyMsgData(app->gtsMsgData);
    DestroyMsgData(app->countryNames);
    MessageFormat_Delete(app->msgFormat);
    ov70_0223887C(app);
    sub_02034DE0();
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    Heap_Free(app->bgConfig);
    Heap_Free(app->args);
    OverlayManager_FreeData(man);
    FontID_Release(4);
    reg_GX_DISPCNT &= 0xFFFF1FFF;
    reg_GXS_DB_DISPCNT &= 0xFFFF1FFF;
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_61);
    return TRUE;
}

#pragma force_active on

void Gts_VBlankCB(void *arg) {
    GtsApp *app = arg;
    // OS_SetIrqCheckFlag(OS_IE_V_BLANK), with the DTCM base as a constant
    vu32 *dtcm = (vu32 *)0x027E0000;

    if (app->vblankTask != NULL) {
        app->vblankTask(app);
        app->vblankTask = NULL;
    }
    if (app->vblankFunc != NULL) {
        app->vblankFunc(app);
    }
    DoScheduledBgGpuUpdates(app->bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    dtcm[0x3FF8 / 4] |= OS_IE_V_BLANK;
}

#pragma force_active reset

void Gts_SetGfxBanks(void) {
    GraphicsBanks graphicsBanks = sGtsGraphicsBanks;
    GfGfx_SetBanks(&graphicsBanks);
}

void Gts_InitData(GtsApp *app, OverlayManager *man) {
    app->args = OverlayManager_GetArgs(man);
    app->screen = 0;
    Gts_SetNextScreen(app, 0, 0);
    app->unk_011C = 0;
    app->unk_0B8A = 0;
    app->unk_0B8C = 3;
    app->unk_0B8D = 0;
    app->unk_0B8E = 0;
    app->unk_0B90 = 0;
    app->unk_11DC = 0;
    app->unk_0120 = 18;
    app->boxMonCount = 0;
    app->countingBox = 0;
    app->unk_11DE = 0;
    app->waitingIcon = NULL;
    app->unk_12CC = 0;
}

void ov70_0223887C(GtsApp *app) {
}

// Shows window 0 (the top right corner of the main screen, 0xF0-0xFF x 0x00-0x10) on the main screen only.
void Gts_SetupWindow0(void) {
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GX_SetVisibleWnd(GX_WNDMASK_W0);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWnd0Position(0xF0, 0, 0xFF, 0x10);
}

void Gts_InitObjTransfer(void) {
    ObjCharTransferTemplate template = sGtsObjCharTransferTemplate;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(20, HEAP_ID_61);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

void Gts_InitSpriteResources(GtsApp *app) {
    NARC *narc = NARC_New(NARC_a_1_0_0, HEAP_ID_61);
    int i;
    void *rawPalette;
    NNSG2dPaletteData *palette;

    NNS_G2dInitOamManagerModule();
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
    OamManager_Create(0, 0x7A, 0, 0x20, 0, 0x7E, 0, 0x20, HEAP_ID_61);
    app->spriteList = G2dRenderer_Init(0x54, &app->renderer, HEAP_ID_61);
    G2dRenderer_SetSubSurfaceCoords(&app->renderer, 0, FX32_CONST(256));
    for (i = 0; i < 4; i++) {
        app->resourceManagers[i] = Create2DGfxResObjMan(3, (GfGfxResType)i, HEAP_ID_61);
    }

    app->resources[0] = AddCharResObjFromOpenNarc(app->resourceManagers[GF_GFX_RES_TYPE_CHAR], narc, 21, TRUE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_61);
    app->resources[1] = AddPlttResObjFromOpenNarc(app->resourceManagers[GF_GFX_RES_TYPE_PLTT], narc, 10, FALSE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 3, HEAP_ID_61);
    app->resources[2] = AddCellOrAnimResObjFromOpenNarc(app->resourceManagers[GF_GFX_RES_TYPE_CELL], narc, 22, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_61);
    app->resources[3] = AddCellOrAnimResObjFromOpenNarc(app->resourceManagers[GF_GFX_RES_TYPE_ANIM], narc, 23, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_61);
    app->resources[4] = AddCharResObjFromOpenNarc(app->resourceManagers[GF_GFX_RES_TYPE_CHAR], narc, 43, TRUE, 1, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_61);
    app->resources[5] = AddPlttResObjFromOpenNarc(app->resourceManagers[GF_GFX_RES_TYPE_PLTT], narc, 9, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 10, HEAP_ID_61);
    app->resources[6] = AddCellOrAnimResObjFromOpenNarc(app->resourceManagers[GF_GFX_RES_TYPE_CELL], narc, 44, TRUE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_61);
    app->resources[7] = AddCellOrAnimResObjFromOpenNarc(app->resourceManagers[GF_GFX_RES_TYPE_ANIM], narc, 45, TRUE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_61);

    SpriteTransfer_CreateCharTransferTask(app->resources[0]);
    SpriteTransfer_CreateCharTransferTask(app->resources[4]);
    SpriteTransfer_CreateExtPlttTransferTask(app->resources[1]);
    SpriteTransfer_CreateExtPlttTransferTask(app->resources[5]);

    // The Pokemon icon palettes, at 0x60 and darkened (each color component halved) at 0xC0.
    rawPalette = GfGfxLoader_GetPlttData(NARC_poketool_icongra_poke_icon, sub_02074490(), &palette, HEAP_ID_61);
    DC_FlushRange(palette->pRawData, 0x60);
    GX_LoadOBJPltt(palette->pRawData, 0x60, 0x60);
    {
        u16 *colors = palette->pRawData;
        int j;
        int red;
        int green;
        int blue;
        for (j = 0; j < 0x30; j++) {
            int color = colors[j];
            red = color & 0x1F;
            green = (color >> 5) & 0x1F;
            blue = (color >> 10) & 0x1F;
            red /= 2;
            green /= 2;
            blue /= 2;
            colors[j] = (blue << 10) | (green << 5) | red;
        }
    }
    DC_FlushRange(palette->pRawData, 0x60);
    GX_LoadOBJPltt(palette->pRawData, 0xC0, 0x60);
    Heap_Free(rawPalette);
    NARC_Delete(narc);
}

void Gts_InitSpriteTemplate(SpriteTemplate *template, GtsApp *app, const SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE screen) {
    template->spriteList = app->spriteList;
    template->header = header;
    template->position.z = 0;
    template->scale.x = FX32_ONE;
    template->scale.y = FX32_ONE;
    template->scale.z = FX32_ONE;
    template->rotation = 0;
    template->drawPriority = 1;
    template->whichScreen = screen;
    template->heapID = HEAP_ID_61;
}

void Gts_InitSpriteResourcesHeaders(GtsApp *app) {
    CreateSpriteResourcesHeader(&app->headers[0], 0, 0, 0, 0, -1, -1, 0, 0, app->resourceManagers[GF_GFX_RES_TYPE_CHAR], app->resourceManagers[GF_GFX_RES_TYPE_PLTT], app->resourceManagers[GF_GFX_RES_TYPE_CELL], app->resourceManagers[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    CreateSpriteResourcesHeader(&app->headers[1], 1, 1, 1, 1, -1, -1, 0, 0, app->resourceManagers[GF_GFX_RES_TYPE_CHAR], app->resourceManagers[GF_GFX_RES_TYPE_PLTT], app->resourceManagers[GF_GFX_RES_TYPE_CELL], app->resourceManagers[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
}

YesNoPrompt *Gts_CreateYesNoPrompt(BgConfig *bgConfig, u8 y, int tileStart, u32 palette, u8 dimScreen) {
    return Gts_CreateYesNoPromptOnBg(bgConfig, y, tileStart, palette, GF_BG_LYR_MAIN_0, dimScreen);
}

// dimScreen: dim the screen the prompt is not on.
YesNoPrompt *Gts_CreateYesNoPromptOnBg(BgConfig *bgConfig, u8 y, int tileStart, u32 palette, int bgId, u8 dimScreen) {
    YesNoPrompt *prompt = YesNoPrompt_Create(HEAP_ID_61);
    YesNoPromptTemplate template;

    if (dimScreen) {
        BOOL mainScreen = FALSE;
        if (bgId <= GF_BG_LYR_MAIN_3) {
            mainScreen = TRUE;
        }
        Gts_DimScreen(mainScreen);
    }
    template.bgConfig = bgConfig;
    template.bgId = bgId;
    template.tileStart = tileStart;
    template.plttSlot = palette;
    template.x = 24;
    template.y = y;
    template.ignoreTouchFlag = 0;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(prompt, &template);
    return prompt;
}

YesNoResponse Gts_HandleYesNoPrompt(GtsApp *app) {
    YesNoResponse response = YesNoPrompt_HandleInput(app->yesNoPrompt);
    if (response == YESNORESPONSE_YES || response == YESNORESPONSE_NO) {
        Gts_ClearDim();
    }
    return response;
}

TouchscreenListMenu *Gts_CreateListMenu(GtsApp *app, u8 numWindows, int y) {
    TouchscreenListMenuHeader header;
    TouchscreenListMenu *menu;

    app->menuSpawner = TouchscreenListMenuSpawner_Create(HEAP_ID_61, NULL);
    MI_CpuFill8(&header, 0, sizeof(header));
    header.template = sGtsListMenuTemplate;
    header.listMenuItems = app->listItems;
    header.bgConfig = app->bgConfig;
    header.numWindows = numWindows;
    menu = TouchscreenListMenu_CreateWithCallback(app->menuSpawner, &header, FALSE, 20, y, 10, 0, Gts_ListMenuCallback, NULL, TRUE);
    Gts_DimScreen(TRUE);
    return menu;
}

void Gts_ListMenuCallback(TouchscreenListMenu *menu, u8 cursorPos, void *callbackArg, int event) {
    switch (event) {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    }
}

void Gts_DestroyListMenu(GtsApp *app) {
    TouchscreenListMenu_Destroy(app->menu);
    TouchscreenListMenuSpawner_Destroy(app->menuSpawner);
    Gts_ClearDim();
}

void Gts_SetState(GtsApp *app, int state, int nextState) {
    app->state = state;
    app->nextState = nextState;
}

#pragma force_active on

void Gts_SetSpritePositionRounded(Sprite *sprite, int x, int y) {
    VecFx32 vec;
    float f;

    if (x > 0) {
        f = 0.5f + (float)(x << FX32_SHIFT);
    } else {
        f = (float)(x << FX32_SHIFT) - 0.5f;
    }
    vec.x = (int)f;
    if (y > 0) {
        f = 0.5f + (float)(y << FX32_SHIFT);
    } else {
        f = (float)(y << FX32_SHIFT) - 0.5f;
    }
    vec.y = (int)f;
    vec.z = 0;
    Sprite_SetMatrix(sprite, &vec);
}

void *Gts_Alloc(void *unused, u32 size, int alignment) {
    OSIntrMode intrMode = OS_DisableInterrupts();
    void *ptr = NNS_FndAllocFromExpHeapEx(gGtsGlobals.heap, size, alignment);
    OS_RestoreInterrupts(intrMode);
    return ptr;
}

void Gts_Free(void *unused, void *ptr) {
    if (ptr != NULL) {
        OSIntrMode intrMode = OS_DisableInterrupts();
        NNS_FndFreeToExpHeap(gGtsGlobals.heap, ptr);
        OS_RestoreInterrupts(intrMode);
    }
}

#pragma force_active reset

int ov70_02238E44(void) {
    return 3 - ov00_021EC9D4();
}

void Gts_SetNextScreen(GtsApp *app, int screen, int arg) {
    app->nextScreen = screen;
    app->nextScreenArg = arg;
}

void Gts_SwitchScreen(GtsApp *app) {
    app->prevScreen = app->screen;
    app->screen = app->nextScreen;
}

int Gts_GetTextFrameDelay(GtsApp *app) {
    return Options_GetTextFrameDelay(app->args->options);
}

#pragma force_active on

void Gts_InitGraphics(GtsApp *app) {
    Gts_SetGfxBanks();
    Gts_InitObjTransfer();
    Gts_InitSpriteResources(app);
    Gts_InitSpriteResourcesHeaders(app);
    Main_SetVBlankIntrCB(Gts_VBlankCB, app);
}

void Gts_FreeGraphics(GtsApp *app) {
    int i;

    ov70_02241308(app);
    SpriteTransfer_DeleteCharTransferTask(app->resources[0]);
    SpriteTransfer_DeleteCharTransferTask(app->resources[4]);
    SpriteTransfer_DeletePlttTransferTask(app->resources[1]);
    SpriteTransfer_DeletePlttTransferTask(app->resources[5]);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(app->resourceManagers[i]);
    }
    SpriteList_Delete(app->spriteList);
    app->spriteList = NULL;
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}

#pragma force_active reset

void Gts_DecrementTimer(GtsApp *app) {
    if (app->timer != 0) {
        app->timer--;
    }
}

void Gts_StartCountingBoxMons(GtsApp *app) {
    app->countingBox = 1;
    app->boxMonCount = 0;
}

void Gts_CountBoxMonsStep(GtsApp *app) {
    u16 box = app->countingBox;
    if (box != 0) {
        PCStorage *pcStorage = app->args->pcStorage;
        app->boxMonCount += PCStorage_CountMonsAndEggsInBox(pcStorage, box - 1);
        app->countingBox++;
        if (app->countingBox == NUM_BOXES + 1) {
            app->countingBox = 0;
        }
    }
}

void Gts_CreateWaitingIcon(GtsApp *app) {
    app->waitingIcon = WaitingIcon_New(&app->waitingIconWindow, 1);
}

void Gts_DeleteWaitingIcon(GtsApp *app) {
    WaitingIcon *icon = app->waitingIcon;
    if (icon != NULL) {
        WaitingIcon_Delete(icon);
        app->waitingIcon = NULL;
    }
}

void Gts_SetSpritePosition(Sprite *sprite, int x, int y) {
    VecFx32 vec;
    vec.x = x << FX32_SHIFT;
    vec.y = y << FX32_SHIFT;
    Sprite_SetMatrix(sprite, &vec);
}

void Gts_DimScreen(BOOL mainScreen) {
    if (mainScreen) {
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, -7);
    } else {
        G2S_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, -7);
    }
}

void Gts_ClearDim(void) {
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
}
