#include "wifi_plaza/wifi_plaza_player_state.h"

#include <nitro/fx/fx_trig.h>
#include <string.h>

#include "global.h"

#include "constants/global_fieldmap.h"

#include "msgdata/msg/msg_0776.h"
#include "msgdata/msg/msg_0777.h"
#include "msgdata/msg/msg_0800.h"
#include "wifi_plaza/wifi_plaza_camera.h"
#include "wifi_plaza/wifi_plaza_event.h"
#include "wifi_plaza/wifi_plaza_game_entry.h"
#include "wifi_plaza/wifi_plaza_map.h"
#include "wifi_plaza/wifi_plaza_obj.h"
#include "wifi_plaza/wifi_plaza_talk.h"

#include "assert.h"
#include "heap.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "math_util.h"
#include "sound_playback.h"
#include "system.h"

// How the own player enters the room, an index into sWiFiPlazaPlayerStateOwnEntries
enum WiFiPlazaRoomEntryMode {
    WIFI_PLAZA_ENTRY_ARRIVAL,
    WIFI_PLAZA_ENTRY_FROM_ATTRACTION,
    WIFI_PLAZA_ENTRY_FROM_GAME,
    WIFI_PLAZA_ENTRY_FROM_SURVEY,
};

enum WiFiPlazaPlayerBounceState {
    WIFI_PLAZA_BOUNCE_IDLE,
    WIFI_PLAZA_BOUNCE_PAUSE,
    WIFI_PLAZA_BOUNCE_HOP,
};

enum WiFiPlazaPlayInstrumentStep {
    WIFI_PLAZA_PLAY_INSTRUMENT_START,
    WIFI_PLAZA_PLAY_INSTRUMENT_WAIT,
    WIFI_PLAZA_PLAY_INSTRUMENT_END,
};

enum WiFiPlazaArrivalStep {
    WIFI_PLAZA_ARRIVAL_CREATE_PLAYER,
    WIFI_PLAZA_ARRIVAL_WAIT_DROP_IN,
    WIFI_PLAZA_ARRIVAL_WAIT_PAUSE,
    WIFI_PLAZA_ARRIVAL_WALK_NORTH,
    WIFI_PLAZA_ARRIVAL_WALK_NORTH_AGAIN,
    WIFI_PLAZA_ARRIVAL_TURN_EAST,
    WIFI_PLAZA_ARRIVAL_WALK_EAST,
    WIFI_PLAZA_ARRIVAL_WAIT_MOVE,
    WIFI_PLAZA_ARRIVAL_WELCOME,
    WIFI_PLAZA_ARRIVAL_ASK_FIRST_TYPE,
    WIFI_PLAZA_ARRIVAL_HANDLE_FIRST_TYPE,
    WIFI_PLAZA_ARRIVAL_CONFIRM_FIRST_TYPE,
    WIFI_PLAZA_ARRIVAL_ASK_SECOND_TYPE,
    WIFI_PLAZA_ARRIVAL_HANDLE_SECOND_TYPE,
    WIFI_PLAZA_ARRIVAL_CONFIRM_SECOND_TYPE = 15,
    WIFI_PLAZA_ARRIVAL_RECEIVE_TAP_TOY,
    WIFI_PLAZA_ARRIVAL_ENJOY_STAY,
    WIFI_PLAZA_ARRIVAL_ASK_TYPES,
    WIFI_PLAZA_ARRIVAL_CHOSE_ONE_TYPE,
    WIFI_PLAZA_ARRIVAL_CHOSE_TWO_TYPES,
    WIFI_PLAZA_ARRIVAL_WAIT_MESSAGE,
    WIFI_PLAZA_ARRIVAL_START_WALK,
};

enum WiFiPlazaReturnStep {
    WIFI_PLAZA_RETURN_CREATE_PLAYER,
    WIFI_PLAZA_RETURN_WAIT_FADE_IN,
    WIFI_PLAZA_RETURN_WAIT_STEP_OUT,
    WIFI_PLAZA_RETURN_PRINT_NOTICE,
    WIFI_PLAZA_RETURN_WAIT_NOTICE,
    WIFI_PLAZA_RETURN_START_WALK,
};

enum WiFiPlazaWalkStep {
    WIFI_PLAZA_WALK_INIT,
    WIFI_PLAZA_WALK_UPDATE,
};

enum WiFiPlazaExitStep {
    WIFI_PLAZA_EXIT_ASK_EXIT,
    WIFI_PLAZA_EXIT_WAIT_ASK_EXIT,
    WIFI_PLAZA_EXIT_ASK_DISCONNECT,
    WIFI_PLAZA_EXIT_WAIT_ASK_DISCONNECT,
    WIFI_PLAZA_EXIT_OPEN_YES_NO,
    WIFI_PLAZA_EXIT_HANDLE_YES_NO,
    WIFI_PLAZA_EXIT_RISE_OUT,
    WIFI_PLAZA_EXIT_WAIT_RISE_OUT,
    WIFI_PLAZA_EXIT_STEP_BACK,
    WIFI_PLAZA_EXIT_WAIT_STEP_BACK,
};

enum WiFiPlazaAttractionStep {
    WIFI_PLAZA_ATTRACTION_JOIN,
    WIFI_PLAZA_ATTRACTION_WAIT_JOIN,
    WIFI_PLAZA_ATTRACTION_ENTER,
    WIFI_PLAZA_ATTRACTION_SLIDE_BACK,
    WIFI_PLAZA_ATTRACTION_WAIT_SLIDE_BACK,
    WIFI_PLAZA_ATTRACTION_STEP_BACK,
    WIFI_PLAZA_ATTRACTION_WAIT_STEP_BACK,
    WIFI_PLAZA_ATTRACTION_PRINT_REFUSAL,
    WIFI_PLAZA_ATTRACTION_WAIT_REFUSAL,
};

enum WiFiPlazaGameBoothStep {
    WIFI_PLAZA_GAME_BOOTH_REGISTER,
    WIFI_PLAZA_GAME_BOOTH_CHECK_RESULT,
    WIFI_PLAZA_GAME_BOOTH_START_GAME,
    WIFI_PLAZA_GAME_BOOTH_STEP_BACK,
    WIFI_PLAZA_GAME_BOOTH_WAIT_STEP_BACK,
    WIFI_PLAZA_GAME_BOOTH_PRINT_RESULT,
    WIFI_PLAZA_GAME_BOOTH_WAIT_RESULT,
    WIFI_PLAZA_GAME_BOOTH_END,
};

enum WiFiPlazaSurveyMonitorStep {
    WIFI_PLAZA_SURVEY_MONITOR_CHECK,
    WIFI_PLAZA_SURVEY_MONITOR_ENTER,
    WIFI_PLAZA_SURVEY_MONITOR_STEP_BACK,
    WIFI_PLAZA_SURVEY_MONITOR_WAIT_STEP_BACK,
    WIFI_PLAZA_SURVEY_MONITOR_PRINT_MESSAGE,
    WIFI_PLAZA_SURVEY_MONITOR_WAIT_MESSAGE,
    WIFI_PLAZA_SURVEY_MONITOR_END,
};

enum WiFiPlazaGreeterStep {
    WIFI_PLAZA_GREETER_PRINT,
    WIFI_PLAZA_GREETER_WAIT_MESSAGE,
};

enum WiFiPlazaRideStep {
    WIFI_PLAZA_RIDE_INIT,
    WIFI_PLAZA_RIDE_JOIN_GROUP,
    WIFI_PLAZA_RIDE_WAIT_FOR_FLOAT,
    WIFI_PLAZA_RIDE_JUMP_ON,
    WIFI_PLAZA_RIDE_LAND,
    WIFI_PLAZA_RIDE_RIDE,
    WIFI_PLAZA_RIDE_STEP_BACK,
    WIFI_PLAZA_RIDE_WAIT_STEP_BACK,
    WIFI_PLAZA_RIDE_PARADE_LATER,
    WIFI_PLAZA_RIDE_FLOATS_FULL,
    WIFI_PLAZA_RIDE_PARADE_OVER,
    WIFI_PLAZA_RIDE_WAIT_MESSAGE,
    WIFI_PLAZA_RIDE_CLOSE_MESSAGE,
    WIFI_PLAZA_RIDE_RIDE_OUT,
    WIFI_PLAZA_RIDE_FINISHED,
    WIFI_PLAZA_RIDE_END,
};

enum WiFiPlazaOtherWaitStep {
    WIFI_PLAZA_OTHER_WAIT_INIT,
    WIFI_PLAZA_OTHER_WAIT_WAIT_ENTER,
    WIFI_PLAZA_OTHER_WAIT_CREATE_OBJ,
    WIFI_PLAZA_OTHER_WAIT_PLACE,
    WIFI_PLAZA_OTHER_WAIT_WAIT_DROP_IN,
    WIFI_PLAZA_OTHER_WAIT_START_PRESENT,
};

enum WiFiPlazaOtherPresentStep {
    WIFI_PLAZA_OTHER_PRESENT_INIT,
    WIFI_PLAZA_OTHER_PRESENT_UPDATE,
};

enum WiFiPlazaOtherLeaveStep {
    WIFI_PLAZA_OTHER_LEAVE_RISE_OUT,
    WIFI_PLAZA_OTHER_LEAVE_WAIT_RISE_OUT,
    WIFI_PLAZA_OTHER_LEAVE_DELETE,
};

enum WiFiPlazaOtherRideStep {
    WIFI_PLAZA_OTHER_RIDE_RISE_OUT,
    WIFI_PLAZA_OTHER_RIDE_WAIT_RISE_OUT,
    WIFI_PLAZA_OTHER_RIDE_MOVE_TO_FLOAT,
    WIFI_PLAZA_OTHER_RIDE_RIDE,
    WIFI_PLAZA_OTHER_RIDE_DELETE,
};

enum WiFiPlazaOtherActivityStep {
    WIFI_PLAZA_OTHER_ACTIVITY_START,
    WIFI_PLAZA_OTHER_ACTIVITY_RISE_OUT,
    WIFI_PLAZA_OTHER_ACTIVITY_WAIT_RISE_OUT,
    WIFI_PLAZA_OTHER_ACTIVITY_DROP_IN,
    WIFI_PLAZA_OTHER_ACTIVITY_WAIT_DROP_IN,
    WIFI_PLAZA_OTHER_ACTIVITY_STAY,
    WIFI_PLAZA_OTHER_ACTIVITY_LEAVE,
    WIFI_PLAZA_OTHER_ACTIVITY_WAIT_LEAVE,
    WIFI_PLAZA_OTHER_ACTIVITY_RETURN,
    WIFI_PLAZA_OTHER_ACTIVITY_WAIT_RETURN,
    WIFI_PLAZA_OTHER_ACTIVITY_END,
};

// Where and how the own player re-enters the room; entryMode picks the own player's first handler from
// sWiFiPlazaPlayerStateOwnEntries
typedef struct WiFiPlazaRoomEntry {
    u16 x; // cell
    u16 y;
    u16 direction;
    u16 entryMode;
    u16 param;
} WiFiPlazaRoomEntry;

// Work of WiFiPlazaPlayerState_OwnArrival
typedef struct WiFiPlazaOwnArrivalWork {
    u16 nextStep;
    u16 listValue;
    u32 timer;
    ListMenuTemplate menuTemplate;
    u16 firstChoice;
    u16 secondChoice;
} WiFiPlazaOwnArrivalWork;

// Work of WiFiPlazaPlayerState_OwnEnterAttraction
typedef struct WiFiPlazaAttractionWork {
    int timer;
    u16 msgId;
    u16 msgBank;
    int mode;
} WiFiPlazaAttractionWork;

// Work of WiFiPlazaPlayerState_OwnRideMover; the WiFiPlazaPlayerRide_* helpers use it
typedef struct WiFiPlazaPlayerRide {
    int mover; // packed mover index / side (WiFiPlaza_SplitGroupPlace)
    u16 nextStep;
    u16 choice;
    s16 timer;
    u8 resumeStep;
    u8 ticks;
    WiFiPlazaObj *obj;
    WiFiPlazaPlayerBounce bounce;
} WiFiPlazaPlayerRide;

// Work of WiFiPlazaPlayerState_OtherRideMover
typedef struct WiFiPlazaOtherRideWork {
    WiFiPlazaObj *obj;
    int mover;
    int side;
    fx32 startY;
    fx32 x;
    int frame;
    WiFiPlazaPlayerBounce bounce;
} WiFiPlazaOtherRideWork;

// Arg of WiFiPlazaPlayerState_OtherDoActivity (written by WiFiPlazaPlayerState_PlaceOtherForActivity): another player's cell before it moves to a lobby activity
typedef struct WiFiPlazaPlacement {
    u8 x; // cell
    u8 y;
    u8 kind;            // lobby activity of the player
    u8 besideOwnPlayer; // placed next to the own player
    u8 justAppeared;    // the object was just created: drop it in without rising out first
} WiFiPlazaPlacement;

// Arg of WiFiPlazaPlayerState_OwnEnterMinigame's pushed handler (WiFiPlazaGameEntry_Register)
typedef struct WiFiPlazaPlayerStateResult {
    u8 result;
    u8 unused;
    u8 param;
} WiFiPlazaPlayerStateResult;

static void WiFiPlazaPlayerState_Set(WiFiPlazaPlayerStateManager *manager, u32 index, const WiFiPlazaPlayerStateHandlers *handlers, void *arg);
static void WiFiPlazaPlayerState_Push(WiFiPlazaPlayerStateManager *manager, u32 index, const WiFiPlazaPlayerStateHandlers *handlers, void *arg);
static void WiFiPlazaPlayerState_RunAll(WiFiPlazaPlayerStateManager *manager, u32 which);
static void WiFiPlazaPlayerState_Init(WiFiPlazaPlayerState *state, u32 index, u32 heapID);
static void WiFiPlazaPlayerState_Clear(WiFiPlazaPlayerState *state);
static void WiFiPlazaPlayerState_Run(WiFiPlazaPlayerState *state, WiFiPlazaPlayerStateManager *manager, u32 which, u32 index);
static void WiFiPlazaPlayerState_SetFrame(WiFiPlazaPlayerState *state, const WiFiPlazaPlayerStateHandlers *handlers, void *arg);
static void WiFiPlazaPlayerState_SetActiveFlag(WiFiPlazaPlayerState *state, u8 active);
static void WiFiPlazaPlayerState_PushFrame(WiFiPlazaPlayerState *state, const WiFiPlazaPlayerStateHandlers *handlers, void *arg);
static void WiFiPlazaPlayerState_PopFrame(WiFiPlazaPlayerState *state);
static BOOL WiFiPlazaPlayerState_IsSavedFree(WiFiPlazaPlayerState *state);
static BOOL WiFiPlazaPlayerState_HasHandler(WiFiPlazaPlayerState *state);
static void WiFiPlazaPlayerStateFrame_Init(WiFiPlazaPlayerStateFrame *frame, const WiFiPlazaPlayerStateHandlers *handlers, void *arg, u32 step, void *work);
static void WiFiPlazaPlayerStateFrame_Clear(WiFiPlazaPlayerStateFrame *frame);
static BOOL WiFiPlazaPlayerState_IsDirectionHeld(int direction);
static void WiFiPlazaPlayerState_RunTileAction(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index, const WiFiPlazaTileAction *actions, u32 tile);
static WiFiPlazaObj *WiFiPlazaPlayerState_CreateOwnPlayer(WiFiPlazaRoomWork *room, int id);
static void WiFiPlazaPlayerState_CreateOwnPlayerAt(WiFiPlazaRoomWork *room, int id, int x, int y, s32 direction);
static void WiFiPlazaPlayerBounce_Start(WiFiPlazaPlayerBounce *bounce);
static fx32 WiFiPlazaPlayerBounce_Update(WiFiPlazaPlayerBounce *bounce);
static void WiFiPlazaPlayerBounce_Stop(WiFiPlazaPlayerBounce *bounce);
static BOOL WiFiPlazaPlayerBounce_IsActive(WiFiPlazaPlayerBounce *bounce);
static BOOL WiFiPlazaPlayerState_PlayInstrument(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OwnArrival(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OwnReturnFromAttraction(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OwnReturnFromMinigame(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OwnReturnFromSurvey(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OwnWalk(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OwnWalkLate(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OwnExit(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OwnEnterAttraction(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OwnEnterMinigame(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OwnUseSurveyMonitor(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static int WiFiPlazaPlayerState_GetAttractionSlot(int tile);
static int WiFiPlazaPlayerState_GetAttractionResult(int tile);
static int WiFiPlazaPlayerState_GetAttractionLobbyState(int tile);
static BOOL WiFiPlazaTalk_Greeter(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OwnRideMover(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static void WiFiPlazaPlayerRide_FollowCamera(WiFiPlazaPlayerRide *ride, WiFiPlazaCamera *camera);
static BOOL WiFiPlazaPlayerRide_IsMoverHere(WiFiPlazaPlayerRide *ride, void *models);
static BOOL WiFiPlazaPlayerRide_JumpOn(WiFiPlazaPlayerRide *ride, void *models, WiFiPlazaCamera *camera);
static BOOL WiFiPlazaPlayerRide_Land(WiFiPlazaPlayerRide *ride, void *models);
static void WiFiPlazaPlayerRide_FollowMover(WiFiPlazaPlayerRide *ride, void *models, WiFiPlazaRoomWork *room);
static void WiFiPlazaPlayerRide_FollowMoverBounce(WiFiPlazaPlayerRide *ride, void *models, WiFiPlazaRoomWork *room);
static void WiFiPlazaPlayerState_InitArrivalMenu(WiFiPlazaOwnArrivalWork *work, WiFiPlazaRoomWork *room);
static void WiFiPlazaPlayerState_FreeArrivalMenu(WiFiPlazaOwnArrivalWork *work, WiFiPlazaRoomWork *room);
static BOOL WiFiPlazaPlayerState_OtherWaitForPlayer(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OtherPresent(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OtherLeave(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OtherRideMover(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_OtherDoActivity(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index);
static BOOL WiFiPlazaPlayerState_PlaceOtherForActivity(WiFiPlazaPlacement *placement, WiFiPlazaWork *lobby, WiFiPlazaRoomWork *room, WiFiPlazaObj *obj, u8 kind, u32 justAppeared);
static BOOL WiFiPlazaPlayerState_FindFreeTaggedCell(WiFiPlazaMap *map, WiFiPlazaObjManager *objManager, const u32 *tags, u32 count, u16 *outPos);

static const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateGameEntryEndLink = {
    .update = WiFiPlazaGameEntry_EndLink,
    .lateUpdate = NULL,
};

static const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateGameEntryRegister = {
    .update = WiFiPlazaGameEntry_Register,
    .lateUpdate = NULL,
};

static const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateOtherWait = {
    .update = WiFiPlazaPlayerState_OtherWaitForPlayer,
    .lateUpdate = NULL,
};

static const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateTalkAnswer = {
    .update = WiFiPlazaTalk_Answer,
    .lateUpdate = NULL,
};

static const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateTalkStart = {
    .update = WiFiPlazaTalk_Start,
    .lateUpdate = NULL,
};

static const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateOtherActivity = {
    .update = WiFiPlazaPlayerState_OtherDoActivity,
    .lateUpdate = NULL,
};

static const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateOtherLeave = {
    .update = WiFiPlazaPlayerState_OtherLeave,
    .lateUpdate = NULL,
};

static const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStatePlayInstrument = {
    .update = WiFiPlazaPlayerState_PlayInstrument,
    .lateUpdate = NULL,
};

static const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateOtherRideMover = {
    .update = WiFiPlazaPlayerState_OtherRideMover,
    .lateUpdate = NULL,
};

static const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateOwnWalk = {
    .update = WiFiPlazaPlayerState_OwnWalk,
    .lateUpdate = WiFiPlazaPlayerState_OwnWalkLate,
};

static const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateOtherPresent = {
    .update = WiFiPlazaPlayerState_OtherPresent,
    .lateUpdate = NULL,
};

static const u32 sWiFiPlazaActivity2Tags[4] = { 79, 80, 81, 82 };

static const u32 sWiFiPlazaActivity4Tags[4] = { 87, 88, 89, 90 };

static const u32 sWiFiPlazaActivity7Tags[4] = { 50, 51, 52, 53 };

static const u32 sWiFiPlazaActivity3Tags[4] = { 83, 84, 85, 86 };

static const u32 sWiFiPlazaActivity8Tags[4] = { 54, 55, 56, 57 };

static const WiFiPlazaPlayerStateHandlers sWiFiPlazaPlayerStateOwnEntries[4] = {
    {
     .update = WiFiPlazaPlayerState_OwnArrival,
     .lateUpdate = NULL,
     },
    {
     .update = WiFiPlazaPlayerState_OwnReturnFromAttraction,
     .lateUpdate = NULL,
     },
    {
     .update = WiFiPlazaPlayerState_OwnReturnFromMinigame,
     .lateUpdate = NULL,
     },
    {
     .update = WiFiPlazaPlayerState_OwnReturnFromSurvey,
     .lateUpdate = NULL,
     },
};

static const ListMenuTemplate sWiFiPlazaArrivalMenuTemplate = {
    .items = NULL,
    .moveCursorFunc = NULL,
    .itemPrintFunc = NULL,
    .window = NULL,
    .totalItems = 18,
    .maxShowed = 7,
    .header_X = 0,
    .item_X = 8,
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

static const u32 sWiFiPlazaActivity5Tags[8] = { 34, 35, 36, 37, 38, 39, 40, 41 };

static const u32 sWiFiPlazaActivity6Tags[8] = { 42, 43, 44, 45, 46, 47, 48, 49 };

static const WiFiPlazaTileAction sWiFiPlazaWalkUpTileActions[2] = {
    {
     .tiles = { 39, 40, 41, 255, 0, 0, 0, 0 },
     .action = 1,
     .handlers = {
            .update = WiFiPlazaGameEntry_ReadSign,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 255, 0, 0, 0, 0, 0, 0, 0 },
     .action = 3,
     .handlers = {
            .update = NULL,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
};

static const WiFiPlazaTileAction sWiFiPlazaCheckTileActions[6] = {
    {
     .tiles = { 36, 255, 0, 0, 0, 0, 0, 0 },
     .action = 1,
     .handlers = {
            .update = WiFiPlazaTalk_Greeter,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 37, 255, 0, 0, 0, 0, 0, 0 },
     .action = 1,
     .handlers = {
            .update = WiFiPlazaTalk_InfoCounter,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 38, 255, 0, 0, 0, 0, 0, 0 },
     .action = 1,
     .handlers = {
            .update = WiFiPlazaTalk_Reception,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 39, 40, 41, 255, 0, 0, 0, 0 },
     .action = 1,
     .handlers = {
            .update = WiFiPlazaGameEntry_ReadSign,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 43, 255, 0, 0, 0, 0, 0, 0 },
     .action = 1,
     .handlers = {
            .update = WiFiPlazaTalk_Survey,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 255, 0, 0, 0, 0, 0, 0, 0 },
     .action = 3,
     .handlers = {
            .update = NULL,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
};

static const WiFiPlazaTileAction sWiFiPlazaStandTileActions[9] = {
    {
     .tiles = { 1, 255, 0, 0, 0, 0, 0, 0 },
     .action = 0,
     .handlers = {
            .update = WiFiPlazaPlayerState_OwnExit,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 22, 23, 24, 25, 255, 0, 0, 0 },
     .action = 0,
     .handlers = {
            .update = WiFiPlazaPlayerState_OwnEnterAttraction,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 26, 27, 28, 29, 255, 0, 0, 0 },
     .action = 0,
     .handlers = {
            .update = WiFiPlazaPlayerState_OwnEnterAttraction,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 6, 7, 8, 9, 10, 11, 12, 13 },
     .action = 0,
     .handlers = {
            .update = WiFiPlazaPlayerState_OwnEnterAttraction,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 14, 15, 16, 17, 18, 19, 20, 21 },
     .action = 0,
     .handlers = {
            .update = WiFiPlazaPlayerState_OwnEnterAttraction,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 33, 34, 35, 255, 0, 0, 0, 0 },
     .action = 0,
     .handlers = {
            .update = WiFiPlazaPlayerState_OwnRideMover,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 30, 31, 32, 255, 0, 0, 0, 0 },
     .action = 0,
     .handlers = {
            .update = WiFiPlazaPlayerState_OwnEnterMinigame,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 44, 255, 0, 0, 0, 0, 0, 0 },
     .action = 0,
     .handlers = {
            .update = WiFiPlazaPlayerState_OwnUseSurveyMonitor,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
    {
     .tiles = { 255, 0, 0, 0, 0, 0, 0, 0 },
     .action = 3,
     .handlers = {
            .update = NULL,
            .lateUpdate = NULL,
        },
     .callback = NULL,
     },
};

WiFiPlazaPlayerStateManager *WiFiPlazaPlayerState_CreateManager(WiFiPlazaRoomWork *room, enum HeapID heapID) {
    int i;
    WiFiPlazaPlayerStateManager *manager = Heap_Alloc(heapID, sizeof(WiFiPlazaPlayerStateManager));
    memset(manager, 0, sizeof(WiFiPlazaPlayerStateManager));
    manager->heapID = heapID;
    manager->room = room;
    for (i = 0; i < WIFI_PLAZA_PLAYER_STATE_COUNT; i++) {
        WiFiPlazaPlayerState_Init(&manager->states[i], i, heapID);
    }
    WiFiPlazaPlayerState_Init(&manager->exclusive, 0, heapID);
    return manager;
}

void WiFiPlazaPlayerState_DeleteManager(WiFiPlazaPlayerStateManager *manager) {
    int i;
    for (i = 0; i < WIFI_PLAZA_PLAYER_STATE_COUNT; i++) {
        WiFiPlazaPlayerState_Clear(&manager->states[i]);
    }
    WiFiPlazaPlayerState_Clear(&manager->exclusive);
    Heap_Free(manager);
}

void WiFiPlazaPlayerState_Update(WiFiPlazaPlayerStateManager *manager) {
    WiFiPlazaPlayerState_RunAll(manager, 0);
}

void WiFiPlazaPlayerState_LateUpdate(WiFiPlazaPlayerStateManager *manager) {
    WiFiPlazaPlayerState_RunAll(manager, 1);
}

void *WiFiPlazaPlayerState_GetArg(WiFiPlazaPlayerState *state) {
    return state->current.arg;
}

void *WiFiPlazaPlayerState_AllocWork(WiFiPlazaPlayerState *state, u32 size) {
    GF_ASSERT(state->current.work == NULL);
    state->current.work = Heap_Alloc((enum HeapID)state->heapID, size);
    memset(state->current.work, 0, size);
    return state->current.work;
}

void WiFiPlazaPlayerState_FreeWork(WiFiPlazaPlayerState *state) {
    GF_ASSERT(state->current.work != NULL);
    Heap_Free(state->current.work);
    state->current.work = NULL;
}

void *WiFiPlazaPlayerState_GetWork(WiFiPlazaPlayerState *state) {
    return state->current.work;
}

u32 WiFiPlazaPlayerState_GetStep(WiFiPlazaPlayerState *state) {
    return state->current.step;
}

void WiFiPlazaPlayerState_SetStep(WiFiPlazaPlayerState *state, u32 step) {
    state->current.step = step;
}

void WiFiPlazaPlayerState_NextStep(WiFiPlazaPlayerState *state) {
    state->current.step++;
}

static void WiFiPlazaPlayerState_Set(WiFiPlazaPlayerStateManager *manager, u32 index, const WiFiPlazaPlayerStateHandlers *handlers, void *arg) {
    if (manager == NULL) {
        GF_AssertFail();
    }
    if (index >= WIFI_PLAZA_PLAYER_STATE_COUNT) {
        GF_AssertFail();
    }
    WiFiPlazaPlayerState_SetFrame(&manager->states[index], handlers, arg);
}

static void WiFiPlazaPlayerState_Push(WiFiPlazaPlayerStateManager *manager, u32 index, const WiFiPlazaPlayerStateHandlers *handlers, void *arg) {
    if (manager == NULL) {
        GF_AssertFail();
    }
    if (index >= WIFI_PLAZA_PLAYER_STATE_COUNT) {
        GF_AssertFail();
    }
    WiFiPlazaPlayerState_PushFrame(&manager->states[index], handlers, arg);
}

void WiFiPlazaPlayerState_SetActive(WiFiPlazaPlayerStateManager *manager, u32 index, u8 active) {
    if (manager == NULL) {
        GF_AssertFail();
    }
    if (index >= WIFI_PLAZA_PLAYER_STATE_COUNT) {
        GF_AssertFail();
    }
    WiFiPlazaPlayerState_SetActiveFlag(&manager->states[index], active);
}

static void WiFiPlazaPlayerState_RunAll(WiFiPlazaPlayerStateManager *manager, u32 which) {
    int i;
    WiFiPlazaPlayerState *state;
    WiFiPlazaPlayerState *running;

    if (WiFiPlazaPlayerState_HasHandler(&manager->exclusive) == TRUE) {
        WiFiPlazaPlayerState_Run(&manager->exclusive, manager, which, 0);
        return;
    }
    state = manager->states;
    i = 0;
    running = state;
    for (; i < WIFI_PLAZA_PLAYER_STATE_COUNT; i++, state++, running++) {
        if (WiFiPlazaPlayerState_HasHandler(state) == TRUE) {
            WiFiPlazaPlayerState_Run(running, manager, which, i);
        }
    }
}

static void WiFiPlazaPlayerState_Init(WiFiPlazaPlayerState *state, u32 index, u32 heapID) {
    state->heapID = heapID;
    state->active = TRUE;
    state->index = index;
}

static void WiFiPlazaPlayerState_Clear(WiFiPlazaPlayerState *state) {
    if (state->current.work != NULL) {
        Heap_Free(state->current.work);
    }
    if (state->saved.work != NULL) {
        Heap_Free(state->saved.work);
    }
    memset(state, 0, sizeof(WiFiPlazaPlayerState));
}

static void WiFiPlazaPlayerState_Run(WiFiPlazaPlayerState *state, WiFiPlazaPlayerStateManager *manager, u32 which, u32 index) {
    WiFiPlazaPlayerStateFunc func;

    if (state->active == FALSE) {
        return;
    }
    switch (which) {
    case 0:
        func = state->current.handlers->update;
        break;
    case 1:
        func = state->current.handlers->lateUpdate;
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (func != NULL && func(state, manager->room, index) == TRUE) {
        WiFiPlazaPlayerState_PopFrame(state);
    }
}

static void WiFiPlazaPlayerState_SetFrame(WiFiPlazaPlayerState *state, const WiFiPlazaPlayerStateHandlers *handlers, void *arg) {
    if (WiFiPlazaPlayerState_IsSavedFree(state) != TRUE) {
        GF_AssertFail();
    }
    if (state->current.work != NULL) {
        GF_AssertFail();
    }
    WiFiPlazaPlayerStateFrame_Init(&state->current, handlers, arg, 0, NULL);
}

static void WiFiPlazaPlayerState_SetActiveFlag(WiFiPlazaPlayerState *state, u8 active) {
    state->active = active;
}

static void WiFiPlazaPlayerState_PushFrame(WiFiPlazaPlayerState *state, const WiFiPlazaPlayerStateHandlers *handlers, void *arg) {
    GF_ASSERT(WiFiPlazaPlayerState_IsSavedFree(state) == TRUE);
    state->saved = state->current;
    WiFiPlazaPlayerStateFrame_Init(&state->current, handlers, arg, 0, NULL);
}

static void WiFiPlazaPlayerState_PopFrame(WiFiPlazaPlayerState *state) {
    GF_ASSERT(state->current.work == NULL);
    state->current = state->saved;
    WiFiPlazaPlayerStateFrame_Clear(&state->saved);
}

static BOOL WiFiPlazaPlayerState_IsSavedFree(WiFiPlazaPlayerState *state) {
    return state->saved.handlers == NULL;
}

static BOOL WiFiPlazaPlayerState_HasHandler(WiFiPlazaPlayerState *state) {
    return state->current.handlers != NULL;
}

static void WiFiPlazaPlayerStateFrame_Init(WiFiPlazaPlayerStateFrame *frame, const WiFiPlazaPlayerStateHandlers *handlers, void *arg, u32 step, void *work) {
    frame->handlers = handlers;
    frame->step = step;
    frame->work = work;
    frame->arg = arg;
}

static void WiFiPlazaPlayerStateFrame_Clear(WiFiPlazaPlayerStateFrame *frame) {
    frame->handlers = NULL;
    frame->step = 0;
    frame->work = NULL;
    frame->arg = NULL;
}

void WiFiPlazaPlayerState_InitOwn(WiFiPlazaRoomWork *room) {
    WiFiPlazaRoomEntry *entry = WiFiPlazaRoom_GetStartParams(room);
    WiFiPlazaPlayerStateManager *manager = WiFiPlazaRoom_GetPlayerStates(room);
    int index = WiFiPlazaRoom_GetOwnPlayerIndex(room);
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    WiFiPlazaWork_SetActivity(lobby, WIFI_PLAZA_ACTIVITY_FREE);
    GF_ASSERT(entry->entryMode < 4);
    WiFiPlazaPlayerState_Set(manager, index, &sWiFiPlazaPlayerStateOwnEntries[entry->entryMode], NULL);
}

void WiFiPlazaPlayerState_InitOthers(WiFiPlazaRoomWork *room) {
    int i;
    int ownIndex = WiFiPlazaRoom_GetOwnPlayerIndex(room);
    WiFiPlazaPlayerStateManager *manager = WiFiPlazaRoom_GetPlayerStates(room);
    for (i = 0; i < WIFI_PLAZA_PLAYER_STATE_COUNT; i++) {
        if (ownIndex != i) {
            WiFiPlazaPlayerState_Set(manager, i, &sWiFiPlazaPlayerStateOtherWait, NULL);
        }
    }
}

// direction: 0 up, 1 down, 2 left, 3 right
static BOOL WiFiPlazaPlayerState_IsDirectionHeld(int direction) {
    int mask;
    switch (direction) {
    case DIR_NORTH:
        mask = PAD_KEY_UP;
        break;
    case DIR_SOUTH:
        mask = PAD_KEY_DOWN;
        break;
    case DIR_WEST:
        mask = PAD_KEY_LEFT;
        break;
    case DIR_EAST:
        mask = PAD_KEY_RIGHT;
        break;
    }
    return (gSystem.heldKeys & mask) ? TRUE : FALSE;
}

// Starts the action of the first table entry listing `tile`
static void WiFiPlazaPlayerState_RunTileAction(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index, const WiFiPlazaTileAction *actions, u32 tile) {
    void *work = WiFiPlazaPlayerState_GetWork(state);
    WiFiPlazaPlayerStateManager *manager = WiFiPlazaRoom_GetPlayerStates(room);
    int i;
    int j;

    for (i = 0; actions[i].action != WIFI_PLAZA_TILE_ACTION_END; i++) {
        for (j = 0; j < 8; j++) {
            if (actions[i].tiles[j] == 0xFF) {
                break;
            }
            if (actions[i].tiles[j] == tile) {
                switch (actions[i].action) {
                case WIFI_PLAZA_TILE_ACTION_SET:
                    WiFiPlazaPlayerState_FreeWork(state);
                    WiFiPlazaPlayerState_Set(manager, index, &actions[i].handlers, NULL);
                    return;
                case WIFI_PLAZA_TILE_ACTION_PUSH:
                    if (actions[i].callback != NULL) {
                        actions[i].callback(work, room, index, actions[i].tiles[j]);
                    }
                    WiFiPlazaPlayerState_Push(manager, index, &actions[i].handlers, NULL);
                    return;
                default:
                    return;
                }
            }
        }
    }
    GF_AssertFail();
}

static WiFiPlazaObj *WiFiPlazaPlayerState_CreateOwnPlayer(WiFiPlazaRoomWork *room, int id) {
    VecFx32 pos;
    WiFiPlazaObj *obj = WiFiPlazaObj_CreateOwnPlayer(WiFiPlazaRoom_GetObjManager(room), id);
    WiFiPlazaCamera *camera = WiFiPlazaRoom_GetCamera(room);
    WiFiPlazaObj_GetPosition(obj, &pos);
    WiFiPlazaCamera_SetTargetWithOffset(camera, pos.x, pos.y, pos.z);
    return obj;
}

static void WiFiPlazaPlayerState_CreateOwnPlayerAt(WiFiPlazaRoomWork *room, int id, int x, int y, s32 direction) {
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaCamera *camera = WiFiPlazaRoom_GetCamera(room);
    WiFiPlazaObj *obj = WiFiPlazaObj_CreateOwnPlayerAt(objManager, id, x, y);
    WiFiPlazaObj_QueueCommand(objManager, obj, 0, direction);
    WiFiPlazaCamera_SetFollowObject(camera, obj);
}

static void WiFiPlazaPlayerBounce_Start(WiFiPlazaPlayerBounce *bounce) {
    bounce->offset = 0;
    bounce->frame = 0;
    bounce->state = WIFI_PLAZA_BOUNCE_PAUSE;
    bounce->timer = MTRandom() & 3;
    bounce->repeat = TRUE;
}

static fx32 WiFiPlazaPlayerBounce_Update(WiFiPlazaPlayerBounce *bounce) {
    switch (bounce->state) {
    case WIFI_PLAZA_BOUNCE_IDLE:
        bounce->offset = 0;
        break;
    case WIFI_PLAZA_BOUNCE_PAUSE: {
        int timer = bounce->timer - 1;
        if (timer > 0) {
            bounce->timer = timer;
        } else {
            bounce->state = WIFI_PLAZA_BOUNCE_HOP;
        }
        break;
    }
    case WIFI_PLAZA_BOUNCE_HOP: {
        int angle;
        bounce->frame++;
        angle = bounce->frame * 0x7FFF / 4;
        bounce->offset = (fx32)(((s64)FX_SinCosTable_[((u16)angle >> 4) * 2] * FX32_CONST(6) + 0x800) >> 12);
        if (bounce->frame >= 4) {
            if (bounce->repeat == TRUE) {
                bounce->frame = 0;
                bounce->state = WIFI_PLAZA_BOUNCE_PAUSE;
                bounce->timer = MTRandom() & 3;
            } else {
                bounce->state = WIFI_PLAZA_BOUNCE_IDLE;
            }
        }
        break;
    }
    }
    return bounce->offset;
}

static void WiFiPlazaPlayerBounce_Stop(WiFiPlazaPlayerBounce *bounce) {
    bounce->repeat = FALSE;
}

static BOOL WiFiPlazaPlayerBounce_IsActive(WiFiPlazaPlayerBounce *bounce) {
    return bounce->state != WIFI_PLAZA_BOUNCE_IDLE;
}

static BOOL WiFiPlazaPlayerState_PlayInstrument(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager;
    WiFiPlazaObj *obj;
    WiFiPlazaWork *lobby;

    objManager = WiFiPlazaRoom_GetObjManager(room);
    obj = WiFiPlazaObj_FindById(objManager, index);
    lobby = WiFiPlazaRoom_GetLobby(room);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_PLAY_INSTRUMENT_START:
        WiFiPlazaRoom_StartInstrument(room, index);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_PLAY_INSTRUMENT_WAIT);
        if (index == WiFiPlazaWork_GetOwnSlot(lobby)) {
            WiFiPlazaRoom_SetPanelButtonDisabled(room, 0);
        }
        break;
    case WIFI_PLAZA_PLAY_INSTRUMENT_WAIT:
        if (WiFiPlazaRoom_IsInstrumentFinished(room, index) == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_PLAY_INSTRUMENT_END);
        }
        break;
    case WIFI_PLAZA_PLAY_INSTRUMENT_END:
        if (index == WiFiPlazaWork_GetOwnSlot(lobby)) {
            WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_PLAYER_INPUT);
        } else {
            if (WiFiPlazaRoom_IsPlayerCardShown(room) == TRUE) {
                if (index != WiFiPlazaRoom_GetPlayerCardPlayer(room)) {
                    WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_WANDER);
                }
            } else {
                WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_WANDER);
            }
            WiFiPlazaRoom_SetPlayerBusy(room, (u8)index, 0);
        }
        WiFiPlazaRoom_ResetPanelButton(room);
        return TRUE;
    default:
        break;
    }

    return FALSE;
}

// First entry of the own player: it walks in, then two list menus pick two values for the lobby
static BOOL WiFiPlazaPlayerState_OwnArrival(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaOwnArrivalWork *work;
    WiFiPlazaWork *lobby;
    WiFiPlazaObjManager *objManager;
    WiFiPlazaObj *ownPlayer;
    u32 choice;
    String *message;
    BOOL done;

    lobby = WiFiPlazaRoom_GetLobby(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
    work = WiFiPlazaPlayerState_GetWork(state);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_ARRIVAL_CREATE_PLAYER:
        work = WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaOwnArrivalWork));
        WiFiPlazaPlayerState_InitArrivalMenu(work, room);
        WiFiPlazaWork_PlayBGM(lobby);
        work->listValue = WiFiPlazaWork_PickTapToy(lobby);
        WiFiPlazaWork_SetTapToy(lobby, work->listValue);
        WiFiPlazaObj_SetHandler(objManager, WiFiPlazaPlayerState_CreateOwnPlayer(room, index), WIFI_PLAZA_OBJ_HANDLER_DROP_IN);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_ARRIVAL_WAIT_DROP_IN:
        if (WiFiPlazaRoom_IsFading(room) == 0 && WiFiPlazaObj_IsFinished(ownPlayer) == TRUE) {
            WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_IDLE);
            WiFiPlazaCamera_SetFollowObject(WiFiPlazaRoom_GetCamera(room), ownPlayer);
            work->timer = 16;
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_ARRIVAL_WAIT_PAUSE:
        if (--work->timer == 0) {
            WiFiPlazaRoom_SetMode(room, 1);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_ARRIVAL_WALK_NORTH:
        WiFiPlazaObj_QueueCommand(objManager, ownPlayer, 2, DIR_NORTH);
        work->nextStep = WIFI_PLAZA_ARRIVAL_WALK_NORTH_AGAIN;
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_WAIT_MOVE);
        break;
    case WIFI_PLAZA_ARRIVAL_WALK_NORTH_AGAIN:
        WiFiPlazaObj_QueueCommand(objManager, ownPlayer, 2, DIR_NORTH);
        work->nextStep = WIFI_PLAZA_ARRIVAL_TURN_EAST;
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_WAIT_MOVE);
        break;
    case WIFI_PLAZA_ARRIVAL_TURN_EAST:
        WiFiPlazaObj_QueueCommand(objManager, ownPlayer, 1, DIR_EAST);
        work->nextStep = WIFI_PLAZA_ARRIVAL_WALK_EAST;
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_WAIT_MOVE);
        break;
    case WIFI_PLAZA_ARRIVAL_WALK_EAST:
        WiFiPlazaObj_QueueCommand(objManager, ownPlayer, 2, DIR_EAST);
        work->nextStep = WIFI_PLAZA_ARRIVAL_WELCOME;
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_WAIT_MOVE);
        break;
    case WIFI_PLAZA_ARRIVAL_WAIT_MOVE:
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            WiFiPlazaPlayerState_SetStep(state, work->nextStep);
        }
        break;
    case WIFI_PLAZA_ARRIVAL_WELCOME:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00000));
        work->nextStep = WIFI_PLAZA_ARRIVAL_RECEIVE_TAP_TOY;
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ARRIVAL_ASK_FIRST_TYPE:
        WiFiPlazaRoom_PrintMessageInstant(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00005));
        WiFiPlazaRoom_OpenList(room, &work->menuTemplate, 0, 0);
        WiFiPlazaRoom_SetListArrows(room, TRUE);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_HANDLE_FIRST_TYPE);
        break;
    case WIFI_PLAZA_ARRIVAL_HANDLE_FIRST_TYPE:
        done = FALSE;
        choice = WiFiPlazaRoom_ProcessList(room);
        switch (choice) {
        case LIST_CANCEL:
            PlaySE(SEQ_SE_DP_SELECT);
        case 0:
            work->firstChoice = 0;
            done = TRUE;
            break;
        case LIST_NOTHING_CHOSEN:
            break;
        default:
            work->firstChoice = choice;
            done = TRUE;
            WiFiPlazaRoom_BufferTypeName(room, 0, choice);
            break;
        }
        if (done == TRUE) {
            WiFiPlazaRoom_CloseList(room, NULL, NULL);
            if (work->firstChoice != 0) {
                WiFiPlazaRoom_PrintMessageInstant(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00008));
                WiFiPlazaRoom_OpenYesNoMenu(room);
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_CONFIRM_FIRST_TYPE);
            } else {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_ENJOY_STAY);
            }
        }
        break;
    case WIFI_PLAZA_ARRIVAL_CONFIRM_FIRST_TYPE:
        switch (WiFiPlazaRoom_GetYesNoResult(room)) {
        case WIFI_PLAZA_YES_NO_YES:
            WiFiPlazaRoom_CloseYesNoMenu(room);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_ASK_SECOND_TYPE);
            WiFiPlazaWork_SetFavoriteTypes(lobby, work->firstChoice, work->secondChoice);
            WiFiPlazaRoom_RefreshPlayerCardTypes(room);
            PlaySE(SEQ_SE_DP_BUTTON3);
            break;
        case WIFI_PLAZA_YES_NO_NO:
            WiFiPlazaRoom_CloseYesNoMenu(room);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_ASK_FIRST_TYPE);
            break;
        }
        break;
    case WIFI_PLAZA_ARRIVAL_ASK_SECOND_TYPE:
        WiFiPlazaRoom_PrintMessageInstant(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00010));
        WiFiPlazaRoom_OpenList(room, &work->menuTemplate, 0, 0);
        WiFiPlazaRoom_SetListArrows(room, TRUE);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_HANDLE_SECOND_TYPE);
        break;
    case WIFI_PLAZA_ARRIVAL_HANDLE_SECOND_TYPE:
        done = FALSE;
        choice = WiFiPlazaRoom_ProcessList(room);
        switch (choice) {
        case LIST_CANCEL:
            PlaySE(SEQ_SE_DP_SELECT);
        case 0:
            work->secondChoice = 0;
            done = TRUE;
            break;
        case LIST_NOTHING_CHOSEN:
            break;
        default:
            work->secondChoice = choice;
            done = TRUE;
            WiFiPlazaRoom_BufferTypeName(room, 0, choice);
            break;
        }
        if (done == TRUE) {
            WiFiPlazaRoom_CloseList(room, NULL, NULL);
            if (work->secondChoice != 0) {
                WiFiPlazaRoom_PrintMessageInstant(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00008));
                WiFiPlazaRoom_OpenYesNoMenu(room);
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_CONFIRM_SECOND_TYPE);
            } else {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_CHOSE_ONE_TYPE);
            }
        }
        break;
    case WIFI_PLAZA_ARRIVAL_CONFIRM_SECOND_TYPE:
        switch (WiFiPlazaRoom_GetYesNoResult(room)) {
        case WIFI_PLAZA_YES_NO_YES:
            WiFiPlazaRoom_CloseYesNoMenu(room);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_CHOSE_TWO_TYPES);
            WiFiPlazaWork_SetFavoriteTypes(lobby, work->firstChoice, work->secondChoice);
            WiFiPlazaRoom_RefreshPlayerCardTypes(room);
            PlaySE(SEQ_SE_DP_BUTTON3);
            break;
        case WIFI_PLAZA_YES_NO_NO:
            WiFiPlazaRoom_CloseYesNoMenu(room);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_ASK_SECOND_TYPE);
            break;
        }
        break;
    case WIFI_PLAZA_ARRIVAL_RECEIVE_TAP_TOY:
        PlaySE(SEQ_SE_PL_EFF03_4);
        WiFiPlazaRoom_StartInfoPanel(room);
        WiFiPlazaRoom_BufferPlayerName(room, index, 0);
        WiFiPlazaRoom_BufferInstrumentName(room, work->listValue, 1);
        if (WiFiPlazaWork_IsSpecialVisitor(lobby, index) == 0) {
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00001);
        } else {
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00124);
        }
        WiFiPlazaRoom_PrintMessage(room, message);
        work->nextStep = WIFI_PLAZA_ARRIVAL_ASK_TYPES;
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ARRIVAL_ENJOY_STAY:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00002));
        work->nextStep = WIFI_PLAZA_ARRIVAL_START_WALK;
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ARRIVAL_ASK_TYPES:
        if (!IsSEPlaying(SEQ_SE_PL_EFF03_4) && (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) != 0) {
            PlaySE(SEQ_SE_DP_SELECT);
            WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00004));
            work->nextStep = WIFI_PLAZA_ARRIVAL_ASK_FIRST_TYPE;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_WAIT_MESSAGE);
            WiFiPlazaWork_SetFavoriteTypes(lobby, work->firstChoice, work->secondChoice);
            WiFiPlazaRoom_ShowPlayerCard(room, index, 0);
        }
        break;
    case WIFI_PLAZA_ARRIVAL_CHOSE_ONE_TYPE:
        WiFiPlazaRoom_BufferTypeName(room, 0, work->firstChoice);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00012));
        work->nextStep = WIFI_PLAZA_ARRIVAL_ENJOY_STAY;
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ARRIVAL_CHOSE_TWO_TYPES:
        WiFiPlazaRoom_BufferTypeName(room, 0, work->firstChoice);
        WiFiPlazaRoom_BufferTypeName(room, 1, work->secondChoice);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00011));
        work->nextStep = WIFI_PLAZA_ARRIVAL_ENJOY_STAY;
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ARRIVAL_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_ARRIVAL_WAIT_MESSAGE:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, work->nextStep);
        }
        break;
    case WIFI_PLAZA_ARRIVAL_START_WALK:
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_PLAYER_INPUT);
        WiFiPlazaRoom_CloseMessage(room);
        WiFiPlazaPlayerState_FreeArrivalMenu(work, room);
        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaPlayerState_Set(WiFiPlazaRoom_GetPlayerStates(room), WiFiPlazaRoom_GetOwnPlayerIndex(room), &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    }
    return FALSE;
}

static BOOL WiFiPlazaPlayerState_OwnReturnFromAttraction(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager;
    WiFiPlazaWork *lobby;
    WiFiPlazaRoomEntry *entry;
    WiFiPlazaObj *ownPlayer;

    lobby = WiFiPlazaRoom_GetLobby(room);
    entry = WiFiPlazaRoom_GetStartParams(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_RETURN_CREATE_PLAYER:
        WiFiPlazaPlayerState_CreateOwnPlayerAt(room, index, entry->x, entry->y, DIR_SOUTH);
        if (WiFiPlaza_HasConnectionFailed() == FALSE) {
            WiFiPlazaLobbyClient_LeaveChannel();
        }
        WiFiPlazaWork_SetLowVolume(lobby, 0);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_RETURN_WAIT_FADE_IN:
        if (WiFiPlaza_HasConnectionFailed() != FALSE || WiFiPlazaLobbyClient_IsNotInChannel() != FALSE) {
            entry = WiFiPlazaRoom_GetStartParams(room);
            if (WiFiPlazaRoom_IsFading(room) == 0) {
                WiFiPlazaPlayerState_NextStep(state);
                WiFiPlazaObj_RunCommand(objManager, ownPlayer, 2, entry->direction);
            }
        }
        break;
    case WIFI_PLAZA_RETURN_WAIT_STEP_OUT:
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            WiFiPlazaRoom_SetMode(room, 1);
            if (WiFiPlazaWork_DidAppQuitForClosing(lobby) == 1) {
                WiFiPlazaPlayerState_NextStep(state);
            } else {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RETURN_START_WALK);
            }
        }
        break;
    case WIFI_PLAZA_RETURN_PRINT_NOTICE:
        WiFiPlazaRoom_BufferActivityName(room, entry->param, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00021));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_RETURN_WAIT_NOTICE:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaRoom_CloseMessage(room);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_RETURN_START_WALK:
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_PLAYER_INPUT);
        state = (WiFiPlazaPlayerState *)WiFiPlazaRoom_GetPlayerStates(room);
        WiFiPlazaPlayerState_Set((WiFiPlazaPlayerStateManager *)state, WiFiPlazaRoom_GetOwnPlayerIndex(room), &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    }

    return FALSE;
}

static BOOL WiFiPlazaPlayerState_OwnReturnFromMinigame(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaWork *lobby;
    WiFiPlazaObjManager *objManager;
    WiFiPlazaPlayerStateManager *manager;
    WiFiPlazaRoomEntry *entry;
    WiFiPlazaObj *ownPlayer;

    manager = WiFiPlazaRoom_GetPlayerStates(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
    lobby = WiFiPlazaRoom_GetLobby(room);
    entry = WiFiPlazaRoom_GetStartParams(room);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_RETURN_CREATE_PLAYER:
        WiFiPlazaPlayerState_CreateOwnPlayerAt(room, index, entry->x, entry->y, DIR_SOUTH);
        WiFiPlazaPlayerState_NextStep(state);
        WiFiPlazaPlayerState_Push(manager, index, &sWiFiPlazaPlayerStateGameEntryEndLink, NULL);
        WiFiPlazaWork_SetMinigameActiveFlag(lobby, 0);
        WiFiPlazaWork_PlayBGM(lobby);
        break;
    case WIFI_PLAZA_RETURN_WAIT_FADE_IN:
        entry = WiFiPlazaRoom_GetStartParams(room);
        if (WiFiPlazaRoom_IsFading(room) == 0) {
            WiFiPlazaPlayerState_NextStep(state);
            WiFiPlazaObj_RunCommand(objManager, ownPlayer, 2, entry->direction);
        }
        break;
    case WIFI_PLAZA_RETURN_WAIT_STEP_OUT:
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            WiFiPlazaRoom_SetMode(room, 1);
            if (WiFiPlazaWork_GetGameConnectionLost(lobby) == TRUE) {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RETURN_PRINT_NOTICE);
            } else {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RETURN_START_WALK);
            }
        }
        break;
    case WIFI_PLAZA_RETURN_PRINT_NOTICE:
        WiFiPlazaRoom_BufferActivityName(room, entry->param, 0);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00008));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_RETURN_WAIT_NOTICE:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaRoom_CloseMessage(room);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_RETURN_START_WALK:
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_PLAYER_INPUT);
        state = (WiFiPlazaPlayerState *)WiFiPlazaRoom_GetPlayerStates(room);
        WiFiPlazaPlayerState_Set((WiFiPlazaPlayerStateManager *)state, WiFiPlazaRoom_GetOwnPlayerIndex(room), &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    }

    return FALSE;
}

static BOOL WiFiPlazaPlayerState_OwnReturnFromSurvey(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    WiFiPlazaRoomEntry *entry = WiFiPlazaRoom_GetStartParams(room);
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaObj *ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_RETURN_CREATE_PLAYER:
        WiFiPlazaPlayerState_CreateOwnPlayerAt(room, index, entry->x, entry->y, DIR_SOUTH);
        WiFiPlazaWork_SetLowVolume(lobby, 0);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_RETURN_WAIT_FADE_IN:
        entry = WiFiPlazaRoom_GetStartParams(room);
        if (WiFiPlazaRoom_IsFading(room) == 0) {
            WiFiPlazaPlayerState_NextStep(state);
            WiFiPlazaObj_RunCommand(objManager, ownPlayer, 2, entry->direction);
        }
        break;
    case WIFI_PLAZA_RETURN_WAIT_STEP_OUT:
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            WiFiPlazaRoom_SetMode(room, 1);
            if (WiFiPlazaWork_DidAppQuitForClosing(lobby) == 1) {
                WiFiPlazaPlayerState_NextStep(state);
            } else {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RETURN_START_WALK);
            }
        }
        break;
    case WIFI_PLAZA_RETURN_PRINT_NOTICE:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00091));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_RETURN_WAIT_NOTICE:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaRoom_CloseMessage(room);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_RETURN_START_WALK:
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_PLAYER_INPUT);
        state = (WiFiPlazaPlayerState *)WiFiPlazaRoom_GetPlayerStates(room);
        WiFiPlazaPlayerState_Set((WiFiPlazaPlayerStateManager *)state, WiFiPlazaRoom_GetOwnPlayerIndex(room), &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    }

    return FALSE;
}

// The own player walks around: map tile actions, talking to other players, the profile panel (X)
static BOOL WiFiPlazaPlayerState_OwnWalk(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    int *savedHandler = WiFiPlazaPlayerState_GetWork(state);
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_WALK_INIT:
        WiFiPlazaPlayerState_AllocWork(state, sizeof(int));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_WALK_UPDATE: {
        int moveState;
        WiFiPlazaObj *ownPlayer;
        int tile;
        WiFiPlazaModels *models;
        WiFiPlazaMap *map;
        WiFiPlazaWork *lobby;
        WiFiPlazaPlayerStateManager *manager;
        int frontY, frontX, direction, frontTile, selected;
        WiFiPlazaObjManager *objManager;
        WiFiPlazaObj *other;
        Field2DPoint pos, frontPos;
        WiFiPlazaRoom_SetPanelButtonDisabled(room, 0);
        manager = WiFiPlazaRoom_GetPlayerStates(room);
        lobby = WiFiPlazaRoom_GetLobby(room);
        objManager = WiFiPlazaRoom_GetObjManager(room);
        map = WiFiPlazaRoom_GetMap(room);
        models = WiFiPlazaRoom_GetModels(room);
        other = WiFiPlazaObj_GetSpecial(objManager);
        if (other) {
            if (!WiFiPlazaWork_IsClosing(lobby) && WiFiPlazaWork_IsLightingStarted(lobby) == TRUE && !WiFiPlazaWork_GetEventScheduleHigh(lobby)) {
                if (WiFiPlazaObj_GetHandler(other) != WIFI_PLAZA_OBJ_HANDLER_HOP) {
                    WiFiPlazaObj_SetHandler(objManager, other, WIFI_PLAZA_OBJ_HANDLER_HOP);
                }
            } else if (WiFiPlazaObj_GetHandler(other) != WIFI_PLAZA_OBJ_HANDLER_IDLE) {
                WiFiPlazaObj_SetHandler(objManager, other, WIFI_PLAZA_OBJ_HANDLER_IDLE);
            }
        }
        ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
        moveState = WiFiPlazaObj_GetParam(ownPlayer, 5);
        direction = WiFiPlazaObj_GetParam(ownPlayer, 6);
        pos = WiFiPlazaObj_GetGridPos(ownPlayer);
        frontPos = Field2D_StepInDirection(pos, direction);
        tile = WiFiPlazaMap_GetTileAttr(map, (u16)(pos.x / 16), (u16)(pos.y / 16));
        frontY = frontPos.y / 16;
        frontX = frontPos.x / 16;
        frontTile = WiFiPlazaMap_GetTileAttr(map, (u16)frontX, (u16)frontY);
        selected = WiFiPlazaRoom_GetPlayerCardPlayer(room);
        if (selected != index && WiFiPlazaRoom_IsCardBlocked(room, (u8)selected)) {
            WiFiPlazaRoom_ShowOwnCard(room);
        }
        if (moveState) {
            if (WiFiPlazaRoom_IsPlayerCardIdle(room) == TRUE && (u32)(moveState - 1) <= 2) {
                selected = WiFiPlazaRoom_GetPlayerCardPlayer(room);
                if (selected != index) {
                    other = WiFiPlazaObj_FindById(objManager, selected);
                    if (other && !WiFiPlazaObj_GetHandler(other)) {
                        WiFiPlazaObj_SetHandler(objManager, other, *savedHandler);
                    }
                }
                WiFiPlazaRoom_ShowOwnCard(room);
            }
            break;
        }
        WiFiPlazaRoom_SetPanelButtonDisabled(room, 0);
        if (WiFiPlazaMap_IsStandActionTile(tile) == TRUE) {
            WiFiPlazaPlayerState_RunTileAction(state, room, index, sWiFiPlazaStandTileActions, tile);
            WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_IDLE);
            WiFiPlazaRoom_SetActionTile(room, (u8)tile);
            WiFiPlazaRoom_SetPanelButtonDisabled(room, 1);
            return FALSE;
        }
        if (WiFiPlazaRoom_TakePanelButtonPress(room) == TRUE) {
            WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_IDLE);
            WiFiPlazaPlayerState_Push(manager, index, &sWiFiPlazaPlayerStatePlayInstrument, NULL);
            WiFiPlazaRoom_SetPanelButtonDisabled(room, 1);
            return FALSE;
        }
        if (WiFiPlazaWork_WasAskedToTalk(lobby)) {
            WiFiPlazaPlayerState_Push(manager, index, &sWiFiPlazaPlayerStateTalkAnswer, NULL);
            WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_IDLE);
            WiFiPlazaRoom_SetPanelButtonDisabled(room, 1);
            return FALSE;
        }
        if (gSystem.newKeys & PAD_BUTTON_A) {
            WiFiPlazaObj *facing = WiFiPlazaObj_FindFacing(objManager, ownPlayer);
            if (facing) {
                int facingMoveState = WiFiPlazaObj_GetParam(facing, 5);
                int id = WiFiPlazaObj_GetParam(facing, 4);
                if (id != 254) {
                    int busy = WiFiPlazaRoom_IsPlayerBusy(room, (u8)id);
                    if (!facingMoveState && !busy) {
                        WiFiPlazaPlayerState_Push(manager, index, &sWiFiPlazaPlayerStateTalkStart, NULL);
                        WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_IDLE);
                        WiFiPlazaObj_SetHandler(objManager, facing, WIFI_PLAZA_OBJ_HANDLER_IDLE);
                        WiFiPlazaRoom_SetPanelButtonDisabled(room, 1);
                        return FALSE;
                    }
                }
            }
            if (WiFiPlazaMap_IsPropTile(frontTile) == TRUE && !WiFiPlazaObj_GetParam(ownPlayer, 6)) {
                WiFiPlazaModels_TouchPropsAt(models, (u8)frontX, (u8)frontY);
                WiFiPlazaRoom_SetActionTile(room, (u8)frontTile);
                WiFiPlazaRoom_SetPanelButtonDisabled(room, 1);
                return FALSE;
            }
            if (WiFiPlazaMap_IsCheckActionTile(frontTile) == TRUE) {
                WiFiPlazaPlayerState_RunTileAction(state, room, index, sWiFiPlazaCheckTileActions, frontTile);
                WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_IDLE);
                WiFiPlazaRoom_SetActionTile(room, (u8)frontTile);
                WiFiPlazaRoom_SetPanelButtonDisabled(room, 1);
                return FALSE;
            }
        }
        if (WiFiPlazaPlayerState_IsDirectionHeld(direction) == TRUE) {
            WiFiPlazaObj *facing;
            if (!direction && WiFiPlazaMap_IsWalkUpActionTile(frontTile) == TRUE) {
                WiFiPlazaPlayerState_RunTileAction(state, room, index, sWiFiPlazaWalkUpTileActions, frontTile);
                WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_IDLE);
                WiFiPlazaRoom_SetActionTile(room, (u8)frontTile);
                WiFiPlazaRoom_SetPanelButtonDisabled(room, 1);
                return FALSE;
            }
            facing = WiFiPlazaObj_FindFacing(objManager, ownPlayer);
            if (facing) {
                int id = WiFiPlazaObj_GetParam(facing, 4);
                if (id != 254 && !WiFiPlazaRoom_IsPlayerCardShown(room) && !WiFiPlazaRoom_IsCardBlocked(room, (u8)id)) {
                    WiFiPlazaRoom_ShowPlayerCard(room, id, 1);
                    *savedHandler = WiFiPlazaObj_GetHandler(facing);
                    WiFiPlazaObj_SetHandler(objManager, facing, WIFI_PLAZA_OBJ_HANDLER_IDLE);
                    return FALSE;
                }
            }
        }
        if (gSystem.newKeys & PAD_BUTTON_X) {
            if (!WiFiPlazaRoom_IsPlayerCardShown(room)) {
                WiFiPlazaRoom_ShowPlayerCard(room, WiFiPlazaWork_GetOwnSlot(lobby), 0);
                PlaySE(SEQ_SE_DP_SELECT);
            } else {
                int current = WiFiPlazaRoom_GetPlayerCardPlayer(room);
                if (current == index) {
                    WiFiPlazaRoom_ShowOwnCard(room);
                    PlaySE(SEQ_SE_DP_SELECT);
                }
            }
            return FALSE;
        }
        break;
    }
    }
    return FALSE;
}

static BOOL WiFiPlazaPlayerState_OwnWalkLate(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    if (WiFiPlazaObj_GetParam(WiFiPlazaObj_GetOwnPlayer(WiFiPlazaRoom_GetObjManager(room)), 5)) {
        WiFiPlazaRoom_SetPanelButtonDisabled(room, 1);
    }
    return FALSE;
}

// Exit tile: asks whether to leave the plaza
static BOOL WiFiPlazaPlayerState_OwnExit(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager;
    WiFiPlazaObj *ownPlayer;
    int closed;

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_EXIT_ASK_EXIT:
        objManager = WiFiPlazaRoom_GetObjManager(room);
        ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_IDLE);
        WiFiPlazaWork_SetActivity(WiFiPlazaRoom_GetLobby(room), WIFI_PLAZA_ACTIVITY_EXIT);
        PlaySE(SEQ_SE_DP_DECIDE);
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00065));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_EXIT_WAIT_ASK_EXIT:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_EXIT_ASK_DISCONNECT:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 2, msg_0800_00025));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_EXIT_WAIT_ASK_DISCONNECT:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_EXIT_OPEN_YES_NO:
        WiFiPlazaRoom_OpenYesNoMenuDefaultNo(room);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_EXIT_HANDLE_YES_NO:
        closed = FALSE;
        switch (WiFiPlazaRoom_GetYesNoResult(room)) {
        case WIFI_PLAZA_YES_NO_YES:
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_EXIT_RISE_OUT);
            closed = TRUE;
            break;
        case WIFI_PLAZA_YES_NO_NO:
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_EXIT_STEP_BACK);
            closed = TRUE;
            break;
        case WIFI_PLAZA_YES_NO_OPEN:
            break;
        }
        if (closed == TRUE) {
            WiFiPlazaRoom_CloseYesNoMenu(room);
            WiFiPlazaRoom_CloseMessage(room);
        }
        break;
    case WIFI_PLAZA_EXIT_RISE_OUT:
        WiFiPlazaRoom_SetMode(room, 0);
        objManager = WiFiPlazaRoom_GetObjManager(room);
        ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
        WiFiPlazaCamera_ClearFollowObject(WiFiPlazaRoom_GetCamera(room));
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_RISE_OUT);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_EXIT_WAIT_RISE_OUT);
        break;
    case WIFI_PLAZA_EXIT_WAIT_RISE_OUT:
        if (WiFiPlazaObj_IsFinished(WiFiPlazaObj_GetOwnPlayer(WiFiPlazaRoom_GetObjManager(room))) == TRUE) {
            WiFiPlazaRoom_SetFinished(room, 1);
            WiFiPlazaRoom_SetResult(room, WIFI_PLAZA_ROOM_RESULT_DISCONNECT);
            WiFiPlazaWork_SetActivity(WiFiPlazaRoom_GetLobby(room), WIFI_PLAZA_ACTIVITY_EXIT);
            return TRUE;
        }
        break;
    case WIFI_PLAZA_EXIT_STEP_BACK:
        objManager = WiFiPlazaRoom_GetObjManager(room);
        ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
        WiFiPlazaObj_RunCommand(objManager, ownPlayer, 2, DIR_NORTH);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_EXIT_WAIT_STEP_BACK:
        WiFiPlazaWork_SetActivity(WiFiPlazaRoom_GetLobby(room), WIFI_PLAZA_ACTIVITY_FREE);
        objManager = WiFiPlazaRoom_GetObjManager(room);
        state = (WiFiPlazaPlayerState *)WiFiPlazaRoom_GetPlayerStates(room);
        ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_PLAYER_INPUT);
            WiFiPlazaPlayerState_Set((WiFiPlazaPlayerStateManager *)state, index, &sWiFiPlazaPlayerStateOwnWalk, NULL);
        }
        break;
    }
    return FALSE;
}

static BOOL WiFiPlazaPlayerState_OwnEnterAttraction(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaObj *obj = WiFiPlazaObj_FindById(objManager, index);
    WiFiPlazaWork *lobby;
    int tile = WiFiPlazaRoom_GetActionTile(room);
    WiFiPlazaAttractionWork *work = WiFiPlazaPlayerState_GetWork(state);
    lobby = WiFiPlazaRoom_GetLobby(room);
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_ATTRACTION_JOIN: {
        u8 kind;
        work = WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaAttractionWork));
        switch ((u8)WiFiPlazaPlayerState_GetAttractionSlot(tile)) {
        case 0:
            work->mode = 3;
            break;
        case 1:
            work->mode = 4;
            break;
        case 2:
            work->mode = 5;
            break;
        case 3:
            work->mode = 6;
            break;
        default:
            GF_AssertFail();
            work->mode = 5;
            break;
        }
        if (WiFiPlazaWork_IsClosing(lobby) == 1) {
            work->msgId = msg_0776_00022;
            work->msgBank = 0;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ATTRACTION_STEP_BACK);
        } else if (WiFiPlazaWork_TakeExcessFlood(lobby) == 1) {
            work->msgId = msg_0777_00072;
            work->msgBank = 1;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ATTRACTION_STEP_BACK);
        } else {
            kind = WiFiPlazaPlayerState_GetAttractionSlot(tile);
            if (!WiFiPlazaLobbyClient_CanJoinChannel(kind)) {
                work->msgId = msg_0776_00020;
                work->msgBank = 0;
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ATTRACTION_SLIDE_BACK);
            } else {
                if (!WiFiPlazaLobbyClient_JoinChannel(kind)) {
                    GF_AssertFail();
                }
                WiFiPlazaPlayerState_NextStep(state);
            }
        }
        break;
    }
    case WIFI_PLAZA_ATTRACTION_WAIT_JOIN: {
        int result = WiFiPlazaLobbyClient_GetJoinChannelResult();
        if (result == 1) {
            WiFiPlazaWork *lobby2;
            u8 value;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ATTRACTION_ENTER);
            {
                u8 players[4] = { 0, 0, 0, 0 };
                lobby2 = WiFiPlazaRoom_GetLobby(room);
                value = WiFiPlaza_GetChannelPlayerCount(lobby2);
                if ((u32)(work->mode - 5) <= 1) {
                    int i;
                    u8 count = 0;
                    for (i = 0; i < 4; i++) {
                        int player = WiFiPlaza_ChannelSlotToSlot(lobby2, i);
                        if (player != -1) {
                            if (count < 4) {
                                players[count] = player;
                            }
                            count++;
                        }
                    }
                } else {
                    players[0] = index;
                }
                WiFiPlaza_SendActivityNews(lobby2, work->mode, value, players[0], players[1], players[2], players[3], 0);
            }
        } else if (result == 2) {
            work->msgId = msg_0776_00020;
            work->msgBank = 0;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ATTRACTION_SLIDE_BACK);
        }
        break;
    }
    case WIFI_PLAZA_ATTRACTION_ENTER: {
        int result = WiFiPlazaPlayerState_GetAttractionResult(tile), lobbyState = WiFiPlazaPlayerState_GetAttractionLobbyState(tile);
        int count, direction;
        WiFiPlazaRoomEntry *entry;
        Field2DPoint pos;
        WiFiPlazaRoom_SetFinished(room, 1);
        WiFiPlazaRoom_SetResult(room, (u8)result);
        WiFiPlazaWork_SetActivity(WiFiPlazaRoom_GetLobby(room), lobbyState);
        switch (work->mode) {
        case 3:
        case 4:
            count = 5;
            break;
        case 5:
            count = 3;
            break;
        case 6:
            count = 4;
            break;
        default:
            GF_AssertFail();
            break;
        }
        WiFiPlazaWork_MarkActivityDone(lobby, count);
        direction = Field2D_GetOppositeDirection(WiFiPlazaObj_GetParam(obj, 6));
        entry = WiFiPlazaRoom_GetStartParams(room);
        pos = WiFiPlazaObj_GetGridPos(obj);
        entry->entryMode = WIFI_PLAZA_ENTRY_FROM_ATTRACTION;
        entry->x = pos.x / 16;
        entry->y = pos.y / 16;
        entry->direction = direction;
        entry->param = work->mode;
        WiFiPlazaPlayerState_FreeWork(state);
        return TRUE;
    }
    case WIFI_PLAZA_ATTRACTION_SLIDE_BACK: {
        int handler;
        switch ((u8)WiFiPlazaObj_GetParam(obj, 6)) {
        case DIR_NORTH:
            handler = WIFI_PLAZA_OBJ_HANDLER_SLIDE_DOWN;
            break;
        case DIR_SOUTH:
            handler = WIFI_PLAZA_OBJ_HANDLER_SLIDE_UP;
            break;
        case DIR_WEST:
            handler = WIFI_PLAZA_OBJ_HANDLER_SLIDE_RIGHT;
            break;
        case DIR_EAST:
            handler = WIFI_PLAZA_OBJ_HANDLER_SLIDE_LEFT;
            break;
        default:
            GF_AssertFail();
            break;
        }
        WiFiPlazaObj_SetHandler(objManager, obj, handler);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    }
    case WIFI_PLAZA_ATTRACTION_WAIT_SLIDE_BACK:
        if (WiFiPlazaObj_IsFinished(obj) == TRUE) {
            work->timer = 8;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ATTRACTION_PRINT_REFUSAL);
        }
        break;
    case WIFI_PLAZA_ATTRACTION_STEP_BACK: {
        u8 direction = ((int (*)(s32))Field2D_GetOppositeDirection)((u8)WiFiPlazaObj_GetParam(obj, 6));
        WiFiPlazaObj_RunCommand(objManager, obj, 2, direction);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    }
    case WIFI_PLAZA_ATTRACTION_WAIT_STEP_BACK:
        if (!WiFiPlazaObj_GetParam(obj, 5)) {
            work->timer = 8;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_ATTRACTION_PRINT_REFUSAL);
        }
        break;
    case WIFI_PLAZA_ATTRACTION_PRINT_REFUSAL:
        if (--work->timer <= 0) {
            WiFiPlazaRoom_BufferActivityName(room, work->mode, 0);
            WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, work->msgBank, work->msgId));
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_ATTRACTION_WAIT_REFUSAL:
        if (WiFiPlazaRoom_IsMessageFinished(room)) {
            WiFiPlazaPlayerState_FreeWork(state);
            WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_PLAYER_INPUT);
            WiFiPlazaRoom_CloseMessage(room);
            WiFiPlazaPlayerState_Set(WiFiPlazaRoom_GetPlayerStates(room), index, &sWiFiPlazaPlayerStateOwnWalk, NULL);
        }
        break;
    }
    return FALSE;
}

static BOOL WiFiPlazaPlayerState_OwnEnterMinigame(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaPlayerStateResult *result = WiFiPlazaPlayerState_GetWork(state);
    WiFiPlazaPlayerStateManager *manager = WiFiPlazaRoom_GetPlayerStates(room);
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaObj *ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    int value;
    int choice;

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_GAME_BOOTH_REGISTER: {
        u8 *selection = WiFiPlazaPlayerState_AllocWork(state, 4);
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_IDLE);
        WiFiPlazaPlayerState_NextStep(state);
        selection[0] = 0;
        switch (WiFiPlazaRoom_GetActionTile(room)) {
        case 30:
            selection[1] = 0;
            selection[2] = 0;
            selection[3] = WIFI_PLAZA_ACTIVITY_SWALOT_PLOP;
            break;
        case 31:
            selection[1] = 1;
            selection[2] = 1;
            selection[3] = WIFI_PLAZA_ACTIVITY_MIME_JR_TOP;
            break;
        case 32:
            selection[1] = 2;
            selection[2] = 2;
            selection[3] = WIFI_PLAZA_ACTIVITY_WOBBUFFET_POP;
            break;
        default:
            GF_AssertFail();
            break;
        }
        WiFiPlazaWork_SetActivity(WiFiPlazaRoom_GetLobby(room), WIFI_PLAZA_ACTIVITY_GAME_ENTRY);
        WiFiPlazaPlayerState_Push(manager, index, &sWiFiPlazaPlayerStateGameEntryRegister, selection);
        break;
    }
    case WIFI_PLAZA_GAME_BOOTH_CHECK_RESULT:
        if (result->result == WIFI_PLAZA_GAME_ENTRY_STARTED) {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_GAME_BOOTH_START_GAME);
        } else {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_GAME_BOOTH_STEP_BACK);
        }
        break;
    case WIFI_PLAZA_GAME_BOOTH_START_GAME: {
        WiFiPlazaRoomEntry *entry;
        Field2DPoint position;
        Field2DPoint current;
        u32 packed;
        u32 direction;
        switch (WiFiPlazaRoom_GetActionTile(room)) {
        case 30:
            value = WIFI_PLAZA_ROOM_RESULT_SWALOT_PLOP;
            choice = 0;
            break;
        case 31:
            value = WIFI_PLAZA_ROOM_RESULT_MIME_JR_TOP;
            choice = 1;
            break;
        case 32:
            value = WIFI_PLAZA_ROOM_RESULT_WOBBUFFET_POP;
            choice = 2;
            break;
        }
        WiFiPlazaRoom_SetFinished(room, 1);
        WiFiPlazaRoom_SetResult(room, (u8)value);
        IncrementGameStat119(lobby);
        WiFiPlazaWork_MarkActivityDone(lobby, choice);
        direction = Field2D_GetOppositeDirection(WiFiPlazaObj_GetParam(ownPlayer, 6));
        entry = WiFiPlazaRoom_GetStartParams(room);
        packed = ((u32(*)(WiFiPlazaObj *))WiFiPlazaObj_GetGridPos)(ownPlayer);
        current.x = packed;
        current.y = packed >> 16;
        position = current;
        entry->entryMode = WIFI_PLAZA_ENTRY_FROM_GAME;
        entry->x = position.x / 16;
        entry->y = position.y / 16;
        entry->direction = direction;
        entry->param = result->param;
        WiFiPlazaWork_SetMinigameActiveFlag(WiFiPlazaRoom_GetLobby(room), 1);
        WiFiPlazaPlayerState_FreeWork(state);
        return TRUE;
    }
    case WIFI_PLAZA_GAME_BOOTH_STEP_BACK:
        WiFiPlazaObj_RunCommand(objManager, ownPlayer, 2, Field2D_GetOppositeDirection(WiFiPlazaObj_GetParam(ownPlayer, 6)));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_GAME_BOOTH_WAIT_STEP_BACK:
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_GAME_BOOTH_PRINT_RESULT: {
        BOOL valid = TRUE;
        switch (result->result) {
        case WIFI_PLAZA_GAME_ENTRY_IN_PLAY:
            WiFiPlazaRoom_BufferActivityName(room, result->param, 0);
            value = msg_0776_00004;
            break;
        case WIFI_PLAZA_GAME_ENTRY_SESSION_FULL:
            value = msg_0776_00005;
            break;
        case WIFI_PLAZA_GAME_ENTRY_PLAZA_CLOSING:
            WiFiPlazaRoom_BufferActivityName(room, result->param, 0);
            value = msg_0776_00006;
            break;
        case WIFI_PLAZA_GAME_ENTRY_NO_MEMBERS:
            value = msg_0776_00019;
            break;
        case WIFI_PLAZA_GAME_ENTRY_LINK_ERROR:
            value = msg_0776_00017;
            break;
        case WIFI_PLAZA_GAME_ENTRY_DROPPED_OUT:
            value = msg_0776_00029;
            break;
        case WIFI_PLAZA_GAME_ENTRY_NOT_READY:
            value = msg_0776_00007;
            break;
        default:
            valid = FALSE;
            break;
        }
        if (valid) {
            WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, value));
            WiFiPlazaPlayerState_NextStep(state);
        } else {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_GAME_BOOTH_END);
        }
        break;
    }
    case WIFI_PLAZA_GAME_BOOTH_WAIT_RESULT:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaRoom_CloseMessage(room);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_GAME_BOOTH_END:
        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaWork_SetActivity(WiFiPlazaRoom_GetLobby(room), WIFI_PLAZA_ACTIVITY_FREE);
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_PLAYER_INPUT);
        WiFiPlazaPlayerState_Set(WiFiPlazaRoom_GetPlayerStates(room), index, &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    }
    return FALSE;
}

static BOOL WiFiPlazaPlayerState_OwnUseSurveyMonitor(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    u16 *work = WiFiPlazaPlayerState_GetWork(state);
    WiFiPlazaObjManager *objManager;
    WiFiPlazaObj *ownPlayer;
    WiFiPlazaWork *lobby;
    WiFiPlazaRoomEntry *entry;
    int value;
    u32 packed;
    Field2DPoint position;
    Field2DPoint current;

    WiFiPlazaRoom_GetPlayerStates(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
    lobby = WiFiPlazaRoom_GetLobby(room);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_SURVEY_MONITOR_CHECK: {
        u16 *newWork = WiFiPlazaPlayerState_AllocWork(state, 12);
        WiFiPlazaWork_SetActivity(WiFiPlazaRoom_GetLobby(room), WIFI_PLAZA_ACTIVITY_SURVEY);
        if (WiFiPlazaWork_IsClosing(lobby) == 1) {
            newWork[0] = msg_0777_00122;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_SURVEY_MONITOR_STEP_BACK);
            break;
        }
        if (WiFiPlazaWork_IsLightingStarted(lobby) == TRUE) {
            newWork[1] = WIFI_PLAZA_ROOM_RESULT_SURVEY_RESULTS;
        } else if (WiFiPlazaWork_HasAnsweredSurvey(lobby) == 1) {
            newWork[0] = msg_0777_00098;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_SURVEY_MONITOR_STEP_BACK);
            break;
        } else {
            newWork[1] = WIFI_PLAZA_ROOM_RESULT_SURVEY_ANSWER;
        }
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_SURVEY_MONITOR_ENTER);
        break;
    }
    case WIFI_PLAZA_SURVEY_MONITOR_ENTER:
        if (work[1] == WIFI_PLAZA_ROOM_RESULT_SURVEY_ANSWER) {
            value = WIFI_PLAZA_ROOM_RESULT_SURVEY_ANSWER;
        } else {
            value = WIFI_PLAZA_ROOM_RESULT_SURVEY_RESULTS;
            WiFiPlazaWork_SetEventScheduleHigh(lobby);
        }
        WiFiPlazaRoom_SetFinished(room, 1);
        WiFiPlazaRoom_SetResult(room, (u8)value);
        WiFiPlazaWork_AddHistory(lobby, 23, -1);
        value = Field2D_GetOppositeDirection(WiFiPlazaObj_GetParam(ownPlayer, 6));
        entry = WiFiPlazaRoom_GetStartParams(room);
        packed = ((u32(*)(WiFiPlazaObj *))WiFiPlazaObj_GetGridPos)(ownPlayer);
        current.x = packed;
        current.y = packed >> 16;
        position = current;
        entry->entryMode = WIFI_PLAZA_ENTRY_FROM_SURVEY;
        entry->x = position.x / 16;
        entry->y = position.y / 16;
        entry->direction = value;
        entry->param = 0;
        WiFiPlazaPlayerState_FreeWork(state);
        return TRUE;
    case WIFI_PLAZA_SURVEY_MONITOR_STEP_BACK:
        WiFiPlazaObj_RunCommand(objManager, ownPlayer, 2, Field2D_GetOppositeDirection(WiFiPlazaObj_GetParam(ownPlayer, 6)));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_SURVEY_MONITOR_WAIT_STEP_BACK:
        if (WiFiPlazaObj_GetParam(ownPlayer, 5) == 0) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_SURVEY_MONITOR_PRINT_MESSAGE:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 1, work[0]));
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_SURVEY_MONITOR_WAIT_MESSAGE:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaRoom_CloseMessage(room);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_SURVEY_MONITOR_END:
        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaWork_SetActivity(WiFiPlazaRoom_GetLobby(room), WIFI_PLAZA_ACTIVITY_FREE);
        WiFiPlazaObj_SetHandler(objManager, ownPlayer, WIFI_PLAZA_OBJ_HANDLER_PLAYER_INPUT);
        WiFiPlazaPlayerState_Set(WiFiPlazaRoom_GetPlayerStates(room), index, &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

static int WiFiPlazaPlayerState_GetAttractionSlot(int tile) {
    switch (tile) {
    case 22:
    case 23:
    case 24:
    case 25:
        return 2;
    case 26:
    case 27:
    case 28:
    case 29:
        return 3;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        return 0;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        return 1;
    }
    GF_AssertFail();
    return 0;
}

static int WiFiPlazaPlayerState_GetAttractionResult(int tile) {
    switch (tile) {
    case 22:
    case 23:
    case 24:
    case 25:
        return WIFI_PLAZA_ROOM_RESULT_VISITOR_PROFILES;
    case 26:
    case 27:
    case 28:
    case 29:
        return WIFI_PLAZA_ROOM_RESULT_PLAZA_NEWS;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        return WIFI_PLAZA_ROOM_RESULT_FOOTPRINT_STAMP_BOARD_0;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        return WIFI_PLAZA_ROOM_RESULT_FOOTPRINT_STAMP_BOARD_1;
    }
    GF_AssertFail();
    return WIFI_PLAZA_ROOM_RESULT_DISCONNECT;
}

static int WiFiPlazaPlayerState_GetAttractionLobbyState(int tile) {
    switch (tile) {
    case 22:
    case 23:
    case 24:
    case 25:
        return WIFI_PLAZA_ACTIVITY_VISITOR_PROFILES;
    case 26:
    case 27:
    case 28:
    case 29:
        return WIFI_PLAZA_ACTIVITY_PLAZA_NEWS;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        return WIFI_PLAZA_ACTIVITY_FOOTPRINT_STAMP_BOARD_0;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        return WIFI_PLAZA_ACTIVITY_FOOTPRINT_STAMP_BOARD_1;
    }
    GF_AssertFail();
    return WIFI_PLAZA_ACTIVITY_NONE;
}

static BOOL WiFiPlazaTalk_Greeter(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    String *message;
    void *object;
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_GREETER_PRINT:
        object = WiFiPlazaRoom_GetLobby(room);
        if (WiFiPlazaWork_IsClosing(object) == 1) {
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00003);
        } else if (WiFiPlazaWork_IsParadeStarted(object) == TRUE) {
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00078);
        } else {
            message = WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00002);
        }
        WiFiPlazaRoom_PrintMessage(room, message);
        PlaySE(SEQ_SE_DP_SELECT);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_GREETER_WAIT_MESSAGE:
        if (WiFiPlazaRoom_IsMessageFinished(room)) {
            WiFiPlazaRoom_CloseMessage(room);
            object = WiFiPlazaRoom_GetObjManager(room);
            WiFiPlazaObj_SetHandler(object, WiFiPlazaObj_GetOwnPlayer(object), WIFI_PLAZA_OBJ_HANDLER_PLAYER_INPUT);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// The own player jumps onto a mover and rides it until the room closes
static BOOL WiFiPlazaPlayerState_OwnRideMover(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaModels *models = WiFiPlazaRoom_GetModels(room);
    WiFiPlazaCamera *camera = WiFiPlazaRoom_GetCamera(room);
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    WiFiPlazaPlayerRide *ride = WiFiPlazaPlayerState_GetWork(state);
    VecFx32 pos;
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_RIDE_INIT:
        ride = WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaPlayerRide));
        ride->obj = WiFiPlazaObj_FindById(objManager, index);
        switch (WiFiPlazaRoom_GetActionTile(room)) {
        case 33:
            ride->choice = 2;
            break;
        case 34:
            ride->choice = 1;
            break;
        case 35:
            ride->choice = 0;
            break;
        default:
            GF_AssertFail();
            break;
        }
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_JOIN_GROUP);
        break;
    case WIFI_PLAZA_RIDE_JOIN_GROUP:
        if (!WiFiPlazaWork_IsParadeStarted(lobby)) {
            ride->nextStep = WIFI_PLAZA_RIDE_PARADE_LATER;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_STEP_BACK);
        } else if (WiFiPlazaWork_IsClosing(lobby) == 1) {
            ride->nextStep = WIFI_PLAZA_RIDE_PARADE_OVER;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_STEP_BACK);
        } else {
            ride->mover = WiFiPlazaWork_JoinGroup(lobby, index, ride->choice);
            if (ride->mover != -1) {
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_WAIT_FOR_FLOAT);
                WiFiPlazaRoom_PrintMessageInstant(room, WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00031));
                WiFiPlazaRoom_ShowWaitingIcon(room);
            } else {
                ride->nextStep = WIFI_PLAZA_RIDE_FLOATS_FULL;
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_STEP_BACK);
            }
        }
        break;
    case WIFI_PLAZA_RIDE_WAIT_FOR_FLOAT:
        if ((gSystem.newKeys & PAD_BUTTON_B) || (gSystem.newKeys & PAD_KEY_DOWN)) {
            WiFiPlazaWork_LeaveGroup(lobby, index);
            WiFiPlazaRoom_HideWaitingIcon(room);
            ride->resumeStep = WIFI_PLAZA_RIDE_STEP_BACK;
            ride->nextStep = WIFI_PLAZA_RIDE_END;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_CLOSE_MESSAGE);
        } else if (WiFiPlazaPlayerRide_IsMoverHere(ride, models) == TRUE) {
            WiFiPlazaRoom_HideWaitingIcon(room);
            WiFiPlazaWork_SendJoinGroup(lobby);
            ride->timer = 0;
            WiFiPlazaWork_SetActivity(WiFiPlazaRoom_GetLobby(room), WIFI_PLAZA_ACTIVITY_PARADE_FLOAT);
            WiFiPlazaRoom_IgnorePlazaClosing(room);
            WiFiPlazaCamera_ClearFollowObject(camera);
            WiFiPlazaObj_SetAvatarMode(ride->obj, 0);
            WiFiPlazaObj_SetPoseEffect(ride->obj, 1);
            PlaySE(SEQ_SE_PL_DANSA5);
            WiFiPlazaRoom_ShowInstrumentPanel(room);
            ride->resumeStep = WIFI_PLAZA_RIDE_JUMP_ON;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_CLOSE_MESSAGE);
        }
        break;
    case WIFI_PLAZA_RIDE_JUMP_ON:
        if (WiFiPlazaPlayerRide_JumpOn(ride, models, camera) == TRUE) {
            WiFiPlazaObj_SetPoseEffect(ride->obj, 0);
            WiFiPlazaObj_SetDirection(ride->obj, 2);
            WiFiPlazaObj_SetLightFlags(ride->obj, 8);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_LAND);
            ride->timer = 0;
        }
        break;
    case WIFI_PLAZA_RIDE_LAND:
        WiFiPlazaPlayerRide_FollowMover(ride, models, room);
        WiFiPlazaPlayerRide_FollowCamera(ride, camera);
        if (WiFiPlazaPlayerRide_Land(ride, models) == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_RIDE);
            WiFiPlazaRoom_SetPanelButtonDisabled(room, 0);
        }
        break;
    case WIFI_PLAZA_RIDE_RIDE:
        WiFiPlazaPlayerRide_FollowMover(ride, models, room);
        WiFiPlazaPlayerRide_FollowCamera(ride, camera);
        WiFiPlazaObj_GetPosition(ride->obj, &pos);
        WiFiPlazaPlayerRide_FollowMoverBounce(ride, models, room);
        if (pos.x < FX32_CONST(96)) {
            ride->ticks = 0;
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_RIDE_OUT);
        }
        break;
    case WIFI_PLAZA_RIDE_STEP_BACK:
        WiFiPlazaObj_RunCommand(objManager, ride->obj, 2, DIR_SOUTH);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_WAIT_STEP_BACK);
        break;
    case WIFI_PLAZA_RIDE_WAIT_STEP_BACK:
        if (!WiFiPlazaObj_GetParam(ride->obj, 5)) {
            WiFiPlazaPlayerState_SetStep(state, ride->nextStep);
        }
        break;
    case WIFI_PLAZA_RIDE_PARADE_LATER:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00030));
        ride->nextStep = WIFI_PLAZA_RIDE_END;
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_RIDE_FLOATS_FULL:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00032));
        ride->nextStep = WIFI_PLAZA_RIDE_END;
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_RIDE_PARADE_OVER:
        WiFiPlazaRoom_PrintMessage(room, WiFiPlazaRoom_GetMsgString(room, 0, msg_0776_00033));
        ride->nextStep = WIFI_PLAZA_RIDE_END;
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_WAIT_MESSAGE);
        break;
    case WIFI_PLAZA_RIDE_WAIT_MESSAGE:
        if (WiFiPlazaRoom_IsMessageFinished(room) == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, ride->nextStep);
        }
        break;
    case WIFI_PLAZA_RIDE_CLOSE_MESSAGE:
        WiFiPlazaRoom_CloseMessage(room);
        WiFiPlazaPlayerState_SetStep(state, ride->resumeStep);
        break;
    case WIFI_PLAZA_RIDE_RIDE_OUT:
        WiFiPlazaPlayerRide_FollowMover(ride, models, room);
        WiFiPlazaPlayerRide_FollowMoverBounce(ride, models, room);
        ride->ticks++;
        if (ride->ticks > 120) {
            WiFiPlazaRoom_SetFinished(room, 1);
            WiFiPlazaRoom_SetResult(room, WIFI_PLAZA_ROOM_RESULT_DISCONNECT);
            WiFiPlazaRoom_CloseMessage(room);
            WiFiPlazaWork_SetActivity(WiFiPlazaRoom_GetLobby(room), WIFI_PLAZA_ACTIVITY_EXIT);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_RIDE_FINISHED);
        }
        break;
    case WIFI_PLAZA_RIDE_FINISHED:
        WiFiPlazaPlayerRide_FollowMover(ride, models, room);
        WiFiPlazaPlayerRide_FollowMoverBounce(ride, models, room);
        break;
    case WIFI_PLAZA_RIDE_END: {
        WiFiPlazaPlayerStateManager *manager = WiFiPlazaRoom_GetPlayerStates(room);
        WiFiPlazaRoom_CloseMessage(room);
        WiFiPlazaObj_SetHandler(objManager, ride->obj, WIFI_PLAZA_OBJ_HANDLER_PLAYER_INPUT);
        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaPlayerState_Set(manager, index, &sWiFiPlazaPlayerStateOwnWalk, NULL);
        break;
    }
    }
    return FALSE;
}

static void WiFiPlazaPlayerRide_FollowCamera(WiFiPlazaPlayerRide *ride, WiFiPlazaCamera *camera) {
    VecFx32 pos;
    WiFiPlazaObj_GetPosition(ride->obj, &pos);
    WiFiPlazaCamera_SetTarget(camera, pos.x, pos.y, pos.z);
}

static BOOL WiFiPlazaPlayerRide_IsMoverHere(WiFiPlazaPlayerRide *ride, void *models) {
    VecFx32 pos;
    VecFx32 moverPos;
    u32 mover;
    u32 side;
    WiFiPlaza_SplitGroupPlace(ride->mover, &mover, &side);
    WiFiPlazaModels_GetMoverFrontPos(models, mover, side, &moverPos);
    WiFiPlazaObj_GetPosition(ride->obj, &pos);
    if (pos.x + FX32_ONE * 16 < moverPos.x) {
        return FALSE;
    }
    return TRUE;
}

// Jump arc from the player's cell onto the mover (24 frames); the camera follows
static BOOL WiFiPlazaPlayerRide_JumpOn(WiFiPlazaPlayerRide *ride, void *models, WiFiPlazaCamera *camera) {
    VecFx32 start, end, pos;
    u32 mover, side;
    Field2DPoint cell, packedCell;
    int done, difference, frame;
    u32 packed;
    ride->timer++;
    if (ride->timer < 24) {
        done = FALSE;
    } else {
        ride->timer = 24;
        done = TRUE;
    }
    WiFiPlaza_SplitGroupPlace(ride->mover, &mover, &side);
    WiFiPlazaModels_GetMoverFrontPos(models, mover, side, &end);
    packed = ((u32(*)(WiFiPlazaObj *))WiFiPlazaObj_GetGridPos)(ride->obj);
    packedCell.x = packed;
    packedCell.y = packed >> 16;
    cell = packedCell;
    WiFiPlazaMap_PosToVecFx32(&cell.x, &start.x);
    pos.x = 0;
    difference = end.y - start.y;
    frame = (int)(ride->timer > 0 ? (float)(ride->timer << 12) + 0.5f : (float)(ride->timer << 12) - 0.5f);
    pos.y = FX_Div((fx32)(((s64)frame * difference + 2048) >> 12), 24 << 12);
    if (ride->timer >= 4) {
        difference = end.z - start.z;
        frame = (int)(ride->timer - 4 > 0 ? (float)((ride->timer - 4) << 12) + 0.5f : (float)((ride->timer - 4) << 12) - 0.5f);
        pos.z = FX_Div((fx32)(((s64)frame * difference + 2048) >> 12), 20 << 12);
    } else {
        pos.z = 0;
    }
    VEC_Add(&pos, &start, &pos);
    WiFiPlazaCamera_SetTarget(camera, pos.x, pos.y, pos.z);
    pos.y += (fx32)(((s64)FX_SinCosTable_[((u16)(0x7FFF * ride->timer / 24) >> 4) * 2] * FX32_CONST(16) + 2048) >> 12);
    WiFiPlazaObj_SetPosition(ride->obj, &pos);
    return done;
}

// Landing hop on the mover (16 frames)
static BOOL WiFiPlazaPlayerRide_Land(WiFiPlazaPlayerRide *ride, void *models) {
    VecFx32 pos;
    BOOL finished;
    fx32 offset;

    ride->timer++;
    if (ride->timer < 16) {
        finished = FALSE;
    } else {
        ride->timer = 16;
        finished = TRUE;
    }

    offset = FX_Mul(FX_SinIdx((u16)(((ride->timer % 8) * 0x7FFF) / 8)), FX32_CONST(3));
    WiFiPlazaObj_GetPosition(ride->obj, &pos);
    pos.y += offset;
    WiFiPlazaObj_SetPosition(ride->obj, &pos);
    return finished;
}

static void WiFiPlazaPlayerRide_FollowMover(WiFiPlazaPlayerRide *ride, void *models, WiFiPlazaRoomWork *room) {
    VecFx32 pos;
    u32 side;
    u32 mover;
    WiFiPlaza_SplitGroupPlace(ride->mover, &mover, &side);
    WiFiPlazaModels_GetMoverFrontPos(models, mover, side, &pos);
    WiFiPlazaObj_SetPosition(ride->obj, &pos);
}

static void WiFiPlazaPlayerRide_FollowMoverBounce(WiFiPlazaPlayerRide *ride, void *models, WiFiPlazaRoomWork *room) {
    VecFx32 pos;
    u32 mover;
    u32 side;
    fx32 offset;
    WiFiPlaza_SplitGroupPlace(ride->mover, &mover, &side);
    if (WiFiPlazaRoom_IsMoverWobbling(room, mover) == TRUE) {
        if (!WiFiPlazaPlayerBounce_IsActive(&ride->bounce)) {
            WiFiPlazaPlayerBounce_Start(&ride->bounce);
        }
    } else {
        if (WiFiPlazaPlayerBounce_IsActive(&ride->bounce) == TRUE) {
            WiFiPlazaPlayerBounce_Stop(&ride->bounce);
        }
    }
    offset = WiFiPlazaPlayerBounce_Update(&ride->bounce);
    WiFiPlazaObj_GetPosition(ride->obj, &pos);
    pos.y += offset;
    WiFiPlazaObj_SetPosition(ride->obj, &pos);
}

static void WiFiPlazaPlayerState_InitArrivalMenu(WiFiPlazaOwnArrivalWork *work, WiFiPlazaRoomWork *room) {
    int i;
    WiFiPlazaRoom_CreateListItems(room, 18);
    for (i = 1; i < 18; i++) {
        WiFiPlazaRoom_BufferTypeName(room, 0, i);
        WiFiPlazaRoom_AddListItem(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00006), i);
    }
    WiFiPlazaRoom_AddListItem(room, WiFiPlazaRoom_GetMsgString(room, 1, msg_0777_00007), 0);
    work->menuTemplate = sWiFiPlazaArrivalMenuTemplate;
    work->menuTemplate.items = WiFiPlazaRoom_GetListItems(room);
}

static void WiFiPlazaPlayerState_FreeArrivalMenu(WiFiPlazaOwnArrivalWork *work, WiFiPlazaRoomWork *room) {
    WiFiPlazaRoom_DeleteListItems(room);
}

// Another player's slot while that player is not in the room: creates its object once the lobby reports it
static BOOL WiFiPlazaPlayerState_OtherWaitForPlayer(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaWork *lobby = WiFiPlazaRoom_GetLobby(room);
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaObj *obj = WiFiPlazaObj_FindById(objManager, index);
    WiFiPlazaPlacement *placement = WiFiPlazaPlayerState_GetWork(state);
    WiFiPlazaPlayerStateManager *manager = WiFiPlazaRoom_GetPlayerStates(room);
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_OTHER_WAIT_INIT:
        WiFiPlazaPlayerState_AllocWork(state, 8);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_OTHER_WAIT_WAIT_ENTER);
    case WIFI_PLAZA_OTHER_WAIT_WAIT_ENTER: {
        int joined = WiFiPlazaWork_TakePlayerEntered(lobby, index);
        int present = WiFiPlazaWork_IsSlotOccupied(lobby, index);
        if (joined == TRUE || present == TRUE) {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_OTHER_WAIT_CREATE_OBJ);
        }
        break;
    }
    case WIFI_PLAZA_OTHER_WAIT_CREATE_OBJ: {
        WiFiPlazaPlayer *player = WiFiPlazaWork_GetPlayer(lobby, index);
        if (!player) {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_OTHER_WAIT_WAIT_ENTER);
            break;
        }
        if (WiFiPlazaPlayer_HasTapToy(player) == TRUE) {
            WiFiPlazaObj *newObj = WiFiPlazaObj_CreatePlayer(objManager, index, WiFiPlazaPlayer_GetAvatar(player));
            if (newObj) {
                WiFiPlazaObj_SetVisible(newObj, 0);
                WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_OTHER_WAIT_PLACE);
            }
        }
        break;
    }
    case WIFI_PLAZA_OTHER_WAIT_PLACE: {
        WiFiPlazaPlayer *player = WiFiPlazaWork_GetPlayer(lobby, index);
        if (!player) {
            if (obj) {
                WiFiPlazaObj_Delete(obj);
            }
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_OTHER_WAIT_WAIT_ENTER);
            break;
        }
        if (WiFiPlazaPlayerState_PlaceOtherForActivity(placement, lobby, room, obj, (u8)WiFiPlazaPlayer_GetActivity(player), 1)) {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_OTHER_WAIT_START_PRESENT);
            WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_IDLE);
            WiFiPlazaRoom_SetCardBlocked(room, (u8)index, 1);
            WiFiPlazaRoom_SetPlayerBusy(room, (u8)index, 1);
            WiFiPlazaObj_SetAvatarMode(obj, 0);
            WiFiPlazaPlayerState_Push(manager, index, &sWiFiPlazaPlayerStateOtherActivity, placement);
        } else {
            WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_DROP_IN);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_OTHER_WAIT_WAIT_DROP_IN);
            WiFiPlazaRoom_SetPlayerBusy(room, (u8)index, 1);
            WiFiPlazaRoom_SetCardBlocked(room, (u8)index, 1);
            WiFiPlazaObj_SetVisible(obj, 1);
        }
        break;
    }
    case WIFI_PLAZA_OTHER_WAIT_WAIT_DROP_IN:
        if (WiFiPlazaObj_IsFinished(obj) == TRUE) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_OTHER_WAIT_START_PRESENT:
        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_WANDER);
        WiFiPlazaPlayerState_Set(manager, index, &sWiFiPlazaPlayerStateOtherPresent, NULL);
        WiFiPlazaRoom_SetPlayerBusy(room, (u8)index, 0);
        WiFiPlazaRoom_SetCardBlocked(room, (u8)index, 0);
        break;
    }
    return FALSE;
}

// Another player's slot while that player is in the room: leaving, riding a mover, moving to an attraction
static BOOL WiFiPlazaPlayerState_OtherPresent(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObj *obj;
    WiFiPlazaPlacement *placement;
    WiFiPlazaObjManager *objManager;
    WiFiPlazaPlayerStateManager *manager;
    WiFiPlazaWork *lobby;
    lobby = WiFiPlazaRoom_GetLobby(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    manager = WiFiPlazaRoom_GetPlayerStates(room);
    obj = WiFiPlazaObj_FindById(objManager, index);
    WiFiPlazaObj_GetOwnPlayer(objManager);
    placement = WiFiPlazaPlayerState_GetWork(state);
    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_OTHER_PRESENT_INIT:
        placement = WiFiPlazaPlayerState_AllocWork(state, 8);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_OTHER_PRESENT_UPDATE);
    case WIFI_PLAZA_OTHER_PRESENT_UPDATE:
        if (WiFiPlazaWork_TakePlayerLeft(lobby, index)) {
            WiFiPlazaPlayerState_FreeWork(state);
            WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_IDLE);
            WiFiPlazaRoom_SetCardBlocked(room, (u8)index, 1);
            WiFiPlazaRoom_SetPlayerBusy(room, (u8)index, 1);
            WiFiPlazaPlayerState_Set(manager, index, &sWiFiPlazaPlayerStateOtherLeave, NULL);
            break;
        }
        WiFiPlazaWork_TakePlayerUpdated(lobby, index);
        if (WiFiPlazaWork_IsParadeStarted(lobby) == TRUE && WiFiPlazaWork_FindGroupPlace(lobby, index) != -1) {
            WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_IDLE);
            WiFiPlazaRoom_SetCardBlocked(room, (u8)index, 1);
            WiFiPlazaRoom_SetPlayerBusy(room, (u8)index, 1);
            WiFiPlazaObj_SetAvatarMode(obj, 0);
            WiFiPlazaPlayerState_FreeWork(state);
            WiFiPlazaPlayerState_Set(manager, index, &sWiFiPlazaPlayerStateOtherRideMover, placement);
            break;
        }
        if (WiFiPlazaPlayerState_PlaceOtherForActivity(placement, lobby, room, obj, (u8)WiFiPlazaPlayer_GetActivity(WiFiPlazaWork_GetPlayer(lobby, index)), 0)) {
            WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_IDLE);
            WiFiPlazaRoom_SetCardBlocked(room, (u8)index, 1);
            WiFiPlazaRoom_SetPlayerBusy(room, (u8)index, 1);
            WiFiPlazaObj_SetAvatarMode(obj, 0);
            WiFiPlazaPlayerState_Push(manager, index, &sWiFiPlazaPlayerStateOtherActivity, placement);
            break;
        }
        if (WiFiPlazaWork_HasPlayedTapToy(lobby, index) == TRUE) {
            WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_IDLE);
            WiFiPlazaRoom_SetPlayerBusy(room, (u8)index, 1);
            WiFiPlazaPlayerState_Push(manager, index, &sWiFiPlazaPlayerStatePlayInstrument, NULL);
        }
        break;
    }
    return FALSE;
}

// Another player left the room: its object rises out and is deleted
static BOOL WiFiPlazaPlayerState_OtherLeave(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager = WiFiPlazaRoom_GetObjManager(room);
    WiFiPlazaObj *obj = WiFiPlazaObj_FindById(objManager, index);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_OTHER_LEAVE_RISE_OUT:
        WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_RISE_OUT);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_OTHER_LEAVE_WAIT_RISE_OUT:
        if (WiFiPlazaObj_IsFinished(obj) != FALSE) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_OTHER_LEAVE_DELETE:
        WiFiPlazaObj_Delete(obj);
        WiFiPlazaPlayerState_Set(WiFiPlazaRoom_GetPlayerStates(room), index, &sWiFiPlazaPlayerStateOtherWait, NULL);
        break;
    }
    return FALSE;
}

// Another player rides a mover
static BOOL WiFiPlazaPlayerState_OtherRideMover(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager;
    WiFiPlazaModels *models;
    WiFiPlazaWork *lobby;
    VecFx32 vecD;
    VecFx32 vecC;
    VecFx32 vecB;
    VecFx32 vecA;
    WiFiPlazaOtherRideWork *work;
    BOOL done;
    fx32 delta;
    fx32 offset;

    lobby = WiFiPlazaRoom_GetLobby(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    models = WiFiPlazaRoom_GetModels(room);
    work = WiFiPlazaPlayerState_GetWork(state);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_OTHER_RIDE_RISE_OUT:
        work = WiFiPlazaPlayerState_AllocWork(state, sizeof(WiFiPlazaOtherRideWork));
        work->obj = WiFiPlazaObj_FindById(objManager, index);
        work->mover = WiFiPlazaWork_FindGroupPlace(lobby, index);
        GF_ASSERT(work->mover != -1);
        WiFiPlaza_SplitGroupPlace(work->mover, (u32 *)&work->mover, (u32 *)&work->side);
        WiFiPlazaObj_SetHandler(objManager, work->obj, WIFI_PLAZA_OBJ_HANDLER_RISE_OUT);
        WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_OTHER_RIDE_WAIT_RISE_OUT);
        break;
    case WIFI_PLAZA_OTHER_RIDE_WAIT_RISE_OUT:
        if (WiFiPlazaObj_IsFinished(work->obj) != FALSE) {
            WiFiPlazaModels_GetMoverFrontPos(models, work->mover, work->side, &vecD);
            work->x = vecD.x - FX32_ONE * 16;
            WiFiPlazaObj_GetPosition(work->obj, &vecD);
            work->startY = vecD.y;
            work->frame = 0;
            {
                Field2DPoint cell = { 0 };
                WiFiPlazaObj_SetGridPos(work->obj, cell);
            }
            PlaySE(SEQ_SE_DP_TELE2);
            WiFiPlazaObj_SetSpinEffect(work->obj, 1);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_OTHER_RIDE_MOVE_TO_FLOAT);
        }
        break;
    case WIFI_PLAZA_OTHER_RIDE_MOVE_TO_FLOAT:
        done = FALSE;
        work->frame++;
        if (work->frame >= 24) {
            work->frame = 24;
            done = TRUE;
        }
        WiFiPlazaModels_GetMoverFrontPos(models, work->mover, work->side, &vecC);
        vecB.z = vecC.z;
        vecB.x = work->x;
        delta = vecC.y - work->startY;
        vecB.y = FX_Div(FX_Mul(FX_F32_TO_FX32(work->frame), delta), FX32_CONST(24));
        vecB.y += work->startY;
        WiFiPlazaObj_SetPosition(work->obj, &vecB);
        if (done == TRUE) {
            WiFiPlazaObj_SetSpinEffect(work->obj, 0);
            WiFiPlazaObj_SetLightFlags(work->obj, 8);
            WiFiPlazaObj_SetDirection(work->obj, 2);
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_OTHER_RIDE_RIDE);
        }
        break;
    case WIFI_PLAZA_OTHER_RIDE_RIDE:
        if (WiFiPlazaRoom_IsMoverWobbling(room, work->mover) == TRUE) {
            if (WiFiPlazaPlayerBounce_IsActive(&work->bounce) == FALSE) {
                WiFiPlazaPlayerBounce_Start(&work->bounce);
            }
        } else if (WiFiPlazaPlayerBounce_IsActive(&work->bounce) == TRUE) {
            WiFiPlazaPlayerBounce_Stop(&work->bounce);
        }
        offset = WiFiPlazaPlayerBounce_Update(&work->bounce);
        WiFiPlazaModels_GetMoverFrontPos(models, work->mover, work->side, &vecA);
        vecA.y += offset;
        WiFiPlazaObj_SetPosition(work->obj, &vecA);
        if (WiFiPlazaWork_GetGroupState(lobby, work->mover) == 2) {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_OTHER_RIDE_DELETE);
        }
        break;
    case WIFI_PLAZA_OTHER_RIDE_DELETE:
        WiFiPlazaObj_Delete(work->obj);
        WiFiPlazaPlayerState_FreeWork(state);
        WiFiPlazaPlayerState_Set(WiFiPlazaRoom_GetPlayerStates(room), index, &sWiFiPlazaPlayerStateOtherWait, NULL);
        break;
    }
    return FALSE;
}

// Another player walks to the cell chosen by WiFiPlazaPlayerState_PlaceOtherForActivity (arg) and stays there while the lobby reports it
static BOOL WiFiPlazaPlayerState_OtherDoActivity(WiFiPlazaPlayerState *state, WiFiPlazaRoomWork *room, u32 index) {
    WiFiPlazaObjManager *objManager;
    WiFiPlazaWork *lobby;
    BOOL leave;
    Field2DPoint current;
    Field2DPoint cell;
    Field2DPoint packedCell;
    WiFiPlazaPlacement *placement;
    void *obj;
    WiFiPlazaObj *ownPlayer;

    placement = WiFiPlazaPlayerState_GetArg(state);
    lobby = WiFiPlazaRoom_GetLobby(room);
    objManager = WiFiPlazaRoom_GetObjManager(room);
    obj = WiFiPlazaObj_FindById(objManager, index);
    ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);

    switch (WiFiPlazaPlayerState_GetStep(state)) {
    case WIFI_PLAZA_OTHER_ACTIVITY_START:
        if (placement->justAppeared == FALSE) {
            WiFiPlazaPlayerState_NextStep(state);
        } else {
            WiFiPlazaPlayerState_SetStep(state, WIFI_PLAZA_OTHER_ACTIVITY_DROP_IN);
        }
        break;
    case WIFI_PLAZA_OTHER_ACTIVITY_RISE_OUT:
        WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_RISE_OUT);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_OTHER_ACTIVITY_WAIT_RISE_OUT:
    case WIFI_PLAZA_OTHER_ACTIVITY_WAIT_LEAVE:
    case WIFI_PLAZA_OTHER_ACTIVITY_WAIT_RETURN:
        if (WiFiPlazaObj_IsFinished(obj) == TRUE) {
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_OTHER_ACTIVITY_DROP_IN: {
        u32 packed = ((u32(*)(WiFiPlazaObj *))WiFiPlazaObj_GetGridPos)(obj);
        packedCell.x = packed;
        packedCell.y = packed >> 16;
        current = packedCell;
        WiFiPlazaObj_SetPositionFromGrid(obj, &current);
        if (WiFiPlazaObj_IsVisible(obj) == 0) {
            WiFiPlazaObj_SetVisible(obj, 1);
        }
        WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_DROP_IN);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    }
    case WIFI_PLAZA_OTHER_ACTIVITY_WAIT_DROP_IN:
        if (WiFiPlazaObj_IsFinished(obj) == TRUE) {
            WiFiPlazaPlayerState_NextStep(state);
            WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_IDLE);
            WiFiPlazaRoom_SetPlayerBusy(room, index, 0);
            WiFiPlazaRoom_SetCardBlocked(room, index, 0);
        }
        break;
    case WIFI_PLAZA_OTHER_ACTIVITY_STAY:
        leave = FALSE;
        if (placement->besideOwnPlayer == TRUE && WiFiPlazaWork_HasTalkPartnerArrived(lobby) == 0 && WiFiPlazaPlayer_GetActivity(WiFiPlazaWork_GetMyPlayer(lobby)) == WIFI_PLAZA_ACTIVITY_TALK) {
            WiFiPlazaWork_SetTalkPartnerArrived(lobby);
            WiFiPlazaObj_RunCommand(objManager, ownPlayer, 0, Field2D_GetOppositeDirection(WiFiPlazaObj_GetParam(obj, 6)));
        }
        obj = WiFiPlazaWork_GetPlayer(lobby, index);
        if (obj == NULL) {
            leave = TRUE;
        } else if (placement->besideOwnPlayer == TRUE && WiFiPlazaWork_IsTalking(lobby) == FALSE) {
            leave = TRUE;
        } else if (WiFiPlazaWork_TakePlayerUpdated(lobby, index) != FALSE) {
            int kind = WiFiPlazaPlayer_GetActivity(obj);
            if (kind != placement->kind) {
                leave = TRUE;
            }
        }
        if (leave == TRUE) {
            WiFiPlazaPlayerState_NextStep(state);
            WiFiPlazaRoom_SetPlayerBusy(room, index, 1);
            WiFiPlazaRoom_SetCardBlocked(room, index, 1);
        }
        break;
    case WIFI_PLAZA_OTHER_ACTIVITY_LEAVE:
        WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_RISE_OUT);
        WiFiPlazaPlayerState_NextStep(state);
        break;
    case WIFI_PLAZA_OTHER_ACTIVITY_RETURN:
        if (WiFiPlazaObj_IsOnCell(WiFiPlazaObj_GetOwnPlayer(objManager), placement->x, placement->y) == FALSE) {
            cell.x = placement->x << 4;
            cell.y = placement->y << 4;
            WiFiPlazaObj_SetGridPos(obj, cell);
            WiFiPlazaObj_SetPositionFromGrid(obj, &cell);
            WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_DROP_IN);
            WiFiPlazaPlayerState_NextStep(state);
        }
        break;
    case WIFI_PLAZA_OTHER_ACTIVITY_END:
        WiFiPlazaObj_SetHandler(objManager, obj, WIFI_PLAZA_OBJ_HANDLER_WANDER);
        WiFiPlazaRoom_SetPlayerBusy(room, index, 0);
        WiFiPlazaRoom_SetCardBlocked(room, index, 0);
        return TRUE;
    }
    return FALSE;
}

// Places another player for lobby activity `kind` on a free cell of the matching map tags
static BOOL WiFiPlazaPlayerState_PlaceOtherForActivity(WiFiPlazaPlacement *placement, WiFiPlazaWork *lobby, WiFiPlazaRoomWork *room, WiFiPlazaObj *obj, u8 kind, u32 justAppeared) {
    s32 direction;
    u16 cell[2];
    Field2DPoint position;
    Field2DPoint current;
    WiFiPlazaObj *ownPlayer;
    WiFiPlazaObjManager *objManager;
    WiFiPlazaMap *map;
    s32 id;
    u32 packed;
    s32 besideOwnPlayer = FALSE;

    objManager = WiFiPlazaRoom_GetObjManager(room);
    ownPlayer = WiFiPlazaObj_GetOwnPlayer(objManager);
    map = WiFiPlazaRoom_GetMap(room);
    id = WiFiPlazaObj_GetParam(obj, 4);
    direction = DIR_SOUTH;

    switch (kind) {
    case WIFI_PLAZA_ACTIVITY_SWALOT_PLOP:
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, sWiFiPlazaActivity2Tags, 4, cell)) {
            return besideOwnPlayer;
        }
        break;
    case WIFI_PLAZA_ACTIVITY_MIME_JR_TOP:
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, sWiFiPlazaActivity3Tags, 4, cell)) {
            return besideOwnPlayer;
        }
        break;
    case WIFI_PLAZA_ACTIVITY_WOBBUFFET_POP:
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, sWiFiPlazaActivity4Tags, 4, cell)) {
            return besideOwnPlayer;
        }
        break;
    case WIFI_PLAZA_ACTIVITY_FOOTPRINT_STAMP_BOARD_0:
        direction = DIR_NORTH;
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, sWiFiPlazaActivity5Tags, 8, cell)) {
            return besideOwnPlayer;
        }
        break;
    case WIFI_PLAZA_ACTIVITY_FOOTPRINT_STAMP_BOARD_1:
        direction = DIR_NORTH;
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, sWiFiPlazaActivity6Tags, 8, cell)) {
            return besideOwnPlayer;
        }
        break;
    case WIFI_PLAZA_ACTIVITY_VISITOR_PROFILES:
        direction = DIR_EAST;
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, sWiFiPlazaActivity7Tags, 4, cell)) {
            return besideOwnPlayer;
        }
        break;
    case WIFI_PLAZA_ACTIVITY_PLAZA_NEWS:
        direction = DIR_NORTH;
        if (!WiFiPlazaPlayerState_FindFreeTaggedCell(map, objManager, sWiFiPlazaActivity8Tags, 4, cell)) {
            return besideOwnPlayer;
        }
        break;
    case WIFI_PLAZA_ACTIVITY_PARADE_FLOAT:
        return besideOwnPlayer;
    case WIFI_PLAZA_ACTIVITY_TALK:
        if (!WiFiPlazaWork_IsTalking(lobby)) {
            return besideOwnPlayer;
        }
        if (!WiFiPlazaWork_WasAskedToTalk(lobby)) {
            return besideOwnPlayer;
        }
        if (id != WiFiPlazaWork_GetTalkPartner(lobby)) {
            return besideOwnPlayer;
        }
        if (WiFiPlazaObj_FindFreeNeighbor(objManager, ownPlayer, &direction, (Field2DPoint *)cell) != TRUE) {
            GF_AssertFail();
        }
        direction = Field2D_GetOppositeDirection(direction);
        besideOwnPlayer = TRUE;
        break;
    case WIFI_PLAZA_ACTIVITY_NONE:
    case WIFI_PLAZA_ACTIVITY_FREE:
    case WIFI_PLAZA_ACTIVITY_EXIT:
    case WIFI_PLAZA_ACTIVITY_GAME_ENTRY:
    case WIFI_PLAZA_ACTIVITY_SURVEY:
    case 14:
    default:
        return FALSE;
    }

    packed = ((u32(*)(WiFiPlazaObj *))WiFiPlazaObj_GetGridPos)(obj);
    current.x = packed;
    current.y = packed >> 16;
    position = current;
    WiFiPlazaObj_SetGridPosAndDir(obj, *(Field2DPoint *)cell, direction);
    placement->x = position.x / 16;
    placement->y = position.y / 16;
    placement->kind = kind;
    placement->besideOwnPlayer = besideOwnPlayer;
    placement->justAppeared = justAppeared;
    return TRUE;
}

static BOOL WiFiPlazaPlayerState_FindFreeTaggedCell(WiFiPlazaMap *map, WiFiPlazaObjManager *objManager, const u32 *tags, u32 count, u16 *outPos) {
    u32 i;
    u16 x;
    u16 y;

    for (i = 0; i < count; i++) {
        if (WiFiPlazaMap_FindTileTag(map, tags[i], &x, &y, 0) == TRUE && WiFiPlazaObj_FindAt(objManager, x, y) == FALSE) {
            outPos[0] = x << 4;
            outPos[1] = y << 4;
            return TRUE;
        }
    }
    return FALSE;
}
