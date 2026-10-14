#ifndef POKEHEARTGOLD_FIELD_SYSTEM_H
#define POKEHEARTGOLD_FIELD_SYSTEM_H

#include "field/area_data.h"
#include "field/area_light.h"
#include "field/draw_map_name.h"
#include "field/dynamic_terrain_height.h"
#include "field/field_3d_object_task.h"
#include "field/hblank_system.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"
#include "field/field_texture_manager.h"
#include "field/map_prop_render.h"
#include "field/signpost.h"
#include "field/weather_manager.h"

#include "bag_cursor.h"
#include "battle_regulation.h"
#include "bg_window.h"
#include "bug_contest_internal.h"
#include "camera.h"
#include "camera_translation.h"
#include "field_types_def.h"
#include "gear_phone.h"
#include "map_events_internal.h"
#include "map_matrix.h"
#include "menu_input_state.h"
#include "field_fog.h"
#include "field/field_effect_manager.h"
#include "overlay_manager.h"
#include "photo_types_def.h"
#include "save_pokegear.h"
#include "scrcmd_9.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "terrain_attributes.h"
#include "field_time_events.h"
#include "pokedex_resume_state.h"

typedef struct FollowMon {
    LocalMapObject *mapObject;
    u32 queuedMovement;
    u32 queuedX;
    u32 queuedZ;
    u32 species;
    u8 gender;
    u8 recreatePending;
    u8 active;
    u8 shiny;
    u16 form;
    u16 dummy;
    u32 queuedStepState;
} FollowMon;

struct GearPhoneRingManager {
    u8 active : 1;
    u8 unk_var0_1 : 1;
    u8 unk_var0_2 : 1;
    u8 ringing : 1;
    u8 unk_var0_4 : 4;
    u8 unk_var1;
    u8 callerId;
    u8 isScriptedCall;
    u8 callScriptID;
    u8 callerParams[2];
    u8 callTriggerID;
    s32 elapsedMinutes;
    s32 callIntervalMinutes;
    u16 maxActiveDuration;
    u16 randomCallChance;
    s64 activeTimer;                       // Seconds? see GearPhoneRingManager_HandleTimeAdvance
    PhoneBookEntry entry;                  // 0x1c
    SavePokegear *pokegearData;            // 0x30
    PhoneCallPersistentState *savingsData; // 0x34
    SaveData *saveData;                    // 0x38
    FieldSystem *sys;                      // 0x3c
    struct PokegearRingingTask {
        SysTask *task;
        u8 counter;
    } gearRing;
}; // size: 0x48

typedef struct FieldProcessManager {
    OverlayManager *parent;
    OverlayManager *child;
    BOOL isPaused;
    BOOL isDone;
} FieldProcessManager;

typedef struct FieldTransitionWork {
    int state;
    u32 direction;
} FieldTransitionWork;

typedef struct FieldMapSystems {
    u32 unk0;
    Field3dObjectTaskManager *field3dObjectTaskManager;
    FieldDrawMapNameInfo *drawMapNameInfo;
    WeatherManager *weatherManager;
    FieldTextureManager *textureManager;
    u32 unk14;
    UnusedFieldModel *unk18;
    HBlankSystem *hBlankSystem;
    void *poisonEffect;
    void *localData;
} FieldMapSystems;

#define MAP_OBJECT_PRELOAD_SENTINEL 0xFFFF
#define MAX_MAP_OBJECTS_TO_PRELOAD  24

typedef struct MapObjectsToPreload {
    u16 count;
    int ids[MAX_MAP_OBJECTS_TO_PRELOAD];
} MapObjectsToPreload;

typedef struct FollowMonMood {
    u32 personality;
    u16 species;
    u16 isRegistered;
    Pokemon *mon;
} FollowMonMood;

// One entry of the following Pokemon's interaction tables: the conditions under which the interaction script
// is picked. Zero means "any" for every condition.
typedef struct FollowMonRule {
    u8 hp;             // hp bucket (FollowMonSituation.hpBucket)
    u8 mood : 4;       // mood range, see FollowMonRule_CheckMood
    u8 friendship : 4; // friendship range, see FollowMonRule_CheckFriendship
    u8 natureGroup : 3;
    u8 gender : 2;
    u8 status : 3;   // 7: any of poison, burn, freeze, paralysis or sleep
    u8 heldItem : 5; // held item pocket; 9: any held item
    u8 unused_3 : 3;
    u8 type : 5; // either of the Pokemon's types
    u8 pokeathlonStat : 3;
    u8 encounterTile;
    u8 speciesAttr; // see FollowMonRule_CheckSpeciesAttr
    u8 shinyLeaf;   // see FollowMonRule_CheckShinyLeaf
    u16 weather : 3;
    u16 unused_8 : 10;
    u16 facing : 3;
    u16 nearbyObjects : 3; // 5: five or more
    u16 timeOfDay : 3;
    u16 script : 10; // interaction script picked when every condition holds
    u16 mapId;       // map id + 1
    u16 metatileBehavior;
    u8 unused_10 : 1;
    u8 level : 2;
    u8 nearbyRock : 2;
    u8 hiddenItems : 3; // 4: four or more
    u8 chance;          // percent
    u16 flag;           // flag that must be set
} FollowMonRule;

// One step of an interaction script.
typedef struct FollowMonInteractionStep {
    u16 movement; // movement script + 1
    u16 message;  // message + 1
    u16 sound;    // sound effect, or a cry above SEQ_SE_END
    u8 emote;     // emote bubble + 1
    u8 wait;      // frames to wait after the step
} FollowMonInteractionStep;

// One step of an interaction movement script.
typedef struct FollowMonMovementStep {
    u8 facing; // direction + 1; 0xFF ends the script
    u8 duration;
    s8 dx;
    s8 dy;
    s8 dz;
    u8 playSound;
    u8 unk6;
    u8 unk7;
} FollowMonMovementStep;

typedef struct FollowMonInteraction {
    Window window;
    String *string;
    FollowMonRule rules[100]; // 0-29 for the current map section, 30-99 everywhere
    FollowMonInteractionStep steps[5];
    u32 askQuestion;
    u16 yesScript;
    u16 noScript;
    s8 friendshipChange;
    s8 moodChange;
    u8 accessory; // accessory + 1 to give
    u8 shinyLeaf; // shiny leaf to give
    FollowMonMovementStep movements[10];
    u8 movementState;
    u8 state;
    u8 movementFrame;
    u8 movementIndex;
    u8 stepIndex : 4;
    u8 stepCount : 4;
    u8 waitFrames;
    u16 printerId;
    VecFx32 savedPosition;
    u8 savedFacing;
    u8 form;
    u16 species;
    u16 answer; // yes/no answer
    u16 metatileBehavior;
} FollowMonInteraction;

struct FieldSystem {
    FieldProcessManager *processManager;
    FieldMapSystems *mapSystems;
    BgConfig *bgConfig;
    SaveData *saveData;
    TaskManager *taskman;
    MapEvents *mapEvents;
    int bottomScreenType;
    int savedBottomScreenAppId;
    Location *location;
    Camera *camera;
    void *cameraShift;
    MapLoadManager *mapLoadManager;
    MapMatrix *mapMatrix;
    AreaDataManager *areaDataManager;
    MapObjectsToPreload *mapObjectsToPreload;
    MapObjectManager *mapObjectManager;
    PlayerAvatar *playerAvatar;
    FieldEffectManager *fieldEffectManager;
    ModelAttributes *modelAttributes;
    FogData *fog;
    AreaLightManager *areaLightManager;
    MapPropAnimationManager *mapPropAnimationManager;
    MapPropOneShotAnimationManager *mapPropOneShotAnimationManager;
    TerrainAttributes *terrainAttributes;
    void *terrainQueries;
    BOOL skipMapAttributes;
    Signpost *signpost;
    BOOL runningFieldMap;
    int mapLoadType;
    const struct MapLoadMode *mapLoadMode;
    u16 unk78;
    u16 lastFacingDirection;   // 0x7A, used to determine whether the following field should be incremented
    u16 reverseTurnFrameSteps; // 0x7C, turning back and forth in place will increase the effective encounter rate
    u16 encounterInhibitSteps; // 0x7E, don't generate an encounter in the first 4 steps on a new map
    struct UnionRoom *unionRoom;
    struct UnionRoomObjects *unionRoomObjects;
    u8 filler_88[0x8];
    int unk90;
    BagCursor *bagCursor;
    DynamicTerrainHeightManager *dynamicTerrainHeightManager;
    MapPropManager *mapPropManager;
    FrontierFieldSystem *frontierFsys;
    LinkBattleRuleset *linkBattleRuleset;
    PokedexResumeState *unkA8;
    u32 unkAC;
    void *unkB0;
    s64 plazaLoginTime;
    int fieldMapUpdateFlags;     // FieldMap_UpdateFrame steps: bit 0 texture free, 1 map load tick, 2 3D render, 3 MapPropAnimationPoolList_Update
    Field3dRenderObjManager *renderObjManager;
    int environmentSoundState;   // Used to keep track of the active weather/soundplate sound and their associated loading status.
    MapPropAnimationPoolList *mapPropAnimationPools;
    void *areaMapAnimation;
    u16 lastTouchMenuInput;
    u8 startMenuState : 6;
    u8 textbox_open : 1;
    u8 unkD2_7 : 1;
    u8 startMenuCursorPos;
    SysTask *touchSaveTask;    // bottom screen type 4 (bottom_screen_app.c)
    SysTask *bottomScreenTask; // the bottom-screen app manager, or the view photo task
    FieldViewPhoto *viewPhotoTask;
    int lastStartMenuAction;
    FollowMon followMon; // E4
    TimeOfDayPropAnimManager *timeOfDayPropAnims;
    FollowMonMood *followMonMood;
    MenuInputStateMgr menuInputState; // Tracks whether the last menu input was touch or keypad
    u8 startMenuInputState;
    u8 safariDecorationCursorPos[3];
    GearPhoneRingManager *phoneRingManager;
    BugContest *bugContest;
    fx32 unk11C;
    FollowMonInteraction *followMonInteraction;
    u32 judgeStatPosition;
}; // size: 0x128

typedef struct FieldInput {
    u16 interact : 1;
    u16 endMovement : 1;
    u16 menu : 1;
    u16 registeredItem : 2;
    u16 sign : 1;
    u16 mapTransition : 1;
    u16 movement : 1;
    u16 unk0_8 : 1;
    u16 switchBottomScreen : 1;
    u16 unk0_A : 1;
    u16 unk0_B : 1;
    u16 unk0_C : 1;
    u16 unk0_D : 1;
    u16 unk2;
    u8 playerDir;
    s8 transitionDir;
    u16 newKeys;
    u16 heldKeys;
    u16 unkA;
} FieldInput;

BOOL FieldMap_Init(OverlayManager *man, int *state);
BOOL FieldMap_Main(OverlayManager *man, int *state);
BOOL FieldMap_Exit(OverlayManager *man, int *state);
BOOL Field_Continue_AppInit(OverlayManager *man, int *unused);
BOOL Field_NewGame_AppInit(OverlayManager *man, int *unused);
BOOL Field_AppExec(OverlayManager *man, int *unused);
BOOL Field_AppExit(OverlayManager *man, int *unused);
void FieldSystem_LoadFieldOverlayInternal(FieldSystem *fieldSystem);
void FieldSystem_ClearRunningFieldMap(FieldSystem *fieldSystem);
u8 FieldSystem_GetStartMenuInputState(FieldSystem *fieldSystem);
void FieldSystem_SetStartMenuInputState(FieldSystem *fieldSystem, int state);
BOOL FieldSystem_IsFieldMapLoaded(FieldSystem *fieldSystem);
BOOL FieldSystem_IsFieldMapRunning(FieldSystem *fieldSystem);
BOOL FieldSystem_IsChildAppRunning(FieldSystem *fieldSystem);
void FieldSystem_LaunchApplication(FieldSystem *fieldSystem, const OverlayManagerTemplate *template, void *parentWork);
FieldSystem *FieldSystem_New(OverlayManager *man);
void FieldSystem_Delete(OverlayManager *man);
BOOL FieldSystem_IsPlayerMovementAllowed(FieldSystem *fieldSystem);
void FieldSystem_Control(FieldSystem *fieldSystem);
void FieldSystem_PauseProcesses();
void FieldSystem_ResumeProcesses();
int FieldSystem_GetBottomScreenState();
void FieldSystem_SetSavedBottomScreenAppId(FieldSystem *fieldSystem, int appId);
BgConfig *FieldSystem_GetBgConfigPtr(FieldSystem *fieldSystem);
SaveData *FieldSystem_GetSaveData(FieldSystem *fieldSystem);
void Task_AntipiracyRandom();
void Field_ForceEnvironmentSoundRecheck();

extern const OverlayManagerTemplate gApplication_NewGameFieldsys;
extern const OverlayManagerTemplate gApplication_ContinueFieldsys;

static inline void InitLocation(Location *location, int mapId, int warpId, int x, int y, int direction) {
    location->mapId = mapId;
    location->warpId = warpId;
    location->x = x;
    location->y = y;
    location->direction = direction;
}

#endif // POKEHEARTGOLD_FIELD_SYSTEM_H
