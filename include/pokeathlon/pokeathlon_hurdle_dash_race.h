#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_RACE_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_RACE_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_types_def.h"
#include "pokeathlon/pokeathlon_hurdle_dash.h"

#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"

// The race of Hurdle Dash: running, jumping and tripping over the hurdles, the Pokemon of the participants without
// a player, the ranking and the standings on the touch screen.

// A hurdle on a lane: a standing sprite and a knocked-over one.
typedef struct PokeathlonHurdle {
    BOOL visible;
    PokeathlonCourseSpritePoolSprite *standingSprite;
    PokeathlonCourseSpritePoolSprite *knockedSprite;
    BOOL standing;
} PokeathlonHurdle;

// The hurdles of a lane: four sprites placed again and again further down the track.
typedef struct PokeathlonHurdleDashLane {
    PokeathlonHurdle *hurdles;
    int next;
} PokeathlonHurdleDashLane;

// The track: where the hurdles are, one flag per 8 pixels of each lane. The host loads one of
// three layouts (NARC a/2/2/6) into layoutToSend, the course link copies it into layout for everyone.
typedef struct PokeathlonHurdleDashTrack {
    PokeathlonHurdleDashLane lanes[3];
    u8 layoutToSend[3][512];
    u8 layout[3][512];
    int lastHurdle[3]; // last flag of each lane a hurdle was placed for
} PokeathlonHurdleDashTrack;

// The dust a running Pokemon kicks up: five sprites per lane.
typedef struct PokeathlonHurdleDashDust {
    BOOL active;
    PokeathlonCourseSpritePoolSprite *sprite;
} PokeathlonHurdleDashDust;

typedef struct PokeathlonHurdleDashDustLane {
    PokeathlonHurdleDashDust dust[5];
    u32 next;
} PokeathlonHurdleDashDustLane;

// One of the 12 Pokemon of the standings bar on the touch screen.
typedef struct PokeathlonHurdleDashStandingsMon {
    Sprite *icon;
    Sprite *frame; // the colour of the Pokemon's team
    Sprite *rank;
    u8 unkC[4];
} PokeathlonHurdleDashStandingsMon;

// The standings bar: the 12 Pokemon icons in the order of the race.
typedef struct PokeathlonHurdleDashStandings {
    enum HeapID heapId;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMans[6];
    SpriteResource *resources[4];
    u8 unk158[8];
    SpriteResourcesHeader header;
    PokeathlonHurdleDashStandingsMon mons[12];
} PokeathlonHurdleDashStandings;

// The Pokemon of the standings bar (species and form).
typedef struct PokeathlonHurdleDashStandingsMonId {
    u16 species;
    u16 form;
} PokeathlonHurdleDashStandingsMonId;

// A participant's link packet (block 0 of the data copy area is the own one, blocks 2 to 5 those of the
// four participants).
typedef struct PokeathlonHurdleDashPacket {
    u16 distances[3];
    u8 unk06;
    u8 unk07;
    u8 unk08;
    u8 done; // the participant's race is over
    u16 finishTimes[3];
    u16 finished[3];
    u8 unk16[0xE];
} PokeathlonHurdleDashPacket;

// The host's packet (block 1 sent, block 6 received).
typedef struct PokeathlonHurdleDashStatus {
    u32 allDone;
    u16 distances[4][3];
    u8 ranks[6]; // a nibble per Pokemon, the low one first
} PokeathlonHurdleDashStatus;

// The ranking of the 12 Pokemon (a sorted list).
typedef struct PokeathlonHurdleDashRankNode {
    struct PokeathlonHurdleDashRankNode *prev;
    struct PokeathlonHurdleDashRankNode *next;
    u16 distance;
    u16 index;
    u16 finishTime;
    u16 finished;
    u32 tied; // same rank as the node before it
} PokeathlonHurdleDashRankNode;

// The ranking of the 12 Pokemon by their team rank (the standings bar).
typedef struct PokeathlonHurdleDashStandingsNode {
    struct PokeathlonHurdleDashStandingsNode *prev;
    struct PokeathlonHurdleDashStandingsNode *next;
    u16 rank;
    u16 index;
} PokeathlonHurdleDashStandingsNode;

typedef struct PokeathlonHurdleDashTouch {
    s32 lane;
    u32 unk4;
} PokeathlonHurdleDashTouch;

typedef struct PokeathlonHurdleDashRect {
    s16 x;
    s16 y;
    s16 width;
    s16 height;
} PokeathlonHurdleDashRect;

typedef struct PokeathlonHurdleDashLaneX {
    s32 x[3];
} PokeathlonHurdleDashLaneX;

BOOL PokeathlonHurdleDash_UpdateRunners(PokeathlonCourseData *course, PokeathlonHurdleDash *hurdleDash);
void PokeathlonHurdleDash_SyncRace(PokeathlonCourseData *course);
BOOL PokeathlonHurdleDash_HaveAllStopped(PokeathlonHurdleDash *hurdleDash);
void PokeathlonHurdleDash_UpdateCpuParticipants(PokeathlonCourseData *course);
void PokeathlonHurdleDash_InitTimeWindow(PokeathlonHurdleDash *hurdleDash);
void PokeathlonHurdleDash_PrintTime(PokeathlonHurdleDash *hurdleDash, s32 frames);
void PokeathlonHurdleDash_InitCpuRunners(PokeathlonCourseData *course, const PokeathlonHurdleDashTable *table);
void PokeathlonHurdleDash_CountFrame(PokeathlonHurdleDash *hurdleDash);
s32 PokeathlonHurdleDash_RoundTime(s32 frames);
PokeathlonHurdleDashTrack *PokeathlonHurdleDashTrack_New(enum HeapID heapId);
void PokeathlonHurdleDashTrack_Free(PokeathlonHurdleDashTrack *track);
void PokeathlonHurdleDashTrack_CreateHurdleSprites(PokeathlonHurdleDashLane *lane, PokeathlonCourseSpritePool *spritePool);
PokeathlonHurdle *PokeathlonHurdleDashTrack_GetLaneHurdles(PokeathlonHurdleDashTrack *track, u32 lane);
void *PokeathlonHurdleDashTrack_GetLayoutToSend(PokeathlonHurdleDashTrack *track);
void *PokeathlonHurdleDashTrack_GetLayout(PokeathlonHurdleDashTrack *track);
int PokeathlonHurdleDashTrack_GetLayoutSize(PokeathlonHurdleDashTrack *track);
void PokeathlonHurdleDashTrack_LoadRandomLayout(PokeathlonHurdleDashTrack *track);
PokeathlonHurdleDashStandings *PokeathlonHurdleDashStandings_New(enum HeapID heapId);
void PokeathlonHurdleDashStandings_Free(PokeathlonHurdleDashStandings *standings);
void PokeathlonHurdleDashStandings_Init(PokeathlonHurdleDashStandings *standings, PokeathlonHurdleDashStandingsMonId *mons);
void PokeathlonHurdleDashStandings_SetDrawFlag(PokeathlonHurdleDashStandings *standings, BOOL draw);
void PokeathlonHurdleDashStandings_Draw(PokeathlonHurdleDashStandings *standings);
PokeathlonHurdleDashDustLane *PokeathlonHurdleDashDust_New(enum HeapID heapId);
void PokeathlonHurdleDashDust_CreateSprites(PokeathlonHurdleDashDustLane *lanes, PokeathlonCourseSpritePool *spritePool, u16 priority);
void PokeathlonHurdleDashDust_Free(PokeathlonHurdleDashDustLane *lanes);
PokeathlonHurdle *PokeathlonHurdleDash_GetHurdle(PokeathlonHurdle *hurdles, int index);
BOOL PokeathlonHurdle_IsStanding(PokeathlonHurdle *hurdle);
void PokeathlonHurdle_KnockDown(PokeathlonHurdle *hurdle);
void PokeathlonHurdle_Scroll(PokeathlonHurdle *hurdle, int distance);
void PokeathlonHurdle_GetRect(PokeathlonHurdle *hurdle, PokeathlonHurdleDashRect *rect);
void PokeathlonHurdleDashTrack_RevealHurdles(PokeathlonHurdleDashTrack *track, int position, int lane);
int PokeathlonHurdleDashTrack_GetHurdleAt(PokeathlonHurdleDashTrack *track, int lane, int position);
void PokeathlonHurdleDashStandings_SetMonX(PokeathlonHurdleDashStandings *standings, int index, int x);
void PokeathlonHurdleDashStandings_SetRank(PokeathlonHurdleDashStandings *standings, u32 index, u32 rank);
void PokeathlonHurdleDashStandings_SetMonDrawFlag(PokeathlonHurdleDashStandings *standings, int index, BOOL draw);
void PokeathlonHurdleDashDust_Spawn(PokeathlonHurdleDashDustLane *lanes, u32 lane, VecFx32 *position);
void PokeathlonHurdleDashDust_Scroll(PokeathlonHurdleDashDustLane *lanes, u32 lane, u32 distance);
void PokeathlonHurdleDashRunner_Init(PokeathlonCourseData *, const PokeathlonHurdleDashTable *, const PokeathlonHurdleDashParams *, PokeathlonHurdleDashTrack *, PokeathlonCourseSpritePool *, PokeathlonCourseMonObjects *, u8, PokeathlonHurdleDashRunner *);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_RACE_H
