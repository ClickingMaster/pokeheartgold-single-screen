#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_RELAY_RUN_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_RELAY_RUN_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_internal.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "geometry_math.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"

// Relay Run: each team runs laps around an oval track. The player flicks the running Pokemon with the stylus to
// speed it up and swaps to the next member of the team before it runs out of energy.

#define RELAY_RUN_TEAMS        4
#define RELAY_RUN_MEMBERS      3
#define RELAY_RUN_OBSTACLES    30
#define RELAY_RUN_DURATION     2700 // frames
#define RELAY_RUN_TRACK_LENGTH 1024 // pixels per lap
#define RELAY_RUN_MAX_LAPS     60
#define RELAY_RUN_RUNNER_X     80 // screen x of the player's own runner
#define RELAY_RUN_TIRED_ENERGY 40

// PokeathlonRelayRunMember.condition
#define RELAY_RUN_FINE      0
#define RELAY_RUN_TIRED     1 // energy at RELAY_RUN_TIRED_ENERGY or below
#define RELAY_RUN_EXHAUSTED 2 // out of energy: can't be flicked until it recovers

// PokeathlonRelayRunTeam.speedGrade
#define RELAY_RUN_SPEED_STANDING 0
#define RELAY_RUN_SPEED_SLOW     1
#define RELAY_RUN_SPEED_FAST     2
#define RELAY_RUN_SPEED_TOP      3 // at its speed limit

// The swap to the next team member (PokeathlonRelayRunTeam.swapPhase, PokeathlonRelayRunTouch.displayPhase)
#define RELAY_RUN_SWAP_DONE   0
#define RELAY_RUN_SWAP_LEAVE  1
#define RELAY_RUN_SWAP_ARRIVE 2

// PokeathlonRelayRunObstacle.kind (0: none)
#define RELAY_RUN_OBSTACLE_BIG_WALL   1 // 48x48
#define RELAY_RUN_OBSTACLE_WALL       2 // 32x32
#define RELAY_RUN_OBSTACLE_SLOW_PATCH 3 // 64x64, under the runners: slows them down to their patchSpeed

// What a runner ran into (PokeathlonRelayRunTeam.wallState): an edge or a corner of a wall, or the side of a wall
// it got pushed out of after ending up inside it.
#define RELAY_RUN_HIT_NONE          0
#define RELAY_RUN_HIT_TOP           2
#define RELAY_RUN_HIT_RIGHT         3
#define RELAY_RUN_HIT_BOTTOM        4
#define RELAY_RUN_HIT_LEFT          5
#define RELAY_RUN_HIT_TOP_LEFT      6
#define RELAY_RUN_HIT_TOP_RIGHT     7
#define RELAY_RUN_HIT_BOTTOM_RIGHT  8
#define RELAY_RUN_HIT_BOTTOM_LEFT   9
#define RELAY_RUN_HIT_INSIDE_TOP    10
#define RELAY_RUN_HIT_INSIDE_RIGHT  11
#define RELAY_RUN_HIT_INSIDE_BOTTOM 12
#define RELAY_RUN_HIT_INSIDE_LEFT   13

// Collision circle of a Pokemon by size (NARC a/1/7/0 member 12).
typedef struct PokeathlonRelayRunCollision {
    s32 offsetY;
    s32 radius;
} PokeathlonRelayRunCollision;

// One member of a team.
typedef struct PokeathlonRelayRunMember {
    f32 acceleration; // scales the speed a flick gives
    f32 weight;       // scales it as well
    f32 pushPower;    // share of its speed it passes on to a runner it bumps into
    f32 energy;       // goes down while it runs, up while it rests
    f32 maxEnergy;
    u8 patchSpeed;   // fastest it runs on a slow patch (pixels per frame)
    u8 tiredSpeed;   // fastest it runs when tired
    u8 speedLimit;   // fastest it runs; the speed grades are measured against it
    u8 recoveryTime; // frames an exhausted member needs to recover
    u8 condition;    // RELAY_RUN_FINE / _TIRED / _EXHAUSTED
    u8 unk19[3];
} PokeathlonRelayRunMember;

// A team: the three Pokemon of one participant, one running at a time.
typedef struct PokeathlonRelayRunTeam {
    void *monObjects[RELAY_RUN_MEMBERS]; // PokeathlonCourseMonObject
    fx32 distance[RELAY_RUN_MEMBERS];    // distance each member ran
    PokeathlonRelayRunMember members[RELAY_RUN_MEMBERS];
    PokeathlonCourseSpritePoolSprite *conditionSprite; // tired / exhausted marks above the runner
    PokeathlonCourseSpritePoolSprite *shadowSprite;
    PokeathlonCourseSpritePoolSprite *swapSprite;  // flies up from the leaving member and down with the next
    PokeathlonCourseSpritePoolSprite *speedSprite; // dash lines
    VecFx32 position;                              // x along the lap (wraps at RELAY_RUN_TRACK_LENGTH), y on the track
    u8 animState;
    u8 unk89[2];
    u8 current; // member running
    VecFx32 velocity;
    u8 bumpedInto[RELAY_RUN_TEAMS]; // touching that team's runner since the last bump
    u8 laps;
    u8 swapping;       // handing over to the next member
    u8 swapPhase;      // 1 swap, 2 wait for the display, 0 done
    u8 shownSwapPhase; // drawing: the swap phase being animated
    u8 swapStep;       // drawing: step of that animation
    u8 swapTimer;
    u8 speedGrade; // 0 standing, 1 slow, 2 fast, 3 at its limit
    u8 tired;      // the running member ran out of energy; it is capped
    u8 recoveryTimer;
    u8 onPatch;        // on a slow patch this frame
    u8 lastCondition;  // drawing: the condition shown last frame
    u8 lastSpeedGrade; // drawing: the speed grade shown last frame
    u8 bestLaps;
    u8 stunTimer; // can't be moved or flicked while it runs
    u8 flicked;   // the stylus was lifted after a short drag
    u8 unkAB;
    int heldFrames; // frames the stylus has been held
    VecFx32 dragStart;
    VecFx32 dragEnd;
    BOOL runnerHidden;  // drawing: hidden during a swap
    int wallState;      // what it ran into last frame (RELAY_RUN_HIT_*)
    u8 owner;           // participant
    u8 behindStart;     // ran backwards over the start line with no laps
    u8 speedSoundGrade; // drawing: speed grade the dash sound was played for
    u8 speedSoundPlayed;
} PokeathlonRelayRunTeam;

// What the host keeps of one participant's stylus.
typedef struct PokeathlonRelayRunPress {
    BOOL pressed; // on the frame the stylus touches the screen
    BOOL held;
    u32 unk8;
} PokeathlonRelayRunPress;

// An obstacle on the track. The host loads the obstacle layout (NARC a/2/2/7, one of ten).
typedef struct PokeathlonRelayRunObstacle {
    BOOL active;
    PokeathlonCourseSpritePoolSprite *sprite;
    u16 x; // along the lap
    u8 y;
    u8 kind; // RELAY_RUN_OBSTACLE_*
} PokeathlonRelayRunObstacle;

// The outline of an obstacle for collisions (PokeathlonRelayRun_GetObstacleShape).
typedef struct PokeathlonRelayRunObstacleShape {
    s32 kind;
    Point2D corners[4];  // top left, top right, bottom right, bottom left (a slow patch: top left only)
    Point2D edges[4][2]; // top, right, bottom, left, moved out by the runner's radius
} PokeathlonRelayRunObstacleShape;

// What touching each edge or corner of a wall counts as (RELAY_RUN_HIT_*).
typedef struct PokeathlonRelayRunHitTable {
    int hits[4];
} PokeathlonRelayRunHitTable;

// The event's stat tables (NARC a/1/7/0 member 2): tables[n][a Pokemon stat] for each member value.
typedef struct PokeathlonRelayRunSetup {
    int tables[8][5];
} PokeathlonRelayRunSetup;

// The lap count shown for a second when it goes up.
typedef struct PokeathlonRelayRunLapCounter {
    Sprite *digits[2];
    PokeathlonCourseSpritePoolSprite *plate;
    u16 timer;
    u16 laps;
} PokeathlonRelayRunLapCounter;

// The stylus of a player.
typedef struct PokeathlonRelayRunTouch {
    BOOL held;
    u8 x;
    u8 y;
    u8 unk06[2];
    u8 displayPhase; // the swap animation reached this phase on the player's screen
    u8 unk09[0x1B];
} PokeathlonRelayRunTouch;

// A block of the link data holding a stylus.
typedef struct PokeathlonRelayRunTouchBlock {
    PokeathlonRelayRunTouch touch;
    u8 unk24[4];
} PokeathlonRelayRunTouchBlock;

// Everything a player draws, sent by the host every frame.
typedef struct PokeathlonRelayRunPacket {
    u16 x[RELAY_RUN_TEAMS];
    u8 y[RELAY_RUN_TEAMS];
    u8 runner[RELAY_RUN_TEAMS]; // current member | speed grade << 2
    u8 laps[RELAY_RUN_TEAMS];   // laps | swapping << 7
    s32 flags;                  // condition of each member (2 bits, team * 3 + member), finished << 24, bump << 25,
                                // behindStart of each team << 26
    u16 timer;
    u16 bumpX;
    u8 bumpY;
    u8 swapPhases;                 // swapPhase of each team (2 bits each)
    u8 swapSigns[RELAY_RUN_TEAMS]; // "change" sign shown at the top of each player's screen
    u8 unk22[6];
} PokeathlonRelayRunPacket;

// The link data of the event (PokeathlonCourse_GetDataCopyArea).
typedef struct PokeathlonRelayRunLink {
    PokeathlonRelayRunTouchBlock touch;                    // this player's stylus
    PokeathlonRelayRunPacket packetToSend;                 // host
    PokeathlonRelayRunTouchBlock touches[RELAY_RUN_TEAMS]; // everyone's stylus, collected by the host
    PokeathlonRelayRunPacket packet;                       // received from the host
} PokeathlonRelayRunLink;

typedef struct PokeathlonRelayRun {
    BgConfig *bgConfig;
    Window window;
    enum HeapID heapId;
    PokeathlonCourseSpritePool *spritePool;
    PokeathlonCourseSpritePoolSprite *signSprite;
    PokeathlonRelayRunLapCounter lapCounter;
    PokeathlonRelayRunTeam teams[RELAY_RUN_TEAMS];
    PokeathlonRelayRunPress presses[RELAY_RUN_TEAMS];
    BOOL dragging[RELAY_RUN_TEAMS];
    u8 finished;
    u8 unk3C1;
    u8 heldTooLong; // someone held the stylus down for 255 frames: more friction
    u8 nextBumpMark;
    PokeathlonCourseSpriteResources *monResources;
    void *monObjects; // PokeathlonCourseMonObjects
    void *bumpMarks[4];
    struct PokeathlonRelayRunCpu *cpu;     // the teams without a player
    struct PokeathlonRelayRunHud *hud;     // the touch screen
    s32 layoutToSend[RELAY_RUN_OBSTACLES]; // host: kind | y << 8 | x << 16
    s32 layout[RELAY_RUN_OBSTACLES];       // received
    PokeathlonRelayRunObstacle obstacles[RELAY_RUN_OBSTACLES];
    u8 bumped; // two runners bumped into each other this frame
    u8 bumpY;
    u16 bumpX;
    u16 timer; // frames left
    u16 signSoundPlayed;
    u8 swapSigns[RELAY_RUN_TEAMS]; // the sign at the top of each player's screen
} PokeathlonRelayRun;

// A team on the touch screen's track map.
typedef struct PokeathlonRelayRunHudTeam {
    PokeathlonCourseSpritePoolSprite *icon;          // its running member's icon
    PokeathlonCourseSpritePoolSprite *conditionMark; // tired / exhausted
    PokeathlonCourseSpritePoolSprite *swapMark;      // shown instead of the icon while it swaps
    PokeathlonCourseSpritePoolSprite *colorMark;     // the team's colour
    u8 lastCondition;
    u8 unk11[0xF];
} PokeathlonRelayRunHudTeam;

// A member's picture (char and palette data, loaded into one of the two picture slots when needed).
typedef struct PokeathlonRelayRunHudPic {
    void *chars;
    void *palette;
} PokeathlonRelayRunHudPic;

// The touch screen.
typedef struct PokeathlonRelayRunHud {
    enum HeapID heapId;
    PokeathlonCourseData *course;
    BgConfig *bgConfig;
    Window nameWindow; // "Next {name}"
    Window lapsWindow; // "Laps"
    MsgData *msgData;  // msg_0309
    MessageFormat *msgFormat;
    void *digitScreenFile;
    NNSG2dScreenData *digitScreen; // the big digits PokeathlonRelayRunHud_PrintDistance copies from
    u8 unk03C[0xC];

    union {
        PokeathlonRelayRunHudTeam teams[RELAY_RUN_TEAMS];

        struct {
            u8 unk00[0x74];
            void *files[RELAY_RUN_TEAMS * RELAY_RUN_MEMBERS]; // the icon files (NARC poketool/icongra)
        } icons;
    } map;

    NNSG2dCharacterData *iconChars[RELAY_RUN_TEAMS * RELAY_RUN_MEMBERS];
    u8 iconPalettes[RELAY_RUN_TEAMS * RELAY_RUN_MEMBERS];
    PokeathlonRelayRunHudPic pics[RELAY_RUN_MEMBERS]; // the player's own members
    u32 picCharLocations[2];                          // VRAM of the two picture slots
    u32 picPaletteLocations[2];
    void *picFrames[2]; // the picture slots (PokeathlonCourseSpritePoolSprite)
    void *tiredMarks[2];
    u16 benchMembers[2]; // member in each picture slot
    u32 rotateState;     // PokeathlonRelayRunHud_RotateMonPics
    int rotateSlot;
    u8 shownMembers[RELAY_RUN_TEAMS]; // member whose icon each team shows
    String *names[RELAY_RUN_MEMBERS]; // the player's members' nicknames
    Sprite *timerSprites[6];          // minutes, ':', tens, ones, '.', tenths

    struct {
        u16 y;
        u16 step;
    } cpuRotations[RELAY_RUN_TEAMS - 1]; // PokeathlonRelayRunHud_RotateCpuMonPics
} PokeathlonRelayRunHud;

BOOL PokeathlonRelayRun_Init(PokeathlonCourseData *course);
BOOL PokeathlonRelayRun_Draw(PokeathlonCourseData *course);
BOOL PokeathlonRelayRun_ShowResults(PokeathlonCourseData *course);
BOOL PokeathlonRelayRun_Exit(PokeathlonCourseData *course);
BOOL PokeathlonRelayRun_Start(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRelayRun_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRelayRun_SaveScores(PokeathlonCourseData *course, u8 *state);

void PokeathlonRelayRun_DrawTeams(PokeathlonCourseData *course, int own, PokeathlonRelayRunTeam *team, PokeathlonRelayRunLink *link);
void PokeathlonRelayRun_ScrollObstacles(PokeathlonRelayRun *relayRun, int x);
void PokeathlonRelayRun_InitMember(PokeathlonCourseData *course, void *setupFile, u8 participant, u8 member, PokeathlonRelayRunTeam *team);
void PokeathlonRelayRun_GetObstacleShape(int inset, const s32 *entry, volatile s32 *shape);
u16 PokeathlonRelayRun_CollideWithObstacles(PokeathlonRelayRun *relayRun, int wallState, int radius, VecFx32 *from, VecFx32 *to, VecFx32 *out);
f32 PokeathlonRelayRunTeam_GetMaxSpeed(PokeathlonRelayRunTeam *team, int member);
void PokeathlonRelayRun_AddFlick(const VecFx32 *velocity, const VecFx32 *flick, VecFx32 *out);
BOOL PokeathlonRelayRun_CollideRunners(PokeathlonCourseData *course, PokeathlonRelayRunTeam *teams, VecFx32 *where);
void PokeathlonRelayRunObstacle_SetDrawPriority(PokeathlonRelayRunObstacle *obstacle);
void PokeathlonRelayRunTeam_SetDrawPriority(PokeathlonRelayRunTeam *team, u8 y, int isOwn);
void PokeathlonRelayRun_UpdateStartLine(BgConfig *bgConfig, int x);

void PokeathlonRelayRun_UpdateLapCounter(PokeathlonRelayRunLapCounter *counter, u8 laps);
void PokeathlonRelayRun_AddResultWindow(PokeathlonRelayRun *relayRun);
void PokeathlonRelayRun_PlaySEIfOnScreen(int x, int seqNo, int isOwn);
void PokeathlonRelayRun_UpdateSwapSign(PokeathlonRelayRun *relayRun, u8 teamId, int touchingSwapButton);
void PokeathlonRelayRun_SaveResult(PokeathlonCourseData *course);
PokeathlonRelayRunHud *PokeathlonRelayRunHud_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
void PokeathlonRelayRunHud_Free(PokeathlonRelayRunHud *hud);
void PokeathlonRelayRunHud_LoadBgGraphics(PokeathlonRelayRunHud *hud);
void PokeathlonRelayRunHud_LoadSpriteGraphics(PokeathlonRelayRunHud *hud, PokeathlonCourseSpritePool *spritePool);
void PokeathlonRelayRunHud_CreateSprites(PokeathlonRelayRunHud *hud, PokeathlonCourseSpritePool *spritePool, PokeathlonCourseSpriteResources *monResources);
void PokeathlonRelayRunHud_LoadMonIcons(PokeathlonRelayRunHud *hud, PokeathlonCoursePoint *mons);
void PokeathlonRelayRunHud_LoadMonPics(PokeathlonRelayRunHud *hud, const PokeathlonCourseMonInfo *mon);
void PokeathlonRelayRunHud_ShowMonPic(PokeathlonRelayRunHud *hud, u32 slot, u32 member);
void PokeathlonRelayRunHud_SetMapPosition(PokeathlonRelayRunHud *hud, u8 teamId, u16 x);
void PokeathlonRelayRunHud_PrintDistance(PokeathlonRelayRunHud *hud, u8 laps, u32 x);
BOOL PokeathlonRelayRunHud_RotateMonPics(PokeathlonRelayRunHud *hud, int member);
BOOL PokeathlonRelayRunHud_RotateCpuMonPics(PokeathlonRelayRunHud *hud, u32 teamId);
void PokeathlonRelayRunHud_UpdateTeam(PokeathlonRelayRunHud *hud, u8 teamId, u8 member, u8 swapping, int condition);
void PokeathlonRelayRunHud_ShowTiredMarks(PokeathlonRelayRunHud *hud, u8 condition0, u8 condition1);
void PokeathlonRelayRunHud_SetTimer(PokeathlonRelayRunHud *hud, int frames);
void PokeathlonRelayRunHud_SetDrawPriorities(PokeathlonRelayRunHud *hud, int own);
u8 PokeathlonRelayRunHud_GetBenchMember(PokeathlonRelayRunHud *hud, int slot);

void PokeathlonRelayRunHud_CreatePicFrames(PokeathlonCourseSpritePool *spritePool, PokeathlonRelayRunHud *hud);
void PokeathlonRelayRunHud_InitWindows(PokeathlonRelayRunHud *hud);
struct PokeathlonRelayRunCpu *PokeathlonRelayRunCpu_New(enum HeapID heapId, int cpuCount, int mode);
void PokeathlonRelayRunCpu_SetTeam(struct PokeathlonRelayRunCpu *cpu, u8 index, PokeathlonRelayRunTeam *team);
void PokeathlonRelayRunCpu_SetObstacles(struct PokeathlonRelayRunCpu *cpu, s32 *layout);
void PokeathlonRelayRunCpu_Delete(struct PokeathlonRelayRunCpu *cpu);
u8 PokeathlonRelayRunCpu_Update(PokeathlonCourseData *course, struct PokeathlonRelayRunCpu *cpu, s32 timer);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RELAY_RUN_H
