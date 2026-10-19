#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "sprite.h"

// Hurdle Dash: the three Pokemon of each team race over a track of hurdles; the team's result is the sum of its
// Pokemon's times.

enum PokeathlonHurdleDashMonState {
    HURDLE_DASH_MON_RUNNING,
    HURDLE_DASH_MON_JUMPING,
    HURDLE_DASH_MON_TRIPPED,
    HURDLE_DASH_MON_CENTERING,
    HURDLE_DASH_MON_STOPPED,
};

enum PokeathlonHurdleDashJumpPhase {
    HURDLE_DASH_JUMP_NONE,
    HURDLE_DASH_JUMP_RISE,
    HURDLE_DASH_JUMP_HANG,
    HURDLE_DASH_JUMP_FALL,
    HURDLE_DASH_JUMP_TRIPPED,
};

// A Pokemon on its lane.
typedef struct PokeathlonHurdleDashMon {
    void *monObject; // PokeathlonCourseMonObject
    u32 state;       // 0 running, 1 jumping, 2 tripped, 3 back to the middle after the finish, 4 stopped
    u32 phase;       // of a jump: 1 up, 2 in the air, 3 down; 4 after tripping
    u8 timer;
    u8 spinTimer; // tripped: turns every 4 frames
    u8 direction; // tripped: the animation it faces
    u8 unk0F;
    s32 x; // sprite position when the jump started
    s32 y;
    s32 groundY;
    s32 markerY;
    PokeathlonCourseSpritePoolSprite *jumpMarker; // shows whether a jump was timed
    PokeathlonCourseSpritePoolSprite *rankSprite;
    PokeathlonCourseSpritePoolSprite *boostEffect;
    f32 topSpeed;
    f32 acceleration;
    u8 jumpStat; // 0 hops instead of jumping
    u8 unk35[3];
    u16 riseFrames; // of a jump, up and down each
    u16 hangFrames;
    u16 jumpQueued; // touched again while coming down
    u16 queuedJumps;
} PokeathlonHurdleDashMon;

// One of the player's three Pokemon and its lane.
typedef struct PokeathlonHurdleDashRunner {
    u16 distance; // 0x1000 = the finish line; sent to the other players every frame
    u16 time;     // frames to the finish line
    u16 subScrollY;
    u16 mainScrollY;
    u8 finished;
    u8 boostLevel; // timed jumps in a row, up to 4
    u8 unk0A[2];
    f32 speed;         // pixels per frame
    f32 speedFraction; // carried to the next frame
    u16 mainBg;        // the BGs of the lane (finish line)
    u16 subBg;
    u8 lane;
    u8 unk19;
    u8 unk1A;
    u8 finishLineY;
    s32 finishLineState;
    void *hurdles; // PokeathlonHurdle[4] of the lane
    PokeathlonHurdleDashMon mon;
    f32 rankScale;
    s16 rank;
    u16 dustTimer;
} PokeathlonHurdleDashRunner;

// A Pokemon of a participant without a player: its race is simulated.
typedef struct PokeathlonHurdleDashCpuRunner {
    u8 state; // 2 tripped
    u8 stunTimer;
    u8 boostLevel;
    u8 boostChance; // percent of the hurdles cleared with a boost
    u8 clearChance; // percent (with boostChance) not tripping over a hurdle
    u8 unk05[3];
    f32 speed;
    f32 speedFraction;
    f32 topSpeed;
    f32 acceleration;
    BOOL finished;
    int time;     // frame of the finish
    int distance; // out of 0x1000
    int lastHurdle;
} PokeathlonHurdleDashCpuRunner;

typedef struct PokeathlonHurdleDash {
    enum HeapID heapId;
    BgConfig *bgConfig;
    Window window;
    u8 unk018[0xC0];
    void *track; // PokeathlonHurdleDashTrack
    void *dust;  // PokeathlonHurdleDashDustLane[3]
    PokeathlonHurdleDashRunner runners[3];
    void *scene;
    void *standings; // PokeathlonHurdleDashStandings
    u32 raceTime;    // frames until the own Pokemon have finished
    u32 frame;
    u32 endTimer;
    PokeathlonHurdleDashCpuRunner cpuRunners[9]; // 3 per participant without a player
    PokeathlonCourseSpriteResources *spriteResources;
    void *monSpriteManager;
    PokeathlonCourseSpritePoolSprite *timerLabel;
    Sprite *sprites[6];
    u16 rankTimer;
    u8 ranksShown;
    u8 started;
} PokeathlonHurdleDash;

// A Pokemon of a team in the course data (PokeathlonCourse_GetParticipantMons).
typedef struct PokeathlonHurdleDashTeamMon {
    u16 species;
    u16 form;
    u8 unk04[0x24];
} PokeathlonHurdleDashTeamMon;

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

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_HURDLE_DASH_H
