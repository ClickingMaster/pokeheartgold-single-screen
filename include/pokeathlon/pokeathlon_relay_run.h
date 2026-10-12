#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_RELAY_RUN_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_RELAY_RUN_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_internal.h"

#include "bg_window.h"
#include "geometry_math.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"

// Relay Run (Pokeathlon event 6): each team runs laps around an oval track for 2700 frames (45 seconds). The
// player flicks the running Pokemon with the stylus to speed it up and swaps to the next member of the team
// before it runs out of energy. Walls on the track bounce the runners back, slow patches slow them down.
// The host steps every team and sends the result to everyone in a 0x28 packet; every player draws from it.
// The touch screen (PokeathlonRelayRunHud) shows the track map, the distance, the timer and the pictures of
// the player's members.

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

// One member of a team (0x1C).
typedef struct PokeathlonRelayRunMember {
    f32 acceleration; // 0x00: scales the speed a flick gives
    f32 weight;       // 0x04: scales it as well
    f32 pushPower;    // 0x08: share of its speed it passes on to a runner it bumps into
    f32 energy;       // 0x0C: goes down while it runs, up while it rests
    f32 maxEnergy;    // 0x10
    u8 patchSpeed;    // 0x14: fastest it runs on a slow patch (pixels per frame)
    u8 tiredSpeed;    // 0x15: fastest it runs when tired
    u8 speedLimit;    // 0x16: fastest it runs; the speed grades are measured against it
    u8 recoveryTime;  // 0x17: frames an exhausted member needs to recover
    u8 condition;     // 0x18: RELAY_RUN_FINE / _TIRED / _EXHAUSTED
    u8 unk19[3];      // 0x19
} PokeathlonRelayRunMember;

// A team (0xD4): the three Pokemon of one participant, one running at a time.
typedef struct PokeathlonRelayRunTeam {
    void *monObjects[RELAY_RUN_MEMBERS];                 // 0x00: PokeathlonCourseMonObject
    fx32 distance[RELAY_RUN_MEMBERS];                    // 0x0C: distance each member ran
    PokeathlonRelayRunMember members[RELAY_RUN_MEMBERS]; // 0x18
    void *conditionSprite;                               // 0x6C: tired / exhausted marks above the runner
    void *shadowSprite;                                  // 0x70
    void *swapSprite;                                    // 0x74: flies up from the leaving member and down with the next
    void *speedSprite;                                   // 0x78: dash lines
    VecFx32 position;                                    // 0x7C: x along the lap (wraps at RELAY_RUN_TRACK_LENGTH), y on the track
    u8 animState;                                        // 0x88
    u8 unk89[2];                                         // 0x89
    u8 current;                                          // 0x8B: member running
    VecFx32 velocity;                                    // 0x8C
    u8 bumpedInto[RELAY_RUN_TEAMS];                      // 0x98: touching that team's runner since the last bump
    u8 laps;                                             // 0x9C
    u8 swapping;                                         // 0x9D: handing over to the next member
    u8 swapPhase;                                        // 0x9E: 1 swap, 2 wait for the display, 0 done
    u8 shownSwapPhase;                                   // 0x9F: drawing: the swap phase being animated
    u8 swapStep;                                         // 0xA0: drawing: step of that animation
    u8 swapTimer;                                        // 0xA1
    u8 speedGrade;                                       // 0xA2: 0 standing, 1 slow, 2 fast, 3 at its limit
    u8 tired;                                            // 0xA3: the running member ran out of energy; it is capped
    u8 recoveryTimer;                                    // 0xA4
    u8 onPatch;                                          // 0xA5: on a slow patch this frame
    u8 lastCondition;                                    // 0xA6: drawing: the condition shown last frame
    u8 lastSpeedGrade;                                   // 0xA7: drawing: the speed grade shown last frame
    u8 bestLaps;                                         // 0xA8
    u8 stunTimer;                                        // 0xA9: can't be moved or flicked while it runs
    u8 flicked;                                          // 0xAA: the stylus was lifted after a short drag
    u8 unkAB;                                            // 0xAB
    int heldFrames;                                      // 0xAC: frames the stylus has been held
    VecFx32 dragStart;                                   // 0xB0
    VecFx32 dragEnd;                                     // 0xBC
    BOOL runnerHidden;                                   // 0xC8: drawing: hidden during a swap
    int wallState;                                       // 0xCC: what it ran into last frame (RELAY_RUN_HIT_*)
    u8 owner;                                            // 0xD0: participant
    u8 behindStart;                                      // 0xD1: ran backwards over the start line with no laps
    u8 speedSoundGrade;                                  // 0xD2: drawing: speed grade the dash sound was played for
    u8 speedSoundPlayed;                                 // 0xD3
} PokeathlonRelayRunTeam;                                // size: 0xD4

// What the host keeps of one participant's stylus (0xC).
typedef struct PokeathlonRelayRunPress {
    BOOL pressed; // 0x0: on the frame the stylus touches the screen
    BOOL held;    // 0x4
    u32 unk8;     // 0x8
} PokeathlonRelayRunPress;

// An obstacle on the track (0xC). The host loads the obstacle layout (NARC a/2/2/7, one of ten).
typedef struct PokeathlonRelayRunObstacle {
    BOOL active;  // 0x0
    void *sprite; // 0x4: PokeathlonCourseSpritePoolSprite
    u16 x;        // 0x8: along the lap
    u8 y;         // 0xA
    u8 kind;      // 0xB: RELAY_RUN_OBSTACLE_*
} PokeathlonRelayRunObstacle;

// The outline of an obstacle for collisions (0x64, PokeathlonRelayRun_GetObstacleShape).
typedef struct PokeathlonRelayRunObstacleShape {
    s32 kind;            // 0x00
    Point2D corners[4];  // 0x04: top left, top right, bottom right, bottom left (a slow patch: top left only)
    Point2D edges[4][2]; // 0x24: top, right, bottom, left, moved out by the runner's radius
} PokeathlonRelayRunObstacleShape;

// What touching each edge or corner of a wall counts as (RELAY_RUN_HIT_*).
typedef struct PokeathlonRelayRunHitTable {
    int hits[4];
} PokeathlonRelayRunHitTable;

// The event's stat tables (NARC a/1/7/0 member 2): tables[n][a Pokemon stat] for each member value.
typedef struct PokeathlonRelayRunSetup {
    int tables[8][5];
} PokeathlonRelayRunSetup;

// The lap count shown for a second when it goes up (0x10).
typedef struct PokeathlonRelayRunLapCounter {
    Sprite *digits[2]; // 0x0
    void *plate;       // 0x8: PokeathlonCourseSpritePoolSprite
    u16 timer;         // 0xC
    u16 laps;          // 0xE
} PokeathlonRelayRunLapCounter;

// The stylus of a player (0x24).
typedef struct PokeathlonRelayRunTouch {
    BOOL held;       // 0x00
    u8 x;            // 0x04
    u8 y;            // 0x05
    u8 unk06[2];     // 0x06
    u8 displayPhase; // 0x08: the swap animation reached this phase on the player's screen
    u8 unk09[0x1B];  // 0x09
} PokeathlonRelayRunTouch;

// A block of the link data holding a stylus (0x28).
typedef struct PokeathlonRelayRunTouchBlock {
    PokeathlonRelayRunTouch touch; // 0x00
    u8 unk24[4];                   // 0x24
} PokeathlonRelayRunTouchBlock;

// Everything a player draws, sent by the host every frame (0x28).
typedef struct PokeathlonRelayRunPacket {
    u16 x[RELAY_RUN_TEAMS];        // 0x00
    u8 y[RELAY_RUN_TEAMS];         // 0x08
    u8 runner[RELAY_RUN_TEAMS];    // 0x0C: current member | speed grade << 2
    u8 laps[RELAY_RUN_TEAMS];      // 0x10: laps | swapping << 7
    s32 flags;                     // 0x14: condition of each member (2 bits, team * 3 + member), finished << 24,
                                   //       bump << 25, behindStart of each team << 26
    u16 timer;                     // 0x18
    u16 bumpX;                     // 0x1A
    u8 bumpY;                      // 0x1C
    u8 swapPhases;                 // 0x1D: swapPhase of each team (2 bits each)
    u8 swapSigns[RELAY_RUN_TEAMS]; // 0x1E: "change" sign shown at the top of each player's screen
    u8 unk22[6];                   // 0x22
} PokeathlonRelayRunPacket;

// The link data of the event (PokeathlonCourse_GetDataCopyArea).
typedef struct PokeathlonRelayRunLink {
    PokeathlonRelayRunTouchBlock touch;                    // 0x000: this player's stylus
    PokeathlonRelayRunPacket packetToSend;                 // 0x028: host
    PokeathlonRelayRunTouchBlock touches[RELAY_RUN_TEAMS]; // 0x050: everyone's stylus, collected by the host
    PokeathlonRelayRunPacket packet;                       // 0x0F0: received from the host
} PokeathlonRelayRunLink;

typedef struct PokeathlonRelayRun {
    BgConfig *bgConfig;                                        // 0x000
    Window window;                                             // 0x004
    enum HeapID heapId;                                        // 0x014
    void *spritePool;                                          // 0x018: PokeathlonCourseSpritePool
    void *signSprite;                                          // 0x01C
    PokeathlonRelayRunLapCounter lapCounter;                   // 0x020
    PokeathlonRelayRunTeam teams[RELAY_RUN_TEAMS];             // 0x030
    PokeathlonRelayRunPress presses[RELAY_RUN_TEAMS];          // 0x380
    BOOL dragging[RELAY_RUN_TEAMS];                            // 0x3B0
    u8 finished;                                               // 0x3C0
    u8 unk3C1;                                                 // 0x3C1
    u8 heldTooLong;                                            // 0x3C2: someone held the stylus down for 255 frames: more friction
    u8 nextBumpMark;                                           // 0x3C3
    void *monResources;                                        // 0x3C4: PokeathlonCourseSpriteResources
    void *monObjects;                                          // 0x3C8: PokeathlonCourseMonObjects
    void *bumpMarks[4];                                        // 0x3CC
    struct PokeathlonRelayRunCpu *cpu;                         // 0x3DC: the teams without a player
    struct PokeathlonRelayRunHud *hud;                         // 0x3E0: the touch screen
    s32 layoutToSend[RELAY_RUN_OBSTACLES];                     // 0x3E4: host: kind | y << 8 | x << 16
    s32 layout[RELAY_RUN_OBSTACLES];                           // 0x45C: received
    PokeathlonRelayRunObstacle obstacles[RELAY_RUN_OBSTACLES]; // 0x4D4
    u8 bumped;                                                 // 0x63C: two runners bumped into each other this frame
    u8 bumpY;                                                  // 0x63D
    u16 bumpX;                                                 // 0x63E
    u16 timer;                                                 // 0x640: frames left
    u16 signSoundPlayed;                                       // 0x642
    u8 swapSigns[RELAY_RUN_TEAMS];                             // 0x644: the sign at the top of each player's screen
} PokeathlonRelayRun;                                          // size: 0x648

// A team on the touch screen's track map. The teams are 0x20 apart, but only their first 0x11 bytes are theirs:
// the last team's other bytes are the start of PokeathlonRelayRunHud.map.icons.files.
typedef struct PokeathlonRelayRunHudTeam {
    void *icon;          // 0x00: its running member's icon (PokeathlonCourseSpritePoolSprite)
    void *conditionMark; // 0x04: tired / exhausted
    void *swapMark;      // 0x08: shown instead of the icon while it swaps
    void *colorMark;     // 0x0C: the team's colour
    u8 lastCondition;    // 0x10
    u8 unk11[0xF];       // 0x11
} PokeathlonRelayRunHudTeam;

// A member's picture (char and palette data, loaded into one of the two picture slots when needed).
typedef struct PokeathlonRelayRunHudPic {
    void *chars;
    void *palette;
} PokeathlonRelayRunHudPic;

// The touch screen (0x1A0).
typedef struct PokeathlonRelayRunHud {
    enum HeapID heapId;            // 0x000
    PokeathlonCourseData *course;  // 0x004
    BgConfig *bgConfig;            // 0x008
    Window nameWindow;             // 0x00C: "Next {name}"
    Window lapsWindow;             // 0x01C: "Laps"
    MsgData *msgData;              // 0x02C: msg_0309
    MessageFormat *msgFormat;      // 0x030
    void *digitScreenFile;         // 0x034
    NNSG2dScreenData *digitScreen; // 0x038: the big digits PokeathlonRelayRunHud_PrintDistance copies from
    u8 unk03C[0xC];                // 0x03C

    union {
        PokeathlonRelayRunHudTeam teams[RELAY_RUN_TEAMS];

        struct {
            u8 unk00[0x74];
            void *files[RELAY_RUN_TEAMS * RELAY_RUN_MEMBERS]; // 0x0BC: the icon files (NARC poketool/icongra)
        } icons;
    } map; // 0x048

    NNSG2dCharacterData *iconChars[RELAY_RUN_TEAMS * RELAY_RUN_MEMBERS]; // 0x0EC
    u8 iconPalettes[RELAY_RUN_TEAMS * RELAY_RUN_MEMBERS];                // 0x11C
    PokeathlonRelayRunHudPic pics[RELAY_RUN_MEMBERS];                    // 0x128: the player's own members
    u32 picCharLocations[2];                                             // 0x140: VRAM of the two picture slots
    u32 picPaletteLocations[2];                                          // 0x148
    void *picFrames[2];                                                  // 0x150: the picture slots (PokeathlonCourseSpritePoolSprite)
    void *tiredMarks[2];                                                 // 0x158
    u16 benchMembers[2];                                                 // 0x160: member in each picture slot
    u32 rotateState;                                                     // 0x164: PokeathlonRelayRunHud_RotateMonPics
    int rotateSlot;                                                      // 0x168
    u8 shownMembers[RELAY_RUN_TEAMS];                                    // 0x16C: member whose icon each team shows
    String *names[RELAY_RUN_MEMBERS];                                    // 0x170: the player's members' nicknames
    Sprite *timerSprites[6];                                             // 0x17C: minutes, ':', tens, ones, '.', tenths

    struct {
        u16 y;
        u16 step;
    } cpuRotations[RELAY_RUN_TEAMS - 1]; // 0x194: PokeathlonRelayRunHud_RotateCpuMonPics
} PokeathlonRelayRunHud;                 // size: 0x1A0

BOOL PokeathlonRelayRun_Init(PokeathlonCourseData *course);
BOOL PokeathlonRelayRun_Draw(PokeathlonCourseData *course);
BOOL PokeathlonRelayRun_ShowResults(PokeathlonCourseData *course);
BOOL PokeathlonRelayRun_Exit(PokeathlonCourseData *course);
void PokeathlonRelayRun_SetGfxBanks(void);
void PokeathlonRelayRun_InitBgs(BgConfig *bgConfig);
void PokeathlonRelayRun_LoadBgGraphics(PokeathlonRelayRun *relayRun);
void PokeathlonRelayRun_LoadSpriteGraphics(void *spritePool);
void PokeathlonRelayRun_CreateSprites(PokeathlonRelayRun *relayRun, void *spritePool);
BOOL PokeathlonRelayRun_Start(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRelayRun_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRelayRun_SaveScores(PokeathlonCourseData *course, u8 *state);
void PokeathlonRelayRun_UpdateHost(PokeathlonCourseData *course);
void PokeathlonRelayRun_PackPacket(PokeathlonRelayRun *relayRun, PokeathlonRelayRunPacket *packet);
BOOL PokeathlonRelayRun_Update(PokeathlonCourseData *course);
void PokeathlonRelayRun_DrawPacket(PokeathlonCourseData *course);
BOOL PokeathlonRelayRun_IsTouchingRunner(PokeathlonRelayRun *relayRun, int teamId, int x, int y);
BOOL PokeathlonRelayRun_IsTouchingSwapButton(PokeathlonRelayRun *relayRun, u32 x, u32 y);
void PokeathlonRelayRun_MoveTeams(PokeathlonCourseData *course);
void PokeathlonRelayRunTeam_UpdateStun(PokeathlonCourseData *course, PokeathlonRelayRun *relayRun, PokeathlonRelayRunTeam *team);

// pokeathlon_relay_run_track.c
void PokeathlonRelayRun_DrawTeams(PokeathlonCourseData *course, int own, PokeathlonRelayRunTeam *team, PokeathlonRelayRunLink *link);
void PokeathlonRelayRun_ScrollObstacles(PokeathlonRelayRun *relayRun, int x);
void PokeathlonRelayRunTeam_AnimateLeave(PokeathlonRelayRunHud *hud, PokeathlonRelayRunTeam *team, VecFx32 *position, u8 teamId, u8 member, PokeathlonRelayRunTouch *touch, BOOL isHostForCpu, BOOL isOwn);
void PokeathlonRelayRunTeam_AnimateArrive(PokeathlonRelayRunTeam *team, VecFx32 *position, u8 unused, u8 member, PokeathlonRelayRunTouch *touch, BOOL reportPhase, BOOL isOwn);
void PokeathlonRelayRunTeam_EndSwap(PokeathlonRelayRunTeam *team, u8 unused, u8 member, PokeathlonRelayRunTouch *touch, BOOL reportPhase, int isOwn);
void PokeathlonRelayRunTeam_DrawSwap(PokeathlonRelayRunHud *hud, PokeathlonRelayRunTeam *team, VecFx32 *ground, PokeathlonRelayRunPacket *packet, u8 teamId, u8 member, PokeathlonRelayRunTouch *touch, int isHost, int isOwn, int isCpu);
void PokeathlonRelayRun_InitMember(PokeathlonCourseData *course, void *setupFile, u8 participant, u8 member, PokeathlonRelayRunTeam *team);
void PokeathlonRelayRun_FindNearbyObstacles(PokeathlonRelayRun *relayRun, fx32 x, s32 **walls, s32 **patches, u8 *wallCount, u8 *patchCount);
void PokeathlonRelayRun_GetObstacleShape(int inset, const s32 *entry, volatile s32 *shape);
int PokeathlonRelayRun_CollideWithObstacle(VecFx32 *start, VecFx32 *end, int radius, PokeathlonRelayRunObstacleShape *shape, VecFx32 *out);
BOOL PokeathlonRelayRun_IsOnSlowPatch(const VecFx32 *position, const PokeathlonRelayRunObstacleShape *shape);
u16 PokeathlonRelayRun_CollideWithObstacles(PokeathlonRelayRun *relayRun, int wallState, int radius, VecFx32 *from, VecFx32 *to, VecFx32 *out);
fx32 PokeathlonRelayRun_CalcCircleEntry(const VecFx32 *a, const VecFx32 *b, const VecFx32 *c, fx32 r);
f32 PokeathlonRelayRunTeam_GetMaxSpeed(PokeathlonRelayRunTeam *team, int member);
void PokeathlonRelayRun_AddFlick(const VecFx32 *velocity, const VecFx32 *flick, VecFx32 *out);
BOOL PokeathlonRelayRun_CollideRunners(PokeathlonCourseData *course, PokeathlonRelayRunTeam *teams, VecFx32 *where);
void PokeathlonRelayRun_PlaceAboveMon(PokeathlonCoursePerformance *performance, const VecFx32 *position, void *sprite);
void PokeathlonRelayRunObstacle_SetDrawPriority(PokeathlonRelayRunObstacle *obstacle);
void PokeathlonRelayRunTeam_SetDrawPriority(PokeathlonRelayRunTeam *team, u8 y, int isOwn);
void PokeathlonRelayRun_UpdateStartLine(BgConfig *bgConfig, int x);

// pokeathlon_relay_run_hud.c
void PokeathlonRelayRun_UpdateLapCounter(PokeathlonRelayRunLapCounter *counter, u8 laps);
void PokeathlonRelayRun_AddResultWindow(PokeathlonRelayRun *relayRun);
void PokeathlonRelayRun_PrintResult(PokeathlonRelayRun *relayRun, int laps, int x);
void PokeathlonRelayRun_PlaySEIfOnScreen(int x, int seqNo, int isOwn);
void PokeathlonRelayRun_UpdateSwapSign(PokeathlonRelayRun *relayRun, u8 teamId, int touchingSwapButton);
void PokeathlonRelayRun_SaveResult(PokeathlonCourseData *course);
PokeathlonRelayRunHud *PokeathlonRelayRunHud_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
void PokeathlonRelayRunHud_Free(PokeathlonRelayRunHud *hud);
void PokeathlonRelayRunHud_LoadBgGraphics(PokeathlonRelayRunHud *hud);
void PokeathlonRelayRunHud_LoadSpriteGraphics(PokeathlonRelayRunHud *hud, void *spritePool);
void PokeathlonRelayRunHud_CreateSprites(PokeathlonRelayRunHud *hud, void *spritePool, PokeathlonCourseSpriteResources *monResources);
void PokeathlonRelayRunHud_LoadMonIcons(PokeathlonRelayRunHud *hud, PokeathlonCoursePoint *mons);
void PokeathlonRelayRunHud_LoadMonPics(PokeathlonRelayRunHud *hud, const PokeathlonCourseMonInfo *mon);
void PokeathlonRelayRunHud_ShowMonPic(PokeathlonRelayRunHud *hud, u32 slot, u32 member);
void PokeathlonRelayRunHud_SetMonIcon(PokeathlonRelayRunHud *hud, u8 teamId, int member);
void PokeathlonRelayRunHud_SetMapPosition(PokeathlonRelayRunHud *hud, u8 teamId, u16 x);
void PokeathlonRelayRunHud_PrintDistance(PokeathlonRelayRunHud *hud, u8 laps, u32 x);
BOOL PokeathlonRelayRunHud_RotateMonPics(PokeathlonRelayRunHud *hud, int member);
BOOL PokeathlonRelayRunHud_RotateCpuMonPics(PokeathlonRelayRunHud *hud, u32 teamId);
void PokeathlonRelayRunHud_UpdateTeam(PokeathlonRelayRunHud *hud, u8 teamId, u8 member, u8 swapping, int condition);
void PokeathlonRelayRunHud_ShowTiredMarks(PokeathlonRelayRunHud *hud, u8 condition0, u8 condition1);
void PokeathlonRelayRunHud_SetTimer(PokeathlonRelayRunHud *hud, int frames);
void PokeathlonRelayRunHud_SetDrawPriorities(PokeathlonRelayRunHud *hud, int own);
u8 PokeathlonRelayRunHud_GetBenchMember(PokeathlonRelayRunHud *hud, int slot);
void PokeathlonRelayRunHud_PrintMonName(PokeathlonRelayRunHud *hud, s32 member);
void PokeathlonRelayRunHud_PrintLaps(PokeathlonRelayRunHud *hud);

// Defined further on in the event (still under their address names).
void ov96_02200C8C(void *spritePool, PokeathlonRelayRunHud *hud);
void PokeathlonRelayRunHud_InitWindows(PokeathlonRelayRunHud *hud);
struct PokeathlonRelayRunCpu *PokeathlonRelayRunCpu_New(enum HeapID heapId, int cpuCount, int mode);
void PokeathlonRelayRunCpu_SetTeam(struct PokeathlonRelayRunCpu *cpu, u8 index, PokeathlonRelayRunTeam *team);
void PokeathlonRelayRunCpu_SetObstacles(struct PokeathlonRelayRunCpu *cpu, s32 *layout);
void PokeathlonRelayRunCpu_Delete(struct PokeathlonRelayRunCpu *cpu);
u8 PokeathlonRelayRunCpu_Update(PokeathlonCourseData *course, struct PokeathlonRelayRunCpu *cpu, s32 timer);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RELAY_RUN_H
