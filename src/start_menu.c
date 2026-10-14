#include "start_menu.h"

#include "global.h"

#include "constants/field/map_load.h"
#include "constants/map_sections.h"
#include "constants/sndseq.h"
#include "constants/start_menu_icons.h"
#include "constants/std_script.h"

#include "field/fieldmap.h"
#include "msgdata/msg/msg_0196.h"

#include "bag_view.h"
#include "field_bgm.h"
#include "field_move.h"
#include "field_move_environment.h"
#include "field_system.h"
#include "field_use_item.h"
#include "gf_gfx_loader.h"
#include "launch_application.h"
#include "map_header.h"
#include "map_object.h"
#include "menu_input_state.h"
#include "overlay_01.h"
#include "field/bottom_screen_app.h"
#include "pokedex.h"
#include "pokedex_util.h"
#include "save_local_field_data.h"
#include "save_vars_flags.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_system.h"
#include "sprite_transfer.h"
#include "sys_flags.h"
#include "system.h"
#include "sound_playback.h"
#include "comm_wireless.h"
#include "comm_state.h"
#include "union_room_talk.h"
#include "union_room_objects.h"
#include "player_avatar_movement.h"
#include "frontier_field_support.h"
#include "field_warp_state.h"
#include "trainer_card_data.h"
#include "evolution_scene.h"
#include "gear_phone_ring_manager.h"

typedef enum StartMenuAction {
    START_MENU_ACTION_POKEDEX,
    START_MENU_ACTION_POKEMON,
    START_MENU_ACTION_BAG,
    START_MENU_ACTION_TRAINER_CARD,
    START_MENU_ACTION_SAVE,
    START_MENU_ACTION_OPTIONS,
    START_MENU_ACTION_RUNNING_SHOES,
    START_MENU_ACTION_7,
    START_MENU_ACTION_RETIRE,
    START_MENU_ACTION_9,
    START_MENU_ACTION_10,
    START_MENU_ACTION_POKEGEAR,
    START_MENU_ACTION_12,
} StartMenuAction;

typedef enum StartMenuActionDisable {
    START_MENU_ACTION_DISABLE_POKEDEX,
    START_MENU_ACTION_DISABLE_POKEMON,
    START_MENU_ACTION_DISABLE_BAG,
    START_MENU_ACTION_DISABLE_TRAINER_CARD,
    START_MENU_ACTION_DISABLE_SAVE,
    START_MENU_ACTION_DISABLE_OPTIONS,
    START_MENU_ACTION_DISABLE_RUNNING_SHOES,
    START_MENU_ACTION_DISABLE_7,
    START_MENU_ACTION_DISABLE_RETIRE,
    START_MENU_ACTION_DISABLE_POKEGEAR,
} StartMenuActionDisable;

typedef struct StartMenuActionFunc {
    int ident;
    TaskFunc func;
} StartMenuActionFunc;

typedef struct StartMenuEvolutionArgs {
    u8 partySlot;
    u8 evoType;
    u16 evoParam;
    u16 species;
    int evoMethod;
} StartMenuEvolutionArgs;

typedef struct StartMenuMoveLearnReturn {
    u16 itemId;
    u16 levelUpMoveSearchState;
} StartMenuMoveLearnReturn;

typedef struct StartMenuAfterEvoPartySlotBak {
    int partySlot;
} StartMenuAfterEvoPartySlotBak;

#define STARTMENUTASKFUNC_CANCEL ((TaskFunc) - 2)
#define STARTMENUTASKFUNC_NONE   ((TaskFunc) - 1)

static StartMenuTaskData *StartMenu_Create(void);
static u32 FieldSystem_GetStartMenuButtonInhibitFlags_Normal(FieldSystem *fieldSystem);
static u32 FieldSystem_GetStartMenuButtonInhibitFlags_Safari(FieldSystem *fieldSystem);
static u32 FieldSystem_GetStartMenuButtonInhibitFlags_BugContest(FieldSystem *fieldSystem);
static u32 FieldSystem_GetStartMenuButtonInhibitFlags_PalPark(FieldSystem *fieldSystem);
static u32 FieldSystem_GetStartMenuButtonInhibitFlags_BattleTowerMultiPartnerSelectRoom(FieldSystem *fieldSystem);
static u32 FieldSystem_GetStartMenuButtonInhibitFlags_Union(FieldSystem *fieldSystem);
static u32 FieldSystem_GetStartMenuButtonInhibitFlags_Colosseum(FieldSystem *fieldSystem);
static BOOL Task_StartMenu(TaskManager *taskManager);
static void Task_StartMenu_DrawCursor(TaskManager *taskManager);
static void StartMenuButton_Insert(u8 *insertionOrderDest, u8 *displayOrderDest, u32 *pLength, u8 item, vu32 position);
static u32 StartMenu_BuildActionLists(StartMenuTaskData *startMenu, u8 *insertionOrderDest, u8 *displayOrderDest);
static void StartMenu_RemoveCursorAndIcons(StartMenuTaskData *startMenu, FieldSystem *fieldSystem);
static BOOL FieldSystem_StartMenuActionIsAvailable(FieldSystem *fieldSystem, StartMenuAction action);
static void FieldSystem_ResetGearRingManagerIfNotSelectingSaveOrPokegear(FieldSystem *fieldSystem);
static BOOL Task_StartMenu_HandleInput(TaskManager *taskManager);
static BOOL StartMenu_HandleKeyInput(TaskManager *taskManager, FieldSystem *fieldSystem, StartMenuTaskData *startMenu);
static BOOL StartMenu_HandleTouchInput(TaskManager *taskManager, FieldSystem *fieldSystem, StartMenuTaskData *startMenu);
static void StartMenu_ResumeUnionRoomSearch(StartMenuTaskData *startMenu, FieldSystem *fieldSystem);
static void StartMenu_CreateCursor(StartMenuTaskData *startMenu, u8 *insertionOrder, u32 numButtons, u8 gender);
static void StartMenu_DestroyCursor(StartMenuTaskData *startMenu);
static void Task_StartMenu_WaitFade(TaskManager *taskManager);
static void Task_StartMenu_WaitApp(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleSelection_Pokedex(TaskManager *taskManager);
static BOOL Task_StartMenu_OpenPokedex(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleReturn_Pokedex(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleSelection_Pokemon(TaskManager *taskManager);
static BOOL Task_StartMenu_Pokemon(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleSelection_Bag(TaskManager *taskManager);
static BOOL Task_StartMenu_Bag(TaskManager *taskManager);
static void StartMenu_InitPartyMenuArgs(PartyMenuArgs *partyMenuArgs, FieldSystem *fieldSystem, StartMenuTaskData *startMenu);
static BOOL Task_StartMenu_HandleReturn(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleSelection_TrainerCard(TaskManager *taskManager);
static BOOL Task_StartMenu_TrainerCard(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleReturn_TrainerCard(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleSelection_Save(TaskManager *taskManager);
static void Task_StartMenu_WaitSaveAppOpen(TaskManager *taskManager);
static void Task_StartMenu_WaitSaveAppClose(TaskManager *taskManager);
static BOOL StartMenu_SelectUnionChatLog(TaskManager *taskManager);
static void StartMenu_State19_Advance(TaskManager *taskManager);
static void StartMenu_State20_Close(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleSelection_Options(TaskManager *taskManager);
static BOOL Task_StartMenu_Options(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleReturn_Options(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleSelection_RemovedEasyChatThing(TaskManager *taskManager);
static BOOL Task_StartMenu_RemovedEasyChatThing(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleReturn_RemovedEasyChatThing(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleSelection_Retire(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleSelection_Pokegear(TaskManager *taskManager);
static BOOL Task_StartMenu_Pokegear(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleReturn_Pokegear(TaskManager *taskManager);
static BOOL Task_StartMenu_HandleReturn_Summary(TaskManager *taskManager);
static void StartMenu_HandleBagAction1(TaskManager *taskManager, int itemId);
static BOOL StartMenu_ReopenBagAtBerries(TaskManager *taskManager);
static void StartMenu_ReturnToPartyMenuAfterMail(FieldSystem *fieldSystem, StartMenuTaskData *startMenu, u8 context);
static void Task_StartMenu_Evolution(TaskManager *taskManager);
static void Task_StartMenu_WaitEvolution(TaskManager *taskManager);

static const int sActionToIconIndex[] = {
    0,
    1,
    2,
    4,
    5,
    6,
    100,
    100,
    100,
    100,
    100,
    3,
};

static const StartMenuActionFunc sStartMenuActions[] = {
    [START_MENU_ACTION_POKEDEX] = { .ident = msg_0196_00000, .func = Task_StartMenu_HandleSelection_Pokedex              },
    [START_MENU_ACTION_POKEMON] = { .ident = msg_0196_00001, .func = Task_StartMenu_HandleSelection_Pokemon              },
    [START_MENU_ACTION_BAG] = { .ident = msg_0196_00002, .func = Task_StartMenu_HandleSelection_Bag                  },
    [START_MENU_ACTION_TRAINER_CARD] = { .ident = msg_0196_00003, .func = Task_StartMenu_HandleSelection_TrainerCard          },
    [START_MENU_ACTION_SAVE] = { .ident = msg_0196_00004, .func = Task_StartMenu_HandleSelection_Save                 },
    [START_MENU_ACTION_OPTIONS] = { .ident = msg_0196_00005, .func = Task_StartMenu_HandleSelection_Options              },
    [START_MENU_ACTION_RUNNING_SHOES] = { .ident = msg_0196_00006, .func = STARTMENUTASKFUNC_CANCEL                            },
    [START_MENU_ACTION_7] = { .ident = msg_0196_00007, .func = Task_StartMenu_HandleSelection_RemovedEasyChatThing },
    [START_MENU_ACTION_RETIRE] = { .ident = msg_0196_00008, .func = Task_StartMenu_HandleSelection_Retire               },
    [START_MENU_ACTION_9] = { .ident = msg_0196_00014, .func = Task_StartMenu_HandleSelection_Pokegear             },
    [START_MENU_ACTION_10] = { .ident = msg_0196_00014, .func = Task_StartMenu_HandleSelection_Pokegear             },
    [START_MENU_ACTION_POKEGEAR] = { .ident = msg_0196_00014, .func = Task_StartMenu_HandleSelection_Pokegear             },
    [START_MENU_ACTION_12] = { .ident = msg_0196_00014, .func = StartMenu_SelectUnionChatLog                                        },
};

static const u8 _020FA0AC[] = {
    1, 4
};

const u8 _020FA0B0[] = {
    0, 1, 2, 3, 4
};

static const u8 sPockets[] = {
    POCKET_ITEMS,
    POCKET_MEDICINE,
    POCKET_BALLS,
    POCKET_TMHMS,
    POCKET_BERRIES,
    POCKET_MAIL,
    POCKET_BATTLE_ITEMS,
    POCKET_KEY_ITEMS,
    POCKET_BAG_VIEW_END
};

BOOL FieldSystem_MapIsNotMysteryZone(FieldSystem *fieldSystem) {
    return MapHeader_GetMapSec(fieldSystem->location->mapId) != MAPSEC_MYSTERY_ZONE;
}

void StartMenu_Init(FieldSystem *fieldSystem) {
    StartMenuTaskData *startMenu = StartMenu_Create();
    if (Save_VarsFlags_CheckSafariSysFlag(Save_VarsFlags_Get(fieldSystem->saveData)) == TRUE) {
        startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_Safari(fieldSystem);
    } else if (Save_VarsFlags_CheckBugContestFlag(Save_VarsFlags_Get(fieldSystem->saveData)) == TRUE) {
        startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_BugContest(fieldSystem);
    } else if (Save_VarsFlags_CheckPalParkSysFlag(Save_VarsFlags_Get(fieldSystem->saveData)) == TRUE) {
        startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_PalPark(fieldSystem);
    } else if (FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(fieldSystem) == TRUE) {
        startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_BattleTowerMultiPartnerSelectRoom(fieldSystem);
    } else {
        startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_Normal(fieldSystem);
    }
    startMenu->inUnionRoom = FALSE;
    if (PlayerAvatar_IsBetweenSteps(fieldSystem->playerAvatar) == TRUE) {
        PlayerAvatar_StopAndFace(fieldSystem->playerAvatar, PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar));
    }
    FieldSystem_CreateTask(fieldSystem, Task_StartMenu, startMenu);
}

void StartMenu_InitUnionRoom(FieldSystem *fieldSystem) {
    StartMenuTaskData *startMenu = StartMenu_Create();
    startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_Union(fieldSystem);
    startMenu->inUnionRoom = TRUE;
    if (PlayerAvatar_IsBetweenSteps(fieldSystem->playerAvatar) == TRUE) {
        PlayerAvatar_StopAndFace(fieldSystem->playerAvatar, PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar));
    }
    FieldSystem_CreateTask(fieldSystem, Task_StartMenu, startMenu);
}

void StartMenu_InitColosseum(FieldSystem *fieldSystem) {
    StartMenuTaskData *startMenu = StartMenu_Create();
    startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_Colosseum(fieldSystem);
    startMenu->inUnionRoom = FALSE;
    if (PlayerAvatar_IsBetweenSteps(fieldSystem->playerAvatar) == TRUE) {
        PlayerAvatar_StopAndFace(fieldSystem->playerAvatar, PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar));
    }
    FieldSystem_CreateTask(fieldSystem, Task_StartMenu, startMenu);
}

void StartMenu_ReopenAfterScript(FieldSystem *fieldSystem) {
    PlaySE(SEQ_SE_DP_WIN_OPEN);
    StartMenuTaskData *startMenu = StartMenu_Create();
    startMenu->inUnionRoom = FALSE;
    if (Save_VarsFlags_CheckSafariSysFlag(Save_VarsFlags_Get(fieldSystem->saveData)) == TRUE) {
        startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_Safari(fieldSystem);
    } else if (Save_VarsFlags_CheckBugContestFlag(Save_VarsFlags_Get(fieldSystem->saveData)) == TRUE) {
        startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_BugContest(fieldSystem);
    } else if (Save_VarsFlags_CheckPalParkSysFlag(Save_VarsFlags_Get(fieldSystem->saveData)) == TRUE) {
        startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_PalPark(fieldSystem);
    } else if (FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(fieldSystem) == TRUE) {
        startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_BattleTowerMultiPartnerSelectRoom(fieldSystem);
    } else if (fieldSystem->mapLoadType == MAP_LOAD_TYPE_COLOSSEUM) {
        startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_Colosseum(fieldSystem);
    } else if (fieldSystem->mapLoadType == MAP_LOAD_TYPE_UNION) {
        startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_Union(fieldSystem);
        startMenu->inUnionRoom = TRUE;
    } else {
        startMenu->inhibitIconFlags = FieldSystem_GetStartMenuButtonInhibitFlags_Normal(fieldSystem);
    }
    TaskManager_Jump(fieldSystem->taskman, Task_StartMenu, startMenu);
}

static StartMenuTaskData *StartMenu_Create(void) {
    StartMenuTaskData *ret = Heap_Alloc(HEAP_ID_FIELD2, sizeof(StartMenuTaskData));
    MI_CpuClearFast(ret, sizeof(StartMenuTaskData));
    ret->state = 0;
    ret->lastButtonSelected = 0;
    ret->exitTaskEnvironment = NULL;
    return ret;
}

static u32 FieldSystem_GetStartMenuButtonInhibitFlags_Normal(FieldSystem *fieldSystem) {
    u32 ret = 0;
    if (!CheckGotPokedex(Save_VarsFlags_Get(fieldSystem->saveData))) {
        ret |= (1 << START_MENU_ACTION_DISABLE_POKEDEX);
    }
    if (!CheckGotStarter(Save_VarsFlags_Get(fieldSystem->saveData))) {
        ret |= (1 << START_MENU_ACTION_DISABLE_POKEMON);
    }
    if (!CheckGotMenuIconI(Save_VarsFlags_Get(fieldSystem->saveData), START_MENU_ICON_UNLOCK_BAG)) {
        ret |= (1 << START_MENU_ACTION_DISABLE_BAG);
    }
    if (!CheckGotPokegear(Save_VarsFlags_Get(fieldSystem->saveData))) {
        ret |= (1 << START_MENU_ACTION_DISABLE_POKEGEAR);
    }
    if (MapHeader_MapIsAmitySquare(fieldSystem->location->mapId) == TRUE) {
        ret |= (1 << START_MENU_ACTION_DISABLE_POKEMON) | (1 << START_MENU_ACTION_DISABLE_BAG);
    }
    ret |= (1 << START_MENU_ACTION_DISABLE_7) | (1 << START_MENU_ACTION_DISABLE_RETIRE);
    return ret;
}

static u32 FieldSystem_GetStartMenuButtonInhibitFlags_Safari(FieldSystem *fieldSystem) {
    return (1 << START_MENU_ACTION_DISABLE_SAVE) | (1 << START_MENU_ACTION_DISABLE_7);
}

static u32 FieldSystem_GetStartMenuButtonInhibitFlags_BugContest(FieldSystem *fieldSystem) {
    return (1 << START_MENU_ACTION_DISABLE_BAG) | (1 << START_MENU_ACTION_DISABLE_SAVE) | (1 << START_MENU_ACTION_DISABLE_7);
}

static u32 FieldSystem_GetStartMenuButtonInhibitFlags_PalPark(FieldSystem *fieldSystem) {
    return (1 << START_MENU_ACTION_DISABLE_BAG) | (1 << START_MENU_ACTION_DISABLE_SAVE) | (1 << START_MENU_ACTION_DISABLE_7);
}

static u32 FieldSystem_GetStartMenuButtonInhibitFlags_BattleTowerMultiPartnerSelectRoom(FieldSystem *fieldSystem) {
    return (1 << START_MENU_ACTION_DISABLE_POKEDEX) | (1 << START_MENU_ACTION_DISABLE_BAG) | (1 << START_MENU_ACTION_DISABLE_SAVE) | (1 << START_MENU_ACTION_DISABLE_7) | (1 << START_MENU_ACTION_DISABLE_RETIRE) | (1 << START_MENU_ACTION_DISABLE_POKEGEAR);
}

static u32 FieldSystem_GetStartMenuButtonInhibitFlags_Union(FieldSystem *fieldSystem) {
    return (1 << START_MENU_ACTION_DISABLE_SAVE) | (1 << START_MENU_ACTION_DISABLE_RETIRE);
}

static u32 FieldSystem_GetStartMenuButtonInhibitFlags_Colosseum(FieldSystem *fieldSystem) {
    return (1 << START_MENU_ACTION_DISABLE_POKEDEX) | (1 << START_MENU_ACTION_DISABLE_SAVE) | (1 << START_MENU_ACTION_DISABLE_7) | (1 << START_MENU_ACTION_DISABLE_RETIRE) | (1 << START_MENU_ACTION_DISABLE_POKEGEAR);
}

static BOOL Task_StartMenu(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    switch (startMenu->state) {
    case START_MENU_STATE_INIT:
        MapObjectManager_PauseAllMovement(fieldSystem->mapObjectManager);
        if (FieldSystem_GetBottomScreenAppId(fieldSystem)) {
            FieldSystem_SwitchBottomScreenApp(fieldSystem, 0, NULL);
            startMenu->state = START_MENU_STATE_2;
            break;
        }
        // fallthrough
    case START_MENU_STATE_INIT_FORCE_CURSOR:
        ItemCheckUseData_Init(fieldSystem, &startMenu->itemCheckUseData);
        FieldMove_InitCheckData(fieldSystem, &startMenu->fieldMoveCheckData);
        Task_StartMenu_DrawCursor(taskManager);
        fieldSystem->startMenuState = 1;
        startMenu->state = START_MENU_STATE_HANDLE_INPUT;
        break;
    case START_MENU_STATE_2:
        if (FieldSystem_GetBottomScreenAppState(fieldSystem) == 6) {
            Task_StartMenu_DrawCursor(taskManager);
            fieldSystem->startMenuState = 1;
        } else if (FieldSystem_GetBottomScreenAppState(fieldSystem) == 1) {
            ItemCheckUseData_Init(fieldSystem, &startMenu->itemCheckUseData);
            FieldMove_InitCheckData(fieldSystem, &startMenu->fieldMoveCheckData);
            startMenu->state = START_MENU_STATE_HANDLE_INPUT;
        }
        break;
    case START_MENU_STATE_HANDLE_INPUT:
        if (!Task_StartMenu_HandleInput(taskManager)) {
            return FALSE;
        }
        break;
    case START_MENU_STATE_WAIT_FADE:
        Task_StartMenu_WaitFade(taskManager);
        break;
    case START_MENU_STATE_WAIT_APP:
        Task_StartMenu_WaitApp(taskManager);
        break;
    case START_MENU_STATE_SAVE:
        Task_StartMenu_WaitSaveAppOpen(taskManager);
        break;
    case START_MENU_STATE_7:
        Task_StartMenu_WaitSaveAppClose(taskManager);
        break;
    case START_MENU_STATE_EVOLUTION:
        Task_StartMenu_Evolution(taskManager);
        break;
    case START_MENU_STATE_WAIT_EVOLUTION:
        Task_StartMenu_WaitEvolution(taskManager);
        break;
    case START_MENU_STATE_RETURN:
        if (FieldSystem_IsOverworldRestored(fieldSystem)) {
            MapObjectManager_PauseAllMovement(fieldSystem->mapObjectManager);
            Task_StartMenu_DrawCursor(taskManager);
            fieldSystem->startMenuState = 1;
            FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_IN);
            startMenu->state = START_MENU_STATE_RETURN_WAIT_FADE;
        }
        break;
    case START_MENU_STATE_10:
        if (FieldSystem_IsOverworldRestored(fieldSystem)) {
            FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_IN);
            startMenu->state = START_MENU_STATE_11;
        }
        break;
    case START_MENU_STATE_11:
        if (IsPaletteFadeFinished()) {
            StartMenu_ResumeUnionRoomSearch(startMenu, fieldSystem);
            Heap_Free(startMenu);
            MapObjectManager_UnpauseAllMovement(fieldSystem->mapObjectManager);
            return TRUE;
        }
        break;
    case START_MENU_STATE_12:
        if (FieldSystem_IsOverworldRestored(fieldSystem)) {
            MapObjectManager_PauseAllMovement(fieldSystem->mapObjectManager);
            FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_IN);
            startMenu->state = START_MENU_STATE_13;
        }
        break;
    case START_MENU_STATE_13:
        if (IsPaletteFadeFinished()) {
            TaskManager_Jump(taskManager, startMenu->exitTaskFunc, startMenu->exitTaskEnvironment);
            Heap_Free(startMenu);
        }
        break;
    case START_MENU_STATE_14:
        startMenu->exitTaskFunc(taskManager);
        break;
    case START_MENU_STATE_18:
        Heap_Free(startMenu);
        MapObjectManager_UnpauseAllMovement(fieldSystem->mapObjectManager);
        return TRUE;
    case START_MENU_STATE_CLOSE:
        StartMenu_ResumeUnionRoomSearch(startMenu, fieldSystem);
        StartMenu_RemoveCursorAndIcons(startMenu, fieldSystem);
        fieldSystem->startMenuState = 2;
        ScheduleBgTilemapBufferTransfer(fieldSystem->bgConfig, GF_BG_LYR_MAIN_3);
        Heap_Free(startMenu);
        MapObjectManager_UnpauseAllMovement(fieldSystem->mapObjectManager);
        return TRUE;
    case START_MENU_STATE_RETURN_WAIT_FADE:
        if (IsPaletteFadeFinished()) {
            startMenu->state = START_MENU_STATE_HANDLE_INPUT;
        }
        break;
    case START_MENU_STATE_19:
        StartMenu_State19_Advance(taskManager);
        break;
    case START_MENU_STATE_20:
        StartMenu_State20_Close(taskManager);
        break;
    }
    if (startMenu->cursorActive) {
        SpriteList_RenderAndAnimateSprites(startMenu->spriteList);
    }
    return FALSE;
}

static void Task_StartMenu_DrawCursor(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    u32 numActiveButtons = StartMenu_BuildActionLists(startMenu, startMenu->insertionOrder, startMenu->selectionToAction);
    startMenu->numActiveButtons = numActiveButtons;
    startMenu->lastButtonSelected = 0;
    for (int i = 0; i < numActiveButtons; ++i) {
        if (fieldSystem->unk90 == startMenu->insertionOrder[i]) {
            startMenu->lastButtonSelected = i;
        }
    }
    startMenu->cursorActive = TRUE;
    GfGfxLoader_LoadCharData(NARC_a_0_1_4, 12, fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_FIELD2);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_4, 15, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x1C0, 0x20, HEAP_ID_FIELD2);
    GfGfxLoader_LoadScrnData(NARC_a_0_1_4, 13, fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_FIELD2);
    StartMenu_CreateCursor(startMenu, startMenu->insertionOrder, numActiveButtons, PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(fieldSystem->saveData)));
}

static void StartMenuButton_Insert(u8 *insertionOrderDest, u8 *displayOrderDest, u32 *pLength, u8 item, vu32 position) {
    insertionOrderDest[*pLength] = item;
    if (position == -1u) {
        position = *pLength;
    }
    displayOrderDest[position] = item;
    ++(*pLength);
}

static u32 StartMenu_BuildActionLists(StartMenuTaskData *startMenu, u8 *insertionOrderDest, u8 *displayOrderDest) {
    u32 numIcons = 0;

    if (!(startMenu->inhibitIconFlags & (1 << START_MENU_ACTION_DISABLE_RETIRE))) {
        StartMenuButton_Insert(insertionOrderDest, displayOrderDest, &numIcons, START_MENU_ACTION_RETIRE, -1u);
    }
    if (!(startMenu->inhibitIconFlags & (1 << START_MENU_ACTION_DISABLE_7))) {
        StartMenuButton_Insert(insertionOrderDest, displayOrderDest, &numIcons, START_MENU_ACTION_7, -1u);
    }
    if (!(startMenu->inhibitIconFlags & (1 << START_MENU_ACTION_DISABLE_POKEDEX))) {
        StartMenuButton_Insert(insertionOrderDest, displayOrderDest, &numIcons, START_MENU_ACTION_POKEDEX, -1u);
    }
    if (!(startMenu->inhibitIconFlags & (1 << START_MENU_ACTION_DISABLE_POKEMON))) {
        StartMenuButton_Insert(insertionOrderDest, displayOrderDest, &numIcons, START_MENU_ACTION_POKEMON, -1u);
    }
    if (!(startMenu->inhibitIconFlags & (1 << START_MENU_ACTION_DISABLE_BAG))) {
        StartMenuButton_Insert(insertionOrderDest, displayOrderDest, &numIcons, START_MENU_ACTION_BAG, -1u);
    }
    if (!(startMenu->inhibitIconFlags & (1 << START_MENU_ACTION_DISABLE_POKEGEAR))) {
        if (startMenu->inUnionRoom) {
            StartMenuButton_Insert(insertionOrderDest, displayOrderDest, &numIcons, START_MENU_ACTION_12, -1u);
        } else {
            StartMenuButton_Insert(insertionOrderDest, displayOrderDest, &numIcons, START_MENU_ACTION_POKEGEAR, -1u);
        }
    }
    if (!(startMenu->inhibitIconFlags & (1 << START_MENU_ACTION_DISABLE_TRAINER_CARD))) {
        StartMenuButton_Insert(insertionOrderDest, displayOrderDest, &numIcons, START_MENU_ACTION_TRAINER_CARD, -1u);
    }
    if (!(startMenu->inhibitIconFlags & (1 << START_MENU_ACTION_DISABLE_SAVE))) {
        StartMenuButton_Insert(insertionOrderDest, displayOrderDest, &numIcons, START_MENU_ACTION_SAVE, -1u);
    }
    if (!(startMenu->inhibitIconFlags & (1 << START_MENU_ACTION_DISABLE_OPTIONS))) {
        StartMenuButton_Insert(insertionOrderDest, displayOrderDest, &numIcons, START_MENU_ACTION_OPTIONS, -1u);
    }
    if (!(startMenu->inhibitIconFlags & (1 << START_MENU_ACTION_DISABLE_RUNNING_SHOES))) {
        StartMenuButton_Insert(insertionOrderDest, displayOrderDest, &numIcons, START_MENU_ACTION_RUNNING_SHOES, -1u);
    }
    StartMenuButton_Insert(insertionOrderDest, displayOrderDest, &numIcons, START_MENU_ACTION_9, 7);
    StartMenuButton_Insert(insertionOrderDest, displayOrderDest, &numIcons, START_MENU_ACTION_10, 8);
    return numIcons;
}

static void StartMenu_RemoveCursorAndIcons(StartMenuTaskData *startMenu, FieldSystem *fieldSystem) {
    StartMenu_DestroyCursor(startMenu);
    FillBgTilemapRect(fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 0, 0x20, 0x18, 0);
    startMenu->cursorActive = FALSE;
}

static BOOL FieldSystem_StartMenuActionIsAvailable(FieldSystem *fieldSystem, StartMenuAction action) {
    return FieldSystem_ShouldDrawStartMenuIcon(fieldSystem, (StartMenuIcon)sActionToIconIndex[action]);
}

BOOL FieldSystem_ShouldDrawStartMenuIcon(FieldSystem *fieldSystem, StartMenuIcon icon) {
    switch (icon) {
    case START_MENU_ICON_POKEDEX:
        return CheckGotPokedex(Save_VarsFlags_Get(fieldSystem->saveData));
    case START_MENU_ICON_POKEMON:
        return CheckGotStarter(Save_VarsFlags_Get(fieldSystem->saveData));
    case START_MENU_ICON_BAG:
        return CheckGotMenuIconI(Save_VarsFlags_Get(fieldSystem->saveData), START_MENU_ICON_UNLOCK_BAG);
    case START_MENU_ICON_POKEGEAR:
        return CheckGotPokegear(Save_VarsFlags_Get(fieldSystem->saveData));
    case START_MENU_ICON_TRAINER_CARD:
        return CheckGotMenuIconI(Save_VarsFlags_Get(fieldSystem->saveData), START_MENU_ICON_UNLOCK_TRAINER_CARD);
    case START_MENU_ICON_SAVE:
        return CheckGotMenuIconI(Save_VarsFlags_Get(fieldSystem->saveData), START_MENU_ICON_UNLOCK_SAVE_BUTTON);
    case START_MENU_ICON_OPTIONS:
        return CheckGotMenuIconI(Save_VarsFlags_Get(fieldSystem->saveData), START_MENU_ICON_UNLOCK_OPTIONS_BUTTON);
    case START_MENU_ICON_RUNNING_SHOES:
        return PlayerSaveData_CheckRunningShoes(LocalFieldData_GetPlayer(Save_LocalFieldData_Get(fieldSystem->saveData)));
    default:
        return TRUE;
    }
}

static void FieldSystem_ResetGearRingManagerIfNotSelectingSaveOrPokegear(FieldSystem *fieldSystem) {
    if (fieldSystem->lastStartMenuAction != START_MENU_ACTION_SAVE && fieldSystem->lastStartMenuAction != START_MENU_ACTION_POKEGEAR) {
        GearPhoneRingManager_ResetIfActive(fieldSystem->phoneRingManager);
    }
}

static BOOL Task_StartMenu_HandleInput(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    if (!FieldSystem_GetBottomScreenAppId(fieldSystem) && FieldSystem_GetBottomScreenAppState(fieldSystem) == TRUE) {
        if (gSystem.newKeys & PAD_BUTTON_A) {
            if (!StartMenu_HandleKeyInput(taskManager, fieldSystem, startMenu)) {
                return FALSE;
            }
            FieldSystem_ResetGearRingManagerIfNotSelectingSaveOrPokegear(fieldSystem);
        } else {
            if (fieldSystem->lastTouchMenuInput == 0) {
                if (gSystem.newKeys & (PAD_BUTTON_B | PAD_BUTTON_X)) {
                    PlaySE(SEQ_SE_GS_GEARCANCEL);
                    startMenu->state = START_MENU_STATE_CLOSE;
                }
            } else {
                if (!StartMenu_HandleTouchInput(taskManager, fieldSystem, startMenu)) {
                    return FALSE;
                }
                FieldSystem_ResetGearRingManagerIfNotSelectingSaveOrPokegear(fieldSystem);
            }
        }
    }
    return TRUE;
}

static BOOL StartMenu_HandleKeyInput(TaskManager *taskManager, FieldSystem *fieldSystem, StartMenuTaskData *startMenu) {
    if (fieldSystem->startMenuCursorPos < startMenu->numActiveButtons) {
        PlaySE(SEQ_SE_DP_SELECT);
        MenuInputStateMgr_SetState(&fieldSystem->menuInputState, MENU_INPUT_STATE_BUTTONS);
        startMenu->selectedIndex = fieldSystem->startMenuCursorPos;
        if (sStartMenuActions[startMenu->selectionToAction[startMenu->selectedIndex]].func == STARTMENUTASKFUNC_CANCEL) {
            startMenu->state = START_MENU_STATE_CLOSE;
        } else if (sStartMenuActions[startMenu->selectionToAction[startMenu->selectedIndex]].func != STARTMENUTASKFUNC_NONE) {
            if (FieldSystem_StartMenuActionIsAvailable(fieldSystem, (StartMenuAction)startMenu->selectionToAction[startMenu->selectedIndex])) {
                TaskFunc func = sStartMenuActions[startMenu->selectionToAction[startMenu->selectedIndex]].func;
                FieldSystem_SetStartMenuInputState(fieldSystem, 0);
                FieldSystem_RequestTouchscreenMenuSelectedIconAnim(fieldSystem);
                fieldSystem->lastTouchMenuInput = 0;
                fieldSystem->lastStartMenuAction = startMenu->selectionToAction[startMenu->selectedIndex];
                return func(taskManager);
            }
        }
    }

    return TRUE;
}

static BOOL StartMenu_HandleTouchInput(TaskManager *taskManager, FieldSystem *fieldSystem, StartMenuTaskData *startMenu) {
    if (fieldSystem->lastTouchMenuInput != 0) {
        MenuInputStateMgr_SetState(&fieldSystem->menuInputState, MENU_INPUT_STATE_TOUCH);
    }
    switch (fieldSystem->lastTouchMenuInput) {
    case 1:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        startMenu->state = START_MENU_STATE_CLOSE;
        fieldSystem->lastTouchMenuInput = 0;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        fieldSystem->startMenuCursorPos = fieldSystem->lastTouchMenuInput - 2;
        if (fieldSystem->startMenuCursorPos < startMenu->numActiveButtons) {
            PlaySE(SEQ_SE_DP_SELECT);
            startMenu->selectedIndex = fieldSystem->startMenuCursorPos;
            if (sStartMenuActions[startMenu->selectionToAction[startMenu->selectedIndex]].func == STARTMENUTASKFUNC_CANCEL) {
                startMenu->state = START_MENU_STATE_CLOSE;
                fieldSystem->lastTouchMenuInput = 0;
            } else if (sStartMenuActions[startMenu->selectionToAction[startMenu->selectedIndex]].func != STARTMENUTASKFUNC_NONE) {
                if (FieldSystem_StartMenuActionIsAvailable(fieldSystem, (StartMenuAction)startMenu->selectionToAction[startMenu->selectedIndex])) {
                    TaskFunc func = sStartMenuActions[startMenu->selectionToAction[startMenu->selectedIndex]].func;
                    fieldSystem->lastTouchMenuInput = 0;
                    FieldSystem_SetStartMenuInputState(fieldSystem, 1);
                    FieldSystem_RequestTouchscreenMenuSelectedIconAnim(fieldSystem);
                    fieldSystem->lastStartMenuAction = startMenu->selectionToAction[startMenu->selectedIndex];
                    return func(taskManager);
                } else {
                    fieldSystem->lastTouchMenuInput = 0;
                }
            }
        }
        break;
    }
    return TRUE;
}

static void StartMenu_ResumeUnionRoomSearch(StartMenuTaskData *startMenu, FieldSystem *fieldSystem) {
    if (CommWireless_Exists() && startMenu->inUnionRoom) {
        UnionRoomObjects_RequestLeaveAll(fieldSystem->unionRoomObjects);
        CommState_ResumeAlternateSearch();
        UnionRoom_SetBeaconActivity(0);
    }
}

static void StartMenu_CreateCursor(StartMenuTaskData *startMenu, u8 *insertionOrder, u32 numButtons, u8 gender) {
    startMenu->spriteList = G2dRenderer_Init(1, &startMenu->g2dRenderer, HEAP_ID_FIELD2);
    for (int i = 0; i < 4; ++i) {
        startMenu->gfxResMan[i] = Create2DGfxResObjMan(1, (GfGfxResType)i, HEAP_ID_FIELD2);
    }
    startMenu->gfxResObj[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(startMenu->gfxResMan[GF_GFX_RES_TYPE_CHAR], NARC_a_0_1_4, 64, TRUE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_FIELD2);
    startMenu->gfxResObj[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(startMenu->gfxResMan[GF_GFX_RES_TYPE_PLTT], NARC_a_0_1_4, 61, FALSE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, HEAP_ID_FIELD2);
    startMenu->gfxResObj[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(startMenu->gfxResMan[GF_GFX_RES_TYPE_CELL], NARC_a_0_1_4, 62, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD2);
    startMenu->gfxResObj[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(startMenu->gfxResMan[GF_GFX_RES_TYPE_ANIM], NARC_a_0_1_4, 63, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD2);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(startMenu->gfxResObj[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreatePlttTransferTask(startMenu->gfxResObj[GF_GFX_RES_TYPE_PLTT]);
    GF2DGfxResObj_FreeData(startMenu->gfxResObj[GF_GFX_RES_TYPE_PLTT]);
    CreateSpriteResourcesHeader(&startMenu->spriteResourcesHeader, 0, 0, 0, 0, -1, -1, 0, 0, startMenu->gfxResMan[GF_GFX_RES_TYPE_CHAR], startMenu->gfxResMan[GF_GFX_RES_TYPE_PLTT], startMenu->gfxResMan[GF_GFX_RES_TYPE_CELL], startMenu->gfxResMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);

    SpriteTemplate spriteTemplate;
    spriteTemplate.spriteList = startMenu->spriteList;
    spriteTemplate.header = &startMenu->spriteResourcesHeader;
    spriteTemplate.position.z = 0;
    spriteTemplate.scale.x = FX32_ONE;
    spriteTemplate.scale.y = FX32_ONE;
    spriteTemplate.scale.z = FX32_ONE;
    spriteTemplate.rotation = 0;
    spriteTemplate.drawPriority = 0;
    spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteTemplate.heapID = HEAP_ID_FIELD2;
    spriteTemplate.header = &startMenu->spriteResourcesHeader; // second assign is necessary to match
    spriteTemplate.position.x = FX32_CONST(100);
    spriteTemplate.position.y = FX32_CONST(144);
    startMenu->cursorSprite = Sprite_CreateAffine(&spriteTemplate);
    Sprite_SetAnimActiveFlag(startMenu->cursorSprite, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void StartMenu_DestroyCursor(StartMenuTaskData *startMenu) {
    SpriteTransfer_DeleteCharTransferTask(startMenu->gfxResObj[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(startMenu->gfxResObj[GF_GFX_RES_TYPE_PLTT]);
    for (u16 i = 0; i < 4; ++i) {
        Destroy2DGfxResObjMan(startMenu->gfxResMan[i]);
    }
    SpriteList_Delete(startMenu->spriteList);
}

static void Task_StartMenu_WaitFade(TaskManager *taskManager) {
    if (IsPaletteFadeFinished()) {
        FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
        StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

        StartMenu_RemoveCursorAndIcons(startMenu, fieldSystem);
        fieldSystem->startMenuState = 0;
        startMenu->exitTaskFunc(taskManager);
        startMenu->state = START_MENU_STATE_WAIT_APP;
    }
}

static void Task_StartMenu_WaitApp(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
        startMenu->exitTaskFunc(taskManager);
        if (startMenu->state == START_MENU_STATE_RETURN && MenuInputStateMgr_GetState(&fieldSystem->menuInputState) == TRUE) {
            startMenu->state = START_MENU_STATE_10;
        }
    }
}

void StartMenu_SetExitTaskFunc(StartMenuTaskData *startMenu, TaskFunc func) {
    startMenu->exitTaskFunc = func;
    startMenu->state = START_MENU_STATE_WAIT_APP;
}

static BOOL Task_StartMenu_HandleSelection_Pokedex(TaskManager *taskManager) {
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
    startMenu->exitTaskFunc = Task_StartMenu_OpenPokedex;
    startMenu->state = START_MENU_STATE_WAIT_FADE;
    return TRUE;
}

static BOOL Task_StartMenu_OpenPokedex(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    LocalFieldData *localFieldData = Save_LocalFieldData_Get(fieldSystem->saveData);
    Location *position = LocalFieldData_GetCurrentPosition(localFieldData);
    Location *specialSpawnWarp = LocalFieldData_GetSpecialSpawnWarpPtr(localFieldData);
    PokedexArgs *pokedexArgs = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PokedexArgs));
    MI_CpuClear8(pokedexArgs, sizeof(PokedexArgs));
    pokedexArgs->pokedex = Save_Pokedex_Get(fieldSystem->saveData);
    pokedexArgs->playerProfile = Save_PlayerData_GetProfile(fieldSystem->saveData);
    pokedexArgs->unk_08 = fieldSystem->unkA8;
    pokedexArgs->menuInputStatePtr = &fieldSystem->menuInputState;
    int x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
    int z = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
    pokedexArgs->mapMatrixId = MapMatrix_GetMapHeader(fieldSystem->mapMatrix, x / 32, z / 32);
    pokedexArgs->mapId = position->mapId;
    if (MapHeader_MapIsOnMainMatrix(pokedexArgs->mapMatrixId)) {
        pokedexArgs->x = x;
        pokedexArgs->y = z;
    } else {
        s16 wmX;
        s16 wmY;
        MapHeader_GetWorldMapCoords(pokedexArgs->mapMatrixId, &wmX, &wmY);
        if (wmX == 0 && wmY == 0) {
            pokedexArgs->x = specialSpawnWarp->x;
            pokedexArgs->y = specialSpawnWarp->y;
        } else {
            pokedexArgs->x = wmX * 32;
            pokedexArgs->y = wmY * 32;
        }
    }
    Pokedex_LaunchApp(fieldSystem, pokedexArgs);
    startMenu->exitTaskEnvironment = pokedexArgs;
    startMenu->exitTaskFunc = Task_StartMenu_HandleReturn_Pokedex;
    return FALSE;
}

static BOOL Task_StartMenu_HandleReturn_Pokedex(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    FieldSystem_LoadFieldOverlay(fieldSystem);
    if (startMenu->exitTaskEnvironment != NULL) {
        Heap_FreeExplicit(HEAP_ID_FIELD2, startMenu->exitTaskEnvironment);
    }
    startMenu->state = START_MENU_STATE_RETURN;
    return FALSE;
}

static BOOL Task_StartMenu_HandleSelection_Pokemon(TaskManager *taskManager) {
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
    startMenu->exitTaskFunc = Task_StartMenu_Pokemon;
    startMenu->state = START_MENU_STATE_WAIT_FADE;
    return TRUE;
}

static BOOL Task_StartMenu_Pokemon(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    startMenu->exitTaskEnvironment = PartyMenu_LaunchApp_Field(fieldSystem, &startMenu->fieldMoveCheckData, 0);
    startMenu->exitTaskFunc = Task_StartMenu_HandleReturn_Pokemon;
    return FALSE;
}

BOOL Task_StartMenu_HandleReturn_Pokemon(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    PartyMenuArgs *partyMenuArgs = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PartyMenuArgs));
    memcpy(partyMenuArgs, startMenu->exitTaskEnvironment, sizeof(PartyMenuArgs));
    Heap_Free(startMenu->exitTaskEnvironment);

    switch (partyMenuArgs->selectedAction) {
    case PARTY_MENU_ACTION_RETURN_1: {
        PokemonSummaryArgs *pokemonSummaryArgs = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PokemonSummaryArgs));
        pokemonSummaryArgs->party = SaveArray_Party_Get(fieldSystem->saveData);
        pokemonSummaryArgs->options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
        pokemonSummaryArgs->monDataType = 1;
        pokemonSummaryArgs->partySlot = partyMenuArgs->partySlot;
        pokemonSummaryArgs->partyCount = Party_GetCount(pokemonSummaryArgs->party);
        pokemonSummaryArgs->moveToLearn = 0;
        pokemonSummaryArgs->mode = 0;
        pokemonSummaryArgs->ribbons = Save_SpecialRibbons_Get(fieldSystem->saveData);
        pokemonSummaryArgs->natDexEnabled = SaveArray_IsNatDexEnabled(fieldSystem->saveData);
        pokemonSummaryArgs->unk2C = PokemonSummary_GetDefaultUnk2C(fieldSystem->saveData);
        pokemonSummaryArgs->chatot = 0;
        pokemonSummaryArgs->menuInputStatePtr = &fieldSystem->menuInputState;
        pokemonSummaryArgs->isFlag982Set = PokemonSummary_IsPokeathlonUnlocked(fieldSystem->saveData);
        PokemonSummary_SetPages(pokemonSummaryArgs, _020FA0B0);
        PokemonSummaryArgs_SetPlayer(pokemonSummaryArgs, Save_PlayerData_GetProfile(fieldSystem->saveData));
        PokemonSummary_LearnForget_LaunchApp(fieldSystem, pokemonSummaryArgs);
        startMenu->exitTaskEnvironment = pokemonSummaryArgs;
        StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn_Summary);
        break;
    }
    case PARTY_MENU_ACTION_RETURN_4: {
        PokemonSummaryArgs *pokemonSummaryArgs = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PokemonSummaryArgs));
        pokemonSummaryArgs->party = SaveArray_Party_Get(fieldSystem->saveData);
        pokemonSummaryArgs->options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
        pokemonSummaryArgs->monDataType = 1;
        pokemonSummaryArgs->partySlot = partyMenuArgs->partySlot;
        pokemonSummaryArgs->partyCount = 1;
        pokemonSummaryArgs->moveToLearn = partyMenuArgs->moveId;
        pokemonSummaryArgs->mode = 2;
        pokemonSummaryArgs->natDexEnabled = SaveArray_IsNatDexEnabled(fieldSystem->saveData);
        pokemonSummaryArgs->unk2C = PokemonSummary_GetDefaultUnk2C(fieldSystem->saveData);
        pokemonSummaryArgs->chatot = 0;
        pokemonSummaryArgs->menuInputStatePtr = &fieldSystem->menuInputState;
        pokemonSummaryArgs->isFlag982Set = PokemonSummary_IsPokeathlonUnlocked(fieldSystem->saveData);
        PokemonSummary_SetPages(pokemonSummaryArgs, _020FA0AC);
        PokemonSummaryArgs_SetPlayer(pokemonSummaryArgs, Save_PlayerData_GetProfile(fieldSystem->saveData));
        PokemonSummary_LearnForget_LaunchApp(fieldSystem, pokemonSummaryArgs);
        StartMenuMoveLearnReturn *unk = Heap_Alloc(HEAP_ID_FIELD2, sizeof(StartMenuMoveLearnReturn));
        unk->itemId = partyMenuArgs->itemId;
        unk->levelUpMoveSearchState = 0;
        startMenu->exitTaskEnvironment2 = unk;
        startMenu->exitTaskEnvironment = pokemonSummaryArgs;
        StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn_Summary);
        break;
    }
    case PARTY_MENU_ACTION_RETURN_5: {
        PokemonSummaryArgs *pokemonSummaryArgs = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PokemonSummaryArgs));
        pokemonSummaryArgs->party = SaveArray_Party_Get(fieldSystem->saveData);
        pokemonSummaryArgs->options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
        pokemonSummaryArgs->monDataType = 1;
        pokemonSummaryArgs->partySlot = partyMenuArgs->partySlot;
        pokemonSummaryArgs->partyCount = 1;
        pokemonSummaryArgs->moveToLearn = partyMenuArgs->moveId;
        pokemonSummaryArgs->mode = 2;
        pokemonSummaryArgs->natDexEnabled = SaveArray_IsNatDexEnabled(fieldSystem->saveData);
        pokemonSummaryArgs->unk2C = PokemonSummary_GetDefaultUnk2C(fieldSystem->saveData);
        pokemonSummaryArgs->chatot = 0;
        pokemonSummaryArgs->menuInputStatePtr = &fieldSystem->menuInputState;
        pokemonSummaryArgs->isFlag982Set = PokemonSummary_IsPokeathlonUnlocked(fieldSystem->saveData);
        PokemonSummary_SetPages(pokemonSummaryArgs, _020FA0AC);
        PokemonSummaryArgs_SetPlayer(pokemonSummaryArgs, Save_PlayerData_GetProfile(fieldSystem->saveData));
        PokemonSummary_LearnForget_LaunchApp(fieldSystem, pokemonSummaryArgs);
        StartMenuMoveLearnReturn *unk = Heap_Alloc(HEAP_ID_FIELD2, sizeof(StartMenuMoveLearnReturn));
        unk->itemId = ITEM_NONE;
        unk->levelUpMoveSearchState = partyMenuArgs->levelUpMoveSearchState;
        startMenu->exitTaskEnvironment2 = unk;
        startMenu->exitTaskEnvironment = pokemonSummaryArgs;
        StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn_Summary);
        break;
    }
    case PARTY_MENU_ACTION_RETURN_6:
        startMenu->exitTaskEnvironment = UseMail_LaunchApp_Compose(fieldSystem, 2, partyMenuArgs->partySlot, ItemToMailId(partyMenuArgs->itemId), HEAP_ID_FIELD2);
        if (partyMenuArgs->context == PARTY_MENU_CONTEXT_10) {
            startMenu->exitTaskEnvironment2 = StartMenu_CreateMailReturnInfo(partyMenuArgs->itemId, 0, partyMenuArgs->partySlot);
        } else {
            startMenu->exitTaskEnvironment2 = StartMenu_CreateMailReturnInfo(partyMenuArgs->itemId, 1, partyMenuArgs->partySlot);
        }
        StartMenu_SetExitTaskFunc(startMenu, Task_ReturnToMenuFromMail);
        break;
    case PARTY_MENU_ACTION_RETURN_READ_MAIL:
        startMenu->exitTaskEnvironment = UseMail_LaunchApp_ReadMonMail(fieldSystem, Party_GetMonByIndex(SaveArray_Party_Get(fieldSystem->saveData), partyMenuArgs->partySlot), HEAP_ID_FIELD2);
        startMenu->exitTaskEnvironment2 = StartMenu_CreateMailReturnInfo(partyMenuArgs->itemId, 2, partyMenuArgs->partySlot);
        StartMenu_SetExitTaskFunc(startMenu, Task_ReturnToMenuFromMail);
        break;
    case 3: {
        StartMenuAfterEvoPartySlotBak *afterEvoPartySlot = Heap_Alloc(HEAP_ID_FIELD2, sizeof(StartMenuAfterEvoPartySlotBak));
        afterEvoPartySlot->partySlot = partyMenuArgs->partySlot;
        startMenu->exitTaskEnvironment2 = afterEvoPartySlot;
        Bag *bag = Save_Bag_Get(fieldSystem->saveData);
        PlayerProfile *playerProfile = Save_PlayerData_GetProfile(fieldSystem->saveData);
        (void)playerProfile;
        startMenu->exitTaskEnvironment = Bag_CreateView(bag, sPockets, HEAP_ID_FIELD2);
        BagView_Init(startMenu->exitTaskEnvironment, fieldSystem->saveData, 1, fieldSystem->bagCursor, &fieldSystem->menuInputState);
        Bag_LaunchApp(fieldSystem, startMenu->exitTaskEnvironment);
        StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn);
        break;
    }
    case PARTY_MENU_ACTION_RETURN_EVO_ITEM_USE: {
        StartMenuEvolutionArgs *unk = Heap_Alloc(HEAP_ID_FIELD2, sizeof(StartMenuEvolutionArgs));
        unk->evoParam = partyMenuArgs->itemId;
        unk->evoType = EVOCTX_ITEM_USE;
        unk->partySlot = partyMenuArgs->partySlot;
        unk->species = partyMenuArgs->species;
        unk->evoMethod = partyMenuArgs->evoMethod;
        startMenu->exitTaskEnvironment = unk;
        startMenu->state = START_MENU_STATE_EVOLUTION;
        break;
    }
    case PARTY_MENU_ACTION_RETURN_EVO_RARE_CANDY: {
        StartMenuEvolutionArgs *unk = Heap_Alloc(HEAP_ID_FIELD2, sizeof(StartMenuEvolutionArgs));
        unk->evoParam = MapHeader_GetMapEvolutionMethod(fieldSystem->location->mapId);
        unk->evoType = EVOCTX_LEVELUP;
        unk->partySlot = partyMenuArgs->partySlot;
        unk->species = partyMenuArgs->species;
        unk->evoMethod = partyMenuArgs->evoMethod;
        startMenu->exitTaskEnvironment = unk;
        startMenu->state = START_MENU_STATE_EVOLUTION;
        break;
    }
    case PARTY_MENU_ACTION_RETURN_CUT:
    case PARTY_MENU_ACTION_RETURN_FLY:
    case PARTY_MENU_ACTION_RETURN_SURF:
    case PARTY_MENU_ACTION_RETURN_STRENGTH:
    case PARTY_MENU_ACTION_RETURN_ROCK_SMASH:
    case PARTY_MENU_ACTION_RETURN_WATERFALL:
    case PARTY_MENU_ACTION_RETURN_ROCK_CLIMB:
    case PARTY_MENU_ACTION_RETURN_WHIRLPOOL:
    case PARTY_MENU_ACTION_RETURN_FLASH:
    case PARTY_MENU_ACTION_RETURN_TELEPORT:
    case PARTY_MENU_ACTION_RETURN_DIG:
    case PARTY_MENU_ACTION_RETURN_SWEET_SCENT:
    case PARTY_MENU_ACTION_RETURN_CHATTER:
    case PARTY_MENU_ACTION_RETURN_HEADBUTT: {
        FieldMoveUseData fieldMoveUseData;

        fieldMoveUseData.fieldMoveIdx = partyMenuArgs->selectedAction - PARTY_MENU_ACTION_RETURN_FIELD_MOVE_BEGIN;
        fieldMoveUseData.partySlot = partyMenuArgs->partySlot;
        fieldMoveUseData.taskManager = taskManager;
        ((FieldMoveUseFunc)FieldMove_GetMoveFunc(FIELD_MOVE_FUNC_USE, fieldMoveUseData.fieldMoveIdx))(&fieldMoveUseData, &startMenu->fieldMoveCheckData);
        break;
    }
    case PARTY_MENU_ACTION_RETURN_10:
        startMenu->exitTaskEnvironment = Bag_LaunchApp_AllPockets(fieldSystem, &startMenu->itemCheckUseData);
        StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn);
        break;
    default:
        if (partyMenuArgs->context == PARTY_MENU_CONTEXT_USE_ITEM || partyMenuArgs->context == PARTY_MENU_CONTEXT_TM_HM || partyMenuArgs->context == PARTY_MENU_CONTEXT_REPLACE_MOVE_TMHM || partyMenuArgs->context == PARTY_MENU_CONTEXT_EVO_STONE || partyMenuArgs->context == PARTY_MENU_CONTEXT_REPLACE_MOVE_LEVELUP) {
            startMenu->exitTaskEnvironment = Bag_LaunchApp_AllPockets(fieldSystem, &startMenu->itemCheckUseData);
            if (partyMenuArgs->partySlot >= 6) {
                BagView_SetPartySlot(startMenu->exitTaskEnvironment, 0);
            } else {
                BagView_SetPartySlot(startMenu->exitTaskEnvironment, partyMenuArgs->partySlot);
            }
            StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn);
        } else if (partyMenuArgs->context == PARTY_MENU_CONTEXT_9) {
            startMenu->exitTaskEnvironment = Bag_LaunchApp_AllPockets(fieldSystem, &startMenu->itemCheckUseData);
            StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn);
        } else {
            FieldSystem_LoadFieldOverlay(fieldSystem);
            startMenu->state = START_MENU_STATE_RETURN;
        }
        break;
    }
    Heap_Free(partyMenuArgs);
    return FALSE;
}

static BOOL Task_StartMenu_HandleSelection_Bag(TaskManager *taskManager) {
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
    startMenu->exitTaskFunc = Task_StartMenu_Bag;
    startMenu->state = START_MENU_STATE_WAIT_FADE;
    return TRUE;
}

static BOOL Task_StartMenu_Bag(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    startMenu->exitTaskEnvironment = Bag_LaunchApp_AllPockets(fieldSystem, &startMenu->itemCheckUseData);
    BagView_SetPartySlot(startMenu->exitTaskEnvironment, 0);
    startMenu->exitTaskFunc = Task_StartMenu_HandleReturn;
    FieldSystem_TryStorePlayerLocation(fieldSystem);
    return FALSE;
}

static void StartMenu_InitPartyMenuArgs(PartyMenuArgs *partyMenuArgs, FieldSystem *fieldSystem, StartMenuTaskData *startMenu) {
    memset(partyMenuArgs, 0, sizeof(PartyMenuArgs));
    partyMenuArgs->party = SaveArray_Party_Get(fieldSystem->saveData);
    partyMenuArgs->bag = Save_Bag_Get(fieldSystem->saveData);
    partyMenuArgs->mailbox = Save_Mailbox_Get(fieldSystem->saveData);
    partyMenuArgs->options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
    partyMenuArgs->fieldMoveCheckData = &startMenu->fieldMoveCheckData;
    partyMenuArgs->panelLayout = 0;
    partyMenuArgs->fieldSystem = fieldSystem;
    partyMenuArgs->menuInputStatePtr = &fieldSystem->menuInputState;
}

static BOOL Task_StartMenu_HandleReturn(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    BagView *bagView = BagView_New(HEAP_ID_FIELD2);
    memcpy(bagView, startMenu->exitTaskEnvironment, BagView_sizeof());
    Heap_Free(startMenu->exitTaskEnvironment);

    switch (BagView_GetSelectedAction(bagView)) {
    case 0: {
        ItemMenuUseData itemMenuUseData;
        itemMenuUseData.itemId = BagView_GetItemId(bagView);
        itemMenuUseData.partySlot = BagView_GetPartySlot(bagView);
        itemMenuUseData.taskManager = taskManager;
        ItemMenuUseFunc func = GetItemFieldUseFunc(USE_ITEM_TASK_MENU, GetItemAttr(itemMenuUseData.itemId, ITEMATTR_FIELDUSEFUNC, HEAP_ID_FIELD2));
        func(&itemMenuUseData, &startMenu->itemCheckUseData);
        break;
    }
    case 1:
        StartMenu_HandleBagAction1(taskManager, BagView_GetItemId(bagView));
        break;
    case 2: {
        PartyMenuArgs *partyMenuArgs = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PartyMenuArgs));
        memset(partyMenuArgs, 0, sizeof(PartyMenuArgs));
        StartMenu_InitPartyMenuArgs(partyMenuArgs, fieldSystem, startMenu);
        partyMenuArgs->context = PARTY_MENU_CONTEXT_9;
        partyMenuArgs->itemId = BagView_GetItemId(bagView);
        FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PartyMenu, partyMenuArgs);
        startMenu->exitTaskEnvironment = partyMenuArgs;
        StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn_Pokemon);
        break;
    }
    case 4: {
        Party *party = SaveArray_Party_Get(fieldSystem->saveData);
        StartMenuAfterEvoPartySlotBak *unk = startMenu->exitTaskEnvironment2;
        int monSlot = unk->partySlot;
        u16 itemId = BagView_GetItemId(bagView);
        Pokemon *pokemon = Party_GetMonByIndex(party, monSlot);
        Heap_Free(startMenu->exitTaskEnvironment2);
        if (ItemIdIsMail(itemId) == TRUE && !GetMonData(pokemon, MON_DATA_HELD_ITEM, NULL)) {
            startMenu->exitTaskEnvironment = UseMail_LaunchApp_Compose(fieldSystem, 2, monSlot, ItemToMailId(itemId), HEAP_ID_FIELD2);
            startMenu->exitTaskEnvironment2 = StartMenu_CreateMailReturnInfo(itemId, 0, monSlot);
            StartMenu_SetExitTaskFunc(startMenu, Task_ReturnToMenuFromMail);
        } else {
            PartyMenuArgs *partyMenuArgs = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PartyMenuArgs));
            memset(partyMenuArgs, 0, sizeof(PartyMenuArgs));
            StartMenu_InitPartyMenuArgs(partyMenuArgs, fieldSystem, startMenu);
            partyMenuArgs->party = party;
            partyMenuArgs->itemId = BagView_GetItemId(bagView);
            partyMenuArgs->partySlot = monSlot;
            if (partyMenuArgs->itemId == ITEM_NONE) {
                partyMenuArgs->context = PARTY_MENU_CONTEXT_0;
            } else {
                partyMenuArgs->context = PARTY_MENU_CONTEXT_10;
            }
            FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PartyMenu, partyMenuArgs);
            startMenu->exitTaskEnvironment = partyMenuArgs;
            StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn_Pokemon);
        }
        break;
    }
    case 3:
    case 5:
    default:
        FieldSystem_LoadFieldOverlay(fieldSystem);
        startMenu->state = START_MENU_STATE_RETURN;
        break;
    }
    Heap_Free(bagView);
    return FALSE;
}

static BOOL Task_StartMenu_HandleSelection_TrainerCard(TaskManager *taskManager) {
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
    startMenu->exitTaskFunc = Task_StartMenu_TrainerCard;
    startMenu->state = START_MENU_STATE_WAIT_FADE;
    return TRUE;
}

static BOOL Task_StartMenu_TrainerCard(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    startMenu->exitTaskEnvironment = TrainerCardAppArgs_New(HEAP_ID_FIELD2);
    TrainerCardAppArgs *trainerCard = startMenu->exitTaskEnvironment;
    TrainerCardData_Build(1, 1, 0, 255, fieldSystem, trainerCard);
    TrainerCard_LaunchApp(fieldSystem, trainerCard);
    startMenu->exitTaskFunc = Task_StartMenu_HandleReturn_TrainerCard;
    return FALSE;
}

static BOOL Task_StartMenu_HandleReturn_TrainerCard(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    TrainerCardData_Free(startMenu->exitTaskEnvironment);
    FieldSystem_LoadFieldOverlay(fieldSystem);
    startMenu->state = START_MENU_STATE_RETURN;
    return FALSE;
}

static BOOL Task_StartMenu_HandleSelection_Save(TaskManager *taskManager) {
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);

    FieldSystem_SwitchBottomScreenApp(fieldSystem, 1, NULL);
    startMenu->state = START_MENU_STATE_SAVE;
    return TRUE;
}

static void Task_StartMenu_WaitSaveAppOpen(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    if (FieldSystem_GetBottomScreenAppId(fieldSystem) == TRUE) {
        startMenu->state = START_MENU_STATE_7;
    }
}

static void Task_StartMenu_WaitSaveAppClose(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    if (!FieldSystem_GetBottomScreenAppId(fieldSystem) && FieldSystem_GetBottomScreenAppState(fieldSystem) == TRUE) {
        if (fieldSystem->lastTouchMenuInput == 1) {
            fieldSystem->lastTouchMenuInput = 0;
            startMenu->state = START_MENU_STATE_CLOSE;
        } else {
            startMenu->state = START_MENU_STATE_HANDLE_INPUT;
        }
    }
}

static BOOL StartMenu_SelectUnionChatLog(TaskManager *taskManager) {
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);

    FieldSystem_SwitchBottomScreenApp(fieldSystem, 8, NULL);
    startMenu->state = START_MENU_STATE_19;
    return TRUE;
}

static void StartMenu_State19_Advance(TaskManager *taskManager) {
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);

    startMenu->state = START_MENU_STATE_20;
}

static void StartMenu_State20_Close(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    startMenu->state = START_MENU_STATE_CLOSE;
}

static BOOL Task_StartMenu_HandleSelection_Options(TaskManager *taskManager) {
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
    startMenu->exitTaskFunc = Task_StartMenu_Options;
    startMenu->state = START_MENU_STATE_WAIT_FADE;
    return TRUE;
}

static BOOL Task_StartMenu_Options(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    startMenu->exitTaskEnvironment = OptionsMenu_LaunchApp(fieldSystem);
    startMenu->exitTaskFunc = Task_StartMenu_HandleReturn_Options;
    return FALSE;
}

static BOOL Task_StartMenu_HandleReturn_Options(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    Heap_Free(startMenu->exitTaskEnvironment);
    FieldSystem_LoadFieldOverlay(fieldSystem);
    startMenu->state = START_MENU_STATE_RETURN;
    return FALSE;
}

static BOOL Task_StartMenu_HandleSelection_RemovedEasyChatThing(TaskManager *taskManager) {
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
    startMenu->exitTaskFunc = Task_StartMenu_RemovedEasyChatThing;
    startMenu->state = START_MENU_STATE_WAIT_FADE;
    return TRUE;
}

static BOOL Task_StartMenu_RemovedEasyChatThing(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    startMenu->exitTaskEnvironment = EasyChat_CreateArgs(2, 0, fieldSystem->saveData, &fieldSystem->menuInputState, HEAP_ID_FIELD2);
    MailMessage mailMessage;
    MailMsg_Init_WithBank(&mailMessage, MAILMSG_BANK_0295_GMM);
    EasyChatArgs_SetMailMessage(startMenu->exitTaskEnvironment, &mailMessage);
    EasyChat_LaunchApp(fieldSystem, startMenu->exitTaskEnvironment);
    startMenu->exitTaskFunc = Task_StartMenu_HandleReturn_RemovedEasyChatThing;
    return FALSE;
}

static BOOL Task_StartMenu_HandleReturn_RemovedEasyChatThing(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    if (!EasyChatArgs_IsCancelled(startMenu->exitTaskEnvironment)) {
        MailMessage mailMessage;
        EasyChatArgs_GetMailMessage(startMenu->exitTaskEnvironment, &mailMessage);
        if (CommWireless_Exists()) {
            UnionRoom_BroadcastChatMessage(&mailMessage);
            UnionRoom_SetOwnChatMessage(fieldSystem->unionRoom, &mailMessage);
        }
        startMenu->state = START_MENU_STATE_10;
    } else {
        startMenu->state = START_MENU_STATE_RETURN;
    }
    EasyChat_FreeArgs(startMenu->exitTaskEnvironment);
    FieldSystem_LoadFieldOverlay(fieldSystem);
    UnionRoomObjects_RequestLeaveAll(fieldSystem->unionRoomObjects);
    return FALSE;
}

static BOOL Task_StartMenu_HandleSelection_Retire(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    StartMenu_RemoveCursorAndIcons(startMenu, fieldSystem);
    fieldSystem->startMenuState = 0;
    if (Save_VarsFlags_CheckSafariSysFlag(Save_VarsFlags_Get(fieldSystem->saveData)) == TRUE) {
        StartScriptFromMenu(taskManager, std_safari_exit, NULL);
    } else if (Save_VarsFlags_CheckBugContestFlag(Save_VarsFlags_Get(fieldSystem->saveData)) == TRUE) {
        StartScriptFromMenu(taskManager, std_bug_contest_retire, NULL);
    } else {
        StartScriptFromMenu(taskManager, 4, NULL);
    }
    Heap_Free(startMenu);
    return FALSE;
}

static BOOL Task_StartMenu_HandleSelection_Pokegear(TaskManager *taskManager) {
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    FieldMap_FadeScreen(FADE_TYPE_BRIGHTNESS_OUT);
    startMenu->exitTaskFunc = Task_StartMenu_Pokegear;
    startMenu->state = START_MENU_STATE_WAIT_FADE;
    return TRUE;
}

static BOOL Task_StartMenu_Pokegear(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    startMenu->exitTaskEnvironment = PokegearPhone_LaunchApp(fieldSystem);
    startMenu->exitTaskFunc = Task_StartMenu_HandleReturn_Pokegear;
    return FALSE;
}

static BOOL Task_StartMenu_HandleReturn_Pokegear(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    FieldSystem_LoadFieldOverlay(fieldSystem);
    if (startMenu->exitTaskEnvironment != NULL) {
        Heap_FreeExplicit(HEAP_ID_FIELD2, startMenu->exitTaskEnvironment);
    }
    startMenu->state = START_MENU_STATE_RETURN;
    return FALSE;
}

static BOOL Task_StartMenu_HandleReturn_Summary(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    PokemonSummaryArgs *summaryArgs = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PokemonSummaryArgs));
    memcpy(summaryArgs, startMenu->exitTaskEnvironment, sizeof(PokemonSummaryArgs));
    Heap_Free(startMenu->exitTaskEnvironment);
    if (summaryArgs->mode == 2) {
        PartyMenuArgs *partyMenuArgs = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PartyMenuArgs));
        StartMenuMoveLearnReturn *r7 = startMenu->exitTaskEnvironment2;
        StartMenu_InitPartyMenuArgs(partyMenuArgs, fieldSystem, startMenu);
        partyMenuArgs->itemId = r7->itemId;
        partyMenuArgs->partySlot = summaryArgs->partySlot;
        partyMenuArgs->moveId = summaryArgs->moveToLearn;
        partyMenuArgs->selectedMoveIdx = summaryArgs->selectedMoveSlot;
        if (r7->itemId != ITEM_NONE) {
            partyMenuArgs->context = PARTY_MENU_CONTEXT_REPLACE_MOVE_TMHM;
            partyMenuArgs->levelUpMoveSearchState = 0;
        } else {
            partyMenuArgs->context = PARTY_MENU_CONTEXT_REPLACE_MOVE_LEVELUP;
            partyMenuArgs->levelUpMoveSearchState = r7->levelUpMoveSearchState;
        }
        partyMenuArgs->menuInputStatePtr = &fieldSystem->menuInputState;
        FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PartyMenu, partyMenuArgs);
        Heap_Free(startMenu->exitTaskEnvironment2);
        startMenu->exitTaskEnvironment = partyMenuArgs;
        StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn_Pokemon);
    } else {
        startMenu->exitTaskEnvironment = PartyMenu_LaunchApp_Field(fieldSystem, &startMenu->fieldMoveCheckData, summaryArgs->partySlot);
        StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn_Pokemon);
    }
    Heap_Free(summaryArgs);
    return FALSE;
}

static void StartMenu_HandleBagAction1(TaskManager *taskManager, int itemId) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    Bag *bag = Save_Bag_Get(fieldSystem->saveData);
    for (u8 i = 0; i < NUM_BERRIES; ++i) {
        if (Bag_HasItem(bag, BerryToItemId(i), 1, HEAP_ID_FIELD2)) { }
    }
    u8 scroll, position;
    BagCursor_Field_PocketGetPosition(fieldSystem->bagCursor, POCKET_BERRIES, &position, &scroll);
    StartMenu_SetExitTaskFunc(startMenu, StartMenu_ReopenBagAtBerries);
}

static BOOL StartMenu_ReopenBagAtBerries(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    BagCursor_Field_PocketSetPosition(fieldSystem->bagCursor, POCKET_BERRIES, 0, 0);
    Heap_FreeExplicit(HEAP_ID_FIELD2, startMenu->exitTaskEnvironment);
    startMenu->exitTaskEnvironment = Bag_LaunchApp_AllPockets(fieldSystem, &startMenu->itemCheckUseData);
    StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn);
    return FALSE;
}

BOOL Task_ReturnToMenuFromAppItem(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    Heap_FreeExplicit(HEAP_ID_FIELD2, startMenu->exitTaskEnvironment);
    startMenu->exitTaskEnvironment = Bag_LaunchApp_AllPockets(fieldSystem, &startMenu->itemCheckUseData);
    StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn);
    return FALSE;
}

BOOL Task_UseFlyInField(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    FieldMoveData *flyMap = startMenu->exitTaskEnvironment2;
    int partySlot = flyMap->partySlot;
    Heap_FreeExplicit(HEAP_ID_FIELD2, flyMap);
    PokegearArgs *pokegearArgs = startMenu->exitTaskEnvironment;
    if (!pokegearArgs->setFlyDestination) {
        Heap_FreeExplicit(HEAP_ID_FIELD2, pokegearArgs);
        startMenu->exitTaskEnvironment = PartyMenu_LaunchApp_Field(fieldSystem, &startMenu->fieldMoveCheckData, partySlot);
        StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn_Pokemon);
    } else {
        Pokemon *pokemon = Party_GetMonByIndex(SaveArray_Party_Get(fieldSystem->saveData), partySlot);
        struct FlyTaskEnv *r5 = FlyTaskEnv_New(HEAP_ID_FIELD2, fieldSystem, pokemon, partySlot, pokegearArgs->selectedFlyDest, pokegearArgs->mapCursorX * 32 + 16, pokegearArgs->mapCursorY * 32 + 16);
        Heap_FreeExplicit(HEAP_ID_FIELD2, startMenu->exitTaskEnvironment);
        FieldSystem_LoadFieldOverlay(fieldSystem);
        startMenu->exitTaskFunc = FieldTask_Fly;
        startMenu->exitTaskEnvironment = r5;
        startMenu->state = START_MENU_STATE_12;
    }
    return FALSE;
}

StartMenuMailReturn *StartMenu_CreateMailReturnInfo(u16 itemId, u8 kind, u8 partySlot) {
    StartMenuMailReturn *ret = Heap_Alloc(HEAP_ID_FIELD2, sizeof(StartMenuMailReturn));
    ret->itemId = itemId;
    ret->partySlot = partySlot;
    ret->kind = kind;
    return ret;
}

BOOL Task_ReturnToMenuFromMail(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    StartMenuMailReturn *unk = startMenu->exitTaskEnvironment2;
    switch (unk->kind) {
    case 3:
        UseMailArgs_Free(startMenu->exitTaskEnvironment);
        startMenu->exitTaskEnvironment = Bag_LaunchApp_AllPockets(fieldSystem, &startMenu->itemCheckUseData);
        StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn);
        break;
    case 2:
        UseMailArgs_Free(startMenu->exitTaskEnvironment);
        startMenu->exitTaskEnvironment = PartyMenu_LaunchApp_Field(fieldSystem, &startMenu->fieldMoveCheckData, unk->partySlot);
        StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn_Pokemon);
        break;
    case 0:
        if (UseMailArgs_WasMailWritten(startMenu->exitTaskEnvironment) == TRUE) {
            StartMenu_ReturnToPartyMenuAfterMail(fieldSystem, startMenu, 12);
        } else {
            UseMailArgs_Free(startMenu->exitTaskEnvironment);
            startMenu->exitTaskEnvironment = PartyMenu_LaunchApp_Field(fieldSystem, &startMenu->fieldMoveCheckData, unk->partySlot);
            StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn_Pokemon);
        }
        break;
    case 1:
        if (UseMailArgs_WasMailWritten(startMenu->exitTaskEnvironment) == TRUE) {
            StartMenu_ReturnToPartyMenuAfterMail(fieldSystem, startMenu, 11);
        } else {
            UseMailArgs_Free(startMenu->exitTaskEnvironment);
            startMenu->exitTaskEnvironment = Bag_LaunchApp_AllPockets(fieldSystem, &startMenu->itemCheckUseData);
            StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn);
        }
        break;
    }
    Heap_Free(startMenu->exitTaskEnvironment2);
    return FALSE;
}

static void StartMenu_ReturnToPartyMenuAfterMail(FieldSystem *fieldSystem, StartMenuTaskData *startMenu, u8 context) {
    StartMenuMailReturn *unk = startMenu->exitTaskEnvironment2;
    PartyMenuArgs *partyMenuArgs = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PartyMenuArgs));
    StartMenu_InitPartyMenuArgs(partyMenuArgs, fieldSystem, startMenu);
    partyMenuArgs->itemId = unk->itemId;
    partyMenuArgs->partySlot = unk->partySlot;
    partyMenuArgs->context = context;
    UseMailArgs_GiveMailToMon(startMenu->exitTaskEnvironment, Party_GetMonByIndex(partyMenuArgs->party, unk->partySlot));
    UseMailArgs_Free(startMenu->exitTaskEnvironment);
    FieldSystem_LaunchApplication(fieldSystem, &gOverlayTemplate_PartyMenu, partyMenuArgs);
    startMenu->exitTaskEnvironment = partyMenuArgs;
    StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn_Pokemon);
}

BOOL Task_ReturnToMenuFromVSRecorder(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    startMenu->exitTaskEnvironment = Bag_LaunchApp_AllPockets(fieldSystem, &startMenu->itemCheckUseData);
    StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn);
    return FALSE;
}

static void Task_StartMenu_Evolution(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    StartMenuEvolutionArgs *unk = startMenu->exitTaskEnvironment;
    Sound_Stop();
    Heap_Create(HEAP_ID_3, HEAP_ID_EVOLUTION, 0x30000);

    Party *party = SaveArray_Party_Get(fieldSystem->saveData);
    Pokemon *pokemon = Party_GetMonByIndex(party, unk->partySlot);
    EvolutionTaskData *evolution;
    if (unk->evoType == EVOCTX_LEVELUP) {
        evolution = EvolutionScene_New(party, pokemon, unk->species, Save_PlayerData_GetOptionsAddr(fieldSystem->saveData), PokemonSummary_GetDefaultUnk2C(fieldSystem->saveData), Save_Pokedex_Get(fieldSystem->saveData), Save_Bag_Get(fieldSystem->saveData), Save_GameStats_Get(fieldSystem->saveData), unk->evoMethod, TRUE, HEAP_ID_EVOLUTION);
    } else {
        evolution = EvolutionScene_New(party, pokemon, unk->species, Save_PlayerData_GetOptionsAddr(fieldSystem->saveData), PokemonSummary_GetDefaultUnk2C(fieldSystem->saveData), Save_Pokedex_Get(fieldSystem->saveData), Save_Bag_Get(fieldSystem->saveData), Save_GameStats_Get(fieldSystem->saveData), unk->evoMethod, FALSE, HEAP_ID_EVOLUTION);
    }
    StartMenuAfterEvoPartySlotBak *newEnv = Heap_Alloc(HEAP_ID_FIELD2, sizeof(StartMenuAfterEvoPartySlotBak));
    newEnv->partySlot = unk->partySlot;
    startMenu->exitTaskEnvironment2 = newEnv;
    Heap_Free(startMenu->exitTaskEnvironment);
    startMenu->exitTaskEnvironment = evolution;
    startMenu->state = START_MENU_STATE_WAIT_EVOLUTION;
}

static void Task_StartMenu_WaitEvolution(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    StartMenuTaskData *startMenu = (StartMenuTaskData *)TaskManager_GetEnvironment(taskManager);

    if (EvolutionScene_IsDone(startMenu->exitTaskEnvironment) == TRUE) {
        EvolutionScene_Delete(startMenu->exitTaskEnvironment);
        Heap_Destroy(HEAP_ID_EVOLUTION);
        StopBGM(SEQ_GS_SHINKA, 0);
        Sound_SetScene(SOUND_SCENE_NONE);
        FieldBGM_PlayEffectiveForMapHeader(fieldSystem, fieldSystem->location->mapId);
        startMenu->exitTaskEnvironment = Bag_LaunchApp_AllPockets(fieldSystem, &startMenu->itemCheckUseData);
        StartMenuAfterEvoPartySlotBak *unk = startMenu->exitTaskEnvironment2;
        BagView_SetPartySlot(startMenu->exitTaskEnvironment, unk->partySlot);
        Heap_Free(startMenu->exitTaskEnvironment2);
        StartMenu_SetExitTaskFunc(startMenu, Task_StartMenu_HandleReturn);
    }
}
