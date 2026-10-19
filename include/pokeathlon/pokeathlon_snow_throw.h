#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_SNOW_THROW_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_SNOW_THROW_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_internal.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "sprite.h"

// Snow Throw: the four teams stand at the sides of a square field and throw snowballs at each other. Tapping a
// Pokemon makes its snowball bigger and a flick throws it; a hit scores for the thrower and takes energy from the target.

#define SNOW_THROW_TEAMS        4
#define SNOW_THROW_MEMBERS      3
#define SNOW_THROW_MONS         (SNOW_THROW_TEAMS * SNOW_THROW_MEMBERS)
#define SNOW_THROW_DURATION     1800 // frames
#define SNOW_THROW_MAX_ENERGY   120
#define SNOW_THROW_FIELD_CENTER 33              // field units; the field is turned around (33, 33) for each player's view
#define SNOW_THROW_NONE         SNOW_THROW_MONS // no Pokemon / no snowball

// PokeathlonSnowThrowBall.state
#define SNOW_THROW_BALL_NONE     0
#define SNOW_THROW_BALL_FLYING   1
#define SNOW_THROW_BALL_HELD     2 // being made bigger
#define SNOW_THROW_BALL_SPLASHED 3

// PokeathlonSnowThrowMon.condition
#define SNOW_THROW_FINE    0
#define SNOW_THROW_STUNNED 1
#define SNOW_THROW_TIRED   2 // energy 100 or less

// The snowball of one Pokemon. Field positions are fx32 field units, x and z (y is 0).
typedef struct PokeathlonSnowThrowBall {
    PokeathlonCourseSpritePoolSprite *sprite; // the snowball in the air
    PokeathlonCourseSpritePoolSprite *shadowSprite;
    PokeathlonCourseSpritePoolSprite *splashSprite;
    int state; // SNOW_THROW_BALL_*
    VecFx32 position;
    VecFx32 velocity;
    VecFx32 dragStart;  // field position the stylus touched the Pokemon at
    VecFx32 touchStart; // the same in touch screen pixels (x, z)
    u8 splashTimer;
    u8 index;       // Pokemon (team * 3 + member)
    u8 splashShown; // drawing: the splash of this throw was shown
    u8 hit;         // hit a Pokemon (sent once, then cleared)
    u16 taps;       // 1-4
    u16 size;       // 0-3, from the taps
} PokeathlonSnowThrowBall;

// One of the twelve Pokemon.
typedef struct PokeathlonSnowThrowMon {
    PokeathlonCourseSpritePoolSprite *stunSprite; // shown over it while stunned
    f32 throwWeight;                              // slows its throws down (tenths, by its Pokeathlon stats)
    f32 throwPower;                               // scales the energy its hits take
    f32 hitDamage;                                // scales the energy hits take from it
    u32 aimSpread;                                // degrees its throws stray by at most
    u16 hits;                                     // the Pokemon its snowballs hit (score, max 999)
    s16 energy;                                   // 0-SNOW_THROW_MAX_ENERGY
    u8 stunTimer;
    u8 condition; // SNOW_THROW_FINE / _STUNNED / _TIRED
    u8 unk1A;
    u8 wasHit;      // a snowball hit it this frame
    u16 animTimer;  // drawing: the animation shown last
    u16 throwShown; // drawing: the throw animation was started
} PokeathlonSnowThrowMon;

// What the host keeps of one participant's stylus.
typedef struct PokeathlonSnowThrowPress {
    BOOL pressed; // on the frame the stylus touches the screen
    BOOL held;
    u32 unk8;
} PokeathlonSnowThrowPress;

// The stylus of a player.
typedef struct PokeathlonSnowThrowTouch {
    BOOL held;
    u8 x;
    u8 y;
    u8 unk06[0x1E];
} PokeathlonSnowThrowTouch;

typedef struct PokeathlonSnowThrowTouchBlock {
    PokeathlonSnowThrowTouch touch;
    u8 unk24[4];
} PokeathlonSnowThrowTouchBlock;

// Everything a player draws, sent by the host every frame.
typedef struct PokeathlonSnowThrowPacket {
    u16 teams[SNOW_THROW_TEAMS]; // hits of the team (bits 0-9) | size of each member's snowball (2 bits) << 10
    u16 mons[SNOW_THROW_MONS];   // snowball x - 1 (bits 0-5), z - 1 (6-11), was hit (12), the snowball hit someone (13),
                                 // condition (14-15)
    s32 flags;                   // state of each snowball (2 bits), finished << 24, seconds left << 25
    u8 unk24[4];
} PokeathlonSnowThrowPacket;

// The link data of the event (PokeathlonCourse_GetDataCopyArea).
typedef struct PokeathlonSnowThrowLink {
    PokeathlonSnowThrowTouchBlock touch;                     // this player's stylus
    PokeathlonSnowThrowPacket packetToSend;                  // host
    PokeathlonSnowThrowTouchBlock touches[SNOW_THROW_TEAMS]; // everyone's stylus, collected by the host
    PokeathlonSnowThrowPacket packet;                        // received from the host
} PokeathlonSnowThrowLink;

// The top screen: the own team's pictures, the score and the player's name.
typedef struct PokeathlonSnowThrowMonPic {
    void *character;
    void *palette;
} PokeathlonSnowThrowMonPic;

typedef struct PokeathlonSnowThrowDisplay {
    enum HeapID heapId;
    PokeathlonCourseData *course;
    BgConfig *bgConfig;
    Window nameWindow;
    void *scoreScreenFile;
    NNSG2dScreenData *scoreScreen; // the score digits (5 x 2 of 4 x 8 tiles)
    PokeathlonSnowThrowMonPic monPics[SNOW_THROW_MEMBERS];
    u32 picCharOffsets[SNOW_THROW_MEMBERS];
    u32 picPlttOffsets[SNOW_THROW_MEMBERS];
    void *picSprites[SNOW_THROW_MEMBERS];
    void *tiredSprites[SNOW_THROW_MEMBERS];
    u8 unk6C[4];
    u8 shaking;
    u8 shakeTimer;
    u8 unk72[2];
} PokeathlonSnowThrowDisplay;

// What the CPU picks the target of a throw by.
typedef struct PokeathlonSnowThrowCpuTarget {
    PokeathlonSnowThrowMon *mon;
    u16 index;
    u16 score;
} PokeathlonSnowThrowCpuTarget;

// A node of the list PokeathlonSnowThrowCpu_SortTargets sorts with (node 0 is the head).
typedef struct PokeathlonSnowThrowCpuTargetNode {
    struct PokeathlonSnowThrowCpuTargetNode *prev;
    struct PokeathlonSnowThrowCpuTargetNode *next;
    PokeathlonSnowThrowCpuTarget *target;
} PokeathlonSnowThrowCpuTargetNode;

// The CPU of the teams without a player: every `interval` frames it looks at one member of each CPU team in turn.
typedef struct PokeathlonSnowThrowCpu {
    PokeathlonSnowThrowMon *mons[SNOW_THROW_MONS];
    PokeathlonSnowThrowBall *balls[SNOW_THROW_MONS];
    PokeathlonSnowThrowCpuTarget targets[SNOW_THROW_MONS];
    u32 thinking;
    u8 cpuCount;
    u8 cursor;
    u8 timer;
    u8 interval;
    u8 firstCpu;
    u8 humanCount;
    u8 member;
    u8 unkCB;
} PokeathlonSnowThrowCpu;

typedef struct PokeathlonSnowThrow {
    BgConfig *bgConfig;
    Window scoreWindow;                       // the result
    Window nameWindows[SNOW_THROW_TEAMS - 1]; // the other players' names
    enum HeapID heapId;
    PokeathlonCourseSpritePool *spritePool;
    PokeathlonCourseSpritePoolSprite *frameSprite;
    Sprite *timerDigits[2];
    void *teamMarks[SNOW_THROW_TEAMS - 1];                  // the colour of each other team
    void *hitMarks[SNOW_THROW_MEMBERS];                     // where the own snowballs hit
    void *tiredMarks[SNOW_THROW_MONS - SNOW_THROW_MEMBERS]; // over the other teams' Pokemon
    void *monObjects[SNOW_THROW_MONS - SNOW_THROW_MEMBERS]; // the other teams' Pokemon (the own are on the top screen)
    PokeathlonSnowThrowBall balls[SNOW_THROW_MONS];
    PokeathlonSnowThrowMon mons[SNOW_THROW_MONS];
    void *sortBuffer; // MATH_QSort work area
    PokeathlonSnowThrowPress presses[SNOW_THROW_TEAMS];
    u8 heldBall[SNOW_THROW_TEAMS]; // the snowball each participant is making / aiming
    u8 finished;
    u8 unk5D1[3];
    PokeathlonCourseSpriteResources *monResources;
    void *monObjectList; // PokeathlonCourseMonObjects
    PokeathlonSnowThrowCpu *cpu;
    PokeathlonSnowThrowDisplay *display;
    u32 unk5E4;
    u16 timer;     // frames left
    u8 holdFrames; // frames the stylus has been held on the own Pokemon (max 30)
    u8 unk5EB;
    u8 shownSizes[SNOW_THROW_MEMBERS]; // drawing: size of the own snowballs shown last
    u8 unk5EF[SNOW_THROW_MEMBERS];
    u8 unk5F2[2];
} PokeathlonSnowThrow;

BOOL PokeathlonSnowThrow_Init(PokeathlonCourseData *course);
BOOL PokeathlonSnowThrow_Draw(PokeathlonCourseData *course);
BOOL PokeathlonSnowThrow_ShowResults(PokeathlonCourseData *course);
BOOL PokeathlonSnowThrow_Exit(PokeathlonCourseData *course);
BOOL PokeathlonSnowThrow_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonSnowThrow_SaveScores(PokeathlonCourseData *course, u8 *state);

PokeathlonSnowThrowDisplay *PokeathlonSnowThrowDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
void PokeathlonSnowThrowDisplay_Delete(PokeathlonSnowThrowDisplay *display);
void PokeathlonSnowThrowDisplay_LoadBgGraphics(PokeathlonSnowThrowDisplay *display, u32 own);
void PokeathlonSnowThrowDisplay_LoadSpriteGraphics(PokeathlonSnowThrowDisplay *display, PokeathlonCourseSpritePool *pool);
void PokeathlonSnowThrowDisplay_CreateSprites(PokeathlonSnowThrowDisplay *display, PokeathlonCourseSpritePool *pool, PokeathlonCourseMonInfo *mons);
void PokeathlonSnowThrowDisplay_ShowTired(PokeathlonSnowThrowDisplay *display, u8 member, BOOL show);
void PokeathlonSnowThrowDisplay_SetScore(PokeathlonSnowThrowDisplay *display, u16 score);
void PokeathlonSnowThrowDisplay_SetPicAnim(PokeathlonSnowThrowDisplay *display, u8 member, int anim);
BOOL PokeathlonSnowThrowDisplay_IsPicAnimated(PokeathlonSnowThrowDisplay *display, u8 member);
void PokeathlonSnowThrowDisplay_Shake(PokeathlonSnowThrowDisplay *display);
void PokeathlonSnowThrowDisplay_UpdateShake(PokeathlonSnowThrowDisplay *display);
void PokeathlonSnowThrowDisplay_SetMonShadows(PokeathlonSnowThrowDisplay *display, PokeathlonCourseSpritePool *pool, PokeathlonCourseSpriteResources *monResources);
void PokeathlonSnowThrow_ScreenToField(int screenX, int screenY, float *fieldX, float *fieldZ);
void PokeathlonSnowThrow_FieldToScreen(float fieldX, float fieldZ, int *screenX, int *screenY);
void PokeathlonSnowThrow_AddAimSpread(int degrees, VecFx32 *direction);
PokeathlonSnowThrowCpu *PokeathlonSnowThrowCpu_New(enum HeapID heapId, int cpuCount, int mode);
void PokeathlonSnowThrowCpu_SetMon(PokeathlonSnowThrowCpu *cpu, u8 index, PokeathlonSnowThrowMon *mon, PokeathlonSnowThrowBall *ball);
void PokeathlonSnowThrowCpu_Delete(PokeathlonSnowThrowCpu *cpu);
int PokeathlonSnowThrowCpu_Update(PokeathlonSnowThrowCpu *cpu, int elapsed);
u8 PokeathlonSnowThrowCpu_SortTargets(u32 ownTeam, PokeathlonSnowThrowCpuTarget **targets, PokeathlonSnowThrowCpuTarget **sorted);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_SNOW_THROW_H
