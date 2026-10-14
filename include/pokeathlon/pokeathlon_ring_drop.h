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

// Ring Drop (Pokeathlon event 7): the four participants each have one of their three Pokemon in
// the ring at a time and try to push the others out. Touching the switch button at the top of the
// touch screen swaps the Pokemon in the ring for the next team member. A Pokemon's stamina drains
// while it is in the ring and refills on the bench. The match lasts 1800 frames; the host runs the
// ring and sends a frame packet each frame.

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

// What a push does to the Pokemon pushed (PokeathlonRingDropMon_GetPushReaction).
#define RING_DROP_REACTION_NONE    0
#define RING_DROP_REACTION_STAND   1
#define RING_DROP_REACTION_STOP    2 // a dash is stopped
#define RING_DROP_REACTION_STAGGER 3

// The top screen: the own team, the ranks and the timer (pokeathlon_ring_drop_ui.c).
typedef struct PokeathlonRingDropUi PokeathlonRingDropUi;

// A team member's numbers, from its performance stats 0-3 (PokeathlonRingDrop_InitMemberParams,
// table NARC 170 #9).
typedef struct PokeathlonRingDropMemberParams {
    s32 dashPower;                // 0x00: push of a dash (x3)
    s32 dashDamage;               // 0x04: stamina a dash takes from the Pokemon hit (x2)
    fx32 stamina;                 // 0x08: the maximum
    fx32 speed;                   // 0x0C: walking speed
    s32 pressPower;               // 0x10: push of landing on a Pokemon
    s32 pressDamage;              // 0x14
    u8 jumpType;                  // 0x18: row of gPokeathlonRingDropJumpHeights (jump heights) and gPokeathlonRingDropFallSpeeds (fall speeds)
} PokeathlonRingDropMemberParams; // size: 0x1C

// A team member's 3D object and stamina.
typedef struct PokeathlonRingDropMember {
    PokeathlonCourseObject *object;                                 // 0x00: the course mon object
    const PokeathlonRingDropMemberParams *params; // 0x04
    fx32 stamina;                                 // 0x08
    u32 index : 4;                                // 0x0C: of the mon object (participant * 3 + member)
    u32 unk0C_4 : 28;
} PokeathlonRingDropMember; // size: 0x10

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
    PokeathlonCourseData *course;                          // 0x00
    PokeathlonRingDropMember *member;                      // 0x04: the member in the ring
    struct PokeathlonRingDropPlayer *player;               // 0x08: the player this is part of
    PokeathlonRingDropUi *ui;                              // 0x0C
    struct PokeathlonRingDropMon *attacker;                // 0x10: the last Pokemon that pushed this one
    s32 state;                                             // 0x14: RING_DROP_MON_*
    s32 attackerState;                                     // 0x18: the attacker's state when it pushed
    s16 targetX;                                           // 0x1C: of a dash
    s16 targetY;                                           // 0x1E
    VecFx32 destination;                                   // 0x20: where it walks to
    VecFx32 position;                                      // 0x2C
    VecFx32 dashDirection;                                 // 0x38
    VecFx32 velocity;                                      // 0x44
    fx32 height;                                           // 0x50: of a jump
    fx32 fallSpeed;                                        // 0x54
    s8 staggerFrames;                                      // 0x58
    s8 dashFrames;                                         // 0x59
    s8 jumpFrames;                                         // 0x5A
    s8 fallOutFrames;                                      // 0x5B
    s8 restFrames;                                         // 0x5C: while exhausted
    s8 hitFrames;                                          // 0x5D: frozen after a hit
    s8 returnFrames;                                       // 0x5E
    u8 status;                                             // 0x5F: the switch button's look (PokeathlonRingDropMon_UpdateStatus)
    PokeathlonRingDropMonFlags flags;                      // 0x60
    u32 hitMarkFrames;                                     // 0x64: attacker and hit stay in the frame packet that long
    PokeathlonRingDropMember members[RING_DROP_TEAM_SIZE]; // 0x68
    u32 index;                                             // 0x98: the participant
} PokeathlonRingDropMon;                                   // size: 0x9C

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
    s16 touchX;                    // 0x00
    s16 touchY;                    // 0x02
    s16 touchStartX;               // 0x04
    s16 touchStartY;               // 0x06
    PokeathlonRingDropTouch touch; // 0x08
    PokeathlonRingDropMon mon;     // 0x0C
} PokeathlonRingDropPlayer;        // size: 0xA8

// The course's data copy area holds 0x28-byte slots: [0] the own input, [1] the frame packet the
// host builds, [2..5] the participants' inputs (host), [6] the frame packet received.
#define RING_DROP_DATA_SLOT_SIZE 0x28
#define RING_DROP_DATA_FRAME_OUT (1 * RING_DROP_DATA_SLOT_SIZE)
#define RING_DROP_DATA_INPUTS    (2 * RING_DROP_DATA_SLOT_SIZE)
#define RING_DROP_DATA_FRAME_IN  (6 * RING_DROP_DATA_SLOT_SIZE)

// A participant's input.
typedef struct PokeathlonRingDropInput {
    u32 x : 8; // 0x00
    u32 y : 8;
    u32 touching : 1;
    u32 switchState : 2; // the switch animation's progress, see PokeathlonRingDropMonFlags
    u32 unk00_19 : 13;
    u32 unk04[7];          // 0x04
    u32 unk20;             // 0x20
} PokeathlonRingDropInput; // size: 0x24

// One participant's Pokemon in the frame packet.
typedef struct PokeathlonRingDropMonPacket {
    u8 x;             // 0x00
    u8 y;             // 0x01
    u8 height : 6;    // 0x02: jump height
    u8 hit : 2;       //       1: tackle hit, 2: press hit
    u8 score;         // 0x03
    u8 state : 4;     // 0x04
    u8 attacker : 2;  //       see PokeathlonRingDropMonFlags
    u8 member : 2;    //       team member in the ring
    u8 status : 3;    // 0x05: the own participant's status sprite
    u8 direction : 2; //       minus 1
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
    PokeathlonRingDropMonPacket mons[RING_DROP_PARTICIPANTS]; // 0x00
    u32 unk18;                                                // 0x18
    PokeathlonRingDropFrameMarks marks;                       // 0x1C
    u32 unk20_0 : 3;                                          // 0x20
    u32 timer : 16;                                           //       frames left
    u32 unk20_19 : 12;
    u32 finished : 1;
} PokeathlonRingDropFramePacket; // size: 0x24

// The sprites of a participant's Pokemon.
typedef struct PokeathlonRingDropMonSprites {
    PokeathlonCourseMonObject *monObject;                          // 0x00
    ManagedSprite *chargeSprite;              // 0x04
    ManagedSprite *markSprite;                // 0x08: above the Pokemon
    ManagedSprite *switchSprite;              // 0x0C: the ball going up and coming down
    ManagedSprite *dustSprite;                // 0x10: where a Pokemon lands
    ManagedSprite *hitSprite;                 // 0x14: between two Pokemon that collide
    int state;                                // 0x18: the last packet state
    u16 palettes[RING_DROP_TEAM_SIZE][2][16]; // 0x1C: normal and grey per team member
    fx32 jumpScale;                           // 0xDC
    u8 unkE0;                                 // 0xE0
    s8 jumpFrames;                            // 0xE1
    s8 switchFrames;                          // 0xE2
    u8 unkE3;                                 // 0xE3
    u32 squashFrames : 8;                     // 0xE4
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
} PokeathlonRingDropMonSprites; // size: 0xE8

// Everything every participant shows of the frame packet.
typedef struct PokeathlonRingDropDisplay {
    PokeathlonRingDropUi *ui;                                  // 0x000: PokeathlonRingDrop.ui
    SpriteSystem *spriteSystem;                                // 0x004
    SpriteManager *spriteManager;                              // 0x008
    ManagedSprite *statusSprite;                               // 0x00C
    PokeathlonRingDropMonSprites mons[RING_DROP_PARTICIPANTS]; // 0x010
    u16 scores[RING_DROP_PARTICIPANTS];                        // 0x3B0
    u8 status;                                                 // 0x3B8
    u8 bumped : 1;                                             // 0x3B9
    u8 unk3B9_1 : 7;
    u16 unk3BA;              // 0x3BA
} PokeathlonRingDropDisplay; // size: 0x3BC

typedef struct PokeathlonRingDrop {
    enum HeapID heapId;                                                                       // 0x000
    BgConfig *bgConfig;                                                                       // 0x004
    SpriteSystem *spriteSystem;                                                               // 0x008
    SpriteManager *spriteManager;                                                             // 0x00C
    MessageFormat *msgFormat;                                                                 // 0x010
    MsgData *msgData;                                                                         // 0x014
    PokeathlonCourseSpriteResources *resources;                                                                          // 0x018
    void *monObjects;                                                                         // 0x01C
    Window window;                                                                            // 0x020
    PokeathlonRingDropMemberParams memberParams[RING_DROP_PARTICIPANTS][RING_DROP_TEAM_SIZE]; // 0x030
    PokeathlonRingDropUi *ui;                                                                 // 0x180
    struct PokeathlonRingDropCpu *cpu;                                                        // 0x184: the computer participants
    u8 contacts[RING_DROP_PARTICIPANTS][RING_DROP_PARTICIPANTS];                              // 0x188: pairs whose push was taken already
    PokeathlonRingDropPlayer players[RING_DROP_PARTICIPANTS];                                 // 0x198
    int timer;                                                                                // 0x438: frames left
    PokeathlonRingDropDisplay display;                                                        // 0x43C
    PokeathlonScreenShake shake;                                                              // 0x7F8
} PokeathlonRingDrop;                                                                         // size: 0x814

// pokeathlon_ring_drop.c
BOOL PokeathlonScreenShake_IsDone(PokeathlonScreenShake *shake);
void PokeathlonScreenShake_Init(PokeathlonScreenShake *shake, BgConfig *bgConfig, SpriteSystem *spriteSystem);
void PokeathlonScreenShake_Start(PokeathlonScreenShake *shake, int magnitude, int duration);
BOOL PokeathlonRingDrop_IsOnSwitchButton(int x, int y);
BOOL PokeathlonRingDrop_IsWithinDistance(VecFx32 *a, VecFx32 *b, fx32 distance);
void PokeathlonRingDrop_Decay(fx32 *value, fx32 amount);
void PokeathlonRingDrop_DecayVelocity(VecFx32 *velocity, fx32 amountX, fx32 amountY);
void PokeathlonRingDrop_ClampVelocity(VecFx32 *velocity, fx32 limit);
BOOL PokeathlonRingDrop_State_FadeIn(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRingDrop_State_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRingDrop_State_Finish(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRingDrop_Init(PokeathlonCourseData *course);
BOOL PokeathlonRingDrop_Draw(PokeathlonCourseData *course);
BOOL PokeathlonRingDrop_ShowResults(PokeathlonCourseData *course);
BOOL PokeathlonRingDrop_Exit(PokeathlonCourseData *course);
s32 PokeathlonRingDrop_GetMonState(PokeathlonCourseData *course, int participant);
VecFx32 *PokeathlonRingDrop_GetMonPosition(PokeathlonCourseData *course, int participant);
void PokeathlonRingDrop_GetMonScreenPosition(PokeathlonCourseData *course, int participant, fx32 *out);
u32 PokeathlonRingDrop_GetMonDirection(PokeathlonCourseData *course, int participant);
u8 PokeathlonRingDrop_GetMonStamina(PokeathlonCourseData *course, int participant);
u8 PokeathlonRingDrop_GetMonStaminaPercent(PokeathlonCourseData *course, int participant);
u32 PokeathlonRingDrop_IsMonSwitching(PokeathlonCourseData *course, int participant);
u32 PokeathlonRingDrop_IsMonPushed(PokeathlonCourseData *course, int participant);
BOOL PokeathlonRingDrop_CanMonAct(PokeathlonCourseData *course, int participant);
void PokeathlonRingDrop_SetMonTarget(PokeathlonCourseData *course, int participant, const s16 *target);
void PokeathlonRingDrop_RequestJump(PokeathlonCourseData *course, int participant);
void PokeathlonRingDrop_StartMonSwitch(PokeathlonCourseData *course, int participant);
void PokeathlonRingDrop_SetMonDestination(PokeathlonCourseData *course, int participant, const s16 *position);
int PokeathlonRingDrop_GetDirection(const VecFx32 *vec);
void PokeathlonRingDrop_SetGfxBanks(void);
void PokeathlonRingDrop_Update(PokeathlonCourseData *course);
void PokeathlonRingDrop_OnFinish(PokeathlonCourseData *course);
BOOL PokeathlonRingDrop_ReadInput(PokeathlonCourseData *course);

// pokeathlon_ring_drop_display.c
void PokeathlonRingDrop_InitSprites(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDrop_FreeSprites(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDrop_LoadSpriteResources(PokeathlonRingDrop *ringDrop);
ManagedSprite *PokeathlonRingDrop_NewSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 animation, u16 drawPriority);
void PokeathlonRingDrop_InitBgs(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDrop_FreeBgs(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDrop_LoadBgGraphics(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDropDisplay_SwitchOut(PokeathlonRingDropMonSprites *mon, s16 *position, PokeathlonRingDropUi *ui, u8 participant, PokeathlonRingDropInput *input, BOOL cpu, BOOL own);
void PokeathlonRingDropDisplay_SwitchIn(PokeathlonRingDropMonSprites *mon, s16 *position, void *ui, u8 participant, PokeathlonRingDropInput *input, BOOL report, BOOL own);
void PokeathlonRingDropDisplay_EndSwitch(PokeathlonRingDropMonSprites *mon, const s16 *position, PokeathlonRingDropInput *input, BOOL report, BOOL own);
void PokeathlonRingDropDisplay_UpdateSwitch(PokeathlonRingDropMonPacket *packet, PokeathlonRingDropMonSprites *mon, void *ui, PokeathlonCourseData *course, u8 participant);
int PokeathlonRingDropDisplay_CompareDepth(const void *a, const void *b);
int PokeathlonRingDropDisplay_CompareScores(const void *a, const void *b);
void PokeathlonRingDropDisplay_ApplyFramePacket(PokeathlonRingDropDisplay *display, PokeathlonRingDropFramePacket *framePacket, PokeathlonCourseData *course);
void PokeathlonRingDropDisplay_Init(PokeathlonRingDropDisplay *display, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonRingDropUi *ui, void *monObjects, enum HeapID heapId);
void PokeathlonRingDropDisplay_Update(PokeathlonRingDropDisplay *display, PokeathlonCourseData *course);
void PokeathlonRingDropDisplay_Free(PokeathlonRingDropDisplay *display);
void PokeathlonRingDropDisplay_StopAnimations(PokeathlonRingDropDisplay *display);
BOOL PokeathlonRingDropMon_CanBePushed(int state, int attackerState);
void PokeathlonRingDropMon_FaceDirection(PokeathlonRingDropMon *mon, const VecFx32 *direction);

// pokeathlon_ring_drop_field.c
BOOL PokeathlonRingDrop_IsAttackState(int state);
BOOL PokeathlonRingDropMon_Push(PokeathlonRingDropMon *target, PokeathlonRingDropMon *source, VecFx32 *direction);
void PokeathlonRingDrop_CreateMonObjects(PokeathlonRingDrop *ringDrop, PokeathlonCourseData *course);
void PokeathlonRingDrop_InitMons(PokeathlonRingDrop *ringDrop, PokeathlonCourseData *course);
void PokeathlonRingDrop_FreeMonObjects(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDropMon_ApplyPushReaction(PokeathlonRingDropMon *mon, int reaction);
int PokeathlonRingDropMon_GetPushReaction(PokeathlonRingDropMon *mon);
void PokeathlonRingDrop_UpdateCollisions(PokeathlonCourseData *course);
void PokeathlonRingDrop_UpdateStamina(PokeathlonRingDropPlayer *players, PokeathlonCourseData *course);
void PokeathlonRingDrop_UpdateMons(PokeathlonRingDropPlayer *players, PokeathlonCourseData *course);
void PokeathlonRingDrop_BuildFramePacket(u8 (*contacts)[RING_DROP_PARTICIPANTS], PokeathlonCourseData *course);
void PokeathlonRingDrop_SetMonAffineCenter(PokeathlonCourseMonObject *object, BOOL centered);
void PokeathlonRingDropMon_SetState(PokeathlonRingDropMon *mon, int state);
BOOL PokeathlonRingDropMon_Collide(PokeathlonRingDropMon *a, PokeathlonRingDropMon *b);
BOOL PokeathlonRingDropMon_CanSwitch(PokeathlonRingDropMon *mon);
BOOL PokeathlonRingDropMon_CanAct(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_SetDestination(PokeathlonRingDropMon *mon, const s16 *position);
int PokeathlonRingDrop_VecToDirection(const VecFx32 *vec);
BOOL PokeathlonRingDropMon_CanCollide(PokeathlonRingDropMon *mon);
void PokeathlonRingDrop_ClearContacts(PokeathlonCourseData *course, s32 participant);
void PokeathlonRingDropMon_Move(PokeathlonRingDropMon *mon, PokeathlonCourseData *course);
BOOL PokeathlonRingDropMon_IsOutOfRing(PokeathlonRingDropMon *mon);

// pokeathlon_ring_drop_mon.c
void PokeathlonRingDropMon_Respawn(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_ResetToStart(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_ApplyGravity(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_Update(PokeathlonRingDropMon *mon, PokeathlonCourseData *course);
void PokeathlonRingDropMon_StartDash(PokeathlonRingDropMon *mon, const s16 *target);
void PokeathlonRingDropMon_EndDash(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_UpdateDash(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_StartJump(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_Land(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_Stagger(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_FallOut(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_StartSwitch(PokeathlonRingDropMon *mon);
void PokeathlonRingDropMon_DrainStamina(PokeathlonRingDropMon *mon, int amount);
void PokeathlonRingDrop_InitMemberParams(PokeathlonCourseData *course);
PokeathlonRingDropMemberParams *PokeathlonRingDrop_GetMemberParams(PokeathlonCourseData *course, u32 participant, u32 member);
void PokeathlonRingDropMon_NextMember(PokeathlonRingDropMon *mon);
BOOL PokeathlonRingDrop_PlaySE(u32 seqNo, BOOL own);
void PokeathlonRingDrop_AddScoreWindow(PokeathlonRingDrop *ringDrop);
void PokeathlonRingDrop_PrintScore(PokeathlonCourseData *course, u32 score);
void PokeathlonRingDropMon_UpdateStatus(PokeathlonRingDropMon *mon, PokeathlonRingDropTouch *touch, BOOL onSwitchButton);

// pokeathlon_ring_drop_ui.c
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
ManagedSprite *PokeathlonRingDropUi_NewSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 animation, u16 priority);
void PokeathlonRingDropUi_AddWindows(PokeathlonRingDropUi *ui);
void PokeathlonRingDropUi_DrawScore(PokeathlonRingDropUi *ui, s32 score);
void PokeathlonRingDropUi_PrintNames(PokeathlonRingDropUi *ui);
void PokeathlonRingDropUi_PrintMessage(Window *window, MessageFormat *msgFormat, MsgData *msgData, u32 msgId, enum HeapID heapId);
void PokeathlonRingDropUi_PrintMessageAt(Window *window, MessageFormat *msgFormat, MsgData *msgData, u32 msgId, enum HeapID heapId, s16 x, s16 y);
void PokeathlonRingDropUi_CreateSprites(PokeathlonRingDropUi *ui);
void PokeathlonRingDropUi_LoadMonPic(PokeathlonCourseData *course, enum HeapID heapId, ManagedSprite *sprite, u32 participant, u8 member);
void PokeathlonRingDropUi_LoadMonIcon(PokeathlonRingDropUi *ui, s32 participant, u32 member);
void *PokeathlonRingDropUi_NewTimerSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 animation, u16 priority);
void PokeathlonRingDropUi_CreateTimerSprites(PokeathlonRingDropUi *ui);
void PokeathlonRingDropUi_PrintNextName(PokeathlonRingDropUi *ui, s32 member);
void PokeathlonRingDropUi_SetPicPosition(PokeathlonRingDropUi *ui, s32 sprite, s32 x, s32 y);
void PokeathlonRingDropUi_ShowSwitching(PokeathlonRingDropUi *ui, s32 participant, BOOL switching);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RING_DROP_H
