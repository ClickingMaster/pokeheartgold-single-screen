#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_LAMP_JUMP_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_LAMP_JUMP_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "camera.h"

// Lamp Jump: flicking one of the player's three Pokemon downward on the trampoline launches it upward, and the
// Pokemon earn points for the lamps they reach. The other participants' points are simulated from their stats.

// One of the player's three Pokemon.
typedef struct PokeathlonLampJumpMon {
    void *sprite;
    PokeathlonCourseSpritePoolSprite *effectSprite;
    fx32 drawX;
    fx32 drawY;
    u32 unk10;
    u32 unk14;
    fx32 velocityX;
    fx32 x;
    u16 score;
    u16 unk22;
    u16 combo;  // lamps hit in this jump, up to 5
    u8 stunned; // cannot be touched while set
    u8 stunTimer;
    u8 bounceReady;
    u8 colliding[3]; // touching each of the other Pokemon
    s32 bounce;      // scales the bounce off the trampoline, in twelfths
    s32 stunFrames;  // after a collision
    s32 flickScale;  // scales the sideways speed of a flick, in twelfths
} PokeathlonLampJumpMon;

// Vertical motion of one Pokemon.
typedef struct PokeathlonLampJumpJump {
    fx32 velocity;
    fx32 unk04;
    s32 height;
    u32 unk0C;
    BOOL airborne; // above the trampoline after a bounce
    BOOL jumping;
    fx32 launchVelocity;
} PokeathlonLampJumpJump;

// The trampoline: a height per column (2 pixels wide); below 0 = pressed down.
typedef struct PokeathlonLampJumpTrampoline {
    s32 surface[128];  // the lowest of the dents and lines below
    s32 dents[3][128]; // the dent under each Pokemon
    s32 lines[3][128]; // the cloth between two Pokemon: 0-1, 1-2, 2-0
    s32 monColumns[3]; // the column of each Pokemon
} PokeathlonLampJumpTrampoline;

typedef struct PokeathlonLampJump {
    BgConfig *bgConfig;
    Window windows[5];
    enum HeapID heapId;
    void *bg2ScreenFile;
    NNSG2dScreenData *bg2Screen;
    PokeathlonCourseSpritePool *scene;
    void *unk64;
    void *playerSprites[4];
    void *effectSprites[3];
    void *timerDigits[2];
    void *lamps;
    PokeathlonLampJumpMon mons[3];
    void *cpuPlayers;
    int timer;      // frames left
    u8 selectedMon; // 3 = none
    u8 holdFrames;
    u8 hitLamp;    // sent to the other players every frame
    u8 nextEffect; // next of effectPositions / effectPending
    VecFx32 effectPositions[3];
    u8 effectPending[3];
    VecFx32 touchStart;
    u8 unk178[0x10];
    void *spriteResources;
    void *monSpriteManager;
    Camera *camera;
    VecFx32 cameraTarget;

    union {
        struct {
            s32 trampolineRows[7][128]; // the trampoline as rows
            s32 monColumns[3];          // x / 2 of each Pokemon, in pixels
        };

        PokeathlonLampJumpTrampoline trampoline;
    };

    PokeathlonLampJumpJump jumps[3];
    NNSG2dCellTransferState *cellTransferState;
} PokeathlonLampJump;

// What PokeathlonCourse_GetMonInfo reports about one Pokemon of a team.
typedef struct PokeathlonLampJumpMonView {
    u32 data[4];
} PokeathlonLampJumpMonView;

typedef struct PokeathlonLampJumpSceneConfig {
    int capacity;
    int resourceCount;
    int resCounts;
} PokeathlonLampJumpSceneConfig;

typedef struct PokeathlonLampJumpSpriteConfig {
    int narcId;
    int mainSize;
    int subSize;
    int heapId;
} PokeathlonLampJumpSpriteConfig;

typedef struct PokeathlonLampJumpMonSpriteInfo {
    int settings[5];
    int sizeTypes[3]; // per Pokemon: PokeathlonCourse_GetSpriteSizeType of the Pokemon
    int unk20[9];
} PokeathlonLampJumpMonSpriteInfo;

// NARC a/1/7/0 member 6: parameters per performance level.
typedef struct PokeathlonLampJumpMonParams {
    u32 acceleration[5];
    u32 velocity[5];
    u32 scale[5];
} PokeathlonLampJumpMonParams;

// NARC a/1/7/0 member 16: sprite offset per Pokemon size class.
typedef struct PokeathlonLampJumpSpriteOffset {
    int x;
    int y;
} PokeathlonLampJumpSpriteOffset;

typedef struct PokeathlonLampJumpScreenPos {
    u16 x;
    u16 y;
} PokeathlonLampJumpScreenPos;

// A block of the link data exchanged every frame (PokeathlonCourse_GetDataCopyArea).
typedef union PokeathlonLampJumpSyncBlock {
    s32 words[9];
    u8 bytes[36];
} PokeathlonLampJumpSyncBlock;

BOOL PokeathlonLampJump_Init(PokeathlonCourseData *course);
BOOL PokeathlonLampJump_Draw(PokeathlonCourseData *course);
BOOL PokeathlonLampJump_ShowResults(PokeathlonCourseData *course);
BOOL PokeathlonLampJump_Exit(PokeathlonCourseData *course);
BOOL PokeathlonLampJump_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonLampJump_CalcScores(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonLampJump_CheckCollision(const VecFx32 *a, fx32 radiusA, const VecFx32 *b, fx32 radiusB);

// One lamp; touching a lit lamp puts it out and scores.
typedef struct PokeathlonLampJumpLamp {
    BOOL lit;
    VecFx32 pos;
    PokeathlonCourseSpritePoolSprite *sprite;
    PokeathlonCourseSpritePoolSprite *pointsSprite; // the points of the hit
    PokeathlonCourseSpritePoolSprite *hitSprite;
} PokeathlonLampJumpLamp;

typedef struct PokeathlonLampJumpLamps {
    PokeathlonLampJumpLamp lamps[29];
    int relightTimer;
} PokeathlonLampJumpLamps;

// The participants without a player: every 3 frames one of them may hit a lamp, more likely with more Jump
// and Skill.
typedef struct PokeathlonLampJumpCpuPlayers {
    BOOL active;
    u8 *dataCopy;       // PokeathlonCourse_GetDataCopyArea
    u8 skill[4];        // per participant, the sum of the Skill of its Pokemon
    u8 jump[4];         // per participant, the sum of the Jump of its Pokemon
    u8 skillUpTo[4][3]; // per participant, the Skill sums up to each Pokemon (picks the one that hits)
    u8 count;
    u8 current;
    u8 tick;
    u8 interval;
    u8 first; // first participant without a player
    u8 unk21; // the same (4: none)
    u8 unk22[2];
} PokeathlonLampJumpCpuPlayers;

void PokeathlonLampJump_Draw3D(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_InitCamera(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_CalcTrampolineSurface(PokeathlonLampJumpTrampoline *trampoline);
void PokeathlonLampJump_UpdateJumps(PokeathlonCourseData *course, PokeathlonLampJump *lampJump);
void PokeathlonLampJump_SetRestingHeights(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_LoadSpriteResources(PokeathlonCourseSpritePool *scene);
void PokeathlonLampJump_CreateSprites(PokeathlonLampJump *lampJump, PokeathlonCourseSpritePool *scene);
void PokeathlonLampJump_PlaceSprites(PokeathlonLampJump *lampJump, u8 self);
void PokeathlonLampJump_UpdateScoreDisplay(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_UpdateTimerDisplay(PokeathlonLampJump *lampJump, u16 frames);
void PokeathlonLampJump_PrintPlayerNames(PokeathlonCourseData *course, PokeathlonLampJump *lampJump);
void PokeathlonLampJump_InitWindows(PokeathlonLampJump *lampJump);
void PokeathlonLampJump_PrintTotal(PokeathlonLampJump *lampJump, u16 score);
void PokeathlonLampJump_SetMonParams(PokeathlonCourseData *course, PokeathlonLampJumpMonParams *params, u8 member, PokeathlonLampJumpMon *mon);
BOOL PokeathlonLampJump_CollideMons(PokeathlonCourseData *course, PokeathlonLampJump *lampJump);
PokeathlonLampJumpLamps *PokeathlonLampJump_NewLamps(enum HeapID heapId);
void PokeathlonLampJump_FreeLamps(PokeathlonLampJumpLamps *lamps);
void PokeathlonLampJump_UpdateLamps(PokeathlonLampJumpLamps *lamps);
PokeathlonLampJumpCpuPlayers *PokeathlonLampJump_NewCpuPlayers(enum HeapID heapId, int count, int unused, u8 *dataCopy, PokeathlonCourseData *course);
void PokeathlonLampJump_FreeCpuPlayers(PokeathlonLampJumpCpuPlayers *cpu);
int PokeathlonLampJump_UpdateCpuPlayers(PokeathlonCourseData *course, PokeathlonLampJumpCpuPlayers *cpu);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_LAMP_JUMP_H
