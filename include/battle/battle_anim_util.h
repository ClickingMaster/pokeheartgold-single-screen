#ifndef POKEHEARTGOLD_BATTLE_BATTLE_ANIM_UTIL_H
#define POKEHEARTGOLD_BATTLE_BATTLE_ANIM_UTIL_H

// Overlay 7 (battle move animations): motion helpers, HBlank scanline buffers, palette blends.

#include "global.h"

#include "battle/battle_anim_battler_util.h"

#include "palette.h"
#include "pokepic.h"
#include "scanline_effect.h"
#include "sprite_system.h"
#include "sys_task.h"

// The shared motion types, as in include/battle/battle_anim_internal.h (not on master yet)
typedef void (*BattleAnimFunc)(BattleAnimSystem *system);
typedef void (*BattleAnimSpriteFunc)(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite);

typedef struct BattleAnimLinearMotion {
    s16 x;
    s16 y;
    int duration;
    fx32 stepX;
    fx32 stepY;
    fx32 curX;
    fx32 curY;
    u8 unk_18[0xC];
} BattleAnimLinearMotion; // size: 0x24

typedef struct BattleAnimSinMotion {
    s16 x;
    s16 y;
    int duration;
    int angleX;
    int ampX;
    int angleY;
    int ampY;
    int stepX;
    int stepY;
    int unk_20;
} BattleAnimSinMotion; // size: 0x24

typedef struct BattleAnimValueMotion {
    int value;
    int duration;
    int step;
    int current;
    int unk_10;
} BattleAnimValueMotion; // size: 0x14

// Integer cosine oscillation: value = amplitude * cos(angle), the angle advancing by angleStep per frame
typedef struct BattleAnimCosMotion {
    int value;
    int duration;
    int angle;
    fx32 amplitude;
    int angleStep;
} BattleAnimCosMotion; // size: 0x14

// Offset (x, y) that alternates +amp, 0, -amp, 0 every interval frames, count cycles
typedef struct BattleAnimShake {
    s16 x;
    s16 y;
    int timer;
    int interval;
    int count;
    int phase;
    int ampX;
    int ampY;
    int prevX;
    int prevY;
} BattleAnimShake; // size: 0x24

typedef BOOL (*BattleAnimSinMotionFunc)(BattleAnimSinMotion *motion);

// Up to four sprites running copies of one motion, each started interval frames after the previous one
typedef struct BattleAnimSpriteSequence {
    s16 x;
    s16 y;
    BattleAnimSinMotion motions[4];
    BattleAnimSinMotionFunc update;
    ManagedSprite *sprites[4];
    u16 interval;
    u16 timer;
    u8 count;
    u8 started;
    u8 scale; // the motion is an affine scale instead of a position offset
    u8 unk_AF;
} BattleAnimSpriteSequence; // size: 0xB0

// BLDALPHA EVA (x) and EVB (y) moved linearly by BattleAnimTask_BlendAlphaMotion
typedef struct BattleAnimBlendAlphaMotion {
    BattleAnimLinearMotion motion;
    BOOL done;
} BattleAnimBlendAlphaMotion; // size: 0x28

typedef void (*BattleAnimScanlineCallback)(void *arg);

// VWait/VBlank task pair driving an HBlank DMA scanline effect
typedef struct BattleAnimScanlineTasks {
    SysTask *vblankTask;
    SysTask *vwaitTask;
    BOOL active;
    BOOL pending; // set by the VWait task, cleared by the VBlank task
    BattleAnimScanlineCallback vwaitCallback;
    BattleAnimScanlineCallback vblankCallback;
    void *callbackArg;
} BattleAnimScanlineTasks; // size: 0x1C

// One u32 per line, double buffered, copied to a register by HBlank DMA
typedef struct BattleAnimScanline {
    BattleAnimScanlineTasks tasks;
    DoubleBuffer *doubleBuffer;
    u32 buffers[2][192];
    void *destination;
} BattleAnimScanline; // size: 0x624

// Sine wave on a BG offset register (ScanlineEffect)
typedef struct BattleAnimScanlineWave {
    BattleAnimScanlineTasks tasks;
    ScanlineEffect *effect;
} BattleAnimScanlineWave; // size: 0x20

// Palette fade between two blend strengths, one step every delay frames
typedef struct BattleAnimPaletteBlend {
    BOOL active;
    SysTask *task;
    PaletteBufferId bufferId;
    u16 offset;
    u16 size;
    u16 color;
    u8 start;
    u8 end;
    u8 current;
    s8 step;
    s8 delay;
    vs8 timer;
    PaletteData *paletteData;
} BattleAnimPaletteBlend; // size: 0x1C

// src/battle/battle_anim_util.c
void BattleAnimUtil_InitSinMotionFromStep(BattleAnimSinMotion *motion, u32 startX, u32 endX, u32 startY, u16 endY, fx32 ampX, fx32 ampY, u16 step);
BOOL BattleAnimUtil_UpdateSinMotion(BattleAnimSinMotion *motion);
BOOL BattleAnimUtil_UpdateSinMotionSprite(BattleAnimSinMotion *motion, s16 x, s16 y, ManagedSprite *sprite);
BOOL BattleAnimUtil_UpdateSinMotionPokepic(BattleAnimSinMotion *motion, int x, int y, Pokepic *pokepic);
void BattleAnimUtil_InitLinearMotion(BattleAnimLinearMotion *motion, s16 startX, s16 endX, s16 startY, s16 endY, u16 duration);
BOOL BattleAnimUtil_UpdateLinearMotion(BattleAnimLinearMotion *motion);
BOOL BattleAnimUtil_UpdateLinearMotionSprite(BattleAnimLinearMotion *motion, ManagedSprite *sprite);
BOOL BattleAnimUtil_UpdateLinearMotionPokepic(BattleAnimLinearMotion *motion, Pokepic *pokepic);
void BattleAnimUtil_InitLinearSinMotion(BattleAnimLinearMotion *linear, BattleAnimSinMotion *sin, s16 startX, s16 endX, s16 startY, s16 endY, u16 duration, fx32 amplitude);
BOOL BattleAnimUtil_UpdateLinearSinMotion(BattleAnimLinearMotion *linear, BattleAnimSinMotion *sin);
BOOL BattleAnimUtil_UpdateLinearSinMotionSprite(BattleAnimLinearMotion *linear, BattleAnimSinMotion *sin, ManagedSprite *sprite);
void BattleAnimUtil_InitValueMotion(BattleAnimValueMotion *motion, int start, int end, int duration);
void BattleAnimUtil_InitValueMotionFx(BattleAnimValueMotion *motion, int start, int end, int duration);
BOOL BattleAnimUtil_UpdateValueMotion(BattleAnimValueMotion *motion);
BOOL BattleAnimUtil_UpdateValueMotionFx(BattleAnimValueMotion *motion);
void BattleAnimUtil_InitCosMotion(BattleAnimCosMotion *motion, u32 startAngle, u32 endAngle, u32 amplitude, u32 duration);
BOOL BattleAnimUtil_UpdateCosMotion(BattleAnimCosMotion *motion);
void BattleAnimUtil_InitScaleMotion(BattleAnimLinearMotion *motion, int start, int divisor, int end, int duration);
BOOL BattleAnimUtil_UpdateScaleMotion(BattleAnimLinearMotion *motion);
void BattleAnimUtil_InitScaleMotionXY(BattleAnimLinearMotion *motion, int startX, int endX, int startY, s16 endY, s16 divisor, int duration);
BOOL BattleAnimUtil_UpdateScaleMotionXY(BattleAnimLinearMotion *motion);
void BattleAnimUtil_GetScaleFloat(BattleAnimLinearMotion *motion, f32 *x, f32 *y);
s16 BattleAnimUtil_CalcScaleYOffset(int y, int height, fx32 scale);
void BattleAnimUtil_AlignPokepicToScale(Pokepic *pokepic, int y, int height, fx32 scale, int mode);
void BattleAnimUtil_AlignSpriteToScale(ManagedSprite *sprite, int y, int height, fx32 scale, int mode);
BOOL BattleAnimUtil_UpdateScaleMotionSprite(BattleAnimLinearMotion *motion, ManagedSprite *sprite);
BOOL BattleAnimUtil_UpdateScaleMotionPokepic(BattleAnimLinearMotion *motion, Pokepic *pokepic);
BOOL BattleAnimUtil_UpdateScaleMotionXYPokepic(BattleAnimLinearMotion *motion, Pokepic *pokepic);
void BattleAnimUtil_InitShake(BattleAnimShake *shake, int ampX, int ampY, int interval, int count);
BOOL BattleAnimUtil_UpdateShake(BattleAnimShake *shake);
BOOL BattleAnimUtil_UpdateShakePokepic(BattleAnimShake *shake, int x, int y, Pokepic *pokepic);
void BattleAnimUtil_InitSpriteSequence(BattleAnimSpriteSequence *sequence, const BattleAnimSinMotion *motion, BattleAnimSinMotionFunc update, s16 x, s16 y, u16 interval, u8 count, u8 scale, ManagedSprite *sprite0, ManagedSprite *sprite1, ManagedSprite *sprite2, ManagedSprite *sprite3);
BOOL BattleAnimUtil_UpdateSpriteSequence(BattleAnimSpriteSequence *sequence);
void ov07_02222A44(BattleAnimSinMotion *motion, int multiplier, int duration);
void BattleAnimTask_BlendAlphaMotion(SysTask *task, void *data);
void BattleAnimUtil_StartBlendAlphaMotion(BattleAnimBlendAlphaMotion *blend, s16 startEva, s16 endEva, s16 startEvb, s16 endEvb, int duration);
BOOL BattleAnimUtil_IsBlendAlphaMotionDone(BattleAnimBlendAlphaMotion *blend);
void BattleAnimScanline_VWaitTask(SysTask *task, void *data);
void BattleAnimScanline_VBlankTask(SysTask *task, void *data);
void BattleAnimScanline_StartTasks(BattleAnimScanlineTasks *tasks, void *arg, BattleAnimScanlineCallback vwaitCallback, BattleAnimScanlineCallback vblankCallback);
void BattleAnimScanline_StopTasks(BattleAnimScanlineTasks *tasks);
void BattleAnimScanline_Deactivate(BattleAnimScanlineTasks *tasks);
void BattleAnimScanline_StartHBlankDma(BattleAnimScanline *scanline);
void BattleAnimScanline_SwapAndStartDma(BattleAnimScanline *scanline);
void BattleAnimScanline_VBlankCallback(BattleAnimScanline *scanline);
BattleAnimScanline *BattleAnimScanline_New(void *destination, u32 fill, enum HeapID heapId);
void BattleAnimScanline_Free(BattleAnimScanline *scanline);
u32 *BattleAnimScanline_GetWriteBuffer(BattleAnimScanline *scanline);
void BattleAnimScanline_Disable(BattleAnimScanline *scanline);
void BattleAnimScanline_WaveVWaitCallback(BattleAnimScanlineWave *wave);
void BattleAnimScanline_WaveVBlankCallback(BattleAnimScanlineWave *wave);
BattleAnimScanlineWave *BattleAnimScanline_NewWave(u8 firstLine, u8 lastLine, u8 step, fx32 amplitude, s16 speed, int bgLayer, u32 priority, u32 fill, enum HeapID heapId);
void BattleAnimScanline_FreeWave(BattleAnimScanlineWave *wave);
void *BattleAnimScanline_GetWaveBuffer(BattleAnimScanlineWave *wave);
void BattleAnimScanline_DisableWave(BattleAnimScanlineWave *wave);
u32 BattleAnimUtil_PackXY(u16 x, u16 y);
void *BattleAnimUtil_GetBgOffsetRegister(int bgLayer);
void BattleAnimUtil_CalcMidpoint(int x0, int y0, int x1, int y1, s16 *x, s16 *y);
void BattleAnimUtil_CalcDistance(s16 x, s16 y, s16 targetX, s16 targetY, fx32 *distance);
void BattleAnimUtil_CalcAngle(s16 x, s16 y, s16 targetX, s16 targetY, u16 *angle);
BOOL BattleAnimUtil_StepTowards(int *value, int target, int step);
void BattleAnimTask_PaletteBlend(SysTask *task, void *data);
BOOL BattleAnimUtil_IsPaletteBlendActive(BattleAnimPaletteBlend *blend);
void BattleAnimUtil_FreePaletteBlend(BattleAnimPaletteBlend *blend);
BattleAnimPaletteBlend *BattleAnimUtil_StartPaletteBlend(PaletteData *paletteData, enum HeapID heapId, PaletteBufferId bufferId, u16 offset, u16 size, s8 delay, s8 step, u8 start, u8 end, u16 color, u32 priority);
void BattleAnimUtil_PaletteToGrayscale(u16 *palette, int count);
void BattleAnimSystem_ApplyGrayscale(BattleAnimSystem *system);
void BattleAnimSystem_RestoreFromGrayscale(BattleAnimSystem *system);
BattleAnimFunc BattleAnimSystem_GetFunc(u32 id);
BattleAnimSpriteFunc BattleAnimSystem_GetSpriteFunc(u32 id);
void BattleAnimFunc_None(BattleAnimSystem *system);

// overlay 7 functions defined in sources not merged yet; position: any motion (s16 x, y first)
int ov07_0221BFC0(BattleAnimSystem *system);
PaletteData *BattleAnimSystem_GetPaletteData(BattleAnimSystem *system);
fx32 ov07_02222040(fx32 start, fx32 end, int duration);
int ov07_0222204C(fx32 start, fx32 end, fx32 step);
void ov07_0222207C(void *position, ManagedSprite *sprite, s16 x, s16 y);
void ov07_0222209C(BattleAnimLinearMotion *scale, ManagedSprite *sprite);
void ov07_022220B8(void *position, Pokepic *pokepic, int x, int y);
void ov07_022220DC(BattleAnimLinearMotion *scale, Pokepic *pokepic);
void ov07_022220FC(BattleAnimSinMotion *motion, int startX, int endX, int startY, u16 endY, fx32 ampX, fx32 ampY, int duration);

// NitroSDK, not declared by lib/include
fx32 FX_Sqrt(fx32 x);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_ANIM_UTIL_H
