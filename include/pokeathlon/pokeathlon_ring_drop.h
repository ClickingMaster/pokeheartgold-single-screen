#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_RING_DROP_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_RING_DROP_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
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

// A team member's 3D object and stamina.
typedef struct PokeathlonRingDropMemberParams {
    s32 unk00;    // 0x00
    s32 unk04;    // 0x04
    fx32 stamina; // 0x08: the maximum
} PokeathlonRingDropMemberParams;

typedef struct PokeathlonRingDropMember {
    void *object;                                 // 0x00: the course mon object
    const PokeathlonRingDropMemberParams *params; // 0x04
    fx32 stamina;                                 // 0x08
} PokeathlonRingDropMember;

typedef union PokeathlonRingDropMonFlags {
    u32 raw;

    struct {
        u32 score : 12; // the participant's points
        u32 unk12 : 4;
        u32 direction : 4;
        u32 participant : 2;
        u32 member : 2;      // team member in the ring
        u32 switchState : 2; // 1: going out, 2: coming in
        u32 switching : 1;
        u32 unk27 : 1;
        u32 exhausted : 1; // stamina ran out; the Pokemon rests for 60 frames
        u32 unk29 : 1;
        u32 hasTarget : 1; // targetX/targetY are set
        u32 unk31 : 1;
    } bits;
} PokeathlonRingDropMonFlags;

// The Pokemon a participant has in the ring.
typedef struct PokeathlonRingDropMon {
    PokeathlonCourseData *course;                          // 0x00
    PokeathlonRingDropMember *member;                      // 0x04: the member in the ring
    void *unk08;                                           // 0x08
    void *unk0C;                                           // 0x0C
    void *unk10;                                           // 0x10
    s32 state;                                             // 0x14
    u32 unk18;                                             // 0x18
    s16 targetX;                                           // 0x1C
    s16 targetY;                                           // 0x1E
    VecFx32 unk20;                                         // 0x20
    VecFx32 position;                                      // 0x2C
    VecFx32 unk38;                                         // 0x38
    VecFx32 unk44;                                         // 0x44
    s32 unk50;                                             // 0x50
    s32 unk54;                                             // 0x54
    s8 unk58[4];                                           // 0x58
    u8 unk5C;                                              // 0x5C
    s8 unk5D;                                              // 0x5D
    s8 unk5E;                                              // 0x5E
    u8 unk5F;                                              // 0x5F
    PokeathlonRingDropMonFlags flags;                      // 0x60
    PokeathlonRingDropMember members[RING_DROP_TEAM_SIZE]; // 0x64
    u8 unk88[0x14];                                        // 0x88
} PokeathlonRingDropMon;                                   // size: 0x9C

// The host's record of a participant's touch.
typedef struct PokeathlonRingDropTouch {
    u32 frames : 14; // length of the current touch
    u32 started : 1; // set on the first frame of a touch
    u32 touching : 1;
    u32 lastFrames : 13; // length of the previous touch
    u32 unk29 : 3;
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
    u8 x;            // 0x00
    u8 y;            // 0x01
    u8 height : 6;   // 0x02: jump height
    u8 hit : 2;      //       1: tackle hit, 2: press hit
    u8 rank;         // 0x03
    u8 state : 4;    // 0x04
    u8 opponent : 2; //       the participant it hit
    u8 member : 2;   //       team member in the ring
    u8 status : 3;   // 0x05: the own participant's status sprite
    u8 power : 2;
    u8 switchState : 2;
    u8 unk5_7 : 1;
} PokeathlonRingDropMonPacket;

typedef union PokeathlonRingDropFrameMarks {
    u32 raw;

    struct {
        u32 unk0 : 7;
        u32 switchPressed : 1; // the own switch button is held
        u32 members : 24;      // 2 bits per team member (participant * 3 + member)
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
    void *monObject;                          // 0x00
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
    u32 opponent : 2;
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
    void *ui;                                                  // 0x000: the rank display (PokeathlonRingDrop.ui)
    SpriteSystem *spriteSystem;                                // 0x004
    SpriteManager *spriteManager;                              // 0x008
    ManagedSprite *statusSprite;                               // 0x00C
    PokeathlonRingDropMonSprites mons[RING_DROP_PARTICIPANTS]; // 0x010
    u16 ranks[RING_DROP_PARTICIPANTS];                         // 0x3B0
    u8 status;                                                 // 0x3B8
    u8 switchPressed : 1;                                      // 0x3B9
    u8 unk3B9_1 : 7;
    u16 unk3BA;              // 0x3BA
} PokeathlonRingDropDisplay; // size: 0x3BC

typedef struct PokeathlonRingDrop {
    enum HeapID heapId;                                       // 0x000
    BgConfig *bgConfig;                                       // 0x004
    SpriteSystem *spriteSystem;                               // 0x008
    SpriteManager *spriteManager;                             // 0x00C
    MessageFormat *msgFormat;                                 // 0x010
    MsgData *msgData;                                         // 0x014
    void *resources;                                          // 0x018
    void *monObjects;                                         // 0x01C
    Window window;                                            // 0x020
    u8 unk030[0x150];                                         // 0x030
    void *ui;                                                 // 0x180
    void *records;                                            // 0x184
    u8 packetState[0x10];                                     // 0x188
    PokeathlonRingDropPlayer players[RING_DROP_PARTICIPANTS]; // 0x198
    int timer;                                                // 0x438: frames left
    PokeathlonRingDropDisplay display;                        // 0x43C
    PokeathlonScreenShake shake;                              // 0x7F8
} PokeathlonRingDrop;                                         // size: 0x814

// pokeathlon_ring_drop.c
BOOL PokeathlonScreenShake_IsDone(PokeathlonScreenShake *shake);
void PokeathlonScreenShake_Init(PokeathlonScreenShake *shake, BgConfig *bgConfig, SpriteSystem *spriteSystem);
void PokeathlonScreenShake_Start(PokeathlonScreenShake *shake, int magnitude, int duration);
BOOL PokeathlonRingDrop_IsOnSwitchButton(int x, int y);
BOOL PokeathlonRingDrop_IsWithinDistance(VecFx32 *a, VecFx32 *b, fx32 distance);
void PokeathlonRingDrop_Decay(fx32 *value, fx32 amount);
void PokeathlonRingDrop_DecayVelocity(fx32 *velocity, fx32 amountX, fx32 amountY);
void PokeathlonRingDrop_ClampVelocity(fx32 *velocity, fx32 limit);
BOOL PokeathlonRingDrop_State_FadeIn(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRingDrop_State_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRingDrop_State_Finish(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonRingDrop_Init(PokeathlonCourseData *course);
BOOL PokeathlonRingDrop_Draw(PokeathlonCourseData *course);
BOOL ov96_02215CE8(PokeathlonCourseData *course);
BOOL PokeathlonRingDrop_Exit(PokeathlonCourseData *course);
s32 PokeathlonRingDrop_GetMonState(PokeathlonCourseData *course, int participant);
VecFx32 *PokeathlonRingDrop_GetMonPosition(PokeathlonCourseData *course, int participant);
void PokeathlonRingDrop_GetMonScreenPosition(PokeathlonCourseData *course, int participant, fx32 *out);
u32 PokeathlonRingDrop_GetMonDirection(PokeathlonCourseData *course, int participant);
u8 PokeathlonRingDrop_GetMonStamina(PokeathlonCourseData *course, int participant);
u8 PokeathlonRingDrop_GetMonStaminaPercent(PokeathlonCourseData *course, int participant);
u32 PokeathlonRingDrop_IsMonSwitching(PokeathlonCourseData *course, int participant);
u32 ov96_02215EB0(PokeathlonCourseData *course, int participant);
BOOL ov96_02215ECC(PokeathlonCourseData *course, int participant);
void PokeathlonRingDrop_SetMonTarget(PokeathlonCourseData *course, int participant, const s16 *target);
void ov96_02215F2C(PokeathlonCourseData *course, int participant);
void ov96_02215F64(PokeathlonCourseData *course, int participant);
void ov96_02215F80(PokeathlonCourseData *course, int participant, void *arg);
int ov96_02215FA0(void);
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
void PokeathlonRingDropDisplay_SwitchOut(PokeathlonRingDropMonSprites *mon, s16 *position, void *ui, u8 participant, PokeathlonRingDropInput *input, BOOL cpu, BOOL own);
void PokeathlonRingDropDisplay_SwitchIn(PokeathlonRingDropMonSprites *mon, s16 *position, void *ui, u8 participant, PokeathlonRingDropInput *input, BOOL report, BOOL own);
void PokeathlonRingDropDisplay_EndSwitch(PokeathlonRingDropMonSprites *mon, const s16 *position, PokeathlonRingDropInput *input, BOOL report, BOOL own);
void PokeathlonRingDropDisplay_UpdateSwitch(PokeathlonRingDropMonPacket *packet, PokeathlonRingDropMonSprites *mon, void *ui, PokeathlonCourseData *course, u8 participant);
int PokeathlonRingDropDisplay_CompareDepth(const void *a, const void *b);
int PokeathlonRingDropDisplay_CompareRanks(const void *a, const void *b);
void PokeathlonRingDropDisplay_ApplyFramePacket(PokeathlonRingDropDisplay *display, PokeathlonRingDropFramePacket *framePacket, PokeathlonCourseData *course);
void PokeathlonRingDropDisplay_Init(PokeathlonRingDropDisplay *display, SpriteSystem *spriteSystem, SpriteManager *spriteManager, void *ui, void *monObjects, enum HeapID heapId);
void PokeathlonRingDropDisplay_Update(PokeathlonRingDropDisplay *display, PokeathlonCourseData *course);
void PokeathlonRingDropDisplay_Free(PokeathlonRingDropDisplay *display);
void PokeathlonRingDropDisplay_StopAnimations(PokeathlonRingDropDisplay *display);
BOOL ov96_0221785C(int state);
void ov96_02217868(PokeathlonRingDropMon *mon, int a1);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_RING_DROP_H
