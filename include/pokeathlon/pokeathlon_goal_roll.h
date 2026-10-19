#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_GOAL_ROLL_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_GOAL_ROLL_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "geometry_math.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task_api.h"

// Goal Roll: four teams of three Pokemon kick two balls into the goals in the corners of the field. A goal scores
// for the team that kicked the ball last (2 points for the big ball) and costs the goal's owner a point.

#define GOAL_ROLL_PARTICIPANTS 4
#define GOAL_ROLL_TEAM_SIZE    3
#define GOAL_ROLL_MONS         (GOAL_ROLL_PARTICIPANTS * GOAL_ROLL_TEAM_SIZE)
#define GOAL_ROLL_BALLS        2
#define GOAL_ROLL_NO_MON       12 // "no Pokemon" in the 0..11 mon indices

// The course's data copy area holds [0] the own input, [1] the frame packet the host builds, [2..5] the
// participants' inputs (host) and [6] the frame packet received.
#define GOAL_ROLL_DATA_SLOT_SIZE 0x28
#define GOAL_ROLL_DATA_FRAME_IN  (6 * GOAL_ROLL_DATA_SLOT_SIZE)

#define GOAL_ROLL_MODE_WALK    0
#define GOAL_ROLL_MODE_DASH    1 // released after a short touch: kicks the balls it runs into
#define GOAL_ROLL_MODE_STUNNED 2 // hit by a kicked ball without power left
#define GOAL_ROLL_MODE_TIRED   3 // out of power: walks slowly

// motion of a Pokemon (pushed back by a ball or after a kick)
#define GOAL_ROLL_MOTION_NONE      0
#define GOAL_ROLL_MOTION_RECOIL    1 // after kicking a ball
#define GOAL_ROLL_MOTION_2         2
#define GOAL_ROLL_MOTION_KNOCKBACK 3 // hit by a kicked ball: spins while pushed back

typedef struct PokeathlonGoalRollMon {
    PokeathlonCourseMonObject *object;
    PokeathlonCourseSpritePoolSprite *dashSprite;      // shown while dashing or stunned
    PokeathlonCourseSpritePoolSprite *trailSprites[3]; // dash trail, used in turn
    s32 startX;
    s32 startY;
    u32 unk1C;
    int radius;
    VecFx32 target; // where it walks to
    VecFx32 position;
    VecFx32 direction;
    int motion;
    VecFx32 motionStep;
    s16 motionFrames;
    s16 motionWait;
    u16 facing; // 1..4
    u16 goals;
    u8 stunTimer;
    u8 recoverTimer;
    u8 trailIndex;
    u8 trailCooldown;
    fx32 walkSpeed;
    fx32 dashSpeed;
    u16 kickAngle; // half width of the kick cone, in degrees
    u16 unk6E;
    u8 maxPower;
    u8 power; // lost when hit by a kicked ball, recovers over time
    u8 kickPower;
    u8 kickRow; // row of the ball speed tables
    u8 powerLevel;
    u8 kickLevel; // how long a kicked ball keeps going (gPokeathlonGoalRollKickSpeeds)
    u8 unk76[2];
    int mode;
} PokeathlonGoalRollMon;

#define GOAL_ROLL_BALL_NONE  0
#define GOAL_ROLL_BALL_SMALL 1 // radius 8, 1 point
#define GOAL_ROLL_BALL_BIG   2 // radius 12, 2 points

#define GOAL_ROLL_BALL_ROLLING 0
#define GOAL_ROLL_BALL_SCORED  1
#define GOAL_ROLL_BALL_KICKED  2
#define GOAL_ROLL_BALL_WAITING 3

typedef struct PokeathlonGoalRollBall {
    PokeathlonCourseSpritePoolSprite *sprite;
    PokeathlonCourseSpritePoolSprite *effectSprite;
    VecFx32 position;
    VecFx32 velocity;
    VecFx32 direction;
    u8 touching[GOAL_ROLL_MONS];
    u8 kind;
    u8 state;
    u8 timer;
    u8 lastKicker; // mon index, GOAL_ROLL_NO_MON if none
    u8 step;
    u8 steps;
    u8 speedTable;
    u8 scored;
    u8 speed;
    u8 speedRow;
    u8 spawnPoint; // where it last appeared (gPokeathlonGoalRollBallSpawnPoints)
    u8 wait;
    u8 shownState; // state shown last (every participant)
    u8 strength;
    u16 kickLevel; // of the last kick
    int bounced;
} PokeathlonGoalRollBall;

// Blinking name plate of a participant (top of the touch screen).
typedef struct PokeathlonGoalRollPlate {
    u8 state;
    u8 id;
    volatile u8 timer;
    s8 unk03;
    SysTask *task;
    struct PokeathlonGoalRoll *owner;
    u8 delay;
    u8 toggle;
    u8 gained; // 1 = scored, 0 = lost a point
    u8 bigBall;
} PokeathlonGoalRollPlate;

typedef struct PokeathlonGoalRollTouch {
    int newPress; // set on the first frame of a touch
    int held;
    int unk08;
} PokeathlonGoalRollTouch;

// The top screen (timer, the own name).
typedef struct PokeathlonGoalRollDisplay {
    enum HeapID heapId;
    PokeathlonCourseData *course;
    Sprite *timerDigits[2];
    BgConfig *bgConfig;
    Window nameWindow;
    Window messageWindow;
    MsgData *msgData;
    MessageFormat *msgFormat;
    NNSG2dScreenData *digitScreen;
    void *digitScreenData;
    u32 ownScore;
    u32 secondWindShown;                        // "Second Wind!" (the second ball) shown instead of the timer
    PokeathlonCourseSpritePoolSprite *timerLabelSprite; // hidden once the message replaces the timer
} PokeathlonGoalRollDisplay;

// The CPU: per participant the order in which it moves its three Pokemon.
typedef struct PokeathlonGoalRollCpuSlot {
    u32 wait;
    PokeathlonGoalRollMon *mon;
} PokeathlonGoalRollCpuSlot;

typedef struct PokeathlonGoalRollCpuTeam {
    PokeathlonGoalRollCpuSlot order[3]; // [0] the striker, then the other two
    PokeathlonGoalRollMon *mons[3];
} PokeathlonGoalRollCpuTeam;

typedef struct PokeathlonGoalRollCpu {
    PokeathlonGoalRollCpuTeam teams[GOAL_ROLL_PARTICIPANTS];
    PokeathlonGoalRollBall *balls;
    u32 run; // set every `interval` frames
    u8 cpuCount;
    u8 current;
    u8 timer;
    u8 interval;
    u8 orderSlot;
    u8 firstCpu;
    u8 humanCount;
    u8 unk9F;
} PokeathlonGoalRollCpu;

// Screen shake (Ring Drop).
typedef struct PokeathlonScreenShake {
    SpriteSystem *spriteSystem;
    BgConfig *bgConfig;
    SysTask *task;
    fx32 offset;
    fx32 step;
    s8 frame;
    u8 duration;
    u8 magnitude;
    u8 unk17;
    u32 direction;
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
    u32 touching;
    u8 x;
    u8 y;
} PokeathlonGoalRollInput;

// The frame the host sends every frame.
typedef struct PokeathlonGoalRollFramePacket {
    u8 monX[GOAL_ROLL_MONS];
    u8 monY[GOAL_ROLL_MONS];
    u8 ballX[GOAL_ROLL_BALLS];
    u8 ballY[GOAL_ROLL_BALLS]; // see PokeathlonGoalRoll_EncodeBallY
    s32 states;                // 2 bits facing - 1 per mon, 2 bits state per ball (<< 24), score slot (<< 28),
                               // big ball (bit 30), second ball active (bit 31)
    s32 flags;                 // 2 bits mode per mon, time up (bit 24), score (<< 25)
} PokeathlonGoalRollFramePacket;

// gPokeathlonGoalRollPaletteData: the plate flash palettes (two for a lost point, two for a goal), followed by the
// total points window template (gPokeathlonGoalRollWindowData) and the four goal palettes.
typedef struct PokeathlonGoalRollWindowData {
    WindowTemplate window;
    u16 goalPalettes[GOAL_ROLL_PARTICIPANTS];
} PokeathlonGoalRollWindowData;

typedef struct PokeathlonGoalRollPaletteData {
    u8 plateFlash[4];
    PokeathlonGoalRollWindowData data;
} PokeathlonGoalRollPaletteData;

// The event's work (PokeathlonCourse_GetWork).
typedef struct PokeathlonGoalRoll {
    PokeathlonCourseSpritePool *spritePool;
    BgConfig *bgConfig;
    Window scoreWindow;
    Window nameWindows[GOAL_ROLL_PARTICIPANTS];
    enum HeapID heapId;
    PokeathlonGoalRollMon mons[GOAL_ROLL_PARTICIPANTS][GOAL_ROLL_TEAM_SIZE];
    PokeathlonGoalRollBall balls[GOAL_ROLL_BALLS];
    PokeathlonCourseSpritePoolSprite *goalSprites[GOAL_ROLL_PARTICIPANTS];
    PokeathlonCourseSpritePoolSprite *goalFlashSprites[GOAL_ROLL_PARTICIPANTS];
    PokeathlonCourseSpritePoolSprite *goalLabelSprites[GOAL_ROLL_PARTICIPANTS];
    s32 scores[GOAL_ROLL_PARTICIPANTS];
    PokeathlonGoalRollTouch touch[GOAL_ROLL_PARTICIPANTS];
    u8 heldMon[GOAL_ROLL_PARTICIPANTS]; // mon index touched by each participant
    int timer;                          // frames left
    u8 packetScoreSlot;                 // whose score goes into the next frame packet
    u8 unk73D;
    u8 timeUp;
    u8 unk73F[3];
    u8 holdFrames;
    u8 bigBallMisses;
    PokeathlonCourseSpriteResources *resources;
    PokeathlonCourseMonObjects *monObjects;
    PokeathlonGoalRollCpu *cpu;
    PokeathlonGoalRollDisplay *display;
    VecFx32 walls[8];
    u8 ballInGoalArea[4]; // per ball, for the sound
    String *plateStrings[3];
    String *nameStrings[GOAL_ROLL_PARTICIPANTS];
    PokeathlonGoalRollPlate plates[GOAL_ROLL_PARTICIPANTS];
    u8 shownScores[GOAL_ROLL_PARTICIPANTS];
    u8 leading[GOAL_ROLL_PARTICIPANTS];
    void *sortWork;
} PokeathlonGoalRoll;

BOOL PokeathlonGoalRoll_Init(PokeathlonCourseData *course);
BOOL PokeathlonGoalRoll_Draw(PokeathlonCourseData *course);
BOOL PokeathlonGoalRoll_ShowResults(PokeathlonCourseData *course);
BOOL PokeathlonGoalRoll_Exit(PokeathlonCourseData *course);
BOOL PokeathlonGoalRoll_State_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonGoalRoll_State_Finish(PokeathlonCourseData *course, u8 *state);

void PokeathlonGoalRoll_ShowGoal(PokeathlonGoalRoll *goalRoll, u8 ballIndex, int bigBall, u8 x, u8 code, u8 own);
void PokeathlonGoalRoll_MoveMons(PokeathlonCourseData *course);
void PokeathlonGoalRoll_CollideBalls(PokeathlonCourseData *course);
void PokeathlonGoalRoll_MoveBalls(PokeathlonCourseData *course);
void PokeathlonGoalRoll_CheckGoals(PokeathlonCourseData *course);
void PokeathlonGoalRoll_UpdateTiredMons(PokeathlonCourseData *course);
void PokeathlonGoalRoll_SetTouchTarget(int x, int y, VecFx32 *out);
int PokeathlonGoalRoll_GetTouchedMon(PokeathlonGoalRoll *goalRoll, u8 participant, u8 touchX, u8 touchY);
void PokeathlonGoalRoll_ResetBall(PokeathlonGoalRollBall *ball);
void PokeathlonGoalRoll_InitMonStats(PokeathlonCourseData *course, PokeathlonGoalRollMonParams *params, u8 participant, u8 member, PokeathlonGoalRollMon *mon);
void PokeathlonGoalRoll_InitWindows(PokeathlonGoalRoll *goalRoll);
void PokeathlonGoalRoll_PrintTotalPoints(PokeathlonGoalRoll *goalRoll, u32 score);
u8 PokeathlonGoalRoll_EncodeBallY(PokeathlonGoalRollBall *ball);
s32 PokeathlonGoalRoll_DecodeBallY(s32 code);
void PokeathlonGoalRoll_PlayGoalAreaSE(PokeathlonGoalRoll *goalRoll, u32 ballIndex, BOOL bigBall, s32 code);
void PokeathlonGoalRoll_PrintPlate(PokeathlonGoalRoll *goalRoll, u8 participant, int mode);
void PokeathlonGoalRoll_LoadStrings(PokeathlonCourseData *course);
void PokeathlonGoalRoll_FreeStrings(PokeathlonCourseData *course);
void PokeathlonGoalRoll_CreatePlateTasks(PokeathlonGoalRoll *goalRoll);
void PokeathlonGoalRoll_DestroyPlateTasks(PokeathlonGoalRoll *goalRoll);

void PokeathlonGoalRoll_StartPlateFlash(PokeathlonGoalRoll *goalRoll, int participant, int gained, int bigBall);
void PokeathlonGoalRoll_PlateTask(SysTask *task, PokeathlonGoalRollPlate *plate);
u32 PokeathlonGoalRoll_GetGoalCode(u32 goal, u8 team);
u32 PokeathlonGoalRoll_GetBallQuadrant(PokeathlonGoalRollBall *ball);
void PokeathlonGoalRoll_UpdateLeaders(PokeathlonGoalRoll *goalRoll);
void PokeathlonGoalRoll_RefreshPlates(PokeathlonGoalRoll *goalRoll);
void PokeathlonGoalRoll_SortOwnTeamOrder(PokeathlonGoalRollMon *team, u8 *monY);
void PokeathlonGoalRoll_SortOtherMonsOrder(PokeathlonGoalRoll *goalRoll, PokeathlonGoalRollFramePacket *frame, u8 own);
void PokeathlonGoalRoll_OnTimeUp(PokeathlonCourseData *course);
PokeathlonGoalRollDisplay *PokeathlonGoalRollDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
void PokeathlonGoalRollDisplay_Free(PokeathlonGoalRollDisplay *display);
void PokeathlonGoalRollDisplay_LoadBgGfx(PokeathlonGoalRollDisplay *display);
void PokeathlonGoalRollDisplay_CreateSprites(PokeathlonCourseSpritePool *pool, PokeathlonGoalRollDisplay *display, PokeathlonCourseSpriteResources *resources);
void PokeathlonGoalRollDisplay_PrintName(PokeathlonGoalRollDisplay *display);
void PokeathlonGoalRollDisplay_PrintSecondWind(PokeathlonGoalRollDisplay *display);
void PokeathlonGoalRollDisplay_SetScore(PokeathlonGoalRollDisplay *display, u32 score, u32 participant);
u32 PokeathlonGoalRollDisplay_GetScore(PokeathlonGoalRollDisplay *display);

void PokeathlonGoalRollDisplay_UpdateTimer(PokeathlonGoalRollDisplay *display, s32 timer);
void PokeathlonGoalRollDisplay_DrawScore(PokeathlonGoalRollDisplay *display, s32 score);
PokeathlonGoalRollCpu *PokeathlonGoalRollCpu_New(enum HeapID heapId, u8 cpuCount);
void PokeathlonGoalRollCpu_SetMon(PokeathlonGoalRollCpuTeam *teams, s32 index, PokeathlonGoalRollMon *mon);
void PokeathlonGoalRollCpu_ChooseAllRoles(PokeathlonGoalRollCpuTeam *teams);
void PokeathlonGoalRollCpu_SetBalls(PokeathlonGoalRollCpu *cpu, PokeathlonGoalRollBall *balls);
void PokeathlonGoalRollCpu_Free(PokeathlonGoalRollCpu *cpu);
void PokeathlonGoalRollCpu_Update(PokeathlonCourseData *course, PokeathlonGoalRollCpu *cpu);
BOOL PokeathlonGoalRollCpu_IsNearPoint(int x, int y, const u16 (*points)[2]);
void PokeathlonScreenShake_Task(SysTask *task, PokeathlonScreenShake *shake);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_GOAL_ROLL_H
