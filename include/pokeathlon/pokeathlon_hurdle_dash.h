#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "sprite.h"

// Hurdle Dash (Pokeathlon event 0): the three Pokemon of each team race over a track of hurdles; the team's
// result is the sum of its Pokemon's times. Event template ov96_0221A90C = {Init, Draw, ShowResults, Exit},
// states ov96_0221DC30 = {Start, Play, CalcScores}. The race itself continues in the next part of the overlay.

// One of the player's three Pokemon.
typedef struct PokeathlonHurdleDashRunner {
    u16 unk00;                // 0x00: sent to the other players every frame
    u16 time;                 // 0x02: frames to the finish line
    u8 unk04[4];              // 0x04
    u8 finished;              // 0x08
    u8 unk09[99];             // 0x09
} PokeathlonHurdleDashRunner; // size: 0x6C

// A Pokemon of a participant without a player: its race is simulated.
typedef struct PokeathlonHurdleDashCpuRunner {
    u8 unk00[8];                 // 0x00
    f32 speed;                   // 0x08
    f32 unk0C;                   // 0x0C
    f32 minSpeed;                // 0x10
    f32 unk14;                   // 0x14
    BOOL finished;               // 0x18
    int time;                    // 0x1C: frame of the finish
    int distance;                // 0x20: out of 0x1000
    int unk24;                   // 0x24
} PokeathlonHurdleDashCpuRunner; // size: 0x28

typedef struct PokeathlonHurdleDash {
    enum HeapID heapId;                          // 0x000
    BgConfig *bgConfig;                          // 0x004
    Window window;                               // 0x008
    u8 unk018[0xC0];                             // 0x018
    void *unkD8;                                 // 0x0D8
    void *unkDC;                                 // 0x0DC
    PokeathlonHurdleDashRunner runners[3];       // 0x0E0
    void *scene;                                 // 0x224
    void *unk228;                                // 0x228: built from the species / form of all 12 Pokemon
    u32 unk22C;                                  // 0x22C
    u32 frame;                                   // 0x230
    u32 endTimer;                                // 0x234
    PokeathlonHurdleDashCpuRunner cpuRunners[9]; // 0x238: 3 per participant without a player
    void *unk3A0;                                // 0x3A0
    void *monSpriteManager;                      // 0x3A4
    void *unk3A8;                                // 0x3A8
    Sprite *sprites[6];                          // 0x3AC
    u16 unk3C4;                                  // 0x3C4
    u8 unk3C6;                                   // 0x3C6
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

// A Pokemon of a team in the course data (PokeathlonCourse_GetParticipantUnk04).
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
