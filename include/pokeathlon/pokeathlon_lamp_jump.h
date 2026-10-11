#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_LAMP_JUMP_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_LAMP_JUMP_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "camera.h"

// Lamp Jump (Pokeathlon event 5): the player's three Pokemon stand in a row on the touch screen.
// Touching a Pokemon and flicking downward launches it upward (the flick also gives it a sideways
// speed); the Pokemon earn points for the lamps they reach. The match lasts 1800 frames; the
// points of the other participants are simulated from their Pokemon's stats.

// One of the player's three Pokemon.
typedef struct PokeathlonLampJumpMon {
    void *sprite;       // 0x00
    void *effectSprite; // 0x04
    fx32 drawX;         // 0x08
    fx32 drawY;         // 0x0C
    u32 unk10;          // 0x10
    u32 unk14;          // 0x14
    fx32 velocityX;     // 0x18
    fx32 x;             // 0x1C
    u16 score;          // 0x20
    u16 unk22;          // 0x22
    u16 combo;          // 0x24: lamps hit in this jump, up to 5
    u8 unk26;           // 0x26: cannot be touched while set
    u8 stunTimer;       // 0x27
    u8 unk28;           // 0x28
    u8 colliding[3];    // 0x29: touching each of the other Pokemon
    s32 bounce;         // 0x2C: scales the bounce off the trampoline, in twelfths
    s32 stunFrames;     // 0x30: after a collision
    s32 flickScale;     // 0x34: scales the sideways speed of a flick, in twelfths
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
    BgConfig *bgConfig;            // 0x0000
    Window windows[5];             // 0x0004
    enum HeapID heapId;            // 0x0054
    void *bg2ScreenFile;           // 0x0058
    NNSG2dScreenData *bg2Screen;   // 0x005C
    void *scene;                   // 0x0060
    void *unk64;                   // 0x0064
    void *playerSprites[4];        // 0x0068
    void *effectSprites[3];        // 0x0078
    void *timerDigits[2];          // 0x0084
    void *unk8C;                   // 0x008C
    PokeathlonLampJumpMon mons[3]; // 0x0090
    void *unk138;                  // 0x0138
    int timer;                     // 0x013C: frames left
    u8 selectedMon;                // 0x0140: 3 = none
    u8 holdFrames;                 // 0x0141
    u8 unk142;                     // 0x0142: sent to the other players every frame
    u8 nextEffect;                 // 0x0143: next of effectPositions / effectPending
    VecFx32 effectPositions[3];    // 0x0144
    u8 effectPending[3];           // 0x0168
    VecFx32 touchStart;            // 0x016C
    u8 unk178[0x10];               // 0x0178
    void *unk188;                  // 0x0188
    void *monSpriteManager;        // 0x018C
    Camera *camera;                // 0x0190
    VecFx32 cameraTarget;          // 0x0194

    union {
        struct {
            s32 unk1A0[7][128]; // 0x01A0: the trampoline as rows (pokeathlon_lamp_jump.c)
            s32 monColumns[3];  // 0x0FA0: x / 2 of each Pokemon, in pixels
        };

        PokeathlonLampJumpTrampoline trampoline; // 0x01A0
    };

    PokeathlonLampJumpJump jumps[3]; // 0x0FAC
    void *cellTransferState;         // 0x1000
} PokeathlonLampJump;                // size: 0x1004

// What PokeathlonCourse_GetMonInfo reports about one Pokemon of a team.
typedef struct PokeathlonLampJumpMonView {
    u32 data[4];
} PokeathlonLampJumpMonView;

typedef struct PokeathlonLampJumpSceneConfig {
    int unk00;
    int unk04;
    int unk08;
} PokeathlonLampJumpSceneConfig;

typedef struct PokeathlonLampJumpSpriteConfig {
    int narcId;
    int mainSize;
    int subSize;
    int heapId;
} PokeathlonLampJumpSpriteConfig;

typedef struct PokeathlonLampJumpMonSpriteInfo {
    int unk00[5];
    int unk14[3]; // per Pokemon: ov96_021E6108 of the Pokemon
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
BOOL ov96_021F54D4(PokeathlonCourseData *course);
BOOL PokeathlonLampJump_Exit(PokeathlonCourseData *course);
void PokeathlonLampJump_SetGfxBanks(void);
BOOL PokeathlonLampJump_Play(PokeathlonCourseData *course, u8 *state);
BOOL PokeathlonLampJump_CalcScores(PokeathlonCourseData *course, u8 *state);
void PokeathlonLampJump_InitBgs(BgConfig *bgConfig);
void PokeathlonLampJump_LoadBgGfx(PokeathlonLampJump *lampJump);
BOOL PokeathlonLampJump_Update(PokeathlonCourseData *course);
void PokeathlonLampJump_UpdateDisplay(PokeathlonCourseData *course);
void PokeathlonLampJump_HandleTouch(PokeathlonCourseData *course);
void PokeathlonLampJump_UpdateMonSprites(PokeathlonCourseData *course);
void PokeathlonLampJump_UpdatePlayerSprites(PokeathlonCourseData *course);
void PokeathlonLampJump_GetFlick(int touchX, int touchY, const VecFx32 *start, VecFx32 *flick);
u8 PokeathlonLampJump_FindTouchedMon(PokeathlonLampJump *lampJump, u16 touchX, u16 touchY);
BOOL PokeathlonLampJump_CheckCollision(const VecFx32 *a, fx32 radiusA, const VecFx32 *b, fx32 radiusB);
void PokeathlonLampJump_LaunchMon(PokeathlonLampJump *lampJump, s32 mon, VecFx32 *flick);
void PokeathlonLampJump_Init3D(PokeathlonLampJump *lampJump);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_LAMP_JUMP_H
