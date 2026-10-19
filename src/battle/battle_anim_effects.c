#include "battle/battle_anim_effects.h"

#include "global.h"

#include "constants/battle.h"

#include "assert.h"
#include "bg_window.h"
#include "brightness.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task.h"

typedef BattleAnimScanlineWave *(*BattleAnimScanlineNewWaveFunc)(u8 firstLine, u8 lastLine, u32 step, fx32 amplitude, s16 speed, int bgLayer, u32 priority, u32 fill, enum HeapID heapId);
#define BattleAnimScanline_NewWaveU32Step ((BattleAnimScanlineNewWaveFunc)BattleAnimScanline_NewWave)

typedef void (*ManagedSpriteSetPaletteOffsetFunc)(ManagedSprite *managedSprite, int offset);

typedef void (*SetBgPriorityU8Func)(u8 bgId, u8 priority);

struct BattleAnimSkewSlideWork {
    BattleAnimSystem *system;
    int state;
    int timer;
    Pokepic *pokepic;
    s16 x; // BG scroll position of the pic copy
    s16 y;
    s16 top; // scanline range of the wave
    s16 bottom;
    BattleAnimScanlineWave *wave;
    BattleAnimLinearMotion slide;
    BattleAnimBlendAlphaMotion blend;
};

typedef struct BattleAnimFadeInShrinkSpriteWork {
    BattleAnimSystem *system;
    SpriteManager *spriteManager;
    int state;
    int timer;
    ManagedSprite *sprite;
    BattleAnimLinearMotion scale;
    BattleAnimValueMotion blend;
    BOOL flipX;
} BattleAnimFadeInShrinkSpriteWork;

typedef struct BattleAnimShakeDefenderBlackFlashWork {
    BattleAnimSystem *system;
    int state;
    Pokepic *pokepic;
    ManagedSprite *sprite; // battler sprite 1, used when the defender is the attacker's partner
    BOOL withSprite;
    BattleAnimShake shake;
    s16 x;
    s16 y;
    int shadowHeight;
} BattleAnimShakeDefenderBlackFlashWork;

struct BattleAnimWaveOnBgWork {
    BattleAnimSystem *system;
    int state;
    int blendState; // state of BattleAnimTask_WaveDefenderOnBgUpdate
    u32 unk_0C;
    Pokepic *pokepic;
    BattleAnimScanlineWave *wave;
    PaletteData *paletteData;
    BattleAnimBlendAlphaMotion blend;
    int timer;
    ManagedSprite *sprite;
};

typedef struct BattleAnimGrowRockShrinkSpriteWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int state;
    ManagedSprite *sprite;
    BattleAnimLinearMotion scale;
    BattleAnimValueMotion rotation;
    int rocks;
    int prevRotation;
    int sign; // x scale sign
} BattleAnimGrowRockShrinkSpriteWork;

typedef struct BattleAnimStackedSpritesWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int state;
    int timer;
    int index;
    Pokepic *pokepic;
    s16 x;
    s16 y;
    BattleAnimShake shake;
    BattleAnimScaleSprite sprites[4];
    u32 unk_124;
} BattleAnimStackedSpritesWork;

typedef struct BattleAnimBoomerangSpriteWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int state;
    Pokepic *attackerPokepic;
    s16 attackerX;
    s16 attackerY;
    BattleAnimLinearMotion lean;
    int direction;
    Pokepic *defenderPokepic;
    s16 defenderX;
    s16 defenderY;
    ManagedSprite *sprite;
    BattleAnimLinearMotion arc;
    BattleAnimSinMotion arcHeight;
} BattleAnimBoomerangSpriteWork;

typedef struct BattleAnimWork0222DB14 {
    BattleAnimSystem *system;
    SpriteManager *spriteManager;
    int state;
    int timer;
    Pokepic *pokepic;
    ManagedSprite *sprite;
    s16 x;
    s16 y;
    BattleAnimSinMotion motion;
    BattleAnimBlendAlphaMotion blend;
    s16 shadowHeight;
} BattleAnimWork0222DB14;

struct BattleAnimRisingSpriteStretchWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int state;
    Pokepic *pokepic;
    BattleAnimLinearMotion picScale;
    int picState;
    ManagedSprite *sprite;
    int spriteState;
    BattleAnimLinearMotion spriteScale;
    BattleAnimLinearMotion rise;
    BattleAnimBlendAlphaMotion blend;
    s16 x;
    s16 y;
};

struct BattleAnimZigzagSlashWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int state;
    Pokepic *pokepic;
    ManagedSprite *sprite;
    BattleAnimLinearMotion leg;
    BattleAnimBlendAlphaMotion blend;
    s16 x;
    s16 y;
    int legIndex;
    int timer;
};

struct BattleAnimZigzagFlashWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int state;
    Pokepic *pokepic;
    Pokepic *pokepics[4];
    ManagedSprite *sprite;
    BattleAnimLinearMotion leg;
    s16 x;
    s16 y;
    int legIndex; // also the blinks left at the end
    int timer;
    BOOL draw;
};

struct BattleAnimWaddlePicWork {
    int state;
    BattleAnimSystem *system;
    Pokepic *pokepic;
    s16 x;
    s16 y;
    BattleAnimValueMotion tilt;
    BattleAnimLinearMotion hop;
    int steps;
    int stepState;
    int stepTimer;
    BOOL direction;
};

struct BattleAnimAttackerStretchUpWork {
    BattleAnimSystem *system;
    int state;
    int timer;
    Pokepic *pokepic;
    BattleAnimLinearMotion scale;
    int delay;
    int stepState;
    int y;
    int height;
};

struct BattleAnimPulseSpriteWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int state;
    int timer;
    ManagedSprite *sprite;
    BattleAnimLinearMotion scale;
    BattleAnimBlendAlphaMotion blend;
    int pulseState;
    int pulseCount;
    int repeats;
    int duration; // x100
    int scaleA;   // x100
    int scaleB;   // x100
    int roundState;
};

struct BattleAnimAttackerDipWork {
    int state;
    BattleAnimSystem *system;
    Pokepic *pokepic;
    s16 x;
    s16 y;
    BattleAnimLinearMotion move;
    int leg;
};

// move1 and move2 are used as linear (x) and as sine motions
typedef struct BattleAnimLungeKnockbackWork {
    int state;
    BattleAnimSystem *system;
    int direction;
    Pokepic *attackerPokepic;
    s16 attackerX;
    s16 attackerY;

    union {
        BattleAnimSinMotion sin; // the dip before the lunge
        BattleAnimLinearMotion linear;
    } attackerMove;

    Pokepic *defenderPokepic;
    s16 defenderX;
    s16 defenderY;
    BattleAnimLinearMotion defenderMove;
} BattleAnimLungeKnockbackWork;

struct BattleAnimFanSpritesWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int state;
    int index;
    u32 unk_14;
    Pokepic *pokepic;
    BattleAnimLinearMotion pulse;
    int pulseState;
    s16 y;
    s16 height;
    ManagedSprite *sprites[6];
    u32 animStates[6];
    int timers[6];
};

typedef struct BattleAnimShrinkIntoSpriteWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int state;
    Pokepic *pokepic;
    BattleAnimLinearMotion scale;
    BattleAnimLinearMotion move;
    ManagedSprite *sprite;
    BattleAnimLinearMotion leave;
    u32 unk_84;
    s16 x;
    s16 y;
} BattleAnimShrinkIntoSpriteWork;

struct BattleAnimDarkSpriteJumpWork {
    BattleAnimSystem *system;
    SpriteManager *spriteManager;
    int state;
    int timer;
    int height;
    int y;
    ManagedSprite *sprites[4]; // rise, drop, and an extra sprite per part
    BattleAnimLinearMotion stretch;
    BattleAnimLinearMotion scale;
    BattleAnimLinearMotion drop;
    int partState;
    int partTimer;
    BattleAnimBlendAlphaMotion blend;
    PaletteData *paletteData;
    BOOL flipRise;
    BOOL flipDrop;
};

struct BattleAnimScaleSpritesWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int state;
    Pokepic *pokepic;
    BattleAnimShake shake;
    s16 x;
    s16 y;
    ManagedSprite *sprites[3];
    BattleAnimSpriteSequence sequence;
    BattleAnimLinearMotion scale;
};

struct BattleAnimOrbitSpritesWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int state;
    int timer;
    int direction;
    ManagedSprite *sprites[6];
    BattleAnimSinMotion paths[6];
    BattleAnimBlendAlphaMotion blend;
    s16 x;
    s16 y;
};

struct BattleAnimFloatingSpritesWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u32 unk_0C;
    int dirX;
    int dirY;
    ManagedSprite *sprites[15];
    BattleAnimLinearMotion motions[15];
    int timer;
    int count;
};

struct BattleAnimWobbleOnBgWork {
    BattleAnimSystem *system;
    BattleAnimScanline *scanline;
    int state;
    Pokepic *pokepic;
    int top;
    int center;
    u32 angle;
    fx32 amplitude;
    int alternation; // +1/-1, flipped every frame
    int slope;
    int timer;
    int phase;
    u32 scroll; // packed BG offset (x, y)
};

typedef struct BattleAnimWindowSweepWork {
    BattleAnimSystem *system;
    int state;
    int timer;
    BattleAnimLinearMotion sweep;
    u32 unk_30;
} BattleAnimWindowSweepWork;

typedef struct BattleAnimFadeSpriteAnimWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int state;
    ManagedSprite *sprite;
    BattleAnimBlendAlphaMotion blend;
} BattleAnimFadeSpriteAnimWork;

typedef struct BattleAnimStretchShakeWork {
    BattleAnimSystem *system;
    int state;
    s16 y;
    s16 x;
    s16 height;
    s16 unk_0E;
    Pokepic *pokepic;
    BattleAnimLinearMotion scale;
    BattleAnimShake shake;
    int startX; // scales x10
    int endX;
    int startY;
    int endY;
    int divisor;
    int repeats;
    u32 durations; // frames out << 16 | frames back
} BattleAnimStretchShakeWork;

typedef struct BattleAnimShootSpriteWork {
    BattleAnimSystem *system;
    SpriteManager *spriteManager;
    BattleAnimLinearMotion motion;
    ManagedSprite *sprite;
} BattleAnimShootSpriteWork;

typedef struct BattleAnimShakeLungeWork {
    BattleAnimSystem *system;
    int state;
    int timer;
    int dirX;
    int dirY;
    Pokepic *pokepic;
    s16 x;
    s16 y;
    BattleAnimLinearMotion move;
    BattleAnimShake shake;
} BattleAnimShakeLungeWork;

typedef struct BattleAnimKnockbackWork {
    BattleAnimSystem *system;
    int state;
    int dirX;
    int dirY;
    Pokepic *pokepic;
    s16 x;
    s16 y;
    BattleAnimLinearMotion move;
    BattleAnimShake shake;
} BattleAnimKnockbackWork;

typedef struct BattleAnimRisingShrinkSpritesWork {
    BattleAnimSystem *system;
    int state;
    int timer;
    SpriteManager *spriteManager;
    int direction;
    ManagedSprite *sprites[2];
    BattleAnimLinearMotion moves[2];
    BattleAnimLinearMotion scales[2];
} BattleAnimRisingShrinkSpritesWork;

typedef struct BattleAnimPaletteCycleSpriteWork {
    BattleAnimSystem *system;
    int state;
    int paletteOffset;
    SpriteManager *spriteManager;
    ManagedSprite *sprite;
    BattleAnimBlendAlphaMotion blend;
} BattleAnimPaletteCycleSpriteWork;

typedef struct BattleAnimSpriteRowWork {
    BattleAnimSystem *system;
    int state;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[6];
    int visibleTimes[6];
    int delays[6];
} BattleAnimSpriteRowWork;

struct BattleAnimObjWindowBgWork {
    BattleAnimObjWindowBgParams params;
    BattleAnimCommonWork common;
    ManagedSprite *sprites[3];
    u8 timer;
};

extern const u16 gBattleAnimDarkSpriteDrawFlags[6];
// NARC members of each background the object window effect can show
typedef struct BattleAnimObjWindowBgFiles {
    u32 graphics;
    u32 palette;
    u32 screen;
    u32 unk_0C;
} BattleAnimObjWindowBgFiles;

extern const BattleAnimObjWindowBgFiles gBattleAnimObjWindowBgFiles[];

void BattleAnimTask_SkewSlideAttackerBgResetScroll(BattleAnimSkewSlideWork *work, u32 *buffer) {
    u32 value = ((u32)-work->y << 16) | (u16)-work->x;
    MIi_CpuClear32(value, buffer, 192 * sizeof(u32));
}

BOOL BattleAnimTask_SkewSlideAttackerBgUpdateScroll(BattleAnimSkewSlideWork *work) {
    u32 *buffer;
    BOOL active;
    int i;
    s16 x;
    s16 y;
    int line;

    buffer = BattleAnimScanline_GetWaveBuffer(work->wave);
    BattleAnimTask_SkewSlideAttackerBgResetScroll(work, buffer);
    active = BattleAnimUtil_UpdateLinearMotion(&work->slide);
    for (i = work->top; i <= work->bottom; i++) {
        x = (s16)buffer[i];
        y = (s16)(buffer[i] >> 16);
        y -= ((work->bottom - i) * work->slide.y) / 100;
        x -= ((work->top + 40 - i) * work->slide.x) / 100;
        line = i - 1;
        if (line < 0) {
            line += 192;
        }
        buffer[line] = BattleAnimUtil_PackXY((u16)x, (u16)y);
    }
    return active;
}

enum {
    SKEW_SLIDE_STATE_SCROLL,
    SKEW_SLIDE_STATE_BLEND_IN,
    SKEW_SLIDE_STATE_HOLD,
    SKEW_SLIDE_STATE_BLEND_OUT,
    SKEW_SLIDE_STATE_END,
};

void BattleAnimTask_SkewSlideAttackerBg(SysTask *task, void *data) {
    BattleAnimSkewSlideWork *work = data;
    int bgLayer;

    switch (work->state) {
    case SKEW_SLIDE_STATE_SCROLL:
        if (!BattleAnimTask_SkewSlideAttackerBgUpdateScroll(work)) {
            work->state++;
            BattleAnimUtil_SetBlendBgOverBattleBg(work->system, 31, 0);
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 31, 0, 0, 31, 16);
            return;
        }
        break;
    case SKEW_SLIDE_STATE_BLEND_IN:
        BattleAnimTask_SkewSlideAttackerBgUpdateScroll(work);
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            work->state++;
            BattleAnimScanline_DisableWave(work->wave);
            work->timer = 0;
            return;
        }
        break;
    case SKEW_SLIDE_STATE_HOLD:
        BgSetPosTextAndCommit(BattleAnimSystem_GetBgConfig(work->system), GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, -work->x);
        BgSetPosTextAndCommit(BattleAnimSystem_GetBgConfig(work->system), GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, -work->y);
        work->timer++;
        if (work->timer > 8) {
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 0, 31, 31, 0, 16);
            work->state++;
            return;
        }
        break;
    case SKEW_SLIDE_STATE_BLEND_OUT:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, FALSE);
            work->state++;
            return;
        }
        break;
    case SKEW_SLIDE_STATE_END:
        BattleAnimScanline_FreeWave(work->wave);
        bgLayer = BattleAnimSystem_GetBgLayer(work->system, 1);
        SetBgPriority((u8)bgLayer, (u8)BattleAnimSystem_GetBgPriority(work->system, 1));
        SetBgPriority(GF_BG_LYR_MAIN_0, (u8)BattleAnimSystem_GetBasePriority(work->system));
        bgLayer = BattleAnimSystem_GetBgLayer(work->system, 1);
        BG_ClearCharDataRange((u8)bgLayer, 0x4000, 0, BattleAnimSystem_GetHeapID(work->system));
        BgClearTilemapBufferAndCommit(BattleAnimSystem_GetBgConfig(work->system), BattleAnimSystem_GetBgLayer(work->system, 1));
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

void BattleAnimFunc_SkewSlideAttackerBg(BattleAnimSystem *system) {
    BattleAnimSkewSlideWork *work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimSkewSlideWork));
    SysTask *task;
    int direction;
    int bgLayer;
    int type;
    u32 bgIndex;
    u32 priority;
    u32 scroll;

    memset(work, 0, sizeof(BattleAnimSkewSlideWork));
    work->system = system;
    task = BattleAnimSystem_CreateTaskDefaultPriority(system, BattleAnimTask_SkewSlideAttackerBg, work);
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->y -= Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H);
    work->x -= 40;
    work->y -= 40;
    Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, TRUE);
    direction = BattleAnimUtil_GetBattlerSideSignByMode(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->top = work->y - 8;
    work->bottom = work->y + 88;
    if (work->top < 0) {
        work->top = 0;
    }
    if (work->bottom > 191) {
        work->bottom = 191;
    }
    bgIndex = BattleAnimSystem_GetBgMaskLayer(system, 1);
    priority = SysTask_GetPriority(task);
    scroll = BattleAnimUtil_PackXY(work->x, work->y);
    work->wave = BattleAnimScanline_NewWaveU32Step(work->top, work->bottom, 910, direction << 15, 80, bgIndex, priority + 1, scroll, BattleAnimSystem_GetHeapID(system));
    BattleAnimUtil_InitLinearMotion(&work->slide, 0, 80, 0, 40, 24);
    work->slide.stepX *= direction;
    type = BattleAnimUtil_GetBattlerType(work->system, BattleAnimSystem_GetAttacker(system));
    if (type == BATTLER_TYPE_ENEMY_SIDE_SLOT_1 || type == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
        bgLayer = BattleAnimSystem_GetBgLayer(work->system, 1);
        SetBgPriority(bgLayer, (u8)BattleAnimSystem_GetBasePriority(work->system));
        SetBgPriority(GF_BG_LYR_MAIN_0, (u8)(BattleAnimSystem_GetBasePriority(work->system) + 1));
    }
    BattleAnimTask_SkewSlideAttackerBg(task, work);
}

enum {
    FADE_SHRINK_STATE_FADE_IN,
    FADE_SHRINK_STATE_HOLD,
    FADE_SHRINK_STATE_SHRINK,
    FADE_SHRINK_STATE_END,
};

void BattleAnimTask_FadeInShrinkSprite(SysTask *task, void *data) {
    BattleAnimFadeInShrinkSpriteWork *work = data;
    f32 x;
    f32 y;

    switch (work->state) {
    case FADE_SHRINK_STATE_FADE_IN: {
        BOOL active = BattleAnimUtil_UpdateValueMotionFx(&work->blend);
        reg_G2_BLDALPHA = work->blend.value | ((31 - work->blend.value) << 8);
        if (active == FALSE) {
            work->state++;
            work->timer = 0;
        }
    } break;
    case FADE_SHRINK_STATE_HOLD:
        work->timer++;
        if (work->timer > 32) {
            work->state++;
        }
        break;
    case FADE_SHRINK_STATE_SHRINK: {
        BOOL active = BattleAnimUtil_UpdateScaleMotion(&work->scale);
        BattleAnimUtil_GetScaleFloat(&work->scale, &x, &y);
        if (work->flipX == TRUE) {
            x = 0.0f - x;
        }
        ManagedSprite_SetAffineScale(work->sprite, x, y);
        if (active == FALSE) {
            ManagedSprite_SetDrawFlag(work->sprite, FALSE);
            work->state++;
        }
        break;
    }
    case FADE_SHRINK_STATE_END:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

enum {
    BLACK_FLASH_STATE_FADE_OUT,
    BLACK_FLASH_STATE_FADE_IN,
    BLACK_FLASH_STATE_END,
};

void BattleAnimTask_ShakeDefenderBlackFlash(SysTask *task, void *data) {
    BattleAnimShakeDefenderBlackFlashWork *work = data;

    switch (work->state) {
    case BLACK_FLASH_STATE_FADE_OUT:
        BattleAnimUtil_UpdateShake(&work->shake);
        BattleAnimUtil_SetPokepicPosWithOffset(&work->shake, work->pokepic, work->x, work->y);
        if (work->withSprite == TRUE) {
            BattleAnimUtil_SetSpritePosWithOffset(&work->shake, work->sprite, work->x, (s16)(work->y - work->shadowHeight));
        }
        if (Pokepic_ResumePaletteFade(work->pokepic) == FALSE) {
            Pokepic_StartPaletteFade(work->pokepic, 16, 0, 0, 0);
            if (work->withSprite == TRUE) {
                u32 mask = 1 << BattleAnimUtil_GetSpritePaletteSlot(work->sprite);
                PaletteData_BeginPaletteFade(BattleAnimSystem_GetPaletteData(work->system), PLTTBUF_MAIN_OBJ_F, (u16)mask, 0, 16, 0, 0);
            }
            work->state++;
        }
        break;
    case BLACK_FLASH_STATE_FADE_IN:
        BattleAnimUtil_UpdateShake(&work->shake);
        BattleAnimUtil_SetPokepicPosWithOffset(&work->shake, work->pokepic, work->x, work->y);
        if (work->withSprite == TRUE) {
            BattleAnimUtil_SetSpritePosWithOffset(&work->shake, work->sprite, work->x, (s16)(work->y - work->shadowHeight));
        }
        if (Pokepic_ResumePaletteFade(work->pokepic) == FALSE) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_X, work->x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y);
            if (work->withSprite == TRUE) {
                if (BattleAnimSystem_IsBattlerSemiInvulnerable(work->system, BattleAnimSystem_GetDefender(work->system)) == FALSE) {
                    Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, FALSE);
                }
                ManagedSprite_SetPositionXY(work->sprite, work->x, (s16)(work->y - work->shadowHeight));
            }
            work->state++;
        }
        break;
    case BLACK_FLASH_STATE_END:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

void BattleAnimFunc_FadeInShrinkSprite(BattleAnimSystem *system) {
    BattleAnimFadeInShrinkSpriteWork *work;

    work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimFadeInShrinkSpriteWork));
    memset(work, 0, sizeof(BattleAnimFadeInShrinkSpriteWork));
    work->system = system;
    work->spriteManager = BattleAnimSystem_GetBattlerSpriteManager(system);
    work->flipX = BattleAnimSystem_GetField08(system);
    work->sprite = BattleAnimSystem_GetBattlerSprite(work->system, 0);
    BattleAnimUtil_InitScaleMotion(&work->scale, 14, 10, 10, 8);
    BattleAnimUtil_InitValueMotionFx(&work->blend, 0, 16, 8);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    BattleAnimUtil_SetBlendWithBattleBg(work->system, 0, 31);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    if (work->flipX == TRUE) {
        ManagedSprite_SetAffineScale(work->sprite, -1.4f, 1.4f);
    } else {
        ManagedSprite_SetAffineScale(work->sprite, 1.4f, 1.4f);
    }
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_FadeInShrinkSprite, work);
}

void BattleAnimFunc_ShakeDefenderBlackFlash(BattleAnimSystem *system) {
    BattleAnimShakeDefenderBlackFlashWork *work;
    u32 mask;
    u32 slot;
    u32 attackerType;
    u32 defenderType;

    work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimShakeDefenderBlackFlashWork));
    memset(work, 0, sizeof(BattleAnimShakeDefenderBlackFlashWork));
    work->system = system;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(system));
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->shadowHeight = Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H);
    BattleAnimUtil_InitShake(&work->shake, 4, 0, 1, 4);
    work->shake.x += BattleAnimUtil_GetBattlerSideSignByMode(work->system, BattleAnimSystem_GetDefender(work->system));
    Pokepic_StartPaletteFade(work->pokepic, 0, 16, 0, 0);
    work->sprite = BattleAnimSystem_GetBattlerSprite(work->system, 1);
    attackerType = BattleAnimUtil_GetBattlerType(work->system, BattleAnimSystem_GetAttacker(work->system));
    defenderType = BattleAnimUtil_GetBattlerType(work->system, BattleAnimSystem_GetDefender(work->system));
    if ((attackerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1 && defenderType == BATTLER_TYPE_PLAYER_SIDE_SLOT_2)
        || (attackerType == BATTLER_TYPE_ENEMY_SIDE_SLOT_2 && defenderType == BATTLER_TYPE_ENEMY_SIDE_SLOT_1)) {
        slot = BattleAnimUtil_GetSpritePaletteSlot(work->sprite);
        mask = 1;
        mask <<= slot;
        PaletteData_BeginPaletteFade(BattleAnimSystem_GetPaletteData(work->system), PLTTBUF_MAIN_OBJ_F, (u16)mask, 0, 0, 16, 0);
        work->withSprite = TRUE;
        if (!BattleAnimSystem_IsBattlerSemiInvulnerable(work->system, BattleAnimSystem_GetDefender(work->system))) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, TRUE);
        }
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_ShakeDefenderBlackFlash, work);
}

enum {
    WAVE_BLEND_STATE_WAIT_BLEND_IN,
    WAVE_BLEND_STATE_HOLD,
    WAVE_BLEND_STATE_BLEND_OUT,
    WAVE_BLEND_STATE_DONE,
};

BOOL BattleAnimTask_WaveDefenderOnBgUpdate(BattleAnimWaveOnBgWork *work) {
    BOOL done = FALSE;

    switch (work->blendState) {
    case WAVE_BLEND_STATE_WAIT_BLEND_IN:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend) == TRUE) {
            work->blendState++;
            work->timer = 32;
        }
        break;
    case WAVE_BLEND_STATE_HOLD:
        if (--work->timer < 0) {
            work->blendState++;
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 8, 0, 8, 16, 24);
        }
        break;
    case WAVE_BLEND_STATE_BLEND_OUT:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend) == TRUE) {
            work->blendState++;
            BattleAnimScanline_FreeWave(work->wave);
        }
        break;
    case WAVE_BLEND_STATE_DONE:
        done = TRUE;
        break;
    }
    return done;
}

enum {
    WAVE_ON_BG_STATE_HIDE_PIC,
    WAVE_ON_BG_STATE_WAVE,
    WAVE_ON_BG_STATE_END,
};

void BattleAnimTask_WaveDefenderOnBg(SysTask *task, void *data) {
    BattleAnimWaveOnBgWork *work = data;

    switch (work->state) {
    case WAVE_ON_BG_STATE_HIDE_PIC:
        BattleAnimTask_WaveDefenderOnBgUpdate(work);
        if (ManagedSprite_GetDrawFlag(work->sprite) == TRUE) {
            int defender = BattleAnimSystem_GetDefender(work->system);
            if (BattleAnimSystem_IsBattlerSemiInvulnerable(work->system, defender) == FALSE) {
                Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, TRUE);
            }
        }
        work->state++;
        break;
    case WAVE_ON_BG_STATE_WAVE:
        if (BattleAnimTask_WaveDefenderOnBgUpdate(work)) {
            work->state++;
        }
        break;
    case WAVE_ON_BG_STATE_END: {
        int bgLayer = BattleAnimSystem_GetBgLayer(work->system, 1);
        int priority = BattleAnimSystem_GetBgPriority(work->system, 1);
        SetBgPriority((u8)bgLayer, (u8)priority);
        SetBgPriority(GF_BG_LYR_MAIN_0, (u8)BattleAnimSystem_GetBasePriority(work->system));
        int defender = BattleAnimSystem_GetDefender(work->system);
        if (BattleAnimSystem_IsBattlerSemiInvulnerable(work->system, defender) == FALSE) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, FALSE);
        }
        bgLayer = BattleAnimSystem_GetBgLayer(work->system, 1);
        BG_ClearCharDataRange((u8)bgLayer, 0x4000, 0, BattleAnimSystem_GetHeapID(work->system));
        BgClearTilemapBufferAndCommit(BattleAnimSystem_GetBgConfig(work->system), (u8)BattleAnimSystem_GetBgLayer(work->system, 1));
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
    }
}

void BattleAnimFunc_WaveDefenderOnBg(BattleAnimSystem *system) {
    BattleAnimWaveOnBgWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimWaveOnBgWork));
    s16 x;
    s16 y;
    s16 bottom;
    int type;
    int bgIndex;
    int top;
    int scroll;

    work->system = system;
    work->paletteData = BattleAnimSystem_GetPaletteData(system);
    BattleAnimUtil_SetBlendBgOverBattleBg(work->system, 0, 16);
    BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 0, 8, 16, 8, 24);
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(work->system));
    x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    y -= Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H);
    y -= 40;
    work->sprite = BattleAnimSystem_GetBattlerSprite(work->system, 0);
    type = BattleAnimUtil_GetBattlerType(work->system, BattleAnimSystem_GetDefender(work->system));
    if (type == BATTLER_TYPE_ENEMY_SIDE_SLOT_2 || type == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
        ManagedSprite_SetDrawFlag(work->sprite, TRUE);
        ManagedSprite_SetPriority(work->sprite, 2);
    } else {
        int bgLayer = BattleAnimSystem_GetBgLayer(work->system, 1);
        SetBgPriority((u8)bgLayer, (u8)BattleAnimSystem_GetBasePriority(work->system));
        SetBgPriority(GF_BG_LYR_MAIN_0, (u8)(BattleAnimSystem_GetBasePriority(work->system) + 1));
        ManagedSprite_SetDrawFlag(work->sprite, FALSE);
    }
    bottom = y + 80;
    top = y;
    if (y < 0) {
        y = 0;
    }
    if (bottom > 191) {
        bottom = 191;
    }
    bgIndex = BattleAnimSystem_GetBgMaskLayer(system, 1);
    scroll = BattleAnimUtil_PackXY((u16)(-(s16)(x - 40)), (u16)(-top));
    type = BattleAnimSystem_GetHeapID(system);
    work->wave = BattleAnimScanline_NewWaveU32Step((u8)y, (u8)bottom, 910, 0x5000, 100, bgIndex, 0, scroll, (enum HeapID)type);
    PaletteData_BlendPalettes(work->paletteData, PLTTBUF_MAIN_BG, 0x100, 8, 0);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_WaveDefenderOnBg, work);
}

enum {
    GROW_ROCK_STATE_GROW,
    GROW_ROCK_STATE_ROCK,
    GROW_ROCK_STATE_SHRINK,
    GROW_ROCK_STATE_END,
};

void BattleAnimTask_GrowRockShrinkSprite(SysTask *task, void *data) {
    BattleAnimGrowRockShrinkSpriteWork *work = data;
    f32 x;
    f32 y;
    BOOL active;

    switch (work->state) {
    case GROW_ROCK_STATE_GROW:
        if (BattleAnimUtil_UpdateScaleMotion(&work->scale) == TRUE) {
            BattleAnimUtil_GetScaleFloat(&work->scale, &x, &y);
            ManagedSprite_SetAffineScale(work->sprite, x * (f32)work->sign, y);
        } else {
            work->state++;
            work->rocks = 5;
            BattleAnimUtil_InitScaleMotion(&work->scale, 10, 10, 1, 8);
        }
        break;
    case GROW_ROCK_STATE_ROCK:
        active = BattleAnimUtil_UpdateValueMotion(&work->rotation);
        ManagedSprite_SetAffineZRotation(work->sprite, work->rotation.value);
        if (!active) {
            if (work->rocks > 0) {
                int current;
                int prev;
                work->rocks--;
                current = work->rotation.value;
                prev = work->prevRotation;
                work->prevRotation = current;
                BattleAnimUtil_InitValueMotion(&work->rotation, current, prev, 4);
            } else {
                work->state++;
            }
        }
        break;
    case GROW_ROCK_STATE_SHRINK:
        if (BattleAnimUtil_UpdateScaleMotion(&work->scale) == TRUE) {
            BattleAnimUtil_GetScaleFloat(&work->scale, &x, &y);
            ManagedSprite_SetAffineScale(work->sprite, x * (f32)work->sign, y);
        } else {
            work->state++;
        }
        break;
    case GROW_ROCK_STATE_END:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_GrowRockShrinkSprite(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimGrowRockShrinkSpriteWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimGrowRockShrinkSpriteWork));
    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    int attacker = BattleAnimSystem_GetAttacker(work->system);
    int direction = BattleAnimUtil_GetBattlerSideSignByMode(work->system, attacker);
    int offset;
    if (BattleAnimSystem_ShouldFlipSide(work->system, 0) == TRUE) {
        work->sign = -1;
        offset = -direction;
    } else {
        work->sign = 1;
        offset = direction;
    }
    s16 x = BattleAnimSystem_GetBattlerScreenCoord(system, BattleAnimSystem_GetAttacker(work->system), 0);
    s16 y = BattleAnimSystem_GetBattlerScreenCoord(system, BattleAnimSystem_GetAttacker(work->system), 1);
    work->sprite = sprite;
    ManagedSprite_SetAffineOverwriteMode(sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    ManagedSprite_SetAffineScale(work->sprite, 1.0f, 1.0f);
    ManagedSprite_SetPositionXY(work->sprite, x, y);
    ManagedSprite_OffsetPositionXY(work->sprite, (s16)(direction * 40), 0);
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    ManagedSprite_SetAffineTranslation(work->sprite, (s16)(offset * -12), 12);
    if (BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetAttacker(system)) == BATTLE_ANIM_SIDE_ENEMY) {
        ManagedSprite_SetAnim(work->sprite, 1);
    } else {
        ManagedSprite_SetAnim(work->sprite, 0);
    }
    BattleAnimUtil_InitScaleMotion(&work->scale, 1, 10, 10, 8);
    if (offset >= 0) {
        BattleAnimUtil_InitValueMotion(&work->rotation, 0xFF48, 0xE38D, 4);
        work->prevRotation = 0xFF48;
    } else {
        BattleAnimUtil_InitValueMotion(&work->rotation, 0, 0x1C71, 4);
        work->prevRotation = 0;
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_GrowRockShrinkSprite, work);
}

void BattleAnimScaleSprite_Init(BattleAnimScaleSprite *scaleSprite, ManagedSprite *sprite, s16 x, s16 y, BOOL flip) {
    scaleSprite->sprite = sprite;
    BattleAnimUtil_InitScaleMotion(&scaleSprite->scale, 10, 10, 8, 8);
    scaleSprite->repeats = 3;
    scaleSprite->start = 8;
    scaleSprite->end = 10;
    scaleSprite->flip = flip;
    ManagedSprite_SetPositionXY(scaleSprite->sprite, x, y);
    ManagedSprite_SetPositionXY(scaleSprite->sprite, x, y);
    ManagedSprite_SetAffineOverwriteMode(scaleSprite->sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    ManagedSprite_SetDrawPriority(scaleSprite->sprite, 100);
    ManagedSprite_SetPriority(scaleSprite->sprite, 1);
    if (flip) {
        ManagedSprite_SetAffineScale(scaleSprite->sprite, -1.0f, 1.0f);
    }
    ManagedSprite_SetDrawFlag(scaleSprite->sprite, FALSE);
}

void BattleAnimScaleSprite_Delete(BattleAnimScaleSprite *scaleSprite) {
    Sprite_DeleteAndFreeResources(scaleSprite->sprite);
}

void BattleAnimScaleSprite_Show(BattleAnimScaleSprite *scaleSprite) {
    ManagedSprite_SetDrawFlag(scaleSprite->sprite, TRUE);
    ManagedSprite_SetAnimateFlag(scaleSprite->sprite, TRUE);
}

BOOL BattleAnimScaleSprite_Update(BattleAnimScaleSprite *scaleSprite) {
    f32 x;
    f32 y;

    if (BattleAnimUtil_UpdateScaleMotion(&scaleSprite->scale) == TRUE) {
        BattleAnimUtil_GetScaleFloat(&scaleSprite->scale, &x, &y);
        if (scaleSprite->flip != FALSE) {
            ManagedSprite_SetAffineScale(scaleSprite->sprite, 0.0f - x, 1.0f);
        } else {
            ManagedSprite_SetAffineScale(scaleSprite->sprite, x, 1.0f);
        }
    } else if (scaleSprite->repeats > 0) {
        scaleSprite->repeats--;
        BattleAnimUtil_InitScaleMotion(&scaleSprite->scale, (s16)scaleSprite->start, 10, (s16)scaleSprite->end, 8);
        {
            int start = scaleSprite->start;
            scaleSprite->start = scaleSprite->end;
            scaleSprite->end = start;
        }
    } else {
        return FALSE;
    }
    return TRUE;
}

enum {
    STACKED_SPRITES_STATE_SHOW,
    STACKED_SPRITES_STATE_SHAKE,
    STACKED_SPRITES_STATE_SCALE,
    STACKED_SPRITES_STATE_END,
};

void BattleAnimTask_StackedSpritesOnDefender(SysTask *task, void *data) {
    BattleAnimStackedSpritesWork *work = data;
    int i;

    switch (work->state) {
    case STACKED_SPRITES_STATE_SHOW:
        if (work->timer >= 4) {
            work->timer = 0;
            BattleAnimScaleSprite_Show(&work->sprites[work->index]);
            work->index++;
            if (work->index >= 4) {
                work->state++;
            }
        }
        work->timer++;
        break;
    case STACKED_SPRITES_STATE_SHAKE:
        if (BattleAnimUtil_UpdateShakePokepic(&work->shake, work->x, work->y, work->pokepic) == FALSE) {
            work->state++;
        }
        break;
    case STACKED_SPRITES_STATE_SCALE: {
        BOOL active;
        for (i = 0; i < 4; i++) {
            active = BattleAnimScaleSprite_Update(&work->sprites[i]);
        }
        if (active == FALSE) {
            work->state++;
        }
        break;
    }
    case STACKED_SPRITES_STATE_END:
        Pokepic_SetAttr(work->pokepic, POKEPIC_X, work->x);
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y);
        for (i = 0; i < 4; i++) {
            BattleAnimScaleSprite_Delete(&work->sprites[i]);
        }
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_StackedSpritesOnDefender(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;
    BattleAnimStackedSpritesWork *work;
    int y;
    int flip;
    int i;

    work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimStackedSpritesWork));
    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(system));
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    BattleAnimUtil_InitShake(&work->shake, 4, 0, 1, 1);
    y = work->y + 16;
    flip = FALSE;
    BattleAnimSystem_GetSpriteTemplate(&template, work->system);
    for (i = flip; i < 4; i++) {
        if (i == 0) {
            BattleAnimScaleSprite_Init(&work->sprites[i], sprite, work->x, y, flip);
        } else {
            ManagedSprite *newSprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
            BattleAnimScaleSprite_Init(&work->sprites[i], newSprite, work->x, y, flip);
        }
        flip ^= TRUE;
        y -= 10;
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_StackedSpritesOnDefender, work);
}

enum {
    BOOMERANG_STATE_LEAN_BACK,
    BOOMERANG_STATE_LEAN_RETURN,
    BOOMERANG_STATE_THROW,
    BOOMERANG_STATE_COME_BACK,
    BOOMERANG_STATE_CATCH,
    BOOMERANG_STATE_RECOVER,
    BOOMERANG_STATE_END,
};

void BattleAnimTask_BoomerangSprite(SysTask *task, void *data) {
    BattleAnimBoomerangSpriteWork *work = data;

    switch (work->state) {
    case BOOMERANG_STATE_LEAN_BACK:
        if (BattleAnimUtil_UpdateLinearMotion(&work->lean)) {
            BattleAnimUtil_SetPokepicPosWithOffset(&work->lean, work->attackerPokepic, work->attackerX, work->attackerY);
            return;
        }
        BattleAnimUtil_InitLinearMotion(&work->lean, work->lean.x, 0, 0, 0, 2);
        work->state++;
        return;
    case BOOMERANG_STATE_LEAN_RETURN:
        if (BattleAnimUtil_UpdateLinearMotion(&work->lean)) {
            BattleAnimUtil_SetPokepicPosWithOffset(&work->lean, work->attackerPokepic, work->attackerX, work->attackerY);
            return;
        }
        Pokepic_SetAttr(work->attackerPokepic, POKEPIC_X, work->attackerX);
        Pokepic_SetAttr(work->attackerPokepic, POKEPIC_Y, work->attackerY);
        BattleAnimUtil_InitLinearMotion(&work->lean, 0, -(work->direction << 5), 0, 0, 4);
        work->state++;
        return;
    case BOOMERANG_STATE_THROW:
        if (!BattleAnimUtil_UpdateLinearSinMotionSprite(&work->arc, &work->arcHeight, work->sprite)) {
            BattleAnimUtil_InitLinearSinMotion(&work->arc, &work->arcHeight, work->defenderX, work->attackerX, work->defenderY, work->attackerY, 10, -2 << 16);
            work->state++;
        }
        break;
    case BOOMERANG_STATE_COME_BACK:
        if (!BattleAnimUtil_UpdateLinearSinMotionSprite(&work->arc, &work->arcHeight, work->sprite)) {
            ManagedSprite_SetDrawFlag(work->sprite, FALSE);
            work->state++;
        }
        break;
    case BOOMERANG_STATE_CATCH:
        if (BattleAnimUtil_UpdateLinearMotion(&work->lean)) {
            BattleAnimUtil_SetPokepicPosWithOffset(&work->lean, work->attackerPokepic, work->attackerX, work->attackerY);
        } else {
            BattleAnimUtil_InitLinearMotion(&work->lean, work->lean.x, 0, 0, 0, 4);
            work->state++;
        }
        break;
    case BOOMERANG_STATE_RECOVER:
        if (BattleAnimUtil_UpdateLinearMotion(&work->lean)) {
            BattleAnimUtil_SetPokepicPosWithOffset(&work->lean, work->attackerPokepic, work->attackerX, work->attackerY);
        } else {
            Pokepic_SetAttr(work->attackerPokepic, POKEPIC_X, work->attackerX);
            Pokepic_SetAttr(work->attackerPokepic, POKEPIC_Y, work->attackerY);
            work->state++;
        }
        break;
    case BOOMERANG_STATE_END:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_BoomerangSprite(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimBoomerangSpriteWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimBoomerangSpriteWork));
    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->direction = BattleAnimUtil_GetBattlerSideSignByMode(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->attackerPokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    work->attackerX = Pokepic_GetAttr(work->attackerPokepic, POKEPIC_X);
    work->attackerY = Pokepic_GetAttr(work->attackerPokepic, POKEPIC_Y);
    BattleAnimUtil_InitLinearMotion(&work->lean, 0, work->direction * -16, 0, 0, 4);
    work->defenderPokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(system));
    work->defenderX = Pokepic_GetAttr(work->defenderPokepic, POKEPIC_X);
    work->defenderY = Pokepic_GetAttr(work->defenderPokepic, POKEPIC_Y);
    work->sprite = sprite;
    ManagedSprite_SetAnimateFlag(work->sprite, TRUE);
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    BattleAnimUtil_InitLinearSinMotion(&work->arc, &work->arcHeight, work->attackerX, work->defenderX, work->attackerY, work->defenderY, 10, 2 << 16);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_BoomerangSprite, work);
}

enum {
    CIRCLE_FADE_STATE_CIRCLE,
    CIRCLE_FADE_STATE_DELAY,
    CIRCLE_FADE_STATE_FADE,
    CIRCLE_FADE_STATE_END,
};

void BattleAnimTask_AttackerCircleFade(SysTask *task, void *data) {
    BattleAnimWork0222DB14 *work = data;

    switch (work->state) {
    case CIRCLE_FADE_STATE_CIRCLE:
        if (BattleAnimUtil_UpdateSinMotionSprite(&work->motion, work->x, work->y - work->shadowHeight, work->sprite) == FALSE) {
            work->state++;
            work->timer = 16;
        }
        break;
    case CIRCLE_FADE_STATE_DELAY:
        work->timer--;
        if (work->timer < 0) {
            work->state++;
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 0, 16, 16, 0, 32);
        }
        break;
    case CIRCLE_FADE_STATE_FADE:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            ManagedSprite_SetDrawFlag(work->sprite, TRUE);
            Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, FALSE);
            work->state++;
        }
        break;
    case CIRCLE_FADE_STATE_END:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimFunc_AttackerCircleFade(BattleAnimSystem *system) {
    BattleAnimWork0222DB14 *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimWork0222DB14));
    int direction;

    work->system = system;
    work->spriteManager = BattleAnimSystem_GetBattlerSpriteManager(system);
    direction = BattleAnimUtil_GetBattlerSideSignByMode(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->shadowHeight = Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H);
    work->y += 8;
    work->sprite = BattleAnimSystem_GetBattlerSprite(work->system, 0);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, TRUE);
    BattleAnimUtil_InitCircleMotion(&work->motion, 2, 16);
    BattleAnimUtil_SetBlendWithBattleBg(work->system, 16, 0);
    BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 16, 0, 0, 16, 32);
    work->motion.ampX *= direction;
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerCircleFade, work);
    SpriteSystem_DrawSprites(work->spriteManager);
}

enum {
    RISING_PIC_STATE_STRETCH,
    RISING_PIC_STATE_RESTORE,
    RISING_PIC_STATE_DONE,
};

BOOL BattleAnimTask_RisingSpriteStretchAttackerPic(BattleAnimRisingSpriteStretchWork *work) {
    BOOL done = FALSE;

    switch (work->picState) {
    case RISING_PIC_STATE_STRETCH:
        if (BattleAnimUtil_UpdateScaleMotion(&work->picScale)) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->picScale.y);
        } else {
            work->picState++;
            BattleAnimUtil_InitScaleMotion(&work->picScale, 15, 10, 10, 12);
        }
        break;
    case RISING_PIC_STATE_RESTORE:
        if (BattleAnimUtil_UpdateScaleMotion(&work->picScale)) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->picScale.y);
        } else {
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, 0x100);
            work->picState++;
        }
        break;
    case RISING_PIC_STATE_DONE:
        done = TRUE;
        break;
    }
    return done;
}

enum {
    RISING_SPRITE_STATE_RISE,
    RISING_SPRITE_STATE_FADE_OUT,
    RISING_SPRITE_STATE_DONE,
};

BOOL BattleAnimTask_RisingSpriteStretchAttackerSprite(BattleAnimRisingSpriteStretchWork *work) {
    BOOL done = FALSE;

    switch (work->spriteState) {
    case RISING_SPRITE_STATE_RISE:
        BattleAnimUtil_UpdateScaleMotionSprite(&work->spriteScale, work->sprite);
        if (BattleAnimUtil_UpdateLinearMotion(&work->rise)) {
            BattleAnimUtil_SetSpritePosWithOffset(&work->rise, work->sprite, work->x, work->y);
        } else {
            work->spriteState++;
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 16, 0, 14, 16, 8);
        }
        break;
    case RISING_SPRITE_STATE_FADE_OUT:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            ManagedSprite_SetDrawFlag(work->sprite, FALSE);
            work->spriteState++;
        }
        break;
    case RISING_SPRITE_STATE_DONE:
        done = TRUE;
        break;
    }
    return done;
}

enum {
    RISING_STRETCH_STATE_RUN,
    RISING_STRETCH_STATE_END,
};

void BattleAnimTask_RisingSpriteStretchAttacker(SysTask *task, void *data) {
    BattleAnimRisingSpriteStretchWork *work = data;

    switch (work->state) {
    case RISING_STRETCH_STATE_RUN:
        BattleAnimTask_RisingSpriteStretchAttackerPic(work);
        if (BattleAnimTask_RisingSpriteStretchAttackerSprite(work)) {
            work->state++;
        }
        break;
    case RISING_STRETCH_STATE_END:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_RisingSpriteStretchAttacker(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimRisingSpriteStretchWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimRisingSpriteStretchWork));
    int dirX;
    int dirY;
    int rise;
    s16 x;
    s16 y;

    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    dirX = BattleAnimUtil_GetBattlerSideSignByMode(work->system, BattleAnimSystem_GetAttacker(work->system));
    dirY = BattleAnimUtil_GetBattlerSideSign(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    x = (s16)Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    y = (s16)Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    BattleAnimUtil_InitScaleMotion(&work->picScale, 10, 10, 15, 12);
    work->sprite = sprite;
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    work->x = (s16)(x + (dirX << 5));
    work->y = y;
    if (dirY < 0) {
        rise = -8;
    } else {
        rise = -24;
    }
    BattleAnimUtil_InitLinearMotion(&work->rise, 0, 0, 0, (s16)(rise * dirY), 32);
    BattleAnimUtil_InitScaleMotion(&work->spriteScale, 5, 10, 12, 32);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 31, 26);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_RisingSpriteStretchAttacker, work);
}

void BattleAnimTask_ZigzagSlashSetLeg(BattleAnimZigzagSlashWork *work) {
    switch (work->legIndex) {
    case 0:
        BattleAnimUtil_InitLinearMotion(&work->leg, 0, 40, 0, 40, 8);
        break;
    case 1:
        BattleAnimUtil_InitLinearMotion(&work->leg, 0, 0, 0, -80, 8);
        break;
    case 2:
        BattleAnimUtil_InitLinearMotion(&work->leg, 0, -80, 0, 80, 8);
        break;
    case 3:
        BattleAnimUtil_InitLinearMotion(&work->leg, 0, 0, 0, -80, 8);
        break;
    case 4:
        BattleAnimUtil_InitLinearMotion(&work->leg, 0, 80, 0, 80, 8);
        break;
    case 5:
        BattleAnimUtil_InitLinearMotion(&work->leg, 0, -40, 0, -40, 8);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

enum {
    ZIGZAG_SLASH_STATE_NEXT_LEG,
    ZIGZAG_SLASH_STATE_MOVE,
    ZIGZAG_SLASH_STATE_FADE_OUT,
    ZIGZAG_SLASH_STATE_RESTORE_PIC,
    ZIGZAG_SLASH_STATE_END,
};

void BattleAnimTask_ZigzagSlash(SysTask *task, void *data) {
    BattleAnimZigzagSlashWork *work = data;

    switch (work->state) {
    case ZIGZAG_SLASH_STATE_NEXT_LEG:
        if (++work->timer > 4) {
            BattleAnimTask_ZigzagSlashSetLeg(work);
            work->state++;
            work->timer = 0;
        }
        break;
    case ZIGZAG_SLASH_STATE_MOVE:
        if (BattleAnimUtil_UpdateLinearMotion(&work->leg)) {
            BattleAnimUtil_SetSpritePosWithOffset(&work->leg, work->sprite, work->x, work->y);
        } else if (++work->legIndex < 6) {
            work->state = ZIGZAG_SLASH_STATE_NEXT_LEG;
            work->x += work->leg.x;
            work->y += work->leg.y;
        } else {
            work->state++;
            ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 16, 0, 0, 16, 16);
            Pokepic_StartPaletteFade(work->pokepic, 0, 10, 0, 0x7FFF);
        }
        break;
    case ZIGZAG_SLASH_STATE_FADE_OUT:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            ManagedSprite_SetDrawFlag(work->sprite, FALSE);
        }
        if (!Pokepic_ResumePaletteFade(work->pokepic)) {
            work->state++;
            Pokepic_StartPaletteFade(work->pokepic, 10, 0, 0, 0x7FFF);
        }
        break;
    case ZIGZAG_SLASH_STATE_RESTORE_PIC:
        if (!Pokepic_ResumePaletteFade(work->pokepic)) {
            work->state++;
        }
        break;
    case ZIGZAG_SLASH_STATE_END:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_ZigzagSlash(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimZigzagSlashWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimZigzagSlashWork));
    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(system));
    work->sprite = sprite;
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    ManagedSprite_GetPositionXY(work->sprite, &work->x, &work->y);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_ZigzagSlash, work);
}

void BattleAnimTask_ZigzagFlashSetLeg(BattleAnimZigzagFlashWork *work) {
    switch (work->legIndex) {
    case 0:
        BattleAnimUtil_InitLinearMotion(&work->leg, 0, 80, 0, 80, 4);
        break;
    case 1:
        BattleAnimUtil_InitLinearMotion(&work->leg, 0, 0, 0, -80, 4);
        break;
    case 2:
        BattleAnimUtil_InitLinearMotion(&work->leg, 0, -80, 0, 80, 4);
        break;
    case 3:
        BattleAnimUtil_InitLinearMotion(&work->leg, 0, 40, 0, -40, 4);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

enum {
    ZIGZAG_FLASH_STATE_NEXT_LEG,
    ZIGZAG_FLASH_STATE_MOVE,
    ZIGZAG_FLASH_STATE_DELAY,
    ZIGZAG_FLASH_STATE_FLASH,
    ZIGZAG_FLASH_STATE_RESTORE,
    ZIGZAG_FLASH_STATE_BLINK,
    ZIGZAG_FLASH_STATE_END,
};

void BattleAnimTask_ZigzagFlash(SysTask *task, void *data) {
    BattleAnimZigzagFlashWork *work = data;
    int i;
    PaletteData *paletteData;

    switch (work->state) {
    case ZIGZAG_FLASH_STATE_NEXT_LEG:
        if (++work->timer <= 1) {
            break;
        }
        BattleAnimTask_ZigzagFlashSetLeg(work);
        work->state++;
        work->timer = 0;
        break;
    case ZIGZAG_FLASH_STATE_MOVE:
        if (BattleAnimUtil_UpdateLinearMotion(&work->leg)) {
            BattleAnimUtil_SetSpritePosWithOffset(&work->leg, work->sprite, work->x, work->y);
            break;
        }
        if (++work->legIndex < 4) {
            work->state = ZIGZAG_FLASH_STATE_NEXT_LEG;
            work->x += work->leg.x;
            work->y += work->leg.y;
        } else {
            work->state++;
            work->timer = 8;
            ManagedSprite_SetAnim(work->sprite, 1);
            ManagedSprite_SetAnimateFlag(work->sprite, TRUE);
        }
        break;
    case ZIGZAG_FLASH_STATE_DELAY:
        if (--work->timer >= 0) {
            break;
        }
        work->state++;
        paletteData = BattleAnimSystem_GetPaletteData(work->system);
        PaletteData_BeginPaletteFade(paletteData, PLTTBUF_MAIN_BG_F, (u16)BattleAnimSystem_GetBattleBgPaletteMask(work->system), -8, 0, 16, 0xFFFF);
        for (i = 0; i < 4; i++) {
            if (work->pokepics[i] != NULL) {
                Pokepic_StartPaletteFade(work->pokepics[i], 14, 16, 0, 0xFFFF);
            }
        }
        break;
    case ZIGZAG_FLASH_STATE_FLASH:
        if (PaletteData_GetSelectedBuffersBitmask(BattleAnimSystem_GetPaletteData(work->system))) {
            break;
        }
        work->state++;
        paletteData = BattleAnimSystem_GetPaletteData(work->system);
        PaletteData_BeginPaletteFade(paletteData, PLTTBUF_MAIN_BG_F, (u16)BattleAnimSystem_GetBattleBgPaletteMask(work->system), -8, 16, 0, 0xFFFF);
        for (i = 0; i < 4; i++) {
            if (work->pokepics[i] != NULL) {
                Pokepic_StartPaletteFade(work->pokepics[i], 2, 0, 0, 0xFFFF);
            }
        }
        break;
    case ZIGZAG_FLASH_STATE_RESTORE:
        if (PaletteData_GetSelectedBuffersBitmask(BattleAnimSystem_GetPaletteData(work->system))) {
            break;
        }
        work->state++;
        work->timer = 4;
        work->legIndex = 4;
        work->draw = FALSE;
        break;
    case ZIGZAG_FLASH_STATE_BLINK:
        if (--work->timer >= 0) {
            break;
        }
        work->timer = 4;
        if (--work->legIndex < 0) {
            work->state++;
            break;
        }
        ManagedSprite_SetDrawFlag(work->sprite, work->draw);
        work->draw ^= TRUE;
        break;
    case ZIGZAG_FLASH_STATE_END:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_ZigzagFlash(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimZigzagFlashWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimZigzagFlashWork));
    int i;

    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(system));
    for (i = 0; i < 4; i++) {
        work->pokepics[i] = BattleAnimSystem_GetBattlerPokepic(work->system, i);
    }
    work->sprite = sprite;
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    ManagedSprite_GetPositionXY(work->sprite, &work->x, &work->y);
    work->x -= 40;
    work->y -= 40;
    ManagedSprite_SetPositionXY(work->sprite, work->x, work->y);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    work->timer = -16;
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 31, 26);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_ZigzagFlash, work);
}

void BattleAnimTask_WaddlePicStartStep(BattleAnimWaddlePicWork *work, BOOL flip) {
    BattleAnimUtil_InitValueMotionFx(&work->tilt, 0, 0xAAA, 3);
    if (flip != FALSE) {
        work->tilt.step = work->tilt.step * ~0u;
        Pokepic_SetAttr(work->pokepic, POKEPIC_XPIVOT, 16);
    } else {
        Pokepic_SetAttr(work->pokepic, POKEPIC_XPIVOT, -16);
    }
    work->stepState = 0;
    work->stepTimer = 0;
}

enum {
    WADDLE_STEP_STATE_TILT,
    WADDLE_STEP_STATE_HOP,
    WADDLE_STEP_STATE_LAND,
    WADDLE_STEP_STATE_DONE,
};

BOOL BattleAnimTask_WaddlePicUpdateStep(BattleAnimWaddlePicWork *work) {
    BOOL done = FALSE;

    switch (work->stepState) {
    case WADDLE_STEP_STATE_TILT:
        BattleAnimUtil_UpdateValueMotionFx(&work->tilt);
        Pokepic_SetAttr(work->pokepic, POKEPIC_ZROT, work->tilt.value);
        if (--work->stepTimer < 0) {
            work->stepState++;
            BattleAnimUtil_InitLinearMotion(&work->hop, work->x, work->x, work->y, (s16)(work->y + 2), 2);
        }
        break;
    case WADDLE_STEP_STATE_HOP:
        if (BattleAnimUtil_UpdateValueMotionFx(&work->tilt)) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_ZROT, work->tilt.value);
        }
        if (!BattleAnimUtil_UpdateLinearMotionPokepic(&work->hop, work->pokepic)) {
            work->stepState++;
            BattleAnimUtil_InitValueMotionFx(&work->tilt, (s16)work->tilt.value, 0, 3);
            BattleAnimUtil_InitLinearMotion(&work->hop, work->x, work->x, (s16)(work->y + 2), work->y, 2);
        }
        break;
    case WADDLE_STEP_STATE_LAND:
        BattleAnimUtil_UpdateLinearMotionPokepic(&work->hop, work->pokepic);
        if (BattleAnimUtil_UpdateValueMotionFx(&work->tilt)) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_ZROT, work->tilt.value);
        } else {
            Pokepic_SetAttr(work->pokepic, POKEPIC_X, work->x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y);
            Pokepic_SetAttr(work->pokepic, POKEPIC_ZROT, done);
            work->stepState++;
        }
        break;
    case WADDLE_STEP_STATE_DONE:
        done = TRUE;
        break;
    }
    return done;
}

enum {
    WADDLE_STATE_STEP,
    WADDLE_STATE_END,
};

void BattleAnimTask_WaddlePic(SysTask *task, void *data) {
    BattleAnimWaddlePicWork *work = data;

    switch (work->state) {
    case WADDLE_STATE_STEP:
        if (BattleAnimTask_WaddlePicUpdateStep(work)) {
            work->steps++;
            if (work->steps < 4) {
                work->direction ^= TRUE;
                BattleAnimTask_WaddlePicStartStep(work, work->direction);
                BattleAnimTask_WaddlePicUpdateStep(work);
            } else {
                work->state++;
            }
        }
        break;
    case WADDLE_STATE_END:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

void BattleAnimFunc_WaddlePic(BattleAnimSystem *system) {
    BattleAnimWaddlePicWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimWaddlePicWork));
    int direction;

    work->system = system;
    direction = BattleAnimUtil_GetBattlerSideSignByMode(work->system, BattleAnimSystem_GetAttacker(system));
    if ((s32)BattleAnimSystem_GetArg(system, 0) == 0) {
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
        work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
        work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    } else {
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(system));
        work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
        work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    }
    Pokepic_SetAttr(work->pokepic, POKEPIC_YPIVOT, 50);
    if (direction == 1) {
        work->direction = FALSE;
    } else {
        work->direction = TRUE;
    }
    BattleAnimTask_WaddlePicStartStep(work, work->direction);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_WaddlePic, work);
}

enum {
    STRETCH_UP_STEP_STATE_STRETCH,
    STRETCH_UP_STEP_STATE_HOLD,
    STRETCH_UP_STEP_STATE_SHRINK,
    STRETCH_UP_STEP_STATE_RESET,
};

BOOL BattleAnimTask_AttackerStretchUpStep(BattleAnimAttackerStretchUpWork *work) {
    BOOL done = FALSE;

    switch (work->stepState) {
    case STRETCH_UP_STEP_STATE_STRETCH:
        if (BattleAnimUtil_UpdateScaleMotionPokepic(&work->scale, work->pokepic)) {
            BattleAnimUtil_AlignPokepicToScale(work->pokepic, (s16)work->y, (s16)work->height, work->scale.curY, 0);
        } else {
            work->stepState++;
        }
        break;
    case STRETCH_UP_STEP_STATE_HOLD:
        if (--work->delay < 0) {
            work->stepState++;
            BattleAnimUtil_InitScaleMotion(&work->scale, 15, 10, 10, 8);
        }
        break;
    case STRETCH_UP_STEP_STATE_SHRINK:
        if (BattleAnimUtil_UpdateScaleMotionPokepic(&work->scale, work->pokepic)) {
            BattleAnimUtil_AlignPokepicToScale(work->pokepic, (s16)work->y, (s16)work->height, work->scale.curY, 0);
        } else {
            work->stepState++;
        }
        break;
    case STRETCH_UP_STEP_STATE_RESET:
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, 0x100);
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, 0x100);
        done = TRUE;
        break;
    }
    return done;
}

enum {
    STRETCH_UP_STATE_STRETCH,
    STRETCH_UP_STATE_DELAY,
    STRETCH_UP_STATE_END,
};

void BattleAnimTask_AttackerStretchUp(SysTask *task, void *data) {
    BattleAnimAttackerStretchUpWork *work = data;

    switch (work->state) {
    case STRETCH_UP_STATE_STRETCH:
        if (BattleAnimTask_AttackerStretchUpStep(work)) {
            work->state++;
            work->timer = 16;
        }
        break;
    case STRETCH_UP_STATE_DELAY:
        work->timer--;
        if (work->timer < 0) {
            work->state++;
        }
        break;
    case STRETCH_UP_STATE_END:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

void BattleAnimFunc_AttackerStretchUp(BattleAnimSystem *system) {
    BattleAnimAttackerStretchUpWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimAttackerStretchUpWork));
    work->system = system;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    BattleAnimUtil_InitScaleMotion(&work->scale, 10, 10, 15, 8);
    work->delay = 4;
    work->height = BattleAnimSystem_GetBattlerHeight(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerStretchUp, work);
}

void BattleAnimTask_PulseSpriteBrightnessFlashStartPulse(BattleAnimPulseSpriteWork *work) {
    BattleAnimUtil_InitScaleMotion(&work->scale, (s16)(work->scaleA / 100), 10, (s16)(work->scaleB / 100), work->duration / 100);
    BattleAnimUtil_SetBlendWithBattleBg(work->system, 16, 0);
    BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 16, 10, 0, 6, (work->duration / 100) * 2);
    work->pulseState = 0;
    work->pulseCount = 0;
}

enum {
    PULSE_STATE_GROW,
    PULSE_STATE_SHRINK,
    PULSE_STATE_WAIT_BLEND,
    PULSE_STATE_DONE,
};

BOOL BattleAnimTask_PulseSpriteBrightnessFlashUpdatePulse(BattleAnimPulseSpriteWork *work) {
    BOOL done = FALSE;

    switch (work->pulseState) {
    case PULSE_STATE_GROW:
        if (!BattleAnimUtil_UpdateScaleMotionSprite(&work->scale, work->sprite)) {
            BattleAnimUtil_InitScaleMotion(&work->scale, (s16)(work->scaleB / 100), 10, (s16)(work->scaleA / 100), work->duration / 100);
            work->pulseState++;
        }
        break;
    case PULSE_STATE_SHRINK:
        if (!BattleAnimUtil_UpdateScaleMotionSprite(&work->scale, work->sprite)) {
            work->pulseCount++;
            if (work->pulseCount < 2) {
                work->pulseState = PULSE_STATE_GROW;
                BattleAnimUtil_InitScaleMotion(&work->scale, (s16)(work->scaleA / 100), 10, (s16)(work->scaleB / 100), work->duration / 100);
            } else {
                work->pulseState++;
            }
        }
        break;
    case PULSE_STATE_WAIT_BLEND:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            done = TRUE;
            work->pulseState++;
        }
        break;
    case PULSE_STATE_DONE:
        done = TRUE;
        break;
    }
    return done;
}

enum {
    PULSE_ROUND_STATE_START,
    PULSE_ROUND_STATE_UPDATE,
    PULSE_ROUND_STATE_DONE,
};

BOOL BattleAnimTask_PulseSpriteBrightnessFlashUpdatePulses(BattleAnimPulseSpriteWork *work) {
    BOOL done = FALSE;

    switch (work->roundState) {
    case PULSE_ROUND_STATE_START:
        BattleAnimTask_PulseSpriteBrightnessFlashStartPulse(work);
        BattleAnimTask_PulseSpriteBrightnessFlashUpdatePulse(work);
        work->roundState++;
        break;
    case PULSE_ROUND_STATE_UPDATE:
        if (BattleAnimTask_PulseSpriteBrightnessFlashUpdatePulse(work)) {
            work->repeats--;
            if (work->repeats >= 0) {
                work->duration -= work->duration / 3;
                work->scaleA -= work->scaleA / 3;
                work->scaleB -= work->scaleB / 3;
                work->roundState = PULSE_ROUND_STATE_START;
            } else {
                done = TRUE;
                work->roundState++;
            }
        }
        break;
    case PULSE_ROUND_STATE_DONE:
        done = TRUE;
        break;
    }
    return done;
}

enum {
    BRIGHTNESS_FLASH_STATE_PULSE,
    BRIGHTNESS_FLASH_STATE_FLASH,
    BRIGHTNESS_FLASH_STATE_WAIT_BRIGHTNESS,
    BRIGHTNESS_FLASH_STATE_FADE_OUT,
    BRIGHTNESS_FLASH_STATE_END,
};

static void BattleAnimTask_PulseSpriteBrightnessFlash(SysTask *task, void *data) {
    BattleAnimPulseSpriteWork *work = data;

    switch (work->state) {
    case BRIGHTNESS_FLASH_STATE_PULSE:
        if (BattleAnimTask_PulseSpriteBrightnessFlashUpdatePulses(work)) {
            work->state++;
            work->timer = 30;
            ManagedSprite_SetAnim(work->sprite, 0);
            ManagedSprite_SetAnimateFlag(work->sprite, TRUE);
            ManagedSprite_SetAffineScale(work->sprite, 1.0f, 1.0f);
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 0, 16, 16, 0, 4);
        }
        break;
    case BRIGHTNESS_FLASH_STATE_FLASH:
        if (work->timer == 0) {
            StartBrightnessTransition(8, 16, 0, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3), SCREEN_MASK_MAIN);
            work->timer = -1;
        } else if (work->timer > 0) {
            work->timer--;
        }
        if (work->timer == -1 && IsBrightnessTransitionActive(SCREEN_MASK_MAIN)) {
            StartBrightnessTransition(8, 0, 16, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3), SCREEN_MASK_MAIN);
            work->timer = -2;
        }
        if (!ManagedSprite_IsAnimated(work->sprite)) {
            work->state++;
        }
        break;
    case BRIGHTNESS_FLASH_STATE_WAIT_BRIGHTNESS:
        if (IsBrightnessTransitionActive(SCREEN_MASK_MAIN)) {
            work->state++;
            BattleAnimUtil_SetBlendWithBattleBg(work->system, 16, 0);
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 16, 0, 0, 16, 8);
        }
        break;
    case BRIGHTNESS_FLASH_STATE_FADE_OUT:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            ManagedSprite_SetDrawFlag(work->sprite, FALSE);
            work->state++;
        }
        break;
    case BRIGHTNESS_FLASH_STATE_END:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_PulseSpriteBrightnessFlash(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimPulseSpriteWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimPulseSpriteWork));
    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->sprite = sprite;
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    ManagedSprite_SetAnimationFrame(work->sprite, 4);
    work->repeats = 3;
    work->duration = 400;
    work->scaleA = 1500;
    work->scaleB = 1000;
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_PulseSpriteBrightnessFlash, work);
}

void BattleAnimTask_AttackerDipSetLeg(BattleAnimAttackerDipWork *work) {
    switch (work->leg) {
    case 0:
    case 2:
        BattleAnimUtil_InitLinearMotion(&work->move, 0, 0, 0, 32, 6);
        break;
    case 1:
    case 3:
        BattleAnimUtil_InitLinearMotion(&work->move, 0, 0, 32, 0, 6);
        break;
    }
}

enum {
    DIP_STATE_NEXT_LEG,
    DIP_STATE_MOVE,
    DIP_STATE_END,
};

static void BattleAnimTask_AttackerDip(SysTask *task, void *data) {
    BattleAnimAttackerDipWork *work = data;

    switch (work->state) {
    case DIP_STATE_NEXT_LEG:
        BattleAnimTask_AttackerDipSetLeg(work);
        work->state++;
        break;
    case DIP_STATE_MOVE:
        if (BattleAnimUtil_UpdateLinearMotion(&work->move)) {
            BattleAnimUtil_SetPokepicPosWithOffset(&work->move, work->pokepic, work->x, work->y);
            break;
        }
        work->leg++;
        if (work->leg < 4) {
            work->state = DIP_STATE_NEXT_LEG;
        } else {
            work->state++;
        }
        break;
    case DIP_STATE_END:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

void BattleAnimFunc_AttackerDip(BattleAnimSystem *system) {
    BattleAnimAttackerDipWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimAttackerDipWork));
    work->system = system;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->leg = 0;
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerDip, work);
}

enum {
    LUNGE_KNOCKBACK_STATE_INIT,
    LUNGE_KNOCKBACK_STATE_CIRCLE,
    LUNGE_KNOCKBACK_STATE_LUNGE,
    LUNGE_KNOCKBACK_STATE_KNOCKBACK,
    LUNGE_KNOCKBACK_STATE_ATTACKER_RETURN,
    LUNGE_KNOCKBACK_STATE_DEFENDER_RETURN,
    LUNGE_KNOCKBACK_STATE_END,
};

static void BattleAnimTask_AttackerLungeKnockback(SysTask *task, void *data) {
    BattleAnimLungeKnockbackWork *work = data;

    switch (work->state) {
    case LUNGE_KNOCKBACK_STATE_INIT:
        BattleAnimUtil_InitCircleMotion(&work->attackerMove.sin, 1, 64);
        work->attackerMove.sin.ampX *= work->direction;
        work->state++;
        break;
    case LUNGE_KNOCKBACK_STATE_CIRCLE:
        if (BattleAnimUtil_UpdateSinMotionPokepic(&work->attackerMove.sin, work->attackerX, (s16)(work->attackerY + 8), work->attackerPokepic)) {
            break;
        }
        Pokepic_SetAttr(work->attackerPokepic, POKEPIC_X, work->attackerX);
        Pokepic_SetAttr(work->attackerPokepic, POKEPIC_Y, work->attackerY);
        BattleAnimUtil_InitLinearMotion(&work->attackerMove.linear, work->attackerX, work->attackerX + 32, 0, 0, 2);
        work->attackerMove.linear.stepX *= work->direction;
        work->state++;
        BattleAnimUtil_UpdateLinearMotion(&work->attackerMove.linear);
        Pokepic_SetAttr(work->attackerPokepic, POKEPIC_X, work->attackerMove.linear.x);
        break;
    case LUNGE_KNOCKBACK_STATE_LUNGE:
        if (BattleAnimUtil_UpdateLinearMotion(&work->attackerMove.linear)) {
            Pokepic_SetAttr(work->attackerPokepic, POKEPIC_X, work->attackerMove.linear.x);
            break;
        }
        BattleAnimUtil_InitLinearMotion(&work->defenderMove, work->defenderX, work->defenderX + (work->direction << 5), 0, 0, 2);
        work->state++;
        break;
    case LUNGE_KNOCKBACK_STATE_KNOCKBACK:
        if (BattleAnimUtil_UpdateLinearMotion(&work->defenderMove)) {
            Pokepic_SetAttr(work->defenderPokepic, POKEPIC_X, work->defenderMove.x);
            break;
        }
        BattleAnimUtil_InitLinearMotion(&work->attackerMove.linear, work->attackerMove.linear.x, work->attackerX, 0, 0, 8);
        work->state++;
        break;
    case LUNGE_KNOCKBACK_STATE_ATTACKER_RETURN:
        if (BattleAnimUtil_UpdateLinearMotion(&work->attackerMove.linear)) {
            Pokepic_SetAttr(work->attackerPokepic, POKEPIC_X, work->attackerMove.linear.x);
            break;
        }
        Pokepic_SetAttr(work->attackerPokepic, POKEPIC_X, work->attackerX);
        BattleAnimUtil_InitLinearMotion(&work->defenderMove, work->defenderMove.x, work->defenderX, 0, 0, 8);
        work->state++;
        break;
    case LUNGE_KNOCKBACK_STATE_DEFENDER_RETURN:
        if (BattleAnimUtil_UpdateLinearMotion(&work->defenderMove)) {
            Pokepic_SetAttr(work->defenderPokepic, POKEPIC_X, work->defenderMove.x);
            break;
        }
        Pokepic_SetAttr(work->defenderPokepic, POKEPIC_X, work->defenderX);
        work->state++;
        break;
    case LUNGE_KNOCKBACK_STATE_END:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

void BattleAnimFunc_AttackerLungeKnockback(BattleAnimSystem *system) {
    BattleAnimLungeKnockbackWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimLungeKnockbackWork));
    work->system = system;
    work->direction = BattleAnimUtil_GetBattlerSideSignByMode(work->system, BattleAnimSystem_GetAttacker(system));
    work->attackerPokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    work->attackerX = Pokepic_GetAttr(work->attackerPokepic, POKEPIC_X);
    work->attackerY = Pokepic_GetAttr(work->attackerPokepic, POKEPIC_Y);
    work->defenderPokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(system));
    work->defenderX = Pokepic_GetAttr(work->defenderPokepic, POKEPIC_X);
    work->defenderY = Pokepic_GetAttr(work->defenderPokepic, POKEPIC_Y);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerLungeKnockback, work);
}

void BattleAnimTask_FanSpritesStartAttackerPulse(BattleAnimFanSpritesWork *work) {
    work->pulseState = 0;
    BattleAnimUtil_InitScaleMotionXY(&work->pulse, 10, 9, 10, 8, 10, 2);
}

enum {
    FAN_PULSE_STATE_WIDEN,
    FAN_PULSE_STATE_STRETCH,
    FAN_PULSE_STATE_RESTORE,
    FAN_PULSE_STATE_DONE,
};

BOOL BattleAnimTask_FanSpritesUpdateAttackerPulse(BattleAnimFanSpritesWork *work) {
    BOOL done = FALSE;

    switch (work->pulseState) {
    case FAN_PULSE_STATE_WIDEN:
        if (BattleAnimUtil_UpdateScaleMotionXYPokepic(&work->pulse, work->pokepic)) {
            BattleAnimUtil_AlignPokepicToScale(work->pokepic, work->y, work->height, work->pulse.curY, 0);
        } else {
            work->pulseState++;
            BattleAnimUtil_InitScaleMotionXY(&work->pulse, 9, 11, 8, 15, 10, 2);
        }
        break;
    case FAN_PULSE_STATE_STRETCH:
        if (BattleAnimUtil_UpdateScaleMotionXYPokepic(&work->pulse, work->pokepic)) {
            BattleAnimUtil_AlignPokepicToScale(work->pokepic, work->y, work->height, work->pulse.curY, 0);
        } else {
            work->pulseState++;
            BattleAnimUtil_InitScaleMotionXY(&work->pulse, 11, 10, 15, 10, 10, 2);
        }
        break;
    case FAN_PULSE_STATE_RESTORE:
        if (BattleAnimUtil_UpdateScaleMotionXYPokepic(&work->pulse, work->pokepic)) {
            BattleAnimUtil_AlignPokepicToScale(work->pokepic, work->y, work->height, work->pulse.curY, 0);
        } else {
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, 0x100);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, 0x100);
            work->pulseState++;
            done = TRUE;
        }
        break;
    case FAN_PULSE_STATE_DONE:
        done = TRUE;
        break;
    }
    return done;
}

void BattleAnimUtil_StartSpriteAnim(ManagedSprite *sprite, u32 *state, fx32 speed) {
    *state = 0;
    ManagedSprite_SetDrawPriority(sprite, 100);
    ManagedSprite_SetPriority(sprite, 1);
    ManagedSprite_SetDrawFlag(sprite, TRUE);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    ManagedSprite_SetAnimSpeed(sprite, speed);
    ManagedSprite_SetAnim(sprite, 0);
    ManagedSprite_SetAnimationFrame(sprite, 0);
    ManagedSprite_TickNFrames(sprite, FX32_CONST(55));
}

enum {
    SPRITE_ANIM_STATE_RUNNING,
    SPRITE_ANIM_STATE_DONE,
};

BOOL BattleAnimUtil_IsSpriteAnimDone(ManagedSprite *sprite, u32 *state) {
    BOOL done = FALSE;

    switch (*state) {
    case SPRITE_ANIM_STATE_RUNNING:
        if (!ManagedSprite_IsAnimated(sprite)) {
            done = TRUE;
            (*state)++;
        }
        break;
    case SPRITE_ANIM_STATE_DONE:
        done = TRUE;
        break;
    default:
        done = TRUE;
        break;
    }
    return done;
}

enum {
    FAN_SPRITES_STATE_START_SPRITE,
    FAN_SPRITES_STATE_WAIT_SPRITE,
    FAN_SPRITES_STATE_DELAY,
    FAN_SPRITES_STATE_START_EXIT,
    FAN_SPRITES_STATE_WAIT_EXIT,
    FAN_SPRITES_STATE_END,
};

void BattleAnimTask_FanSpritesAroundAttacker(SysTask *task, void *data) {
    BattleAnimFanSpritesWork *work = data;
    int i;

    switch (work->state) {
    case FAN_SPRITES_STATE_START_SPRITE:
        BattleAnimTask_FanSpritesStartAttackerPulse(work);
        BattleAnimUtil_StartSpriteAnim(work->sprites[work->index], &work->animStates[work->index], FX32_CONST(2));
        work->state++;
        break;
    case FAN_SPRITES_STATE_WAIT_SPRITE: {
        BOOL pulseDone = BattleAnimTask_FanSpritesUpdateAttackerPulse(work);
        BOOL animDone = BattleAnimUtil_IsSpriteAnimDone(work->sprites[work->index], &work->animStates[work->index]);
        if (pulseDone == animDone && pulseDone == TRUE) {
            ManagedSprite_SetAnim(work->sprites[work->index], 1);
            work->state++;
        }
        break;
    }
    case FAN_SPRITES_STATE_DELAY:
        work->timers[work->index]--;
        if (work->timers[work->index] < 0) {
            work->index++;
            if (work->index < 6) {
                work->state = FAN_SPRITES_STATE_START_SPRITE;
            } else {
                work->state++;
            }
        }
        break;
    case FAN_SPRITES_STATE_START_EXIT:
        for (i = 0; i < 6; i++) {
            BattleAnimUtil_StartSpriteAnim(work->sprites[i], &work->animStates[i], FX32_CONST(2));
        }
        work->state++;
        break;
    case FAN_SPRITES_STATE_WAIT_EXIT: {
        BOOL done = FALSE;
        for (i = 0; i < 6; i++) {
            done = BattleAnimUtil_IsSpriteAnimDone(work->sprites[i], &work->animStates[i]);
        }
        if (done == TRUE) {
            work->state++;
        }
        break;
    }
    case FAN_SPRITES_STATE_END:
        for (i = 0; i < 6; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_FanSpritesAroundAttacker(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimFanSpritesWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimFanSpritesWork));
    s16 x;
    s16 y;
    s16 dx;
    s16 dy;
    ManagedSpriteTemplate template;
    int i;

    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(work->system));
    x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->y = y;
    work->height = BattleAnimSystem_GetBattlerHeight(work->system, BattleAnimSystem_GetAttacker(work->system));
    BattleAnimSystem_GetSpriteTemplate(&template, work->system);
    for (i = 0; i < 6; i++) {
        int angle;
        if (i == 0) {
            work->sprites[i] = sprite;
        } else {
            work->sprites[i] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
        }
        ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
        angle = (i / 2) * 0x1555;
        dx = FX_Mul(FX_CosIdx(angle), FX32_CONST(48)) >> FX32_SHIFT;
        dy = FX_Mul(FX_SinIdx(angle), FX32_CONST(48)) >> FX32_SHIFT;
        if (i % 2 == 0) {
            dx = x + dx;
            ManagedSprite_SetFlipMode(work->sprites[i], 1);
        } else {
            dx = x - dx;
        }
        dy = y - dy;
        ManagedSprite_SetPositionXY(work->sprites[i], dx, dy);
        work->timers[i] = i < 2 ? 8 : 0;
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_FanSpritesAroundAttacker, work);
}

enum {
    SHRINK_INTO_STATE_START_ANIM,
    SHRINK_INTO_STATE_WAIT_ANIM,
    SHRINK_INTO_STATE_SHRINK,
    SHRINK_INTO_STATE_WAIT_CLOSE_ANIM,
    SHRINK_INTO_STATE_LEAVE,
    SHRINK_INTO_STATE_END,
};

void BattleAnimTask_ShrinkAttackerIntoSprite(SysTask *task, void *data) {
    BattleAnimShrinkIntoSpriteWork *work = data;

    switch (work->state) {
    case SHRINK_INTO_STATE_START_ANIM:
        ManagedSprite_SetAnimateFlag(work->sprite, TRUE);
        work->state++;
        break;
    case SHRINK_INTO_STATE_WAIT_ANIM:
        if (!ManagedSprite_IsAnimated(work->sprite)) {
            ManagedSprite_SetAnimateFlag(work->sprite, FALSE);
            work->state++;
            BattleAnimUtil_InitScaleMotion(&work->scale, 10, 10, 0, 8);
            int x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
            int y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
            BattleAnimUtil_InitLinearMotion(&work->move, x, work->x, y, work->y, 8);
        }
        break;
    case SHRINK_INTO_STATE_SHRINK:
        BattleAnimUtil_UpdateLinearMotionPokepic(&work->move, work->pokepic);
        if (!BattleAnimUtil_UpdateScaleMotionPokepic(&work->scale, work->pokepic)) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, TRUE);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, 0x100);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, 0x100);
            ManagedSprite_SetAnim(work->sprite, 1);
            ManagedSprite_SetAnimSpeed(work->sprite, FX32_ONE);
            ManagedSprite_SetAnimateFlag(work->sprite, TRUE);
            work->state++;
        }
        break;
    case SHRINK_INTO_STATE_WAIT_CLOSE_ANIM:
        if (!ManagedSprite_IsAnimated(work->sprite)) {
            ManagedSprite_SetAnimateFlag(work->sprite, FALSE);
            work->state++;
            BattleAnimUtil_InitLinearMotion(&work->leave, 0, 0, work->y, 0, 8);
        }
        break;
    case SHRINK_INTO_STATE_LEAVE:
        if (BattleAnimUtil_UpdateLinearMotion(&work->leave)) {
            ManagedSprite_SetPositionXY(work->sprite, work->x, work->leave.y);
        } else {
            work->state++;
        }
        break;
    case SHRINK_INTO_STATE_END:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_ShrinkAttackerIntoSprite(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimShrinkIntoSpriteWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimShrinkIntoSpriteWork));
    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, FALSE);
    work->x = BattleAnimSystem_GetBattlerScreenCoord(system, BattleAnimSystem_GetAttacker(system), 0);
    work->y = BattleAnimSystem_GetBattlerScreenCoord(system, BattleAnimSystem_GetAttacker(system), 1);
    work->sprite = sprite;
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    ManagedSprite_SetPositionXY(work->sprite, work->x, work->y);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_ShrinkAttackerIntoSprite, work);
}

void BattleAnimTask_DarkSpriteJumpDarkenSprite(BattleAnimDarkSpriteJumpWork *work, ManagedSprite *sprite) {
    u32 mask = 1 << BattleAnimUtil_GetSpritePaletteSlot(sprite);
    PaletteData_BlendPalettes(work->paletteData, PLTTBUF_MAIN_OBJ, (u16)mask, 8, 0);
}

void BattleAnimTask_DarkSpriteJumpStartRise(BattleAnimDarkSpriteJumpWork *work) {
    f32 x;
    f32 y;

    BattleAnimUtil_InitScaleMotionXY(&work->stretch, 10, 10, 10, 20, 10, 8);
    BattleAnimUtil_InitScaleMotion(&work->scale, 10, 10, 1, 4);
    work->partState = 0;
    work->partTimer = 6;
    ManagedSprite_SetDrawFlag(work->sprites[2], gBattleAnimDarkSpriteDrawFlags[BattleAnimUtil_GetBattlerType(work->system, BattleAnimSystem_GetAttacker(work->system))]);
    ManagedSprite_SetDrawFlag(work->sprites[0], TRUE);
    BattleAnimUtil_GetScaleFloat(&work->stretch, &x, &y);
    if (work->flipRise == TRUE) {
        x = 0.0f - x;
    }
    ManagedSprite_SetAffineScale(work->sprites[0], x, y);
    BattleAnimTask_DarkSpriteJumpDarkenSprite(work, work->sprites[0]);
}

enum {
    JUMP_RISE_STATE_STRETCH,
    JUMP_RISE_STATE_SCALE,
    JUMP_RISE_STATE_DONE,
};

BOOL BattleAnimTask_DarkSpriteJumpUpdateRise(BattleAnimDarkSpriteJumpWork *work) {
    f32 x;
    f32 y;
    BOOL done = FALSE;

    switch (work->partState) {
    case JUMP_RISE_STATE_STRETCH:
        BattleAnimUtil_UpdateScaleMotionXY(&work->stretch);
        BattleAnimUtil_GetScaleFloat(&work->stretch, &x, &y);
        if (work->flipRise == TRUE) {
            x = 0.0f - x;
        }
        ManagedSprite_SetAffineScale(work->sprites[0], x, y);
        BattleAnimUtil_AlignSpriteToScale(work->sprites[0], (s16)work->y, (s16)work->height, work->stretch.curY, 0);
        work->partTimer--;
        if (work->partTimer < 0) {
            work->partState++;
            BattleAnimUtil_InitScaleMotionXY(&work->stretch, 10, 1, 20, 20, 10, 4);
        }
        break;
    case JUMP_RISE_STATE_SCALE:
        BattleAnimUtil_UpdateScaleMotionXY(&work->stretch);
        if (BattleAnimUtil_UpdateScaleMotion(&work->scale)) {
            work->stretch.x = work->scale.x;
            BattleAnimUtil_GetScaleFloat(&work->stretch, &x, &y);
            if (work->flipRise == TRUE) {
                x = 0.0f - x;
            }
            ManagedSprite_SetAffineScale(work->sprites[0], x, y);
            BattleAnimUtil_AlignSpriteToScale(work->sprites[0], (s16)work->y, (s16)work->height, work->stretch.curY, 0);
        } else {
            work->partState++;
            ManagedSprite_SetDrawFlag(work->sprites[2], FALSE);
            ManagedSprite_SetDrawFlag(work->sprites[0], FALSE);
            done = TRUE;
        }
        break;
    case JUMP_RISE_STATE_DONE:
        done = TRUE;
        break;
    }
    return done;
}

void BattleAnimTask_DarkSpriteJumpStartDrop(BattleAnimDarkSpriteJumpWork *work) {
    s16 x;
    s16 y;
    f32 scaleX;
    f32 scaleY;

    ManagedSprite_GetPositionXY(work->sprites[1], &x, &y);
    BattleAnimUtil_InitLinearMotion(&work->drop, x, x, (s16)(y - 64), y, 8);
    BattleAnimUtil_InitScaleMotionXY(&work->stretch, 2, 10, 20, 10, 10, 8);
    BattleAnimUtil_GetScaleFloat(&work->stretch, &scaleX, &scaleY);
    if (work->flipDrop == TRUE) {
        scaleX = 0.0f - scaleX;
    }
    ManagedSprite_SetAffineScale(work->sprites[1], scaleX, scaleY);
    BattleAnimUtil_UpdateLinearMotionSprite(&work->drop, work->sprites[1]);
    work->partState = 0;
    work->partTimer = 6;
    ManagedSprite_SetDrawFlag(work->sprites[3], gBattleAnimDarkSpriteDrawFlags[BattleAnimUtil_GetBattlerType(work->system, BattleAnimSystem_GetDefender(work->system))]);
    ManagedSprite_SetDrawFlag(work->sprites[1], TRUE);
    BattleAnimTask_DarkSpriteJumpDarkenSprite(work, work->sprites[1]);
}

enum {
    JUMP_DROP_STATE_FALL,
    JUMP_DROP_STATE_LAND,
    JUMP_DROP_STATE_DONE,
};

BOOL BattleAnimTask_DarkSpriteJumpUpdateDrop(BattleAnimDarkSpriteJumpWork *work) {
    BOOL done = FALSE;
    BOOL active;
    f32 x;
    f32 y;

    switch (work->partState) {
    case JUMP_DROP_STATE_FALL:
        work->partTimer--;
        BattleAnimUtil_UpdateLinearMotionSprite(&work->drop, work->sprites[1]);
        if (work->partTimer < 0) {
            work->partState++;
        }
        break;
    case JUMP_DROP_STATE_LAND:
        BattleAnimUtil_UpdateLinearMotionSprite(&work->drop, work->sprites[1]);
        active = BattleAnimUtil_UpdateScaleMotionXY(&work->stretch);
        BattleAnimUtil_GetScaleFloat(&work->stretch, &x, &y);
        if (work->flipDrop == TRUE) {
            x = 0.0f - x;
        }
        ManagedSprite_SetAffineScale(work->sprites[1], x, y);
        if (!active) {
            work->partState++;
            done = TRUE;
        }
        break;
    case JUMP_DROP_STATE_DONE:
        done = TRUE;
        break;
    }
    return done;
}

enum {
    DARK_JUMP_STATE_FADE_IN,
    DARK_JUMP_STATE_WAIT_FADE_IN,
    DARK_JUMP_STATE_RISE,
    DARK_JUMP_STATE_DELAY,
    DARK_JUMP_STATE_DROP,
    DARK_JUMP_STATE_FADE_OUT,
    DARK_JUMP_STATE_END,
};

void BattleAnimTask_DarkSpriteJump(SysTask *task, void *data) {
    BattleAnimDarkSpriteJumpWork *work = data;

    switch (work->state) {
    case DARK_JUMP_STATE_FADE_IN:
        BattleAnimUtil_SetBlendWithBattleBg(work->system, 0, 16);
        BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 0, 8, 16, 8, 8);
        work->state++;
        break;
    case DARK_JUMP_STATE_WAIT_FADE_IN:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            work->state++;
            BattleAnimTask_DarkSpriteJumpStartRise(work);
        }
        break;
    case DARK_JUMP_STATE_RISE:
        if (BattleAnimTask_DarkSpriteJumpUpdateRise(work)) {
            work->state++;
            work->timer = 2;
        }
        break;
    case DARK_JUMP_STATE_DELAY:
        if (--work->timer < 0) {
            BattleAnimTask_DarkSpriteJumpStartDrop(work);
            work->state++;
        }
        break;
    case DARK_JUMP_STATE_DROP:
        if (BattleAnimTask_DarkSpriteJumpUpdateDrop(work)) {
            work->state++;
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 8, 0, 8, 16, 8);
        }
        break;
    case DARK_JUMP_STATE_FADE_OUT:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            work->state++;
        }
        break;
    case DARK_JUMP_STATE_END:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimFunc_DarkSpriteJump(BattleAnimSystem *system) {
    BattleAnimDarkSpriteJumpWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimDarkSpriteJumpWork));
    Pokepic *attackerPokepic;
    Pokepic *defenderPokepic;
    s16 x;
    s16 y;
    s16 defenderX;
    s16 defenderY;

    work->system = system;
    work->spriteManager = BattleAnimSystem_GetBattlerSpriteManager(system);
    work->paletteData = BattleAnimSystem_GetPaletteData(work->system);
    attackerPokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    x = Pokepic_GetAttr(attackerPokepic, POKEPIC_X);
    y = Pokepic_GetAttr(attackerPokepic, POKEPIC_Y);
    y -= Pokepic_GetAttr(attackerPokepic, POKEPIC_SHADOW_H);
    defenderPokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(system));
    defenderX = Pokepic_GetAttr(defenderPokepic, POKEPIC_X);
    defenderY = Pokepic_GetAttr(defenderPokepic, POKEPIC_Y);
    defenderY -= Pokepic_GetAttr(defenderPokepic, POKEPIC_SHADOW_H);
    work->y = y;
    work->height = BattleAnimSystem_GetBattlerHeight(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->sprites[0] = BattleAnimSystem_GetBattlerSprite(work->system, 0);
    ManagedSprite_SetDrawPriority(work->sprites[0], 100);
    ManagedSprite_SetPriority(work->sprites[0], 1);
    ManagedSprite_SetPositionXY(work->sprites[0], x, y);
    ManagedSprite_SetDrawFlag(work->sprites[0], FALSE);
    ManagedSprite_SetOamMode(work->sprites[0], GX_OAM_MODE_XLU);
    ManagedSprite_SetAffineOverwriteMode(work->sprites[0], NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    work->sprites[1] = BattleAnimSystem_GetBattlerSprite(work->system, 1);
    ManagedSprite_SetDrawPriority(work->sprites[1], 100);
    ManagedSprite_SetPriority(work->sprites[1], 1);
    ManagedSprite_SetPositionXY(work->sprites[1], defenderX, defenderY);
    ManagedSprite_SetDrawFlag(work->sprites[1], FALSE);
    ManagedSprite_SetOamMode(work->sprites[1], GX_OAM_MODE_XLU);
    ManagedSprite_SetAffineOverwriteMode(work->sprites[1], NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    ManagedSprite_SetAffineTranslation(work->sprites[1], 0, 40);
    work->flipRise = BattleAnimSystem_ShouldFlipSide(work->system, 0);
    work->flipDrop = BattleAnimSystem_ShouldFlipSide(work->system, 1);
    work->sprites[2] = BattleAnimSystem_GetBattlerSprite(work->system, 2);
    work->sprites[3] = BattleAnimSystem_GetBattlerSprite(work->system, 3);
    ManagedSprite_SetDrawFlag(work->sprites[2], FALSE);
    ManagedSprite_SetDrawFlag(work->sprites[3], FALSE);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_DarkSpriteJump, work);
}

void BattleAnimTask_ScaleSpritesOverDefenderStartGrow(BattleAnimScaleSpritesWork *work) {
    BattleAnimUtil_InitScaleMotion(&work->scale, 25, 10, 10, 10);
    BattleAnimUtil_InitSpriteSequence(&work->sequence, (const BattleAnimSinMotion *)&work->scale, (BattleAnimSinMotionFunc)BattleAnimUtil_UpdateScaleMotion, work->x, work->y, 9, 3, TRUE, work->sprites[2], work->sprites[1], work->sprites[0], NULL);
}

void BattleAnimTask_ScaleSpritesOverDefenderStartShrink(BattleAnimScaleSpritesWork *work) {
    BattleAnimUtil_InitScaleMotion(&work->scale, 10, 10, 25, 6);
    BattleAnimUtil_InitSpriteSequence(&work->sequence, (const BattleAnimSinMotion *)&work->scale, (BattleAnimSinMotionFunc)BattleAnimUtil_UpdateScaleMotion, work->x, work->y, 5, 3, TRUE, work->sprites[0], work->sprites[1], work->sprites[2], NULL);
    ManagedSprite_SetDrawFlag(work->sprites[0], TRUE);
}

enum {
    SCALE_SPRITES_STATE_START_GROW,
    SCALE_SPRITES_STATE_GROW,
    SCALE_SPRITES_STATE_SHAKE,
    SCALE_SPRITES_STATE_SHRINK,
    SCALE_SPRITES_STATE_END,
};

void BattleAnimTask_ScaleSpritesOverDefender(SysTask *task, void *data) {
    BattleAnimScaleSpritesWork *work = data;
    int i;

    switch (work->state) {
    case SCALE_SPRITES_STATE_START_GROW:
        BattleAnimTask_ScaleSpritesOverDefenderStartGrow(work);
        work->state++;
        break;
    case SCALE_SPRITES_STATE_GROW:
        if (BattleAnimUtil_UpdateSpriteSequence(&work->sequence) == FALSE) {
            ManagedSprite_SetDrawFlag(work->sprites[0], TRUE);
            work->state++;
            BattleAnimUtil_InitShake(&work->shake, 4, 0, 1, 6);
        }
        break;
    case SCALE_SPRITES_STATE_SHAKE:
        if (BattleAnimUtil_UpdateShakePokepic(&work->shake, work->x, work->y, work->pokepic) == FALSE) {
            work->state++;
            BattleAnimTask_ScaleSpritesOverDefenderStartShrink(work);
        }
        break;
    case SCALE_SPRITES_STATE_SHRINK:
        if (BattleAnimUtil_UpdateSpriteSequence(&work->sequence) == FALSE) {
            work->state++;
        }
        break;
    case SCALE_SPRITES_STATE_END:
        for (i = 0; i < 3; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_ScaleSpritesOverDefender(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;
    BattleAnimScaleSpritesWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimScaleSpritesWork));
    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(system));
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->y = work->y - Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H);
    BattleAnimSystem_GetSpriteTemplate(&template, system);
    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            work->sprites[i] = sprite;
            ManagedSprite_SetAnim(sprite, 1);
        } else {
            work->sprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
            ManagedSprite_SetOamMode(work->sprites[i], GX_OAM_MODE_XLU);
        }
        ManagedSprite_SetDrawPriority(work->sprites[i], 100);
        ManagedSprite_SetPriority(work->sprites[i], 1);
        ManagedSprite_SetPositionXY(work->sprites[i], work->x, work->y);
        ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
        ManagedSprite_SetAffineOverwriteMode(work->sprites[i], NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_ScaleSpritesOverDefender, work);
}

void BattleAnimTask_SpritesOrbitAttackerBobbingInit(BattleAnimOrbitSpritesWork *work, int direction) {
    int i;

    for (i = 0; i < 6; i++) {
        BattleAnimUtil_InitSinMotion(&work->paths[i], 0, 0x7FFF, 0, 0, FX32_CONST(50), 0, 48);
        work->paths[i].angleX += i * 0x2AAA;
        work->paths[i].stepX *= direction;
    }
}

void BattleAnimTask_SpritesOrbitAttackerBobbingUpdate(BattleAnimOrbitSpritesWork *work) {
    int i;

    for (i = 0; i < 6; i++) {
        fx32 offset;
        BattleAnimUtil_UpdateSinMotion(&work->paths[i]);
        offset = FX_Mul(FX_SinIdx((u16)(work->paths[i].angleX * 5)), FX32_CONST(10) * work->direction);
        ManagedSprite_SetPositionXY(work->sprites[i], work->x + work->paths[i].x, work->y + (s16)(offset >> FX32_SHIFT));
        if (work->paths[i].angleX >= 0x3FFF && work->paths[i].angleX <= 0xBF49) {
            ManagedSprite_SetPriority(work->sprites[i], 1);
        } else {
            ManagedSprite_SetPriority(work->sprites[i], BattleAnimSystem_GetBasePriority(work->system) + 1);
        }
    }
}

enum {
    ORBIT_BOBBING_STATE_FADE_IN,
    ORBIT_BOBBING_STATE_WAIT_FADE_IN,
    ORBIT_BOBBING_STATE_ORBIT,
    ORBIT_BOBBING_STATE_FADE_OUT,
    ORBIT_BOBBING_STATE_END,
};

void BattleAnimTask_SpritesOrbitAttackerBobbing(SysTask *task, void *data) {
    BattleAnimOrbitSpritesWork *work = data;
    int i;

    switch (work->state) {
    case ORBIT_BOBBING_STATE_FADE_IN:
        BattleAnimUtil_SetBlendWithBattleBg(work->system, 1, 15);
        BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 1, 16, 15, 0, 10);
        BattleAnimTask_SpritesOrbitAttackerBobbingInit(work, work->direction);
        BattleAnimTask_SpritesOrbitAttackerBobbingUpdate(work);
        work->state++;
        break;
    case ORBIT_BOBBING_STATE_WAIT_FADE_IN:
        BattleAnimTask_SpritesOrbitAttackerBobbingUpdate(work);
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            work->state++;
            work->timer = 28;
        }
        break;
    case ORBIT_BOBBING_STATE_ORBIT:
        BattleAnimTask_SpritesOrbitAttackerBobbingUpdate(work);
        work->timer--;
        if (work->timer < 0) {
            work->state++;
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 16, 1, 0, 15, 10);
        }
        break;
    case ORBIT_BOBBING_STATE_FADE_OUT:
        BattleAnimTask_SpritesOrbitAttackerBobbingUpdate(work);
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            work->state++;
        }
        break;
    case ORBIT_BOBBING_STATE_END:
        for (i = 0; i < 6; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_SpritesOrbitAttackerBobbing(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;
    BattleAnimOrbitSpritesWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimOrbitSpritesWork));
    Pokepic *pokepic;
    int i;

    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    work->x = Pokepic_GetAttr(pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(pokepic, POKEPIC_Y);
    BattleAnimSystem_GetSpriteTemplate(&template, system);
    for (i = 0; i < 6; i++) {
        if (i == 0) {
            work->sprites[i] = sprite;
        } else {
            work->sprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        }
        ManagedSprite_SetDrawPriority(work->sprites[i], 100);
        ManagedSprite_SetOamMode(work->sprites[i], GX_OAM_MODE_XLU);
        ManagedSprite_SetAnimateFlag(work->sprites[i], TRUE);
    }
    work->direction = BattleAnimUtil_GetBattlerSideSignByMode(work->system, BattleAnimSystem_GetAttacker(work->system));
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_SpritesOrbitAttackerBobbing, work);
}

void BattleAnimTask_FloatingSpriteStreamLaunch(ManagedSprite *sprite, BattleAnimLinearMotion *motion, int dirX, int dirY) {
    s16 x;
    s16 y;

    ManagedSprite_GetPositionXY(sprite, &x, &y);
    int startY = y;
    int startX = x;
    BattleAnimUtil_InitLinearMotion(motion, startX, startX + (dirX << 8), startY, startY + dirY * -128, 59);
    ManagedSprite_SetDrawFlag(sprite, TRUE);
}

BOOL BattleAnimTask_FloatingSpriteStreamUpdateSprite(ManagedSprite *sprite, BattleAnimLinearMotion *motion) {
    s16 x;
    s16 y;
    s16 angle;
    fx32 sin;
    f32 scale;

    if (BattleAnimUtil_UpdateLinearMotionSprite(motion, sprite)) {
        ManagedSprite_GetPositionXY(sprite, &x, &y);
        angle = x;
        sin = FX_SinIdx((u16)(angle * 364));
        ManagedSprite_SetPositionXY(sprite, angle, y + (s16)(FX_Mul(sin, FX32_CONST(14)) >> FX32_SHIFT));
        scale = 1.0f + (f32)(-FX_Mul(sin, 696)) / 4096.0f;
        ManagedSprite_SetAffineScale(sprite, scale, scale);
        return FALSE;
    }
    return TRUE;
}

BOOL BattleAnimTask_FloatingSpriteStreamUpdate(BattleAnimFloatingSpritesWork *work) {
    int i;
    BOOL done;

    if (work->count < 15) {
        work->timer++;
        if (work->timer > 6) {
            work->timer = 0;
            BattleAnimTask_FloatingSpriteStreamLaunch(work->sprites[work->count], &work->motions[work->count], work->dirX, work->dirY);
            work->count++;
        }
    }
    for (i = 0; i < work->count; i++) {
        done = BattleAnimTask_FloatingSpriteStreamUpdateSprite(work->sprites[i], &work->motions[i]);
    }
    if (work->count >= 15 && done == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void BattleAnimTask_FloatingSpriteStream(SysTask *task, void *data) {
    BattleAnimFloatingSpritesWork *work = data;
    int i;

    if (BattleAnimTask_FloatingSpriteStreamUpdate(work)) {
        for (i = 0; i < 15; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task);
    } else {
        SpriteSystem_DrawSprites(work->spriteManager);
    }
}

void BattleAnimSpriteFunc_FloatingSpriteStream(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;
    s16 x;
    s16 y;
    Pokepic *pokepic;
    int i;
    BattleAnimFloatingSpritesWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimFloatingSpritesWork));

    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    x = Pokepic_GetAttr(pokepic, POKEPIC_X);
    y = Pokepic_GetAttr(pokepic, POKEPIC_Y);
    BattleAnimSystem_GetSpriteTemplate(&template, system);
    for (i = 0; i < 15; i++) {
        if (i == 0) {
            work->sprites[i] = sprite;
        } else {
            work->sprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        }
        ManagedSprite_SetDrawPriority(work->sprites[i], 100);
        ManagedSprite_SetPriority(work->sprites[i], 1);
        ManagedSprite_SetAffineOverwriteMode(work->sprites[i], NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
        ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
        ManagedSprite_SetPositionXY(work->sprites[i], x, y);
        ManagedSprite_SetAnim(work->sprites[i], i % 3);
    }
    work->dirX = BattleAnimUtil_GetBattlerSideSignByMode(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->dirY = BattleAnimUtil_GetBattlerSideSign(work->system, BattleAnimSystem_GetAttacker(work->system));
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_FloatingSpriteStream, work);
}

void BattleAnimTask_WobbleDefenderOnBgSetPhase(BattleAnimWobbleOnBgWork *work) {
    switch (work->phase) {
    case 0:
        work->angle = 0x7FFF;
        work->slope = 5;
        work->amplitude = FX32_ONE * 16;
        work->timer = 16;
        break;
    case 1:
        work->angle = 0x7FFF;
        work->slope = -5;
        work->amplitude = -FX32_ONE * 16;
        work->timer = 16;
        break;
    case 2:
        work->angle = 0x7FFF;
        work->slope = 10;
        work->amplitude = FX32_ONE * 20;
        work->timer = 16;
        break;
    }
}

void BattleAnimTask_WobbleDefenderOnBgUpdateScanlines(BattleAnimWobbleOnBgWork *work) {
    u32 *buffer;
    int start;
    int end;
    int line;
    s16 offset;
    int dest;
    fx32 amplitude;
    s16 x;
    s16 y;
    u32 scroll;

    buffer = BattleAnimScanline_GetWriteBuffer(work->scanline);
    end = start = work->top;
    start -= 8;
    end += 88;
    if (start < 0) {
        start = 0;
    }
    if (end > 192) {
        end = 192;
    }
    work->alternation *= -1;
    for (line = start; line < end; line++) {
        if (line & 2) {
            amplitude = work->amplitude + (work->alternation << FX32_SHIFT);
        } else {
            amplitude = work->amplitude - (work->alternation << FX32_SHIFT);
        }
        offset = FX_Mul(FX_SinIdx((u16)(work->angle + (line - start) * 409)), amplitude) >> FX32_SHIFT;
        offset += (line - work->center) * work->slope / 10;
        scroll = work->scroll;
        x = scroll;
        y = scroll >> 16;
        dest = line - 1;
        if (dest < 0) {
            dest += 192;
        }
        buffer[dest] = BattleAnimUtil_PackXY(x + offset, y);
    }
}

enum {
    WOBBLE_STATE_SET_PHASE,
    WOBBLE_STATE_WOBBLE,
    WOBBLE_STATE_END,
};

void BattleAnimTask_WobbleDefenderOnBg(SysTask *task, void *data) {
    BattleAnimWobbleOnBgWork *work = data;

    switch (work->state) {
    case WOBBLE_STATE_SET_PHASE:
        BattleAnimTask_WobbleDefenderOnBgSetPhase(work);
        BattleAnimTask_WobbleDefenderOnBgUpdateScanlines(work);
        work->state++;
        break;
    case WOBBLE_STATE_WOBBLE:
        work->timer--;
        BattleAnimTask_WobbleDefenderOnBgUpdateScanlines(work);
        if (work->timer < 0) {
            work->phase++;
            if (work->phase < 3) {
                work->state = WOBBLE_STATE_SET_PHASE;
            } else {
                work->state++;
            }
        }
        break;
    case WOBBLE_STATE_END: {
        u32 bgLayer;
        BgConfig *bgConfig;

        if (BattleAnimSystem_IsBattlerSemiInvulnerable(work->system, BattleAnimSystem_GetDefender(work->system)) == FALSE) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, FALSE);
        }
        BattleAnimScanline_Disable(work->scanline);
        BattleAnimScanline_Free(work->scanline);
        bgLayer = BattleAnimSystem_GetBgLayer(work->system, 1);
        SetBgPriority(bgLayer, (u8)BattleAnimSystem_GetBgPriority(work->system, 1));
        SetBgPriority(GF_BG_LYR_MAIN_0, (u8)BattleAnimSystem_GetBasePriority(work->system));
        bgLayer = BattleAnimSystem_GetBgLayer(work->system, 1);
        BG_ClearCharDataRange(bgLayer, 0x4000, 0, BattleAnimSystem_GetHeapID(work->system));
        bgConfig = BattleAnimSystem_GetBgConfig(work->system);
        bgLayer = BattleAnimSystem_GetBgLayer(work->system, 1);
        BgClearTilemapBufferAndCommit(bgConfig, bgLayer);
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task);
        break;
    }
    }
}

void BattleAnimFunc_WobbleDefenderOnBg(BattleAnimSystem *system) {
    BattleAnimWobbleOnBgWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimWobbleOnBgWork));
    s16 x;
    int y;

    work->system = system;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(system));
    x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    y = (s16)Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    y = (s16)(y - Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H));
    if (BattleAnimSystem_IsBattlerSemiInvulnerable(work->system, BattleAnimSystem_GetDefender(work->system)) == FALSE) {
        Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, TRUE);
    }
    x -= 40;
    work->center = y;
    y -= 40;
    work->top = y;
    work->scroll = BattleAnimUtil_PackXY(-x, -y);
    work->scanline = BattleAnimScanline_New(BattleAnimUtil_GetBgOffsetRegister(BattleAnimSystem_GetBgMaskLayer(work->system, 1)), work->scroll, BattleAnimSystem_GetHeapID(work->system));
    work->alternation = 1;
    {
        int type = BattleAnimUtil_GetBattlerType(work->system, BattleAnimSystem_GetDefender(system));
        if (type == BATTLER_TYPE_ENEMY_SIDE_SLOT_1 || type == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
            int bgLayer = BattleAnimSystem_GetBgLayer(work->system, 1);
            ((SetBgPriorityU8Func)SetBgPriority)(bgLayer, BattleAnimSystem_GetBasePriority(work->system));
            ((SetBgPriorityU8Func)SetBgPriority)(0, BattleAnimSystem_GetBasePriority(work->system) + 1);
        }
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_WobbleDefenderOnBg, work);
}

enum {
    WINDOW_SWEEP_STATE_INIT,
    WINDOW_SWEEP_STATE_DELAY,
    WINDOW_SWEEP_STATE_SWEEP,
    WINDOW_SWEEP_STATE_WAIT_FLASH,
    WINDOW_SWEEP_STATE_WAIT_RESTORE,
    WINDOW_SWEEP_STATE_END,
};

void BattleAnimTask_WindowSweepFlashWhite(SysTask *task, void *data) {
    BattleAnimWindowSweepWork *work = data;
    PaletteData *paletteData;

    switch (work->state) {
    case WINDOW_SWEEP_STATE_INIT:
        BattleAnimUtil_InitLinearMotion(&work->sweep, 127, 0, 0, 0, 8);
        GX_SetVisibleWnd(GX_WNDMASK_W0);
        BattleAnimUtil_SetWindowInsidePlanes(work->system, 0, FALSE);
        reg_G2_WIN0H = 255;
        reg_G2_WIN0V = 191;
        work->timer = 7;
        work->state++;
        break;
    case WINDOW_SWEEP_STATE_DELAY:
        if (--work->timer < 0) {
            paletteData = BattleAnimSystem_GetPaletteData(work->system);
            PaletteData_BeginPaletteFade(paletteData, PLTTBUF_MAIN_BG_F, BattleAnimSystem_GetBattleBgPaletteMask(work->system), 0, 0, 16, 0xFFFF);
            work->state++;
        }
        break;
    case WINDOW_SWEEP_STATE_SWEEP:
        if (BattleAnimUtil_UpdateLinearMotion(&work->sweep)) {
            s16 x = work->sweep.x;
            reg_G2_WIN0H = (((127 - x) << 8) & 0xFF00) | (u8)(x + 128);
            reg_G2_WIN0V = 191;
        } else {
            work->state++;
        }
        break;
    case WINDOW_SWEEP_STATE_WAIT_FLASH:
        if (PaletteData_GetSelectedBuffersBitmask(BattleAnimSystem_GetPaletteData(work->system)) == 0) {
            work->state++;
            GX_SetVisibleWnd(GX_WNDMASK_NONE);
            PaletteData_BeginPaletteFade(BattleAnimSystem_GetPaletteData(work->system), PLTTBUF_MAIN_BG_F, BattleAnimSystem_GetBattleBgPaletteMask(work->system), 0, 16, 0, 0xFFFF);
        }
        break;
    case WINDOW_SWEEP_STATE_WAIT_RESTORE:
        paletteData = BattleAnimSystem_GetPaletteData(work->system);
        if (PaletteData_GetSelectedBuffersBitmask(paletteData) == 0) {
            work->state++;
        }
        break;
    case WINDOW_SWEEP_STATE_END:
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task);
        break;
    }
}

void BattleAnimFunc_WindowSweepFlashWhite(BattleAnimSystem *system) {
    BattleAnimWindowSweepWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimWindowSweepWork));
    work->system = system;
    BattleAnimSystem_CreateTaskDefaultPriority(system, BattleAnimTask_WindowSweepFlashWhite, work);
}

enum {
    FADE_SPRITE_ANIM_STATE_FADE_IN,
    FADE_SPRITE_ANIM_STATE_WAIT_FADE_IN,
    FADE_SPRITE_ANIM_STATE_WAIT_ANIM,
    FADE_SPRITE_ANIM_STATE_FADE_OUT,
    FADE_SPRITE_ANIM_STATE_END,
};

void BattleAnimTask_FadeSpriteAnim(SysTask *task, void *data) {
    BattleAnimFadeSpriteAnimWork *work = data;

    switch (work->state) {
    case FADE_SPRITE_ANIM_STATE_FADE_IN:
        BattleAnimUtil_SetBlendWithBattleBg(work->system, 0, 16);
        BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 0, 16, 16, 0, 8);
        work->state++;
        break;
    case FADE_SPRITE_ANIM_STATE_WAIT_FADE_IN:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            work->state++;
        }
        break;
    case FADE_SPRITE_ANIM_STATE_WAIT_ANIM:
        if (!ManagedSprite_IsAnimated(work->sprite)) {
            work->state++;
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 16, 0, 0, 16, 8);
        }
        break;
    case FADE_SPRITE_ANIM_STATE_FADE_OUT:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            work->state++;
        }
        break;
    case FADE_SPRITE_ANIM_STATE_END:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_FadeSpriteAnim(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimFadeSpriteAnimWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimFadeSpriteAnimWork));
    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->sprite = sprite;
    ManagedSprite_SetAnimateFlag(work->sprite, TRUE);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_FadeSpriteAnim, work);
}

enum {
    STRETCH_SHAKE_STATE_START_STRETCH,
    STRETCH_SHAKE_STATE_STRETCH,
    STRETCH_SHAKE_STATE_START_RESTORE,
    STRETCH_SHAKE_STATE_RESTORE,
    STRETCH_SHAKE_STATE_END,
};

void BattleAnimTask_StretchShakeDefender(SysTask *task, void *data) {
    BattleAnimStretchShakeWork *work = data;
    BOOL active;

    switch (work->state) {
    case STRETCH_SHAKE_STATE_START_STRETCH:
        BattleAnimUtil_InitScaleMotionXY(&work->scale, (s16)work->startX, (s16)work->endX, (s16)work->startY, (s16)work->endY, (s16)work->divisor, (s32)work->durations >> 16);
        BattleAnimUtil_UpdateShakePokepic(&work->shake, work->x, work->y, work->pokepic);
        BattleAnimUtil_UpdateScaleMotionXYPokepic(&work->scale, work->pokepic);
        BattleAnimUtil_AlignPokepicToScale(work->pokepic, work->y, work->height, work->scale.curY, 0);
        work->state++;
        return;
    case STRETCH_SHAKE_STATE_STRETCH:
        BattleAnimUtil_UpdateShakePokepic(&work->shake, work->x, work->y, work->pokepic);
        active = BattleAnimUtil_UpdateScaleMotionXYPokepic(&work->scale, work->pokepic);
        BattleAnimUtil_AlignPokepicToScale(work->pokepic, work->y, work->height, work->scale.curY, 0);
        if (!active) {
            work->state++;
            return;
        }
        break;
    case STRETCH_SHAKE_STATE_START_RESTORE:
        BattleAnimUtil_InitScaleMotionXY(&work->scale, (s16)work->endX, (s16)work->startX, (s16)work->endY, (s16)work->startY, (s16)work->divisor, (u16)work->durations);
        BattleAnimUtil_UpdateShakePokepic(&work->shake, work->x, work->y, work->pokepic);
        BattleAnimUtil_UpdateScaleMotionXYPokepic(&work->scale, work->pokepic);
        BattleAnimUtil_AlignPokepicToScale(work->pokepic, work->y, work->height, work->scale.curY, 0);
        work->state++;
        return;
    case STRETCH_SHAKE_STATE_RESTORE:
        BattleAnimUtil_UpdateShakePokepic(&work->shake, work->x, work->y, work->pokepic);
        active = BattleAnimUtil_UpdateScaleMotionXYPokepic(&work->scale, work->pokepic);
        BattleAnimUtil_AlignPokepicToScale(work->pokepic, work->y, work->height, work->scale.curY, 0);
        if (!active) {
            if (--work->repeats <= 0) {
                work->state++;
            } else {
                work->state = STRETCH_SHAKE_STATE_START_STRETCH;
            }
            return;
        }
        break;
    case STRETCH_SHAKE_STATE_END:
        Pokepic_SetAttr(work->pokepic, POKEPIC_X, work->x);
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y);
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, 0x100);
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, 0x100);
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task);
        break;
    }
}

void BattleAnimFunc_StretchShakeDefender(BattleAnimSystem *system) {
    BattleAnimStretchShakeWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimStretchShakeWork));
    work->system = system;
    work->height = BattleAnimSystem_GetBattlerHeight(work->system, BattleAnimSystem_GetDefender(system));
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(work->system));
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->startX = 10;
    work->endX = 7;
    work->startY = 10;
    work->endY = 30;
    work->divisor = 10;
    work->repeats = 1;
    work->durations = (4 << 16) | 3;
    BattleAnimUtil_InitShake(&work->shake, 2, 0, 1, 2);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_StretchShakeDefender, work);
}

void BattleAnimTask_ShootSpriteFromAttacker(SysTask *task, void *data) {
    BattleAnimShootSpriteWork *work = data;

    if (!BattleAnimUtil_UpdateLinearMotionSprite(&work->motion, work->sprite)) {
        Sprite_DeleteAndFreeResources(work->sprite);
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task);
    } else {
        SpriteSystem_DrawSprites(work->spriteManager);
    }
}

void BattleAnimFunc_ShootSpriteFromAttacker(BattleAnimSystem *system) {
    BattleAnimShootSpriteWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimShootSpriteWork));
    u16 attacker;

    work->system = system;
    work->spriteManager = BattleAnimSystem_GetFirstSpriteManager(system);
    attacker = BattleAnimSystem_GetAttacker(system);
    if (BattleAnimUtil_GetBattlerSide(system, attacker) == BATTLE_ANIM_SIDE_PLAYER) {
        work->sprite = BattleAnimSystem_GetSprite(system, 0);
        Sprite_DeleteAndFreeResources(BattleAnimSystem_GetSprite(system, 1));
    } else {
        work->sprite = BattleAnimSystem_GetSprite(system, 1);
        Sprite_DeleteAndFreeResources(BattleAnimSystem_GetSprite(system, 0));
    }
    {
        int dirX = BattleAnimUtil_GetBattlerSideSignByMode(system, attacker);
        int dirY = BattleAnimUtil_GetBattlerSideSign(system, attacker);
        int baseX = BattleAnimSystem_GetBattlerScreenCoord(system, attacker, 0);
        int baseY = BattleAnimSystem_GetBattlerScreenCoord(system, attacker, 1);
        int x = baseX + dirX * 64;
        int dy = -(dirY * 16);
        int y = baseY + dy;
        ManagedSprite_SetPositionXY(work->sprite, x, y);
        BattleAnimUtil_InitLinearMotion(&work->motion, x, x + dirX * 48, y, y + dy, 6);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_ShootSpriteFromAttacker, work);
}

enum {
    SHAKE_LUNGE_STATE_START_SHAKE,
    SHAKE_LUNGE_STATE_SHAKE,
    SHAKE_LUNGE_STATE_LUNGE,
    SHAKE_LUNGE_STATE_HOLD,
    SHAKE_LUNGE_STATE_RETURN,
    SHAKE_LUNGE_STATE_END,
};

void BattleAnimTask_AttackerShakeLunge(SysTask *task, void *data) {
    BattleAnimShakeLungeWork *work = data;

    switch (work->state) {
    case SHAKE_LUNGE_STATE_START_SHAKE:
        BattleAnimUtil_InitShake(&work->shake, 4, 0, 1, 4);
        work->state++;
        break;
    case SHAKE_LUNGE_STATE_SHAKE:
        if (!BattleAnimUtil_UpdateShakePokepic(&work->shake, work->x, work->y, work->pokepic)) {
            work->state++;
            BattleAnimUtil_InitLinearMotion(&work->move, work->x, work->x + work->dirX * 40, work->y, work->y + work->dirY * -7, 4);
        }
        break;
    case SHAKE_LUNGE_STATE_LUNGE:
        if (!BattleAnimUtil_UpdateLinearMotionPokepic(&work->move, work->pokepic)) {
            work->state++;
            work->timer = 8;
        }
        break;
    case SHAKE_LUNGE_STATE_HOLD:
        if (--work->timer < 0) {
            work->state++;
            BattleAnimUtil_InitLinearMotion(&work->move, work->x + work->dirX * 40, work->x, work->y + work->dirY * -7, work->y, 4);
        }
        break;
    case SHAKE_LUNGE_STATE_RETURN:
        if (!BattleAnimUtil_UpdateLinearMotionPokepic(&work->move, work->pokepic)) {
            work->state++;
        }
        break;
    case SHAKE_LUNGE_STATE_END:
        Pokepic_SetAttr(work->pokepic, POKEPIC_X, work->x);
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y);
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task);
        break;
    }
}

void BattleAnimFunc_AttackerShakeLunge(BattleAnimSystem *system) {
    BattleAnimShakeLungeWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimShakeLungeWork));
    u16 attacker;

    work->system = system;
    attacker = BattleAnimSystem_GetAttacker(system);
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(system, attacker);
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->dirX = BattleAnimUtil_GetBattlerSideSignByMode(system, attacker);
    work->dirY = BattleAnimUtil_GetBattlerSideSign(system, attacker);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerShakeLunge, work);
}

enum {
    KNOCKBACK_STATE_START,
    KNOCKBACK_STATE_MOVE,
    KNOCKBACK_STATE_SHAKE,
    KNOCKBACK_STATE_RETURN,
    KNOCKBACK_STATE_END,
};

void BattleAnimTask_DefenderKnockback(SysTask *task, void *data) {
    BattleAnimKnockbackWork *work = data;

    switch (work->state) {
    case KNOCKBACK_STATE_START:
        BattleAnimUtil_InitLinearMotion(&work->move, work->x, work->x + work->dirX * -40, work->y, work->y + work->dirY * 16, 4);
        work->state++;
        break;
    case KNOCKBACK_STATE_MOVE:
        if (!BattleAnimUtil_UpdateLinearMotionPokepic(&work->move, work->pokepic)) {
            work->state++;
            BattleAnimUtil_InitShake(&work->shake, 4, 0, 1, 4);
        }
        break;
    case KNOCKBACK_STATE_SHAKE: {
        s16 x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
        s16 y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
        if (!BattleAnimUtil_UpdateShakePokepic(&work->shake, x, y, work->pokepic)) {
            work->state++;
            BattleAnimUtil_InitLinearMotion(&work->move, work->x + work->dirX * -40, work->x, work->y + work->dirY * 16, work->y, 4);
        }
        break;
    }
    case KNOCKBACK_STATE_RETURN:
        if (!BattleAnimUtil_UpdateLinearMotionPokepic(&work->move, work->pokepic)) {
            work->state++;
        }
        break;
    case KNOCKBACK_STATE_END:
        Pokepic_SetAttr(work->pokepic, POKEPIC_X, work->x);
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y);
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task);
        break;
    }
}

void BattleAnimFunc_DefenderKnockback(BattleAnimSystem *system) {
    BattleAnimKnockbackWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimKnockbackWork));
    int defender;

    work->system = system;
    defender = BattleAnimSystem_GetDefender(system);
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(system, defender);
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->dirX = BattleAnimUtil_GetBattlerSideSignByMode(system, defender);
    work->dirY = BattleAnimUtil_GetBattlerSideSign(system, defender);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_DefenderKnockback, work);
}

void BattleAnimSpriteFunc_RisingShrinkSprites(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimRisingShrinkSpritesWork *work;
    ManagedSpriteTemplate template;
    int attacker;
    int i;

    work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimRisingShrinkSpritesWork));
    work->system = system;
    work->spriteManager = spriteManager;
    attacker = BattleAnimSystem_GetAttacker(system);
    work->direction = BattleAnimUtil_GetBattlerSideSignByMode(system, attacker);
    BattleAnimSystem_GetSpriteTemplate(&template, work->system);
    template.x = BattleAnimSystem_GetBattlerScreenCoord(system, attacker, 0);
    template.y = BattleAnimSystem_GetBattlerScreenCoord(system, attacker, 1);
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            work->sprites[i] = sprite;
            ManagedSprite_SetPositionXY(sprite, template.x, template.y);
        } else {
            work->sprites[i] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
        }
        ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
        ManagedSprite_SetDrawPriority(work->sprites[i], 100);
        ManagedSprite_SetPriority(work->sprites[i], 1);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_RisingShrinkSprites, work);
}

enum {
    RISING_SHRINK_STATE_LAUNCH_FIRST,
    RISING_SHRINK_STATE_DELAY,
    RISING_SHRINK_STATE_LAUNCH_SECOND,
    RISING_SHRINK_STATE_WAIT,
    RISING_SHRINK_STATE_END,
};

void BattleAnimTask_RisingShrinkSprites(SysTask *task, void *data) {
    BattleAnimRisingShrinkSpritesWork *work = data;
    int i;

    switch (work->state) {
    case RISING_SHRINK_STATE_LAUNCH_FIRST:
        BattleAnimTask_RisingShrinkSpritesLaunch(work->sprites[0], &work->moves[0], &work->scales[0], work->direction);
        BattleAnimTask_RisingShrinkSpritesUpdate(work->sprites[0], &work->moves[0], &work->scales[0]);
        work->state++;
        work->timer = 8;
        break;
    case RISING_SHRINK_STATE_DELAY:
        work->timer--;
        BattleAnimTask_RisingShrinkSpritesUpdate(work->sprites[0], &work->moves[0], &work->scales[0]);
        if (work->timer < 0) {
            work->state++;
        }
        break;
    case RISING_SHRINK_STATE_LAUNCH_SECOND:
        BattleAnimTask_RisingShrinkSpritesLaunch(work->sprites[1], &work->moves[1], &work->scales[1], work->direction);
        BattleAnimTask_RisingShrinkSpritesUpdate(work->sprites[1], &work->moves[1], &work->scales[1]);
        BattleAnimTask_RisingShrinkSpritesUpdate(work->sprites[0], &work->moves[0], &work->scales[0]);
        work->state++;
        break;
    case RISING_SHRINK_STATE_WAIT:
        BattleAnimTask_RisingShrinkSpritesUpdate(work->sprites[0], &work->moves[0], &work->scales[0]);
        if (BattleAnimTask_RisingShrinkSpritesUpdate(work->sprites[1], &work->moves[1], &work->scales[1])) {
            work->state++;
        }
        break;
    case RISING_SHRINK_STATE_END:
        for (i = 0; i < 2; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimTask_RisingShrinkSpritesLaunch(ManagedSprite *sprite, BattleAnimLinearMotion *move, BattleAnimLinearMotion *scale, int direction) {
    s16 x;
    s16 y;

    ManagedSprite_SetAffineOverwriteMode(sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    ManagedSprite_SetDrawFlag(sprite, TRUE);
    ManagedSprite_GetPositionXY(sprite, &x, &y);
    BattleAnimUtil_InitLinearMotion(move, x, (s16)(x + direction * 16), y, (s16)(y - 32), 32);
    BattleAnimUtil_InitScaleMotion(scale, 2, 10, 10, 32);
}

BOOL BattleAnimTask_RisingShrinkSpritesUpdate(ManagedSprite *sprite, BattleAnimLinearMotion *move, BattleAnimLinearMotion *scale) {
    BattleAnimUtil_UpdateLinearMotionSprite(move, sprite);
    if (BattleAnimUtil_UpdateScaleMotionSprite(scale, sprite) != FALSE) {
        return FALSE;
    }
    ManagedSprite_SetDrawFlag(sprite, FALSE);
    return TRUE;
}

void BattleAnimSpriteFunc_PaletteCycleSprite(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimPaletteCycleSpriteWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimPaletteCycleSpriteWork));
    work->system = system;
    work->spriteManager = spriteManager;
    work->sprite = sprite;
    ManagedSprite_SetDrawFlag(work->sprite, FALSE);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    work->paletteOffset = ManagedSprite_GetPaletteOverrideOffset(work->sprite);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_PaletteCycleSprite, work);
}

enum {
    PALETTE_CYCLE_STATE_FADE_IN,
    PALETTE_CYCLE_STATE_WAIT_FADE_IN,
    PALETTE_CYCLE_STATE_CYCLE,
    PALETTE_CYCLE_STATE_FADE_OUT,
    PALETTE_CYCLE_STATE_END,
};

void BattleAnimTask_PaletteCycleSprite(SysTask *task, void *data) {
    BattleAnimPaletteCycleSpriteWork *work = data;

    switch (work->state) {
    case PALETTE_CYCLE_STATE_FADE_IN:
        BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 0, 16, 20, 4, 10);
        ManagedSprite_SetDrawFlag(work->sprite, TRUE);
        BattleAnimUtil_SetBlendWithBattleBg(work->system, 0, 20);
        work->state++;
        break;
    case PALETTE_CYCLE_STATE_WAIT_FADE_IN:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            work->state++;
            ManagedSprite_SetAnimateFlag(work->sprite, TRUE);
            ManagedSprite_SetAnimSpeed(work->sprite, FX32_ONE);
        }
        break;
    case PALETTE_CYCLE_STATE_CYCLE:
        ((ManagedSpriteSetPaletteOffsetFunc)ManagedSprite_SetPaletteOverrideOffset)(work->sprite, work->paletteOffset + ManagedSprite_GetAnimationFrame(work->sprite) % 3);
        if (!ManagedSprite_IsAnimated(work->sprite)) {
            work->state++;
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 16, 0, 4, 20, 8);
        }
        break;
    case PALETTE_CYCLE_STATE_FADE_OUT:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            work->state++;
            ManagedSprite_SetDrawFlag(work->sprite, FALSE);
        }
        break;
    case PALETTE_CYCLE_STATE_END:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_SpriteRowSequence(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;
    BattleAnimSpriteRowWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSpriteRowWork));
    int attacker;
    int direction;
    int i;

    work->system = system;
    work->spriteManager = spriteManager;
    attacker = BattleAnimSystem_GetAttacker(work->system);
    direction = BattleAnimUtil_GetBattlerSideSignByMode(system, attacker);
    BattleAnimSystem_GetSpriteTemplate(&template, work->system);
    template.x = BattleAnimSystem_GetBattlerScreenCoord(system, attacker, 0);
    template.y = BattleAnimSystem_GetBattlerScreenCoord(system, attacker, 1);
    for (i = 5; i >= 0; i--) {
        if (i == 5) {
            work->sprites[i] = sprite;
            ManagedSprite_SetPositionXY(work->sprites[i], template.x, template.y);
        } else {
            work->sprites[i] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
        }
        ManagedSprite_SetDrawPriority(work->sprites[i], 100);
        ManagedSprite_SetPriority(work->sprites[i], 1);
        ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
        BattleAnimTask_SpriteRowSequencePlace(work->sprites[i], &work->visibleTimes[i], &work->delays[i], i, direction);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_SpriteRowSequence, work);
}

void BattleAnimTask_SpriteRowSequencePlace(ManagedSprite *sprite, int *visibleTime, int *delay, int index, int direction) {
    s16 x;
    s16 y;

    ManagedSprite_SetAnim(sprite, index / 2);
    *delay = index * 2;
    *visibleTime = 16;
    ManagedSprite_GetPositionXY(sprite, &x, &y);
    int delta = 2;
    delta = (delta - 14) * index + 40;
    x += direction * delta;
    y += 40;
    ManagedSprite_SetPositionXY(sprite, x, y);
}

BOOL BattleAnimTask_SpriteRowSequenceUpdate(ManagedSprite *sprite, int *visibleTime, int *delay) {
    if (*delay > 0) {
        (*delay)--;
        if (*delay == 0) {
            ManagedSprite_SetDrawFlag(sprite, TRUE);
        }
    } else if (*visibleTime > 0) {
        (*visibleTime)--;
    } else {
        ManagedSprite_SetDrawFlag(sprite, FALSE);
        return TRUE;
    }
    return FALSE;
}

enum {
    SPRITE_ROW_STATE_UPDATE,
    SPRITE_ROW_STATE_END,
};

void BattleAnimTask_SpriteRowSequence(SysTask *task, void *data) {
    BattleAnimSpriteRowWork *work = data;
    int i;

    switch (work->state) {
    case SPRITE_ROW_STATE_UPDATE: {
        BOOL done = FALSE;
        for (i = 0; i < 6; i++) {
            done = BattleAnimTask_SpriteRowSequenceUpdate(work->sprites[i], &work->visibleTimes[i], &work->delays[i]);
        }
        if (done != FALSE) {
            work->state++;
        }
        break;
    }
    case SPRITE_ROW_STATE_END:
        for (i = 0; i < 6; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimTask_SpritesOrbitAttackerInit(BattleAnimOrbitSpritesWork *work, int direction) {
    int i;

    for (i = 0; i < 6; i++) {
        BattleAnimUtil_InitSinMotion(&work->paths[i], 0, 0x7FFF, 0, 0, FX32_CONST(50), 0, 48);
        work->paths[i].angleX += i * 0x2AAA;
        work->paths[i].stepX *= direction;
    }
}

void BattleAnimTask_SpritesOrbitAttackerUpdate(BattleAnimOrbitSpritesWork *work) {
    int i;

    for (i = 0; i < 6; i++) {
        BattleAnimUtil_UpdateSinMotion(&work->paths[i]);
        ManagedSprite_SetPositionXY(work->sprites[i], work->x + work->paths[i].x, work->y);
        if (work->paths[i].angleX >= 0x3FFF && work->paths[i].angleX <= 0xBF49) {
            ManagedSprite_SetPriority(work->sprites[i], 1);
        } else {
            ManagedSprite_SetPriority(work->sprites[i], BattleAnimSystem_GetBasePriority(work->system) + 1);
        }
    }
}

enum {
    ORBIT_STATE_FADE_IN,
    ORBIT_STATE_WAIT_FADE_IN,
    ORBIT_STATE_ORBIT,
    ORBIT_STATE_FADE_OUT,
    ORBIT_STATE_END,
};

void BattleAnimTask_SpritesOrbitAttacker(SysTask *task, void *data) {
    BattleAnimOrbitSpritesWork *work = data;
    int i;

    switch (work->state) {
    case ORBIT_STATE_FADE_IN:
        BattleAnimUtil_SetBlendWithBattleBg(work->system, 1, 15);
        BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 1, 16, 15, 0, 10);
        BattleAnimTask_SpritesOrbitAttackerInit(work, work->direction);
        BattleAnimTask_SpritesOrbitAttackerUpdate(work);
        work->state++;
        break;
    case ORBIT_STATE_WAIT_FADE_IN:
        BattleAnimTask_SpritesOrbitAttackerUpdate(work);
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            work->state++;
            work->timer = 28;
        }
        break;
    case ORBIT_STATE_ORBIT:
        BattleAnimTask_SpritesOrbitAttackerUpdate(work);
        work->timer--;
        if (work->timer < 0) {
            work->state++;
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 16, 1, 0, 15, 10);
        }
        break;
    case ORBIT_STATE_FADE_OUT:
        BattleAnimTask_SpritesOrbitAttackerUpdate(work);
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            work->state++;
        }
        break;
    case ORBIT_STATE_END:
        for (i = 0; i < 6; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_SpritesOrbitAttacker(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;
    BattleAnimOrbitSpritesWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimOrbitSpritesWork));
    Pokepic *pokepic;
    int i;

    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    work->x = Pokepic_GetAttr(pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(pokepic, POKEPIC_Y);
    BattleAnimSystem_GetSpriteTemplate(&template, system);
    for (i = 0; i < 6; i++) {
        if (i == 0) {
            work->sprites[i] = sprite;
        } else {
            work->sprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        }
        ManagedSprite_SetDrawPriority(work->sprites[i], 100);
        ManagedSprite_SetOamMode(work->sprites[i], GX_OAM_MODE_XLU);
        ManagedSprite_SetAnimateFlag(work->sprites[i], TRUE);
    }
    work->direction = BattleAnimUtil_GetBattlerSideSignByMode(work->system, BattleAnimSystem_GetAttacker(work->system));
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_SpritesOrbitAttacker, work);
}

void BattleAnimObjWindowBg_SetScrollVelocity(BattleAnimObjWindowBgParams *params, s16 x, s16 y) {
    params->velocityX = x;
    params->velocityY = y;
}

void BattleAnimObjWindowBg_SetBlend(BattleAnimObjWindowBgParams *params, s16 eva, s16 evb) {
    params->eva = eva;
    params->evb = evb;
}

void BattleAnimObjWindowBg_LoadGraphics(BattleAnimObjWindowBgWork *work, int bgLayer) {
    enum HeapID heapId = BattleAnimSystem_GetHeapID(work->common.system);
    SetBgPriority((u8)bgLayer, (u8)BattleAnimSystem_GetBgPriority(work->common.system, 3));
    GfGfxLoader_LoadCharData(work->params.narcId, gBattleAnimObjWindowBgFiles[work->params.graphic].graphics, work->common.bgConfig, (GFBgLayer)bgLayer, 0, 0, TRUE, heapId);
    PaletteData_LoadNarc(work->common.paletteData, work->params.narcId, gBattleAnimObjWindowBgFiles[work->params.graphic].palette, heapId, PLTTBUF_MAIN_BG, 32, 128);
    BgClearTilemapBufferAndCommit(work->common.bgConfig, (u8)bgLayer);
    GfGfxLoader_LoadScrnData(work->params.narcId, gBattleAnimObjWindowBgFiles[work->params.graphic].screen, work->common.bgConfig, (GFBgLayer)bgLayer, 0, 0, TRUE, heapId);
}

enum {
    OBJ_WINDOW_BG_STATE_LOAD,
    OBJ_WINDOW_BG_STATE_SET_BLEND,
    OBJ_WINDOW_BG_STATE_SCROLL,
    OBJ_WINDOW_BG_STATE_FADE_OUT,
};

void BattleAnimTask_ObjWindowBgScroll(SysTask *task, void *data) {
    BattleAnimObjWindowBgWork *work = data;

    switch (work->common.state[0]) {
    case OBJ_WINDOW_BG_STATE_LOAD:
        BattleAnimObjWindowBg_LoadGraphics(work, GF_BG_LYR_MAIN_2);
        ++work->common.state[0];
        break;
    case OBJ_WINDOW_BG_STATE_SET_BLEND:
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, work->params.eva, work->params.evb);
        ++work->common.state[0];
        break;
    case OBJ_WINDOW_BG_STATE_SCROLL:
        if (work->timer > 20) {
            ++work->common.state[0];
        } else {
            ++work->timer;
        }
        break;
    case OBJ_WINDOW_BG_STATE_FADE_OUT:
        if (work->params.eva) {
            --work->params.eva;
        }
        if (work->params.evb < 16) {
            ++work->params.evb;
        }
        if (!work->params.eva && work->params.evb == 16) {
            BgClearTilemapBufferAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2);
            ManagedSprite_SetDrawFlag(work->sprites[0], FALSE);
            ManagedSprite_SetDrawFlag(work->sprites[1], FALSE);
            ManagedSprite_SetDrawFlag(work->sprites[2], FALSE);
            ++work->common.state[0];
        }
        reg_G2_BLDALPHA = work->params.eva | (work->params.evb << 8);
        break;
    default:
        G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
        // OBJ window inside: the same planes
        reg_G2_WINOUT = (reg_G2_WINOUT & ~0x3F00) | 0x1F00;
        BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        BattleAnimUtil_FreeWork(work);
        return;
    }
    work->params.x += work->params.velocityX;
    work->params.y += work->params.velocityY;
    BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, work->params.x);
    BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, work->params.y);
    ManagedSprite_TickFrame(work->sprites[0]);
    ManagedSprite_TickFrame(work->sprites[1]);
    ManagedSprite_TickFrame(work->sprites[2]);
    SpriteSystem_DrawSprites(work->common.battlerSpriteManager);
}

void BattleAnimObjWindowBg_InitParams(BattleAnimSystem *system, BattleAnimObjWindowBgParams *params) {
    params->narcId = NARC_a_0_0_7;
    params->graphic = (s32)BattleAnimSystem_GetArg(system, 0);
    params->target = (s32)BattleAnimSystem_GetArg(system, 1);
}

void BattleAnimObjWindowBg_Start(BattleAnimSystem *system, const BattleAnimObjWindowBgParams *params) {
    BattleAnimObjWindowBgWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimObjWindowBgWork));
    u32 type;

    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->params = *params;
    work->sprites[0] = BattleAnimSystem_GetBattlerSprite(work->common.system, 0);
    work->sprites[1] = BattleAnimSystem_GetBattlerSprite(work->common.system, 1);
    work->sprites[2] = BattleAnimSystem_GetBattlerSprite(work->common.system, 2);
    BattleAnimSystem_GetBgPriority(work->common.system, 2);
    ManagedSprite_SetPriority(work->sprites[0], 2);
    ManagedSprite_SetPriority(work->sprites[1], 2);
    type = BattleAnimUtil_GetBattlerType(system, work->params.target == 0 ? BattleAnimSystem_GetAttacker(system) : BattleAnimSystem_GetDefender(system));
    if ((u32)BattleAnimSystem_GetField08(system) == 1) {
        ManagedSprite_SetDrawFlag(work->sprites[2], FALSE);
        type = BattleAnimSystem_GetBgPriority(work->common.system, 2);
        ManagedSprite_SetPriority(work->sprites[0], type);
        ManagedSprite_SetPriority(work->sprites[1], type);
    } else if (type <= BATTLER_TYPE_SOLO_ENEMY) {
        ManagedSprite_SetDrawFlag(work->sprites[2], FALSE);
    } else if (type == BATTLER_TYPE_ENEMY_SIDE_SLOT_1 || type == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
        ManagedSprite_SetPriority(work->sprites[2], 3);
    } else {
        ManagedSprite_SetPriority(work->sprites[2], 1);
    }
    {
        // OBJ window inside: BG1, BG2, OBJ and blending
        u32 winout = (reg_G2_WINOUT & ~REG_G2_WINOUT_OBJWININ_MASK) | ((GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_OBJ) << REG_G2_WINOUT_OBJWININ_SHIFT);
        winout |= 0x20 << REG_G2_WINOUT_OBJWININ_SHIFT;
        reg_G2_WINOUT = (u16)winout;
        G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
        GX_SetVisibleWnd(GX_WNDMASK_OW);
    }
    ManagedSprite_SetOamMode(work->sprites[1], GX_OAM_MODE_OBJWND);
    work->params.x = 0;
    work->params.y = 0;
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_ObjWindowBgScroll, work);
}

void BattleAnimFunc_ObjWindowBgScrollUp(BattleAnimSystem *system) {
    BattleAnimObjWindowBgParams params;
    BattleAnimObjWindowBg_InitParams(system, &params);
    BattleAnimObjWindowBg_SetScrollVelocity(&params, 0, 3);
    BattleAnimObjWindowBg_SetBlend(&params, 12, 4);
    BattleAnimObjWindowBg_Start(system, &params);
}

void BattleAnimFunc_ObjWindowBgScrollDown(BattleAnimSystem *system) {
    BattleAnimObjWindowBgParams params;
    BattleAnimObjWindowBg_InitParams(system, &params);
    BattleAnimObjWindowBg_SetScrollVelocity(&params, 0, -3);
    BattleAnimObjWindowBg_SetBlend(&params, 12, 4);
    BattleAnimObjWindowBg_Start(system, &params);
}

void BattleAnimFunc_ObjWindowBgScrollUpAlt(BattleAnimSystem *system) {
    BattleAnimObjWindowBgParams params;
    BattleAnimObjWindowBg_InitParams(system, &params);
    BattleAnimObjWindowBg_SetScrollVelocity(&params, 0, 3);
    BattleAnimObjWindowBg_SetBlend(&params, 12, 4);
    BattleAnimObjWindowBg_Start(system, &params);
}

void BattleAnimFunc_ObjWindowBgScrollDownFast(BattleAnimSystem *system) {
    BattleAnimObjWindowBgParams params;
    BattleAnimObjWindowBg_InitParams(system, &params);
    BattleAnimObjWindowBg_SetScrollVelocity(&params, 0, -6);
    BattleAnimObjWindowBg_SetBlend(&params, 12, 4);
    BattleAnimObjWindowBg_Start(system, &params);
}
