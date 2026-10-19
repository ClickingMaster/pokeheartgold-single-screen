#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_CIRCLE_PUSH_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_CIRCLE_PUSH_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_circle_push_display.h"
#include "pokeathlon/pokeathlon_course_objects.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "sys_task_api.h"

// Circle Push: the twelve Pokemon of the four teams push each other in a ring. Pokemon in the bonus zones score
// when the round's timer runs out; Pokemon pushed out of the ring come back at their start.

enum PokeathlonCirclePushMonState {
    CIRCLE_PUSH_MON_OUT,
    CIRCLE_PUSH_MON_IN_RING,
    CIRCLE_PUSH_MON_AT_START,
};

// One of the twelve Pokemon.
typedef struct PokeathlonCirclePushMon {
    PokeathlonCourseMonObject *object;
    PokeathlonCourseSpritePoolSprite *effectSprite; // shadow, and the effects of scoring
    PokeathlonCourseSpritePoolSprite *tiredSprite;  // shown while the Pokemon is exhausted
    fx32 x;                                         // drawn position
    fx32 y;
    u8 unk14[4];
    u32 state;      // 0 out of the ring, 1 in it, 2 back at the start
    VecFx32 target; // where it runs to
    VecFx32 position;
    VecFx32 velocity;
    u8 direction; // 1..4
    u8 pushing;
    u16 points;
    u8 exhausted;
    u8 exhaustTimer;
    u8 boost; // speed bonus right after a touch
    s8 boostTimer;
    u16 palette[16];
    u16 grayPalette[16]; // out of the ring
    u8 unk88;
    u8 runSpeed;   // from the Pokemon's power
    u16 pushSpeed; // from its speed
    u16 maxStamina;
    u16 stamina;
} PokeathlonCirclePushMon;

// Speeds and stamina by performance stat (NARC a/1/7/0 #1).
typedef struct PokeathlonCirclePushRingData {
    u32 pushSpeeds[5];
    u32 staminas[5];
    u32 runSpeeds[5];
} PokeathlonCirclePushRingData;

typedef struct PokeathlonCirclePushTouch {
    BOOL pressed; // first frame of a touch
    BOOL held;
    u8 unk8[4];
} PokeathlonCirclePushTouch;

// A bonus zone of a layout (sPokeathlonCirclePushLayoutZones[layout], up to 4).
typedef struct PokeathlonCirclePushZone {
    u32 kind; // 0: none; sPokeathlonCirclePushZoneKinds[kind - 1] = radius, points
    u16 x;
    u16 y;
} PokeathlonCirclePushZone;

typedef struct PokeathlonCirclePushZoneMarker {
    BOOL shown;
    const PokeathlonCirclePushZone *zone;
    u32 layout;
    PokeathlonCourseSpritePoolSprite *sprite;
} PokeathlonCirclePushZoneMarker;

typedef struct PokeathlonCirclePushZoneMarkers {
    PokeathlonCirclePushZoneMarker markers[8];
    u32 next;
} PokeathlonCirclePushZoneMarkers;

// The ring's effects: the change of the zone layout between rounds (slide, cross fade, shake)
// and the bounce of a Pokemon that scored.
typedef struct PokeathlonCirclePushEffects {
    u8 state;
    u8 fadeState;
    u8 slideState;
    u8 bounceActive;
    SysTask *changeTask;
    SysTask *slideTask;
    SysTask *fadeTask;
    SysTask *shakeTask;
    SysTask *bounceTask;
    u16 fadeTimer;
    u16 slideTimer;
    u16 oldLayout;
    u16 newLayout;
    PokeathlonCirclePushZoneMarkers *markers;
    s16 shakeOffset;
    u16 shakeStep;
    u32 bounceMon; // 12: none
    s16 bounceOffset;
    u16 bounceStep;
    PokeathlonCourseData *course;
} PokeathlonCirclePushEffects;

// What the host sends every frame.
typedef struct PokeathlonCirclePushPacket {
    u8 x[12];
    u8 y[12];
    s32 shapeBits; // 2 bits per Pokemon: direction - 1; bits 24..: timer
    s32 stateBits; // 2 bits per Pokemon: state; bits 24..27: zone layout, 28..31: scoring Pokemon
    s32 flagBits;  // bit per Pokemon: exhausted; 12: over; 13: round starts; 14: scoring; 15..17:
                   // effect; 18..25: points of a team; 26..27: that team; 28..31: round
} PokeathlonCirclePushPacket;

// What every player sends.
typedef struct PokeathlonCirclePushInput {
    BOOL touching;
    u8 x;
    u8 y;
    u8 unk06[2];
    u8 ready; // the effects of the zone change are over
    u8 unk09[0x1B];
} PokeathlonCirclePushInput;

// The CPU players.
typedef struct PokeathlonCirclePushAiMon {
    int cell; // of the 7x5 grid of the ring, -1 outside
    int speed;
    u16 *stamina;
    VecFx32 *position;
    u32 *state;
    int slot;        // team * 3 + member
    VecFx32 *target; // NULL for the players' Pokemon
    PokeathlonCourseMonObject *object;
} PokeathlonCirclePushAiMon;

typedef struct PokeathlonCirclePushAiCell {
    int x; // column, then pixel
    int y;
    int pixelX;
    int pixelY;
    BOOL inRing;
    int cellIndex;
    int score;
    int index;
} PokeathlonCirclePushAiCell;

typedef struct PokeathlonCirclePushAiZone {
    int radius;
    u16 x;
    u16 y;
} PokeathlonCirclePushAiZone;

typedef struct PokeathlonCirclePushAi {
    PokeathlonCirclePushAiMon mons[12];
    PokeathlonCirclePushAiCell cells[25];
    int zoneCells[4];
    BOOL moving;
    int skipped[12];
    u8 teamCount;
    u8 team;
    u8 timer;
    u8 interval;
    u8 member;
    u8 firstTeam;
    u8 firstTeam2;
} PokeathlonCirclePushAi;

typedef struct PokeathlonCirclePushStack {
    u8 mons[12];
    s32 count;
} PokeathlonCirclePushStack;

typedef struct PokeathlonCirclePush {
    BgConfig *bgConfig;
    Window scoreWindow;
    enum HeapID heapId;
    PokeathlonCourseSpritePool *spritePool;
    Sprite *markerSprite;
    PokeathlonCirclePushMon mons[4][3];
    int scores[4];
    PokeathlonCirclePushTouch touches[4];
    u8 heldMons[4]; // per player, 12: none
    u8 packetTeam;  // team whose score the next packet carries
    u8 timerTick;
    u8 timer; // 0..43
    u8 round;
    u8 roundDelay;
    u8 over;
    u8 changeLayout;
    u8 scoreZones;
    PokeathlonCirclePushZoneMarkers *zoneMarkers;
    u16 layout; // drawn
    u16 nextLayout;
    void *ringScreenFile;
    NNSG2dScreenData *ringScreen;
    u8 grayed[12]; // drawn with the gray palette
    u8 roundStart;
    u8 scoringDelay;
    u8 scoring;
    u8 scoringMon; // 12: none
    u8 scoringEffect;
    u8 scoringTimer;
    u8 scoringCount;
    u8 scoringIndex;
    u8 scoringMons[12];
    u8 scoringPoints[12];
    PokeathlonCourseSpriteResources *spriteResources;
    void *objects;
    PokeathlonCirclePushAi *ai;
    PokeathlonCirclePushDisplay *display;

    struct {
        u16 state;
        u16 timer;
    } falls[12]; // Pokemon falling out of the ring

    BOOL alarm;
    PokeathlonCirclePushEffects effects;
    u8 targetOutOfRing[12]; // the held Pokemon is dragged out
    BOOL waitingForReady;
    void *sortBuffer;
} PokeathlonCirclePush;

BOOL PokeathlonCirclePush_Init(PokeathlonCourseData *course);
BOOL PokeathlonCirclePush_Main(PokeathlonCourseData *course);
BOOL PokeathlonCirclePush_CalcResults(PokeathlonCourseData *course);
BOOL PokeathlonCirclePush_Exit(PokeathlonCourseData *course);
BOOL PokeathlonCirclePush_FadeIn(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonCirclePush_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonCirclePush_End(PokeathlonCourseData *course, u8 *state);

PokeathlonCirclePushZoneMarkers *PokeathlonCirclePushZoneMarkers_New(enum HeapID heapId);
void PokeathlonCirclePushZoneMarkers_Free(PokeathlonCirclePushZoneMarkers *markers);
void PokeathlonCirclePushZoneMarkers_CreateSprites(PokeathlonCirclePushZoneMarkers *markers, PokeathlonCourseSpritePool *pool);
u8 PokeathlonCirclePush_ChooseLayout(u8 layout, int kind);
int PokeathlonCirclePush_GetZonePoints(u8 layout, u16 x, u16 y, u8 *zone);
void PokeathlonCirclePushZoneMarkers_Show(PokeathlonCirclePushZoneMarkers *markers, u8 layout);
void PokeathlonCirclePushZoneMarkers_Hide(PokeathlonCirclePushZoneMarkers *markers, u8 layout);
int PokeathlonCirclePush_GetPointsEffect(int points);
PokeathlonCirclePushAi *PokeathlonCirclePushAi_New(enum HeapID heapId, int teamCount, int linkMode);
void PokeathlonCirclePushAi_SetMon(PokeathlonCirclePushAi *ai, u8 slot, u16 speed, u16 *stamina, VecFx32 *position, u32 *state, VecFx32 *target, PokeathlonCourseMonObject *object);
void PokeathlonCirclePushAi_Free(PokeathlonCirclePushAi *ai);
int PokeathlonCirclePushAi_Update(PokeathlonCirclePushAi *ai, u8 layout);
PokeathlonCirclePushDisplay *PokeathlonCirclePushDisplay_New(enum HeapID heapId, BgConfig *bgConfig, PokeathlonCourseData *course);
void PokeathlonCirclePushDisplay_Free(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_LoadScreens(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_LoadPics(PokeathlonCirclePushDisplay *display);
void PokeathlonCirclePushDisplay_LoadSpriteResources(PokeathlonCourseSpritePool *pool, PokeathlonCirclePushDisplay *display);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_CIRCLE_PUSH_H
