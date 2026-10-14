#include "global.h"

#include "constants/maps.h"

#include "application/pokegear/map/pokegear_map_internal.h"
#include "msgdata/msg/msg_0273.h"

#include "brightness.h"
#include "phonebook_dat.h"
#include "poke_overlay.h"
#include "pokegear_apps.h"
#include "roamer.h"
#include "screen_fade.h"
#include "sys_flags.h"
#include "touchscreen.h"
#include "sound_playback.h"

FS_EXTERN_OVERLAY(OVY_26);

static BOOL IsMapSinjoh(u16 mapID);
static BOOL IsMapSSAqua(u16 mapID);
static void PokegearMap_InitInternal(PokegearMapAppData *mapApp);
static void PokegearMap_ExitInternal(PokegearMapAppData *mapApp);
static int PokegearMap_MainTask_LoadGFX(PokegearMapAppData *mapApp);
static int PokegearMap_MainTask_HandleInput(PokegearMapAppData *mapApp);
static int PokegearMap_MainTask_UnloadGFX(PokegearMapAppData *mapApp);
static int PokegearMap_MainTask_EnterMarkingMode(PokegearMapAppData *mapApp);
static int PokegearMap_MainTask_HandleInput_MarkingMode(PokegearMapAppData *mapApp);
static int PokegearMap_MainTask_ExitMarkingMode(PokegearMapAppData *mapApp);
static int PokegearMap_MainTask_FadeOutForWordSelect(PokegearMapAppData *mapApp);
static int PokegearMap_MainTask_FadeIn(PokegearMapAppData *mapApp);
static int PokegearMap_MainTask_FadeOut(PokegearMapAppData *mapApp);
static int PokegearMap_MainTask_FadeInApp(PokegearMapAppData *mapApp);
static int PokegearMap_MainTask_FadeOutApp(PokegearMapAppData *mapApp);

static const u16 sMapXScrollLimits[] = {
    26, // Johto only
    29, // Indigo Plateau
    45, // All of Kanto
};

BOOL PokegearMap_Init(OverlayManager *man, int *state) {
    PokegearAppData *pokegearApp = OverlayManager_GetArgs(man);
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_26), OVY_LOAD_ASYNC);
    Heap_Create(HEAP_ID_3, HEAP_ID_POKEGEAR_APP, 196608);
    PokegearMapAppData *mapApp = OverlayManager_CreateAndGetData(man, sizeof(PokegearMapAppData), HEAP_ID_POKEGEAR_APP);
    memset(mapApp, 0, sizeof(PokegearMapAppData));
    mapApp->pokegear = pokegearApp;
    mapApp->heapID = HEAP_ID_POKEGEAR_APP;
    PokegearMap_InitInternal(mapApp);
    mapApp->locationSpecs = sLocationSpecs;
    mapApp->numLocationSpecs = NELEMS(sLocationSpecs);
    return TRUE;
}

BOOL PokegearMap_Main(OverlayManager *man, int *state) {
    PokegearMapAppData *mapApp = OverlayManager_GetData(man);

    switch (*state) {
    case PGMAP_MAIN_STATE_LOAD:
        *state = PokegearMap_MainTask_LoadGFX(mapApp);
        break;
    case PGMAP_MAIN_STATE_HANDLE_INPUT:
        *state = PokegearMap_MainTask_HandleInput(mapApp);
        break;
    case PGMAP_MAIN_STATE_UNLOAD:
        *state = PokegearMap_MainTask_UnloadGFX(mapApp);
        break;
    case PGMAP_MAIN_STATE_FADE_IN:
        *state = PokegearMap_MainTask_FadeIn(mapApp);
        break;
    case PGMAP_MAIN_STATE_FADE_OUT:
        *state = PokegearMap_MainTask_FadeOut(mapApp);
        break;
    case PGMAP_MAIN_STATE_FADE_IN_APP:
        *state = PokegearMap_MainTask_FadeInApp(mapApp);
        break;
    case PGMAP_MAIN_STATE_FADE_OUT_APP:
        *state = PokegearMap_MainTask_FadeOutApp(mapApp);
        break;
    case PGMAP_MAIN_STATE_ENTER_MARKING_MODE:
        *state = PokegearMap_MainTask_EnterMarkingMode(mapApp);
        break;
    case PGMAP_MAIN_STATE_HANDLE_INPUT_MARKING_MODE:
        *state = PokegearMap_MainTask_HandleInput_MarkingMode(mapApp);
        break;
    case PGMAP_MAIN_STATE_EXIT_MARKING_MODE:
        *state = PokegearMap_MainTask_ExitMarkingMode(mapApp);
        break;
    case PGMAP_MAIN_STATE_FADE_OUT_FOR_WORD_SELECT:
        *state = PokegearMap_MainTask_FadeOutForWordSelect(mapApp);
        break;
    case PGMAP_MAIN_STATE_QUIT:
        return TRUE;
    }

    return FALSE;
}

BOOL PokegearMap_Exit(OverlayManager *man, int *state) {
    enum HeapID heapID;
    PokegearMapAppData *mapApp = OverlayManager_GetData(man);

    PokegearMap_ExitInternal(mapApp);
    MapMatrix_MapData_Free(mapApp->mapData);
    if (mapApp->pokegear->appReturnCode != GEAR_RETURN_CANCEL) {
        mapApp->pokegear->isSwitchApp = TRUE;
    }
    heapID = mapApp->heapID;
    OverlayManager_FreeData(man);
    Heap_Destroy(heapID);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_26));
    return TRUE;
}

static BOOL IsMapSinjoh(u16 mapID) {
    const u16 sSinjohMapIDs[] = {
        MAP_SINJOH_RUINS_EXTERIOR,
        MAP_SINJOH_RUINS_MYSTRI_STAGE,
        MAP_SINJOH_RUINS_CABIN,
    };

    int i;
    for (i = 0; i < NELEMS(sSinjohMapIDs); ++i) {
        if (mapID == sSinjohMapIDs[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

static BOOL IsMapSSAqua(u16 mapID) {
    const u16 sSSAquaMapIDs[] = {
        MAP_SS_AQUA_1F,
        MAP_SS_AQUA_CAPTAIN_ROOM,
        MAP_SS_AQUA_1F_SOUTHEAST_ROOMS,
        MAP_SS_AQUA_1F_SOUTHWEST_ROOMS,
        MAP_SS_AQUA_1F_NORTHEAST_ROOMS,
        MAP_SS_AQUA_1F_NORTHWEST_ROOMS,
        MAP_SS_AQUA_B1F,
    };

    int i;
    for (i = 0; i < NELEMS(sSSAquaMapIDs); ++i) {
        if (mapID == sSSAquaMapIDs[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

static void PokegearMap_InitInternal(PokegearMapAppData *mapApp) {
    int i;
    MapMarkingsHeapNode *mapMarkingsHeapNode;
    RoamerSaveData *roamers;

    mapApp->pokegear->childAppdata = mapApp;
    mapApp->pokegear->reselectAppCB = PokegearMap_ShowMapCursor;
    mapApp->pokegear->deselectAppCB = PokegearMap_DeselectApp;
    MapApp_LoadMarkingsListFromSave(mapApp);
    if (mapApp->pokegear->app != GEAR_APP_CANCEL) {
        mapApp->inMarkingsMode = FALSE;
        mapApp->pokegear->app = GEAR_APP_MAP;
        mapApp->zoomed = Pokegear_GetMapZoomed(mapApp->pokegear->savePokegear);
    } else {
        mapApp->inMarkingsMode = TRUE;
        mapApp->sessionState = mapApp->pokegear->mapSessionState;
        mapApp->zoomed = mapApp->sessionState.zoomed;
        if (mapApp->sessionState.word != EC_WORD_NULL) {
            mapMarkingsHeapNode = MapApp_GetMarkingsHeapNodeByMapID(mapApp, mapApp->sessionState.mapID);
            if (mapMarkingsHeapNode == NULL) {
                mapMarkingsHeapNode = MapApp_GetOrCreateMarkingsHeapNodeByMapID(mapApp, mapApp->sessionState.mapID);
            }
            MapMarkingsHeapNode_SetWord(mapMarkingsHeapNode, mapApp->sessionState.index, mapApp->sessionState.word);
        }
        mapApp->pokegear->returnFromEasyChat = 0;
        mapApp->pokegear->app = GEAR_APP_MAP;
    }
    mapApp->unk_138_4 = FALSE;
    mapApp->unk_138_1 = 0;
    mapApp->mapUnlockLevel = Pokegear_GetMapUnlockLevel(mapApp->pokegear->savePokegear);
    mapApp->mapData = MapMatrix_MapData_New(mapApp->heapID);
    mapApp->phoneCallSave = SaveData_GetPhoneCallPersistentState(mapApp->pokegear->saveData);
    mapApp->phoneBook = AllocAndReadPhoneBook(mapApp->heapID);
    mapApp->phoneContact = SavePokegear_AllocAndCopyPhonebook(mapApp->pokegear->savePokegear, mapApp->heapID);
    mapApp->numPhonebookSlots = SavePokegear_FindEmptyPhonebookSlot(mapApp->pokegear->savePokegear);
    mapApp->matrixX = mapApp->pokegear->args->matrixXCoord;
    mapApp->matrixY = mapApp->pokegear->args->matrixYCoord + 2;
    mapApp->mapID = mapApp->pokegear->args->mapID;
    mapApp->playerGender = mapApp->pokegear->args->playerGender;
    mapApp->playerX = mapApp->pokegear->args->matrixXCoord;
    mapApp->playerY = mapApp->pokegear->args->matrixYCoord + 2;
    mapApp->minXscroll = 1;
    mapApp->minYscroll = 1;
    mapApp->maxXscroll = sMapXScrollLimits[mapApp->mapUnlockLevel];
    mapApp->maxYscroll = 17;
    mapApp->centerY = 8;
    mapApp->centerX = 8;
    mapApp->yOffset = 0;
    mapApp->canSeeSafariZone = TRUE;
    mapApp->canFlyToGoldenrod = Save_VarsFlags_FlypointFlagAction(mapApp->pokegear->saveVarsFlags, FLAG_ACTION_CHECK, FLYPOINT_GOLDENROD);
    mapApp->isMapSinjoh = IsMapSinjoh(mapApp->mapID);
    mapApp->isMapSSAqua = IsMapSSAqua(mapApp->mapID);

    roamers = Save_Roamers_Get(mapApp->pokegear->saveData);
    for (i = 0; i < ROAMER_MAX; ++i) {
        if (GetRoamerIsActiveByIndex(roamers, i)) {
            mapApp->roamerLocations[i] = GetRoamerData(Roamers_GetRoamMonStats(roamers, i), ROAMER_DATA_MET_LOCATION);
        } else {
            mapApp->roamerLocations[i] = SPECIES_NONE;
        }
    }
}

static void PokegearMap_ExitInternal(PokegearMapAppData *mapApp) {
    Heap_Free(mapApp->phoneContact);
    FreePhoneBook(mapApp->phoneBook);
    mapApp->pokegear->reselectAppCB = NULL;
    mapApp->pokegear->deselectAppCB = NULL;
    PokegearMap_SaveMapMarkings(mapApp);
    Pokegear_SetMapZoomed(mapApp->pokegear->savePokegear, mapApp->zoomed);
}

static int PokegearMap_MainTask_LoadGFX(PokegearMapAppData *mapApp) {
    if (!PokegearMap_LoadGFX(mapApp)) {
        return PGMAP_MAIN_STATE_LOAD;
    }
    if (mapApp->pokegear->isSwitchApp) {
        return PGMAP_MAIN_STATE_FADE_IN_APP;
    } else {
        return PGMAP_MAIN_STATE_FADE_IN;
    }
}

static int PokegearMap_MainTask_HandleInput(PokegearMapAppData *mapApp) {
    BOOL isTouch = FALSE;
    int input;

    input = PokegearMap_HandleTouchInput(mapApp, &isTouch);
    if (!isTouch) {
        PokegearApp_HandleInputModeChangeToButtons(mapApp->pokegear);
        if (mapApp->pokegear->cursorInAppSwitchZone == TRUE) {
            input = PokegearApp_HandleKeyInput_SwitchApps(mapApp->pokegear);
        } else {
            input = PokegearMap_HandleKeyInput(mapApp);
        }
    }
    mapApp->pokegear->appReturnCode = input;
    switch (input) {
    case TOUCH_MENU_NO_INPUT:
        break;
    case GEAR_RETURN_7:
        return PGMAP_MAIN_STATE_ENTER_MARKING_MODE;
    case GEAR_RETURN_4:
        return PGMAP_MAIN_STATE_FADE_OUT;
    default:
        return PGMAP_MAIN_STATE_FADE_OUT_APP;
    }

    return PGMAP_MAIN_STATE_HANDLE_INPUT;
}

static int PokegearMap_MainTask_UnloadGFX(PokegearMapAppData *mapApp) {
    if (!PokegearMap_UnloadGFX(mapApp)) {
        return PGMAP_MAIN_STATE_UNLOAD;
    }
    mapApp->pokegear->mapSessionState = mapApp->sessionState;
    return PGMAP_MAIN_STATE_QUIT;
}

static int PokegearMap_MainTask_EnterMarkingMode(PokegearMapAppData *mapApp) {
    if (PokegearMap_AnimateSwitchToMarkingMode(mapApp)) {
        return PGMAP_MAIN_STATE_HANDLE_INPUT_MARKING_MODE;
    } else {
        return PGMAP_MAIN_STATE_ENTER_MARKING_MODE;
    }
}

static int PokegearMap_MainTask_HandleInput_MarkingMode(PokegearMapAppData *mapApp) {
    int input = -1;
    BOOL isTouch = FALSE;

    switch (mapApp->draggingType) {
    case PGMAP_DRAG_NONE:
        input = PokegearMap_HandleTouchInput_SelectMarkingsSlot(mapApp, &isTouch);
        break;
    case PGMAP_DRAG_FROM_POOL:
        if (mapApp->pokegear->menuInputState != MENU_INPUT_STATE_BUTTONS) {
            input = PokegearMap_HandleTouchInput_DragItemFromPool(mapApp);
            isTouch = TRUE;
        }
        break;
    case PGMAP_DRAG_FROM_SET:
        if (mapApp->pokegear->menuInputState != MENU_INPUT_STATE_BUTTONS) {
            input = PokegearMap_HandleTouchInput_DragMarkingSlot(mapApp);
            isTouch = TRUE;
        }
        break;
    }
    if (isTouch) {
        if (mapApp->pokegear->menuInputState != MENU_INPUT_STATE_TOUCH) {
            PokegearMap_InMarkingsMode_HideCursor(mapApp);
        }
        mapApp->pokegear->menuInputState = MENU_INPUT_STATE_TOUCH;
        if (input >= 0) {
            return input;
        } else {
            return PGMAP_MAIN_STATE_HANDLE_INPUT_MARKING_MODE;
        }
    }
    if (PokegearApp_HandleInputModeChangeToButtons(mapApp->pokegear) && mapApp->pokegear->menuInputState != mapApp->pokegear->menuInputStateBak) {
        PokegearMap_InMarkingsMode_ShowCursor(mapApp);
        PlaySE(SEQ_SE_GS_GEARCURSOR);
        return PGMAP_MAIN_STATE_HANDLE_INPUT_MARKING_MODE;
    }

    switch (mapApp->draggingType) {
    case PGMAP_DRAG_NONE:
        input = PokegearMap_HandleKeyInput_SelectMarkingsSlot(mapApp);
        break;
    case PGMAP_DRAG_FROM_POOL:
        input = PokegearMap_HandleKeyInput_SelectedIconFromPool(mapApp);
        break;
    case PGMAP_DRAG_FROM_SET:
        input = PokegearMap_HandleKeyInput_SelectedMarkingSlot(mapApp);
        break;
    }
    if (input < 0) {
        input = PGMAP_MAIN_STATE_HANDLE_INPUT_MARKING_MODE;
    }
    return input;
}

static int PokegearMap_MainTask_ExitMarkingMode(PokegearMapAppData *mapApp) {
    if (PokegearMap_AnimateSwitchFromMarkingMode(mapApp)) {
        return PGMAP_MAIN_STATE_HANDLE_INPUT;
    } else {
        return PGMAP_MAIN_STATE_EXIT_MARKING_MODE;
    }
}

static int PokegearMap_MainTask_FadeOutForWordSelect(PokegearMapAppData *mapApp) {
    mapApp->pokegear->isSwitchApp = FALSE;
    mapApp->pokegear->appReturnCode = GEAR_RETURN_CANCEL;
    return PGMAP_MAIN_STATE_FADE_OUT;
}

static int PokegearMap_MainTask_FadeIn(PokegearMapAppData *mapApp) {
    int i;

    switch (mapApp->state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, mapApp->heapID);
        if (mapApp->inMarkingsMode == 1) {
            for (i = 0; i < 4; ++i) {
                ToggleBgLayer(i + GF_BG_LYR_MAIN_1, TRUE);
            }
        } else {
            for (i = 0; i < 8; ++i) {
                ToggleBgLayer(i, TRUE);
            }
        }
        PaletteData_SetAutoTransparent(mapApp->pokegear->plttData, TRUE);
        PaletteData_BlendPalette(mapApp->pokegear->plttData, PLTTBUF_MAIN_BG, 0, 224, 0, 0);
        PaletteData_BlendPalette(mapApp->pokegear->plttData, PLTTBUF_MAIN_OBJ, 64, 192, 0, 0);
        PaletteData_PushTransparentBuffers(mapApp->pokegear->plttData);
        PaletteData_SetAutoTransparent(mapApp->pokegear->plttData, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
        ++mapApp->state;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            mapApp->state = 0;
            if (mapApp->inMarkingsMode == 1) {
                return PGMAP_MAIN_STATE_HANDLE_INPUT_MARKING_MODE;
            } else {
                return PGMAP_MAIN_STATE_HANDLE_INPUT;
            }
        }
        break;
    }

    return PGMAP_MAIN_STATE_FADE_IN;
}

static int PokegearMap_MainTask_FadeOut(PokegearMapAppData *mapApp) {
    switch (mapApp->state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, mapApp->heapID);
        ++mapApp->state;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            for (int i = 0; i < 8; ++i) {
                ToggleBgLayer(i, FALSE);
            }
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
            mapApp->state = 0;
            return PGMAP_MAIN_STATE_UNLOAD;
        }
        break;
    }

    return PGMAP_MAIN_STATE_FADE_OUT;
}

static int PokegearMap_MainTask_FadeInApp(PokegearMapAppData *mapApp) {
    BOOL plltFadeDone;
    BOOL scrollDone;

    switch (mapApp->state) {
    case 0:
        PaletteData_SetAutoTransparent(mapApp->pokegear->plttData, TRUE);
        mapApp->fadeStep = 0;
        for (int i = 0; i < 3; ++i) {
            ToggleBgLayer(i + GF_BG_LYR_MAIN_1, TRUE);
        }
        SetBlendBrightness(0, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3), SCREEN_MASK_MAIN);
        ++mapApp->state;
        break;
    case 1:
        plltFadeDone = PokegearMap_FadeMapScreen(mapApp, 0);
        scrollDone = PokegearMap_RunScrollMarkingsPanelTopScreen(mapApp, 0);
        if (plltFadeDone && scrollDone) {
            ++mapApp->state;
        }
        break;
    case 2:
        PaletteData_SetAutoTransparent(mapApp->pokegear->plttData, FALSE);
        mapApp->state = 0;
        mapApp->markingsPanelScrollActive = 0;
        return PGMAP_MAIN_STATE_HANDLE_INPUT;
    }

    return PGMAP_MAIN_STATE_FADE_IN_APP;
}

static int PokegearMap_MainTask_FadeOutApp(PokegearMapAppData *mapApp) {
    int i;
    BOOL r4;
    BOOL r0;

    switch (mapApp->state) {
    case 0:
        PaletteData_SetAutoTransparent(mapApp->pokegear->plttData, TRUE);
        for (i = 0; i < 3; ++i) {
            ToggleBgLayer(i + GF_BG_LYR_MAIN_1, TRUE);
        }
        PokegearMap_BeginScrollMarkingsPanelTopScreen(mapApp, 1);
        mapApp->fadeStep = 0;
        ++mapApp->state;
        break;
    case 1:
        r4 = PokegearMap_FadeMapScreen(mapApp, 1);
        r0 = PokegearMap_RunScrollMarkingsPanelTopScreen(mapApp, 1);
        if (r4 && r0) {
            ++mapApp->state;
        }
        break;
    case 2:
        PaletteData_BlendPalette(mapApp->pokegear->plttData, PLTTBUF_MAIN_BG, 0, 224, 16, 0);
        PaletteData_BlendPalette(mapApp->pokegear->plttData, PLTTBUF_MAIN_OBJ, 64, 192, 16, 0);
        PaletteData_PushTransparentBuffers(mapApp->pokegear->plttData);
        for (int i = 0; i < 3; ++i) {
            ToggleBgLayer(i + GF_BG_LYR_MAIN_1, FALSE);
        }
        PaletteData_SetAutoTransparent(mapApp->pokegear->plttData, FALSE);
        mapApp->state = 0;
        mapApp->markingsPanelScrollActive = 0;
        return PGMAP_MAIN_STATE_UNLOAD;
    }

    return PGMAP_MAIN_STATE_FADE_OUT_APP;
}

const PokegearMapLocationSpec sLocationSpecs[PGMAP_NUM_LOCATIONS] = {
    {
     .mapId = MAP_SINJOH_RUINS_EXTERIOR,
     .x = 19,
     .y = 2,
     .width = 3,
     .height = 3,
     .objXoffset = 0,
     .objYoffset = 12,
     .flavorText = msg_0273_00065,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 0,
     .tilemapCopyRectSrcY = 0,
     .tilemapCopyRectWidth = 0,
     .tilemapCopyRectHeight = 0,
     },
    {
     .mapId = MAP_SS_AQUA_1F,
     .x = 24,
     .y = 16,
     .width = 3,
     .height = 3,
     .objXoffset = 0,
     .objYoffset = 12,
     .flavorText = msg_0273_00069,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 0,
     .tilemapCopyRectSrcY = 0,
     .tilemapCopyRectWidth = 0,
     .tilemapCopyRectHeight = 0,
     },
    {
     .mapId = MAP_ICE_PATH_1F,
     .x = 21,
     .y = 6,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00055,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_DARK_CAVE_ROUTE_45_SIDE,
     .x = 17,
     .y = 10,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00058,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 10,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_DARK_CAVE_ROUTE_45_SIDE,
     .x = 19,
     .y = 12,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00058,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 12,
     .tilemapCopyRectSrcY = 44,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_DARK_CAVE_ROUTE_45_SIDE,
     .x = 20,
     .y = 8,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00058,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 13,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_SPROUT_TOWER_1F,
     .x = 15,
     .y = 9,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00039,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_UNION_CAVE_1F,
     .x = 14,
     .y = 15,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00046,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 8,
     .tilemapCopyRectSrcY = 44,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_SLOWPOKE_WELL_ENTRANCE,
     .x = 13,
     .y = 16,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00047,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ILEX_FOREST,
     .x = 11,
     .y = 16,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00052,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 45,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_NATIONAL_PARK,
     .x = 10,
     .y = 8,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 4,
     .flavorText = msg_0273_00043,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 0,
     .tilemapCopyRectSrcY = 37,
     .tilemapCopyRectWidth = 2,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_BELL_TOWER_1F,
     .x = 12,
     .y = 6,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00040,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_BURNED_TOWER_1F,
     .x = 11,
     .y = 6,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00042,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_MOUNT_MORTAR_1F_ENTRANCE,
     .x = 14,
     .y = 7,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00054,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_WHIRL_ISLANDS_1F,
     .x = 7,
     .y = 13,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00056,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 1,
     .tilemapCopyRectSrcY = 46,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_TOHJO_FALLS,
     .x = 23,
     .y = 14,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00061,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 6,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_VICTORY_ROAD_1F,
     .x = 28,
     .y = 9,
     .width = 1,
     .height = 2,
     .objXoffset = 0,
     .objYoffset = 4,
     .flavorText = msg_0273_00059,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 3,
     .tilemapCopyRectSrcY = 37,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_VIRIDIAN_FOREST,
     .x = 32,
     .y = 7,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00062,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 15,
     .tilemapCopyRectSrcY = 41,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_DIGLETT_CAVE,
     .x = 32,
     .y = 6,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00033,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 15,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_DIGLETT_CAVE,
     .x = 42,
     .y = 11,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00033,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 28,
     .tilemapCopyRectSrcY = 43,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_MOUNT_MOON,
     .x = 36,
     .y = 5,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00034,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 6,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_CERULEAN_CAVE_1F,
     .x = 40,
     .y = 5,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00035,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROCK_TUNNEL_1F,
     .x = 44,
     .y = 6,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00036,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 31,
     .tilemapCopyRectSrcY = 41,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_10_POWER_PLANT_BROKEN,
     .x = 44,
     .y = 7,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00067,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 31,
     .tilemapCopyRectSrcY = 42,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_SEAFOAM_ISLANDS_1F,
     .x = 35,
     .y = 17,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00038,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 6,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_BATTLE_FRONTIER,
     .x = 6,
     .y = 8,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 4,
     .flavorText = msg_0273_00050,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 0,
     .tilemapCopyRectSrcY = 37,
     .tilemapCopyRectWidth = 2,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_FUCHSIA_PAL_PARK_ENTRANCE,
     .x = 37,
     .y = 14,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00037,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_OLIVINE_LIGHTHOUSE_2F,
     .x = 9,
     .y = 10,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00048,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_PALLET,
     .x = 32,
     .y = 13,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00010,
     .subBg3BlockID = 1,
     .tilemapCopyRectSrcX = 0,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 3,
     },
    {
     .mapId = MAP_VIRIDIAN,
     .x = 31,
     .y = 9,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 4,
     .flavorText = msg_0273_00011,
     .subBg3BlockID = 2,
     .tilemapCopyRectSrcX = 35,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_PEWTER,
     .x = 32,
     .y = 4,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 4,
     .flavorText = msg_0273_00012,
     .subBg3BlockID = 3,
     .tilemapCopyRectSrcX = 35,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_CERULEAN,
     .x = 40,
     .y = 5,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 8,
     .flavorText = msg_0273_00013,
     .subBg3BlockID = 4,
     .tilemapCopyRectSrcX = 18,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_LAVENDER,
     .x = 44,
     .y = 9,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00014,
     .subBg3BlockID = 5,
     .tilemapCopyRectSrcX = 0,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 3,
     },
    {
     .mapId = MAP_VERMILION,
     .x = 40,
     .y = 11,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 4,
     .flavorText = msg_0273_00015,
     .subBg3BlockID = 6,
     .tilemapCopyRectSrcX = 35,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_CELADON,
     .x = 37,
     .y = 9,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 4,
     .flavorText = msg_0273_00016,
     .subBg3BlockID = 7,
     .tilemapCopyRectSrcX = 35,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_FUCHSIA,
     .x = 37,
     .y = 14,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 8,
     .flavorText = msg_0273_00017,
     .subBg3BlockID = 8,
     .tilemapCopyRectSrcX = 18,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_CINNABAR_ISLAND,
     .x = 32,
     .y = 17,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00018,
     .subBg3BlockID = 9,
     .tilemapCopyRectSrcX = 0,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 3,
     },
    {
     .mapId = MAP_SAFFRON,
     .x = 40,
     .y = 8,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 4,
     .flavorText = msg_0273_00020,
     .subBg3BlockID = 10,
     .tilemapCopyRectSrcX = 35,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_NEW_BARK,
     .x = 21,
     .y = 14,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00021,
     .subBg3BlockID = 11,
     .tilemapCopyRectSrcX = 0,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 3,
     },
    {
     .mapId = MAP_CHERRYGROVE,
     .x = 16,
     .y = 14,
     .width = 2,
     .height = 1,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00022,
     .subBg3BlockID = 12,
     .tilemapCopyRectSrcX = 26,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 3,
     },
    {
     .mapId = MAP_VIOLET,
     .x = 14,
     .y = 9,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 8,
     .flavorText = msg_0273_00023,
     .subBg3BlockID = 13,
     .tilemapCopyRectSrcX = 14,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_AZALEA,
     .x = 12,
     .y = 16,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00024,
     .subBg3BlockID = 14,
     .tilemapCopyRectSrcX = 22,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 3,
     },
    {
     .mapId = MAP_CIANWOOD,
     .x = 5,
     .y = 12,
     .width = 1,
     .height = 2,
     .objXoffset = 0,
     .objYoffset = 4,
     .flavorText = msg_0273_00025,
     .subBg3BlockID = 15,
     .tilemapCopyRectSrcX = 3,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_GOLDENROD,
     .x = 9,
     .y = 12,
     .width = 3,
     .height = 2,
     .objXoffset = 8,
     .objYoffset = 4,
     .flavorText = msg_0273_00026,
     .subBg3BlockID = 16,
     .tilemapCopyRectSrcX = 30,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 5,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_OLIVINE,
     .x = 8,
     .y = 9,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00027,
     .subBg3BlockID = 17,
     .tilemapCopyRectSrcX = 6,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_ECRUTEAK,
     .x = 11,
     .y = 6,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 8,
     .flavorText = msg_0273_00028,
     .subBg3BlockID = 18,
     .tilemapCopyRectSrcX = 10,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_MAHOGANY,
     .x = 16,
     .y = 7,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00029,
     .subBg3BlockID = 19,
     .tilemapCopyRectSrcX = 0,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 3,
     },
    {
     .mapId = MAP_BLACKTHORN,
     .x = 20,
     .y = 6,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 8,
     .flavorText = msg_0273_00031,
     .subBg3BlockID = 20,
     .tilemapCopyRectSrcX = 14,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_INDIGO_PLATEAU,
     .x = 28,
     .y = 8,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00019,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_LAKE_OF_RAGE,
     .x = 15,
     .y = 3,
     .width = 3,
     .height = 2,
     .objXoffset = 8,
     .objYoffset = 4,
     .flavorText = msg_0273_00030,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 30,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 5,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_MOUNT_SILVER,
     .x = 25,
     .y = 10,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00057,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_RUINS_OF_ALPH_UNUSED,
     .x = 13,
     .y = 10,
     .width = 1,
     .height = 2,
     .objXoffset = 0,
     .objYoffset = 4,
     .flavorText = msg_0273_00045,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 3,
     .tilemapCopyRectSrcY = 32,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_SAFARI_ZONE_GATE,
     .x = 2,
     .y = 10,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 4,
     .flavorText = msg_0273_00068,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 9,
     .tilemapCopyRectSrcY = 37,
     .tilemapCopyRectWidth = 2,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_ROUTE_1,
     .x = 32,
     .y = 11,
     .width = 1,
     .height = 2,
     .objXoffset = 0,
     .objYoffset = 4,
     .flavorText = msg_0273_00070,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 15,
     .tilemapCopyRectSrcY = 46,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_ROUTE_2,
     .x = 32,
     .y = 8,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00071,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 15,
     .tilemapCopyRectSrcY = 42,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_3,
     .x = 34,
     .y = 5,
     .width = 2,
     .height = 1,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00072,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 25,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 2,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_4,
     .x = 37,
     .y = 5,
     .width = 3,
     .height = 1,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00073,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 28,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_5,
     .x = 40,
     .y = 7,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00074,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 25,
     .tilemapCopyRectSrcY = 44,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_6,
     .x = 40,
     .y = 10,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00075,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 26,
     .tilemapCopyRectSrcY = 44,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_7,
     .x = 39,
     .y = 9,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00076,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 24,
     .tilemapCopyRectSrcY = 44,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_8,
     .x = 42,
     .y = 9,
     .width = 2,
     .height = 1,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00077,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 27,
     .tilemapCopyRectSrcY = 44,
     .tilemapCopyRectWidth = 2,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_9,
     .x = 42,
     .y = 6,
     .width = 2,
     .height = 1,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00078,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 29,
     .tilemapCopyRectSrcY = 41,
     .tilemapCopyRectWidth = 2,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_10,
     .x = 44,
     .y = 8,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00079,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 31,
     .tilemapCopyRectSrcY = 43,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_11,
     .x = 43,
     .y = 11,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00080,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 29,
     .tilemapCopyRectSrcY = 43,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_12,
     .x = 44,
     .y = 10,
     .width = 1,
     .height = 3,
     .objXoffset = 0,
     .objYoffset = 4,
     .flavorText = msg_0273_00081,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 30,
     .tilemapCopyRectSrcY = 42,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 3,
     },
    {
     .mapId = MAP_ROUTE_13,
     .x = 42,
     .y = 13,
     .width = 3,
     .height = 1,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00082,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 28,
     .tilemapCopyRectSrcY = 45,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_14,
     .x = 42,
     .y = 14,
     .width = 1,
     .height = 2,
     .objXoffset = 0,
     .objYoffset = 4,
     .flavorText = msg_0273_00083,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 28,
     .tilemapCopyRectSrcY = 46,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_ROUTE_15,
     .x = 39,
     .y = 15,
     .width = 3,
     .height = 1,
     .objXoffset = 12,
     .objYoffset = 0,
     .flavorText = msg_0273_00084,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 25,
     .tilemapCopyRectSrcY = 47,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_16,
     .x = 35,
     .y = 10,
     .width = 2,
     .height = 1,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00085,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 23,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 2,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_17,
     .x = 35,
     .y = 11,
     .width = 1,
     .height = 4,
     .objXoffset = 0,
     .objYoffset = 12,
     .flavorText = msg_0273_00086,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 23,
     .tilemapCopyRectSrcY = 41,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_ROUTE_18,
     .x = 35,
     .y = 15,
     .width = 2,
     .height = 1,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00087,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 23,
     .tilemapCopyRectSrcY = 45,
     .tilemapCopyRectWidth = 2,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_19,
     .x = 37,
     .y = 16,
     .width = 1,
     .height = 2,
     .objXoffset = 0,
     .objYoffset = 4,
     .flavorText = msg_0273_00088,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 22,
     .tilemapCopyRectSrcY = 45,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_ROUTE_20,
     .x = 33,
     .y = 17,
     .width = 4,
     .height = 1,
     .objXoffset = 8,
     .objYoffset = 0,
     .flavorText = msg_0273_00089,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 18,
     .tilemapCopyRectSrcY = 46,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_21,
     .x = 32,
     .y = 14,
     .width = 1,
     .height = 3,
     .objXoffset = 0,
     .objYoffset = 12,
     .flavorText = msg_0273_00090,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 17,
     .tilemapCopyRectSrcY = 45,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 3,
     },
    {
     .mapId = MAP_ROUTE_22,
     .x = 29,
     .y = 10,
     .width = 2,
     .height = 1,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00091,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 21,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 2,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_24,
     .x = 41,
     .y = 4,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00093,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 24,
     .tilemapCopyRectSrcY = 42,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_25,
     .x = 41,
     .y = 3,
     .width = 4,
     .height = 1,
     .objXoffset = 12,
     .objYoffset = 0,
     .flavorText = msg_0273_00094,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 24,
     .tilemapCopyRectSrcY = 41,
     .tilemapCopyRectWidth = 4,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_26,
     .x = 28,
     .y = 11,
     .width = 1,
     .height = 4,
     .objXoffset = 0,
     .objYoffset = 12,
     .flavorText = msg_0273_00095,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 20,
     .tilemapCopyRectSrcY = 41,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_ROUTE_27,
     .x = 22,
     .y = 14,
     .width = 6,
     .height = 1,
     .objXoffset = 15,
     .objYoffset = 0,
     .flavorText = msg_0273_00096,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 14,
     .tilemapCopyRectSrcY = 44,
     .tilemapCopyRectWidth = 6,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_28,
     .x = 26,
     .y = 10,
     .width = 2,
     .height = 1,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00097,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 18,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 2,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_29,
     .x = 18,
     .y = 14,
     .width = 3,
     .height = 1,
     .objXoffset = 12,
     .objYoffset = 0,
     .flavorText = msg_0273_00098,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 11,
     .tilemapCopyRectSrcY = 46,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_30,
     .x = 17,
     .y = 11,
     .width = 1,
     .height = 3,
     .objXoffset = 0,
     .objYoffset = 4,
     .flavorText = msg_0273_00099,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 10,
     .tilemapCopyRectSrcY = 41,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 3,
     },
    {
     .mapId = MAP_ROUTE_31,
     .x = 16,
     .y = 10,
     .width = 2,
     .height = 1,
     .objXoffset = 1,
     .objYoffset = 0,
     .flavorText = msg_0273_00100,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 9,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_32,
     .x = 14,
     .y = 11,
     .width = 1,
     .height = 4,
     .objXoffset = 0,
     .objYoffset = 12,
     .flavorText = msg_0273_00101,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 8,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_ROUTE_33,
     .x = 14,
     .y = 16,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00102,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 8,
     .tilemapCopyRectSrcY = 45,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_34,
     .x = 11,
     .y = 14,
     .width = 1,
     .height = 3,
     .objXoffset = 0,
     .objYoffset = 4,
     .flavorText = msg_0273_00103,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 43,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_ROUTE_35,
     .x = 11,
     .y = 10,
     .width = 1,
     .height = 2,
     .objXoffset = 0,
     .objYoffset = 4,
     .flavorText = msg_0273_00104,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 2,
     .tilemapCopyRectSrcY = 45,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_ROUTE_36,
     .x = 11,
     .y = 9,
     .width = 3,
     .height = 1,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00105,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 2,
     .tilemapCopyRectSrcY = 44,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_37,
     .x = 12,
     .y = 8,
     .width = 1,
     .height = 1,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00106,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 3,
     .tilemapCopyRectSrcY = 43,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_38,
     .x = 9,
     .y = 7,
     .width = 2,
     .height = 1,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00107,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 1,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 2,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_39,
     .x = 8,
     .y = 7,
     .width = 1,
     .height = 2,
     .objXoffset = 0,
     .objYoffset = 4,
     .flavorText = msg_0273_00108,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 0,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_ROUTE_40,
     .x = 7,
     .y = 10,
     .width = 1,
     .height = 2,
     .objXoffset = 0,
     .objYoffset = 4,
     .flavorText = msg_0273_00109,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 1,
     .tilemapCopyRectSrcY = 42,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_ROUTE_41,
     .x = 6,
     .y = 12,
     .width = 2,
     .height = 2,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00110,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 0,
     .tilemapCopyRectSrcY = 44,
     .tilemapCopyRectWidth = 2,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_ROUTE_42,
     .x = 13,
     .y = 7,
     .width = 3,
     .height = 1,
     .objXoffset = 2,
     .objYoffset = 0,
     .flavorText = msg_0273_00111,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 2,
     .tilemapCopyRectSrcY = 42,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_43,
     .x = 16,
     .y = 5,
     .width = 1,
     .height = 2,
     .objXoffset = 0,
     .objYoffset = 4,
     .flavorText = msg_0273_00112,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 3,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_ROUTE_44,
     .x = 17,
     .y = 7,
     .width = 3,
     .height = 1,
     .objXoffset = 8,
     .objYoffset = 0,
     .flavorText = msg_0273_00113,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 5,
     .tilemapCopyRectSrcY = 42,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_45,
     .x = 20,
     .y = 9,
     .width = 1,
     .height = 4,
     .objXoffset = 0,
     .objYoffset = 8,
     .flavorText = msg_0273_00114,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 13,
     .tilemapCopyRectSrcY = 41,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 4,
     },
    {
     .mapId = MAP_ROUTE_46,
     .x = 19,
     .y = 13,
     .width = 1,
     .height = 2,
     .objXoffset = 0,
     .objYoffset = 0,
     .flavorText = msg_0273_00115,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 12,
     .tilemapCopyRectSrcY = 45,
     .tilemapCopyRectWidth = 1,
     .tilemapCopyRectHeight = 1,
     },
    {
     .mapId = MAP_ROUTE_47,
     .x = 2,
     .y = 13,
     .width = 3,
     .height = 2,
     .objXoffset = 8,
     .objYoffset = 4,
     .flavorText = msg_0273_00116,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 32,
     .tilemapCopyRectSrcY = 41,
     .tilemapCopyRectWidth = 3,
     .tilemapCopyRectHeight = 2,
     },
    {
     .mapId = MAP_ROUTE_48,
     .x = 2,
     .y = 12,
     .width = 2,
     .height = 1,
     .objXoffset = 4,
     .objYoffset = 0,
     .flavorText = msg_0273_00117,
     .subBg3BlockID = 0,
     .tilemapCopyRectSrcX = 32,
     .tilemapCopyRectSrcY = 40,
     .tilemapCopyRectWidth = 2,
     .tilemapCopyRectHeight = 1,
     },
};
