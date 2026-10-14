#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_RACE_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_RACE_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_hurdle_dash.h"

#include "sprite.h"
#include "sprite_resource_header.h"
#include "sprite_resource.h"

// The race of Hurdle Dash (Pokeathlon event 0): running, jumping and tripping over the hurdles, the
// Pokemon of the participants without a player, the ranking and the standings on the touch screen.

// A hurdle on a lane (0x10): a standing sprite and a knocked-over one.
typedef struct PokeathlonHurdle {
    BOOL visible;         // 0x0
    void *standingSprite; // 0x4: PokeathlonCourseSpritePoolSprite
    void *knockedSprite;  // 0x8: PokeathlonCourseSpritePoolSprite
    BOOL standing;        // 0xC
} PokeathlonHurdle;

// The hurdles of a lane: four sprites placed again and again further down the track.
typedef struct PokeathlonHurdleDashLane {
    PokeathlonHurdle *hurdles;
    int next;
} PokeathlonHurdleDashLane;

// The track (0xC24): where the hurdles are, one flag per 8 pixels of each lane. The host loads one of
// three layouts (NARC a/2/2/6) into layoutToSend, the course link copies it into layout for everyone.
typedef struct PokeathlonHurdleDashTrack {
    PokeathlonHurdleDashLane lanes[3]; // 0x000
    u8 layoutToSend[3][512];           // 0x018
    u8 layout[3][512];                 // 0x618
    int lastHurdle[3];                 // 0xC18: last flag of each lane a hurdle was placed for
} PokeathlonHurdleDashTrack;

// The dust a running Pokemon kicks up: five sprites per lane.
typedef struct PokeathlonHurdleDashDust {
    BOOL active;
    void *sprite; // PokeathlonCourseSpritePoolSprite
} PokeathlonHurdleDashDust;

typedef struct PokeathlonHurdleDashDustLane {
    PokeathlonHurdleDashDust dust[5];
    u32 next;
} PokeathlonHurdleDashDustLane;

// One of the 12 Pokemon of the standings bar on the touch screen.
typedef struct PokeathlonHurdleDashStandingsMon {
    Sprite *icon;  // 0x0
    Sprite *frame; // 0x4: the colour of the Pokemon's team
    Sprite *rank;  // 0x8
    u8 unkC[4];    // 0xC
} PokeathlonHurdleDashStandingsMon;

// The standings bar (0x244): the 12 Pokemon icons in the order of the race.
typedef struct PokeathlonHurdleDashStandings {
    enum HeapID heapId;                        // 0x000
    SpriteList *spriteList;                    // 0x004
    G2dRenderer renderer;                      // 0x008
    GF_2DGfxResMan *resMans[6];                // 0x130
    SpriteResource *resources[4];              // 0x148
    u8 unk158[8];                              // 0x158
    SpriteResourcesHeader header;              // 0x160
    PokeathlonHurdleDashStandingsMon mons[12]; // 0x184
} PokeathlonHurdleDashStandings;

// The Pokemon of the standings bar (species and form).
typedef struct PokeathlonHurdleDashStandingsMonId {
    u16 species;
    u16 form;
} PokeathlonHurdleDashStandingsMonId;

// A participant's link packet (block 0 of the data copy area is the own one, blocks 2 to 5 those of the
// four participants).
typedef struct PokeathlonHurdleDashPacket {
    u16 distances[3];   // 0x00
    u8 unk06;           // 0x06
    u8 unk07;           // 0x07
    u8 unk08;           // 0x08
    u8 done;            // 0x09: the participant's race is over
    u16 finishTimes[3]; // 0x0A
    u16 finished[3];    // 0x10
    u8 unk16[0xE];      // 0x16
} PokeathlonHurdleDashPacket;

// The host's packet (block 1 sent, block 6 received).
typedef struct PokeathlonHurdleDashStatus {
    u32 allDone;         // 0x00
    u16 distances[4][3]; // 0x04
    u8 ranks[6];         // 0x1C: a nibble per Pokemon, the low one first
} PokeathlonHurdleDashStatus;

// The ranking of the 12 Pokemon (a sorted list).
typedef struct PokeathlonHurdleDashRankNode {
    struct PokeathlonHurdleDashRankNode *prev;
    struct PokeathlonHurdleDashRankNode *next;
    u16 distance;   // 0x08
    u16 index;      // 0x0A
    u16 finishTime; // 0x0C
    u16 finished;   // 0x0E
    u32 tied;       // 0x10: same rank as the node before it
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

void *PokeathlonHurdleDashRunner_GetHurdle(PokeathlonHurdleDashRunner *runner, int index);
void PokeathlonHurdleDash_UpdateRunner(PokeathlonCourseData *course, PokeathlonHurdleDash *hurdleDash, PokeathlonHurdleDashRunner *runner, PokeathlonHurdleDashTouch *touch);
BOOL PokeathlonHurdleDash_UpdateRunners(PokeathlonCourseData *course, PokeathlonHurdleDash *hurdleDash);
void PokeathlonHurdleDashMon_StartJump(PokeathlonCourseData *course, u32 unused, u32 lane, PokeathlonHurdleDashMon *mon);
void PokeathlonHurdleDashMon_Update(int lane, PokeathlonHurdleDashMon *mon, f32 *speed);
BOOL PokeathlonHurdleDash_IsTouchingHurdle(s32 y, s32 radius, const PokeathlonHurdleDashRect *rect);
BOOL PokeathlonHurdleDash_IsJumpTimed(s32 y, const PokeathlonHurdleDashRect *rect);
void PokeathlonHurdleDashRunner_DrawFinishLine(BgConfig *bgConfig, PokeathlonHurdleDashRunner *runner);
void PokeathlonHurdleDash_SyncRace(PokeathlonCourseData *course);
void PokeathlonHurdleDash_UpdateStandings(PokeathlonHurdleDash *hurdleDash, PokeathlonHurdleDashStatus *status);
void PokeathlonHurdleDash_UpdateRanks(PokeathlonCourseData *course);
void PokeathlonHurdleDash_CalcRanks(PokeathlonCourseData *course);
BOOL PokeathlonHurdleDashRankNode_InsertFinished(PokeathlonHurdleDashRankNode *node, PokeathlonHurdleDashRankNode *at);
BOOL PokeathlonHurdleDashRankNode_InsertRunning(PokeathlonHurdleDashRankNode *node, PokeathlonHurdleDashRankNode *at);
void PokeathlonHurdleDash_SetRank(PokeathlonHurdleDashStatus *status, int index, u32 rank);
BOOL PokeathlonHurdleDash_HaveAllStopped(PokeathlonHurdleDash *hurdleDash);
void PokeathlonHurdleDash_UpdateCpuParticipants(PokeathlonCourseData *course);
void PokeathlonHurdleDash_RevealHurdles(PokeathlonHurdleDash *hurdleDash, int distance, int lane);
void PokeathlonHurdleDashRunner_UpdateDust(PokeathlonHurdleDash *hurdleDash, PokeathlonHurdleDashRunner *runner);
u8 PokeathlonHurdleDash_GetTouchedLane(PokeathlonHurdleDash *hurdleDash, s32 x, s32 y);
void PokeathlonHurdleDash_InitTimeWindow(PokeathlonHurdleDash *hurdleDash);
void PokeathlonHurdleDash_PrintTime(PokeathlonHurdleDash *hurdleDash, s32 frames);
void PokeathlonHurdleDashMon_ScaleForHeight(PokeathlonHurdleDashMon *mon, s32 height);
void PokeathlonHurdleDashMon_ScaleForHop(PokeathlonHurdleDashMon *mon, BOOL landing);
void PokeathlonHurdleDash_UpdateTimerDigits(PokeathlonHurdleDash *hurdleDash);
void PokeathlonHurdleDash_UpdateCpuRunner(PokeathlonCourseData *course, PokeathlonHurdleDash *hurdleDash, PokeathlonHurdleDashPacket *packet, u8 participant, int member, u8 cpuIndex);
void PokeathlonHurdleDash_InitCpuRunners(PokeathlonCourseData *course, const PokeathlonHurdleDashTable *table);
void PokeathlonHurdleDashRunner_Boost(PokeathlonCourseData *course, PokeathlonHurdleDashRunner *runner);
void PokeathlonHurdleDash_CountFrame(PokeathlonHurdleDash *hurdleDash);
s32 PokeathlonHurdleDash_RoundTime(s32 frames);
PokeathlonHurdle *PokeathlonHurdleDash_AllocHurdles(u32 count, enum HeapID heapId);
void PokeathlonHurdleDash_FreeHurdles(PokeathlonHurdle *hurdles);
PokeathlonHurdle *PokeathlonHurdleDash_GetHurdle(PokeathlonHurdle *hurdles, int index);
void PokeathlonHurdle_SetSprites(PokeathlonHurdle *hurdle, void *standingSprite, void *knockedSprite);
void PokeathlonHurdle_SetActive(PokeathlonHurdle *hurdle, BOOL active);
BOOL PokeathlonHurdle_IsVisible(PokeathlonHurdle *hurdle);
BOOL PokeathlonHurdle_IsStanding(PokeathlonHurdle *hurdle);
void PokeathlonHurdle_KnockDown(PokeathlonHurdle *hurdle);
void PokeathlonHurdle_SetPosition(PokeathlonHurdle *hurdle, VecFx32 *position);
void PokeathlonHurdle_Scroll(PokeathlonHurdle *hurdle, int distance);
void PokeathlonHurdle_GetRect(PokeathlonHurdle *hurdle, PokeathlonHurdleDashRect *rect);
PokeathlonHurdle *PokeathlonHurdleDashTrack_PlaceHurdle(PokeathlonHurdleDashTrack *track, int lane, int y);
PokeathlonHurdleDashTrack *PokeathlonHurdleDashTrack_New(enum HeapID heapId);
void PokeathlonHurdleDashTrack_Free(PokeathlonHurdleDashTrack *track);
void PokeathlonHurdleDashTrack_CreateHurdleSprites(PokeathlonHurdleDashLane *lane, void *spritePool);
PokeathlonHurdle *PokeathlonHurdleDashTrack_GetLaneHurdles(PokeathlonHurdleDashTrack *track, u32 lane);
void *PokeathlonHurdleDashTrack_GetLayoutToSend(PokeathlonHurdleDashTrack *track);
void *PokeathlonHurdleDashTrack_GetLayout(PokeathlonHurdleDashTrack *track);
int PokeathlonHurdleDashTrack_GetLayoutSize(PokeathlonHurdleDashTrack *track);
void PokeathlonHurdleDashTrack_RevealHurdles(PokeathlonHurdleDashTrack *track, int position, int lane);
void PokeathlonHurdleDashTrack_LoadRandomLayout(PokeathlonHurdleDashTrack *track);
int PokeathlonHurdleDashTrack_GetHurdleAt(PokeathlonHurdleDashTrack *track, int lane, int position);
PokeathlonHurdleDashStandings *PokeathlonHurdleDashStandings_New(enum HeapID heapId);
void PokeathlonHurdleDashStandings_Free(PokeathlonHurdleDashStandings *standings);
void PokeathlonHurdleDashStandings_Init(PokeathlonHurdleDashStandings *standings, PokeathlonHurdleDashStandingsMonId *mons);
void PokeathlonHurdleDashStandings_SetMonX(PokeathlonHurdleDashStandings *standings, int index, int x);
void PokeathlonHurdleDashStandings_SetRank(PokeathlonHurdleDashStandings *standings, u32 index, u32 rank);
void PokeathlonHurdleDashStandings_SetDrawFlag(PokeathlonHurdleDashStandings *standings, BOOL draw);
void PokeathlonHurdleDashStandings_SetMonDrawFlag(PokeathlonHurdleDashStandings *standings, int index, BOOL draw);
void PokeathlonHurdleDashStandings_Draw(PokeathlonHurdleDashStandings *standings);
void PokeathlonHurdleDashStandings_LoadResources(PokeathlonHurdleDashStandings *standings);
void PokeathlonHurdleDashStandings_InitResourcesHeader(PokeathlonHurdleDashStandings *standings);
void PokeathlonHurdleDashStandings_CreateSprites(PokeathlonHurdleDashStandings *standings, PokeathlonHurdleDashStandingsMonId *mons);
void PokeathlonHurdleDashStandings_InitSpriteTemplate(SpriteTemplate *template, PokeathlonHurdleDashStandings *standings, const SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen);
PokeathlonHurdleDashDustLane *PokeathlonHurdleDashDust_New(enum HeapID heapId);
void PokeathlonHurdleDashDust_CreateSprites(PokeathlonHurdleDashDustLane *lanes, void *spritePool, u16 priority);
void PokeathlonHurdleDashDust_Free(PokeathlonHurdleDashDustLane *lanes);
void PokeathlonHurdleDashDust_Spawn(PokeathlonHurdleDashDustLane *lanes, u32 lane, const VecFx32 *position);
void PokeathlonHurdleDashDust_Scroll(PokeathlonHurdleDashDustLane *lanes, u32 lane, u32 distance);
void PokeathlonHurdleDashDust_Remove(PokeathlonHurdleDashDust *dust);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_RACE_H
