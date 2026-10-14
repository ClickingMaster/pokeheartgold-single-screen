#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_CIRCLE_PUSH_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_CIRCLE_PUSH_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_circle_push_display.h"
#include "pokeathlon/pokeathlon_course_objects.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "sys_task_api.h"

// Circle Push (Pokeathlon event 2; template gPokeathlonCirclePushEvent = {Init, Main, CalcResults, Exit},
// states gPokeathlonCirclePushStates = {FadeIn, Play, End}). The twelve Pokemon of the four teams push each
// other in a ring; dragging a Pokemon makes it run after the stylus. Pokemon in the bonus zones
// score when the round's timer runs out (six rounds); Pokemon pushed out of the ring come back
// at their start. The host moves every Pokemon (the players send their touches) and sends the
// result every frame; every player draws from that packet. The top screen is
// PokeathlonCirclePushDisplay.

// One of the twelve Pokemon (0x90).
typedef struct PokeathlonCirclePushMon {
    PokeathlonCourseObject *object;                 // 0x00
    PokeathlonCourseSpritePoolSprite *effectSprite; // 0x04: shadow, and the effects of scoring
    PokeathlonCourseSpritePoolSprite *tiredSprite;  // 0x08: shown while the Pokemon is exhausted
    fx32 x;                                         // 0x0C: drawn position
    fx32 y;                                         // 0x10
    u8 unk14[4];                                    // 0x14
    u32 state;                                      // 0x18: 0 out of the ring, 1 in it, 2 back at the start
    VecFx32 target;                                 // 0x1C: where it runs to
    VecFx32 position;                               // 0x28
    VecFx32 velocity;                               // 0x34
    u8 direction;                                   // 0x40: 1..4
    u8 pushing;                                     // 0x41
    u16 points;                                     // 0x42
    u8 exhausted;                                   // 0x44
    u8 exhaustTimer;                                // 0x45
    u8 boost;                                       // 0x46: speed bonus right after a touch
    s8 boostTimer;                                  // 0x47
    u16 palette[16];                                // 0x48
    u16 grayPalette[16];                            // 0x68: out of the ring
    u8 unk88;                                       // 0x88
    u8 runSpeed;                                    // 0x89: from the Pokemon's power
    u16 pushSpeed;                                  // 0x8A: from its speed
    u16 maxStamina;                                 // 0x8C
    u16 stamina;                                    // 0x8E
} PokeathlonCirclePushMon;

// Speeds and stamina by performance stat (NARC a/1/7/0 #1, 60 bytes).
typedef struct PokeathlonCirclePushRingData {
    u32 pushSpeeds[5];
    u32 staminas[5];
    u32 runSpeeds[5];
} PokeathlonCirclePushRingData;

typedef struct PokeathlonCirclePushTouch {
    BOOL pressed; // 0x0: first frame of a touch
    BOOL held;    // 0x4
    u8 unk8[4];   // 0x8
} PokeathlonCirclePushTouch;

// A bonus zone of a layout (gPokeathlonCirclePushLayoutZones[layout], up to 4).
typedef struct PokeathlonCirclePushZone {
    u32 kind; // 0: none; gPokeathlonCirclePushZoneKinds[kind - 1] = radius, points
    u16 x;
    u16 y;
} PokeathlonCirclePushZone;

typedef struct PokeathlonCirclePushZoneMarker {
    BOOL shown;                               // 0x0
    const PokeathlonCirclePushZone *zone;     // 0x4
    u32 layout;                               // 0x8
    PokeathlonCourseSpritePoolSprite *sprite; // 0xC
} PokeathlonCirclePushZoneMarker;

typedef struct PokeathlonCirclePushZoneMarkers {
    PokeathlonCirclePushZoneMarker markers[8]; // 0x00
    u32 next;                                  // 0x80
} PokeathlonCirclePushZoneMarkers;             // size: 0x84

// The ring's effects: the change of the zone layout between rounds (slide, cross fade, shake)
// and the bounce of a Pokemon that scored.
typedef struct PokeathlonCirclePushEffects {
    u8 state;                                 // 0x00
    u8 fadeState;                             // 0x01
    u8 slideState;                            // 0x02
    u8 bounceActive;                          // 0x03
    SysTask *changeTask;                      // 0x04
    SysTask *slideTask;                       // 0x08
    SysTask *fadeTask;                        // 0x0C
    SysTask *shakeTask;                       // 0x10
    SysTask *bounceTask;                      // 0x14
    u16 fadeTimer;                            // 0x18
    u16 slideTimer;                           // 0x1A
    u16 oldLayout;                            // 0x1C
    u16 newLayout;                            // 0x1E
    PokeathlonCirclePushZoneMarkers *markers; // 0x20
    s16 shakeOffset;                          // 0x24
    u16 shakeStep;                            // 0x26
    u32 bounceMon;                            // 0x28: 12: none
    s16 bounceOffset;                         // 0x2C
    u16 bounceStep;                           // 0x2E
    PokeathlonCourseData *course;             // 0x30
} PokeathlonCirclePushEffects;

// What the host sends every frame (at course data copy area + 0x28, received at + 0xF0).
typedef struct PokeathlonCirclePushPacket {
    u8 x[12];      // 0x00
    u8 y[12];      // 0x0C
    s32 shapeBits; // 0x18: 2 bits per Pokemon: direction - 1; bits 24..: timer
    s32 stateBits; // 0x1C: 2 bits per Pokemon: state; bits 24..27: zone layout, 28..31: scoring Pokemon
    s32 flagBits;  // 0x20: bit per Pokemon: exhausted; 12: over; 13: round starts; 14: scoring; 15..17:
                   // effect; 18..25: points of a team; 26..27: that team; 28..31: round
} PokeathlonCirclePushPacket;

// What every player sends (at course data copy area + 0x00, the host keeps them at + 0x50 +
// 0x28 * participant).
typedef struct PokeathlonCirclePushInput {
    BOOL touching; // 0x00
    u8 x;          // 0x04
    u8 y;          // 0x05
    u8 unk06[2];   // 0x06
    u8 ready;      // 0x08: the effects of the zone change are over
    u8 unk09[0x1B];
} PokeathlonCirclePushInput; // size: 0x24

// The CPU players (0x4EC).
typedef struct PokeathlonCirclePushAiMon {
    int cell;                       // 0x00: of the 7x5 grid of the ring, -1 outside
    int speed;                      // 0x04
    u16 *stamina;                   // 0x08
    VecFx32 *position;              // 0x0C
    u32 *state;                     // 0x10
    int slot;                       // 0x14: team * 3 + member
    VecFx32 *target;                // 0x18: NULL for the players' Pokemon
    PokeathlonCourseObject *object; // 0x1C
} PokeathlonCirclePushAiMon;

typedef struct PokeathlonCirclePushAiCell {
    int x;         // 0x00: column, then pixel
    int y;         // 0x04
    int pixelX;    // 0x08
    int pixelY;    // 0x0C
    void *inRing;  // 0x10
    int cellIndex; // 0x14
    int score;     // 0x18
    int index;     // 0x1C
} PokeathlonCirclePushAiCell;

typedef struct PokeathlonCirclePushAiZone {
    int radius; // 0
    u16 x;
    u16 y;
} PokeathlonCirclePushAiZone;

typedef struct PokeathlonCirclePushAi {
    PokeathlonCirclePushAiMon mons[12];   // 0x000
    PokeathlonCirclePushAiCell cells[25]; // 0x180
    int zoneCells[4];                     // 0x4A0
    BOOL moving;                          // 0x4B0
    int skipped[12];                      // 0x4B4
    u8 teamCount;                         // 0x4E4
    u8 team;                              // 0x4E5
    u8 timer;                             // 0x4E6
    u8 interval;                          // 0x4E7
    u8 member;                            // 0x4E8
    u8 firstTeam;                         // 0x4E9
    u8 firstTeam2;                        // 0x4EA
} PokeathlonCirclePushAi;                 // size: 0x4EC

typedef struct PokeathlonCirclePushStack {
    u8 mons[12];
    s32 count;
} PokeathlonCirclePushStack;

typedef struct PokeathlonCirclePush {
    BgConfig *bgConfig;                           // 0x000
    Window scoreWindow;                           // 0x004
    enum HeapID heapId;                           // 0x014
    PokeathlonCourseSpritePool *spritePool;       // 0x018
    Sprite *markerSprite;                         // 0x01C
    PokeathlonCirclePushMon mons[4][3];           // 0x020
    int scores[4];                                // 0x6E0
    PokeathlonCirclePushTouch touches[4];         // 0x6F0
    u8 heldMons[4];                               // 0x720: per player, 12: none
    u8 packetTeam;                                // 0x724: team whose score the next packet carries
    u8 timerTick;                                 // 0x725
    u8 timer;                                     // 0x726: 0..43
    u8 round;                                     // 0x727
    u8 roundDelay;                                // 0x728
    u8 over;                                      // 0x729
    u8 changeLayout;                              // 0x72A
    u8 scoreZones;                                // 0x72B
    PokeathlonCirclePushZoneMarkers *zoneMarkers; // 0x72C
    u16 layout;                                   // 0x730: drawn
    u16 nextLayout;                               // 0x732
    void *ringScreenFile;                         // 0x734
    NNSG2dScreenData *ringScreen;                 // 0x738
    u8 grayed[12];                                // 0x73C: drawn with the gray palette
    u8 roundStart;                                // 0x748
    u8 scoringDelay;                              // 0x749
    u8 scoring;                                   // 0x74A
    u8 scoringMon;                                // 0x74B: 12: none
    u8 scoringEffect;                             // 0x74C
    u8 scoringTimer;                              // 0x74D
    u8 scoringCount;                              // 0x74E
    u8 scoringIndex;                              // 0x74F
    u8 scoringMons[12];                           // 0x750
    u8 scoringPoints[12];                         // 0x75C
    PokeathlonCourseSpriteResources *spriteResources;                        // 0x768
    void *objects;                                // 0x76C
    PokeathlonCirclePushAi *ai;                   // 0x770
    PokeathlonCirclePushDisplay *display;         // 0x774

    struct {
        u16 state;
        u16 timer;
    } falls[12]; // 0x778: Pokemon falling out of the ring

    BOOL alarm;                          // 0x7A8
    PokeathlonCirclePushEffects effects; // 0x7AC
    u8 targetOutOfRing[12];              // 0x7E0: the held Pokemon is dragged out
    BOOL waitingForReady;                // 0x7EC
    void *sortBuffer;                    // 0x7F0
} PokeathlonCirclePush;                  // size: 0x7F4

BOOL PokeathlonCirclePush_Init(PokeathlonCourseData *course);
BOOL PokeathlonCirclePush_Main(PokeathlonCourseData *course);
BOOL PokeathlonCirclePush_CalcResults(PokeathlonCourseData *course);
BOOL PokeathlonCirclePush_Exit(PokeathlonCourseData *course);
void PokeathlonCirclePush_SetBanks(void);
BOOL PokeathlonCirclePush_FadeIn(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonCirclePush_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonCirclePush_End(PokeathlonCourseData *course, u8 *state);
void PokeathlonCirclePush_InitBgs(BgConfig *bgConfig);
void PokeathlonCirclePush_LoadBgGfx(PokeathlonCirclePush *circlePush);
void PokeathlonCirclePush_MakePacket(PokeathlonCirclePush *circlePush, PokeathlonCirclePushPacket *packet);
BOOL PokeathlonCirclePush_Update(PokeathlonCourseData *course);
void PokeathlonCirclePush_RunHost(PokeathlonCourseData *course);
void PokeathlonCirclePush_UpdateScreen(PokeathlonCourseData *course);
void PokeathlonCirclePush_MoveMons(PokeathlonCourseData *course);
void PokeathlonCirclePush_SetTarget(int x, int y, const VecFx32 *position, fx32 range, VecFx32 *target);
u8 PokeathlonCirclePush_FindTouchedMon(PokeathlonCirclePush *circlePush, u8 team, u16 x, u16 y);
BOOL PokeathlonCirclePush_MonsCollide(PokeathlonCirclePushMon *a, PokeathlonCirclePushMon *b);
void PokeathlonCirclePush_UpdateContacts(PokeathlonCirclePush *circlePush);
BOOL PokeathlonCirclePush_CanMove(PokeathlonCirclePush *circlePush, PokeathlonCirclePushMon *mon, u32 index, const VecFx32 *delta);
void PokeathlonCirclePush_PushMons(PokeathlonCirclePush *circlePush);
u8 PokeathlonCirclePushStack_Push(PokeathlonCirclePushStack *stack, u8 mon);
u8 PokeathlonCirclePushStack_Pop(PokeathlonCirclePushStack *stack);
void PokeathlonCirclePush_Project(VecFx32 *a, VecFx32 *b, VecFx32 *out);
fx32 PokeathlonCirclePush_Cosine(const VecFx32 *a, const VecFx32 *b);
void PokeathlonCirclePush_ScoreZones(PokeathlonCirclePush *circlePush);
void PokeathlonCirclePush_DrawTimer(int time, BgConfig *bgConfig);
void PokeathlonCirclePush_DrawTimerTile(u32 index, BgConfig *bgConfig);
void PokeathlonCirclePush_RespawnMons(PokeathlonCirclePush *circlePush);
BOOL PokeathlonCirclePush_IsTeamOut(PokeathlonCirclePush *circlePush, int team);
void PokeathlonCirclePush_RecoverStamina(PokeathlonCirclePushMon *mon);
void PokeathlonCirclePush_SetMonStats(PokeathlonCourseData *course, PokeathlonCirclePushRingData *ringData, u8 team, u8 member, PokeathlonCirclePushMon *mon);
void PokeathlonCirclePush_StartLayoutChange(PokeathlonCirclePushEffects *effects, u8 oldLayout, u8 newLayout, PokeathlonCourseData *course);
void PokeathlonCirclePush_LayoutChangeTask(SysTask *task, PokeathlonCirclePushEffects *effects);
void PokeathlonCirclePush_LayoutFadeTask(SysTask *task, PokeathlonCirclePushEffects *effects);
void PokeathlonCirclePush_RingSlideTask(SysTask *task, PokeathlonCirclePushEffects *effects);
void PokeathlonCirclePush_ShakeTask(SysTask *task, void *data);
void PokeathlonCirclePush_StopEffects(PokeathlonCirclePushEffects *effects);
void PokeathlonCirclePush_StartBounce(PokeathlonCirclePushEffects *effects, u8 mon);
void PokeathlonCirclePush_BounceTask(SysTask *task, void *data);
void PokeathlonCirclePush_ApplyBounce(PokeathlonCourseData *course, PokeathlonCirclePushEffects *effects, void *object, u8 mon, int anim);
void PokeathlonCirclePush_AddScoreWindow(PokeathlonCirclePush *circlePush);
void PokeathlonCirclePush_PrintScore(PokeathlonCirclePush *circlePush, s32 score);
BOOL PokeathlonCirclePush_IsOutOfRing(PokeathlonCourseObject *object, fx32 x, fx32 y);
int PokeathlonCirclePush_CompareDepth(const void *a, const void *b);
void PokeathlonCirclePush_SortByDepth(PokeathlonCirclePush *circlePush, PokeathlonCirclePushPacket *packet);
PokeathlonCirclePushZoneMarkers *PokeathlonCirclePushZoneMarkers_New(enum HeapID heapId);
void PokeathlonCirclePushZoneMarkers_Free(PokeathlonCirclePushZoneMarkers *markers);
void PokeathlonCirclePushZoneMarkers_CreateSprites(PokeathlonCirclePushZoneMarkers *markers, PokeathlonCourseSpritePool *pool);
u8 PokeathlonCirclePush_ChooseLayout(u8 layout, int kind);
int PokeathlonCirclePush_GetZonePoints(u8 layout, u16 x, u16 y, u8 *zone);
void PokeathlonCirclePushZoneMarkers_Show(PokeathlonCirclePushZoneMarkers *markers, u8 layout);
void PokeathlonCirclePushZoneMarkers_Hide(PokeathlonCirclePushZoneMarkers *markers, u8 layout);
int PokeathlonCirclePush_GetPointsEffect(int points);
BOOL PokeathlonCirclePush_IsInCircle(int x, int y, int centerX, int centerY, u16 radius);
u8 PokeathlonCirclePush_GetZoneCircles(int layout, PokeathlonCirclePushAiZone *circles);
PokeathlonCirclePushAi *PokeathlonCirclePushAi_New(enum HeapID heapId, int teamCount, int linkMode);
void PokeathlonCirclePushAi_SetMon(PokeathlonCirclePushAi *ai, u8 slot, u16 speed, u16 *stamina, VecFx32 *position, u32 *state, VecFx32 *target, PokeathlonCourseObject *object);
void PokeathlonCirclePushAi_Free(PokeathlonCirclePushAi *ai);
int PokeathlonCirclePushAi_Update(PokeathlonCirclePushAi *ai, u8 layout);
void PokeathlonCirclePushAi_ChooseTarget(PokeathlonCirclePushAi *ai, u8 index, u8 layout);
void PokeathlonCirclePushAi_PlaceInCells(u8 count, PokeathlonCirclePushAiCell **cells);
void PokeathlonCirclePushAi_ScoreCells(PokeathlonCirclePushAi *ai, u8 count, PokeathlonCirclePushAiZone *zones, PokeathlonCirclePushAiMon *mon, PokeathlonCirclePushAiCell **cells);
int PokeathlonCirclePushAi_GetCell(int x, int y);
int PokeathlonCirclePushAi_ScoreCell(PokeathlonCirclePushAi *ai, PokeathlonCirclePushAiZone *zones, int cell, PokeathlonCirclePushAiMon *mon, int x, int y, BOOL *inZone);
int PokeathlonCirclePushAi_CountRivals(const PokeathlonCirclePushAiMon *mons, const PokeathlonCirclePushAiMon *mon, int cell);
BOOL PokeathlonCirclePushAi_IsInZone(PokeathlonCirclePushAiZone *zones, fx32 x, fx32 y);
BOOL PokeathlonCirclePushAi_IsZoneCell(PokeathlonCirclePushAi *ai, u8 cell);
void PokeathlonCirclePushAi_SnapToZone(PokeathlonCirclePushAi *ai, PokeathlonCirclePushAiZone *zones, s32 cell, s32 *x, s32 *y);
BOOL PokeathlonCirclePushAi_GetColumnRow(s32 x, s32 y, s16 *column, s16 *row);
u8 PokeathlonCirclePushAi_SortCells(u32 count, PokeathlonCirclePushAiCell **cells, PokeathlonCirclePushAiCell **sorted);
PokeathlonCirclePushDisplay *PokeathlonCirclePushDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
void PokeathlonCirclePushDisplay_Free(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_LoadScreens(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_LoadPics(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_LoadSpriteResources(PokeathlonCourseSpritePool *pool, PokeathlonCirclePushDisplay *display);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_CIRCLE_PUSH_H
