#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_DISC_CATCH_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_DISC_CATCH_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "sprite_system.h"
#include "sys_task_api.h"

// Disc Catch: discs fly down from the top screen and the Pokemon catch them by jumping under them as they land
// (1, 2, 3 or 5 points by the disc). Dragging a Pokemon moves it, tapping it makes it jump.

#define DISC_CATCH_PARTICIPANTS 4
#define DISC_CATCH_TEAM_SIZE    3
#define DISC_CATCH_MONS         (DISC_CATCH_PARTICIPANTS * DISC_CATCH_TEAM_SIZE)
#define DISC_CATCH_DISCS        2
#define DISC_CATCH_NO_MON       12 // "no Pokemon" in the 0..11 mon indices

typedef struct PokeathlonDiscCatchScoreboard PokeathlonDiscCatchScoreboard; // the top screen
typedef struct PokeathlonDiscCatchCpu PokeathlonDiscCatchCpu;               // the participants without a player (host only)

// A Pokemon's abilities, from its ratings (NARC a/1/7/0 file 7).
typedef struct PokeathlonDiscCatchStats {
    u8 jumpFrames; // half the jump
    u8 jumpHeight;
    u8 jump;    // the jump rating; 0 can't jump
    fx32 power; // how hard it pushes the Pokemon it runs into
    fx32 speed; // the longest step per frame
} PokeathlonDiscCatchStats;

enum PokeathlonDiscCatchMonState {
    DISC_CATCH_MON_JUMPING = 1,
    DISC_CATCH_MON_ON_FIELD,
    DISC_CATCH_MON_FALLEN, // pushed off the field: back after 90 frames
    DISC_CATCH_MON_RESPAWNED,
};

// One Pokemon on the field (positions in touch screen pixels, fx32).
typedef struct PokeathlonDiscCatchMon {
    PokeathlonCourseData *course;
    PokeathlonCourseMonObject *monPic; // its course 3D object (PokeathlonCourseMonObjects_Get)
    PokeathlonDiscCatchStats *stats;
    u32 state;
    VecFx32 target; // where it walks to
    VecFx32 position;
    VecFx32 direction;
    VecFx32 velocity; // while knocked back
    u32 jumpTimer : 8;
    u32 jumpSignal : 8;                // frames the jump is reported in the frame packet
    u32 index : 4;                     // 0..11
    u32 facing : 4;                    // 1..4
    u32 large : 2;
    u32 knockedBack : 1;
    u32 dragged : 1; // its target was set this frame
    u32 moving : 1;
    u32 draggedOff : 1; // dragged to a spot off the field
    u32 unk40_30 : 2;
    u8 respawnWait;
    u8 participant;
    u8 member;
    u8 unk47;
} PokeathlonDiscCatchMon;

typedef struct PokeathlonDiscCatchResult {
    u32 participant : 2; // its index
    u32 score : 16;
    u32 unk0_18 : 14;
} PokeathlonDiscCatchResult;

// A participant's team.
typedef struct PokeathlonDiscCatchTeam {
    s16 touchX;
    s16 touchY;
    u32 touchFrames : 14;
    u32 touchNew : 1;
    u32 touching : 1;
    u32 lastTouchFrames : 13; // of the last touch: up to 5 is a tap
    u32 holding : 1;
    u32 heldMon : 2;
    PokeathlonDiscCatchMon mons[DISC_CATCH_TEAM_SIZE];
    PokeathlonDiscCatchResult result;
} PokeathlonDiscCatchTeam;

typedef struct PokeathlonDiscCatchField {
    u8 touching[DISC_CATCH_MONS][DISC_CATCH_MONS]; // pairs of Pokemon in contact
    PokeathlonDiscCatchTeam teams[DISC_CATCH_PARTICIPANTS];
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
    s16 x;
    s16 progress;  // down the two screens, 0..480
    u32 owner : 2; // who caught it
    u32 state : 3;
    u32 active : 1;
    u32 timer : 8;
    u32 unk4_14 : 18;
    u8 index;
    u8 path; // shape of the path, 0..14
    u8 speed;
} PokeathlonDiscCatchDisc;

typedef struct PokeathlonDiscCatchDiscs {
    PokeathlonDiscCatchTeam *teams;
    PokeathlonDiscCatchDisc discs[DISC_CATCH_DISCS];
    u32 delay : 8;                                   // frames to the next disc
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
    u32 touchX : 8;
    u32 touchY : 8;
    u32 touching : 1;
    u32 unk0_17 : 15;
    u8 monX[DISC_CATCH_MONS]; // the Pokemon's places in the course's 3D field
    u8 monY[DISC_CATCH_MONS];
    u32 facings : 24;         // 2 bits per Pokemon, facing - 1
    u32 seconds : 6;          // time left
    u32 scoreParticipant : 2; // whose score this packet carries (round robin)
    u32 score : 7;
    u32 fallen : 12;          // a bit per Pokemon
    u32 jumped : 12;
    u32 finished : 1;
} PokeathlonDiscCatchFrame;

// Squashes a Pokemon that taps without being able to jump.
typedef struct PokeathlonDiscCatchSquash {
    PokeathlonCourseMonObject *monPic;
    SysTask *task;
    f32 scale;
    u16 frames;
    u16 state;
} PokeathlonDiscCatchSquash;

typedef struct PokeathlonDiscCatchMonSprites {
    ManagedSprite *splash; // where it fell off
    PokeathlonDiscCatchSquash squash;
    fx32 fallScale;
    u32 jumpFrame : 8;
    u32 fallFrames : 8;
    u32 jumping : 1;
    u32 squashed : 1;
    u32 falling : 1;
    u32 fallen : 1;
    u32 unk18_20 : 12;
} PokeathlonDiscCatchMonSprites;

typedef struct PokeathlonDiscCatchDiscSprites {
    ManagedSprite *disc[2]; // on the top screen, on the touch screen
    ManagedSprite *shadow[2];
    ManagedSprite *points;
    ManagedSprite *sparkle;
    u32 angle : 16;
    u32 shownState : 8;
    u32 unk18_24 : 8;
} PokeathlonDiscCatchDiscSprites;

typedef struct PokeathlonDiscCatchSprites {
    PokeathlonDiscCatchScoreboard *scoreboard;
    PokeathlonDiscCatchDiscSprites discs[DISC_CATCH_DISCS];
    PokeathlonDiscCatchMonSprites mons[DISC_CATCH_MONS];
    u16 shownScores[DISC_CATCH_PARTICIPANTS];
} PokeathlonDiscCatchSprites;

// Shakes the top screen when the own team catches a disc.
typedef struct PokeathlonDiscCatchShake {
    SpriteSystem *spriteSystem;
    BgConfig *bgConfig;
    SysTask *task;
    s8 distance;
    s8 frames;
    u32 flip;
} PokeathlonDiscCatchShake;

typedef struct PokeathlonDiscCatch {
    enum HeapID heapId;
    PokeathlonCourseData *course;
    BgConfig *bgConfig;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    MessageFormat *messageFormat;
    MsgData *msgData;
    PokeathlonCourseSpriteResources *monPicResources;
    void *monPics;
    Window window;
    PokeathlonDiscCatchStats stats[DISC_CATCH_PARTICIPANTS][DISC_CATCH_TEAM_SIZE];
    PokeathlonDiscCatchScoreboard *scoreboard;
    PokeathlonDiscCatchCpu *cpu;
    PokeathlonDiscCatchField field;
    PokeathlonDiscCatchDiscs discs;
    int timer; // frames left
    PokeathlonDiscCatchSprites sprites;
    u8 bgFrameTimer;
    u8 bgFrame;
    PokeathlonDiscCatchShake shake;
} PokeathlonDiscCatch;

void PokeathlonDiscCatch_SetMonScaleCenter(PokeathlonCourseMonObject *monPic, int falling);
BOOL PokeathlonDiscCatchMon_IsTouching(PokeathlonDiscCatchMon *mon, PokeathlonDiscCatchMon *other);
void PokeathlonDiscCatchMon_Jump(PokeathlonDiscCatchMon *mon);
BOOL PokeathlonDiscCatchMon_IsInPlay(PokeathlonDiscCatchMon *mon);
void PokeathlonDiscCatchMon_SetTarget(PokeathlonDiscCatchMon *mon, const s16 *position);
int PokeathlonDiscCatch_GetFacing(const VecFx32 *direction);
BOOL PokeathlonDiscCatchMon_IsOffField(PokeathlonDiscCatchMon *mon, fx32 x, fx32 y);
void PokeathlonDiscCatchMon_Update(PokeathlonDiscCatchMon *mon, PokeathlonCourseData *course);
int PokeathlonDiscCatch_FindTouchedMon(PokeathlonDiscCatchMon *mons, s16 x, s16 y);
void PokeathlonDiscCatch_UpdateDiscSprites(PokeathlonDiscCatchSprites *sprites, PokeathlonDiscCatchDiscPacket *discs, PokeathlonCourseData *course);
void PokeathlonDiscCatch_UpdateMonSprites(PokeathlonDiscCatchSprites *sprites, PokeathlonDiscCatchFrame *frame, PokeathlonCourseData *course);
u32 PokeathlonDiscCatch_Intro(PokeathlonCourseData *course, u8 *state);
void PokeathlonDiscCatch_EndGame(PokeathlonCourseData *course);
u32 PokeathlonDiscCatch_Play(PokeathlonCourseData *course, u8 *state);
u32 PokeathlonDiscCatch_Finish(PokeathlonCourseData *course, u8 *state);
u32 PokeathlonDiscCatch_Init(PokeathlonCourseData *course);
u32 PokeathlonDiscCatch_Draw(PokeathlonCourseData *course);
u32 PokeathlonDiscCatch_ShowResults(PokeathlonCourseData *course);
u32 PokeathlonDiscCatch_Exit(PokeathlonCourseData *course);
s16 PokeathlonDiscCatch_GetDiscX(int progress, int path);
BOOL PokeathlonDiscCatch_IsMonInPlay(PokeathlonDiscCatch *work, u32 participant, u32 member);
PokeathlonDiscCatchDisc *PokeathlonDiscCatch_GetDisc(PokeathlonDiscCatch *work, u32 index);
void PokeathlonDiscCatch_GetMonPosition(PokeathlonDiscCatch *work, u32 participant, u32 member, s16 *position, void **radius);
u8 PokeathlonDiscCatch_CountTeamsAhead(PokeathlonCourseData *course, u32 participant);
void PokeathlonDiscCatch_SetMonTarget(PokeathlonDiscCatch *work, const s16 *position, u32 participant, u32 member);
void PokeathlonDiscCatch_MakeMonJump(PokeathlonDiscCatch *work, u32 participant, u32 member);
u8 PokeathlonDiscCatch_GetTeamScore(PokeathlonDiscCatch *work, int participant);
void PokeathlonDiscCatch_ClampVelocity(VecFx32 *velocity, fx32 limit);
void PokeathlonDiscCatch_StopAnims(PokeathlonDiscCatch *work);
ManagedSprite *PokeathlonDiscCatch_NewDiscSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority, BOOL onTouchScreen);
ManagedSprite *PokeathlonDiscCatch_NewEffectSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_DISC_CATCH_H
