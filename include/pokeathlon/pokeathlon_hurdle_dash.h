#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "sprite.h"

// Hurdle Dash (Pokeathlon event 0): the three Pokemon of each team race over a track of hurdles; the team's
// result is the sum of its Pokemon's times. Event template gPokeathlonHurdleDashEvent = {Init, Draw, ShowResults, Exit},
// states gPokeathlonHurdleDashStates = {Start, Play, CalcScores}. The race: pokeathlon_hurdle_dash_race.c.

// A Pokemon on its lane (0x40, at 0x24 of its runner).
typedef struct PokeathlonHurdleDashMon {
    void *monObject;   // 0x00: PokeathlonCourseMonObject
    u32 state;         // 0x04: 0 running, 1 jumping, 2 tripped, 3 back to the middle after the finish, 4 stopped
    u32 phase;         // 0x08: of a jump: 1 up, 2 in the air, 3 down; 4 after tripping
    u8 timer;          // 0x0C
    u8 spinTimer;      // 0x0D: tripped: turns every 4 frames
    u8 direction;      // 0x0E: tripped: the animation it faces
    u8 unk0F;          // 0x0F
    s32 x;             // 0x10: sprite position when the jump started
    s32 y;             // 0x14
    s32 groundY;       // 0x18
    s32 markerY;       // 0x1C
    PokeathlonCourseSpritePoolSprite *jumpMarker;  // 0x20: PokeathlonCourseSpritePoolSprite: shows whether a jump was timed
    PokeathlonCourseSpritePoolSprite *rankSprite;  // 0x24: PokeathlonCourseSpritePoolSprite
    PokeathlonCourseSpritePoolSprite *boostEffect; // 0x28: PokeathlonCourseSpritePoolSprite
    f32 topSpeed;      // 0x2C
    f32 acceleration;  // 0x30
    u8 jumpStat;       // 0x34: 0 hops instead of jumping
    u8 unk35[3];       // 0x35
    u16 riseFrames;    // 0x38: of a jump, up and down each
    u16 hangFrames;    // 0x3A
    u16 jumpQueued;    // 0x3C: touched again while coming down
    u16 queuedJumps;   // 0x3E
} PokeathlonHurdleDashMon;

// One of the player's three Pokemon and its lane.
typedef struct PokeathlonHurdleDashRunner {
    u16 distance;                // 0x00: 0x1000 = the finish line; sent to the other players every frame
    u16 time;                    // 0x02: frames to the finish line
    u16 subScrollY;              // 0x04
    u16 mainScrollY;             // 0x06
    u8 finished;                 // 0x08
    u8 boostLevel;               // 0x09: timed jumps in a row, up to 4
    u8 unk0A[2];                 // 0x0A
    f32 speed;                   // 0x0C: pixels per frame
    f32 speedFraction;           // 0x10: carried to the next frame
    u16 mainBg;                  // 0x14: the BGs of the lane (finish line)
    u16 subBg;                   // 0x16
    u8 lane;                     // 0x18
    u8 unk19;                    // 0x19
    u8 unk1A;                    // 0x1A
    u8 finishLineY;              // 0x1B
    s32 finishLineState;         // 0x1C
    void *hurdles;               // 0x20: PokeathlonHurdle[4] of the lane
    PokeathlonHurdleDashMon mon; // 0x24
    f32 rankScale;               // 0x64
    s16 rank;                    // 0x68
    u16 dustTimer;               // 0x6A
} PokeathlonHurdleDashRunner;    // size: 0x6C

// A Pokemon of a participant without a player: its race is simulated.
typedef struct PokeathlonHurdleDashCpuRunner {
    u8 state;                    // 0x00: 2 tripped
    u8 stunTimer;                // 0x01
    u8 boostLevel;               // 0x02
    u8 boostChance;              // 0x03: percent of the hurdles cleared with a boost
    u8 clearChance;              // 0x04: percent (with boostChance) not tripping over a hurdle
    u8 unk05[3];                 // 0x05
    f32 speed;                   // 0x08
    f32 speedFraction;           // 0x0C
    f32 topSpeed;                // 0x10
    f32 acceleration;            // 0x14
    BOOL finished;               // 0x18
    int time;                    // 0x1C: frame of the finish
    int distance;                // 0x20: out of 0x1000
    int lastHurdle;              // 0x24
} PokeathlonHurdleDashCpuRunner; // size: 0x28

typedef struct PokeathlonHurdleDash {
    enum HeapID heapId;                          // 0x000
    BgConfig *bgConfig;                          // 0x004
    Window window;                               // 0x008
    u8 unk018[0xC0];                             // 0x018
    void *track;                                 // 0x0D8: PokeathlonHurdleDashTrack
    void *dust;                                  // 0x0DC: PokeathlonHurdleDashDustLane[3]
    PokeathlonHurdleDashRunner runners[3];       // 0x0E0
    void *scene;                                 // 0x224
    void *standings;                             // 0x228: PokeathlonHurdleDashStandings
    u32 raceTime;                                // 0x22C: frames until the own Pokemon have finished
    u32 frame;                                   // 0x230
    u32 endTimer;                                // 0x234
    PokeathlonHurdleDashCpuRunner cpuRunners[9]; // 0x238: 3 per participant without a player
    PokeathlonCourseSpriteResources *spriteResources;                                // 0x3A0
    void *monSpriteManager;                      // 0x3A4
    PokeathlonCourseSpritePoolSprite *unk3A8;                                // 0x3A8
    Sprite *sprites[6];                          // 0x3AC
    u16 rankTimer;                               // 0x3C4
    u8 ranksShown;                               // 0x3C6
    u8 started;                                  // 0x3C7
} PokeathlonHurdleDash;                          // size: 0x3C8

// What PokeathlonCourse_GetMonInfo reports about one Pokemon of a team (for the Pokemon sprite loader).
typedef struct PokeathlonHurdleDashMonView {
    u16 species;     // 0x00
    u16 form;        // 0x02
    u16 isLarge;     // 0x04
    u8 isShiny;      // 0x06
    u8 gender;       // 0x07
    u32 participant; // 0x08
    u32 personality; // 0x0C
} PokeathlonHurdleDashMonView;

// A Pokemon of a team in the course data (PokeathlonCourse_GetParticipantMons).
typedef struct PokeathlonHurdleDashTeamMon {
    u16 species;
    u16 form;
    u8 unk04[0x24];
} PokeathlonHurdleDashTeamMon;

typedef struct PokeathlonHurdleDashSceneConfig {
    s32 unk00;
    s32 unk04;
    u8 unk08[4];
} PokeathlonHurdleDashSceneConfig;

// NARC a/1/7/0 member 10.
typedef struct PokeathlonHurdleDashParams {
    u32 values[3][2];
} PokeathlonHurdleDashParams;

// NARC a/1/7/0 member 0.
typedef struct PokeathlonHurdleDashTable {
    s32 rows[4][5];
} PokeathlonHurdleDashTable;

typedef struct PokeathlonHurdleDashNumberX {
    u32 x[3];
} PokeathlonHurdleDashNumberX;

BOOL PokeathlonHurdleDash_Start(PokeathlonCourseData *course);
BOOL PokeathlonHurdleDash_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonHurdleDash_CalcScores(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonHurdleDash_Init(PokeathlonCourseData *course);
BOOL PokeathlonHurdleDash_Draw(PokeathlonCourseData *course);
BOOL PokeathlonHurdleDash_ShowResults(PokeathlonCourseData *course);
BOOL PokeathlonHurdleDash_Exit(PokeathlonCourseData *course);
void PokeathlonHurdleDash_SetGfxBanks(void);
void PokeathlonHurdleDash_InitBgs(BgConfig *bgConfig);
void PokeathlonHurdleDash_LoadBgTiles(BgConfig *bgConfig, enum HeapID heapId);
void PokeathlonHurdleDash_LoadBgScreens(BgConfig *bgConfig, enum HeapID heapId);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_H
