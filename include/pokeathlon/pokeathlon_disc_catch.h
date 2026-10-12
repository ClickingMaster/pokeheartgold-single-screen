#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_DISC_CATCH_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_DISC_CATCH_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "sprite_system.h"
#include "sys_task_api.h"

// Disc Catch (Pokeathlon event 4): the four participants' teams of three Pokemon share the touch
// screen; discs fly down from the top screen along a wavy path. Dragging a Pokemon moves it, tapping
// it makes it jump; a Pokemon that jumps under a landing disc catches it (1, 2, 3 or 5 points by the
// disc). Pokemon push each other around and the ones pushed off the field come back after 90 frames.
// The event runs for 1800 frames (60 seconds); the host simulates the field and sends a frame packet
// each frame; the participants without a player are steered by a simple CPU.

#define DISC_CATCH_PARTICIPANTS 4
#define DISC_CATCH_TEAM_SIZE    3
#define DISC_CATCH_MONS         (DISC_CATCH_PARTICIPANTS * DISC_CATCH_TEAM_SIZE)
#define DISC_CATCH_DISCS        2
#define DISC_CATCH_NO_MON       12 // "no Pokemon" in the 0..11 mon indices

typedef struct PokeathlonDiscCatchScoreboard PokeathlonDiscCatchScoreboard; // the top screen (0xE8)
typedef struct PokeathlonDiscCatchCpu PokeathlonDiscCatchCpu;               // the participants without a player (host only)

// A Pokemon's abilities, from its ratings (NARC a/1/7/0 file 7).
typedef struct PokeathlonDiscCatchStats {
    u8 jumpFrames;          // 0x00: half the jump
    u8 jumpHeight;          // 0x01
    u8 jump;                // 0x02: the jump rating; 0 can't jump
    fx32 power;             // 0x04: how hard it pushes the Pokemon it runs into
    fx32 speed;             // 0x08: the longest step per frame
} PokeathlonDiscCatchStats; // size: 0xC

enum PokeathlonDiscCatchMonState {
    DISC_CATCH_MON_JUMPING = 1,
    DISC_CATCH_MON_ON_FIELD,
    DISC_CATCH_MON_FALLEN, // pushed off the field: back after 90 frames
    DISC_CATCH_MON_RESPAWNED,
};

// One Pokemon on the field (positions in touch screen pixels, fx32).
typedef struct PokeathlonDiscCatchMon {
    PokeathlonCourseData *course;    // 0x00
    void *monPic;                    // 0x04: its course 3D object (PokeathlonCourseMonObjects_Get)
    PokeathlonDiscCatchStats *stats; // 0x08
    u32 state;                       // 0x0C
    VecFx32 target;                  // 0x10: where it walks to
    VecFx32 position;                // 0x1C
    VecFx32 direction;               // 0x28
    VecFx32 velocity;                // 0x34: while knocked back
    u32 jumpTimer : 8;               // 0x40
    u32 jumpSignal : 8;              // frames the jump is reported in the frame packet
    u32 index : 4;                   // 0..11
    u32 facing : 4;                  // 1..4
    u32 large : 2;
    u32 knockedBack : 1;
    u32 dragged : 1; // its target was set this frame
    u32 moving : 1;
    u32 draggedOff : 1; // dragged to a spot off the field
    u32 unk40_30 : 2;
    u8 respawnWait; // 0x44
    u8 participant; // 0x45
    u8 member;      // 0x46
    u8 unk47;
} PokeathlonDiscCatchMon; // size: 0x48

typedef struct PokeathlonDiscCatchResult {
    u32 participant : 2; // its index
    u32 score : 16;
    u32 unk0_18 : 14;
} PokeathlonDiscCatchResult;

// A participant's team.
typedef struct PokeathlonDiscCatchTeam {
    s16 touchX;           // 0x00
    s16 touchY;           // 0x02
    u32 touchFrames : 14; // 0x04
    u32 touchNew : 1;
    u32 touching : 1;
    u32 lastTouchFrames : 13; // of the last touch: up to 5 is a tap
    u32 holding : 1;
    u32 heldMon : 2;
    PokeathlonDiscCatchMon mons[DISC_CATCH_TEAM_SIZE]; // 0x08
    PokeathlonDiscCatchResult result;                  // 0xE0
} PokeathlonDiscCatchTeam;                             // size: 0xE4

typedef struct PokeathlonDiscCatchField {
    u8 touching[DISC_CATCH_MONS][DISC_CATCH_MONS];          // 0x00: pairs of Pokemon in contact
    PokeathlonDiscCatchTeam teams[DISC_CATCH_PARTICIPANTS]; // 0x90
} PokeathlonDiscCatchField;

enum PokeathlonDiscCatchDiscState {
    DISC_CATCH_DISC_NONE,
    DISC_CATCH_DISC_FLYING,
    DISC_CATCH_DISC_LANDED,
    DISC_CATCH_DISC_CAUGHT_1, // caught; worth 1, 2, 3 or 5 points
    DISC_CATCH_DISC_CAUGHT_2,
    DISC_CATCH_DISC_CAUGHT_3,
    DISC_CATCH_DISC_CAUGHT_5,
};

// A disc in flight (two at most).
typedef struct PokeathlonDiscCatchDisc {
    s16 x;         // 0x00
    s16 progress;  // 0x02: down the two screens, 0..480
    u32 owner : 2; // 0x04: who caught it
    u32 state : 3;
    u32 active : 1;
    u32 timer : 8;
    u32 unk4_14 : 18;
    u8 index;              // 0x08
    u8 path;               // 0x09: shape of the path, 0..14
    u8 speed;              // 0x0A
} PokeathlonDiscCatchDisc; // size: 0xC

typedef struct PokeathlonDiscCatchDiscs {
    PokeathlonDiscCatchTeam *teams;                  // 0x00
    PokeathlonDiscCatchDisc discs[DISC_CATCH_DISCS]; // 0x04
    u32 delay : 8;                                   // 0x1C: frames to the next disc
    u32 phase : 4;                                   // by the time left
    u32 unk1C_12 : 20;
} PokeathlonDiscCatchDiscs;

// A disc in the frame packet.
typedef struct PokeathlonDiscCatchDiscPacket {
    u16 progress : 7; // a quarter of the disc's progress
    u16 path : 4;
    u16 state : 3;
    u16 owner : 2;
} PokeathlonDiscCatchDiscPacket;

// The frame packet (the host's, and each participant's touch input in the same layout).
typedef struct PokeathlonDiscCatchFrame {
    u32 touchX : 8; // 0x00
    u32 touchY : 8;
    u32 touching : 1;
    u32 unk0_17 : 15;
    u8 monX[DISC_CATCH_MONS]; // 0x04: the Pokemon's places in the course's 3D field
    u8 monY[DISC_CATCH_MONS]; // 0x10
    u32 facings : 24;         // 0x1C: 2 bits per Pokemon, facing - 1
    u32 seconds : 6;          // time left
    u32 scoreParticipant : 2; // whose score this packet carries (round robin)
    u32 score : 7;            // 0x20
    u32 fallen : 12;          // a bit per Pokemon
    u32 jumped : 12;
    u32 finished : 1;
} PokeathlonDiscCatchFrame;

// Squashes a Pokemon that taps without being able to jump.
typedef struct PokeathlonDiscCatchSquash {
    void *monPic;  // 0x00
    SysTask *task; // 0x04
    f32 scale;     // 0x08
    u16 frames;    // 0x0C
    u16 state;     // 0x0E
} PokeathlonDiscCatchSquash;

typedef struct PokeathlonDiscCatchMonSprites {
    ManagedSprite *splash;            // 0x00: where it fell off
    PokeathlonDiscCatchSquash squash; // 0x04
    fx32 fallScale;                   // 0x14
    u32 jumpFrame : 8;                // 0x18
    u32 fallFrames : 8;
    u32 jumping : 1;
    u32 squashed : 1;
    u32 falling : 1;
    u32 fallen : 1;
    u32 unk18_20 : 12;
} PokeathlonDiscCatchMonSprites; // size: 0x1C

typedef struct PokeathlonDiscCatchDiscSprites {
    ManagedSprite *disc[2];   // 0x00: on the top screen, on the touch screen
    ManagedSprite *shadow[2]; // 0x08
    ManagedSprite *points;    // 0x10
    ManagedSprite *sparkle;   // 0x14
    u32 angle : 16;           // 0x18
    u32 shownState : 8;
    u32 unk18_24 : 8;
} PokeathlonDiscCatchDiscSprites; // size: 0x1C

typedef struct PokeathlonDiscCatchSprites {
    PokeathlonDiscCatchScoreboard *scoreboard;              // 0x000
    PokeathlonDiscCatchDiscSprites discs[DISC_CATCH_DISCS]; // 0x004
    PokeathlonDiscCatchMonSprites mons[DISC_CATCH_MONS];    // 0x03C
    u16 shownScores[DISC_CATCH_PARTICIPANTS];               // 0x18C
} PokeathlonDiscCatchSprites;                               // size: 0x194

// Shakes the top screen when the own team catches a disc.
typedef struct PokeathlonDiscCatchShake {
    SpriteSystem *spriteSystem; // 0x00
    BgConfig *bgConfig;         // 0x04
    SysTask *task;              // 0x08
    s8 distance;                // 0x0C
    s8 frames;                  // 0x0D
    u32 flip;                   // 0x10
} PokeathlonDiscCatchShake;     // size: 0x14

typedef struct PokeathlonDiscCatch {
    enum HeapID heapId;                                                            // 0x000
    PokeathlonCourseData *course;                                                  // 0x004
    BgConfig *bgConfig;                                                            // 0x008
    SpriteSystem *spriteSystem;                                                    // 0x00C
    SpriteManager *spriteManager;                                                  // 0x010
    MessageFormat *messageFormat;                                                  // 0x014
    MsgData *msgData;                                                              // 0x018
    void *monPicResources;                                                         // 0x01C
    void *monPics;                                                                 // 0x020
    Window window;                                                                 // 0x024
    PokeathlonDiscCatchStats stats[DISC_CATCH_PARTICIPANTS][DISC_CATCH_TEAM_SIZE]; // 0x034
    PokeathlonDiscCatchScoreboard *scoreboard;                                     // 0x0C4
    PokeathlonDiscCatchCpu *cpu;                                                   // 0x0C8
    PokeathlonDiscCatchField field;                                                // 0x0CC
    PokeathlonDiscCatchDiscs discs;                                                // 0x4EC
    int timer;                                                                     // 0x50C: frames left
    PokeathlonDiscCatchSprites sprites;                                            // 0x510
    u8 bgFrameTimer;                                                               // 0x6A4
    u8 bgFrame;                                                                    // 0x6A5
    PokeathlonDiscCatchShake shake;                                                // 0x6A8
} PokeathlonDiscCatch;                                                             // size: 0x6BC

// pokeathlon_disc_catch.c
void PokeathlonDiscCatch_ShakeTask(SysTask *task, PokeathlonDiscCatchShake *shake);
BOOL PokeathlonDiscCatchShake_IsDone(PokeathlonDiscCatchShake *shake);
void PokeathlonDiscCatchShake_Init(PokeathlonDiscCatchShake *shake, BgConfig *bgConfig, SpriteSystem *spriteSystem);
void PokeathlonDiscCatchShake_Start(PokeathlonDiscCatchShake *shake, u8 distance, u8 frames);
void PokeathlonDiscCatch_SquashTask(SysTask *task, void *data);
void PokeathlonDiscCatch_StartSquash(PokeathlonDiscCatchSquash *squash, void *monPic);
void PokeathlonDiscCatch_SetMonScaleCenter(void *monPic, int falling);
BOOL PokeathlonDiscCatchMon_IsTouching(PokeathlonDiscCatchMon *mon, PokeathlonDiscCatchMon *other);
void PokeathlonDiscCatchMon_Jump(PokeathlonDiscCatchMon *mon);
BOOL PokeathlonDiscCatchMon_IsInPlay(PokeathlonDiscCatchMon *mon);
void PokeathlonDiscCatchMon_SetTarget(PokeathlonDiscCatchMon *mon, const s16 *position);
int PokeathlonDiscCatch_GetFacing(const VecFx32 *direction);
void PokeathlonDiscCatchMon_Move(PokeathlonDiscCatchMon *mon);
BOOL PokeathlonDiscCatchMon_IsOffField(PokeathlonDiscCatchMon *mon, fx32 x, fx32 y);
void PokeathlonDiscCatchMon_Respawn(PokeathlonDiscCatchMon *mon);
void PokeathlonDiscCatchMon_Update(PokeathlonDiscCatchMon *mon, PokeathlonCourseData *course);
int PokeathlonDiscCatch_FindTouchedMon(PokeathlonDiscCatchMon *mons, s16 x, s16 y);
void PokeathlonDiscCatch_ShowDisc(PokeathlonDiscCatchDiscSprites *sprites, u32 mode, BOOL draw);
void PokeathlonDiscCatch_ScaleDisc(PokeathlonDiscCatchDiscSprites *sprites, f32 scale);
void PokeathlonDiscCatch_ShowPoints(PokeathlonDiscCatchDiscSprites *sprites, int state, s16 x, s16 y);
void PokeathlonDiscCatch_PlayCatchSound(u32 state);
void PokeathlonDiscCatch_UpdateDiscSprites(PokeathlonDiscCatchSprites *sprites, PokeathlonDiscCatchDiscPacket *discs, PokeathlonCourseData *course);
s32 PokeathlonDiscCatch_CompareDrawOrder(const void *a, const void *b);
void PokeathlonDiscCatch_UpdateMonSprites(PokeathlonDiscCatchSprites *sprites, PokeathlonDiscCatchFrame *frame, PokeathlonCourseData *course);
u32 PokeathlonDiscCatch_Intro(PokeathlonCourseData *course, u8 *state);
void PokeathlonDiscCatch_EndGame(PokeathlonCourseData *course);
u32 PokeathlonDiscCatch_Play(PokeathlonCourseData *course, u8 *state);
u32 PokeathlonDiscCatch_Finish(PokeathlonCourseData *course, u8 *state);
u32 PokeathlonDiscCatch_Init(PokeathlonCourseData *course);
u32 PokeathlonDiscCatch_Draw(PokeathlonCourseData *course);
u32 ov96_0220E60C(PokeathlonCourseData *course);
u32 PokeathlonDiscCatch_Exit(PokeathlonCourseData *course);
s16 PokeathlonDiscCatch_GetDiscX(int progress, int path);
BOOL PokeathlonDiscCatch_IsMonInPlay(PokeathlonDiscCatch *work, u32 participant, u32 member);
PokeathlonDiscCatchDisc *PokeathlonDiscCatch_GetDisc(PokeathlonDiscCatch *work, u32 index);
void PokeathlonDiscCatch_GetMonPosition(PokeathlonDiscCatch *work, u32 participant, u32 member, s16 *position, void **radius);
u8 PokeathlonDiscCatch_CountTeamsAhead(PokeathlonCourseData *course, u32 participant);
void PokeathlonDiscCatch_SetMonTarget(PokeathlonDiscCatch *work, const s16 *position, u32 participant, u32 member);
void PokeathlonDiscCatch_MakeMonJump(PokeathlonDiscCatch *work, u32 participant, u32 member);
u8 PokeathlonDiscCatch_GetTeamScore(PokeathlonDiscCatch *work, int participant);
void PokeathlonDiscCatch_SetVramBanks(void);
int PokeathlonDiscCatch_Square(int value);
void PokeathlonDiscCatch_Approach0(fx32 *value, fx32 step);
int PokeathlonDiscCatch_GetJumpOffset(PokeathlonDiscCatchStats *stats, int frame, f32 *scale);
void PokeathlonDiscCatch_SlowDown(VecFx32 *velocity, fx32 stepX, fx32 stepY);
void PokeathlonDiscCatch_ClampVelocity(VecFx32 *velocity, fx32 limit);
void PokeathlonDiscCatch_PrintScore(PokeathlonCourseData *course, int score);
void PokeathlonDiscCatch_InitSpriteSystem(PokeathlonDiscCatch *work);
void PokeathlonDiscCatch_FreeSpriteSystem(PokeathlonDiscCatch *work);
void PokeathlonDiscCatch_StopAnims(PokeathlonDiscCatch *work);
void PokeathlonDiscCatch_LoadSpriteResources(PokeathlonDiscCatch *work);
ManagedSprite *PokeathlonDiscCatch_NewDiscSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority, BOOL onTouchScreen);
ManagedSprite *PokeathlonDiscCatch_NewEffectSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority);
void PokeathlonDiscCatch_InitBgs(PokeathlonDiscCatch *work);
void PokeathlonDiscCatch_FreeBgs(PokeathlonDiscCatch *work);
void PokeathlonDiscCatch_LoadBgGraphics(PokeathlonDiscCatch *work);
void PokeathlonDiscCatch_AnimateBg(u8 *timer, BgConfig *bgConfig, enum HeapID heapId);
void PokeathlonDiscCatch_UpdateHost(PokeathlonCourseData *course);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_DISC_CATCH_H
