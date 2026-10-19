#include "geonet_globe_app.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0186.h"

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
#include "sound_playback.h"
#include "swap_3d_buffers.h"
#include "system.h"
#include "text.h"

#define GEONET_GLOBE_MAX_LOCATIONS 1024

enum GeonetGlobeState {
    GEONET_GLOBE_STATE_LOAD,
    GEONET_GLOBE_STATE_WAIT_FADE_IN,
    GEONET_GLOBE_STATE_PRINT_WELCOME,
    GEONET_GLOBE_STATE_OPEN_MENU,
    GEONET_GLOBE_STATE_HANDLE_MENU,
    GEONET_GLOBE_STATE_ASK_REGISTER,
    GEONET_GLOBE_STATE_HANDLE_REGISTER_YES_NO,
    GEONET_GLOBE_STATE_SELECT_COUNTRY,
    GEONET_GLOBE_STATE_HANDLE_COUNTRY_LIST,
    GEONET_GLOBE_STATE_SELECT_REGION,
    GEONET_GLOBE_STATE_HANDLE_REGION_LIST,
    GEONET_GLOBE_STATE_SHOW_LOCATION,
    GEONET_GLOBE_STATE_ASK_CONFIRM,
    GEONET_GLOBE_STATE_HANDLE_CONFIRM_YES_NO,
    GEONET_GLOBE_STATE_START_GLOBE,
    GEONET_GLOBE_STATE_GLOBE,
    GEONET_GLOBE_STATE_ZOOM,
    GEONET_GLOBE_STATE_FADE_OUT,
    GEONET_GLOBE_STATE_WAIT_FADE_OUT,
};

fx32 FX_Sqrt(fx32 x);

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
    s16 x;
    s16 y;
    MtxFx33 rotation;
    u16 seenState; // also the index of the marker model
    u16 country;
    u16 region;
} GeonetLocation;

typedef struct GeonetGlobeAngle {
    s32 x;
    s32 y;
    s32 z;
} GeonetGlobeAngle;

typedef struct GeonetGlobeTouch {
    u32 input; // PAD_* style bits produced by touch gestures
    u32 state;
    u32 x;
    u32 y;
    s32 dx;
    s32 dy;
    u32 timer;
} GeonetGlobeTouch;

typedef struct GeonetGlobe {
    enum HeapID heapID;
    SaveWiFiHistory *wifiHistory;
    Options *options;
    u32 numLocations;
    GeonetLocation locations[GEONET_GLOBE_MAX_LOCATIONS];
    BgConfig *bgConfig;
    Window windows[GEONET_GLOBE_WINDOW_COUNT];
    struct ListMenu *listMenu;
    ListMenuItem *listMenuItems;
    struct ListMenu2D *yesNoMenu;
    MsgData *msgData;
    int printState;
    int printerId;
    String *printString;
    MessageFormat *msgFormat;
    NNSG3dRenderObj globeObj;
    NNSG3dResMdl *globeMdl;
    void *globeFile;
    NNSG3dRenderObj markerObjs[4]; // indexed by GeonetLocation.seenState (0 unused)
    NNSG3dRenderObj cursorObj;
    NNSG3dResMdl *markerMdls[4];
    NNSG3dResMdl *cursorMdl;
    void *markerFiles[4];
    void *cursorFile;
    VecFx32 translation;
    VecFx32 globeScale;
    GeonetGlobeAngle rotation;
    VecFx32 markerScale;
    Camera *camera;
    u8 padding_C2E0[8];
    u16 zoomedIn;
    u8 filler_C2EA[2];
    VecFx32 lightVector;
    u32 drawState;
    u32 unk_C2FC;
    u32 isJapanese;
    u16 freeRotation;
    u8 filler_C306[2];
    GeonetGlobeTouch touch;
    u32 playerCountry;
    u32 playerRegion;
    u32 selectedCountry;
    u32 selectedRegion;
    u32 nonJapanese;
    u32 showLocationName;
} GeonetGlobe;

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

static void GeonetGlobe_SetGfxBanks(void);
static void GeonetGlobe_SetGraphicsModes(void);
static void GeonetGlobe_LoadLocations(GeonetGlobe *globe);
static void GeonetGlobe_SetLocation(GeonetGlobe *globe, u32 index, s32 x, s32 y, u16 country, u16 region);
static void GeonetGlobe_MarkPlayerLocation(GeonetGlobe *globe);
static int GeonetGlobe_GetPlaceIndex(int country);
static void GeonetGlobe_HandleTouch(GeonetGlobe *globe);
static void GeonetGlobe_GetTouchDrag(int x, int y, u32 *dirX, u32 *distX, u32 *dirY, u32 *distY);
static void GeonetGlobe_InitBgs(GeonetGlobe *globe, NARC *narc);
static void GeonetGlobe_FreeBgs(GeonetGlobe *globe);
static BOOL GeonetGlobe_PrintMessage(GeonetGlobe *globe, int msgId, BOOL noWait);
static void GeonetGlobe_ListMenuMoveCursor(struct ListMenu *menu, s32 index, u8 onInit);
static void GeonetGlobe_CreateMenu(GeonetGlobe *globe, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *menuTemplate, const GeonetGlobeMenuEntry *entries);
static void GeonetGlobe_CreatePlaceList(GeonetGlobe *globe, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *menuTemplate, int msgBank, const u8 *msgIds, u32 count);
static void GeonetGlobe_DestroyListMenu(GeonetGlobe *globe);
static void GeonetGlobe_ShowPlayerLocation(GeonetGlobe *globe);
static void GeonetGlobe_ShowLocation(GeonetGlobe *globe, int country, int region);
static void GeonetGlobe_HideLocationWindow(GeonetGlobe *globe);
static BOOL GeonetGlobe_FindNearestLocation(GeonetGlobe *globe, int *index);
static void GeonetGlobe_UpdateLocationName(GeonetGlobe *globe);
static void GeonetGlobe_LoadModels(GeonetGlobe *globe, NARC *narc);
static void GeonetGlobe_FreeModels(GeonetGlobe *globe);
static void GeonetGlobe_ResetTransform(GeonetGlobe *globe);
static void GeonetGlobe_InitCamera(GeonetGlobe *globe);
static void GeonetGlobe_InitLight(GeonetGlobe *globe);
static void GeonetGlobe_CalcGlobeRotation(MtxFx33 *dst, const GeonetGlobeAngle *angle);
static void GeonetGlobe_CalcLocationRotation(MtxFx33 *dst, const GeonetGlobeAngle *angle);
static BOOL GeonetGlobe_HandleRotationInput(GeonetGlobe *globe, u32 newKeys, u32 heldKeys);
static BOOL GeonetGlobe_StepZoom(GeonetGlobe *globe);
static void GeonetGlobe_Draw(GeonetGlobe *globe);
static BOOL GeonetGlobe_GetLocationNames(int country, int region, String *countryName, String *regionName, enum HeapID heapID);
static BOOL GeonetGlobe_CountryHasRegions(int country);
static void GeonetGlobe_WrapPosition(int *position);
static int GeonetGlobe_CalcDistance(const GeonetGlobePoint *a, const GeonetGlobePoint *b);

static const WindowTemplate sGeonetPlaceListWindowTemplate = {
    .bgId = GF_BG_LYR_SUB_2,
    .left = 3,
    .top = 2,
    .width = 26,
    .height = 14,
    .palette = 4,
    .baseTile = 1,
};

static const WindowTemplate sGeonetGlobeListWindowTemplate = {
    .bgId = GF_BG_LYR_SUB_2,
    .left = 19,
    .top = 11,
    .width = 12,
    .height = 6,
    .palette = 4,
    .baseTile = 0x125,
};

static const WindowTemplate sGeonetLocationWindowTemplate = {
    .bgId = GF_BG_LYR_SUB_2,
    .left = 2,
    .top = 1,
    .width = 27,
    .height = 6,
    .palette = 4,
    .baseTile = 0xB3,
};

static const WindowTemplate sGeonetExitButtonWindowTemplate = {
    .bgId = GF_BG_LYR_MAIN_2,
    .left = 25,
    .top = 21,
    .width = 6,
    .height = 2,
    .palette = 15,
    .baseTile = 0x1CD,
};

static const WindowTemplate sGeonetMessageWindowTemplate = {
    .bgId = GF_BG_LYR_SUB_2,
    .left = 2,
    .top = 19,
    .width = 27,
    .height = 4,
    .palette = 4,
    .baseTile = 0x16D,
};

static const WindowTemplate sGeonetYesNoWindowTemplate = {
    .bgId = GF_BG_LYR_SUB_2,
    .left = 25,
    .top = 13,
    .width = 6,
    .height = 4,
    .palette = 4,
    .baseTile = 0x155,
};

static const WindowTemplate sGeonetInfoButtonWindowTemplate = {
    .bgId = GF_BG_LYR_MAIN_2,
    .left = 1,
    .top = 21,
    .width = 6,
    .height = 2,
    .palette = 15,
    .baseTile = 0x1C1,
};

static const VecFx32 sGeonetCameraPosition = { 0, 0, FX32_CONST(296) };

static const GraphicsModes sGeonetGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const GeonetGlobeMenuEntry sGeonetMenuEntries[3] = {
    { msg_0186_00010, 0 },
    { msg_0186_00011, 1 },
    { msg_0186_00012, 2 },
};

static const BgTemplate sGeonetBgTemplate_Text = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x7000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sGeonetBgTemplate_Frame = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0x7800,
    .charBase = GX_BG_CHARBASE_0x04000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const ListMenuTemplate sGeonetMenuTemplate = {
    .items = NULL,
    .moveCursorFunc = NULL,
    .itemPrintFunc = NULL,
    .window = NULL,
    .totalItems = 3,
    .maxShowed = 3,
    .header_X = 0,
    .item_X = 12,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 1,
    .fillValue = 15,
    .cursorShadowPal = 2,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = 0,
    .fontId = 0,
    .cursorKind = 0,
    .data = NULL,
};

static const ListMenuTemplate sGeonetPlaceListTemplate = {
    .items = NULL,
    .moveCursorFunc = NULL,
    .itemPrintFunc = NULL,
    .window = NULL,
    .totalItems = 0,
    .maxShowed = 7,
    .header_X = 0,
    .item_X = 12,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 1,
    .fillValue = 15,
    .cursorShadowPal = 2,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = 1,
    .fontId = 0,
    .cursorKind = 0,
    .data = NULL,
};

static const MtxFx33 sGeonetMarkerRotationInit = { FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0, FX32_ONE };

static const MtxFx33 sGeonetLocationRotationInit = { FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0, FX32_ONE };

static const MtxFx33 sGeonetGlobeRotationInit = { FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0, FX32_ONE };

static const MtxFx33 sGeonetCursorRotation = { FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0, FX32_ONE };

static const GraphicsBanks sGeonetGraphicsBanks = {
    .bg = GX_VRAM_BG_128_C,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_32_H,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_16_F,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_01_AB,
    .texpltt = GX_VRAM_TEXPLTT_0123_E,
};

BOOL GeonetGlobe_Init(OverlayManager *man, int *state) {
    GeonetGlobe *globe;
    SaveData *saveData;
    Options *options;

    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GX_SetVisiblePlane(GX_PLANEMASK_NONE);
    GXS_SetVisiblePlane(GX_PLANEMASK_NONE);
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
    case GEONET_GLOBE_STATE_START_GLOBE:
    case GEONET_GLOBE_STATE_GLOBE:
    case GEONET_GLOBE_STATE_ZOOM:
        break;
    default:
        if (System_GetTouchNew()) {
            gSystem.simulatedInputs = 1;
        }
        break;
    }

    switch (*state) {
    case GEONET_GLOBE_STATE_LOAD:
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
        *state = GEONET_GLOBE_STATE_WAIT_FADE_IN;
        break;
    case GEONET_GLOBE_STATE_WAIT_FADE_IN:
        if (IsPaletteFadeFinished() == TRUE) {
            *state = GEONET_GLOBE_STATE_PRINT_WELCOME;
        }
        break;
    case GEONET_GLOBE_STATE_PRINT_WELCOME:
        if (GeonetGlobe_PrintMessage(globe, msg_0186_00000, TRUE) == TRUE) {
            if (globe->playerCountry == 0) {
                *state = GEONET_GLOBE_STATE_OPEN_MENU;
            } else {
                *state = GEONET_GLOBE_STATE_START_GLOBE;
            }
        }
        break;
    case GEONET_GLOBE_STATE_OPEN_MENU:
        if (GeonetGlobe_PrintMessage(globe, msg_0186_00001, TRUE) == TRUE) {
            GeonetGlobe_CreateMenu(globe, &globe->windows[GEONET_GLOBE_WINDOW_LIST], &sGeonetGlobeListWindowTemplate, &sGeonetMenuTemplate, sGeonetMenuEntries);
            *state = GEONET_GLOBE_STATE_HANDLE_MENU;
        }
        break;
    case GEONET_GLOBE_STATE_HANDLE_MENU:
        choice = ListMenu_ProcessInput(globe->listMenu);
        if (choice != LIST_NOTHING_CHOSEN) {
            GeonetGlobe_DestroyListMenu(globe);
            PlaySE(SEQ_SE_DP_SELECT);
            switch (choice) {
            case 0:
            case LIST_NOTHING_CHOSEN:
            default:
                *state = GEONET_GLOBE_STATE_START_GLOBE;
                break;
            case 1:
                *state = GEONET_GLOBE_STATE_ASK_REGISTER;
                break;
            case LIST_CANCEL:
            case 2:
                *state = GEONET_GLOBE_STATE_FADE_OUT;
                break;
            }
        }
        break;
    case GEONET_GLOBE_STATE_ASK_REGISTER:
        if (GeonetGlobe_PrintMessage(globe, msg_0186_00002, TRUE) == TRUE) {
            globe->yesNoMenu = Std_CreateYesNoMenu(globe->bgConfig, &sGeonetYesNoWindowTemplate, 473, 7, globe->heapID);
            *state = GEONET_GLOBE_STATE_HANDLE_REGISTER_YES_NO;
        }
        break;
    case GEONET_GLOBE_STATE_HANDLE_REGISTER_YES_NO:
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
            *state = GEONET_GLOBE_STATE_SELECT_REGION;
        } else {
            *state = GEONET_GLOBE_STATE_SELECT_COUNTRY;
        }
        break;
case6_no:
        *state = GEONET_GLOBE_STATE_OPEN_MENU;
        break;
    case GEONET_GLOBE_STATE_SELECT_COUNTRY:
        if (GeonetGlobe_PrintMessage(globe, msg_0186_00003, TRUE) == TRUE) {
            globe->selectedCountry = 0;
            msgIds = LocationGmmDatGetDistrictNameMsgIdsPtr(0);
            count = LocationGmmDatGetDistrictCount(0);
            GeonetGlobe_CreatePlaceList(globe, &globe->windows[GEONET_GLOBE_WINDOW_LIST], &sGeonetPlaceListWindowTemplate, &sGeonetPlaceListTemplate, NARC_msg_msg_0798_bin, msgIds, count);
            *state = GEONET_GLOBE_STATE_HANDLE_COUNTRY_LIST;
        }
        break;
    case GEONET_GLOBE_STATE_HANDLE_COUNTRY_LIST:
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
                    *state = GEONET_GLOBE_STATE_SELECT_REGION;
                } else {
                    globe->selectedRegion = 0;
                    *state = GEONET_GLOBE_STATE_SHOW_LOCATION;
                }
            } else {
                *state = GEONET_GLOBE_STATE_OPEN_MENU;
            }
        }
        break;
    case GEONET_GLOBE_STATE_SELECT_REGION:
        if (GeonetGlobe_PrintMessage(globe, msg_0186_00004, TRUE) == TRUE) {
            globe->selectedRegion = 0;
            placeIndex = LocationGmmDatIndexGetByCountryMsgNo(globe->selectedCountry);
            msgBank = LocationGmmDatGetGmmNo(placeIndex);
            msgIds = LocationGmmDatGetDistrictNameMsgIdsPtr(placeIndex);
            count = LocationGmmDatGetDistrictCount(placeIndex);
            GeonetGlobe_CreatePlaceList(globe, &globe->windows[GEONET_GLOBE_WINDOW_LIST], &sGeonetPlaceListWindowTemplate, &sGeonetPlaceListTemplate, msgBank, msgIds, count);
            *state = GEONET_GLOBE_STATE_HANDLE_REGION_LIST;
        }
        break;
    case GEONET_GLOBE_STATE_HANDLE_REGION_LIST:
        choice = ListMenu_ProcessInput(globe->listMenu);
        if (choice != LIST_NOTHING_CHOSEN) {
            GeonetGlobe_DestroyListMenu(globe);
            PlaySE(SEQ_SE_DP_SELECT);
            if (choice != LIST_CANCEL) {
                choice = LocationGmmDatGetDistrictNameMsgIdsPtr(LocationGmmDatIndexGetByCountryMsgNo(globe->selectedCountry))[choice];
            }
            if (choice != LIST_CANCEL) {
                globe->selectedRegion = choice;
                *state = GEONET_GLOBE_STATE_SHOW_LOCATION;
            } else if (globe->isJapanese == TRUE) {
                *state = GEONET_GLOBE_STATE_OPEN_MENU;
            } else {
                *state = GEONET_GLOBE_STATE_SELECT_COUNTRY;
            }
        }
        break;
    case GEONET_GLOBE_STATE_SHOW_LOCATION:
        GeonetGlobe_ShowLocation(globe, globe->selectedCountry, globe->selectedRegion);
        *state = GEONET_GLOBE_STATE_ASK_CONFIRM;
        break;
    case GEONET_GLOBE_STATE_ASK_CONFIRM:
        if (GeonetGlobe_PrintMessage(globe, msg_0186_00005, TRUE) == TRUE) {
            globe->yesNoMenu = Std_CreateYesNoMenu(globe->bgConfig, &sGeonetYesNoWindowTemplate, 473, 7, globe->heapID);
            *state = GEONET_GLOBE_STATE_HANDLE_CONFIRM_YES_NO;
        }
        break;
    case GEONET_GLOBE_STATE_HANDLE_CONFIRM_YES_NO:
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
        *state = GEONET_GLOBE_STATE_START_GLOBE;
        break;
case13_no:
        GeonetGlobe_HideLocationWindow(globe);
        *state = GEONET_GLOBE_STATE_OPEN_MENU;
        break;
    case GEONET_GLOBE_STATE_START_GLOBE:
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
        *state = GEONET_GLOBE_STATE_GLOBE;
        break;
    case GEONET_GLOBE_STATE_GLOBE: {
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
                *state = GEONET_GLOBE_STATE_OPEN_MENU;
            } else {
                GeonetGlobe_HideLocationWindow(globe);
                *state = GEONET_GLOBE_STATE_FADE_OUT;
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
            *state = GEONET_GLOBE_STATE_ZOOM;
            PlaySE(SEQ_SE_PL_FTIMER03);
        }
        break;
    }
    case GEONET_GLOBE_STATE_ZOOM:
        if (GeonetGlobe_StepZoom(globe) == TRUE) {
            *state = GEONET_GLOBE_STATE_GLOBE;
        }
        break;
    case GEONET_GLOBE_STATE_FADE_OUT:
        globe->unk_C2FC = 0;
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, globe->heapID);
        *state = GEONET_GLOBE_STATE_WAIT_FADE_OUT;
        break;
    case GEONET_GLOBE_STATE_WAIT_FADE_OUT:
        if (IsPaletteFadeFinished() == TRUE) {
            globe->drawState = 1;
            GeonetGlobe_FreeBgs(globe);
            GeonetGlobe_FreeModels(globe);
            DestroyMsgData(globe->msgData);
            *state = GEONET_GLOBE_STATE_LOAD;
            result = TRUE;
        }
        break;
    }
    GeonetGlobe_Draw(globe);
    return result;
}

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

static void GeonetGlobe_SetGfxBanks(void) {
    GraphicsBanks banks = sGeonetGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void GeonetGlobe_SetGraphicsModes(void) {
    GraphicsModes modes = sGeonetGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
}

static void GeonetGlobe_LoadLocations(GeonetGlobe *globe) {
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

static void GeonetGlobe_SetLocation(GeonetGlobe *globe, u32 index, s32 x, s32 y, u16 country, u16 region) {
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

static void GeonetGlobe_MarkPlayerLocation(GeonetGlobe *globe) {
    u32 i;
    GeonetGlobe *shifted;

    for (i = 0; i < globe->numLocations; i++) {
        shifted = (GeonetGlobe *)((u8 *)globe + i * sizeof(GeonetLocation));
        if (globe->playerCountry == shifted->locations[0].country && globe->playerRegion == shifted->locations[0].region) {
            shifted->locations[0].seenState = 3;
            globe->rotation.x = shifted->locations[0].x;
            globe->rotation.y = shifted->locations[0].y;
        }
    }
}

static int GeonetGlobe_GetPlaceIndex(int country) {
    return LocationGmmDatIndexGetByCountryMsgNo(country);
}

// Turns touch input into button bits: the two touch buttons (B / X), a short tap (A) and drags (D-pad).
static void GeonetGlobe_HandleTouch(GeonetGlobe *globe) {
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

static void GeonetGlobe_GetTouchDrag(int x, int y, u32 *dirX, u32 *distX, u32 *dirY, u32 *distY) {
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

static void GeonetGlobe_InitBgs(GeonetGlobe *globe, NARC *narc) {
    u16 colors[4];
    String *string;

    InitBgFromTemplate(globe->bgConfig, GF_BG_LYR_SUB_2, &sGeonetBgTemplate_Text, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(globe->bgConfig, GF_BG_LYR_SUB_2);
    InitBgFromTemplate(globe->bgConfig, GF_BG_LYR_SUB_3, &sGeonetBgTemplate_Frame, GF_BG_TYPE_TEXT);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, globe->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, globe->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 6, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x80, globe->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 7, globe->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, globe->heapID);
    LoadUserFrameGfx2(globe->bgConfig, GF_BG_LYR_SUB_2, 482, 6, (u8)Options_GetFrame(globe->options), globe->heapID);
    LoadUserFrameGfx1(globe->bgConfig, GF_BG_LYR_SUB_2, 473, 7, 0, globe->heapID);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_4_OFFSET, globe->heapID);
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
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 6, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x80, globe->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 7, globe->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, globe->heapID);
    LoadUserFrameGfx1(globe->bgConfig, GF_BG_LYR_MAIN_2, 473, 7, 0, globe->heapID);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_4_OFFSET, globe->heapID);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, globe->heapID);
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
    ReadMsgDataIntoString(globe->msgData, msg_0186_00016, string);
    AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON], 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    AddWindow(globe->bgConfig, &globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON], &sGeonetInfoButtonWindowTemplate);
    FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON], 15, 0, 0, 216, 32);
    ReadMsgDataIntoString(globe->msgData, msg_0186_00013, string);
    AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON], 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    String_Delete(string);
    FontID_Release(4);
}

static void GeonetGlobe_FreeBgs(GeonetGlobe *globe) {
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON]);
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_INFO_BUTTON]);
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE]);
    FreeBgTilemapBuffer(globe->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(globe->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(globe->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(globe->bgConfig, GF_BG_LYR_SUB_3);
}

static BOOL GeonetGlobe_PrintMessage(GeonetGlobe *globe, int msgId, BOOL noWait) {
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

static void GeonetGlobe_ListMenuMoveCursor(struct ListMenu *menu, s32 index, u8 onInit) {
    if (onInit == FALSE) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

static void GeonetGlobe_CreateMenu(GeonetGlobe *globe, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *menuTemplate, const GeonetGlobeMenuEntry *entries) {
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

static void GeonetGlobe_CreatePlaceList(GeonetGlobe *globe, Window *window, const WindowTemplate *windowTemplate, const ListMenuTemplate *menuTemplate, int msgBank, const u8 *msgIds, u32 count) {
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

static void GeonetGlobe_DestroyListMenu(GeonetGlobe *globe) {
    EraseFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_LIST], FALSE);
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_LIST]);
    DestroyListMenu(globe->listMenu, NULL, NULL);
    ListMenuItems_Delete(globe->listMenuItems);
}

static void GeonetGlobe_ShowPlayerLocation(GeonetGlobe *globe) {
    String *string = String_New(1024, globe->heapID);
    String *template = String_New(1024, globe->heapID);

    AddWindow(globe->bgConfig, &globe->windows[GEONET_GLOBE_WINDOW_LOCATION], &sGeonetLocationWindowTemplate);
    FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], 15, 0, 0, 216, 48);
    DrawFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], FALSE, 473, 7);
    BufferCountryName(globe->msgFormat, 0, globe->playerCountry);
    BufferCityName(globe->msgFormat, 1, globe->playerCountry, globe->playerRegion);
    ReadMsgDataIntoString(globe->msgData, msg_0186_00014, template);
    StringExpandPlaceholders(globe->msgFormat, string, template);
    AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], 0, string, 0, 0, TEXT_SPEED_INSTANT, NULL);
    String_Delete(template);
    String_Delete(string);
    CopyWindowToVram(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION]);
}

static void GeonetGlobe_ShowLocation(GeonetGlobe *globe, int country, int region) {
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

static void GeonetGlobe_HideLocationWindow(GeonetGlobe *globe) {
    EraseFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], FALSE);
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION]);
}

// Finds the seen location nearest to the globe's current orientation (within 128 units in x and y).
static BOOL GeonetGlobe_FindNearestLocation(GeonetGlobe *globe, int *index) {
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

static void GeonetGlobe_UpdateLocationName(GeonetGlobe *globe) {
    String *countryName;
    String *regionName;
    int index;

    if (globe->showLocationName == FALSE) {
        String *string = String_New(1024, globe->heapID);
        FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 15, 0, 0, 216, 48);
        ReadMsgDataIntoString(globe->msgData, msg_0186_00015, string);
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

static void GeonetGlobe_LoadModels(GeonetGlobe *globe, NARC *narc) {
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

static void GeonetGlobe_FreeModels(GeonetGlobe *globe) {
    Heap_Free(globe->markerFiles[2]);
    Heap_Free(globe->markerFiles[1]);
    Heap_Free(globe->cursorFile);
    Heap_Free(globe->markerFiles[3]);
    Heap_Free(globe->globeFile);
}

static void GeonetGlobe_ResetTransform(GeonetGlobe *globe) {
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

static void GeonetGlobe_InitCamera(GeonetGlobe *globe) {
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

static void GeonetGlobe_InitLight(GeonetGlobe *globe) {
    globe->lightVector.x = 0;
    globe->lightVector.y = 0;
    globe->lightVector.z = -0xFFF;
    NNS_G3dGlbLightVector(GX_LIGHTID_0, (s16)globe->lightVector.x, (s16)globe->lightVector.y, (s16)globe->lightVector.z);
}

static void GeonetGlobe_CalcGlobeRotation(MtxFx33 *dst, const GeonetGlobeAngle *angle) {
    MtxFx33 mtx;

    MTX_RotY33_(dst, FX_SinIdx((u16)angle->y), FX_CosIdx((u16)angle->y));
    MTX_RotX33_(&mtx, FX_SinIdx((u16)angle->x), FX_CosIdx((u16)angle->x));
    MTX_Concat33(dst, &mtx, dst);
    MTX_RotZ33_(&mtx, FX_SinIdx((u16)angle->z), FX_CosIdx((u16)angle->z));
    MTX_Concat33(dst, &mtx, dst);
}

static void GeonetGlobe_CalcLocationRotation(MtxFx33 *dst, const GeonetGlobeAngle *angle) {
    MtxFx33 mtx;

    MTX_RotY33_(dst, FX_SinIdx((u16)angle->x), FX_CosIdx((u16)angle->x));
    MTX_RotX33_(&mtx, FX_SinIdx((u16)-angle->y), FX_CosIdx((u16)-angle->y));
    MTX_Concat33(dst, &mtx, dst);
    MTX_RotZ33_(&mtx, FX_CosIdx((u16)angle->z), FX_SinIdx((u16)angle->z));
    MTX_Concat33(dst, &mtx, dst);
}

static BOOL GeonetGlobe_HandleRotationInput(GeonetGlobe *globe, u32 newKeys, u32 heldKeys) {
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

static BOOL GeonetGlobe_StepZoom(GeonetGlobe *globe) {
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

static void GeonetGlobe_Draw(GeonetGlobe *globe) {
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

static BOOL GeonetGlobe_GetLocationNames(int country, int region, String *countryName, String *regionName, enum HeapID heapID) {
    BOOL hasRegions;
    int placeIndex = GeonetGlobe_GetPlaceIndex(country);
    enum HeapID heap = *(volatile enum HeapID *)&heapID;
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

static BOOL GeonetGlobe_CountryHasRegions(int country) {
    if (GeonetGlobe_GetPlaceIndex(country) != 0) {
        return TRUE;
    }
    return FALSE;
}

static void GeonetGlobe_WrapPosition(int *position) {
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

static int GeonetGlobe_CalcDistance(const GeonetGlobePoint *a, const GeonetGlobePoint *b) {
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
