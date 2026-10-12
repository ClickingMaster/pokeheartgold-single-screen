#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_SNOW_THROW_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_SNOW_THROW_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_internal.h"

#include "bg_window.h"
#include "sprite.h"

// Snow Throw (Pokeathlon event 8): the four teams stand at the sides of a square field (64 x 64 units) for 1800
// frames (30 seconds). The player touches one of the own three Pokemon, taps it to make its snowball bigger
// (up to 4 taps) and flicks towards the other teams. A snowball that reaches another team's Pokemon scores a hit
// for the thrower and takes energy from the Pokemon it hit, which is stunned after a hard hit and gets tired when
// its energy is low. Every player sees the field turned so that the own team stands at the bottom. The host steps
// the snowballs and sends the result to everyone in a 0x28 packet; the teams without a player are thrown for by
// a CPU.

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

// The snowball of one Pokemon (0x48). Field positions are fx32 field units, x and z (y is 0).
typedef struct PokeathlonSnowThrowBall {
    void *sprite;       // 0x00: PokeathlonCourseSpritePoolSprite: the snowball in the air
    void *shadowSprite; // 0x04
    void *splashSprite; // 0x08
    int state;          // 0x0C: SNOW_THROW_BALL_*
    VecFx32 position;   // 0x10
    VecFx32 velocity;   // 0x1C
    VecFx32 dragStart;  // 0x28: field position the stylus touched the Pokemon at
    VecFx32 touchStart; // 0x34: the same in touch screen pixels (x, z)
    u8 splashTimer;     // 0x40
    u8 index;           // 0x41: Pokemon (team * 3 + member)
    u8 splashShown;     // 0x42: drawing: the splash of this throw was shown
    u8 hit;             // 0x43: hit a Pokemon (sent once, then cleared)
    u16 taps;           // 0x44: 1-4
    u16 size;           // 0x46: 0-3, from the taps
} PokeathlonSnowThrowBall;

// One of the twelve Pokemon (0x20).
typedef struct PokeathlonSnowThrowMon {
    void *stunSprite; // 0x00: PokeathlonCourseSpritePoolSprite: shown over it while stunned
    f32 throwWeight;  // 0x04: slows its throws down (tenths, by its Pokeathlon stats)
    f32 throwPower;   // 0x08: scales the energy its hits take
    f32 hitDamage;    // 0x0C: scales the energy hits take from it
    u32 aimSpread;    // 0x10: degrees its throws stray by at most
    u16 hits;         // 0x14: the Pokemon its snowballs hit (score, max 999)
    s16 energy;       // 0x16: 0-SNOW_THROW_MAX_ENERGY
    u8 stunTimer;     // 0x18
    u8 condition;     // 0x19: SNOW_THROW_FINE / _STUNNED / _TIRED
    u8 unk1A;         // 0x1A
    u8 wasHit;        // 0x1B: a snowball hit it this frame
    u16 animTimer;    // 0x1C: drawing: the animation shown last
    u16 throwShown;   // 0x1E: drawing: the throw animation was started
} PokeathlonSnowThrowMon;

// What the host keeps of one participant's stylus (0xC).
typedef struct PokeathlonSnowThrowPress {
    BOOL pressed; // 0x0: on the frame the stylus touches the screen
    BOOL held;    // 0x4
    u32 unk8;     // 0x8
} PokeathlonSnowThrowPress;

// The stylus of a player (0x24, in a 0x28 block of the link data).
typedef struct PokeathlonSnowThrowTouch {
    BOOL held;      // 0x00
    u8 x;           // 0x04
    u8 y;           // 0x05
    u8 unk06[0x1E]; // 0x06
} PokeathlonSnowThrowTouch;

typedef struct PokeathlonSnowThrowTouchBlock {
    PokeathlonSnowThrowTouch touch; // 0x00
    u8 unk24[4];                    // 0x24
} PokeathlonSnowThrowTouchBlock;

// Everything a player draws, sent by the host every frame (0x28).
typedef struct PokeathlonSnowThrowPacket {
    u16 teams[SNOW_THROW_TEAMS]; // 0x00: hits of the team (bits 0-9) | size of each member's snowball (2 bits) << 10
    u16 mons[SNOW_THROW_MONS];   // 0x08: snowball x - 1 (bits 0-5), z - 1 (6-11), was hit (12), the snowball hit
                                 //       someone (13), condition (14-15)
    s32 flags;                   // 0x20: state of each snowball (2 bits), finished << 24, seconds left << 25
    u8 unk24[4];                 // 0x24
} PokeathlonSnowThrowPacket;

// The link data of the event (PokeathlonCourse_GetDataCopyArea).
typedef struct PokeathlonSnowThrowLink {
    PokeathlonSnowThrowTouchBlock touch;                     // 0x000: this player's stylus
    PokeathlonSnowThrowPacket packetToSend;                  // 0x028: host
    PokeathlonSnowThrowTouchBlock touches[SNOW_THROW_TEAMS]; // 0x050: everyone's stylus, collected by the host
    PokeathlonSnowThrowPacket packet;                        // 0x0F0: received from the host
} PokeathlonSnowThrowLink;

// The top screen: the own team's pictures, the score and the player's name (0x74).
typedef struct PokeathlonSnowThrowMonPic {
    void *character;
    void *palette;
} PokeathlonSnowThrowMonPic;

typedef struct PokeathlonSnowThrowDisplay {
    enum HeapID heapId;                                    // 0x00
    PokeathlonCourseData *course;                          // 0x04
    BgConfig *bgConfig;                                    // 0x08
    Window nameWindow;                                     // 0x0C
    void *scoreScreenFile;                                 // 0x1C
    NNSG2dScreenData *scoreScreen;                         // 0x20: the score digits (5 x 2 of 4 x 8 tiles)
    PokeathlonSnowThrowMonPic monPics[SNOW_THROW_MEMBERS]; // 0x24
    u32 picCharOffsets[SNOW_THROW_MEMBERS];                // 0x3C
    u32 picPlttOffsets[SNOW_THROW_MEMBERS];                // 0x48
    void *picSprites[SNOW_THROW_MEMBERS];                  // 0x54
    void *tiredSprites[SNOW_THROW_MEMBERS];                // 0x60
    u8 unk6C[4];                                           // 0x6C
    u8 shaking;                                            // 0x70
    u8 shakeTimer;                                         // 0x71
    u8 unk72[2];                                           // 0x72
} PokeathlonSnowThrowDisplay;                              // size: 0x74

// What the CPU picks the target of a throw by (8).
typedef struct PokeathlonSnowThrowCpuTarget {
    PokeathlonSnowThrowMon *mon; // 0x0
    u16 index;                   // 0x4
    u16 score;                   // 0x6
} PokeathlonSnowThrowCpuTarget;

// A node of the list PokeathlonSnowThrowCpu_SortTargets sorts with (node 0 is the head).
typedef struct PokeathlonSnowThrowCpuTargetNode {
    struct PokeathlonSnowThrowCpuTargetNode *prev;
    struct PokeathlonSnowThrowCpuTargetNode *next;
    PokeathlonSnowThrowCpuTarget *target;
} PokeathlonSnowThrowCpuTargetNode;

// The CPU of the teams without a player: every `interval` frames it looks at one member of each CPU team in turn.
typedef struct PokeathlonSnowThrowCpu {
    PokeathlonSnowThrowMon *mons[SNOW_THROW_MONS];         // 0x00
    PokeathlonSnowThrowBall *balls[SNOW_THROW_MONS];       // 0x30
    PokeathlonSnowThrowCpuTarget targets[SNOW_THROW_MONS]; // 0x60
    u32 thinking;                                          // 0xC0
    u8 cpuCount;                                           // 0xC4
    u8 cursor;                                             // 0xC5
    u8 timer;                                              // 0xC6
    u8 interval;                                           // 0xC7
    u8 firstCpu;                                           // 0xC8
    u8 humanCount;                                         // 0xC9
    u8 member;                                             // 0xCA
    u8 unkCB;                                              // 0xCB
} PokeathlonSnowThrowCpu;                                  // size: 0xCC

typedef struct PokeathlonSnowThrow {
    BgConfig *bgConfig;                                     // 0x000
    Window scoreWindow;                                     // 0x004: the result
    Window nameWindows[SNOW_THROW_TEAMS - 1];               // 0x014: the other players' names
    enum HeapID heapId;                                     // 0x044
    void *spritePool;                                       // 0x048: PokeathlonCourseSpritePool
    void *frameSprite;                                      // 0x04C
    Sprite *timerDigits[2];                                 // 0x050
    void *teamMarks[SNOW_THROW_TEAMS - 1];                  // 0x058: the colour of each other team
    void *hitMarks[SNOW_THROW_MEMBERS];                     // 0x064: where the own snowballs hit
    void *tiredMarks[SNOW_THROW_MONS - SNOW_THROW_MEMBERS]; // 0x070: over the other teams' Pokemon
    void *monObjects[SNOW_THROW_MONS - SNOW_THROW_MEMBERS]; // 0x094: the other teams' Pokemon (the own are on the top screen)
    PokeathlonSnowThrowBall balls[SNOW_THROW_MONS];         // 0x0B8
    PokeathlonSnowThrowMon mons[SNOW_THROW_MONS];           // 0x418
    void *sortBuffer;                                       // 0x598: MATH_QSort work area
    PokeathlonSnowThrowPress presses[SNOW_THROW_TEAMS];     // 0x59C
    u8 heldBall[SNOW_THROW_TEAMS];                          // 0x5CC: the snowball each participant is making / aiming
    u8 finished;                                            // 0x5D0
    u8 unk5D1[3];                                           // 0x5D1
    void *monResources;                                     // 0x5D4: PokeathlonCourseSpriteResources
    void *monObjectList;                                    // 0x5D8: PokeathlonCourseMonObjects
    PokeathlonSnowThrowCpu *cpu;                            // 0x5DC
    PokeathlonSnowThrowDisplay *display;                    // 0x5E0
    u32 unk5E4;                                             // 0x5E4
    u16 timer;                                              // 0x5E8: frames left
    u8 holdFrames;                                          // 0x5EA: frames the stylus has been held on the own Pokemon (max 30)
    u8 unk5EB;                                              // 0x5EB
    u8 shownSizes[SNOW_THROW_MEMBERS];                      // 0x5EC: drawing: size of the own snowballs shown last
    u8 unk5EF[SNOW_THROW_MEMBERS];                          // 0x5EF
    u8 unk5F2[2];                                           // 0x5F2
} PokeathlonSnowThrow;                                      // size: 0x5F4

// pokeathlon_snow_throw.c
BOOL PokeathlonSnowThrow_Init(PokeathlonCourseData *course);
BOOL PokeathlonSnowThrow_Draw(PokeathlonCourseData *course);
BOOL PokeathlonSnowThrow_ShowResults(PokeathlonCourseData *course);
BOOL PokeathlonSnowThrow_Exit(PokeathlonCourseData *course);
void PokeathlonSnowThrow_SetGfxBanks(void);
void PokeathlonSnowThrow_InitBgs(BgConfig *bgConfig);
void PokeathlonSnowThrow_LoadBgGraphics(PokeathlonSnowThrow *work, u8 own);
void PokeathlonSnowThrow_LoadSpriteGraphics(void *pool);
void PokeathlonSnowThrow_CreateSprites(PokeathlonSnowThrow *work, void *pool, u8 own);
BOOL PokeathlonSnowThrow_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonSnowThrow_SaveScores(PokeathlonCourseData *course, u8 *state);
void PokeathlonSnowThrow_UpdateHost(PokeathlonCourseData *course);
void PokeathlonSnowThrow_PackPacket(PokeathlonSnowThrow *work, PokeathlonSnowThrowPacket *packet);
BOOL PokeathlonSnowThrow_Update(PokeathlonCourseData *course);
void PokeathlonSnowThrow_DrawPacket(PokeathlonCourseData *course);
u8 PokeathlonSnowThrow_FindOwnMonAt(PokeathlonSnowThrow *work, u8 team, u32 x, u32 y, VecFx32 *out);
void PokeathlonSnowThrow_InitMon(PokeathlonCourseData *course, s32 stats[][5], u8 team, u8 member, PokeathlonSnowThrowMon *mon);
void PokeathlonSnowThrow_InitWindows(PokeathlonSnowThrow *work);
void PokeathlonSnowThrow_PrintScore(PokeathlonSnowThrow *work, u16 score);
void PokeathlonSnowThrow_UpdateMons(PokeathlonCourseData *course);
void PokeathlonSnowThrow_MoveBalls(PokeathlonCourseData *course);
void PokeathlonSnowThrow_HitMons(PokeathlonCourseData *course, PokeathlonSnowThrow *work);

// pokeathlon_snow_throw_display.c
void PokeathlonSnowThrow_CollideBalls(PokeathlonSnowThrow *work);
void PokeathlonSnowThrow_PrintNames(PokeathlonSnowThrow *work, PokeathlonCourseData *course);
void PokeathlonSnowThrow_SetTimerDigits(PokeathlonSnowThrow *work, u32 seconds);
void PokeathlonSnowThrow_ShowOwnScore(PokeathlonCourseData *course);
int PokeathlonSnowThrow_CompareDrawOrder(const void *a, const void *b);
void PokeathlonSnowThrow_SortSprites(PokeathlonSnowThrow *work, u16 own);
PokeathlonSnowThrowDisplay *PokeathlonSnowThrowDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
void PokeathlonSnowThrowDisplay_Delete(PokeathlonSnowThrowDisplay *display);
void PokeathlonSnowThrowDisplay_LoadBgGraphics(PokeathlonSnowThrowDisplay *display, u32 own);
void PokeathlonSnowThrowDisplay_LoadSpriteGraphics(PokeathlonSnowThrowDisplay *display, void *pool);
void PokeathlonSnowThrowDisplay_CreateSprites(PokeathlonSnowThrowDisplay *display, void *pool, PokeathlonCourseMonInfo *mons);
void PokeathlonSnowThrowDisplay_ShowTired(PokeathlonSnowThrowDisplay *display, u8 member, BOOL show);
void PokeathlonSnowThrowDisplay_SetScore(PokeathlonSnowThrowDisplay *display, u16 score);
void PokeathlonSnowThrowDisplay_SetPicAnim(PokeathlonSnowThrowDisplay *display, u8 member, int anim);
BOOL PokeathlonSnowThrowDisplay_IsPicAnimated(PokeathlonSnowThrowDisplay *display, u8 member);
void PokeathlonSnowThrowDisplay_Shake(PokeathlonSnowThrowDisplay *display);
void PokeathlonSnowThrowDisplay_UpdateShake(PokeathlonSnowThrowDisplay *display);
void PokeathlonSnowThrowDisplay_SetMonShadows(PokeathlonSnowThrowDisplay *display, void *pool, void *monResources);
void PokeathlonSnowThrowDisplay_LoadMonPics(PokeathlonSnowThrowDisplay *display, PokeathlonCourseMonInfo *mons);
void PokeathlonSnowThrowDisplay_CreatePicSprites(void *pool, PokeathlonSnowThrowDisplay *display, PokeathlonCourseMonInfo *mons);
void PokeathlonSnowThrowDisplay_TransferMonPics(PokeathlonSnowThrowDisplay *display);
void PokeathlonSnowThrowDisplay_GetPicVramOffsets(void *pool, PokeathlonSnowThrowDisplay *display);
void PokeathlonSnowThrowDisplay_InitWindow(PokeathlonSnowThrowDisplay *display);
void PokeathlonSnowThrowDisplay_PrintName(PokeathlonSnowThrowDisplay *display);
void PokeathlonSnowThrow_ScreenToField(int screenX, int screenY, float *fieldX, float *fieldZ);
void PokeathlonSnowThrow_FieldToScreen(float fieldX, float fieldZ, int *screenX, int *screenY);

// pokeathlon_snow_throw_cpu.c
void PokeathlonSnowThrow_AddAimSpread(int degrees, VecFx32 *direction);
PokeathlonSnowThrowCpu *PokeathlonSnowThrowCpu_New(enum HeapID heapId, int cpuCount, int mode);
void PokeathlonSnowThrowCpu_SetMon(PokeathlonSnowThrowCpu *cpu, u8 index, PokeathlonSnowThrowMon *mon, PokeathlonSnowThrowBall *ball);
void PokeathlonSnowThrowCpu_Delete(PokeathlonSnowThrowCpu *cpu);
int PokeathlonSnowThrowCpu_Update(PokeathlonSnowThrowCpu *cpu, int elapsed);
void PokeathlonSnowThrowCpu_Think(PokeathlonSnowThrowCpu *cpu, u8 team, u8 mon, int elapsed);
u8 PokeathlonSnowThrowCpu_SortTargets(u32 ownTeam, PokeathlonSnowThrowCpuTarget **targets, PokeathlonSnowThrowCpuTarget **sorted);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_SNOW_THROW_H
