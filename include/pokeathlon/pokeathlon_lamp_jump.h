#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_LAMP_JUMP_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_LAMP_JUMP_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "camera.h"

// Lamp Jump (Pokeathlon event 5): the player's three Pokemon stand in a row on the touch screen.
// Touching a Pokemon and flicking downward launches it upward (the flick also gives it a sideways
// speed); the Pokemon earn points for the lamps they reach. The match lasts 1800 frames; the
// points of the other participants are simulated from their Pokemon's stats.

// One of the player's three Pokemon.
typedef struct PokeathlonLampJumpMon {
    void *sprite;                                   // 0x00
    PokeathlonCourseSpritePoolSprite *effectSprite; // 0x04
    fx32 drawX;                                     // 0x08
    fx32 drawY;                                     // 0x0C
    u32 unk10;                                      // 0x10
    u32 unk14;                                      // 0x14
    fx32 velocityX;                                 // 0x18
    fx32 x;                                         // 0x1C
    u16 score;                                      // 0x20
    u16 unk22;                                      // 0x22
    u16 combo;                                      // 0x24: lamps hit in this jump, up to 5
    u8 stunned;                                     // 0x26: cannot be touched while set
    u8 stunTimer;                                   // 0x27
    u8 bounceReady;                                 // 0x28
    u8 colliding[3];                                // 0x29: touching each of the other Pokemon
    s32 bounce;                                     // 0x2C: scales the bounce off the trampoline, in twelfths
    s32 stunFrames;                                 // 0x30: after a collision
    s32 flickScale;                                 // 0x34: scales the sideways speed of a flick, in twelfths
} PokeathlonLampJumpMon;

// Vertical motion of one Pokemon.
typedef struct PokeathlonLampJumpJump {
    fx32 velocity;       // 0x00
    fx32 unk04;          // 0x04
    s32 height;          // 0x08
    u32 unk0C;           // 0x0C
    BOOL airborne;       // 0x10: above the trampoline after a bounce
    BOOL jumping;        // 0x14
    fx32 launchVelocity; // 0x18
} PokeathlonLampJumpJump;

// The trampoline: a height per column (2 pixels wide); below 0 = pressed down.
typedef struct PokeathlonLampJumpTrampoline {
    s32 surface[128];  // the lowest of the dents and lines below
    s32 dents[3][128]; // the dent under each Pokemon
    s32 lines[3][128]; // the cloth between two Pokemon: 0-1, 1-2, 2-0
    s32 monColumns[3]; // the column of each Pokemon
} PokeathlonLampJumpTrampoline;

typedef struct PokeathlonLampJump {
    BgConfig *bgConfig;                // 0x0000
    Window windows[5];                 // 0x0004
    enum HeapID heapId;                // 0x0054
    void *bg2ScreenFile;               // 0x0058
    NNSG2dScreenData *bg2Screen;       // 0x005C
    PokeathlonCourseSpritePool *scene; // 0x0060
    void *unk64;                       // 0x0064
    void *playerSprites[4];            // 0x0068
    void *effectSprites[3];            // 0x0078
    void *timerDigits[2];              // 0x0084
    void *lamps;                       // 0x008C
    PokeathlonLampJumpMon mons[3];     // 0x0090
    void *cpuPlayers;                  // 0x0138
    int timer;                         // 0x013C: frames left
    u8 selectedMon;                    // 0x0140: 3 = none
    u8 holdFrames;                     // 0x0141
    u8 hitLamp;                        // 0x0142: sent to the other players every frame
    u8 nextEffect;                     // 0x0143: next of effectPositions / effectPending
    VecFx32 effectPositions[3];        // 0x0144
    u8 effectPending[3];               // 0x0168
    VecFx32 touchStart;                // 0x016C
    u8 unk178[0x10];                   // 0x0178
    void *spriteResources;             // 0x0188
    void *monSpriteManager;            // 0x018C
    Camera *camera;                    // 0x0190
    VecFx32 cameraTarget;              // 0x0194

    union {
        struct {
            s32 trampolineRows[7][128]; // 0x01A0: the trampoline as rows
            s32 monColumns[3];          // 0x0FA0: x / 2 of each Pokemon, in pixels
        };

        PokeathlonLampJumpTrampoline trampoline; // 0x01A0
    };

    PokeathlonLampJumpJump jumps[3];            // 0x0FAC
    NNSG2dCellTransferState *cellTransferState; // 0x1000
} PokeathlonLampJump;                           // size: 0x1004

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

// A block of the link data exchanged every frame (PokeathlonCourse_GetDataCopyArea: 0x28 bytes each).
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

// The trampoline the Pokemon bounce on (drawn in 3D as a band of
// changing colors), the lamps (29, up to 25 lit at a time, relit every 240 frames), the bouncing and the
// collisions of the three Pokemon, the score and timer displays and the participants without a player.

// One lamp. It is lit while `lit`; touching a lit lamp puts it out and scores.
typedef struct PokeathlonLampJumpLamp {
    BOOL lit;                                       // 0x00
    VecFx32 pos;                                    // 0x04
    PokeathlonCourseSpritePoolSprite *sprite;       // 0x10
    PokeathlonCourseSpritePoolSprite *pointsSprite; // 0x14: the points of the hit
    PokeathlonCourseSpritePoolSprite *hitSprite;    // 0x18
} PokeathlonLampJumpLamp;                           // size: 0x1C

typedef struct PokeathlonLampJumpLamps {
    PokeathlonLampJumpLamp lamps[29]; // 0x000
    int relightTimer;                 // 0x32C
} PokeathlonLampJumpLamps;            // size: 0x330

// The participants without a player: every 3 frames one of them may hit a lamp, more likely with more Jump
// and Skill.
typedef struct PokeathlonLampJumpCpuPlayers {
    BOOL active;                // 0x00
    u8 *dataCopy;               // 0x04: PokeathlonCourse_GetDataCopyArea
    u8 skill[4];                // 0x08: per participant, the sum of the Skill of its Pokemon
    u8 jump[4];                 // 0x0C: per participant, the sum of the Jump of its Pokemon
    u8 skillUpTo[4][3];         // 0x10: per participant, the Skill sums up to each Pokemon (picks the one that hits)
    u8 count;                   // 0x1C
    u8 current;                 // 0x1D
    u8 tick;                    // 0x1E
    u8 interval;                // 0x1F
    u8 first;                   // 0x20: first participant without a player
    u8 unk21;                   // 0x21: the same (4: none)
    u8 unk22[2];                // 0x22
} PokeathlonLampJumpCpuPlayers; // size: 0x24

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
