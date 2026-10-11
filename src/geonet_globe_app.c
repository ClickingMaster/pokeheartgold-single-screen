#include "geonet_globe_app.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu.h"
#include "list_menu_2d.h"
#include "list_menu_items.h"
#include "location_gmm_dat.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "player_data.h"
#include "render_text.h"
#include "render_window.h"
#include "save_wifi_history.h"
#include "screen_fade.h"
#include "system.h"
#include "text.h"
#include "unk_02005D10.h"
#include "unk_02026E30.h"

#define GEONET_GLOBE_MAX_LOCATIONS 1024

fx32 FX_Sqrt(fx32 x); // not declared by the lib/include fx headers

enum GeonetGlobeWindow {
    GEONET_GLOBE_WINDOW_MESSAGE,
    GEONET_GLOBE_WINDOW_LIST,
    GEONET_GLOBE_WINDOW_EXIT_BUTTON,
    GEONET_GLOBE_WINDOW_INFO_BUTTON,
    GEONET_GLOBE_WINDOW_LOCATION,
    GEONET_GLOBE_WINDOW_COUNT,
};

// A place marked on the globe: a city/region of a country, at globe angles (x, y).
typedef struct GeonetLocation {
    s16 x;            // 0x00
    s16 y;            // 0x02
    MtxFx33 rotation; // 0x04
    u16 seenState;    // 0x28, also the index of the marker model
    u16 country;      // 0x2A
    u16 region;       // 0x2C
} GeonetLocation;     // size: 0x30

typedef struct GeonetGlobeAngle {
    s32 x;
    s32 y;
    s32 z;
} GeonetGlobeAngle;

typedef struct GeonetGlobeTouch {
    u32 input; // 0x00: PAD_* style bits produced by touch gestures
    u32 state; // 0x04
    u32 x;     // 0x08
    u32 y;     // 0x0C
    s32 dx;    // 0x10
    s32 dy;    // 0x14
    u32 timer; // 0x18
} GeonetGlobeTouch;

typedef struct GeonetGlobe {
    enum HeapID heapID;                                   // 0x0000
    SaveWiFiHistory *wifiHistory;                         // 0x0004
    Options *options;                                     // 0x0008
    u32 numLocations;                                     // 0x000C
    GeonetLocation locations[GEONET_GLOBE_MAX_LOCATIONS]; // 0x0010
    BgConfig *bgConfig;                                   // 0xC010
    Window windows[GEONET_GLOBE_WINDOW_COUNT];            // 0xC014
    struct ListMenu *listMenu;                            // 0xC064
    ListMenuItem *listMenuItems;                          // 0xC068
    struct ListMenu2D *yesNoMenu;                         // 0xC06C
    MsgData *msgData;                                     // 0xC070
    int printState;                                       // 0xC074
    int printerId;                                        // 0xC078
    String *printString;                                  // 0xC07C
    MessageFormat *msgFormat;                             // 0xC080
    NNSG3dRenderObj globeObj;                             // 0xC084
    NNSG3dResMdl *globeMdl;                               // 0xC0D8
    void *globeFile;                                      // 0xC0DC
    NNSG3dRenderObj markerObjs[4];                        // 0xC0E0, indexed by GeonetLocation.seenState (0 unused)
    NNSG3dRenderObj cursorObj;                            // 0xC230
    NNSG3dResMdl *markerMdls[4];                          // 0xC284
    NNSG3dResMdl *cursorMdl;                              // 0xC294
    void *markerFiles[4];                                 // 0xC298
    void *cursorFile;                                     // 0xC2A8
    VecFx32 translation;                                  // 0xC2AC
    VecFx32 globeScale;                                   // 0xC2B8
    GeonetGlobeAngle rotation;                            // 0xC2C4
    VecFx32 markerScale;                                  // 0xC2D0
    Camera *camera;                                       // 0xC2DC
    u8 filler_C2E0[8];                                    // 0xC2E0
    u16 zoomedIn;                                         // 0xC2E8
    u8 filler_C2EA[2];                                    // 0xC2EA
    VecFx32 lightVector;                                  // 0xC2EC
    u32 drawState;                                        // 0xC2F8
    u32 unk_C2FC;                                         // 0xC2FC
    u32 isJapanese;                                       // 0xC300
    u16 freeRotation;                                     // 0xC304
    u8 filler_C306[2];                                    // 0xC306
    GeonetGlobeTouch touch;                               // 0xC308
    u32 playerCountry;                                    // 0xC324
    u32 playerRegion;                                     // 0xC328
    u32 selectedCountry;                                  // 0xC32C
    u32 selectedRegion;                                   // 0xC330
    u32 nonJapanese;                                      // 0xC334
    u32 showLocationName;                                 // 0xC338
} GeonetGlobe;                                            // size: 0xC33C

typedef struct GeonetGlobeMenuEntry {
    int msgId;
    int value;
} GeonetGlobeMenuEntry;

// Location tables in the earth place NARC.
typedef struct GeonetCountryPlace {
    u16 kind;
    s16 x;
    s16 y;
} GeonetCountryPlace;

typedef struct GeonetRegionPlace {
    s16 x;
    s16 y;
} GeonetRegionPlace;

typedef struct GeonetGlobePoint {
    u32 x;
    u32 y;
} GeonetGlobePoint;

typedef union GeonetGlobePosition {
    GeonetGlobePoint point;
    int values[2];
} GeonetGlobePosition;

extern const WindowTemplate _021E7644;
extern const WindowTemplate sGeonetLocationWindowTemplate;
extern const WindowTemplate sGeonetInfoButtonWindowTemplate;
extern const WindowTemplate sGeonetExitButtonWindowTemplate;
extern const WindowTemplate sGeonetYesNoWindowTemplate;
extern const WindowTemplate sGeonetMessageWindowTemplate;
extern const WindowTemplate sGeonetPlaceListWindowTemplate;
extern const VecFx32 sGeonetCameraPosition;
extern const GraphicsModes sGeonetGraphicsModes;
extern const GeonetGlobeMenuEntry sGeonetMenuEntries[];
extern const BgTemplate sGeonetBgTemplate_Text;
extern const BgTemplate sGeonetBgTemplate_Frame;
extern const ListMenuTemplate sGeonetMenuTemplate;
extern const ListMenuTemplate sGeonetPlaceListTemplate;
extern const MtxFx33 sGeonetGlobeRotationInit;
extern const MtxFx33 sGeonetMarkerRotationInit;
extern const MtxFx33 sGeonetLocationRotationInit;
extern const MtxFx33 sGeonetCursorRotation;
extern const GraphicsBanks sGeonetGraphicsBanks;

void GeonetGlobe_SetGfxBanks(void);
void GeonetGlobe_SetGraphicsModes(void);
void GeonetGlobe_LoadLocations(GeonetGlobe *globe);
void GeonetGlobe_SetLocation(GeonetGlobe *globe, u32 index, s32 x, s32 y, u16 country, u16 region);
void GeonetGlobe_MarkPlayerLocation(GeonetGlobe *globe);
int GeonetGlobe_GetPlaceIndex(int country);
void GeonetGlobe_HandleTouch(GeonetGlobe *globe);
void GeonetGlobe_GetTouchDrag(int x, int y, u32 *dirX, u32 *distX, u32 *dirY, u32 *distY);
void GeonetGlobe_InitBgs(GeonetGlobe *globe, NARC *narc);
void GeonetGlobe_FreeBgs(GeonetGlobe *globe);
BOOL GeonetGlobe_PrintMessage(GeonetGlobe *globe, int msgId, BOOL noWait);
void GeonetGlobe_ListMenuMoveCursor(struct ListMenu *menu, s32 index, u8 onInit);
void GeonetGlobe_CreateMenu(GeonetGlobe *globe, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *menuTemplate, const GeonetGlobeMenuEntry *entries);
void GeonetGlobe_CreatePlaceList(GeonetGlobe *globe, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *menuTemplate, int msgBank, const u8 *msgIds, u32 count);
void GeonetGlobe_DestroyListMenu(GeonetGlobe *globe);
void GeonetGlobe_ShowPlayerLocation(GeonetGlobe *globe);
void GeonetGlobe_ShowLocation(GeonetGlobe *globe, int country, int region);
void GeonetGlobe_HideLocationWindow(GeonetGlobe *globe);
BOOL GeonetGlobe_FindNearestLocation(GeonetGlobe *globe, int *index);
void GeonetGlobe_UpdateLocationName(GeonetGlobe *globe);
void GeonetGlobe_LoadModels(GeonetGlobe *globe, NARC *narc);
void GeonetGlobe_FreeModels(GeonetGlobe *globe);
void GeonetGlobe_ResetTransform(GeonetGlobe *globe);
void GeonetGlobe_InitCamera(GeonetGlobe *globe);
void GeonetGlobe_InitLight(GeonetGlobe *globe);
void GeonetGlobe_CalcGlobeRotation(MtxFx33 *dst, const GeonetGlobeAngle *angle);
void GeonetGlobe_CalcLocationRotation(MtxFx33 *dst, const GeonetGlobeAngle *angle);
BOOL GeonetGlobe_HandleRotationInput(GeonetGlobe *globe, u32 newKeys, u32 heldKeys);
BOOL GeonetGlobe_StepZoom(GeonetGlobe *globe);
void GeonetGlobe_Draw(GeonetGlobe *globe);
BOOL GeonetGlobe_GetLocationNames(int country, int region, String *countryName, String *regionName, enum HeapID heapID);
BOOL GeonetGlobe_CountryHasRegions(int country);
void GeonetGlobe_WrapPosition(int *position);
int GeonetGlobe_CalcDistance(const GeonetGlobePoint *a, const GeonetGlobePoint *b);

BOOL GeonetGlobe_Init(OverlayManager *man, int *state) {
    GeonetGlobe *globe;
    SaveData *saveData;
    Options *options;

    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    Heap_Create(HEAP_ID_3, HEAP_ID_49, 0x80000);
    globe = OverlayManager_CreateAndGetData(man, sizeof(GeonetGlobe), HEAP_ID_49);
    memset(globe, 0, sizeof(GeonetGlobe));
    globe->heapID = HEAP_ID_49;
    globe->isJapanese = gGameLanguage == LANGUAGE_JAPANESE;
    saveData = OverlayManager_GetArgs(man);
    globe->wifiHistory = Save_WiFiHistory_Get(saveData);
    globe->playerCountry = WifiHistory_GetPlayerCountry(globe->wifiHistory);
    globe->playerRegion = WiFiHistory_GetPlayerRegion(globe->wifiHistory);
    globe->nonJapanese = WiFiHistory_GetNonJapaneseFlag(globe->wifiHistory);
    options = Save_PlayerData_GetOptionsAddr(saveData);
    globe->options = options;
    GeonetGlobe_SetGfxBanks();
    GeonetGlobe_SetGraphicsModes();
    GF3dRender_InitSimpleManager((enum HeapID)(u8)globe->heapID);
    globe->bgConfig = BgConfig_Alloc(globe->heapID);
    GfGfx_BothDispOn();
    ResetAllTextPrinters();
    globe->msgFormat = MessageFormat_New_Custom(8, 64, globe->heapID);
    globe->camera = Camera_New(globe->heapID);
    globe->drawState = 0;
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    SetKeyRepeatTimers(4, 8);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    GeonetGlobe_LoadLocations(globe);
    return TRUE;
}

#pragma force_active on

BOOL GeonetGlobe_Main(OverlayManager *man, int *state) {
    GeonetGlobe *globe = OverlayManager_GetData(man);
    BOOL result = FALSE;
    int choice;
    int placeIndex;
    int count;
    int msgBank;
    const u8 *msgIds;
    u32 keys;
    u32 touchInput;
    int nearestLocation;
    NARC *narc;

    switch (*state) {
    case 14:
    case 15:
    case 16:
        break;
    default:
        if (System_GetTouchNew()) {
            gSystem.simulatedInputs = 1;
        }
        break;
    }

    switch (*state) {
    case 0:
        globe->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0186_bin, globe->heapID);
        narc = NARC_New(NARC_a_1_2_3, globe->heapID);
        GeonetGlobe_LoadModels(globe, narc);
        GeonetGlobe_InitBgs(globe, narc);
        NARC_Delete(narc);
        globe->unk_C2FC = 0;
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, globe->heapID);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        *state = 1;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            *state = 2;
        }
        break;
    case 2:
        if (GeonetGlobe_PrintMessage(globe, 0, TRUE) == TRUE) {
            if (globe->playerCountry == 0) {
                *state = 3;
            } else {
                *state = 14;
            }
        }
        break;
    case 3:
        if (GeonetGlobe_PrintMessage(globe, 1, TRUE) == TRUE) {
            GeonetGlobe_CreateMenu(globe, &globe->windows[GEONET_GLOBE_WINDOW_LIST], &_021E7644, &sGeonetMenuTemplate, sGeonetMenuEntries);
            *state = 4;
        }
        break;
    case 4:
        choice = ListMenu_ProcessInput(globe->listMenu);
        if (choice != LIST_NOTHING_CHOSEN) {
            GeonetGlobe_DestroyListMenu(globe);
            PlaySE(SEQ_SE_DP_SELECT);
            switch (choice) {
            case 0:
            case LIST_NOTHING_CHOSEN:
            default:
                *state = 14;
                break;
            case 1:
                *state = 5;
                break;
            case LIST_CANCEL:
            case 2:
                *state = 17;
                break;
            }
        }
        break;
    case 5:
        if (GeonetGlobe_PrintMessage(globe, 2, TRUE) == TRUE) {
            globe->yesNoMenu = Std_CreateYesNoMenu(globe->bgConfig, &sGeonetYesNoWindowTemplate, 473, 7, globe->heapID);
            *state = 6;
        }
        break;
    case 6:
        choice = Handle2dMenuInput_DeleteOnFinish(globe->yesNoMenu, globe->heapID);
        if (choice == 0) {
            goto case6_yes;
        }
        if (choice == LIST_CANCEL) {
            goto case6_no;
        }
        break;
case6_yes:
        if (globe->isJapanese == TRUE) {
            globe->selectedCountry = 103;
            *state = 9;
        } else {
            *state = 7;
        }
        break;
case6_no:
        *state = 3;
        break;
    case 7:
        if (GeonetGlobe_PrintMessage(globe, 3, TRUE) == TRUE) {
            globe->selectedCountry = 0;
            msgIds = LocationGmmDatGetDistrictNameMsgIdsPtr(0);
            count = LocationGmmDatGetDistrictCount(0);
            GeonetGlobe_CreatePlaceList(globe, &globe->windows[GEONET_GLOBE_WINDOW_LIST], &sGeonetPlaceListWindowTemplate, &sGeonetPlaceListTemplate, NARC_msg_msg_0798_bin, msgIds, count);
            *state = 8;
        }
        break;
    case 8:
        choice = ListMenu_ProcessInput(globe->listMenu);
        if (choice != LIST_NOTHING_CHOSEN) {
            GeonetGlobe_DestroyListMenu(globe);
            PlaySE(SEQ_SE_DP_SELECT);
            if (choice != LIST_CANCEL) {
                choice = LocationGmmDatGetDistrictNameMsgIdsPtr(0)[choice];
            }
            if (choice != LIST_CANCEL) {
                globe->selectedCountry = choice;
                if (GeonetGlobe_CountryHasRegions(globe->selectedCountry) == TRUE) {
                    *state = 9;
                } else {
                    globe->selectedRegion = 0;
                    *state = 11;
                }
            } else {
                *state = 3;
            }
        }
        break;
    case 9:
        if (GeonetGlobe_PrintMessage(globe, 4, TRUE) == TRUE) {
            globe->selectedRegion = 0;
            placeIndex = LocationGmmDatIndexGetByCountryMsgNo(globe->selectedCountry);
            msgBank = LocationGmmDatGetGmmNo(placeIndex);
            msgIds = LocationGmmDatGetDistrictNameMsgIdsPtr(placeIndex);
            count = LocationGmmDatGetDistrictCount(placeIndex);
            GeonetGlobe_CreatePlaceList(globe, &globe->windows[GEONET_GLOBE_WINDOW_LIST], &sGeonetPlaceListWindowTemplate, &sGeonetPlaceListTemplate, msgBank, msgIds, count);
            *state = 10;
        }
        break;
    case 10:
        choice = ListMenu_ProcessInput(globe->listMenu);
        if (choice != LIST_NOTHING_CHOSEN) {
            GeonetGlobe_DestroyListMenu(globe);
            PlaySE(SEQ_SE_DP_SELECT);
            if (choice != LIST_CANCEL) {
                choice = LocationGmmDatGetDistrictNameMsgIdsPtr(LocationGmmDatIndexGetByCountryMsgNo(globe->selectedCountry))[choice];
            }
            if (choice != LIST_CANCEL) {
                globe->selectedRegion = choice;
                *state = 11;
            } else if (globe->isJapanese == TRUE) {
                *state = 3;
            } else {
                *state = 7;
            }
        }
        break;
    case 11:
        GeonetGlobe_ShowLocation(globe, globe->selectedCountry, globe->selectedRegion);
        *state = 12;
        break;
    case 12:
        if (GeonetGlobe_PrintMessage(globe, 5, TRUE) == TRUE) {
            globe->yesNoMenu = Std_CreateYesNoMenu(globe->bgConfig, &sGeonetYesNoWindowTemplate, 473, 7, globe->heapID);
            *state = 13;
        }
        break;
    case 13:
        choice = Handle2dMenuInput_DeleteOnFinish(globe->yesNoMenu, globe->heapID);
        if (choice == 0) {
            goto case13_yes;
        }
        if (choice == LIST_CANCEL) {
            goto case13_no;
        }
        break;
case13_yes:
        GeonetGlobe_HideLocationWindow(globe);
        WiFiHistory_SetPlayerGlobeInfo(globe->wifiHistory, globe->selectedCountry, globe->selectedRegion);
        globe->playerCountry = globe->selectedCountry;
        globe->playerRegion = globe->selectedRegion;
        *state = 14;
        break;
case13_no:
        GeonetGlobe_HideLocationWindow(globe);
        *state = 3;
        break;
    case 14:
        if (globe->isJapanese == TRUE && globe->nonJapanese == FALSE) {
            globe->freeRotation = FALSE;
        } else {
            globe->freeRotation = TRUE;
        }
        GeonetGlobe_ResetTransform(globe);
        GeonetGlobe_MarkPlayerLocation(globe);
        GeonetGlobe_InitCamera(globe);
        GeonetGlobe_InitLight(globe);
        FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 15, 0, 0, 216, 32);
        DrawFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON], FALSE, 473, 7);
        if (globe->playerCountry != 0) {
            GeonetGlobe_ShowPlayerLocation(globe);
        }
        GeonetGlobe_UpdateLocationName(globe);
        globe->showLocationName = FALSE;
        globe->drawState = 1;
        *state = 15;
        break;
    case 15: {
        u16 wasZoomedIn = globe->zoomedIn;

        GeonetGlobe_HandleTouch(globe);
        if (GeonetGlobe_FindNearestLocation(globe, &nearestLocation)) {
            DrawFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON], FALSE, 473, 7);
        } else {
            EraseFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON], FALSE);
        }
        keys = gSystem.newKeys;
        if ((keys & PAD_BUTTON_B) || ((touchInput = globe->touch.input) & PAD_BUTTON_B)) {
            EraseFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON], FALSE);
            EraseFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON], FALSE);
            PlaySE(SEQ_SE_DP_DECIDE);
            FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 15, 0, 0, 216, 32);
            if (globe->playerCountry == 0) {
                globe->drawState = 2;
                *state = 3;
            } else {
                GeonetGlobe_HideLocationWindow(globe);
                *state = 17;
            }
            break;
        }
        if ((keys & PAD_BUTTON_X) || (touchInput & PAD_BUTTON_X)) {
            if (globe->showLocationName == FALSE) {
                globe->showLocationName = TRUE;
                GeonetGlobe_UpdateLocationName(globe);
                if (globe->showLocationName == TRUE) {
                    PlaySE(SEQ_SE_DP_DECIDE);
                }
                break;
            }
        }
        if (((keys & (PAD_BUTTON_X | PAD_BUTTON_A | PAD_BUTTON_B)) || (touchInput & PAD_BUTTON_X)) && globe->showLocationName == TRUE) {
            globe->showLocationName = FALSE;
            GeonetGlobe_UpdateLocationName(globe);
            break;
        }
        if (GeonetGlobe_HandleRotationInput(globe, keys, gSystem.heldKeys) == TRUE && globe->showLocationName == TRUE) {
            globe->showLocationName = FALSE;
            GeonetGlobe_UpdateLocationName(globe);
        }
        if (wasZoomedIn != globe->zoomedIn) {
            *state = 16;
            PlaySE(SEQ_SE_PL_FTIMER03);
        }
        break;
    }
    case 16:
        if (GeonetGlobe_StepZoom(globe) == TRUE) {
            *state = 15;
        }
        break;
    case 17:
        globe->unk_C2FC = 0;
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, globe->heapID);
        *state = 18;
        break;
    case 18:
        if (IsPaletteFadeFinished() == TRUE) {
            globe->drawState = 1;
            GeonetGlobe_FreeBgs(globe);
            GeonetGlobe_FreeModels(globe);
            DestroyMsgData(globe->msgData);
            *state = 0;
            result = TRUE;
        }
        break;
    }
    GeonetGlobe_Draw(globe);
    return result;
}

#pragma force_active reset

BOOL GeonetGlobe_Exit(OverlayManager *man, int *state) {
    GeonetGlobe *globe = OverlayManager_GetData(man);
    enum HeapID heapID = globe->heapID;

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    Camera_Delete(globe->camera);
    MessageFormat_Delete(globe->msgFormat);
    GF3dRender_DeleteSimpleManager();
    Heap_Free(globe->bgConfig);
    Main_SetVBlankIntrCB(NULL, NULL);
    OverlayManager_FreeData(man);
    Heap_Destroy(heapID);
    gSystem.screensFlipped = FALSE;
    return TRUE;
}

void GeonetGlobe_SetGfxBanks(void) {
    GraphicsBanks banks = sGeonetGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void GeonetGlobe_SetGraphicsModes(void) {
    GraphicsModes modes = sGeonetGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
}

void GeonetGlobe_LoadLocations(GeonetGlobe *globe) {
    u32 countrySize;
    u32 regionSize;
    NARC *narc;
    void *countryFile;
    void *regionFile;
    const GeonetCountryPlace *country;
    const GeonetRegionPlace *region;
    s32 numCountries;
    s32 numEntries;
    s32 i;
    s32 j;
    GeonetGlobe *work;

    work = globe;
    narc = NARC_New(NARC_a_0_9_8, work->heapID);
    work->numLocations = 0;
    countryFile = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(narc, 18, FALSE, work->heapID, FALSE, &countrySize);
    numEntries = countrySize / sizeof(GeonetCountryPlace);
    country = (const GeonetCountryPlace *)((u8 *)countryFile + sizeof(GeonetCountryPlace));
    i = 1;
    for (; i < numEntries; i++) {
        if (country->kind != 2) {
            GeonetGlobe_SetLocation(work, work->numLocations, country->x, country->y, (u16)i, 0);
            work->numLocations++;
        }
        country++;
    }
    Heap_Free(countryFile);
    i = 1;
    numCountries = LocationGmmDatCountGet();
    for (; i < numCountries; i++) {
        regionFile = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(narc, LocationGmmDatGetEarthPlaceDatId(i), FALSE, work->heapID, FALSE, &regionSize);
        numEntries = regionSize / sizeof(GeonetRegionPlace);
        region = (const GeonetRegionPlace *)((u8 *)regionFile + sizeof(GeonetRegionPlace));
        j = 1;
        for (; j < numEntries; j++) {
            GeonetGlobe_SetLocation(work, work->numLocations, region->x, region->y, (u16)LocationGmmDatGetCountryMsgNo(i), (u16)j);
            work->numLocations++;
            region++;
        }
        Heap_Free(regionFile);
    }
    NARC_Delete(narc);
}

void GeonetGlobe_SetLocation(GeonetGlobe *globe, u32 index, s32 x, s32 y, u16 country, u16 region) {
    GeonetGlobe *work;
    MtxFx33 rotation;
    GeonetGlobeAngle angle;

    work = globe;
    rotation = sGeonetLocationRotationInit;
    work->locations[index].x = (s16)x;
    work->locations[index].y = (s16)y;
    angle.x = x;
    angle.y = y;
    angle.z = 0;
    GeonetGlobe_CalcLocationRotation(&rotation, &angle);
    work->locations[index].rotation = rotation;
    work->locations[index].seenState = (s16)WiFiHistory_GetLocationSeenState(work->wifiHistory, country, region);
    work->locations[index].country = (s16)country;
    work->locations[index].region = (s16)region;
}

void GeonetGlobe_MarkPlayerLocation(GeonetGlobe *globe) {
    u32 i;
    GeonetGlobe *shifted;

    for (i = 0; i < globe->numLocations; i++) {
        // The work pointer shifted by i locations: locations[0] through it is locations[i].
        shifted = (GeonetGlobe *)((u8 *)globe + i * sizeof(GeonetLocation));
        if (globe->playerCountry == shifted->locations[0].country && globe->playerRegion == shifted->locations[0].region) {
            shifted->locations[0].seenState = 3;
            globe->rotation.x = shifted->locations[0].x;
            globe->rotation.y = shifted->locations[0].y;
        }
    }
}

int GeonetGlobe_GetPlaceIndex(int country) {
    return LocationGmmDatIndexGetByCountryMsgNo(country);
}

// Turns touch input into button bits: the two touch buttons (B / X), a short tap (A) and drags (D-pad).
void GeonetGlobe_HandleTouch(GeonetGlobe *globe) {
    u32 button = 0;

    globe->touch.input = 0;
    if (gSystem.touchX >= 192 && gSystem.touchX <= 256 && gSystem.touchY >= 160 && gSystem.touchY <= 184) {
        button = PAD_BUTTON_B;
    } else if (gSystem.touchX <= 64 && gSystem.touchY >= 160 && gSystem.touchY <= 184) {
        button = PAD_BUTTON_X;
    }
    if (gSystem.touchNew) {
        if (button) {
            globe->touch.input = button;
            return;
        }
        globe->touch.state = 0;
        globe->touch.dx = 0;
        globe->touch.dy = 0;
        globe->touch.timer = 0;
        globe->touch.input = 0;
        globe->touch.x = gSystem.touchX;
        globe->touch.y = gSystem.touchY;
        globe->touch.timer = 4;
    }
    if (gSystem.touchHeld) {
        switch (globe->touch.state) {
        case 0:
            if (globe->touch.timer == 0) {
                globe->touch.state++;
            } else {
                globe->touch.timer--;
            }
        case 1:
            break;
        default:
            return;
        }
        if (button == 0) {
            u32 dirX, distX, dirY, distY;
            GeonetGlobe_GetTouchDrag(globe->touch.x, globe->touch.y, &dirX, &distX, &dirY, &distY);
            globe->touch.input = dirX | dirY;
            globe->touch.dx = distX;
            globe->touch.dy = distY;
            globe->touch.x = gSystem.touchX;
            globe->touch.y = gSystem.touchY;
        }
    } else {
        if (button == 0 && globe->touch.timer != 0 && globe->touch.dx < 4 && globe->touch.dy < 4) {
            globe->touch.input = PAD_BUTTON_A;
        }
        globe->touch.state = 0;
        globe->touch.dx = 0;
        globe->touch.dy = 0;
        globe->touch.timer = 0;
    }
}

void GeonetGlobe_GetTouchDrag(int x, int y, u32 *dirX, u32 *distX, u32 *dirY, u32 *distY) {
    u32 horizontal = 0;
    u32 vertical = 0;
    int dx = 0;
    int dy = 0;

    if (gSystem.touchX != 0xFFFF) {
        dx = (int)gSystem.touchX - x;
        if (dx < 0) {
            dx ^= -1;
            horizontal = PAD_KEY_RIGHT;
        } else if (dx > 0) {
            horizontal = PAD_KEY_LEFT;
        }
    }
    *dirX = horizontal;
    *distX = dx & 0x3F;
    if (gSystem.touchY != 0xFFFF) {
        dy = (int)gSystem.touchY - y;
        if (dy < 0) {
            dy ^= -1;
            vertical = PAD_KEY_DOWN;
        } else if (dy > 0) {
            vertical = PAD_KEY_UP;
        }
    }
    *dirY = vertical;
    *distY = dy & 0x3F;
}

#pragma force_active on

void GeonetGlobe_InitBgs(GeonetGlobe *globe, NARC *narc) {
    u16 colors[4];
    String *string;

    InitBgFromTemplate(globe->bgConfig, GF_BG_LYR_SUB_2, &sGeonetBgTemplate_Text, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(globe->bgConfig, GF_BG_LYR_SUB_2);
    InitBgFromTemplate(globe->bgConfig, GF_BG_LYR_SUB_3, &sGeonetBgTemplate_Frame, GF_BG_TYPE_TEXT);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, globe->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, globe->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 6, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0, 0x80, globe->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 7, globe->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, globe->heapID);
    LoadUserFrameGfx2(globe->bgConfig, GF_BG_LYR_SUB_2, 482, 6, (u8)Options_GetFrame(globe->options), globe->heapID);
    LoadUserFrameGfx1(globe->bgConfig, GF_BG_LYR_SUB_2, 473, 7, 0, globe->heapID);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x80, globe->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, globe->heapID);
    BG_SetMaskColor(GF_BG_LYR_SUB_2, RGB(19, 26, 17));
    AddWindow(globe->bgConfig, &globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], &sGeonetMessageWindowTemplate);
    FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 15, 0, 0, 216, 32);
    DrawFrameAndWindow2(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], FALSE, 482, 6);
    globe->printState = 0;

    InitBgFromTemplate(globe->bgConfig, GF_BG_LYR_MAIN_2, &sGeonetBgTemplate_Text, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(globe->bgConfig, GF_BG_LYR_MAIN_2);
    InitBgFromTemplate(globe->bgConfig, GF_BG_LYR_MAIN_3, &sGeonetBgTemplate_Frame, GF_BG_TYPE_TEXT);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, globe->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, globe->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 6, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0, 0x80, globe->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 7, globe->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, globe->heapID);
    LoadUserFrameGfx1(globe->bgConfig, GF_BG_LYR_MAIN_2, 473, 7, 0, globe->heapID);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x80, globe->heapID);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x1E0, globe->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, globe->heapID);
    BG_SetMaskColor(GF_BG_LYR_MAIN_2, RGB_BLACK);
    string = String_New(16, globe->heapID);
    FontID_Alloc(4, globe->heapID);
    colors[3] = RGB_WHITE;
    colors[2] = RGB(7, 7, 7);
    colors[1] = RGB(18, 19, 19);
    colors[0] = RGB_WHITE;
    BG_LoadPlttData(GF_BG_LYR_MAIN_2, &colors[3], sizeof(u16), 482);
    BG_LoadPlttData(GF_BG_LYR_MAIN_2, &colors[2], sizeof(u16), 484);
    BG_LoadPlttData(GF_BG_LYR_MAIN_2, &colors[1], sizeof(u16), 486);
    BG_LoadPlttData(GF_BG_LYR_MAIN_2, &colors[0], sizeof(u16), 510);
    AddWindow(globe->bgConfig, &globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON], &sGeonetExitButtonWindowTemplate);
    FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON], 15, 0, 0, 216, 32);
    ReadMsgDataIntoString(globe->msgData, 16, string);
    AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON], 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    AddWindow(globe->bgConfig, &globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON], &sGeonetInfoButtonWindowTemplate);
    FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON], 15, 0, 0, 216, 32);
    ReadMsgDataIntoString(globe->msgData, 13, string);
    AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON], 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    String_Delete(string);
    FontID_Release(4);
}

#pragma force_active reset

void GeonetGlobe_FreeBgs(GeonetGlobe *globe) {
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON]);
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON]);
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE]);
    FreeBgTilemapBuffer(globe->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(globe->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(globe->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(globe->bgConfig, GF_BG_LYR_SUB_3);
}

#pragma force_active on

BOOL GeonetGlobe_PrintMessage(GeonetGlobe *globe, int msgId, BOOL noWait) {
    BOOL done = FALSE;

    switch (globe->printState) {
    case 0:
        FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 15, 0, 0, 216, 32);
        globe->printString = String_New(1024, globe->heapID);
        ReadMsgDataIntoString(globe->msgData, msgId, globe->printString);
        globe->printerId = AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 1, globe->printString, 0, 0, Options_GetTextFrameDelay(globe->options), NULL);
        globe->printState = 1;
        break;
    case 1:
        if (!TextPrinterCheckActive((u8)globe->printerId)) {
            String_Delete(globe->printString);
            globe->printState = 2;
        }
        break;
    case 2:
        if (noWait || (gSystem.newKeys & PAD_BUTTON_A)) {
            globe->printState = 0;
            done = TRUE;
        }
        break;
    }
    return done;
}

#pragma force_active reset

void GeonetGlobe_ListMenuMoveCursor(struct ListMenu *menu, s32 index, u8 onInit) {
    if (onInit == FALSE) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

#pragma force_active on

void GeonetGlobe_CreateMenu(GeonetGlobe *globe, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *menuTemplate, const GeonetGlobeMenuEntry *entries) {
    ListMenuTemplate template;
    int i;

    AddWindow(globe->bgConfig, window, windowTemplate);
    globe->listMenuItems = ListMenuItems_New(menuTemplate->totalItems, globe->heapID);
    i = 0;
    if ((int)menuTemplate->totalItems > 0) {
        const GeonetGlobeMenuEntry *entry = entries;
        do {
            ListMenuItems_AppendFromMsgData(globe->listMenuItems, globe->msgData, entry->msgId, entry->value);
            i++;
            entry++;
        } while (i < menuTemplate->totalItems);
    }
    template = *menuTemplate;
    template.items = globe->listMenuItems;
    template.window = window;
    template.moveCursorFunc = GeonetGlobe_ListMenuMoveCursor;
    globe->listMenu = ListMenuInit(&template, 0, 0, (enum HeapID)(u8)globe->heapID);
    DrawFrameAndWindow1(template.window, TRUE, 473, 7);
    CopyWindowToVram(window);
}

void GeonetGlobe_CreatePlaceList(GeonetGlobe *globe, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *menuTemplate, int msgBank, const u8 *msgIds, u32 count) {
    ListMenuTemplate template;
    MsgData *msgData;
    u32 i;

    AddWindow(globe->bgConfig, window, windowTemplate);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, msgBank, globe->heapID);
    globe->listMenuItems = ListMenuItems_New(count, globe->heapID);
    for (i = 0; i < count; i++) {
        ListMenuItems_AppendFromMsgData(globe->listMenuItems, msgData, msgIds[i], i);
    }
    DestroyMsgData(msgData);
    template = *menuTemplate;
    template.items = globe->listMenuItems;
    template.totalItems = count;
    template.window = window;
    template.moveCursorFunc = GeonetGlobe_ListMenuMoveCursor;
    globe->listMenu = ListMenuInit(&template, 0, 0, (enum HeapID)(u8)globe->heapID);
    DrawFrameAndWindow1(template.window, TRUE, 473, 7);
    CopyWindowToVram(window);
}

#pragma force_active reset

void GeonetGlobe_DestroyListMenu(GeonetGlobe *globe) {
    EraseFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_LIST], FALSE);
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_LIST]);
    DestroyListMenu(globe->listMenu, NULL, NULL);
    ListMenuItems_Delete(globe->listMenuItems);
}

void GeonetGlobe_ShowPlayerLocation(GeonetGlobe *globe) {
    String *string = String_New(1024, globe->heapID);
    String *template = String_New(1024, globe->heapID);

    AddWindow(globe->bgConfig, &globe->windows[GEONET_GLOBE_WINDOW_LOCATION], &sGeonetLocationWindowTemplate);
    FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], 15, 0, 0, 216, 48);
    DrawFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], FALSE, 473, 7);
    BufferCountryName(globe->msgFormat, 0, globe->playerCountry);
    BufferCityName(globe->msgFormat, 1, globe->playerCountry, globe->playerRegion);
    ReadMsgDataIntoString(globe->msgData, 14, template);
    StringExpandPlaceholders(globe->msgFormat, string, template);
    AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], 0, string, 0, 0, TEXT_SPEED_INSTANT, NULL);
    String_Delete(template);
    String_Delete(string);
    CopyWindowToVram(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION]);
}

void GeonetGlobe_ShowLocation(GeonetGlobe *globe, int country, int region) {
    String *countryName = String_New(64, globe->heapID);
    String *regionName = String_New(64, globe->heapID);

    AddWindow(globe->bgConfig, &globe->windows[GEONET_GLOBE_WINDOW_LOCATION], &sGeonetLocationWindowTemplate);
    FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], 15, 0, 0, 216, 48);
    DrawFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], FALSE, 473, 7);
    GeonetGlobe_GetLocationNames(country, region, countryName, regionName, globe->heapID);
    if (region != 0) {
        AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], 0, regionName, 0, 16, TEXT_SPEED_NOTRANSFER, NULL);
    }
    AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], 0, countryName, 0, 0, TEXT_SPEED_INSTANT, NULL);
    String_Delete(regionName);
    String_Delete(countryName);
    CopyWindowToVram(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION]);
}

void GeonetGlobe_HideLocationWindow(GeonetGlobe *globe) {
    EraseFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], FALSE);
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION]);
}

#pragma force_active on

// Finds the seen location nearest to the globe's current orientation (within 128 units in x and y).
BOOL GeonetGlobe_FindNearestLocation(GeonetGlobe *globe, int *index) {
    BOOL found = FALSE;
    int posX = globe->rotation.x;
    int posY = globe->rotation.y;
    int minX = (s16)((s32)((u32)posX - 128));
    int maxX = (s16)((s32)((u32)posX + 128));
    int minY = (s16)((s32)((u32)posY - 128));
    int maxY = (s16)((s32)((u32)posY + 128));
    int minY2;
    int maxY2;
    GeonetGlobePosition origin;
    GeonetGlobePosition position;
    u32 bestDistance = 256;
    u32 best = globe->numLocations;
    u32 i;

    origin.values[0] = (int)globe->rotation.x;
    origin.values[1] = (int)globe->rotation.y;
    GeonetGlobe_WrapPosition(origin.values);
    {
        // The y range wrapped around: also search the range on the other side of the seam.
        int range = maxY - minY;
        if (range < 0) {
            range = -range;
        }
        if (range > 256) {
            maxY2 = minY2 = (s16)globe->rotation.y;
            if (minY2 > 0) {
                maxY = minY = minY2 - 65536;
            } else {
                maxY = minY = minY2 + 65536;
            }
            minY2 -= 128;
            maxY2 += 128;
            minY -= 128;
            maxY += 128;
        } else {
            minY2 = minY;
            maxY2 = maxY;
        }
    }
    for (i = 0; i < globe->numLocations; i++) {
        if (globe->locations[i].x > minX && globe->locations[i].x < maxX
            && ((globe->locations[i].y > minY && globe->locations[i].y < maxY) || (globe->locations[i].y > minY2 && globe->locations[i].y < maxY2))
            && globe->locations[i].seenState) {
            u32 distance;
            position.point.x = (u32)globe->locations[i].x;
            position.point.y = (u32)globe->locations[i].y;
            GeonetGlobe_WrapPosition(position.values);
            distance = (u32)GeonetGlobe_CalcDistance(&origin.point, &position.point);
            if (distance < bestDistance) {
                bestDistance = distance;
                best = i;
            }
        }
    }
    *index = (int)best;
    if (best != globe->numLocations) {
        found = TRUE;
    }
    return found;
}

void GeonetGlobe_UpdateLocationName(GeonetGlobe *globe) {
    String *countryName;
    String *regionName;
    int index;

    if (globe->showLocationName == FALSE) {
        String *string = String_New(1024, globe->heapID);
        FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 15, 0, 0, 216, 48);
        ReadMsgDataIntoString(globe->msgData, 15, string);
        AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 1, string, 0, 0, TEXT_SPEED_INSTANT, NULL);
        String_Delete(string);
        return;
    }
    if (GeonetGlobe_FindNearestLocation(globe, &index)) {
        countryName = String_New(64, globe->heapID);
        regionName = String_New(64, globe->heapID);
        FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 15, 0, 0, 216, 48);
        GeonetGlobe_GetLocationNames(globe->locations[index].country, globe->locations[index].region, countryName, regionName, globe->heapID);
        if (globe->locations[index].region != 0) {
            AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 1, regionName, 0, 16, TEXT_SPEED_NOTRANSFER, NULL);
        }
        AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 1, countryName, 0, 0, TEXT_SPEED_INSTANT, NULL);
        String_Delete(regionName);
        String_Delete(countryName);
        globe->rotation.x = globe->locations[index].x;
        globe->rotation.y = globe->locations[index].y;
    } else {
        globe->showLocationName = FALSE;
    }
}

#pragma force_active reset

void GeonetGlobe_LoadModels(GeonetGlobe *globe, NARC *narc) {
    globe->globeFile = NARC_AllocAndReadWholeMember(narc, 0, globe->heapID);
    GF3dRender_InitObjFromHeader(&globe->globeObj, &globe->globeMdl, (NNSG3dResFileHeader **)&globe->globeFile);
    globe->markerFiles[3] = NARC_AllocAndReadWholeMember(narc, 1, globe->heapID);
    GF3dRender_InitObjFromHeader(&globe->markerObjs[3], &globe->markerMdls[3], (NNSG3dResFileHeader **)&globe->markerFiles[3]);
    globe->cursorFile = NARC_AllocAndReadWholeMember(narc, 2, globe->heapID);
    GF3dRender_InitObjFromHeader(&globe->cursorObj, &globe->cursorMdl, (NNSG3dResFileHeader **)&globe->cursorFile);
    globe->markerFiles[1] = NARC_AllocAndReadWholeMember(narc, 3, globe->heapID);
    GF3dRender_InitObjFromHeader(&globe->markerObjs[1], &globe->markerMdls[1], (NNSG3dResFileHeader **)&globe->markerFiles[1]);
    globe->markerFiles[2] = NARC_AllocAndReadWholeMember(narc, 4, globe->heapID);
    GF3dRender_InitObjFromHeader(&globe->markerObjs[2], &globe->markerMdls[2], (NNSG3dResFileHeader **)&globe->markerFiles[2]);
}

void GeonetGlobe_FreeModels(GeonetGlobe *globe) {
    Heap_Free(globe->markerFiles[2]);
    Heap_Free(globe->markerFiles[1]);
    Heap_Free(globe->cursorFile);
    Heap_Free(globe->markerFiles[3]);
    Heap_Free(globe->globeFile);
}

void GeonetGlobe_ResetTransform(GeonetGlobe *globe) {
    globe->translation.x = 0;
    globe->translation.y = 0;
    globe->translation.z = 0;
    globe->globeScale.x = FX32_ONE;
    globe->globeScale.y = FX32_ONE;
    globe->globeScale.z = FX32_ONE;
    globe->rotation.x = 0x1A40;
    globe->rotation.y = 0x7C00;
    globe->rotation.z = 0;
    globe->markerScale.x = FX32_ONE;
    globe->markerScale.y = FX32_ONE;
    globe->markerScale.z = FX32_ONE;
}

#pragma force_active on

void GeonetGlobe_InitCamera(GeonetGlobe *globe) {
    VecFx32 target = { 0, 0, 0 };
    VecFx32 position = sGeonetCameraPosition;

    Camera_Init_FromTargetAndPos(&target, &position, 0x5C1, 0, FALSE, globe->camera);
    Camera_SetPerspectiveClippingPlane(0, FX32_CONST(100), globe->camera);
    Camera_ApplyPerspectiveType(0, globe->camera);
    Camera_SetStaticPtr(globe->camera);
    if (globe->freeRotation == FALSE) {
        globe->zoomedIn = TRUE;
    } else {
        globe->zoomedIn = FALSE;
    }
    while (GeonetGlobe_StepZoom(globe) != TRUE) {
    }
}

#pragma force_active reset

void GeonetGlobe_InitLight(GeonetGlobe *globe) {
    globe->lightVector.x = 0;
    globe->lightVector.y = 0;
    globe->lightVector.z = -0xFFF;
    NNS_G3dGlbLightVector(GX_LIGHTID_0, (s16)globe->lightVector.x, (s16)globe->lightVector.y, (s16)globe->lightVector.z);
}

void GeonetGlobe_CalcGlobeRotation(MtxFx33 *dst, const GeonetGlobeAngle *angle) {
    MtxFx33 mtx;

    MTX_RotY33_(dst, FX_SinIdx((u16)angle->y), FX_CosIdx((u16)angle->y));
    MTX_RotX33_(&mtx, FX_SinIdx((u16)angle->x), FX_CosIdx((u16)angle->x));
    MTX_Concat33(dst, &mtx, dst);
    MTX_RotZ33_(&mtx, FX_SinIdx((u16)angle->z), FX_CosIdx((u16)angle->z));
    MTX_Concat33(dst, &mtx, dst);
}

void GeonetGlobe_CalcLocationRotation(MtxFx33 *dst, const GeonetGlobeAngle *angle) {
    MtxFx33 mtx;

    MTX_RotY33_(dst, FX_SinIdx((u16)angle->x), FX_CosIdx((u16)angle->x));
    MTX_RotX33_(&mtx, FX_SinIdx((u16)-angle->y), FX_CosIdx((u16)-angle->y));
    MTX_Concat33(dst, &mtx, dst);
    MTX_RotZ33_(&mtx, FX_CosIdx((u16)angle->z), FX_SinIdx((u16)angle->z));
    MTX_Concat33(dst, &mtx, dst);
}

BOOL GeonetGlobe_HandleRotationInput(GeonetGlobe *globe, u32 newKeys, u32 heldKeys) {
    BOOL moved = FALSE;
    u16 stepY;
    u16 stepX;
    s16 angleX = (s16)globe->rotation.x;
    s16 angleY = (s16)globe->rotation.y;

    if ((newKeys & PAD_BUTTON_A) || (globe->touch.input & PAD_BUTTON_A)) {
        if (globe->freeRotation == TRUE) {
            if (globe->zoomedIn == FALSE) {
                globe->zoomedIn = TRUE;
            } else {
                globe->zoomedIn = FALSE;
            }
        }
        return TRUE;
    }

    if (globe->zoomedIn == FALSE) {
        if (globe->touch.dx != 0 || globe->touch.dy != 0) {
            stepY = (u16)(globe->touch.dx * 85);
            stepX = (u16)(globe->touch.dy * 85);
        } else {
            stepX = stepY = 0x200;
        }
    } else {
        if (globe->touch.dx != 0 || globe->touch.dy != 0) {
            stepY = (u16)(globe->touch.dx * 10);
            stepX = (u16)(globe->touch.dy * 10);
        } else {
            stepX = stepY = 32;
        }
    }

    if ((heldKeys & PAD_KEY_LEFT) || (globe->touch.input & PAD_KEY_LEFT)) {
        if (globe->freeRotation == TRUE) {
            globe->rotation.y += stepY;
        } else if (angleY < -10208) {
            globe->rotation.y += stepY;
        }
        moved = TRUE;
    }
    if ((heldKeys & PAD_KEY_RIGHT) || (globe->touch.input & PAD_KEY_RIGHT)) {
        if (globe->freeRotation == TRUE) {
            globe->rotation.y -= stepY;
        } else if (angleY > -13184) {
            globe->rotation.y -= stepY;
        }
        moved = TRUE;
    }
    if ((heldKeys & PAD_KEY_UP) || (globe->touch.input & PAD_KEY_UP)) {
        if (globe->freeRotation == TRUE) {
            if (angleX + stepX < 0x3E00) {
                globe->rotation.x += stepX;
            } else {
                globe->rotation.x = 0x3E00;
            }
        } else if (angleX < 0x2020) {
            globe->rotation.x += stepX;
        }
        moved = TRUE;
    }
    if ((heldKeys & PAD_KEY_DOWN) || (globe->touch.input & PAD_KEY_DOWN)) {
        if (globe->freeRotation == TRUE) {
            if (angleX - stepX > -0x3E00) {
                globe->rotation.x -= stepX;
            } else {
                globe->rotation.x = -0x3E00;
            }
        } else if (angleX > 0x1300) {
            globe->rotation.x -= stepX;
        }
        moved = TRUE;
    }
    return moved;
}

BOOL GeonetGlobe_StepZoom(GeonetGlobe *globe) {
    fx32 distance = Camera_GetDistance(globe->camera);
    BOOL done = FALSE;

    switch (globe->zoomedIn) {
    case TRUE:
        if (distance > FX32_CONST(88)) {
            distance -= FX32_CONST(8);
            globe->markerScale.x -= 128;
            globe->markerScale.y = globe->markerScale.x;
        } else {
            distance = FX32_CONST(80);
            done = TRUE;
        }
        break;
    case FALSE:
        if (distance < FX32_CONST(288)) {
            distance += FX32_CONST(8);
            globe->markerScale.x += 128;
            globe->markerScale.y = globe->markerScale.x;
        } else {
            distance = FX32_CONST(296);
            done = TRUE;
        }
        break;
    }
    Camera_SetDistance(distance, globe->camera);
    return done;
}

void GeonetGlobe_Draw(GeonetGlobe *globe) {
    MtxFx33 globeRotation = sGeonetGlobeRotationInit;

    switch (globe->drawState) {
    case 0:
        break;
    case 2:
        Thunk_G3X_Reset();
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
        globe->drawState = 0;
        break;
    case 1: {
        MtxFx33 cursorRotation;
        MtxFx33 markerRotation;
        u32 i;

        Thunk_G3X_Reset();
        Camera_PushLookAtToNNSGlb();
        GeonetGlobe_CalcGlobeRotation(&globeRotation, &globe->rotation);
        GF3dRender_DrawModel(&globe->globeObj, &globe->translation, &globeRotation, &globe->globeScale);
        cursorRotation = sGeonetCursorRotation;
        GF3dRender_DrawModel(&globe->cursorObj, &globe->translation, &cursorRotation, &globe->markerScale);
        markerRotation = sGeonetMarkerRotationInit;
        for (i = 0; i < globe->numLocations; i++) {
            MTX_Concat33(&globe->locations[i].rotation, &globeRotation, &markerRotation);
            if (globe->locations[i].seenState != 0) {
                GF3dRender_DrawModel(&globe->markerObjs[globe->locations[i].seenState], &globe->translation, &markerRotation, &globe->markerScale);
            }
        }
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
        break;
    }
    }
}

BOOL GeonetGlobe_GetLocationNames(int country, int region, String *countryName, String *regionName, enum HeapID heapID) {
    BOOL hasRegions;
    int placeIndex = GeonetGlobe_GetPlaceIndex(country);
    enum HeapID heap = *(volatile enum HeapID *)&heapID; // forces the spill of the argument
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0798_bin, heap);

    ReadMsgDataIntoString(msgData, country, countryName);
    DestroyMsgData(msgData);
    if (placeIndex == 0) {
        placeIndex = 1;
        region = 0;
        hasRegions = FALSE;
    } else {
        hasRegions = TRUE;
    }
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, LocationGmmDatGetGmmNo(placeIndex), heap);
    ReadMsgDataIntoString(msgData, region, regionName);
    DestroyMsgData(msgData);
    return hasRegions;
}

BOOL GeonetGlobe_CountryHasRegions(int country) {
    if (GeonetGlobe_GetPlaceIndex(country) != 0) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

void GeonetGlobe_WrapPosition(int *position) {
    if (position[0] >= 0) {
        position[0] %= 65535;
    } else {
        position[0] += ((position[0] < 0 ? -position[0] : position[0]) / 65535 + 1) * 65535;
    }
    if (position[1] >= 0) {
        position[1] %= 65535;
    } else {
        position[1] += ((position[1] < 0 ? -position[1] : position[1]) / 65535 + 1) * 65535;
    }
}

#pragma force_active reset

int GeonetGlobe_CalcDistance(const GeonetGlobePoint *a, const GeonetGlobePoint *b) {
    int dx = a->x - b->x;
    int dy;
    int half;

    if (dx < 0) {
        dx = -dx;
    }
    dy = a->y - b->y;
    if (dy < 0) {
        dy = -dy;
    }
    half = GF_DegreeToSinCosIdxNoWrap(180);
    if (dx > half) {
        dx = 0xFFFF - dx;
    }
    half = GF_DegreeToSinCosIdxNoWrap(180);
    if (dy > half) {
        dy = 0xFFFF - dy;
    }
    return (int)FX_Sqrt((dx * dx + dy * dy) << FX32_SHIFT) >> FX32_SHIFT;
}
