#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_GOAL_ROLL_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_GOAL_ROLL_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "geometry_math.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task_api.h"

// Goal Roll (Pokeathlon event 9): the four participants' teams of three Pokemon share one field
// with a goal in each corner. Touching a Pokemon and dragging moves it; a Pokemon that dashes into
// a ball kicks it away. A ball that rolls into a goal scores for the team that kicked it last
// (1 point, 2 for the big ball) and takes a point from the goal's owner; an own goal only costs a
// point. Field coordinates are whole pixels of the 256 x 192 touch screen (fx32). The match lasts
// 2700 frames; the host simulates the field and sends a frame packet each frame; the participants
// without a player are steered by a simple CPU.

#define GOAL_ROLL_PARTICIPANTS 4
#define GOAL_ROLL_TEAM_SIZE    3
#define GOAL_ROLL_MONS         (GOAL_ROLL_PARTICIPANTS * GOAL_ROLL_TEAM_SIZE)
#define GOAL_ROLL_BALLS        2
#define GOAL_ROLL_NO_MON       12 // "no Pokemon" in the 0..11 mon indices

// Sprite pool of the event (the course sprite helpers' view of it).
typedef struct PokeathlonGoalRollSprite {
    u16 active;     // 0x00
    u16 shown;      // 0x02
    Sprite *sprite; // 0x04
    u16 kind;       // 0x08
    u16 id;         // 0x0A
} PokeathlonGoalRollSprite;

typedef struct PokeathlonGoalRollSpritePoolConfig {
    s32 capacity;        // 0x00
    s32 resourceCount;   // 0x04
    u8 managerCounts[4]; // 0x08
} PokeathlonGoalRollSpritePoolConfig;

typedef struct PokeathlonGoalRollSpritePool {
    u32 heapId;                                // 0x00
    s32 used;                                  // 0x04
    PokeathlonGoalRollSpritePoolConfig config; // 0x08
    void *spriteList;                          // 0x14
    G2dRenderer renderer;                      // 0x18
    void *resources[4];
    u8 *resourceRecords;
    PokeathlonGoalRollSprite *records;
} PokeathlonGoalRollSpritePool;

// A Pokemon's object in the course's 3D object list (PokeathlonCourseMonObjects_Get).
typedef struct PokeathlonGoalRollMonObject {
    void *object;              // 0x00
    void *sprite1;             // 0x04
    void *sprite2;             // 0x08
    u8 unk0C[4];               // 0x0C
    s32 offsetY;               // 0x10
    s32 offsetX;               // 0x14
    s32 halfHeight;            // 0x18
    s32 radius;                // 0x1C
    s32 x;                     // 0x20
    s32 y;                     // 0x24
    s32 center;                // 0x28
    s32 bottom;                // 0x2C
    s32 height;                // 0x30
    u32 state;                 // 0x34
    u32 anim;                  // 0x38
    s32 priority;              // 0x3C
    u32 hideSecondary;         // 0x40
} PokeathlonGoalRollMonObject; // size: 0x44

typedef struct PokeathlonGoalRollMonObjects {
    u32 unk00;                                           // 0x00
    int unk04;                                           // 0x04
    int unk08;                                           // 0x08
    u32 unk0C;                                           // 0x0C
    void *unk10;                                         // 0x10
    void *unk14;                                         // 0x14
    void *unk18;                                         // 0x18
    PokeathlonGoalRollMonObject entries[GOAL_ROLL_MONS]; // 0x1C
} PokeathlonGoalRollMonObjects;

typedef struct PokeathlonGoalRollResources {
    int heapId;
    int mask;
    int param;
    void *managers[4];
    u8 unk1C[0x98];
} PokeathlonGoalRollResources;

// mode of a Pokemon
#define GOAL_ROLL_MODE_WALK    0
#define GOAL_ROLL_MODE_DASH    1 // released after a short touch: kicks the balls it runs into
#define GOAL_ROLL_MODE_STUNNED 2 // hit by a kicked ball without power left
#define GOAL_ROLL_MODE_TIRED   3 // out of power: walks slowly

// motion of a Pokemon (pushed back by a ball or after a kick)
#define GOAL_ROLL_MOTION_NONE      0
#define GOAL_ROLL_MOTION_RECOIL    1 // after kicking a ball
#define GOAL_ROLL_MOTION_2         2
#define GOAL_ROLL_MOTION_KNOCKBACK 3 // hit by a kicked ball: spins while pushed back

// One Pokemon on the field.
typedef struct PokeathlonGoalRollMon {
    PokeathlonGoalRollMonObject *object;       // 0x00
    PokeathlonGoalRollSprite *dashSprite;      // 0x04: shown while dashing or stunned
    PokeathlonGoalRollSprite *trailSprites[3]; // 0x08: dash trail, used in turn
    s32 startX;                                // 0x14
    s32 startY;                                // 0x18
    u32 unk1C;                                 // 0x1C
    int radius;                                // 0x20
    VecFx32 target;                            // 0x24: where it walks to
    VecFx32 position;                          // 0x30
    VecFx32 direction;                         // 0x3C
    int motion;                                // 0x48
    VecFx32 motionStep;                        // 0x4C
    s16 motionFrames;                          // 0x58
    s16 motionWait;                            // 0x5A
    u16 facing;                                // 0x5C: 1..4
    u16 goals;                                 // 0x5E
    u8 stunTimer;                              // 0x60
    u8 recoverTimer;                           // 0x61
    u8 trailIndex;                             // 0x62
    u8 trailCooldown;                          // 0x63
    fx32 walkSpeed;                            // 0x64
    fx32 dashSpeed;                            // 0x68
    u16 kickAngle;                             // 0x6C: half width of the kick cone, in degrees
    u16 unk6E;                                 // 0x6E
    u8 maxPower;                               // 0x70
    u8 power;                                  // 0x71: lost when hit by a kicked ball, recovers over time
    u8 kickPower;                              // 0x72
    u8 kickRow;                                // 0x73: row of the ball speed tables
    u8 unk74;                                  // 0x74
    u8 kickLevel;                              // 0x75: how long a kicked ball keeps going (ov96_0221D640)
    u8 unk76[2];                               // 0x76
    int mode;                                  // 0x78
} PokeathlonGoalRollMon;                       // size: 0x7C

// ball kind
#define GOAL_ROLL_BALL_NONE  0
#define GOAL_ROLL_BALL_SMALL 1 // radius 8, 1 point
#define GOAL_ROLL_BALL_BIG   2 // radius 12, 2 points

// ball state
#define GOAL_ROLL_BALL_ROLLING 0
#define GOAL_ROLL_BALL_SCORED  1
#define GOAL_ROLL_BALL_KICKED  2
#define GOAL_ROLL_BALL_WAITING 3

typedef struct PokeathlonGoalRollBall {
    PokeathlonGoalRollSprite *sprite;       // 0x00
    PokeathlonGoalRollSprite *effectSprite; // 0x04
    VecFx32 position;                       // 0x08
    VecFx32 velocity;                       // 0x14
    VecFx32 direction;                      // 0x20
    u8 touching[GOAL_ROLL_MONS];            // 0x2C
    u8 kind;                                // 0x38
    u8 state;                               // 0x39
    u8 timer;                               // 0x3A
    u8 lastKicker;                          // 0x3B: mon index, GOAL_ROLL_NO_MON if none
    u8 step;                                // 0x3C
    u8 steps;                               // 0x3D
    u8 speedTable;                          // 0x3E
    u8 scored;                              // 0x3F
    u8 speed;                               // 0x40
    u8 speedRow;                            // 0x41
    u8 spawnPoint;                          // 0x42: where it last appeared (ov96_0221D438)
    u8 wait;                                // 0x43
    u8 shownState;                          // 0x44: state shown last (every participant)
    u8 strength;                            // 0x45
    u16 kickLevel;                          // 0x46: of the last kick
    int bounced;                            // 0x48
} PokeathlonGoalRollBall;                   // size: 0x4C

// Blinking name plate of a participant (top of the touch screen).
typedef struct PokeathlonGoalRollPlate {
    u8 state;                         // 0x00
    u8 id;                            // 0x01
    volatile u8 timer;                // 0x02
    s8 unk03;                         // 0x03
    SysTask *task;                    // 0x04
    struct PokeathlonGoalRoll *owner; // 0x08
    u8 delay;                         // 0x0C
    u8 toggle;                        // 0x0D
    u8 gained;                        // 0x0E: 1 = scored, 0 = lost a point
    u8 bigBall;                       // 0x0F
} PokeathlonGoalRollPlate;            // size: 0x10

// A participant's touch input state.
typedef struct PokeathlonGoalRollTouch {
    int newPress; // 0x00: set on the first frame of a touch
    int held;     // 0x04
    int unk08;    // 0x08
} PokeathlonGoalRollTouch;

// The top screen (timer, the own name).
typedef struct PokeathlonGoalRollDisplay {
    enum HeapID heapId;                         // 0x00
    PokeathlonCourseData *course;               // 0x04
    Sprite *timerDigits[2];                     // 0x08
    BgConfig *bgConfig;                         // 0x10
    Window nameWindow;                          // 0x14
    Window messageWindow;                       // 0x24
    MsgData *msgData;                           // 0x34
    MessageFormat *msgFormat;                   // 0x38
    NNSG2dScreenData *digitScreen;              // 0x3C
    void *digitScreenData;                      // 0x40
    u32 ownScore;                               // 0x44
    u32 secondWindShown;                        // 0x48: "Second Wind!" (the second ball) shown instead of the timer
    PokeathlonGoalRollSprite *timerLabelSprite; // 0x4C: hidden once the message replaces the timer
} PokeathlonGoalRollDisplay;                    // size: 0x50

// The CPU: per participant the order in which it moves its three Pokemon.
typedef struct PokeathlonGoalRollCpuSlot {
    u32 wait;
    PokeathlonGoalRollMon *mon;
} PokeathlonGoalRollCpuSlot;

typedef struct PokeathlonGoalRollCpuTeam {
    PokeathlonGoalRollCpuSlot order[3]; // 0x00: [0] the striker, then the other two
    PokeathlonGoalRollMon *mons[3];     // 0x18
} PokeathlonGoalRollCpuTeam;            // size: 0x24

typedef struct PokeathlonGoalRollCpu {
    PokeathlonGoalRollCpuTeam teams[GOAL_ROLL_PARTICIPANTS]; // 0x00
    PokeathlonGoalRollBall *balls;                           // 0x90
    u32 run;                                                 // 0x94: set every `interval` frames
    u8 cpuCount;                                             // 0x98
    u8 current;                                              // 0x99
    u8 timer;                                                // 0x9A
    u8 interval;                                             // 0x9B
    u8 orderSlot;                                            // 0x9C
    u8 firstCpu;                                             // 0x9D
    u8 humanCount;                                           // 0x9E
    u8 unk9F;                                                // 0x9F
} PokeathlonGoalRollCpu;                                     // size: 0xA0

// Screen shake (Ring Drop; the task is defined with the Goal Roll CPU code).
typedef struct PokeathlonScreenShake {
    SpriteSystem *spriteSystem; // 0x00
    BgConfig *bgConfig;         // 0x04
    SysTask *task;              // 0x08
    fx32 offset;                // 0x0C
    fx32 step;                  // 0x10
    s8 frame;                   // 0x14
    u8 duration;                // 0x15
    u8 magnitude;               // 0x16
    u8 unk17;                   // 0x17
    u32 direction;              // 0x18
} PokeathlonScreenShake;

// NARC a/1/7/0 member 8: the move parameters per stat level (indexed by the Pokemon's levels).
typedef struct PokeathlonGoalRollMonParams {
    s32 kickAngle[5];
    s32 unk6E[5];
    s32 power[5];
    s32 walkSpeed[5]; // in tenths
    s32 dashSpeed[5]; // in tenths
    s32 kickPower[5];
} PokeathlonGoalRollMonParams;

// A participant's input (in the course's data copy area).
typedef struct PokeathlonGoalRollInput {
    u32 touching; // 0x00
    u8 x;         // 0x04
    u8 y;         // 0x05
} PokeathlonGoalRollInput;

// The frame the host sends every frame.
typedef struct PokeathlonGoalRollFramePacket {
    u8 monX[GOAL_ROLL_MONS];     // 0x00
    u8 monY[GOAL_ROLL_MONS];     // 0x0C
    u8 ballX[GOAL_ROLL_BALLS];   // 0x18
    u8 ballY[GOAL_ROLL_BALLS];   // 0x1A: see PokeathlonGoalRoll_EncodeBallY
    s32 states;                  // 0x1C: 2 bits facing - 1 per mon, 2 bits state per ball (<< 24),
                                 //       score slot (<< 28), big ball (bit 30), second ball active (bit 31)
    s32 flags;                   // 0x20: 2 bits mode per mon, time up (bit 24), score (<< 25)
} PokeathlonGoalRollFramePacket; // size: 0x24

// ov96_0221D1F4: the plate flash palettes (two for a lost point, two for a goal), followed by the
// total points window template (ov96_0221D1F8) and the four goal palettes.
typedef struct PokeathlonGoalRollWindowData {
    WindowTemplate window;
    u16 goalPalettes[GOAL_ROLL_PARTICIPANTS];
} PokeathlonGoalRollWindowData;

typedef struct PokeathlonGoalRollPaletteData {
    u8 plateFlash[4];
    PokeathlonGoalRollWindowData data;
} PokeathlonGoalRollPaletteData;

// The event's work (PokeathlonCourse_GetHeapAllocPtr4).
typedef struct PokeathlonGoalRoll {
    PokeathlonGoalRollSpritePool *spritePool;                                // 0x000
    BgConfig *bgConfig;                                                      // 0x004
    Window scoreWindow;                                                      // 0x008
    Window nameWindows[GOAL_ROLL_PARTICIPANTS];                              // 0x018
    enum HeapID heapId;                                                      // 0x058
    PokeathlonGoalRollMon mons[GOAL_ROLL_PARTICIPANTS][GOAL_ROLL_TEAM_SIZE]; // 0x05C
    PokeathlonGoalRollBall balls[GOAL_ROLL_BALLS];                           // 0x62C
    PokeathlonGoalRollSprite *goalSprites[GOAL_ROLL_PARTICIPANTS];           // 0x6C4
    PokeathlonGoalRollSprite *goalFlashSprites[GOAL_ROLL_PARTICIPANTS];      // 0x6D4
    PokeathlonGoalRollSprite *goalLabelSprites[GOAL_ROLL_PARTICIPANTS];      // 0x6E4
    s32 scores[GOAL_ROLL_PARTICIPANTS];                                      // 0x6F4
    PokeathlonGoalRollTouch touch[GOAL_ROLL_PARTICIPANTS];                   // 0x704
    u8 heldMon[GOAL_ROLL_PARTICIPANTS];                                      // 0x734: mon index touched by each participant
    int timer;                                                               // 0x738: frames left
    u8 packetScoreSlot;                                                      // 0x73C: whose score goes into the next frame packet
    u8 unk73D;                                                               // 0x73D
    u8 timeUp;                                                               // 0x73E
    u8 unk73F[3];                                                            // 0x73F
    u8 holdFrames;                                                           // 0x742
    u8 bigBallMisses;                                                        // 0x743
    PokeathlonGoalRollResources *resources;                                  // 0x744
    PokeathlonGoalRollMonObjects *monObjects;                                // 0x748
    PokeathlonGoalRollCpu *cpu;                                              // 0x74C
    PokeathlonGoalRollDisplay *display;                                      // 0x750
    VecFx32 walls[8];                                                        // 0x754
    u8 ballInGoalArea[4];                                                    // 0x7B4: per ball, for the sound
    String *plateStrings[3];                                                 // 0x7B8
    String *nameStrings[GOAL_ROLL_PARTICIPANTS];                             // 0x7C4
    PokeathlonGoalRollPlate plates[GOAL_ROLL_PARTICIPANTS];                  // 0x7D4
    u8 shownScores[GOAL_ROLL_PARTICIPANTS];                                  // 0x814
    u8 leading[GOAL_ROLL_PARTICIPANTS];                                      // 0x818
    void *sortWork;                                                          // 0x81C
} PokeathlonGoalRoll;                                                        // size: 0x820

// pokeathlon_goal_roll.c
BOOL PokeathlonGoalRoll_Init(PokeathlonCourseData *course);
BOOL PokeathlonGoalRoll_Draw(PokeathlonCourseData *course);
BOOL PokeathlonGoalRoll_ShowResults(PokeathlonCourseData *course);
BOOL PokeathlonGoalRoll_Exit(PokeathlonCourseData *course);
void PokeathlonGoalRoll_SetGfxBanks(void);
BOOL PokeathlonGoalRoll_State_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonGoalRoll_State_Finish(PokeathlonCourseData *course, u8 *state);
void PokeathlonGoalRoll_InitBgs(BgConfig *bgConfig);
void PokeathlonGoalRoll_LoadBgGfx(PokeathlonGoalRoll *goalRoll);
BOOL PokeathlonGoalRoll_HandleInput(PokeathlonCourseData *course);
void PokeathlonGoalRoll_HostUpdate(PokeathlonCourseData *course);
void PokeathlonGoalRoll_PackFrame(PokeathlonGoalRoll *goalRoll, PokeathlonGoalRollFramePacket *out);
void PokeathlonGoalRoll_ApplyFrame(PokeathlonCourseData *course);
void PokeathlonGoalRoll_ApplyFrameScore(PokeathlonCourseData *course);

// pokeathlon_goal_roll_field.c
void PokeathlonGoalRoll_ShowGoal(PokeathlonGoalRoll *goalRoll, u8 ballIndex, int bigBall, u8 x, u8 code, u8 own);
void PokeathlonGoalRoll_MoveMons(PokeathlonCourseData *course);
void PokeathlonGoalRoll_CollideBalls(PokeathlonCourseData *course);
void PokeathlonGoalRoll_MoveBalls(PokeathlonCourseData *course);
void PokeathlonGoalRoll_CollideBallPair(PokeathlonGoalRollBall *first, PokeathlonGoalRollBall *second);
void PokeathlonGoalRoll_BounceBallOff(PokeathlonGoalRollBall *ball, const PokeathlonGoalRollBall *other, int side);
void PokeathlonGoalRoll_CheckGoals(PokeathlonCourseData *course);
void PokeathlonGoalRoll_UpdateTiredMons(PokeathlonCourseData *course);
void PokeathlonGoalRoll_SetTouchTarget(int x, int y, VecFx32 *out);
int PokeathlonGoalRoll_GetTouchedMon(PokeathlonGoalRoll *goalRoll, u8 participant, u8 touchX, u8 touchY);
BOOL PokeathlonGoalRoll_CirclesOverlap(const VecFx32 *a, fx32 radiusA, const VecFx32 *b, fx32 radiusB);
void PokeathlonGoalRoll_ResetBall(PokeathlonGoalRollBall *ball);
void PokeathlonGoalRoll_SetBallDirection(PokeathlonGoalRoll *goalRoll, PokeathlonGoalRollBall *ball, const VecFx32 *direction);
fx32 PokeathlonGoalRoll_CalcNormalizedDot(const VecFx32 *a, const VecFx32 *b);
BOOL PokeathlonGoalRoll_IsWithinAngle(const VecFx32 *a, const VecFx32 *b, u32 angle);
void PokeathlonGoalRoll_RecoverPower(PokeathlonGoalRollMon *mon);
void PokeathlonGoalRoll_InitMonStats(PokeathlonCourseData *course, PokeathlonGoalRollMonParams *params, u8 participant, u8 member, PokeathlonGoalRollMon *mon);
void PokeathlonGoalRoll_InitWindows(PokeathlonGoalRoll *goalRoll);
void PokeathlonGoalRoll_PrintTotalPoints(PokeathlonGoalRoll *goalRoll, u32 score);
int PokeathlonGoalRoll_CollideWithWalls(VecFx32 *position, VecFx32 *previous, s32 radius, int checkCorners, VecFx32 *out);
BOOL PokeathlonGoalRoll_SegmentsIntersect(Point2D *a, Point2D *b, Point2D *c, const VecFx32 *point);
BOOL PokeathlonGoalRoll_IntersectWall(const VecFx32 *a, const VecFx32 *b, const VecFx32 *c, const VecFx32 *d, VecFx32 *out);
void PokeathlonGoalRoll_PickBallSpawn(PokeathlonGoalRoll *goalRoll, PokeathlonGoalRollBall *ball);
u8 PokeathlonGoalRoll_EncodeBallY(PokeathlonGoalRollBall *ball);
s32 PokeathlonGoalRoll_DecodeBallY(s32 code);
void PokeathlonGoalRoll_NextFacing(PokeathlonGoalRollMon *mon);
void PokeathlonGoalRoll_PlayGoalAreaSE(PokeathlonGoalRoll *goalRoll, u32 ballIndex, BOOL bigBall, s32 code);
void PokeathlonGoalRoll_PrintPlate(PokeathlonGoalRoll *goalRoll, u8 participant, int mode);
void PokeathlonGoalRoll_LoadStrings(PokeathlonCourseData *course);
void PokeathlonGoalRoll_FreeStrings(PokeathlonCourseData *course);
void PokeathlonGoalRoll_CreatePlateTasks(PokeathlonGoalRoll *goalRoll);
void PokeathlonGoalRoll_DestroyPlateTasks(PokeathlonGoalRoll *goalRoll);

// pokeathlon_goal_roll_display.c
void PokeathlonGoalRoll_StartPlateFlash(PokeathlonGoalRoll *goalRoll, int participant, int gained, int bigBall);
void PokeathlonGoalRoll_PlateTask(SysTask *task, PokeathlonGoalRollPlate *plate);
u32 PokeathlonGoalRoll_GetGoalCode(u32 goal, u8 team);
u32 PokeathlonGoalRoll_GetBallQuadrant(PokeathlonGoalRollBall *ball);
u32 PokeathlonGoalRoll_GetQuadrant(s32 x, s32 y);
void PokeathlonGoalRoll_UpdateLeaders(PokeathlonGoalRoll *goalRoll);
void PokeathlonGoalRoll_RefreshPlates(PokeathlonGoalRoll *goalRoll);
void PokeathlonGoalRoll_SortOwnTeamOrder(PokeathlonGoalRollMon *team, u8 *monY);
int PokeathlonGoalRoll_CompareMonY(const void *a, const void *b);
void PokeathlonGoalRoll_SortOtherMonsOrder(PokeathlonGoalRoll *goalRoll, PokeathlonGoalRollFramePacket *frame, u8 own);
void PokeathlonGoalRoll_OnTimeUp(PokeathlonCourseData *course);
PokeathlonGoalRollDisplay *PokeathlonGoalRollDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
void PokeathlonGoalRollDisplay_Free(PokeathlonGoalRollDisplay *display);
void PokeathlonGoalRollDisplay_LoadBgGfx(PokeathlonGoalRollDisplay *display);
void PokeathlonGoalRollDisplay_CreateSprites(PokeathlonGoalRollSpritePool *pool, PokeathlonGoalRollDisplay *display, PokeathlonGoalRollResources *resources);
void PokeathlonGoalRollDisplay_PrintName(PokeathlonGoalRollDisplay *display);
void PokeathlonGoalRollDisplay_PrintSecondWind(PokeathlonGoalRollDisplay *display);
void PokeathlonGoalRollDisplay_InitWindows(PokeathlonGoalRollDisplay *display);
void PokeathlonGoalRollDisplay_SetScore(PokeathlonGoalRollDisplay *display, u32 score, u32 participant);
u32 PokeathlonGoalRollDisplay_GetScore(PokeathlonGoalRollDisplay *display);

// pokeathlon_goal_roll_cpu.c
void PokeathlonGoalRollDisplay_UpdateTimer(PokeathlonGoalRollDisplay *display, s32 timer);
void PokeathlonGoalRollDisplay_DrawScore(PokeathlonGoalRollDisplay *display, s32 score);
PokeathlonGoalRollCpu *PokeathlonGoalRollCpu_New(enum HeapID heapId, u8 cpuCount);
void PokeathlonGoalRollCpu_SetMon(PokeathlonGoalRollCpuTeam *teams, s32 index, PokeathlonGoalRollMon *mon);
void PokeathlonGoalRollCpu_ChooseAllRoles(PokeathlonGoalRollCpuTeam *teams);
void PokeathlonGoalRollCpu_ChooseRoles(PokeathlonGoalRollCpuTeam *team);
void PokeathlonGoalRollCpu_SetBalls(PokeathlonGoalRollCpu *cpu, PokeathlonGoalRollBall *balls);
void PokeathlonGoalRollCpu_Free(PokeathlonGoalRollCpu *cpu);
void PokeathlonGoalRollCpu_Update(PokeathlonCourseData *course, PokeathlonGoalRollCpu *cpu);
int PokeathlonGoalRollCpu_ThinkChaser(PokeathlonCourseData *course, u8 participant, u8 member, PokeathlonGoalRollMon *mon, PokeathlonGoalRollBall *balls);
int PokeathlonGoalRollCpu_ApplyChaserDecision(PokeathlonGoalRollMon *mon, const PokeathlonGoalRollBall *ball, u8 decision, int quadrant);
int PokeathlonGoalRollCpu_ThinkGuard(PokeathlonCourseData *course, u8 participant, u8 member, PokeathlonGoalRollMon *mon, PokeathlonGoalRollBall *balls);
int PokeathlonGoalRollCpu_ApplyGuardDecision(PokeathlonGoalRollMon *mon, PokeathlonGoalRollBall *ball, int decision, int participant);
int PokeathlonGoalRollCpu_ThinkFlex(PokeathlonCourseData *course, u8 participant, u8 member, PokeathlonGoalRollMon *mon, PokeathlonGoalRollBall *balls, PokeathlonGoalRollCpuTeam *teams);
int PokeathlonGoalRollCpu_ApplyFlexDecision(PokeathlonGoalRollMon *mon, PokeathlonGoalRollBall *ball, int decision, int participant, PokeathlonGoalRollCpuTeam *teams);
int PokeathlonGoalRollCpu_MoveToEmptierQuadrant(PokeathlonGoalRollMon *mon, PokeathlonGoalRollCpuTeam *teams, u8 excluded);
void PokeathlonGoalRollCpu_Think(PokeathlonCourseData *course, PokeathlonGoalRollCpu *cpu, PokeathlonGoalRollCpuSlot *slot);
int PokeathlonGoalRollCpu_GetQuadrant(int x, int y, int pointX, int pointY);
BOOL PokeathlonGoalRollCpu_IsMonInTargetQuadrantOfBall(int x1, int y1, int x2, int y2, int quadrantInput, int participantInput);
BOOL PokeathlonGoalRollCpu_IsMonNotOppositeOfBall(int x1, int y1, int x2, int y2, int quadrantInput, int participantInput);
BOOL PokeathlonGoalRollCpu_IsNearPoint(int x, int y, const u16 (*points)[2]);
BOOL PokeathlonGoalRollCpu_CirclesTouch(const VecFx32 *a, fx32 radiusA, const VecFx32 *b, fx32 radiusB);
void PokeathlonScreenShake_Task(SysTask *task, PokeathlonScreenShake *shake);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_GOAL_ROLL_H
