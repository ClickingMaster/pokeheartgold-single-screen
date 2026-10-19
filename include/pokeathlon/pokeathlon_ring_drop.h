#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_RING_DROP_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_RING_DROP_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_types_def.h"
#include "pokeathlon/pokeathlon_goal_roll.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "sprite_system.h"

// Ring Drop: each participant has one of its three Pokemon in the ring at a time and tries to push the others
// out. A Pokemon's stamina drains while it is in the ring and refills on the bench.

#define RING_DROP_PARTICIPANTS 4
#define RING_DROP_TEAM_SIZE    3
#define RING_DROP_FRAMES       1800

// The Pokemon states (PokeathlonRingDropMon.state).
#define RING_DROP_MON_STAND      1 // walks to its destination
#define RING_DROP_MON_STAGGER    2 // knocked about by a dash or a press
#define RING_DROP_MON_FALL_OUT   3 // left the ring; the next team member comes in at the start
#define RING_DROP_MON_RETURN     4 // back at the start, cannot be pushed for 60 frames
#define RING_DROP_MON_DASH_START 5
#define RING_DROP_MON_DASH       6
#define RING_DROP_MON_DASH_END   7
#define RING_DROP_MON_JUMP_START 8
#define RING_DROP_MON_JUMP_RISE  9
#define RING_DROP_MON_JUMP_TOP   10
#define RING_DROP_MON_JUMP_FALL  11 // presses down on whatever it lands on
#define RING_DROP_MON_LANDING    12

// The switch to the next team member (PokeathlonRingDropMonFlags.switchState).
#define RING_DROP_SWITCH_NONE 0
#define RING_DROP_SWITCH_OUT  1 // the ball flies up off the Pokemon
#define RING_DROP_SWITCH_IN   2 // the ball comes down with the next member

// What a push does to the Pokemon pushed (PokeathlonRingDropMon_GetPushReaction).
#define RING_DROP_REACTION_NONE    0
#define RING_DROP_REACTION_STAND   1
#define RING_DROP_REACTION_STOP    2 // a dash is stopped
#define RING_DROP_REACTION_STAGGER 3

// The top screen: the own team, the ranks and the timer.
typedef struct PokeathlonRingDropUi PokeathlonRingDropUi;

// A team member's numbers, from its performance stats 0-3 (PokeathlonRingDrop_InitMemberParams,
// table NARC 170 #9).
typedef struct PokeathlonRingDropMemberParams {
    s32 dashPower;  // push of a dash (x3)
    s32 dashDamage; // stamina a dash takes from the Pokemon hit (x2)
    fx32 stamina;   // the maximum
    fx32 speed;     // walking speed
    s32 pressPower; // push of landing on a Pokemon
    s32 pressDamage;
    u8 jumpType; // row of gPokeathlonRingDropJumpHeights (jump heights) and gPokeathlonRingDropFallSpeeds (fall speeds)
} PokeathlonRingDropMemberParams;

// A team member's 3D object and stamina.
typedef struct PokeathlonRingDropMember {
    PokeathlonCourseMonObject *object;
    const PokeathlonRingDropMemberParams *params;
    fx32 stamina;
    u32 index : 4; // of the mon object (participant * 3 + member)
    u32 unk0C_4 : 28;
} PokeathlonRingDropMember;

typedef union PokeathlonRingDropMonFlags {
    u32 raw;

    struct {
        u32 score : 12;    // the participant's points
        u32 attacker : 2;  // the participant that last hit this Pokemon (skipping the own index), 0: none
        u32 hit : 2;       // 1: tackled by a dash, 2: pressed by a jump
        u32 direction : 4; // 1..4
        u32 participant : 2;
        u32 member : 2;      // team member in the ring
        u32 switchState : 2; // 1: going out, 2: coming in
        u32 switching : 1;
        u32 pushed : 1;        // slides with its velocity until it stops
        u32 exhausted : 1;     // stamina ran out; the Pokemon rests for 60 frames
        u32 jumpRequested : 1; // a jump was asked for
        u32 hasTarget : 1;     // a dash to targetX/targetY was asked for
        u32 bumped : 1;        // bumped into another Pokemon this frame (sound)
    } bits;
} PokeathlonRingDropMonFlags;

struct PokeathlonRingDropPlayer;

// The Pokemon a participant has in the ring.
typedef struct PokeathlonRingDropMon {
    PokeathlonCourseData *course;
    PokeathlonRingDropMember *member;        // the member in the ring
    struct PokeathlonRingDropPlayer *player; // the player this is part of
    PokeathlonRingDropUi *ui;
    struct PokeathlonRingDropMon *attacker; // the last Pokemon that pushed this one
    s32 state;                              // RING_DROP_MON_*
    s32 attackerState;                      // the attacker's state when it pushed
    s16 targetX;                            // of a dash
    s16 targetY;
    VecFx32 destination; // where it walks to
    VecFx32 position;
    VecFx32 dashDirection;
    VecFx32 velocity;
    fx32 height; // of a jump
    fx32 fallSpeed;
    s8 staggerFrames;
    s8 dashFrames;
    s8 jumpFrames;
    s8 fallOutFrames;
    s8 restFrames; // while exhausted
    s8 hitFrames;  // frozen after a hit
    s8 returnFrames;
    u8 status; // the switch button's look (PokeathlonRingDropMon_UpdateStatus)
    PokeathlonRingDropMonFlags flags;
    u32 hitMarkFrames; // attacker and hit stay in the frame packet that long
    PokeathlonRingDropMember members[RING_DROP_TEAM_SIZE];
    u32 index; // the participant
} PokeathlonRingDropMon;

// The host's record of a participant's touch.
typedef struct PokeathlonRingDropTouch {
    u32 frames : 14; // length of the current touch
    u32 started : 1; // set on the first frame of a touch
    u32 touching : 1;
    u32 lastFrames : 13; // length of the previous touch
    u32 onMon : 1;       // the touch started on the own Pokemon
    u32 unk30 : 2;
} PokeathlonRingDropTouch;

// A participant: the touch state the host tracks and the Pokemon in the ring.
typedef struct PokeathlonRingDropPlayer {
    s16 touchX;
    s16 touchY;
    s16 touchStartX;
    s16 touchStartY;
    PokeathlonRingDropTouch touch;
    PokeathlonRingDropMon mon;
} PokeathlonRingDropPlayer;

// The course's data copy area holds [0] the own input, [1] the frame packet the host builds, [2..5] the
// participants' inputs (host) and [6] the frame packet received.

// A participant's input.
typedef struct PokeathlonRingDropInput {
    u32 x : 8;
    u32 y : 8;
    u32 touching : 1;
    u32 switchState : 2; // the switch animation's progress, see PokeathlonRingDropMonFlags
    u32 unk00_19 : 13;
    u32 unk04[7];
    u32 unk20;
} PokeathlonRingDropInput;

// One participant's Pokemon in the frame packet.
typedef struct PokeathlonRingDropMonPacket {
    u8 x;
    u8 y;
    u8 height : 6; // jump height
    u8 hit : 2;    // 1: tackle hit, 2: press hit
    u8 score;
    u8 state : 4;
    u8 attacker : 2;  // see PokeathlonRingDropMonFlags
    u8 member : 2;    // team member in the ring
    u8 status : 3;    // the own participant's status sprite
    u8 direction : 2; // minus 1
    u8 switchState : 2;
    u8 unk5_7 : 1;
} PokeathlonRingDropMonPacket;

typedef union PokeathlonRingDropFrameMarks {
    u32 raw;

    struct {
        u32 unk0 : 7;
        u32 bumped : 1;   // two Pokemon bumped into each other
        u32 members : 24; // 2 bits per team member (participant * 3 + member): 0 fine, 1 stamina below 30, 2 none left
    } bits;
} PokeathlonRingDropFrameMarks;

// The frame the host sends.
typedef struct PokeathlonRingDropFramePacket {
    PokeathlonRingDropMonPacket mons[RING_DROP_PARTICIPANTS];
    u32 unk18;
    PokeathlonRingDropFrameMarks marks;
    u32 unk20_0 : 3;
    u32 timer : 16; // frames left
    u32 unk20_19 : 12;
    u32 finished : 1;
} PokeathlonRingDropFramePacket;

// The sprites of a participant's Pokemon.
// The sprites that go with a Pokemon in the ring (PokeathlonRingDropMonSprites.sprites).
enum PokeathlonRingDropMonSprite {
    RING_DROP_MON_SPRITE_CHARGE,
    RING_DROP_MON_SPRITE_MARK,   // above the Pokemon
    RING_DROP_MON_SPRITE_SWITCH, // the ball going up and coming down
    RING_DROP_MON_SPRITE_DUST,   // where a Pokemon lands
    RING_DROP_MON_SPRITE_HIT,    // between two Pokemon that collide
    RING_DROP_MON_SPRITE_COUNT,
};

typedef struct PokeathlonRingDropMonSprites {
    PokeathlonCourseMonObject *monObject;
    ManagedSprite *sprites[RING_DROP_MON_SPRITE_COUNT];
    int state;                                // the last packet state
    u16 palettes[RING_DROP_TEAM_SIZE][2][16]; // normal and grey per team member
    fx32 jumpScale;
    u8 unkE0;
    s8 jumpFrames;
    s8 switchFrames;
    u8 unkE3;
    u32 squashFrames : 8;
    u32 switchStep : 4;
    u32 switchState : 2;
    u32 member : 2;
    u32 attacker : 2;
    u32 unkE4_18 : 1;
    u32 jumping : 1;
    u32 airborne : 1;
    u32 recordJump : 1;
    u32 unkE4_22 : 1;
    u32 hit : 2;
    u32 unkE4_25 : 7;
} PokeathlonRingDropMonSprites;

// Everything every participant shows of the frame packet.
typedef struct PokeathlonRingDropDisplay {
    PokeathlonRingDropUi *ui;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *statusSprite;
    PokeathlonRingDropMonSprites mons[RING_DROP_PARTICIPANTS];
    u16 scores[RING_DROP_PARTICIPANTS];
    u8 status;
    u8 bumped : 1;
    u8 unk3B9_1 : 7;
    u16 unk3BA;
} PokeathlonRingDropDisplay;

typedef struct PokeathlonRingDrop {
    enum HeapID heapId;
    BgConfig *bgConfig;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    MessageFormat *msgFormat;
    MsgData *msgData;
    PokeathlonCourseSpriteResources *resources;
    void *monObjects;
    Window window;
    PokeathlonRingDropMemberParams memberParams[RING_DROP_PARTICIPANTS * RING_DROP_TEAM_SIZE];
    PokeathlonRingDropUi *ui;
    struct PokeathlonRingDropCpu *cpu;                           // the computer participants
    u8 contacts[RING_DROP_PARTICIPANTS][RING_DROP_PARTICIPANTS]; // pairs whose push was taken already
    PokeathlonRingDropPlayer players[RING_DROP_PARTICIPANTS];
    int timer; // frames left
    PokeathlonRingDropDisplay display;
    PokeathlonScreenShake shake;
} PokeathlonRingDrop;

void PokeathlonScreenShake_Start(PokeathlonScreenShake *shake, int magnitude, int duration);
BOOL PokeathlonRingDrop_IsOnSwitchButton(int x, int y);
BOOL PokeathlonRingDrop_IsWithinDistance(VecFx32 *a, VecFx32 *b, fx32 distance);
void PokeathlonRingDrop_DecayVelocity(VecFx32 *velocity, fx32 amountX, fx32 amountY);
void PokeathlonRingDrop_ClampVelocity(VecFx32 *velocity, fx32 limit);
BOOL PokeathlonRingDrop_State_FadeIn(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRingDrop_State_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRingDrop_State_Finish(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRingDrop_Init(PokeathlonCourseData *course);
BOOL PokeathlonRingDrop_Draw(PokeathlonCourseData *course);
BOOL PokeathlonRingDrop_ShowResults(PokeathlonCourseData *course);
BOOL PokeathlonRingDrop_Exit(PokeathlonCourseData *course);
s32 PokeathlonRingDrop_GetMonState(PokeathlonCourseData *course, u8 participant);
VecFx32 *PokeathlonRingDrop_GetMonPosition(PokeathlonCourseData *course, u8 participant);
void PokeathlonRingDrop_GetMonScreenPosition(PokeathlonCourseData *course, u8 participant, VecFx32 *out);
u32 PokeathlonRingDrop_GetMonDirection(PokeathlonCourseData *course, u8 participant);
u8 PokeathlonRingDrop_GetMonStamina(PokeathlonCourseData *course, u8 participant);
u8 PokeathlonRingDrop_GetMonStaminaPercent(PokeathlonCourseData *course, u8 participant);
u32 PokeathlonRingDrop_IsMonSwitching(PokeathlonCourseData *course, u8 participant);
u32 PokeathlonRingDrop_IsMonPushed(PokeathlonCourseData *course, u8 participant);
BOOL PokeathlonRingDrop_CanMonAct(PokeathlonCourseData *course, u8 participant);
void PokeathlonRingDrop_SetMonTarget(PokeathlonCourseData *course, u8 participant, const s16 *target);
void PokeathlonRingDrop_RequestJump(PokeathlonCourseData *course, u8 participant);
void PokeathlonRingDrop_StartMonSwitch(PokeathlonCourseData *course, u8 participant);
void PokeathlonRingDrop_SetMonDestination(PokeathlonCourseData *course, u8 participant, const s16 *position);
int PokeathlonRingDrop_GetDirection(const VecFx32 *vec);

void PokeathlonRingDrop_InitSprites(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDrop_FreeSprites(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDrop_LoadSpriteResources(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDrop_InitBgs(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDrop_FreeBgs(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDrop_LoadBgGraphics(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDropDisplay_Init(PokeathlonRingDropDisplay *display, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonRingDropUi *ui, void *monObjects, enum HeapID heapId);
void PokeathlonRingDropDisplay_Update(PokeathlonRingDropDisplay *display, PokeathlonCourseData *course);
void PokeathlonRingDropDisplay_Free(PokeathlonRingDropDisplay *display);
void PokeathlonRingDropDisplay_StopAnimations(PokeathlonRingDropDisplay *display);
BOOL PokeathlonRingDropMon_CanBePushed(int state, int attackerState);
void PokeathlonRingDropMon_FaceDirection(PokeathlonRingDropMon *mon, const VecFx32 *direction);

void PokeathlonRingDrop_CreateMonObjects(PokeathlonRingDrop *ringDrop, PokeathlonCourseData *course);
void PokeathlonRingDrop_InitMons(PokeathlonRingDrop *ringDrop, PokeathlonCourseData *course);
void PokeathlonRingDrop_FreeMonObjects(PokeathlonRingDrop *ringDrop);
int PokeathlonRingDropMon_GetPushReaction(PokeathlonRingDropMon *mon);
void PokeathlonRingDrop_UpdateMons(PokeathlonRingDropPlayer *players, PokeathlonCourseData *course);
void PokeathlonRingDrop_BuildFramePacket(u8 (*contacts)[RING_DROP_PARTICIPANTS], PokeathlonCourseData *course);
void PokeathlonRingDrop_SetMonAffineCenter(PokeathlonCourseMonObject *object, BOOL centered);
void PokeathlonRingDropMon_SetState(PokeathlonRingDropMon *mon, int state);
BOOL PokeathlonRingDropMon_CanSwitch(PokeathlonRingDropMon *mon);
BOOL PokeathlonRingDropMon_CanAct(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_SetDestination(PokeathlonRingDropMon *mon, const s16 *position);
int PokeathlonRingDrop_VecToDirection(const VecFx32 *vec);
void PokeathlonRingDropMon_Move(PokeathlonRingDropMon *mon, PokeathlonCourseData *course);
BOOL PokeathlonRingDropMon_IsOutOfRing(PokeathlonRingDropMon *mon);

void PokeathlonRingDropMon_ResetToStart(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_Update(PokeathlonRingDropMon *mon, PokeathlonCourseData *course);
void PokeathlonRingDropMon_Stagger(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_StartSwitch(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_DrainStamina(PokeathlonRingDropMon *mon, int amount);
void PokeathlonRingDrop_InitMemberParams(PokeathlonCourseData *course);
PokeathlonRingDropMemberParams *PokeathlonRingDrop_GetMemberParams(PokeathlonCourseData *course, u8 participant, u8 member);
void PokeathlonRingDropMon_NextMember(PokeathlonRingDropMon *mon);
BOOL PokeathlonRingDrop_PlaySE(u32 seqNo, BOOL own);
void PokeathlonRingDrop_AddScoreWindow(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDrop_PrintScore(PokeathlonCourseData *course, u32 score);
void PokeathlonRingDropMon_UpdateStatus(PokeathlonRingDropMon *mon, PokeathlonRingDropTouch *touch, BOOL onSwitchButton);

PokeathlonRingDropUi *PokeathlonRingDropUi_New(SpriteSystem *spriteSystem, SpriteManager *spriteManager, void *resources, BgConfig *bgConfig, MsgData *msgData, MessageFormat *msgFormat, u8 participant, enum HeapID heapId, PokeathlonCourseData *course);
void PokeathlonRingDropUi_Init(PokeathlonRingDropUi *ui);
void PokeathlonRingDropUi_Free(PokeathlonRingDropUi *ui);
void PokeathlonRingDropUi_StopAnimations(PokeathlonRingDropUi *ui);
void PokeathlonRingDropUi_SetRanks(PokeathlonRingDropUi *ui, u16 participant, u16 score, const u8 *sorted);
void PokeathlonRingDropUi_SetTimer(PokeathlonRingDropUi *ui, s32 seconds);
void PokeathlonRingDropUi_ShowNextReady(PokeathlonRingDropUi *ui, BOOL next, BOOL afterNext);
u16 PokeathlonRingDropUi_GetScore(PokeathlonRingDropUi *ui);
BOOL PokeathlonRingDropUi_RunSwitch(PokeathlonRingDropUi *ui, u8 participant, u8 member);
BOOL PokeathlonRingDropUi_WaitSwitch(PokeathlonRingDropUi *ui, u8 participant, u8 member);
void PokeathlonRingDropUi_SetSwitching(PokeathlonRingDropUi *ui, u8 participant, u32 member, BOOL switching);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RING_DROP_H
