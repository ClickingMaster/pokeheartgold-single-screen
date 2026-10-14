#include "battle/battle_anim_move_effects.h"

#include "global.h"

#include "constants/battle.h"

#include "assert.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "obj_pltt_transfer.h"
#include "pokemon.h"
#include "sprite.h"

// Starts the effect task and runs it once right away. BattleAnimSystem_CreateTaskDefaultPriority is void but leaves
// the new task in r0, which is passed to the task function.
typedef SysTask *(*BattleAnimCreateTaskReturningFunc)(BattleAnimSystem *system, SysTaskFunc func, void *data);
#define BattleAnim_CreateTaskAndRun(system, func, work) func(((BattleAnimCreateTaskReturningFunc)BattleAnimSystem_CreateTaskDefaultPriority)(system, func, work), work)

// BattleAnimScanline_NewWave takes the step as a u8; these callers pass it untruncated (as a u32)
typedef BattleAnimScanlineWave *(*BattleAnimScanlineNewWaveFunc)(u8 firstLine, u8 lastLine, u32 step, fx32 amplitude, s16 speed, int bgLayer, u32 priority, u32 fill, enum HeapID heapId);
#define BattleAnimScanline_NewWaveU32Step ((BattleAnimScanlineNewWaveFunc)BattleAnimScanline_NewWave)

// ManagedSprite_SetDrawPriority takes a u16; these callers pass the priority untruncated
typedef void (*ManagedSpriteSetDrawPriorityFunc)(ManagedSprite *managedSprite, int priority);
#define ManagedSprite_SetDrawPriorityInt ((ManagedSpriteSetDrawPriorityFunc)ManagedSprite_SetDrawPriority)

// BattleAnimUtil_InitScaleMotionXY takes int start scales; BattleAnimFunc_StretchFadeSprite passes them as s16
typedef void (*BattleAnimInitScaleMotionXYS16Func)(BattleAnimLinearMotion *motion, s16 startX, s16 endX, int startY, s16 endY, s16 divisor, int duration);
#define BattleAnimUtil_InitScaleMotionXYS16 ((BattleAnimInitScaleMotionXYS16Func)BattleAnimUtil_InitScaleMotionXY)

// ManagedSprite_SetPositionXY takes s16 coordinates; BattleAnimSpriteFunc_FlashingSpritesAroundAttacker passes ints
typedef void (*ManagedSpriteSetPositionXYIntFunc)(ManagedSprite *managedSprite, int x, int y);
#define ManagedSprite_SetPositionXYInt ((ManagedSpriteSetPositionXYIntFunc)ManagedSprite_SetPositionXY)

// BattleAnimUtil_InitSinMotionFromStep takes u32 start angles; BattleAnimMotionSprite_Update passes the x ones as u16
typedef void (*BattleAnimInitSinMotionFromStepU16Func)(BattleAnimSinMotion *motion, u16 startX, u16 endX, u32 startY, u16 endY, fx32 ampX, fx32 ampY, u16 step);
#define BattleAnimUtil_InitSinMotionFromStepU16 ((BattleAnimInitSinMotionFromStepU16Func)BattleAnimUtil_InitSinMotionFromStep)

// OBJ window inside planes (WINOUT bits 8-13), written like G2_SetWndOutsidePlane
static inline void BattleAnim_SetObjWindowInsidePlane(int wnd, BOOL effect) {
    u32 tmp = (reg_G2_WINOUT & ~REG_G2_WINOUT_OBJWININ_MASK) | ((u32)wnd << REG_G2_WINOUT_OBJWININ_SHIFT);
    if (effect) {
        tmp |= 0x20 << REG_G2_WINOUT_OBJWININ_SHIFT;
    }
    reg_G2_WINOUT = (u16)tmp;
}

typedef struct BattleAnimDropSpriteStepwiseWork {
    BattleAnimCommonWork common;
    ManagedSprite *sprite; // 0x1C
    int spriteIndex;
    int duration;
    int priorityKind; // 0xFF: keep
    int drawPriority; // 0xFF: keep
    BOOL stepwise;
    int windowMode;
    int drops;
    int fallStart;
} BattleAnimDropSpriteStepwiseWork; // size: 0x40

typedef struct BattleAnimAnimateSpritesWork {
    BattleAnimCommonWork common;
    ManagedSprite *sprites[4]; // 0x1C
    int timer;
    u8 unk_30[4];
} BattleAnimAnimateSpritesWork; // size: 0x34

typedef struct BattleAnimBattleBgWaveWork {
    int timer;
    int duration;
    BattleAnimCommonWork common; // 0x08
    BattleAnimScanlineWave *wave;
} BattleAnimBattleBgWaveWork; // size: 0x28

typedef struct BattleAnimBlinkScaleFadeSpritesWork {
    u8 state;
    u8 blinkTimers[3];
    u8 timer;
    u8 eva;
    u8 evb;
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[3];
    BattleAnimLinearMotion scale;      // 0x20
} BattleAnimBlinkScaleFadeSpritesWork; // size: 0x44

typedef struct BattleAnimAttackerGrowFlashRedWork {
    u8 state;
    s16 y;
    int height;
    u8 flashCount;
    BattleAnimSystem *system;
    Pokepic *pokepic;
    BattleAnimLinearMotion scale;     // 0x14
    BattleAnimShake shake;            // 0x38
} BattleAnimAttackerGrowFlashRedWork; // size: 0x5C

typedef struct BattleAnimAttackerSquashStretchWork {
    u8 state;
    s16 y;
    int height;
    BattleAnimSystem *system;
    Pokepic *pokepic;
    BattleAnimLinearMotion scale; // 0x10
    u8 unk_34[0x24];
} BattleAnimAttackerSquashStretchWork; // size: 0x58

typedef struct BattleAnimAttackerAfterimagesWork {
    u8 count;
    u8 state;
    u8 unk_02[2];
    u8 eva;
    u8 evb;
    u8 unk_06[2];
    s16 x;
    s16 y;
    BattleAnimSystem *system;
    Pokepic *pokepic;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[4];
    BattleAnimLinearMotion motions[4]; // 0x28
} BattleAnimAttackerAfterimagesWork;   // size: 0xB8

typedef struct BattleAnimAttackerPulseWhiteWork {
    u8 state;
    u8 count;
    u8 whitened;
    BattleAnimSystem *system;
    Pokepic *pokepic;
    BattleAnimLinearMotion scale;   // 0x0C
} BattleAnimAttackerPulseWhiteWork; // size: 0x30

typedef struct BattleAnimAttackerStretchStepsWork {
    u8 state;
    u8 step;
    s16 y;
    s16 height;
    BattleAnimSystem *system; // 0x08
    Pokepic *pokepic;
    BattleAnimLinearMotion scale;     // 0x10
} BattleAnimAttackerStretchStepsWork; // size: 0x34

typedef struct BattleAnimAttackerLeapOffscreenWork {
    u8 state;
    u8 step;
    s16 y;
    s16 baseY;
    s16 height;
    BattleAnimSystem *system;
    Pokepic *pokepic;
    BattleAnimLinearMotion motion;     // 0x10
} BattleAnimAttackerLeapOffscreenWork; // size: 0x34

// Per sprite: frame from which it blinks, frames per blink toggle
extern const u8 gBattleAnimBlinkScaleFadeTimings[][2];
// Afterimage passes: start offset, end offset, duration (two rows per pass parity)
extern const s16 gBattleAnimAttackerAfterimageMotions[][3];
extern const s16 ov07_02236734[][3];
extern const s16 ov07_02236736[][3];
// Pulses (dim / back): start scale, end scale, duration
extern const u8 gBattleAnimPulseWhiteScales[][3];
extern const u8 ov07_022366CD[][3];
extern const u8 ov07_022366CE[][3];
// Stretch steps: start x scale, end x scale, start y scale, end y scale, duration
extern const u8 gBattleAnimAttackerStretchSteps[][5];
extern const u8 ov07_02236714[][5];
extern const u8 ov07_02236715[][5];
extern const u8 ov07_02236716[][5];
extern const u8 ov07_02236717[][5];
// Leap steps: start x scale, end x scale, start y scale, end y scale, duration
extern const u8 gBattleAnimLeapOffscreenScales[][5];
extern const u8 ov07_022366EC[][5];
extern const u8 ov07_022366ED[][5];
extern const u8 ov07_022366EE[][5];
extern const u8 ov07_022366EF[][5];
// Draw priority of the four effect sprites
extern const int gBattleAnimSpriteDrawPriorities[4];

static inline void BattleAnimTask_AttackerSquashStretchApply(BattleAnimAttackerSquashStretchWork *work) {
    Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
    Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
    Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y + BattleAnimUtil_CalcScaleYOffset(work->y, work->height, work->scale.curY));
}

void BattleAnimFunc_SetBgPaletteGrayscale(BattleAnimSystem *system) {
    if ((int)BattleAnimSystem_GetArg(system, 0) != 0) {
        BattleAnimSystem_ApplyGrayscale(system);
    } else {
        BattleAnimSystem_RestoreFromGrayscale(system);
    }
}

#pragma force_active on
#pragma push
#pragma opt_propagation off

void BattleAnimTask_DropSpriteStepwiseUpdate(void *data) {
    BattleAnimDropSpriteStepwiseWork *work = data;
    s16 x, y;

    switch (work->common.state[0]) {
    case 0:
        if (work->windowMode == 0) {
            GX_SetVisibleWnd(GX_WNDMASK_W0);
            G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3, TRUE);
            G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
            G2_SetWnd0Position(0, 160, 128, 192);
            work->fallStart = LCRandom() % 5 + 35;
        } else {
            GX_SetVisibleWnd(GX_WNDMASK_W0);
            G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3, TRUE);
            G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
            G2_SetWnd0Position(128, 86, 0, 192);
            work->fallStart = LCRandom() % 5 + 35;
        }
        break;
    case 5:
    case 6:
        if (LCRandom() % 2 && work->drops == 0) {
            work->drops++;
            ManagedSprite_OffsetPositionXY(work->sprite, 0, 4);
        }
        break;
    case 7:
        if (work->drops != 1) {
            work->drops++;
            ManagedSprite_OffsetPositionXY(work->sprite, 0, 4);
        }
        break;
    case 10:
    case 11:
        if (LCRandom() % 2 && work->drops == 1) {
            work->drops++;
            ManagedSprite_OffsetPositionXY(work->sprite, 0, 4);
        }
        break;
    case 12:
        if (work->drops != 2) {
            work->drops++;
            ManagedSprite_OffsetPositionXY(work->sprite, 0, 4);
        }
        break;
    case 15:
    case 16:
        if (LCRandom() % 2 && work->drops == 2) {
            work->drops++;
            ManagedSprite_OffsetPositionXY(work->sprite, 0, 4);
        }
        break;
    case 17:
        if (work->drops != 3) {
            work->drops++;
            ManagedSprite_OffsetPositionXY(work->sprite, 0, 4);
        }
        break;
    case 22:
    case 23:
        if (LCRandom() % 2 && work->drops == 3) {
            work->drops++;
            ManagedSprite_OffsetPositionXY(work->sprite, 0, 8);
        }
        break;
    case 24:
        if (work->drops != 4) {
            work->drops++;
            ManagedSprite_OffsetPositionXY(work->sprite, 0, 8);
        }
        break;
    default:
        if (work->common.state[0] > work->fallStart) {
            if (work->drops < 20) {
                ManagedSprite_OffsetPositionXY(work->sprite, 0, 4);
                ManagedSprite_GetPositionXY(work->sprite, &x, &y);
                if (y > 130) {
                    ManagedSprite_SetDrawFlag(work->sprite, FALSE);
                }
                work->drops++;
            } else {
                ManagedSprite_SetDrawFlag(work->sprite, FALSE);
            }
        }
        break;
    }
}

#pragma pop

void BattleAnimTask_DropSpriteStepwise(SysTask *task, void *data) {
    BattleAnimDropSpriteStepwiseWork *work = data;

    if (work->stepwise) {
        BattleAnimTask_DropSpriteStepwiseUpdate(work);
    }
    work->common.state[0]++;
    if (work->common.state[0] >= work->duration) {
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
        G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_NONE, FALSE);
        G2_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE, FALSE);
        G2_SetWnd0Position(0, 0, 0, 0);
        ManagedSprite_SetDrawFlag(work->sprite, FALSE);
        SpriteSystem_DrawSprites(work->common.battlerSpriteManager);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
    } else {
        ManagedSprite_TickFrame(work->sprite);
        SpriteSystem_DrawSprites(work->common.battlerSpriteManager);
    }
}

#pragma force_active reset

void BattleAnimFunc_DropSpriteStepwise(BattleAnimSystem *system) {
    BattleAnimDropSpriteStepwiseWork *work;
    int mode;
    int attacker;
    int defender;

    work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimDropSpriteStepwiseWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->spriteIndex = BattleAnimSystem_GetArg(system, 0);
    work->duration = BattleAnimSystem_GetArg(system, 1);
    work->priorityKind = BattleAnimSystem_GetArg(system, 2);
    work->drawPriority = BattleAnimSystem_GetArg(system, 3);
    work->stepwise = BattleAnimSystem_GetArg(system, 5);
    work->windowMode = BattleAnimSystem_GetArg(system, 6);
    work->sprite = BattleAnimSystem_GetBattlerSprite(work->common.system, work->spriteIndex);
    work->drops = 0;

    if (work->priorityKind != 0xFF) {
        ManagedSprite_SetPriority(work->sprite, BattleAnimSystem_GetBgPriority(system, work->priorityKind));
    }
    if (work->drawPriority != 0xFF) {
        ManagedSprite_SetDrawPriorityInt(work->sprite, work->drawPriority);
    }

    if (BattleAnimSystem_IsDoubleBattle(system) != TRUE) {
        if (BattleAnimSystem_GetArg(system, 4) == 2 || BattleAnimSystem_GetArg(system, 4) == 3) {
            ManagedSprite_SetDrawFlag(work->sprite, FALSE);
            Heap_Free(work);
            return;
        }
    }

    // 0/2: in front of / behind the attacker, 1/3: in front of / behind the defender
    mode = BattleAnimSystem_GetArg(system, 4);
    attacker = BattleAnimSystem_GetAttacker(system);
    defender = BattleAnimSystem_GetDefender(system);
    if (work->drawPriority != 0xFF) {
        attacker = BattleAnimUtil_GetBattlerType(system, attacker);
        defender = BattleAnimUtil_GetBattlerType(system, defender);
        switch (mode) {
        case 0:
            switch (attacker) {
            case BATTLER_TYPE_PLAYER_SIDE_SLOT_1:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            case BATTLER_TYPE_ENEMY_SIDE_SLOT_1:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            case BATTLER_TYPE_PLAYER_SIDE_SLOT_2:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            case BATTLER_TYPE_ENEMY_SIDE_SLOT_2:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            }
            break;
        case 2:
            switch (attacker) {
            case BATTLER_TYPE_PLAYER_SIDE_SLOT_1:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            case BATTLER_TYPE_ENEMY_SIDE_SLOT_1:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            case BATTLER_TYPE_PLAYER_SIDE_SLOT_2:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            case BATTLER_TYPE_ENEMY_SIDE_SLOT_2:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            }
            break;
        case 1:
            switch (defender) {
            case BATTLER_TYPE_PLAYER_SIDE_SLOT_1:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            case BATTLER_TYPE_ENEMY_SIDE_SLOT_1:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            case BATTLER_TYPE_PLAYER_SIDE_SLOT_2:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            case BATTLER_TYPE_ENEMY_SIDE_SLOT_2:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            }
            break;
        case 3:
            switch (defender) {
            case BATTLER_TYPE_PLAYER_SIDE_SLOT_1:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            case BATTLER_TYPE_ENEMY_SIDE_SLOT_1:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            case BATTLER_TYPE_PLAYER_SIDE_SLOT_2:
                ManagedSprite_SetDrawPriority(work->sprite, 20);
                break;
            case BATTLER_TYPE_ENEMY_SIDE_SLOT_2:
                ManagedSprite_SetDrawPriority(work->sprite, 10);
                break;
            }
            break;
        }
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_DropSpriteStepwise, work);
}

void BattleAnimTask_AnimateSprites(SysTask *task, void *data) {
    BattleAnimAnimateSpritesWork *work = data;
    int i;

    if (work->timer == 0) {
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        return;
    }
    work->timer--;
    for (i = 0; i < 4; i++) {
        if (work->sprites[i] != NULL) {
            ManagedSprite_TickFrame(work->sprites[i]);
        }
    }
    SpriteSystem_DrawSprites(work->common.battlerSpriteManager);
}

#pragma force_active on

void BattleAnimFunc_AnimateSprites(BattleAnimSystem *system) {
    BattleAnimAnimateSpritesWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimAnimateSpritesWork));
    int i;
    const int *drawPriorities;

    BattleAnimUtil_InitCommonWork(system, &work->common);
    if ((int)BattleAnimSystem_GetArg(system, 0) == 0) {
        work->timer = 3;
    } else {
        work->timer = BattleAnimSystem_GetArg(system, 0);
    }
    drawPriorities = gBattleAnimSpriteDrawPriorities;
    for (i = 0; i < 4; i++) {
        work->sprites[i] = BattleAnimSystem_GetBattlerSprite(work->common.system, i);
        if (work->sprites[i] != NULL) {
            ManagedSprite_SetDrawPriorityInt(work->sprites[i], drawPriorities[i]);
        }
    }
    BattleAnimTask_AnimateSprites(((BattleAnimCreateTaskReturningFunc)BattleAnimSystem_CreateTaskDefaultPriority)(work->common.system, BattleAnimTask_AnimateSprites, work), work);
}

void BattleAnimTask_BattleBgWave(SysTask *task, void *data) {
    BattleAnimBattleBgWaveWork *work = data;

    switch (work->common.state[0]) {
    case 0: {
        u32 bgLayer;
        u32 fill;
        enum HeapID heapId;

        work->timer = 0;
        bgLayer = BattleAnimSystem_GetBgMaskLayer(work->common.system, 2);
        fill = BattleAnimUtil_PackXY(0, 0);
        heapId = BattleAnimSystem_GetHeapID(work->common.system);
        work->wave = BattleAnimScanline_NewWaveU32Step(0, 160, 341, FX32_CONST(32), 200, bgLayer, 0, fill, heapId);
        work->common.state[0]++;
        break;
    }
    case 1:
        work->timer++;
        if (work->timer < work->duration) {
            break;
        }
        BattleAnimScanline_FreeWave(work->wave);
        work->common.state[0]++;
        // fallthrough
    default:
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        break;
    }
}

#pragma force_active reset

void BattleAnimFunc_BattleBgWave(BattleAnimSystem *system) {
    BattleAnimBattleBgWaveWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimBattleBgWaveWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->duration = BattleAnimSystem_GetArg(system, 0);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_BattleBgWave, work);
}

void BattleAnimTask_BlinkScaleFadeSprites(SysTask *task, void *data) {
    BattleAnimBlinkScaleFadeSpritesWork *work = data;
    f32 scaleX;
    f32 scaleY;
    s16 x;
    s16 y;
    int i;

    switch (work->state) {
    case 0:
        work->timer++;
        for (i = 0; i < (int)BattleAnimSystem_GetArg(work->system, 0); i++) {
            ManagedSprite_GetPositionXY(work->sprites[i], &x, &y);
            if (work->timer >= gBattleAnimBlinkScaleFadeTimings[i][0]) {
                work->blinkTimers[i]++;
                if (work->blinkTimers[i] >= gBattleAnimBlinkScaleFadeTimings[i][1]) {
                    work->blinkTimers[i] = 0;
                    if (ManagedSprite_GetDrawFlag(work->sprites[i]) == TRUE) {
                        ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
                    } else {
                        ManagedSprite_SetDrawFlag(work->sprites[i], TRUE);
                    }
                }
            } else {
                ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
            }
        }
        if (work->timer >= 45) {
            for (i = 0; i < (int)BattleAnimSystem_GetArg(work->system, 0); i++) {
                ManagedSprite_SetDrawFlag(work->sprites[i], TRUE);
            }
            work->timer = 0;
            work->state++;
        }
        break;
    case 1:
        BattleAnimUtil_InitScaleMotionXY(&work->scale, 100, 60, 100, 100, 100, 10);
        work->state++;
        break;
    case 2:
        if (BattleAnimUtil_UpdateScaleMotionXY(&work->scale) == TRUE) {
            for (i = 0; i < (int)BattleAnimSystem_GetArg(work->system, 0); i++) {
                BattleAnimUtil_GetScaleFloat(&work->scale, &scaleX, &scaleY);
                ManagedSprite_SetAffineScale(work->sprites[i], scaleX, scaleY);
            }
        } else {
            work->timer++;
            if (work->timer >= 45) {
                work->state++;
            }
        }
        break;
    case 3:
        if (work->eva != 0) {
            work->eva--;
        }
        if (work->evb < 15) {
            work->evb++;
        }
        if (work->eva == 0 && work->evb == 15) {
            work->state++;
        }
        reg_G2_BLDALPHA = work->eva | (work->evb << 8);
        break;
    default:
        for (i = 0; i < (int)BattleAnimSystem_GetArg(work->system, 0); i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    for (i = 0; i < (int)BattleAnimSystem_GetArg(work->system, 0); i++) {
        Sprite_TickFrame(work->sprites[i]->sprite);
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_BlinkScaleFadeSprites(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;
    BattleAnimPosition pos;
    int i;
    BattleAnimBlinkScaleFadeSpritesWork *work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimBlinkScaleFadeSpritesWork));
    GF_ASSERT(work != NULL);

    work->timer = 0;
    work->state = 0;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->system = system;
    BattleAnimSystem_GetSpriteTemplate(&template, system);
    BattleAnimUtil_SetBlendWithBattleBg(work->system, -1, -1);
    work->eva = 15;
    work->evb = 0;
    reg_G2_BLDALPHA = work->eva | (work->evb << 8);
    work->sprites[0] = sprite;

    if (BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetDefender(system)) == BATTLE_ANIM_SIDE_PLAYER) {
        BattleAnimUtil_GetBattlerTypePosition(BATTLER_TYPE_SOLO_PLAYER, BattleAnimSystem_GetField08(system), &pos);
    } else {
        BattleAnimUtil_GetBattlerTypePosition(BATTLER_TYPE_SOLO_ENEMY, BattleAnimSystem_GetField08(system), &pos);
    }
    for (i = 1; i < (int)BattleAnimSystem_GetArg(work->system, 0); i++) {
        work->sprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        ManagedSprite_SetPositionXY(work->sprites[i], pos.x, pos.y);
    }
    ManagedSprite_SetPositionXY(work->sprites[0], pos.x, pos.y);
    for (i = 0; i < (int)BattleAnimSystem_GetArg(work->system, 0); i++) {
        work->blinkTimers[i] = 0;
        ManagedSprite_SetAffineOverwriteMode(work->sprites[i], NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
        ManagedSprite_OffsetPositionXY(work->sprites[i], 0, 32 - i * 4);
        ManagedSprite_SetOamMode(work->sprites[i], GX_OAM_MODE_XLU);
    }
    BattleAnimSystem_CreateTask(system, BattleAnimTask_BlinkScaleFadeSprites, work, 0x1000);
}

void BattleAnimTask_AttackerGrowFlashRed(SysTask *task, void *data) {
    BattleAnimAttackerGrowFlashRedWork *work = data;

    switch (work->state) {
    case 0:
        if (BattleAnimUtil_UpdateScaleMotion(&work->scale) == TRUE) {
            BattleAnimUtil_UpdateShake(&work->shake);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
            Pokepic_AddAttr(work->pokepic, POKEPIC_X, work->shake.x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y + BattleAnimUtil_CalcScaleYOffset(work->y, work->height, work->scale.curY));
            return;
        }
        work->state++;
        return;
    case 1:
        Pokepic_StartPaletteFade(work->pokepic, 0, 10, 0, 31);
        work->state++;
        return;
    case 2:
        if (Pokepic_ResumePaletteFade(work->pokepic) != FALSE) {
            break;
        }
        Pokepic_StartPaletteFade(work->pokepic, 10, 0, 0, 31);
        work->state++;
        return;
    case 3:
        if (Pokepic_ResumePaletteFade(work->pokepic) != FALSE) {
            break;
        }
        work->flashCount++;
        if (work->flashCount >= 3) {
            int start = BattleAnimSystem_GetArg(work->system, 0);
            int end = BattleAnimSystem_GetArg(work->system, 1);
            BattleAnimUtil_InitScaleMotion(&work->scale, (s16)start, 100, (s16)end, BattleAnimSystem_GetArg(work->system, 3));
            work->state++;
            return;
        }
        work->state = 1;
        return;
    case 4:
        if (BattleAnimUtil_UpdateScaleMotion(&work->scale) == TRUE) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y + BattleAnimUtil_CalcScaleYOffset(work->y, work->height, work->scale.curY));
            return;
        }
        work->state++;
        return;
    case 5:
        if (BattleAnimUtil_UpdateScaleMotion(&work->scale) == TRUE) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y + BattleAnimUtil_CalcScaleYOffset(work->y, work->height, work->scale.curY));
            return;
        }
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, 256);
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, 256);
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y);
        work->state++;
        return;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

void BattleAnimFunc_AttackerGrowFlashRed(BattleAnimSystem *system) {
    int end;
    BattleAnimAttackerGrowFlashRedWork *work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimAttackerGrowFlashRedWork));

    work->flashCount = 0;
    work->state = 0;
    work->system = system;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->height = BattleAnimSystem_GetBattlerHeight(work->system, BattleAnimSystem_GetAttacker(work->system));
    end = BattleAnimSystem_GetArg(system, 0);
    BattleAnimUtil_InitScaleMotion(&work->scale, 100, 100, (s16)end, BattleAnimSystem_GetArg(system, 2));
    BattleAnimUtil_InitShake(&work->shake, 2, 0, 0, 10);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerGrowFlashRed, work);
}

#pragma force_active on

void BattleAnimTask_AttackerSquashStretch(SysTask *task, void *data) {
    BattleAnimAttackerSquashStretchWork *work = data;

    switch (work->state) {
    case 0:
        if (BattleAnimUtil_UpdateScaleMotionXY(&work->scale) == TRUE) {
            BattleAnimTask_AttackerSquashStretchApply(work);
            return;
        }
        BattleAnimUtil_InitScaleMotionXY(&work->scale, 150, 50, 50, 150, 100, 10);
        work->state++;
        return;
    case 1:
        if (BattleAnimUtil_UpdateScaleMotionXY(&work->scale) == TRUE) {
            BattleAnimTask_AttackerSquashStretchApply(work);
            return;
        }
        BattleAnimUtil_InitScaleMotionXY(&work->scale, 50, 100, 150, 100, 100, 5);
        work->state++;
        return;
    case 2:
        if (BattleAnimUtil_UpdateScaleMotionXY(&work->scale) == TRUE) {
            BattleAnimTask_AttackerSquashStretchApply(work);
            return;
        }
        BattleAnimUtil_InitScaleMotionXY(&work->scale, 100, 150, 100, 150, 100, 5);
        work->state++;
        return;
    case 3:
        if (BattleAnimUtil_UpdateScaleMotionXY(&work->scale) == TRUE) {
            BattleAnimTask_AttackerSquashStretchApply(work);
            return;
        }
        BattleAnimUtil_InitScaleMotionXY(&work->scale, 150, 100, 150, 100, 100, 5);
        work->state++;
        return;
    case 4:
        if (BattleAnimUtil_UpdateScaleMotionXY(&work->scale) == TRUE) {
            BattleAnimTask_AttackerSquashStretchApply(work);
            return;
        }
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, 256);
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, 256);
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y);
        work->state++;
        return;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
}

#pragma force_active reset

void BattleAnimFunc_AttackerSquashStretch(BattleAnimSystem *system) {
    BattleAnimAttackerSquashStretchWork *work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimAttackerSquashStretchWork));

    work->state = 0;
    work->system = system;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->height = BattleAnimSystem_GetBattlerHeight(work->system, BattleAnimSystem_GetAttacker(work->system));
    BattleAnimUtil_InitScaleMotionXY(&work->scale, 100, 150, 100, 50, 100, 10);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerSquashStretch, work);
}

#pragma force_active on

void BattleAnimTask_AttackerAfterimages(SysTask *task, void *data) {
    BattleAnimAttackerAfterimagesWork *work = data;
    int i;
    u8 done;
    int row;
    int delay;
    int duration;
    int duration2;

    switch (work->state) {
    case 0:
        // each pass is a little faster than the previous one
        row = (work->count % 2) * 2;
        delay = (work->count + 1) / 2;
        duration = ov07_02236736[row][0] - delay;
        BattleAnimUtil_InitLinearMotion(&work->motions[0], work->x + gBattleAnimAttackerAfterimageMotions[row][0], work->x + ov07_02236734[row][0], work->y, work->y, duration);
        BattleAnimUtil_InitLinearMotion(&work->motions[1], work->x - gBattleAnimAttackerAfterimageMotions[row][0], work->x - ov07_02236734[row][0], work->y, work->y, duration);
        duration2 = ov07_02236736[row + 1][0] - delay;
        BattleAnimUtil_InitLinearMotion(&work->motions[2], work->x + gBattleAnimAttackerAfterimageMotions[row + 1][0], work->x + ov07_02236734[row + 1][0], work->y, work->y, duration2);
        BattleAnimUtil_InitLinearMotion(&work->motions[3], work->x - gBattleAnimAttackerAfterimageMotions[row + 1][0], work->x - ov07_02236734[row + 1][0], work->y, work->y, duration2);
        work->count++;
        work->state++;
        // fallthrough
    case 1:
        done = 0;
        for (i = 0; i < 4; i++) {
            if (BattleAnimUtil_UpdateLinearMotion(&work->motions[i]) == FALSE) {
                done++;
            } else {
                ManagedSprite_SetPositionXY(work->sprites[i], work->motions[i].x, work->motions[i].y);
            }
            ManagedSprite_TickFrame(work->sprites[i]);
        }
        if (done == 4) {
            if (work->count == 9) {
                work->state++;
            } else {
                work->state = 0;
            }
        }
        break;
    case 2:
        if (work->eva) {
            work->eva--;
        }
        if (work->evb < 15) {
            work->evb++;
        }
        if (work->eva == 0 && work->evb == 15) {
            work->state++;
        }
        reg_G2_BLDALPHA = work->eva | (work->evb << 8);
        break;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

#pragma force_active reset

void BattleAnimFunc_AttackerAfterimages(BattleAnimSystem *system) {
    BattleAnimAttackerAfterimagesWork *work;
    int i;
    int priority;
    int paletteMember;
    u16 paletteOffset;
    NarcId narcId;

    work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimAttackerAfterimagesWork));
    work->state = 0;
    work->count = 0;
    work->system = system;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->y -= Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H);
    work->spriteManager = BattleAnimSystem_GetBattlerSpriteManager(work->system);
    work->eva = 8;
    work->evb = 6;
    BattleAnimUtil_SetBlendWithBattleBg(work->system, -1, -1);
    reg_G2_BLDALPHA = work->eva | (work->evb << 8);
    paletteMember = BattleAnimSystem_GetBattlerPaletteId(work->system, BattleAnimSystem_GetAttacker(work->system));
    priority = BattleAnimSystem_GetBasePriority(work->system);
    narcId = (NarcId)BattleAnimSystem_GetBattlerNarcId(work->system, BattleAnimSystem_GetAttacker(work->system));
    for (i = 0; i < 4; i++) {
        work->sprites[i] = BattleAnimSystem_GetBattlerSprite(work->system, i);
        ManagedSprite_SetOamMode(work->sprites[i], GX_OAM_MODE_XLU);
    }

    // the afterimages behind the attacker are tinted darker than the ones in front of it
    if (BattleAnimUtil_GetBattlerSide(work->system, BattleAnimSystem_GetAttacker(work->system)) == BATTLE_ANIM_SIDE_PLAYER) {
        ManagedSprite_SetDrawPriority(work->sprites[0], 10);
        ManagedSprite_SetDrawPriority(work->sprites[1], 10);
        ManagedSprite_SetDrawPriority(work->sprites[2], 20);
        ManagedSprite_SetDrawPriority(work->sprites[3], 20);
        ManagedSprite_SetPriority(work->sprites[0], priority);
        ManagedSprite_SetPriority(work->sprites[1], priority);
        ManagedSprite_SetPriority(work->sprites[2], priority);
        ManagedSprite_SetPriority(work->sprites[3], priority);
        paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(work->sprites[0]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
        PaletteData_LoadNarc_CustomTint(BattleAnimSystem_GetPaletteData(work->system), narcId, paletteMember, BattleAnimSystem_GetHeapID(system), PLTTBUF_MAIN_OBJ, 0x20, paletteOffset << 4, 0x80, 0x80, 0x80);
        paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(work->sprites[2]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
        PaletteData_LoadNarc_CustomTint(BattleAnimSystem_GetPaletteData(work->system), narcId, paletteMember, BattleAnimSystem_GetHeapID(system), PLTTBUF_MAIN_OBJ, 0x20, paletteOffset << 4, 0xC4, 0xC4, 0xC4);
    } else {
        ManagedSprite_SetDrawPriority(work->sprites[0], 20);
        ManagedSprite_SetDrawPriority(work->sprites[1], 20);
        ManagedSprite_SetDrawPriority(work->sprites[2], 10);
        ManagedSprite_SetDrawPriority(work->sprites[3], 10);
        ManagedSprite_SetPriority(work->sprites[0], priority + 1);
        ManagedSprite_SetPriority(work->sprites[1], priority + 1);
        ManagedSprite_SetPriority(work->sprites[2], priority + 1);
        ManagedSprite_SetPriority(work->sprites[3], priority + 1);
        ManagedSprite_SetPriority(work->sprites[0], priority);
        ManagedSprite_SetPriority(work->sprites[1], priority);
        ManagedSprite_SetPriority(work->sprites[2], priority);
        ManagedSprite_SetPriority(work->sprites[3], priority);
        paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(work->sprites[0]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
        PaletteData_LoadNarc_CustomTint(BattleAnimSystem_GetPaletteData(work->system), narcId, paletteMember, BattleAnimSystem_GetHeapID(system), PLTTBUF_MAIN_OBJ, 0x20, paletteOffset << 4, 0xC4, 0xC4, 0xC4);
        paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(work->sprites[2]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
        PaletteData_LoadNarc_CustomTint(BattleAnimSystem_GetPaletteData(work->system), narcId, paletteMember, BattleAnimSystem_GetHeapID(system), PLTTBUF_MAIN_OBJ, 0x20, paletteOffset << 4, 0x80, 0x80, 0x80);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerAfterimages, work);
}

#pragma force_active on

void BattleAnimTask_AttackerPulseWhite(SysTask *task, void *data) {
    BattleAnimAttackerPulseWhiteWork *work = data;

    switch (work->state) {
    case 0: {
        int i = work->whitened;
        BattleAnimUtil_InitScaleMotion(&work->scale, gBattleAnimPulseWhiteScales[i][0], 100, ov07_022366CD[i][0], ov07_022366CE[i][0]);
        if (work->whitened == FALSE) {
            Pokepic_StartPaletteFade(work->pokepic, 0, 6, 0, 0x7FFF);
        } else {
            Pokepic_StartPaletteFade(work->pokepic, 6, 0, 0, 0x7FFF);
        }
        work->whitened ^= TRUE;
        work->count++;
        work->state++;
        break;
    }
    case 1:
        if (BattleAnimUtil_UpdateScaleMotion(&work->scale) == FALSE && Pokepic_ResumePaletteFade(work->pokepic) == FALSE) {
            if (work->count < 4) {
                work->state--;
            } else {
                work->state++;
            }
        }
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
        break;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

#pragma force_active reset

void BattleAnimFunc_AttackerPulseWhite(BattleAnimSystem *system) {
    BattleAnimAttackerPulseWhiteWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimAttackerPulseWhiteWork));
    work->state = 0;
    work->whitened = FALSE;
    work->count = 0;
    work->system = system;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(work->system));
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerPulseWhite, work);
}

#pragma force_active on

void BattleAnimTask_AttackerStretchSteps(SysTask *task, void *data) {
    BattleAnimAttackerStretchStepsWork *work = data;

    switch (work->state) {
    case 0:
        BattleAnimUtil_InitScaleMotionXY(&work->scale, gBattleAnimAttackerStretchSteps[work->step][0], ov07_02236714[work->step][0], ov07_02236715[work->step][0], ov07_02236716[work->step][0], 100, ov07_02236717[work->step][0]);
        work->step++;
        work->state++;
        break;
    case 1:
        if (BattleAnimUtil_UpdateScaleMotionXY(&work->scale) == FALSE) {
            if (work->step < 3) {
                work->state--;
            } else {
                work->state++;
            }
        }
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y + BattleAnimUtil_CalcScaleYOffset(work->y, work->height, work->scale.curY));
        break;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

#pragma force_active reset

void BattleAnimFunc_AttackerStretchSteps(BattleAnimSystem *system) {
    BattleAnimAttackerStretchStepsWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimAttackerStretchStepsWork));
    work->state = 0;
    work->step = 0;
    work->system = system;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->height = BattleAnimSystem_GetBattlerHeight(work->system, BattleAnimSystem_GetAttacker(work->system));
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerStretchSteps, work);
}

#pragma force_active on

void BattleAnimTask_AttackerLeapOffscreen(SysTask *task, void *data) {
    BattleAnimAttackerLeapOffscreenWork *work = data;

    switch (work->state) {
    case 0: {
        int i = work->step;
        BattleAnimUtil_InitScaleMotionXY(&work->motion, gBattleAnimLeapOffscreenScales[i][0], ov07_022366EC[i][0], ov07_022366ED[i][0], ov07_022366EE[i][0], 100, ov07_022366EF[i][0]);
        work->step++;
        work->state++;
        break;
    }
    case 1:
        if (!BattleAnimUtil_UpdateScaleMotionXY(&work->motion)) {
            work->state++;
        }
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->motion.x);
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->motion.y);
        BattleAnimUtil_AlignPokepicToScale(work->pokepic, work->y, work->height, work->motion.curY, 0);
        break;
    case 2:
        BattleAnimUtil_InitLinearMotion(&work->motion, 0, 0, Pokepic_GetAttr(work->pokepic, POKEPIC_Y), 0, 5);
        work->state++;
        break;
    case 3:
        if (!BattleAnimUtil_UpdateLinearMotion(&work->motion)) {
            work->state++;
        }
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->motion.y);
        break;
    case 4: {
        int i = work->step;
        BattleAnimUtil_InitScaleMotionXY(&work->motion, gBattleAnimLeapOffscreenScales[i][0], ov07_022366EC[i][0], ov07_022366ED[i][0], ov07_022366EE[i][0], 100, ov07_022366EF[i][0]);
        work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
        work->state++;
        break;
    }
    case 5:
        if (!BattleAnimUtil_UpdateScaleMotionXY(&work->motion)) {
            work->state++;
        }
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->motion.x);
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->motion.y);
        BattleAnimUtil_AlignPokepicToScale(work->pokepic, work->y, work->height, work->motion.curY, 1);
        break;
    default:
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->baseY);
        Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, TRUE);
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, 256);
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, 256);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

#pragma force_active reset

void BattleAnimFunc_AttackerLeapOffscreen(BattleAnimSystem *system) {
    BattleAnimAttackerLeapOffscreenWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimAttackerLeapOffscreenWork));
    work->state = 0;
    work->step = 0;
    work->system = system;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->baseY = work->y;
    work->height = BattleAnimSystem_GetBattlerHeight(work->system, BattleAnimSystem_GetAttacker(work->system));
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerLeapOffscreen, work);
}

typedef struct BattleAnimAttackerSilhouetteWork {
    u8 state;
    u8 timer;
    u8 unk_02[2];
    PaletteData *paletteData;
    BattleAnimSystem *system;
    u32 unk_0C;
    Pokepic *pokepic;
} BattleAnimAttackerSilhouetteWork; // size: 0x14

typedef struct BattleAnimAttackerHopWork {
    u8 state;
    u8 hop; // row of the hop tables
    u8 round;
    u8 unk_03;
    s16 y;
    s16 height;
    BattleAnimSystem *system;
    Pokepic *pokepic;
    BattleAnimLinearMotion scale;  // 0x10
    BattleAnimLinearMotion offset; // 0x34
} BattleAnimAttackerHopWork;       // size: 0x58

typedef struct BattleAnimShowAttackerObjWindowSpritesWork {
    vu8 state; // volatile: needed to match
    vu8 timer;
    u8 unk_02[2];
    BattleAnimSystem *system;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[2];                // [1]: drawn as the OBJ window
} BattleAnimShowAttackerObjWindowSpritesWork; // size: 0x14

typedef struct BattleAnimSquashAfterimage {
    u8 state;
    u8 timer;
    u8 unk_02[2];
    ManagedSprite *sprite;
    BattleAnimLinearMotion scale; // 0x08
} BattleAnimSquashAfterimage;     // size: 0x2C

typedef struct BattleAnimAttackerSquashAfterimagesWork {
    u8 state;
    u8 round;
    s16 y;
    int height;
    s8 direction; // sign of the x scale
    u8 unk_09[3];
    Pokepic *pokepic; // 0x0C
    BattleAnimSystem *system;
    SpriteManager *spriteManager;
    BattleAnimSquashAfterimage afterimages[3]; // 0x18
} BattleAnimAttackerSquashAfterimagesWork;     // size: 0x9C

// BattleAnimTarget whose first word holds the pic's position before the shake
typedef struct BattleAnimShakeTarget {
    s16 x;
    s16 y;
    u32 unk_04;
    Pokepic *pokepic;
    ManagedSprite *sprite;
    int battler;
} BattleAnimShakeTarget; // size: 0x14

typedef struct BattleAnimShakeBattlefieldFlashingWork {
    u8 state;
    u8 shakeCount;
    u8 white;
    u8 flashFrames;
    int targetCount;
    BgConfig *bgConfig;
    BattleAnimSystem *system;
    SpriteManager *spriteManager;
    BattleAnimShakeTarget targets[4]; // 0x14
    u8 unk_64[0x50];
    BattleAnimShake shake;                // 0xB4
} BattleAnimShakeBattlefieldFlashingWork; // size: 0xD8

typedef struct BattleAnimSpriteDriftFadeWork {
    u8 state;
    u8 unk_01;
    u8 unk_02[2];
    s16 x; // 0x04
    s16 y;
    BgConfig *bgConfig;
    BattleAnimSystem *system;
    SpriteManager *spriteManager;
    Pokepic *pokepic;                 // 0x14
    ManagedSprite *sprite;            // 0x18
    BattleAnimShake shake;            // 0x1C
    BattleAnimLinearMotion motion;    // 0x40
    BattleAnimBlendAlphaMotion blend; // 0x64
} BattleAnimSpriteDriftFadeWork;      // size: 0x8C

typedef struct BattleAnimRockOrShakeAttackerWork {
    BattleAnimCommonWork common; // common.state: [0] state, [1] swing direction, [2] swings done
    s16 shakeAmpX;               // 0x1C
    s16 shakeAmpY;
    s16 shakeInterval;
    s16 shakeCount;
    int unk_24;
    Pokepic *attackerPic; // 0x28
    Pokepic *defenderPic;
    u8 unk_30[8];
    BattleAnimShake shake;           // 0x38
    BattleAnimValueMotion rotation;  // 0x5C
} BattleAnimRockOrShakeAttackerWork; // size: 0x70

typedef struct BattleAnimShakeBattlefieldByPowerWork {
    BattleAnimCommonWork common;
    u8 unk_1C[0xC];
    s16 positions[4][2];                 // 0x28
    Pokepic *pokepics[4];                // 0x38
    int amplitude;                       // 0x48
    BattleAnimShake shake;               // 0x4C
} BattleAnimShakeBattlefieldByPowerWork; // size: 0x70

typedef struct BattleAnimShakeSpritePairWork {
    BattleAnimCommonWork common;
    u8 unk_1C[0xC];
    s16 attackerPos[2]; // 0x28
    s16 defenderPos[2];
    Pokepic *attackerPic; // 0x30
    Pokepic *defenderPic;
    ManagedSprite *sprites[2];   // 0x38
    BattleAnimShake shake;       // 0x40
    BattleAnimShake jitter;      // 0x64
} BattleAnimShakeSpritePairWork; // size: 0x88

typedef struct BattleAnimThrowSpriteArcWork {
    u8 unk_00[4];
    int duration;
    int height;
    s16 offsetX; // 0x0C, added to the defender's position
    s16 offsetY;
    BattleAnimCommonWork common; // 0x10
    u8 unk_2C[0xC];
    ManagedSprite *sprite;     // 0x38
    BattleAnimTarget attacker; // 0x3C
    BattleAnimTarget defender; // 0x50
    u8 unk_64[0x28];
    BattleAnimLinearMotion path;    // 0x8C
    BattleAnimSinMotion arc;        // 0xB0
    BattleAnimValueMotion rotation; // 0xD4
} BattleAnimThrowSpriteArcWork;     // size: 0xE8

// [row][0]: x scale, y scale, ... (5 per row)
extern const u8 gBattleAnimAttackerHopScales[];
extern const u8 ov07_022366F6[];
extern const u8 ov07_022366F7[];
extern const u8 ov07_022366F8[];
extern const u8 ov07_022366F9[];
// [row * 3]: hop start y, end y, duration
extern const u8 gBattleAnimAttackerHopOffsets[];
extern const u8 ov07_022366D9[];
extern const u8 ov07_022366DA[];
extern const u8 gBattleAnimSquashAfterimageDelays[];
extern const s16 gBattleAnimShakeBattlefieldAmplitudes[];
// {angle, duration} pairs; ov07_02236750 is the duration column
extern const int gBattleAnimRockAttackerSteps[][2];
extern const int ov07_02236750[][2];

#pragma force_active on

void BattleAnimTask_AttackerSilhouette(SysTask *task, void *data) {
    BattleAnimAttackerSilhouetteWork *work = data;

    switch (work->state) {
    case 0:
        PaletteData_BeginPaletteFade(work->paletteData, PLTTBUF_MAIN_BG_F, (u16)BattleAnimSystem_GetBattleBgPaletteMask(work->system), -2, 0, 16, 0x7FFF);
        Pokepic_StartPaletteFade(work->pokepic, 0, 16, 0, 0);
        work->state++;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(work->paletteData) == 0 && Pokepic_ResumePaletteFade(work->pokepic) == FALSE) {
            work->state++;
        }
        break;
    case 2:
        work->timer++;
        if (work->timer > 5) {
            work->state++;
        }
        break;
    case 3:
        PaletteData_BeginPaletteFade(work->paletteData, PLTTBUF_MAIN_BG_F, (u16)BattleAnimSystem_GetBattleBgPaletteMask(work->system), -2, 16, 0, 0x7FFF);
        Pokepic_StartPaletteFade(work->pokepic, 16, 0, 0, 0);
        work->state++;
        break;
    case 4:
        if (PaletteData_GetSelectedBuffersBitmask(work->paletteData) == 0 && Pokepic_ResumePaletteFade(work->pokepic) == FALSE) {
            work->state++;
        }
        break;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

#pragma force_active reset

void BattleAnimFunc_AttackerSilhouette(BattleAnimSystem *system) {
    BattleAnimAttackerSilhouetteWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimAttackerSilhouetteWork));

    work->system = system;
    work->paletteData = BattleAnimSystem_GetPaletteData(work->system);
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(work->system));
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerSilhouette, work);
}

#pragma force_active on
#pragma push
#pragma opt_common_subs off

void BattleAnimTask_AttackerHop(SysTask *task, void *data) {
    BattleAnimAttackerHopWork *work = data;

    switch (work->state) {
    case 0: {
        int row = work->hop * 5;
        BattleAnimUtil_InitScaleMotionXY(&work->scale, gBattleAnimAttackerHopScales[row], ov07_022366F6[row], ov07_022366F7[row], ov07_022366F8[row], 100, ov07_022366F9[row]);
        row = work->hop * 3;
        {
            int y = work->y;
            BattleAnimUtil_InitLinearMotion(&work->offset, 0, 0, (s16)(y + gBattleAnimAttackerHopOffsets[row]), (s16)(y + ov07_022366D9[row]), ov07_022366DA[row]);
        }
        work->hop++;
        work->state++;
        break;
    }
    case 1: {
        int done = 0;
        if (!BattleAnimUtil_UpdateScaleMotionXY(&work->scale)) {
            done++;
        }
        if (!BattleAnimUtil_UpdateLinearMotion(&work->offset)) {
            done++;
        }
        if (done >= 2) {
            if (work->hop >= 3) {
                work->round++;
                work->hop = 0;
                if (work->round >= 3) {
                    work->state++;
                }
            } else {
                work->state = 0;
            }
        }
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
        BattleAnimUtil_AlignPokepicToScale(work->pokepic, work->offset.y, work->height, work->scale.curY, 0);
        break;
    }
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

#pragma pop
#pragma force_active reset

void BattleAnimFunc_AttackerHop(BattleAnimSystem *system) {
    BattleAnimAttackerHopWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimAttackerHopWork));

    work->state = 0;
    work->hop = 0;
    work->system = system;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->height = BattleAnimSystem_GetBattlerHeight(work->system, BattleAnimSystem_GetAttacker(work->system));
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerHop, work);
}

#pragma force_active on

void BattleAnimTask_ShowAttackerObjWindowSprites(SysTask *task, void *data) {
    BattleAnimShowAttackerObjWindowSpritesWork *work = data;

    switch (work->state) {
    case 0:
        work->timer++;
        if (work->timer >= 10) {
            work->timer = 0;
            work->state++;
        }
        break;
    case 1:
        ManagedSprite_SetDrawFlag(work->sprites[0], TRUE);
        ManagedSprite_SetDrawFlag(work->sprites[1], TRUE);
        ManagedSprite_SetOamMode(work->sprites[1], GX_OAM_MODE_OBJWND);
        work->state++;
        break;
    case 2:
        work->timer++;
        if (work->timer >= 10) {
            G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
            BattleAnim_SetObjWindowInsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
            ManagedSprite_SetDrawFlag(work->sprites[0], FALSE);
            ManagedSprite_SetDrawFlag(work->sprites[1], FALSE);
            work->timer = 0;
            work->state++;
        }
        break;
    default:
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    ManagedSprite_TickFrame(work->sprites[0]);
    ManagedSprite_TickFrame(work->sprites[1]);
    SpriteSystem_DrawSprites(work->spriteManager);
}

#pragma force_active reset

void BattleAnimFunc_ShowAttackerObjWindowSprites(BattleAnimSystem *system) {
    int member;
    NarcId narcId;
    u32 paletteOffset;
    PaletteData *paletteData;
    BattleAnimShowAttackerObjWindowSpritesWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimShowAttackerObjWindowSpritesWork));

    work->system = system;
    work->spriteManager = BattleAnimSystem_GetBattlerSpriteManager(system);
    work->sprites[0] = BattleAnimSystem_GetBattlerSprite(work->system, 0);
    work->sprites[1] = BattleAnimSystem_GetBattlerSprite(work->system, 1);
    GX_SetVisibleWnd(GX_WNDMASK_OW);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
    BattleAnim_SetObjWindowInsidePlane(GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_OBJ, FALSE);
    member = BattleAnimSystem_GetBattlerPaletteId(work->system, BattleAnimSystem_GetAttacker(work->system));
    narcId = (NarcId)BattleAnimSystem_GetBattlerNarcId(work->system, BattleAnimSystem_GetAttacker(work->system));
    paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(work->sprites[0]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    paletteData = BattleAnimSystem_GetPaletteData(work->system);
    PaletteData_LoadNarc_CustomTint(paletteData, narcId, member, BattleAnimSystem_GetHeapID(system), PLTTBUF_MAIN_OBJ, 0x20, (u16)(paletteOffset * 16), 0x100, 0x100, 0x100);
    ManagedSprite_SetDrawFlag(work->sprites[0], FALSE);
    ManagedSprite_SetDrawFlag(work->sprites[1], FALSE);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_ShowAttackerObjWindowSprites, work);
}

#pragma force_active on

void BattleAnimTask_AttackerSquashAfterimages(SysTask *task, void *data) {
    BattleAnimAttackerSquashAfterimagesWork *work = data;

    switch (work->state) {
    case 0:
        Pokepic_SetAttr(work->pokepic, POKEPIC_VISIBLE, TRUE);
        work->state++;
        break;
    case 1: {
        int i;
        int done = 0;
        f32 scaleX;
        f32 scaleY;
        for (i = 0; i < 3; i++) {
            switch (work->afterimages[i].state) {
            case 0:
                work->afterimages[i].timer++;
                if (work->afterimages[i].timer >= gBattleAnimSquashAfterimageDelays[i]) {
                    BattleAnimUtil_InitScaleMotionXY(&work->afterimages[i].scale, 100, 5, 100, 5, 100, 5);
                    work->afterimages[i].state++;
                }
                break;
            case 1:
                if (BattleAnimUtil_UpdateScaleMotionXY(&work->afterimages[i].scale) == FALSE) {
                    work->afterimages[i].state++;
                } else {
                    BattleAnimUtil_GetScaleFloat(&work->afterimages[i].scale, &scaleX, &scaleY);
                    ManagedSprite_SetAffineScale(work->afterimages[i].sprite, scaleX * work->direction, scaleY);
                    BattleAnimUtil_AlignSpriteToScale(work->afterimages[i].sprite, work->y, (s16)work->height, work->afterimages[i].scale.curY, 0);
                }
                break;
            default:
                done++;
                break;
            }
            ManagedSprite_TickFrame(work->afterimages[i].sprite);
        }
        if (done < 3) {
            break;
        }
        work->round++;
        if (work->round < 3) {
            s16 x;
            s16 y;
            for (i = 0; i < 3; i++) {
                ManagedSprite_GetPositionXY(work->afterimages[i].sprite, &x, &y);
                ManagedSprite_SetPositionXY(work->afterimages[i].sprite, x, work->y);
                ManagedSprite_SetAffineScale(work->afterimages[i].sprite, work->direction, 1.0f);
                work->afterimages[i].state = 0;
                work->afterimages[i].timer = 0;
            }
            work->state--;
        } else {
            work->state++;
        }
        break;
    }
    case 2: {
        int i;
        for (i = 0; i < 3; i++) {
            ManagedSprite_SetOamMode(work->afterimages[i].sprite, GX_OAM_MODE_NORMAL);
            BattleAnimUtil_InitScaleMotionXY(&work->afterimages[i].scale, 5, 100, 5, 100, 100, 5);
        }
        work->state++;
        break;
    }
    case 3: {
        int done = 0;
        int i;
        f32 scaleX;
        f32 scaleY;
        for (i = 0; i < 3; i++) {
            if (BattleAnimUtil_UpdateScaleMotionXY(&work->afterimages[i].scale) == FALSE) {
                done++;
            } else {
                BattleAnimUtil_GetScaleFloat(&work->afterimages[i].scale, &scaleX, &scaleY);
                ManagedSprite_SetAffineScale(work->afterimages[i].sprite, scaleX * work->direction, scaleY);
                BattleAnimUtil_AlignSpriteToScale(work->afterimages[i].sprite, work->y, (s16)work->height, work->afterimages[i].scale.curY, 0);
            }
        }
        if (done >= 3) {
            work->state++;
        }
        break;
    }
    default:
        Pokepic_SetAttr(work->pokepic, POKEPIC_VISIBLE, FALSE);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

#pragma force_active reset

void BattleAnimFunc_AttackerSquashAfterimages(BattleAnimSystem *system) {
    int i;
    BattleAnimAttackerSquashAfterimagesWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimAttackerSquashAfterimagesWork));

    work->state = 0;
    work->round = 0;
    work->system = system;
    work->spriteManager = BattleAnimSystem_GetBattlerSpriteManager(system);
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->y = work->y - Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H);
    work->height = BattleAnimSystem_GetBattlerHeight(work->system, BattleAnimSystem_GetAttacker(work->system));
    BattleAnimUtil_SetBlendWithBattleBg(work->system, -1, -1);
    reg_G2_BLDALPHA = 12 | (6 << 8);
    for (i = 0; i < 3; i++) {
        work->afterimages[i].state = 0;
        work->afterimages[i].timer = 0;
        work->afterimages[i].sprite = BattleAnimSystem_GetBattlerSprite(work->system, i);
        ManagedSprite_SetAffineOverwriteMode(work->afterimages[i].sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
        ManagedSprite_SetOamMode(work->afterimages[i].sprite, GX_OAM_MODE_XLU);
        ManagedSprite_SetDrawPriorityInt(work->afterimages[i].sprite, i + 1);
    }
    if (BattleAnimSystem_ShouldFlipSide(system, 0) == TRUE) {
        work->direction = -1;
    } else {
        work->direction = 1;
    }
    for (i = 0; i < 3; i++) {
        ManagedSprite_SetAffineScale(work->afterimages[i].sprite, work->direction, 1.0f);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerSquashAfterimages, work);
}

#pragma force_active on

void BattleAnimTask_ShakeBattlefieldFlashing(SysTask *task, void *data) {
    int i;
    BattleAnimShakeBattlefieldFlashingWork *work = data;

    switch (work->state) {
    case 0: {
        PaletteData *paletteData = BattleAnimSystem_GetPaletteData(work->system);
        u32 mask = BattleAnimSystem_GetBattleBgPaletteMask(work->system);
        PaletteData_BeginPaletteFade(paletteData, PLTTBUF_MAIN_BG_F, (u16)mask, 1, 0, 10, 0);
        work->state++;
        break;
    }
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(BattleAnimSystem_GetPaletteData(work->system))) {
            break;
        }
        work->state++;
    case 2:
        BattleAnimUtil_InitShake(&work->shake, gBattleAnimShakeBattlefieldAmplitudes[work->shakeCount], 0, 0, 5);
        if (!work->white) {
            PaletteData_BlendPalettes(BattleAnimSystem_GetPaletteData(work->system), PLTTBUF_MAIN_BG, 0xFF, 10, 0);
        } else {
            PaletteData_BlendPalettes(BattleAnimSystem_GetPaletteData(work->system), PLTTBUF_MAIN_BG, 0xFF, 10, 0x7FFF);
        }
        work->flashFrames = 3;
        work->white ^= 1;
        work->shakeCount++;
        work->state++;
    case 3:
        if (!BattleAnimUtil_UpdateShake(&work->shake)) {
            if (work->shakeCount >= 8) {
                work->state++;
            } else {
                work->state--;
            }
        }
        if (work->flashFrames) {
            work->flashFrames--;
            if (!work->flashFrames) {
                PaletteData_BlendPalettes(BattleAnimSystem_GetPaletteData(work->system), PLTTBUF_MAIN_BG, 0xFF, 0, 0);
            }
        }
        for (i = 0; i < 4; i++) {
            if (work->targets[i].pokepic) {
                Pokepic_SetAttr(work->targets[i].pokepic, POKEPIC_X, work->shake.x + work->targets[i].x);
            }
        }
        BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, work->shake.x);
        break;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

#pragma force_active reset

void BattleAnimFunc_ShakeBattlefieldFlashing(BattleAnimSystem *system) {
    u32 flags;
    int i;
    BattleAnimShakeBattlefieldFlashingWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimShakeBattlefieldFlashingWork));

    work->state = 0;
    work->shakeCount = 0;
    work->system = system;
    work->spriteManager = BattleAnimSystem_GetBattlerSpriteManager(system);
    work->bgConfig = BattleAnimSystem_GetBgConfig(work->system);
    if (BattleAnimSystem_IsDoubleBattle(work->system) == TRUE) {
        flags = 0x40;
    } else {
        flags = 0xA;
    }
    BattleAnimUtil_CollectPicsByFlags(system, flags, (BattleAnimTarget *)work->targets, &work->targetCount);
    for (i = 0; i < work->targetCount; i++) {
        work->targets[i].x = Pokepic_GetAttr(work->targets[i].pokepic, POKEPIC_X);
        work->targets[i].y = Pokepic_GetAttr(work->targets[i].pokepic, POKEPIC_Y);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_ShakeBattlefieldFlashing, work);
}

void BattleAnimTask_SpriteDriftFade(SysTask *task, void *data) {
    BattleAnimSpriteDriftFadeWork *work = data;

    if (work->state == 0) {
        if (BattleAnimUtil_UpdateLinearMotion(&work->motion) == FALSE) {
            work->state++;
        }
        BattleAnimUtil_UpdateShake(&work->shake);
        BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend);
        ManagedSprite_SetPositionXY(work->sprite, work->motion.x, work->motion.y);
        ManagedSprite_TickFrame(work->sprite);
        SpriteSystem_DrawSprites(work->spriteManager);
        return;
    }
    BattleAnimSystem_DestroyTask(work->system, task);
    Heap_Free(work);
}

#pragma force_active on

void BattleAnimFunc_SpriteDriftFade(BattleAnimSystem *system) {
    BattleAnimSpriteDriftFadeWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSpriteDriftFadeWork));
    int battler;
    int signX;
    int signY;
    int layer2;
    int layer1;

    work->state = 0;
    work->unk_01 = 0;
    work->system = system;
    work->spriteManager = BattleAnimSystem_GetBattlerSpriteManager(system);
    work->bgConfig = BattleAnimSystem_GetBgConfig(work->system);
    work->sprite = BattleAnimSystem_GetBattlerSprite(work->system, 0);
    if ((s32)BattleAnimSystem_GetArg(system, 0) == 0) {
        battler = BattleAnimSystem_GetDefender(work->system);
    } else {
        battler = BattleAnimSystem_GetAttacker(work->system);
    }
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, battler);
    BattleAnimUtil_GetPicPosition(work->pokepic, &work->x);
    work->y -= Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H);
    signX = BattleAnimUtil_GetBattlerSideSignByMode(work->system, battler);
    signY = BattleAnimUtil_GetBattlerSideSign(work->system, battler);
    BattleAnimUtil_InitLinearMotion(&work->motion, work->x, (s16)(work->x + -20 * signX), work->y, (s16)(work->y + 20 * signY), 20);
    layer2 = BattleAnimSystem_GetBgMaskLayer(system, 2);
    layer1 = BattleAnimSystem_GetBgMaskLayer(system, 1);
    BattleAnimUtil_SetBlendWithMask(work->system, (1 << layer2) | GX_BLEND_PLANEMASK_BD | (1 << layer1) | GX_BLEND_PLANEMASK_BG0, -1, -1);
    BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 31, 0, 0, 31, 15);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_SpriteDriftFade, work);
}

void BattleAnimTask_RockAttacker(SysTask *task, void *data) {
    BattleAnimRockOrShakeAttackerWork *work = data;

    switch (work->common.state[0]) {
    case 0: {
        int from;
        int to;
        if (work->common.state[1] == 0) {
            to = gBattleAnimRockAttackerSteps[work->common.state[2]][0];
            from = -to;
        } else {
            from = gBattleAnimRockAttackerSteps[work->common.state[2]][0];
            to = -from;
        }
        BattleAnimUtil_InitValueMotion(&work->rotation, from, to, ov07_02236750[work->common.state[2]][0]);
        if (work->common.state[1] == 1) {
            work->common.state[2]++;
        }
        work->common.state[1] ^= 1;
        work->common.state[0]++;
    }
    case 1:
        if (BattleAnimUtil_UpdateValueMotion(&work->rotation) == TRUE) {
            Pokepic_SetAttr(work->attackerPic, POKEPIC_ZROT, (u16)work->rotation.value);
            return;
        }
        if (work->common.state[2] >= 5) {
            work->common.state[0]++;
        } else {
            work->common.state[0]--;
        }
        return;
    default:
        Pokepic_SetAttr(work->attackerPic, POKEPIC_ZROT, 0);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        return;
    }
}

#pragma force_active reset

void BattleAnimTask_ShakePic(SysTask *task, void *data) {
    s16 pos[2];
    BattleAnimRockOrShakeAttackerWork *work = data;

    switch (work->common.state[0]) {
    case 0:
        BattleAnimUtil_InitShake(&work->shake, work->shakeAmpX, work->shakeAmpY, work->shakeInterval, work->shakeCount);
        work->common.state[0]++;
        return;
    case 1:
        BattleAnimUtil_GetPicPosition(work->defenderPic, pos);
        if (BattleAnimUtil_UpdateShakePokepic(&work->shake, pos[0], pos[1], work->defenderPic) == FALSE) {
            work->common.state[0]++;
            return;
        }
        return;
    default:
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        break;
    }
}

#pragma force_active on

void BattleAnimFunc_RockOrShakeAttacker(BattleAnimSystem *system) {
    BattleAnimRockOrShakeAttackerWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimRockOrShakeAttackerWork));
    int shake;

    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->attackerPic = BattleAnimSystem_GetBattlerPokepic(work->common.system, BattleAnimSystem_GetAttacker(work->common.system));
    work->defenderPic = BattleAnimSystem_GetBattlerPokepic(work->common.system, BattleAnimSystem_GetDefender(work->common.system));
    Pokepic_SetAttr(work->attackerPic, POKEPIC_YPIVOT, 100);
    shake = (s32)BattleAnimSystem_GetArg(system, 0);
    work->shakeAmpX = (s32)BattleAnimSystem_GetArg(system, 1);
    work->shakeAmpY = (s32)BattleAnimSystem_GetArg(system, 2);
    work->shakeInterval = (s32)BattleAnimSystem_GetArg(system, 3);
    work->shakeCount = (s32)BattleAnimSystem_GetArg(system, 4);
    work->unk_24 = (s32)BattleAnimSystem_GetArg(system, 5);
    if (!shake) {
        BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_RockAttacker, work);
    } else {
        BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_ShakePic, work);
    }
}

void BattleAnimTask_ShakeBattlefieldByPower(SysTask *task, void *data) {
    int i;
    BattleAnimShakeBattlefieldByPowerWork *work = data;

    switch (work->common.state[0]) {
    case 0:
        BattleAnimUtil_InitShake(&work->shake, (s16)(work->amplitude + 2), (s16)work->amplitude, 0, 10);
        work->common.state[0]++;
        break;
    case 1:
        if (!BattleAnimUtil_UpdateShake(&work->shake)) {
            work->common.state[0]++;
            break;
        }
        for (i = 0; i < 4; i++) {
            if (work->pokepics[i]) {
                Pokepic_SetAttr(work->pokepics[i], POKEPIC_X, work->shake.x + work->positions[i][0]);
            }
        }
        BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, work->shake.x);
        BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, work->shake.y);
        break;
    default:
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        break;
    }
}

void BattleAnimFunc_ShakeBattlefieldByPower(BattleAnimSystem *system) {
    BattleAnimShakeBattlefieldByPowerWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimShakeBattlefieldByPowerWork));
    int i;

    BattleAnimUtil_InitCommonWork(system, &work->common);
    switch ((int)BattleAnimSystem_GetMoveParam(work->common.system, 1)) {
    case 150:
        work->amplitude = 6;
        break;
    case 110:
        work->amplitude = 5;
        break;
    case 90:
        work->amplitude = 4;
        break;
    case 70:
        work->amplitude = 3;
        break;
    case 50:
        work->amplitude = 2;
        break;
    case 30:
        work->amplitude = 1;
        break;
    default:
        work->amplitude = 0;
        break;
    }
    for (i = 0; i < 4; i++) {
        work->pokepics[i] = BattleAnimSystem_GetBattlerPokepic(work->common.system, i);
        if (work->pokepics[i]) {
            BattleAnimUtil_GetPicPosition(work->pokepics[i], work->positions[i]);
        }
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_ShakeBattlefieldByPower, work);
}

#pragma force_active reset

void BattleAnimTask_ShakeSpritePair(SysTask *task, void *data) {
    BattleAnimShakeSpritePairWork *work = data;

    switch (work->common.state[0]) {
    case 0:
        BattleAnimUtil_InitShake(&work->shake, 20, 0, 0, 10);
        BattleAnimUtil_InitShake(&work->jitter, 2, 0, 0, 10);
        work->common.state[0]++;
        return;
    case 1:
        if (BattleAnimUtil_UpdateShake(&work->shake) == FALSE) {
            work->common.state[0]++;
        } else {
            ManagedSprite_SetPositionXY(work->sprites[0], work->defenderPos[0] + work->shake.x, work->defenderPos[1] + work->shake.y);
            ManagedSprite_SetPositionXY(work->sprites[1], work->shake.x - work->defenderPos[0], work->defenderPos[1] + work->shake.y);
            ManagedSprite_TickFrame(work->sprites[0]);
            ManagedSprite_TickFrame(work->sprites[1]);
        }
        SpriteSystem_DrawSprites(work->common.battlerSpriteManager);
        return;
    default:
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        return;
    }
}

void BattleAnimFunc_ShakeSpritePair(BattleAnimSystem *system) {
    BattleAnimShakeSpritePairWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimShakeSpritePairWork));

    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->sprites[0] = BattleAnimSystem_GetBattlerSprite(work->common.system, 0);
    work->sprites[1] = BattleAnimSystem_GetBattlerSprite(work->common.system, 1);
    work->attackerPic = BattleAnimSystem_GetBattlerPokepic(work->common.system, BattleAnimSystem_GetAttacker(work->common.system));
    work->defenderPic = BattleAnimSystem_GetBattlerPokepic(work->common.system, BattleAnimSystem_GetDefender(work->common.system));
    BattleAnimUtil_GetPicPosition(work->attackerPic, work->attackerPos);
    BattleAnimUtil_GetPicPosition(work->defenderPic, work->defenderPos);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_ShakeSpritePair, work);
}

void BattleAnimTask_ThrowSpriteArc(SysTask *task, void *data) {
    BattleAnimThrowSpriteArcWork *work = data;

    if (BattleAnimUtil_UpdateLinearSinMotionSprite(&work->path, &work->arc, work->sprite) == FALSE) {
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        BattleAnimUtil_FreeWork(work);
        return;
    }
    if (BattleAnimUtil_UpdateValueMotion(&work->rotation) == TRUE) {
        ManagedSprite_SetAffineZRotation(work->sprite, (u16)work->rotation.value);
    }
    ManagedSprite_TickFrame(work->sprite);
    SpriteSystem_DrawSprites(work->common.spriteManager);
}

void BattleAnimSpriteFunc_ThrowSpriteArc(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    int sign;
    BattleAnimThrowSpriteArcWork *work;

    work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimThrowSpriteArcWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->offsetX = (s32)BattleAnimSystem_GetArg(system, 0);
    work->offsetY = (s32)BattleAnimSystem_GetArg(system, 1);
    work->duration = (s32)BattleAnimSystem_GetArg(system, 2);
    work->height = (s32)BattleAnimSystem_GetArg(system, 3);
    sign = BattleAnimUtil_GetBattlerSideSignByMode(system, BattleAnimSystem_GetAttacker(system));
    work->sprite = sprite;
    ManagedSprite_SetAffineOverwriteMode(sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    work->attacker.pokepic = BattleAnimSystem_GetBattlerPokepic(system, BattleAnimSystem_GetAttacker(system));
    work->defender.pokepic = BattleAnimSystem_GetBattlerPokepic(system, BattleAnimSystem_GetDefender(system));
    BattleAnimUtil_GetPicPosition(work->attacker.pokepic, work->attacker.pos);
    BattleAnimUtil_GetPicPosition(work->defender.pokepic, work->defender.pos);
    BattleAnimUtil_InitLinearSinMotion(&work->path, &work->arc, work->attacker.pos[0], (s16)(work->defender.pos[0] + work->offsetX * sign), work->attacker.pos[1], (s16)(work->defender.pos[1] + work->offsetY * sign), (u16)work->duration, work->height << FX32_SHIFT);
    if (BattleAnimSystem_GetBattlerType(system, BattleAnimSystem_GetAttacker(system)) == BATTLER_TYPE_PLAYER_SIDE_SLOT_2 && BattleAnimSystem_GetBattlerType(system, BattleAnimSystem_GetDefender(system)) == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
        sign *= -1;
    }
    if (BattleAnimSystem_GetBattlerType(system, BattleAnimSystem_GetAttacker(system)) == BATTLER_TYPE_ENEMY_SIDE_SLOT_2 && BattleAnimSystem_GetBattlerType(system, BattleAnimSystem_GetDefender(system)) == BATTLER_TYPE_ENEMY_SIDE_SLOT_1) {
        sign *= -1;
    }
    if (sign > 0) {
        BattleAnimUtil_InitValueMotion(&work->rotation, 0xE38 * sign, 0x5C71 * sign, 10);
    } else {
        BattleAnimUtil_InitValueMotion(&work->rotation, 0x3FFF * sign, 0x5C71 * sign, 10);
    }
    ManagedSprite_SetAffineZRotation(work->sprite, (u16)work->rotation.value);
    BattleAnimUtil_UpdateLinearSinMotionSprite(&work->path, &work->arc, work->sprite);
    ManagedSprite_TickFrame(work->sprite);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_ThrowSpriteArc, work);
}

typedef struct BattleAnimStretchFadeSpriteWork {
    int unk_00;
    int holdTimer;
    u8 unk_08[4];
    u8 eva; // 0x0C
    u8 evb;
    u8 endEva;
    u8 endEvb;
    u8 peakEva;
    u8 peakEvb;
    s16 direction; // 0x12, sign of the x scale
    s16 x;
    s16 y;
    BattleAnimCommonWork common; // 0x18
    u8 unk_34[0xC];
    ManagedSprite *sprite;     // 0x40, one of sprites[]
    ManagedSprite *sprites[2]; // 0x44
    u8 unk_4C[0x50];
    BattleAnimLinearMotion scale; // 0x9C
    u8 unk_C0[0x14];
} BattleAnimStretchFadeSpriteWork; // size: 0xD4

typedef struct BattleAnimCrossingSprite {
    ManagedSprite *sprite;
    BattleAnimLinearMotion path; // 0x04
    BattleAnimSinMotion arc;     // 0x28
} BattleAnimCrossingSprite;      // size: 0x4C

// The attacker squashes and hops, then two sprites drop in and swap places along arcs
typedef struct BattleAnimCrossingSpritesWork {
    u8 state;
    u8 timer;
    u8 unk_02[2];
    int eva; // 0x04
    int evb;
    u8 round; // 0x0C
    u8 unk_0D[3];
    BattleAnimSystem *system; // 0x10
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    Pokepic *pokepic; // 0x1C
    s16 y;
    u8 unk_22[2];
    int height; // 0x24
    u16 angle;  // 0x28
    u8 unk_2A[2];
    fx32 distance;                       // 0x2C
    BattleAnimCrossingSprite sprites[2]; // 0x30
    s16 midX;                            // 0xC8
    s16 midY;
    BattleAnimLinearMotion scale; // 0xCC
} BattleAnimCrossingSpritesWork;  // size: 0xF0

// A sprite position read into a y-first pair (the layout is needed to match)
typedef struct BattleAnimSpritePos {
    s16 y;
    s16 x;
} BattleAnimSpritePos;

typedef struct BattleAnimBlinkSpritesCenterWork {
    u8 state;
    u8 blinkTimers[3];
    u8 timer; // 0x04
    u8 count;
    u8 eva;
    u8 evb;
    BattleAnimSystem *system; // 0x08
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[12];     // 0x14
} BattleAnimBlinkSpritesCenterWork; // size: 0x44

typedef struct BattleAnimSlidingSpritePairsWork {
    u8 state;
    u8 unk_01[6];
    u8 unk_07;
    u8 count; // 0x08
    u8 eva;
    u8 evb;
    u8 unk_0B;
    BattleAnimSystem *system; // 0x0C
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[6]; // 0x18
    int pair;                  // 0x30
    ManagedSprite *first;
    ManagedSprite *second;
    BattleAnimLinearMotion firstMotion;  // 0x3C
    BattleAnimLinearMotion secondMotion; // 0x60
} BattleAnimSlidingSpritePairsWork;      // size: 0x84

typedef struct BattleAnimScatterSprite {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u8 state; // 0x0C
    u8 timer;
    u8 delay;
    u8 target;                     // row of the target position tables
    ManagedSprite *sprite;         // 0x10
    BattleAnimLinearMotion motion; // 0x14
    f32 scale;                     // 0x38
    int *status;                   // 0x3C, in the group work: 1 waiting, 2 released, 3 done
} BattleAnimScatterSprite;         // size: 0x40

typedef struct BattleAnimPopAndScatterSpritesWork {
    u8 state;
    u8 unk_01;
    u8 count;
    u8 unk_03[5];
    BattleAnimSystem *system; // 0x08
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    BattleAnimScatterSprite sprites[20]; // 0x14
    int status[20];                      // 0x514
} BattleAnimPopAndScatterSpritesWork;    // size: 0x564

typedef struct BattleAnimScatterArea {
    s16 y;
    s16 x;
    s16 yRange;
    s16 xRange;
} BattleAnimScatterArea;

typedef struct BattleAnimWaveAttackerOnBgWork {
    int timer;
    BattleAnimCommonWork common;      // 0x04
    ManagedSprite *windowSprite;      // 0x20, drawn as the OBJ window
    ManagedSprite *sprite;            // 0x24
    BattleAnimScanlineWave *wave;     // 0x28
    BattleAnimBlendAlphaMotion blend; // 0x2C
} BattleAnimWaveAttackerOnBgWork;     // size: 0x54

typedef struct BattleAnimSpriteQuadWork {
    BattleAnimCommonWork common;
    s16 delay; // 0x1C, frames before the second pair animates
    s16 timers[4];
    u8 unk_26[2];
    ManagedSprite *sprites[4]; // 0x28
} BattleAnimSpriteQuadWork;    // size: 0x38

typedef struct BattleAnimFlashingSpritesAroundAttackerWork {
    BattleAnimCommonWork common;
    int paletteSlot; // 0x1C
    s16 unk_20;
    s16 timers[4];
    u8 unk_2A[2];
    ManagedSprite *sprites[4];                // 0x2C
    BattleAnimPaletteBlend *paletteBlends[6]; // 0x3C
    int eva;                                  // 0x54
    int evb;
} BattleAnimFlashingSpritesAroundAttackerWork; // size: 0x5C

typedef struct BattleAnimShrinkSpriteOnDefenderWork {
    BattleAnimCommonWork common;
    u8 unk_1C[8];
    Pokepic *pokepic;      // 0x24
    ManagedSprite *sprite; // 0x28
    u8 unk_2C[0x10];
    ManagedSprite *sprite2; // 0x3C
    u8 unk_40[0x10];
    ManagedSprite *sprite3; // 0x50
    u8 unk_54[4];
    s16 y; // 0x58
    s16 height;
    s16 baseY;
    u8 unk_5E[2];
    BattleAnimLinearMotion scale;       // 0x60
} BattleAnimShrinkSpriteOnDefenderWork; // size: 0x84

typedef struct BattleAnimAttackerDashAfterimagesWork {
    BattleAnimCommonWork common;
    u8 unk_1C[4];
    s16 x; // 0x20
    s16 y;
    Pokepic *pokepic;       // 0x24
    ManagedSprite *sprite1; // 0x28
    u32 unk_2C;
    ManagedSprite *sprite2;               // 0x30
    BattleAnimLinearMotion pokepicMotion; // 0x34
    BattleAnimLinearMotion sprite1Motion; // 0x58
    BattleAnimLinearMotion sprite2Motion; // 0x7C
    u8 leg;                               // 0xA0
    u8 unk_A1;
    u8 eva;
    u8 evb;
    s16 dy;                              // 0xA4, sprite offset from the pic
} BattleAnimAttackerDashAfterimagesWork; // size: 0xA8

typedef struct BattleAnimSpriteTrailToDefenderWork {
    BattleAnimCommonWork common;
    s16 delay;     // 0x1C
    s16 timers[8]; // 0x1E
    u8 unk_2E[2];
    ManagedSprite *sprites[8];         // 0x30
    BattleAnimLinearMotion motion;     // 0x50
} BattleAnimSpriteTrailToDefenderWork; // size: 0x74

extern const u8 gBattleAnimCrossingSpritesScales[];
extern const u8 ov07_02236705[];
extern const u8 ov07_02236706[];
extern const u8 ov07_02236707[];
extern const u8 ov07_02236708[];
extern const s8 gBattleAnimCrossingSpritesArcSigns[5][2];
extern const u8 gBattleAnimSlidingSpritePairDurations[];
// {start frame, blink interval} per sprite
extern const u8 gBattleAnimBlinkCenterTimings[][2];
extern const s16 gBattleAnimScatterTargets[][2];
extern const s16 ov07_02236780[][2];
extern const s16 gBattleAnimSlidingSpritePairPositions[][2];
// {x, y, x2, y2} per pair; each label points at one column
extern const s16 gBattleAnimSlidingSpritePairTargets[][4];
extern const s16 ov07_022367B0[][4];
extern const s16 ov07_022367B2[][4];
extern const s16 ov07_022367B4[][4];
extern const BattleAnimScatterArea gBattleAnimScatterAreas[];

static inline void BattleAnimTask_SlidingSpritePairsSetBlend(int eva, int evb) {
    vu16 *bldalpha = &reg_G2_BLDALPHA;
    int value = eva | (evb << 8);
    *bldalpha = value;
}

static inline void BattleAnimTask_FlashingSpritesAroundAttackerStartFade(BattleAnimFlashingSpritesAroundAttackerWork *work) {
    BattleAnimUtil_SetBlendWithBattleBg(work->common.system, -1, -1);
    ManagedSprite_SetOamMode(work->sprites[0], GX_OAM_MODE_XLU);
    ManagedSprite_SetOamMode(work->sprites[1], GX_OAM_MODE_XLU);
    ManagedSprite_SetOamMode(work->sprites[2], GX_OAM_MODE_XLU);
    ManagedSprite_SetOamMode(work->sprites[3], GX_OAM_MODE_XLU);
    work->eva = 15;
    work->evb = 0;
}

#pragma force_active on

void BattleAnimTask_StretchFadeSprite(SysTask *task, void *data) {
    f32 scaleX;
    f32 scaleY;
    f32 scaleX2;
    f32 scaleY2;
    BattleAnimStretchFadeSpriteWork *work = data;

    switch (work->common.state[0]) {
    case 0:
        BattleAnimUtil_InitScaleMotionXY(&work->scale, (s16)(work->direction * 100), (s16)(work->direction * 60), 5, 150, 100, 12);
        work->common.state[0]++;
        break;
    case 1:
        if (BattleAnimUtil_UpdateScaleMotionXY(&work->scale) == FALSE) {
            BattleAnimUtil_InitScaleMotionXY(&work->scale, (s16)(work->direction * 60), (s16)(work->direction * 150), 150, 10, 100, 12);
            work->common.state[0]++;
        } else {
            BattleAnimUtil_GetScaleFloat(&work->scale, &scaleX, &scaleY);
            ManagedSprite_SetAffineScale(work->sprite, scaleX, scaleY);
            ManagedSprite_SetPositionXY(work->sprite, work->x, (s16)(work->y + BattleAnimUtil_CalcScaleYOffset(work->y, 16, work->scale.curY)));
            if (work->eva < work->peakEva) {
                work->eva++;
            }
            if (work->evb > work->peakEvb) {
                work->evb--;
            }
            reg_G2_BLDALPHA = work->eva | (work->evb << 8);
        }
        break;
    case 2:
        if (++work->holdTimer >= 4) {
            work->common.state[0]++;
        }
        break;
    case 3:
        if (BattleAnimUtil_UpdateScaleMotionXY(&work->scale) == FALSE) {
            work->common.state[0]++;
        } else {
            BattleAnimUtil_GetScaleFloat(&work->scale, &scaleX2, &scaleY2);
            ManagedSprite_SetAffineScale(work->sprite, scaleX2, scaleY2);
            ManagedSprite_SetPositionXY(work->sprite, work->x, (s16)(work->y + BattleAnimUtil_CalcScaleYOffset(work->y, 16, work->scale.curY)));
            if (work->eva > work->endEva) {
                work->eva--;
            }
            if (work->evb < work->endEvb) {
                work->evb++;
            }
            reg_G2_BLDALPHA = work->eva | (work->evb << 8);
        }
        break;
    default:
        Sprite_DeleteAndFreeResources(work->sprites[0]);
        Sprite_DeleteAndFreeResources(work->sprites[1]);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        BattleAnimUtil_FreeWork(work);
        return;
    }
    ManagedSprite_TickFrame(work->sprite);
    SpriteSystem_DrawSprites(work->common.spriteManager);
}

void BattleAnimFunc_StretchFadeSprite(BattleAnimSystem *system) {
    BattleAnimStretchFadeSpriteWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimStretchFadeSpriteWork));
    f32 scaleX;
    f32 scaleY;

    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->sprites[0] = BattleAnimSystem_GetSprite(system, 0);
    work->sprites[1] = BattleAnimSystem_GetSprite(system, 1);
    BattleAnimUtil_SetBlendWithBattleBg(work->common.system, -1, -1);
    BattleAnimSystem_GetArg(system, 0);
    work->eva = 0;
    work->evb = 31;
    work->endEva = 0;
    work->endEvb = 31;
    work->peakEva = 31;
    work->peakEvb = 4;
    if (BattleAnimSystem_GetField08(system) == 1) {
        work->sprite = work->sprites[0];
        ManagedSprite_SetDrawFlag(work->sprites[1], FALSE);
        ManagedSprite_SetAnim(work->sprite, 0);
        work->x = 179;
        work->y = 120;
        ManagedSprite_SetPriority(work->sprite, BattleAnimSystem_GetBasePriority(system) + 1);
        work->direction = -1;
    } else {
        if (BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetAttacker(system)) == BATTLE_ANIM_SIDE_ENEMY) {
            work->sprite = work->sprites[1];
            ManagedSprite_SetDrawFlag(work->sprites[0], FALSE);
            ManagedSprite_SetAnim(work->sprite, 1);
            work->x = 144;
            work->y = 64;
        } else {
            work->sprite = work->sprites[0];
            ManagedSprite_SetDrawFlag(work->sprites[1], FALSE);
            ManagedSprite_SetAnim(work->sprite, 0);
            work->x = 76;
            work->y = 120;
            ManagedSprite_SetPriority(work->sprite, BattleAnimSystem_GetBasePriority(system) + 1);
        }
        work->direction = 1;
    }
    ManagedSprite_SetPositionXY(work->sprite, work->x, work->y);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    BattleAnimUtil_InitScaleMotionXYS16(&work->scale, work->direction * 100, work->direction * 100, 100, 5, 100, 1);
    BattleAnimUtil_UpdateScaleMotionXY(&work->scale);
    BattleAnimUtil_GetScaleFloat(&work->scale, &scaleX, &scaleY);
    ManagedSprite_SetAffineScale(work->sprite, scaleX, scaleY);
    ManagedSprite_SetPositionXY(work->sprite, work->x, (s16)(work->y + BattleAnimUtil_CalcScaleYOffset(work->y, 16, work->scale.curY)));
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_StretchFadeSprite, work);
}

void BattleAnimTask_CrossingSprites(SysTask *task, void *data) {
    BattleAnimCrossingSpritesWork *work = data;
    BattleAnimSpritePos pos3;
    BattleAnimSpritePos pos4;
    BattleAnimSpritePos pos5a;
    BattleAnimSpritePos pos5b;
    BattleAnimSpritePos pos6a;
    BattleAnimSpritePos pos6b;
    BattleAnimSpritePos pos;
    int i;
    int count;

    switch (work->state) {
    case 0: {
        int row = work->round * 5;
        BattleAnimUtil_InitScaleMotionXY(&work->scale, gBattleAnimCrossingSpritesScales[row], ov07_02236705[row], ov07_02236706[row], ov07_02236707[row], 100, ov07_02236708[row]);
        work->round++;
        work->state++;
        break;
    }
    case 1:
        if (BattleAnimUtil_UpdateScaleMotionXY(&work->scale) == TRUE) {
            int dy;
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
            dy = BattleAnimUtil_CalcScaleYOffset(work->y, work->height, work->scale.curY);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y + dy);
        } else if (work->round >= 3) {
            work->state++;
        } else {
            work->state--;
        }
        break;
    case 2:
        work->timer++;
        if (work->timer >= 1) {
            work->timer = 0;
            work->round = 0;
            work->state++;
        }
        break;
    case 3:
        ManagedSprite_GetPositionXY(work->sprites[0].sprite, &pos3.x, &pos3.y);
        if (pos3.y <= 104) {
            ManagedSprite_OffsetPositionXY(work->sprites[0].sprite, 0, 2);
            ManagedSprite_OffsetPositionXY(work->sprites[1].sprite, 0, 2);
        } else {
            work->state++;
        }
        break;
    case 4:
        ManagedSprite_GetPositionXY(work->sprites[0].sprite, &pos4.x, &pos4.y);
        if (pos4.y <= 104) {
            ManagedSprite_OffsetPositionXY(work->sprites[0].sprite, 0, 2);
            ManagedSprite_OffsetPositionXY(work->sprites[1].sprite, 0, 2);
        } else {
            reg_G2_BLDALPHA = 16;
            work->state++;
        }
        work->timer++;
        BattleAnimUtil_StepTowards(&work->eva, 16, 16);
        BattleAnimUtil_StepTowards(&work->evb, 0, -16);
        reg_G2_BLDALPHA = work->eva | (work->evb << 8);
        if (work->timer == 10) {
            ManagedSprite_OffsetPositionXY(work->sprites[0].sprite, 0, 16);
        }
        if (work->timer == 12) {
            ManagedSprite_OffsetPositionXY(work->sprites[1].sprite, 0, 16);
        }
        break;
    case 5:
        work->timer++;
        if (work->timer >= 1) {
            work->timer = 0;
            ManagedSprite_GetPositionXY(work->sprites[0].sprite, &pos5a.x, &pos5a.y);
            ManagedSprite_GetPositionXY(work->sprites[1].sprite, &pos5b.x, &pos5b.y);
            BattleAnimUtil_CalcMidpoint(pos5a.x, pos5a.y, pos5b.x, pos5b.y, &work->midX, &work->midY);
            BattleAnimUtil_CalcDistance(pos5a.x, pos5a.y, work->midX, work->midY, &work->distance);
            BattleAnimUtil_CalcAngle(pos5a.x, pos5a.y, work->midX, work->midY, &work->angle);
            work->distance = FX32_CONST(40);
            work->state++;
        }
        break;
    case 6: {
        s8 arcSigns[5][2];
        ARRAY_ASSIGN(arcSigns, gBattleAnimCrossingSpritesArcSigns);
        ManagedSprite_GetPositionXY(work->sprites[0].sprite, &pos6a.x, &pos6a.y);
        ManagedSprite_GetPositionXY(work->sprites[1].sprite, &pos6b.x, &pos6b.y);
        BattleAnimUtil_InitLinearSinMotion(&work->sprites[0].path, &work->sprites[0].arc, pos6a.x, pos6b.x, pos6a.y, pos6b.y, 10, work->distance * arcSigns[work->round][0]);
        BattleAnimUtil_InitLinearSinMotion(&work->sprites[1].path, &work->sprites[1].arc, pos6b.x, pos6a.x, pos6b.y, pos6a.y, 10, work->distance * arcSigns[work->round][1]);
        work->round++;
        work->state++;
        break;
    }
    case 7: {
        u8 done = 0;
        if (!BattleAnimUtil_UpdateLinearSinMotionSprite(&work->sprites[0].path, &work->sprites[0].arc, work->sprites[0].sprite)) {
            done++;
        }
        if (!BattleAnimUtil_UpdateLinearSinMotionSprite(&work->sprites[1].path, &work->sprites[1].arc, work->sprites[1].sprite)) {
            done++;
        }
        if (done == 2) {
            if (work->round == 5) {
                work->state++;
            } else {
                work->state--;
            }
        }
        break;
    }
    case 8:
        count = 0;
        if (BattleAnimUtil_StepTowards(&work->eva, 0, -2) == TRUE) {
            count++;
        }
        if (BattleAnimUtil_StepTowards(&work->evb, 16, 2) == TRUE) {
            count++;
        }
        reg_G2_BLDALPHA = work->eva | (work->evb << 8);
        if (count == 2) {
            work->state++;
        }
        break;
    default:
        for (i = 0; i < (int)BattleAnimSystem_GetArg(work->system, 0); i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i].sprite);
        }
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    for (i = 0; i < (int)BattleAnimSystem_GetArg(work->system, 0); i++) {
        ManagedSprite_GetPositionXY(work->sprites[i].sprite, &pos.x, &pos.y);
        if (pos.y < 80) {
            ManagedSprite_SetPriority(work->sprites[i].sprite, BattleAnimSystem_GetBasePriority(work->system));
        } else if (pos.x > 128) {
            ManagedSprite_SetPriority(work->sprites[i].sprite, BattleAnimSystem_GetBasePriority(work->system));
        } else {
            ManagedSprite_SetPriority(work->sprites[i].sprite, BattleAnimSystem_GetBasePriority(work->system) + 1);
        }
    }
    if (work->state >= 4) {
        int argIndex;
        i = 0;
        argIndex = i;
        for (; i < (int)BattleAnimSystem_GetArg(work->system, argIndex); i++) {
            ManagedSprite_TickFrame(work->sprites[i].sprite);
        }
        SpriteSystem_DrawSprites(work->spriteManager);
    }
}

void BattleAnimSpriteFunc_CrossingSprites(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimCrossingSpritesWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimCrossingSpritesWork));
    int layer2;
    int layer1;
    int i;
    ManagedSpriteTemplate template;

    work->state = 0;
    work->timer = 0;
    work->system = system;
    work->spriteManager = spriteManager;
    work->spriteSystem = spriteSystem;
    work->eva = 0;
    work->evb = 16;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->height = BattleAnimSystem_GetBattlerHeight(work->system, BattleAnimSystem_GetAttacker(work->system));
    layer2 = BattleAnimSystem_GetBgMaskLayer(system, 2);
    layer1 = BattleAnimSystem_GetBgMaskLayer(system, 1);
    BattleAnimUtil_SetBlendWithMask(work->system, (1 << layer2) | (1 << layer1) | GX_BLEND_PLANEMASK_BG0, -1, -1);
    reg_G2_BLDALPHA = work->eva | (work->evb << 8);
    BattleAnimSystem_GetSpriteTemplate(&template, system);
    work->sprites[0].sprite = sprite;
    for (i = 1; i < (int)BattleAnimSystem_GetArg(work->system, 0); i++) {
        work->sprites[i].sprite = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
    }
    for (i = 0; i < (int)BattleAnimSystem_GetArg(work->system, 0); i++) {
        ManagedSprite_SetOamMode(work->sprites[i].sprite, GX_OAM_MODE_XLU);
        ManagedSprite_SetPriority(work->sprites[i].sprite, BattleAnimSystem_GetBasePriority(work->system) + 1);
    }
    if (BattleAnimSystem_GetField08(system) == 1) {
        ManagedSprite_SetPositionXY(work->sprites[0].sprite, 160, 54);
        ManagedSprite_SetPositionXY(work->sprites[1].sprite, 80, 39);
    } else {
        ManagedSprite_SetPositionXY(work->sprites[0].sprite, 100, 54);
        ManagedSprite_SetPositionXY(work->sprites[1].sprite, 180, 39);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_CrossingSprites, work);
}

#pragma force_active reset

void BattleAnimTask_BlinkSpritesCenter(SysTask *task, void *data) {
    int i;
    s16 x;
    s16 y;
    BattleAnimBlinkSpritesCenterWork *work = data;

    switch (work->state) {
    case 0:
        if (work->eva < 15) {
            work->eva++;
        }
        if (work->evb != 0) {
            work->evb--;
        }
        reg_G2_BLDALPHA = work->eva | (work->evb << 8);
        work->timer++;
        for (i = 0; i < work->count; i++) {
            ManagedSprite_GetPositionXY(work->sprites[i], &x, &y);
            if (work->timer >= gBattleAnimBlinkCenterTimings[i][0]) {
                work->blinkTimers[i]++;
                if (work->blinkTimers[i] >= gBattleAnimBlinkCenterTimings[i][1]) {
                    work->blinkTimers[i] = 0;
                }
            }
        }
        if (work->timer >= 45) {
            work->state++;
        }
        break;
    case 1:
        if (work->eva != 0) {
            work->eva--;
        }
        if (work->evb < 15) {
            work->evb++;
        }
        if (work->eva == 0 && work->evb == 15) {
            work->state++;
        }
        reg_G2_BLDALPHA = work->eva | (work->evb << 8);
        break;
    default:
        for (i = 0; i < work->count; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    for (i = 0; i < work->count; i++) {
        Sprite_TickFrame(work->sprites[i]->sprite);
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

#pragma force_active on

void BattleAnimSpriteFunc_BlinkSpritesCenter(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimBlinkSpritesCenterWork *work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimBlinkSpritesCenterWork));
    ManagedSpriteTemplate template;
    int i;
    int evb;
    int eva;

    GF_ASSERT(work != NULL);
    work->timer = 0;
    work->state = 0;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->system = system;
    BattleAnimSystem_GetSpriteTemplate(&template, system);
    BattleAnimUtil_SetBlendWithBattleBg(work->system, -1, -1);
    work->count = BattleAnimSystem_GetArg(work->system, 0);
    work->eva = 0;
    work->evb = 15;
    // volatile reads: needed to match
    evb = *(vu8 *)&work->evb;
    eva = *(vu8 *)&work->eva;
    reg_G2_BLDALPHA = (evb << 8) | eva;
    work->sprites[0] = sprite;
    for (i = 1; i < work->count; i++) {
        work->sprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
    }
    if (BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetDefender(system)) == BATTLE_ANIM_SIDE_PLAYER) {
        if (BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetDefender(system)) == BATTLE_ANIM_SIDE_PLAYER && BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetAttacker(system)) == BATTLE_ANIM_SIDE_PLAYER) {
            ManagedSprite_SetAnim(work->sprites[0], 0);
        } else {
            ManagedSprite_SetAnim(work->sprites[0], 1);
        }
        ManagedSprite_SetPositionXY(work->sprites[0], 128, 80);
    } else {
        if (BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetDefender(system)) == BATTLE_ANIM_SIDE_ENEMY && BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetAttacker(system)) == BATTLE_ANIM_SIDE_ENEMY) {
            ManagedSprite_SetAnim(work->sprites[0], 1);
        } else {
            ManagedSprite_SetAnim(work->sprites[0], 0);
        }
        ManagedSprite_SetPositionXY(work->sprites[0], 128, 80);
    }
    for (i = 0; i < work->count; i++) {
        work->blinkTimers[i] = 0;
        ManagedSprite_SetAffineOverwriteMode(work->sprites[i], NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
        ManagedSprite_SetOamMode(work->sprites[i], GX_OAM_MODE_XLU);
        if (BattleAnimSystem_GetField08(work->system) == 1) {
            ManagedSprite_SetAffineScale(work->sprites[i], -1.0f, 1.0f);
        }
    }
    BattleAnimSystem_CreateTask(system, BattleAnimTask_BlinkSpritesCenter, work, 1100);
}

void BattleAnimTask_SlidingSpritePairs(SysTask *task, void *data) {
    BattleAnimSlidingSpritePairsWork *work = data;
    BOOL stopAnimation = FALSE;
    int evb;
    int eva;

    switch (work->state) {
    case 0:
        if (work->eva < 15) {
            work->eva++;
        }
        if (work->evb > 0) {
            work->evb--;
        }
        if (work->eva == 15 && work->evb == 0) {
            work->state++;
        }
        // volatile reads: needed to match
        evb = *(vu8 *)&work->evb;
        eva = *(vu8 *)&work->eva;
        reg_G2_BLDALPHA = (evb << 8) | eva;
        break;
    case 1: {
        s16 x;
        s16 y;
        switch (work->pair) {
        case 0:
            work->first = work->sprites[0];
            work->second = work->sprites[3];
            ManagedSprite_SetDrawFlag(work->first, TRUE);
            ManagedSprite_SetDrawFlag(work->second, TRUE);
            break;
        case 1:
            work->first = work->sprites[1];
            work->second = work->sprites[2];
            ManagedSprite_SetDrawFlag(work->first, TRUE);
            ManagedSprite_SetDrawFlag(work->second, TRUE);
            break;
        case 2:
        case 3:
            work->first = work->sprites[4];
            work->second = work->sprites[5];
            ManagedSprite_SetDrawFlag(work->first, TRUE);
            ManagedSprite_SetDrawFlag(work->second, TRUE);
            break;
        }
        ManagedSprite_GetPositionXY(work->first, &x, &y);
        BattleAnimUtil_InitLinearMotion(&work->firstMotion, x, gBattleAnimSlidingSpritePairTargets[work->pair][0], y, ov07_022367B0[work->pair][0], gBattleAnimSlidingSpritePairDurations[work->pair]);
        ManagedSprite_GetPositionXY(work->second, &x, &y);
        BattleAnimUtil_InitLinearMotion(&work->secondMotion, x, ov07_022367B2[work->pair][0], y, ov07_022367B4[work->pair][0], gBattleAnimSlidingSpritePairDurations[work->pair]);
        work->pair++;
        work->state++;
    } break;
    case 2: {
        int done = 0;
        if (!BattleAnimUtil_UpdateLinearMotionSprite(&work->firstMotion, work->first)) {
            done++;
        }
        if (!BattleAnimUtil_UpdateLinearMotionSprite(&work->secondMotion, work->second)) {
            done++;
        }
        if (done >= 2) {
            if (work->pair <= 3) {
                ManagedSprite_SetDrawFlag(work->first, FALSE);
                ManagedSprite_SetDrawFlag(work->second, FALSE);
                work->state--;
            } else {
                work->state++;
            }
        }
    } break;
    case 3:
        if (work->eva > 0) {
            work->eva--;
        }
        if (work->evb < 15) {
            work->evb++;
        }
        if (work->evb >= 7) {
            stopAnimation = TRUE;
        }
        if (work->eva == 0 && work->evb == 15) {
            work->state++;
        }
        evb = *(vu8 *)&work->evb;
        eva = *(vu8 *)&work->eva;
        reg_G2_BLDALPHA = (evb << 8) | eva;
        break;
    default: {
        int i;
        for (i = 0; i < work->count; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    }
    if (work->state < 3 && !stopAnimation) {
        int i;
        for (i = 0; i < work->count; i++) {
            Sprite_TickFrame(work->sprites[i]->sprite);
        }
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_SlidingSpritePairs(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimSlidingSpritePairsWork *work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimSlidingSpritePairsWork));
    ManagedSpriteTemplate template;
    int i;

    GF_ASSERT(work != NULL);
    work->unk_07 = 0;
    work->state = 0;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->pair = 0;
    work->system = system;
    BattleAnimSystem_GetSpriteTemplate(&template, system);
    BattleAnimUtil_SetBlendWithBattleBg(work->system, -1, -1);
    work->eva = 0;
    work->evb = 15;
    BattleAnimTask_SlidingSpritePairsSetBlend(work->eva, work->evb);
    work->count = BattleAnimSystem_GetArg(work->system, 0);
    work->sprites[0] = sprite;
    for (i = 1; i < work->count; i++) {
        work->sprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
    }
    ManagedSprite_SetAnim(work->sprites[0], 0);
    ManagedSprite_SetAnim(work->sprites[1], 0);
    ManagedSprite_SetAnim(work->sprites[2], 1);
    ManagedSprite_SetAnim(work->sprites[3], 1);
    ManagedSprite_SetAnim(work->sprites[4], 2);
    ManagedSprite_SetAnim(work->sprites[5], 3);
    ManagedSprite_SetFlipMode(work->sprites[0], 1);
    ManagedSprite_SetFlipMode(work->sprites[3], 1);
    for (i = 0; i < work->count; i++) {
        ManagedSprite_SetAnimateFlag(work->sprites[i], TRUE);
        ManagedSprite_SetPositionXY(work->sprites[i], gBattleAnimSlidingSpritePairPositions[i][0], gBattleAnimSlidingSpritePairPositions[i][1]);
        ManagedSprite_SetOamMode(work->sprites[i], GX_OAM_MODE_XLU);
    }
    ManagedSprite_SetDrawFlag(work->sprites[1], FALSE);
    ManagedSprite_SetDrawFlag(work->sprites[2], FALSE);
    ManagedSprite_SetDrawFlag(work->sprites[4], FALSE);
    ManagedSprite_SetDrawFlag(work->sprites[5], FALSE);
    BattleAnimSystem_CreateTask(system, BattleAnimTask_SlidingSpritePairs, work, 1100);
}

void BattleAnimTask_PopAndScatterSprite(SysTask *task, void *data) {
    BattleAnimScatterSprite *work = data;

    switch (work->state) {
    case 0:
        work->delay--;
        if (work->delay <= 10) {
            work->state++;
        }
        break;
    case 1:
        work->timer++;
        if (work->timer >= 3) {
            ManagedSprite_SetDrawFlag(work->sprite, TRUE);
            work->timer = 0;
            work->state++;
        }
        break;
    case 2:
        work->timer++;
        if (work->timer <= 3) {
            work->scale += 0.1f;
            ManagedSprite_SetAffineScale(work->sprite, work->scale, work->scale);
        } else {
            work->timer = 0;
            work->state++;
        }
        break;
    case 3:
        work->timer++;
        if (work->timer <= 3) {
            work->scale -= 0.1f;
            ManagedSprite_SetAffineScale(work->sprite, work->scale, work->scale);
        } else {
            work->timer = 0;
            work->state++;
        }
        break;
    case 4:
        work->delay--;
        if (work->delay <= 10) {
            *work->status = 1;
            work->timer = 0;
            work->state++;
        }
        break;
    case 5:
        work->timer++;
        if (work->timer > 30) {
            work->timer = 0;
            work->state++;
        } else if (*work->status == 2) {
            work->state++;
        }
        break;
    case 6: {
        int duration = (int)LCRandom() % 10 + 10;
        s16 y;
        s16 x;
        ManagedSprite_GetPositionXY(work->sprite, &x, &y);
        BattleAnimUtil_InitLinearMotion(&work->motion, x, gBattleAnimScatterTargets[work->target][0], y, ov07_02236780[work->target][0], duration);
        work->state++;
    } break;
    case 7:
        if (BattleAnimUtil_UpdateLinearMotionSprite(&work->motion, work->sprite)) {
            break;
        }
        ManagedSprite_SetDrawFlag(work->sprite, FALSE);
        work->state++;
        // fallthrough
    default:
        *work->status = 3;
        BattleAnimSystem_DestroyTask(work->system, task);
        break;
    }
    ManagedSprite_TickFrame(work->sprite);
}

void BattleAnimTask_PopAndScatterSprites(SysTask *task, void *data) {
    int i;
    BOOL ready;
    BOOL done;
    BattleAnimPopAndScatterSpritesWork *work = data;

    switch (work->state) {
    case 0:
        ready = TRUE;
        for (i = 0; i < work->count; i++) {
            if (work->status[i] != 1) {
                ready = FALSE;
                break;
            }
        }
        if (ready == TRUE) {
            work->state++;
        }
        break;
    case 1:
        for (i = 0; i < work->count; i++) {
            work->status[i] = 2;
        }
        work->state++;
        break;
    case 2:
        done = TRUE;
        for (i = 0; i < work->count; i++) {
            if (work->status[i] != 3) {
                done = FALSE;
                break;
            }
        }
        if (done == TRUE) {
            work->state++;
        }
        break;
    case 3:
        for (i = 0; i < work->count; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i].sprite);
        }
        work->state++;
        break;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_PopAndScatterSprites(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;
    BattleAnimPopAndScatterSpritesWork *work;
    int i;
    int area;
    s16 x;
    s16 y;

    work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimPopAndScatterSpritesWork));
    GF_ASSERT(work != NULL);
    work->unk_01 = 0;
    work->state = 0;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->system = system;
    BattleAnimSystem_GetSpriteTemplate(&template, system);
    BattleAnimUtil_SetBlendWithBattleBg(work->system, -1, -1);
    work->count = BattleAnimSystem_GetArg(work->system, 0);
    work->count = 12; // the script argument is overridden
    work->sprites[0].sprite = sprite;
    area = 0;
    for (i = 0; i < work->count; i++) {
        if (i != 0) {
            work->sprites[i].sprite = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        }
        sprite = work->sprites[i].sprite;
        work->sprites[i].state = 0;
        work->sprites[i].timer = 0;
        work->sprites[i].delay = i + (LCRandom() % 10 + 10);
        work->sprites[i].target = LCRandom() % 6;
        work->sprites[i].scale = 1.0f;
        work->sprites[i].system = work->system;
        work->sprites[i].spriteSystem = work->spriteSystem;
        work->sprites[i].spriteManager = work->spriteManager;
        work->sprites[i].status = &work->status[i];
        x = gBattleAnimScatterAreas[area].x + LCRandom() % gBattleAnimScatterAreas[area].xRange;
        y = gBattleAnimScatterAreas[area].y + LCRandom() % gBattleAnimScatterAreas[area].yRange;
        ManagedSprite_SetPositionXY(sprite, x, y);
        ManagedSprite_SetAffineOverwriteMode(sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
        ManagedSprite_SetAffineScale(sprite, work->sprites[i].scale, work->sprites[i].scale);
        ManagedSprite_SetDrawFlag(sprite, FALSE);
        BattleAnimSystem_CreateTask(system, BattleAnimTask_PopAndScatterSprite, &work->sprites[i], 1099);
        area = (area + 1) % 6;
    }
    BattleAnimSystem_CreateTask(system, BattleAnimTask_PopAndScatterSprites, work, 1101);
}

#pragma force_active reset

void BattleAnimTask_WaveAttackerOnBg(SysTask *task, void *data) {
    BattleAnimWaveAttackerOnBgWork *work = data;
    int layer;
    u32 fill;

    switch (work->common.state[0]) {
    case 0:
        Pokepic_SetAttr(BattleAnimSystem_GetBattlerPokepic(work->common.system, BattleAnimSystem_GetAttacker(work->common.system)), POKEPIC_VANISHED, TRUE);
        work->common.state[0]++;
        break;
    case 1:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            work->timer = 0;
            layer = BattleAnimSystem_GetBgMaskLayer(work->common.system, 1);
            fill = BattleAnimUtil_PackXY(0, 0);
            work->wave = BattleAnimScanline_NewWaveU32Step(0, 160, 364, 0xC000, 200, layer, 0, fill, BattleAnimSystem_GetHeapID(work->common.system));
            work->common.state[0]++;
        }
        break;
    case 2:
        work->timer++;
        if (work->timer >= 120) {
            BattleAnimScanline_FreeWave(work->wave);
            BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 2, 16, 16, 2, 16);
            work->common.state[0]++;
        }
        break;
    case 3:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
            Pokepic_SetAttr(BattleAnimSystem_GetBattlerPokepic(work->common.system, BattleAnimSystem_GetAttacker(work->common.system)), POKEPIC_VANISHED, FALSE);
            work->common.state[0]++;
        }
        break;
    default:
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
        BattleAnimBackground_ResetLayer(work->common.system, GF_BG_LYR_MAIN_2);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        return;
    }
    ManagedSprite_TickFrame(work->sprite);
    ManagedSprite_TickFrame(work->windowSprite);
    SpriteSystem_DrawSprites(work->common.battlerSpriteManager);
}

void BattleAnimFunc_WaveAttackerOnBg(BattleAnimSystem *system) {
    BattleAnimWaveAttackerOnBgWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimWaveAttackerOnBgWork));

    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->windowSprite = BattleAnimSystem_GetBattlerSprite(work->common.system, 0);
    ManagedSprite_SetOamMode(work->windowSprite, GX_OAM_MODE_OBJWND);
    GX_SetVisibleWnd(GX_WNDMASK_OW);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, FALSE);
    BattleAnim_SetObjWindowInsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_OBJ, FALSE);
    work->sprite = BattleAnimSystem_GetBattlerSprite(work->common.system, 1);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    BattleAnimUtil_StartBlendAlphaMotion(&work->blend, 16, 2, 2, 16, 16);
    BattleAnimBackground_LoadBattleBgToLayerDefault(work->common.system, GF_BG_LYR_MAIN_2);
    ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_ON);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_WaveAttackerOnBg, work);
}

void BattleAnimUtil_TickSpriteIfVisible(ManagedSprite *sprite) {
    if (ManagedSprite_GetDrawFlag(sprite) == TRUE) {
        ManagedSprite_TickFrame(sprite);
    }
}

void BattleAnimTask_SpriteQuad(SysTask *task, void *data) {
    int i;
    int hidden;
    BattleAnimSpriteQuadWork *work = data;

    if (work->delay <= 0) {
        BattleAnimUtil_TickSpriteIfVisible(work->sprites[2]);
        BattleAnimUtil_TickSpriteIfVisible(work->sprites[3]);
    } else {
        work->delay--;
    }
    BattleAnimUtil_TickSpriteIfVisible(work->sprites[0]);
    BattleAnimUtil_TickSpriteIfVisible(work->sprites[1]);
    hidden = 0;
    for (i = 0; i < 4; i++) {
        work->timers[i]++;
        if (work->timers[i] >= 40) {
            ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
            hidden++;
        }
    }
    if (hidden == 4) {
        for (i = 0; i < 4; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
    } else {
        SpriteSystem_DrawSprites(work->common.spriteManager);
    }
}

void BattleAnimSpriteFunc_SpriteQuad(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;
    BattleAnimSpriteQuadWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSpriteQuadWork));
    int i;

    work->delay = 10;
    BattleAnimUtil_InitCommonWork(system, &work->common);
    BattleAnimSystem_GetSpriteTemplate(&template, work->common.system);
    work->sprites[0] = sprite;
    work->timers[0] = 0;
    for (i = 1; i < 4; i++) {
        work->timers[i] = 0;
        work->sprites[i] = SpriteSystem_NewSprite(work->common.spriteSystem, work->common.spriteManager, &template);
    }
    ManagedSprite_SetFlipMode(work->sprites[0], 1);
    ManagedSprite_SetFlipMode(work->sprites[1], 1);
    ManagedSprite_OffsetPositionXY(work->sprites[0], -32, 0);
    ManagedSprite_OffsetPositionXY(work->sprites[1], -32, 32);
    ManagedSprite_OffsetPositionXY(work->sprites[2], 32, 0);
    ManagedSprite_OffsetPositionXY(work->sprites[3], 32, 32);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_SpriteQuad, work);
}

#pragma force_active on

void BattleAnimTask_FlashingSpritesAroundAttacker(SysTask *task, void *data) {
    int i;
    int done = 0;
    BattleAnimFlashingSpritesAroundAttackerWork *work = data;

    for (i = 0; i < 4; i++) {
        work->timers[i]++;
        if (work->timers[i] >= 20) {
            BattleAnimUtil_TickSpriteIfVisible(work->sprites[i]);
            if (work->timers[0] == 110) {
                BattleAnimTask_FlashingSpritesAroundAttackerStartFade(work);
            }
        }
    }
    if (work->timers[0] == 50) {
        work->paletteBlends[0] = BattleAnimUtil_StartPaletteBlend(work->common.paletteData, BattleAnimSystem_GetHeapID(work->common.system), PLTTBUF_MAIN_OBJ, (u16)(work->paletteSlot << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
    }
    if (work->timers[0] == 60) {
        work->paletteBlends[1] = BattleAnimUtil_StartPaletteBlend(work->common.paletteData, BattleAnimSystem_GetHeapID(work->common.system), PLTTBUF_MAIN_OBJ, (u16)(work->paletteSlot << 4), 16, -2, 2, 14, 0, 0xFFFF, 1002);
    }
    if (work->timers[0] == 70) {
        work->paletteBlends[2] = BattleAnimUtil_StartPaletteBlend(work->common.paletteData, BattleAnimSystem_GetHeapID(work->common.system), PLTTBUF_MAIN_OBJ, (u16)(work->paletteSlot << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
    }
    if (work->timers[0] == 80) {
        work->paletteBlends[3] = BattleAnimUtil_StartPaletteBlend(work->common.paletteData, BattleAnimSystem_GetHeapID(work->common.system), PLTTBUF_MAIN_OBJ, (u16)(work->paletteSlot << 4), 16, -2, 2, 14, 0, 0xFFFF, 1002);
    }
    if (work->timers[0] == 90) {
        work->paletteBlends[4] = BattleAnimUtil_StartPaletteBlend(work->common.paletteData, BattleAnimSystem_GetHeapID(work->common.system), PLTTBUF_MAIN_OBJ, (u16)(work->paletteSlot << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
    }
    if (work->timers[0] == 100) {
        work->paletteBlends[5] = BattleAnimUtil_StartPaletteBlend(work->common.paletteData, BattleAnimSystem_GetHeapID(work->common.system), PLTTBUF_MAIN_OBJ, (u16)(work->paletteSlot << 4), 16, -2, 2, 14, 0, 0xFFFF, 1002);
    }
    if (work->timers[0] == 110) {
        BattleAnimTask_FlashingSpritesAroundAttackerStartFade(work);
    }
    if (work->timers[0] >= 110) {
        if (work->eva > 0) {
            work->eva--;
        }
        if (work->evb < 15) {
            work->evb++;
        }
        reg_G2_BLDALPHA = work->eva | (work->evb << 8);
        if (work->eva == 0 && work->evb == 15) {
            done = 4;
        }
    }
    if (done == 4) {
        for (i = 0; i < 4; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        for (i = 0; i < 6; i++) {
            BattleAnimUtil_FreePaletteBlend(work->paletteBlends[i]);
        }
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
    } else {
        SpriteSystem_DrawSprites(work->common.spriteManager);
    }
}

void BattleAnimSpriteFunc_FlashingSpritesAroundAttacker(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimFlashingSpritesAroundAttackerWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimFlashingSpritesAroundAttackerWork));
    ManagedSpriteTemplate template;
    Pokepic *pokepic;
    int i;
    int delay;
    int y;
    s16 x;

    work->unk_20 = 10;
    BattleAnimUtil_InitCommonWork(system, &work->common);
    BattleAnimSystem_GetSpriteTemplate(&template, work->common.system);
    work->sprites[0] = sprite;
    work->timers[0] = 0;
    for (i = 1, delay = 5; i < 4; delay += 5, i++) {
        work->timers[i] = delay + (s32)LCRandom() % 4;
        work->sprites[i] = SpriteSystem_NewSprite(work->common.spriteSystem, work->common.spriteManager, &template);
    }
    ManagedSprite_SetFlipMode(work->sprites[1], 1);
    ManagedSprite_SetFlipMode(work->sprites[2], 1);
    pokepic = BattleAnimSystem_GetBattlerPokepic(system, BattleAnimSystem_GetAttacker(system));
    x = Pokepic_GetAttr(pokepic, POKEPIC_X);
    Pokepic_GetAttr(pokepic, POKEPIC_Y);
    Pokepic_GetAttr(pokepic, POKEPIC_SHADOW_H);
    if (BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetAttacker(system)) == BATTLE_ANIM_SIDE_PLAYER) {
        y = 140;
    } else {
        y = 84;
    }
    for (i = 0; i < 4; i++) {
        ManagedSprite_SetPositionXYInt(work->sprites[i], x, y);
    }
    if (BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetAttacker(system)) == BATTLE_ANIM_SIDE_PLAYER) {
        ManagedSprite_SetDrawPriority(work->sprites[0], 10);
        ManagedSprite_SetDrawPriority(work->sprites[2], 10);
        ManagedSprite_SetDrawPriority(work->sprites[1], 18);
        ManagedSprite_SetDrawPriority(work->sprites[3], 18);
    } else {
        ManagedSprite_SetDrawPriority(work->sprites[0], 18);
        ManagedSprite_SetDrawPriority(work->sprites[2], 18);
        ManagedSprite_SetDrawPriority(work->sprites[1], 10);
        ManagedSprite_SetDrawPriority(work->sprites[3], 10);
    }
    ManagedSprite_OffsetPositionXY(work->sprites[0], -32, 0);
    ManagedSprite_OffsetPositionXY(work->sprites[3], -40, 4);
    ManagedSprite_OffsetPositionXY(work->sprites[2], 32, 0);
    ManagedSprite_OffsetPositionXY(work->sprites[1], 40, 4);
    work->paletteSlot = ManagedSprite_GetPaletteOverrideOffset(work->sprites[0]);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_FlashingSpritesAroundAttacker, work);
}

#pragma force_active reset

void BattleAnimTask_ShrinkSpriteOnDefender(SysTask *task, void *data) {
    f32 scaleX;
    f32 scaleY;
    BattleAnimShrinkSpriteOnDefenderWork *work = data;

    if (work->common.state[0] == 0) {
        if (!BattleAnimUtil_UpdateScaleMotionXY(&work->scale)) {
            // sprite3 is not hidden
            ManagedSprite_SetDrawFlag(work->sprite, FALSE);
            ManagedSprite_SetDrawFlag(work->sprite2, FALSE);
            ManagedSprite_SetDrawFlag(work->sprite2, FALSE);
            BattleAnimSystem_DestroyTask(work->common.system, task);
            BattleAnimUtil_FreeWork(work);
            return;
        }
        BattleAnimUtil_GetScaleFloat(&work->scale, &scaleX, &scaleY);
        ManagedSprite_SetAffineScale(work->sprite, scaleX, scaleY);
        BattleAnimUtil_AlignSpriteToScale(work->sprite, work->y, work->height, work->scale.curY, 0);
        ManagedSprite_TickFrame(work->sprite);
        ManagedSprite_TickFrame(work->sprite2);
        ManagedSprite_TickFrame(work->sprite3);
        SpriteSystem_DrawSprites(work->common.battlerSpriteManager);
    }
}

void BattleAnimFunc_ShrinkSpriteOnDefender(BattleAnimSystem *system) {
    u32 battlerType;
    BattleAnimShrinkSpriteOnDefenderWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimShrinkSpriteOnDefenderWork));

    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->common.system, BattleAnimSystem_GetDefender(work->common.system));
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->baseY = work->y;
    work->height = BattleAnimSystem_GetBattlerHeight(work->common.system, BattleAnimSystem_GetDefender(work->common.system));
    work->sprite = BattleAnimSystem_GetBattlerSprite(system, 0);
    work->sprite2 = BattleAnimSystem_GetBattlerSprite(system, 1);
    work->sprite3 = BattleAnimSystem_GetBattlerSprite(system, 2);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    ManagedSprite_SetPriority(work->sprite, BattleAnimSystem_GetBasePriority(system));
    ManagedSprite_SetPriority(work->sprite2, BattleAnimSystem_GetBasePriority(system));
    ManagedSprite_SetPriority(work->sprite3, BattleAnimSystem_GetBasePriority(system));
    battlerType = BattleAnimSystem_GetBattlerType(work->common.system, BattleAnimSystem_GetDefender(work->common.system));
    if (battlerType == BATTLER_TYPE_ENEMY_SIDE_SLOT_1 || battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
        ManagedSprite_SetDrawPriority(work->sprite, 30);
        ManagedSprite_SetDrawPriority(work->sprite2, 50);
        ManagedSprite_SetDrawPriority(work->sprite3, 70);
    } else {
        ManagedSprite_SetDrawPriority(work->sprite, 60);
        ManagedSprite_SetDrawPriority(work->sprite2, 70);
        ManagedSprite_SetDrawPriority(work->sprite3, 50);
    }
    BattleAnimUtil_InitScaleMotionXY(&work->scale, 100, 20, 100, 20, 100, 10);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_ShrinkSpriteOnDefender, work);
}

#pragma force_active on

void BattleAnimTask_AttackerDashAfterimages(SysTask *task, void *data) {
    int done;
    BattleAnimAttackerDashAfterimagesWork *work = data;

    switch (work->common.state[0]) {
    case 0:
        if (!work->leg) {
            BattleAnimUtil_InitLinearMotion(&work->pokepicMotion, work->x, work->x + 48, work->y, work->y, 3);
            BattleAnimUtil_InitLinearMotion(&work->sprite1Motion, work->x, work->x + 48, work->y + work->dy, work->y + work->dy, 6);
            BattleAnimUtil_InitLinearMotion(&work->sprite2Motion, work->x, work->x + 48, work->y + work->dy, work->y + work->dy, 8);
        } else {
            BattleAnimUtil_InitLinearMotion(&work->pokepicMotion, work->x + 48, work->x, work->y, work->y, 3);
            BattleAnimUtil_InitLinearMotion(&work->sprite1Motion, work->x + 48, work->x, work->y + work->dy, work->y + work->dy, 6);
            BattleAnimUtil_InitLinearMotion(&work->sprite2Motion, work->x + 48, work->x, work->y + work->dy, work->y + work->dy, 9);
        }
        work->leg++;
        work->common.state[0]++;
        break;
    case 1:
        done = 0;
        if (!BattleAnimUtil_UpdateLinearMotionSprite(&work->sprite1Motion, work->sprite1)) {
            done++;
        }
        if (!BattleAnimUtil_UpdateLinearMotionPokepic(&work->pokepicMotion, work->pokepic)) {
            done++;
        }
        if (!BattleAnimUtil_UpdateLinearMotionSprite(&work->sprite2Motion, work->sprite2)) {
            done++;
        }
        if (done >= 3) {
            if (work->leg == 1) {
                work->common.state[0]--;
            } else {
                work->common.state[0]++;
            }
        }
        break;
    default:
        BattleAnimSystem_DestroyTask(work->common.system, task);
        BattleAnimUtil_FreeWork(work);
        return;
    }
    ManagedSprite_TickFrame(work->sprite1);
    SpriteSystem_DrawSprites(work->common.battlerSpriteManager);
}

void BattleAnimFunc_AttackerDashAfterimages(BattleAnimSystem *system) {
    BattleAnimAttackerDashAfterimagesWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimAttackerDashAfterimagesWork));
    int member;
    NarcId narcId;
    ManagedSprite *sprite;
    u32 paletteOffset;
    PaletteData *paletteData;

    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->unk_A1 = 0;
    work->eva = 8;
    work->evb = 8;
    BattleAnimUtil_SetBlendWithBattleBg(work->common.system, -1, -1);
    reg_G2_BLDALPHA = work->eva | (work->evb << 8);
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->common.system, BattleAnimSystem_GetAttacker(work->common.system));
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->sprite1 = BattleAnimSystem_GetBattlerSprite(work->common.system, 0);
    work->sprite2 = BattleAnimSystem_GetBattlerSprite(work->common.system, 1);
    work->dy = -Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H);
    member = BattleAnimSystem_GetBattlerPaletteId(work->common.system, BattleAnimSystem_GetAttacker(work->common.system));
    narcId = (NarcId)BattleAnimSystem_GetBattlerNarcId(work->common.system, BattleAnimSystem_GetAttacker(work->common.system));
    sprite = work->sprite1;
    ManagedSprite_SetPriority(sprite, BattleAnimSystem_GetBasePriority(work->common.system) + 1);
    paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(sprite->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    paletteData = BattleAnimSystem_GetPaletteData(work->common.system);
    PaletteData_LoadNarc_CustomTint(paletteData, narcId, member, BattleAnimSystem_GetHeapID(work->common.system), PLTTBUF_MAIN_OBJ, 32, (u16)(paletteOffset << 4), 196, 196, 196);
    ManagedSprite_SetOamMode(sprite, GX_OAM_MODE_XLU);
    sprite = work->sprite2;
    ManagedSprite_SetPriority(sprite, BattleAnimSystem_GetBasePriority(work->common.system) + 1);
    paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(sprite->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    paletteData = BattleAnimSystem_GetPaletteData(work->common.system);
    PaletteData_LoadNarc_CustomTint(paletteData, narcId, member, BattleAnimSystem_GetHeapID(work->common.system), PLTTBUF_MAIN_OBJ, 32, (u16)(paletteOffset << 4), 196, 196, 196);
    ManagedSprite_SetOamMode(sprite, GX_OAM_MODE_XLU);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_AttackerDashAfterimages, work);
}

#pragma force_active reset

void BattleAnimTask_SpriteTrailToDefender(SysTask *task, void *data) {
    int i;
    int hidden = 0;
    BattleAnimSpriteTrailToDefenderWork *work = data;

    for (i = 0; i < 8; i++) {
        work->timers[i]++;
        if (work->timers[i] >= 32) {
            BattleAnimUtil_TickSpriteIfVisible(work->sprites[i]);
            if (work->timers[i] >= 160) {
                ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
                hidden++;
            }
        }
    }
    if (hidden == 8) {
        for (i = 0; i < 8; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
    } else {
        SpriteSystem_DrawSprites(work->common.spriteManager);
    }
}

#pragma force_active on

void BattleAnimSpriteFunc_SpriteTrailToDefender(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;
    BattleAnimSpriteTrailToDefenderWork *work;
    int i;
    s16 attackerX;
    s16 defenderX;
    s16 attackerY;
    s16 defenderY;
    Pokepic *attackerPic;
    Pokepic *defenderPic;
    int frame;
    int count;
    BOOL active;
    BOOL skip;
    int priority1;
    int priority2;

    work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSpriteTrailToDefenderWork));
    work->delay = 10;
    BattleAnimUtil_InitCommonWork(system, &work->common);
    BattleAnimSystem_GetSpriteTemplate(&template, work->common.system);
    work->sprites[0] = sprite;
    for (i = 1; i < 8; i++) {
        work->sprites[i] = SpriteSystem_NewSprite(work->common.spriteSystem, work->common.spriteManager, &template);
        if (i % 2) {
            ManagedSprite_SetFlipMode(work->sprites[i], 1);
        }
    }
    attackerPic = BattleAnimSystem_GetBattlerPokepic(work->common.system, BattleAnimSystem_GetAttacker(work->common.system));
    attackerX = Pokepic_GetAttr(attackerPic, POKEPIC_X);
    attackerY = Pokepic_GetAttr(attackerPic, POKEPIC_Y);
    defenderPic = BattleAnimSystem_GetBattlerPokepic(work->common.system, BattleAnimSystem_GetDefender(work->common.system));
    defenderX = Pokepic_GetAttr(defenderPic, POKEPIC_X);
    defenderY = Pokepic_GetAttr(defenderPic, POKEPIC_Y);
    BattleAnimUtil_InitLinearMotion(&work->motion, attackerX, defenderX, attackerY, defenderY, 20);
    frame = 0;
    count = 0;
    skip = FALSE;
    if (BattleAnimUtil_GetBattlerSide(work->common.system, BattleAnimSystem_GetAttacker(work->common.system)) == BATTLE_ANIM_SIDE_ENEMY) {
        skip = TRUE;
    }
    // place the sprites on every other step of the path
    do {
        active = BattleAnimUtil_UpdateLinearMotion(&work->motion);
        if (frame % 2) {
            if (skip == FALSE) {
                if (count < 8) {
                    ManagedSprite_SetPositionXY(work->sprites[count], work->motion.x, work->motion.y);
                    count++;
                }
            } else {
                skip = FALSE;
            }
        }
        frame++;
    } while (active == TRUE);
    priority1 = BattleAnimSystem_GetBgPriority(work->common.system, 1);
    priority2 = BattleAnimSystem_GetBgPriority(work->common.system, 2);
    for (i = 0; i < 8; i++) {
        work->timers[i] = (8 - i) * 4;
        if (BattleAnimSystem_GetField08(work->common.system) == 0) {
            if (BattleAnimUtil_GetBattlerSide(work->common.system, BattleAnimSystem_GetAttacker(work->common.system)) == BATTLE_ANIM_SIDE_PLAYER) {
                if (i < 4) {
                    ManagedSprite_SetPriority(work->sprites[i], priority2);
                } else {
                    ManagedSprite_SetPriority(work->sprites[i], priority1);
                }
            } else {
                if (i >= 4) {
                    ManagedSprite_SetPriority(work->sprites[i], priority2);
                } else {
                    ManagedSprite_SetPriority(work->sprites[i], priority1);
                }
            }
            ManagedSprite_SetDrawPriorityInt(work->sprites[i], 8 - i);
        } else {
            ManagedSprite_SetDrawPriorityInt(work->sprites[i], i);
            ManagedSprite_SetPriority(work->sprites[i], priority2);
        }
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_SpriteTrailToDefender, work);
}

#pragma force_active reset

// A copy of a battle sprite next to the original squashes flat while the palettes of two sprites blend
typedef struct BattleAnimSquashSpriteWork {
    BattleAnimCommonWork common;
    ManagedSprite *sprites[3]; // 0x1C, [0]: the squashed copy
    int timer;
    f32 scaleX;
    f32 scaleY;
    u8 eva; // 0x34
    u8 evb;
    s8 direction; // sign of the x scale
    u8 unk_37;
    BattleAnimPaletteBlend *paletteBlends[2]; // 0x38, of sprites[0] and sprites[2]
} BattleAnimSquashSpriteWork;                 // size: 0x40

typedef struct BattleAnimSlideOutAndTransformWork {
    BattleAnimCommonWork common;
    BattleAnimTarget target;       // 0x1C
    BattleAnimLinearMotion motion; // 0x30
    s16 direction;                 // 0x54, 1: attacker on the player side
    u8 unk_56[2];
    BOOL sameSide;                   // 0x58
    BattleAnimPosition positions[2]; // 0x5C, attacker and defender
    u8 unk_64[4];
    int leg; // 0x68
    int targetX[3];
    int targetY[3];
    int targetZ[3];
} BattleAnimSlideOutAndTransformWork; // size: 0x90

typedef struct BattleAnimWipeOutDefenderWork {
    int count; // strips wiped
    int top;
    int left;
    int bottom;
    int stripWidth;
    BOOL fromRight;
    int alpha;
    Pokepic *pokepic;
    BattleAnimCommonWork common; // 0x20
    u8 unk_3C[0x14];
} BattleAnimWipeOutDefenderWork; // size: 0x50

// The attacker's pic circles while two sprites follow the same path
typedef struct BattleAnimPicOrbitWork {
    BattleAnimSystem *system;
    SpriteManager *spriteManager;
    Pokepic *pokepic;
    BattleAnimSinMotion motion;        // 0x0C
    ManagedSprite *sprites[2];         // 0x30
    BattleAnimSpriteSequence sequence; // 0x38
    s16 x;                             // 0xE8
    s16 y;
    s16 shadowHeight;
    u8 unk_EE[2];
} BattleAnimPicOrbitWork; // size: 0xF0

typedef struct BattleAnimAttackerRecoilTiltWork {
    BattleAnimSystem *system;
    int state;
    s16 timer;
    Pokepic *pokepic;               // 0x0C
    BattleAnimLinearMotion offset;  // 0x10
    BattleAnimValueMotion rotation; // 0x34
    s16 x;                          // 0x48
    s16 y;
    u8 unk_4C[0xA4];
} BattleAnimAttackerRecoilTiltWork; // size: 0xF0

typedef struct BattleAnimOrbitingSpriteTrailWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    BattleAnimMotionSprite main;     // 0x0C
    BattleAnimMotionSprite trail[2]; // 0x58
    int state;                       // 0xF0
    int timer;
    int index;
    int blend;                       // EVA * 10
} BattleAnimOrbitingSpriteTrailWork; // size: 0x100

struct BattleAnimShakeDefenderWithOverlayWork {
    BattleAnimSystem *system;
    SpriteManager *spriteManager;
    int state;
    int unk_0C;
    BattleAnimShake shake;        // 0x10
    BattleAnimLinearMotion scale; // 0x34
    ManagedSprite *sprite;        // 0x58
    Pokepic *pokepic;
    s16 x; // 0x60
    s16 y;
    s16 jitter; // sign of the x scale jitter
    s16 shadowHeight;
}; // size: 0x68

void BattleAnimTask_SquashSprite(SysTask *task, void *data) {
    u32 paletteOffset;
    f32 scaleX;
    BattleAnimSquashSpriteWork *work = data;

    switch (work->common.state[0]) {
    case 0:
        if (work->timer > 14) {
            work->scaleY -= 0.2f;
            work->scaleX += 0.2f;
            if (work->scaleY <= 0.2) {
                work->common.state[0]++;
                ManagedSprite_SetDrawFlag(work->sprites[0], FALSE);
            } else {
                scaleX = work->scaleX * (f32)work->direction;
                ManagedSprite_SetAffineScale(work->sprites[0], scaleX, work->scaleY);
            }
        } else {
            work->eva++;
            work->evb--;
            reg_G2_BLDALPHA = work->eva | (work->evb << 8);
            work->timer++;
        }
        break;
    case 1:
        if (BattleAnimUtil_IsPaletteBlendActive(work->paletteBlends[1])) {
            break;
        }
        BattleAnimUtil_FreePaletteBlend(work->paletteBlends[0]);
        BattleAnimUtil_FreePaletteBlend(work->paletteBlends[1]);
        paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(work->sprites[2]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
        work->paletteBlends[1] = BattleAnimUtil_StartPaletteBlend(work->common.paletteData, BattleAnimSystem_GetHeapID(work->common.system), PLTTBUF_MAIN_OBJ, (u16)(paletteOffset << 4), 16, 0, 1, 15, 0, 0xFFFF, 1100);
        work->common.state[0]++;
        break;
    case 2:
    default:
        if (BattleAnimUtil_IsPaletteBlendActive(work->paletteBlends[1])) {
            break;
        }
        ManagedSprite_TickFrame(work->sprites[1]);
        BattleAnimUtil_FreePaletteBlend(work->paletteBlends[1]);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        return;
    }
    ManagedSprite_TickFrame(work->sprites[0]);
    ManagedSprite_TickFrame(work->sprites[1]);
    ManagedSprite_TickFrame(work->sprites[2]);
    SpriteSystem_DrawSprites(work->common.battlerSpriteManager);
}

#pragma force_active on

void BattleAnimFunc_SquashSprite(BattleAnimSystem *system) {
    s16 y;
    s16 x;
    s16 originX;
    s16 originY;
    BattleAnimPosition pos;
    BattleAnimSquashSpriteWork *work;
    int offset;
    u32 paletteOffset;
    u8 battler;
    u8 battlerType;
    int facing;
    int species;
    int gender;
    int form;
    int height;

    work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSquashSpriteWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->sprites[0] = BattleAnimSystem_GetBattlerSprite(work->common.system, 0);
    work->sprites[1] = BattleAnimSystem_GetBattlerSprite(work->common.system, 1);
    work->sprites[2] = BattleAnimSystem_GetBattlerSprite(work->common.system, 2);
    // the factor goes through offset (zero first) to match
    offset = 0;
    work->timer = offset;
    offset = -32;
    offset = (s16)BattleAnimUtil_GetBattlerSideSignByMode(system, BattleAnimSystem_GetAttacker(system)) * offset;
    work->scaleX = 1.0f;
    work->scaleY = 1.0f;
    if (BattleAnimSystem_ShouldFlipSide(system, 1) == TRUE) {
        work->direction = -1;
    } else {
        work->direction = 1;
    }
    ManagedSprite_GetPositionXY(work->sprites[1], &originX, &originY);
    ManagedSprite_SetPositionXY(work->sprites[0], originX + offset, originY);
    ManagedSprite_SetAffineOverwriteMode(work->sprites[0], NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    ManagedSprite_SetAffineScale(work->sprites[0], work->scaleX * (f32)work->direction, work->scaleY);
    paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(work->sprites[0]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    work->paletteBlends[0] = BattleAnimUtil_StartPaletteBlend(work->common.paletteData, BattleAnimSystem_GetHeapID(system), PLTTBUF_MAIN_OBJ, (u16)(paletteOffset << 4), 16, 0, 1, 0, 15, 0xFFFF, 1100);
    paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(work->sprites[2]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    work->paletteBlends[1] = BattleAnimUtil_StartPaletteBlend(work->common.paletteData, BattleAnimSystem_GetHeapID(system), PLTTBUF_MAIN_OBJ, (u16)(paletteOffset << 4), 16, 0, 1, 0, 15, 0xFFFF, 1100);
    battler = BattleAnimSystem_GetAttacker(work->common.system);
    battlerType = BattleAnimUtil_GetBattlerType(work->common.system, battler);
    BattleAnimUtil_GetBattlerTypePosition(battlerType, BattleAnimSystem_GetField08(work->common.system), &pos);
    ManagedSprite_GetPositionXY(work->sprites[0], &x, &y);
    if (BattleAnimUtil_GetBattlerSide(work->common.system, battler) == BATTLE_ANIM_SIDE_PLAYER) {
        facing = 0;
        ManagedSprite_SetPriority(work->sprites[0], 1);
        ManagedSprite_SetDrawPriority(work->sprites[0], facing);
    } else {
        ManagedSprite_SetPriority(work->sprites[0], 2);
        ManagedSprite_SetDrawPriority(work->sprites[0], 0);
        facing = 2;
    }
    battler = BattleAnimSystem_GetDefender(work->common.system);
    species = BattleAnimSystem_GetBattlerSpecies(work->common.system, battler);
    gender = BattleAnimSystem_GetBattlerGender(work->common.system, battler);
    form = BattleAnimSystem_GetBattlerForm(work->common.system, battler);
    height = GetMonPicHeightBySpeciesGenderForm(species, gender, facing, form, BattleAnimSystem_GetBattlerPersonality(work->common.system, battler));
    ManagedSprite_SetPositionXY(work->sprites[0], x, pos.y + height);
    ManagedSprite_SetDrawFlag(work->sprites[0], TRUE);
    work->eva = 0;
    work->evb = 15;
    BattleAnimUtil_SetBlendWithBattleBg(work->common.system, -1, -1);
    reg_G2_BLDALPHA = work->eva | (work->evb << 8);
    BattleAnim_CreateTaskAndRun(work->common.system, BattleAnimTask_SquashSprite, work);
}

#pragma force_active reset

void BattleAnimTask_SlideOutAndTransform(SysTask *task, void *data) {
    BattleAnimSlideOutAndTransformWork *work = data;
    BattleAnimBattlerPicParams picParams;

    switch (work->common.state[0]) {
    case 0:
        BattleAnimUtil_InitLinearMotion(&work->motion, work->target.pos[0], (s16)work->targetX[work->leg], (s16)work->targetY[work->leg], (s16)work->targetY[work->leg], 15);
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_Z, work->targetZ[work->leg]);
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_SHADOW_YADJ_REQ, 1);
        work->common.state[0]++;
    case 1:
        if (!BattleAnimUtil_UpdateLinearMotion(&work->motion)) {
            work->leg++;
            if (work->leg > 2) {
                work->common.state[0]++;
            } else {
                if (!work->sameSide) {
                    BattleAnimSystem_BuildBattlerPicParams(work->common.system, &picParams, 3);
                    if (work->leg == 2) {
                        BattleAnimUtil_RestorePic(&picParams, BattleAnimSystem_GetHeapID(work->common.system));
                    } else {
                        BattleAnimUtil_LoadMonIntoPicOppositeFacing(&picParams, BattleAnimSystem_GetHeapID(work->common.system));
                    }
                }
                work->common.state[0] = 0;
            }
            work->target.pos[0] = Pokepic_GetAttr(work->target.pokepic, POKEPIC_X);
        }
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_X, work->motion.x);
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_Y, work->motion.y);
        break;
    default:
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_SHADOW_YADJ_REQ, 0);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        break;
    }
}

void BattleAnimFunc_SlideOutAndTransform(BattleAnimSystem *system) {
    int count;
    u32 attacker;
    u32 attackerSide;
    u32 defenderSide;
    int dy;
    BattleAnimSlideOutAndTransformWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSlideOutAndTransformWork));

    BattleAnimUtil_InitCommonWork(system, &work->common);
    attacker = BattleAnimSystem_GetAttacker(work->common.system);
    BattleAnimUtil_GetBattlerPosition(work->common.system, BattleAnimSystem_GetAttacker(work->common.system), &work->positions[0]);
    BattleAnimUtil_GetBattlerPosition(work->common.system, BattleAnimSystem_GetDefender(work->common.system), &work->positions[1]);
    if (BattleAnimUtil_GetBattlerSide(work->common.system, attacker) == BATTLE_ANIM_SIDE_PLAYER) {
        work->direction = 1;
    } else {
        work->direction = -1;
    }
    attackerSide = BattleAnimUtil_GetBattlerSide(work->common.system, BattleAnimSystem_GetAttacker(work->common.system));
    defenderSide = BattleAnimUtil_GetBattlerSide(work->common.system, BattleAnimSystem_GetDefender(work->common.system));
    work->sameSide = attackerSide == defenderSide;
    work->leg = 0;
    BattleAnimUtil_CollectPicsByFlags(system, BattleAnimSystem_GetArg(system, 0), &work->target, &count);
    dy = Pokepic_GetAttr(work->target.pokepic, POKEPIC_Y) - work->positions[0].y;
    if (work->direction > 0) {
        work->targetX[0] = 335;
        work->targetX[1] = -80;
        work->targetX[2] = work->positions[0].x;
        work->targetY[0] = work->positions[0].y + dy;
        work->targetY[1] = work->positions[1].y + dy;
        work->targetY[2] = work->positions[0].y + dy;
        work->targetZ[0] = 10;
        work->targetZ[1] = -650;
        work->targetZ[2] = Pokepic_GetAttr(work->target.pokepic, POKEPIC_Z);
    } else {
        work->targetX[0] = -80;
        work->targetX[1] = 335;
        work->targetX[2] = work->positions[0].x;
        work->targetY[0] = work->positions[0].y + dy;
        work->targetY[1] = work->positions[1].y + dy;
        work->targetY[2] = work->positions[0].y + dy;
        work->targetZ[0] = -650;
        work->targetZ[1] = 10;
        work->targetZ[2] = Pokepic_GetAttr(work->target.pokepic, POKEPIC_Z);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_SlideOutAndTransform, work);
}

// Shows only a strip of the pic: width x height pixels at x, yFromBottom above the bottom of the 80px pic
void BattleAnimUtil_SetPicVisibleRect(Pokepic *pokepic, int x, int width, int yFromBottom, int height) {
    Pokepic_SetVisible(pokepic, x, 80 - yFromBottom, width, height);
}

#pragma force_active on

void BattleAnimTask_WipeOutDefender(SysTask *task, void *data) {
    BattleAnimWipeOutDefenderWork *work = data;

    switch (work->common.state[0]) {
    case 0:
        work->stripWidth += 79;
        if (work->stripWidth >= 80) {
            work->common.state[0]++;
            work->fromRight ^= 1;
            work->stripWidth = 0;
        } else if (work->fromRight == FALSE) {
            BattleAnimUtil_SetPicVisibleRect(work->pokepic, 0, work->stripWidth, (work->count + 2) * 2, 2);
        } else {
            BattleAnimUtil_SetPicVisibleRect(work->pokepic, 80 - work->stripWidth, work->stripWidth, (work->count + 2) * 2, 2);
        }
        break;
    case 1:
        work->top--;
        work->bottom++;
        G2_SetWnd0Position(work->left, work->top, work->left + 80, work->bottom);
        work->common.state[0]++;
        break;
    case 2:
        work->top--;
        work->bottom++;
        G2_SetWnd0Position(work->left, work->top, work->left + 80, work->bottom);
        work->count++;
        if (work->count > 38) {
            work->common.state[0]++;
        } else {
            work->common.state[0] = 0;
        }
        break;
    case 3:
        Pokepic_SetAttr(work->pokepic, POKEPIC_VISIBLE, FALSE);
        work->common.state[0]++;
        break;
    default:
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
        G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_NONE, FALSE);
        G2_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE, FALSE);
        G2_SetWnd0Position(0, 0, 0, 0);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Pokepic_SetAttr(work->pokepic, POKEPIC_ALPHA, work->alpha);
        Heap_Free(work);
        break;
    }
}

void BattleAnimFunc_WipeOutDefender(BattleAnimSystem *system) {
    BattleAnimWipeOutDefenderWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimWipeOutDefenderWork));
    int battlerType;
    s16 x;
    s16 y;

    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(system, BattleAnimSystem_GetDefender(system));
    work->count = 0;
    battlerType = BattleAnimUtil_GetBattlerType(system, BattleAnimSystem_GetDefender(system));
    switch (battlerType) {
    case BATTLER_TYPE_ENEMY_SIDE_SLOT_1:
    case BATTLER_TYPE_PLAYER_SIDE_SLOT_2:
        SetBgPriority(GF_BG_LYR_MAIN_2, (u8)(BattleAnimSystem_GetBasePriority(system) - 1));
        break;
    }
    x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    y = y - Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H);
    work->alpha = Pokepic_GetAttr(work->pokepic, POKEPIC_ALPHA);
    Pokepic_SetAttr(work->pokepic, POKEPIC_ALPHA, 8);
    work->left = x - 40;
    work->top = y + 40;
    work->stripWidth = 0;
    work->bottom = work->top;
    work->fromRight = FALSE;
    GX_SetVisibleWnd(GX_WNDMASK_W0);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWnd0Position(work->left, work->top, work->left + 80, work->bottom);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_WipeOutDefender, work);
}

#pragma force_active reset

void BattleAnimTask_PicOrbit(SysTask *task, void *data) {
    BattleAnimPicOrbitWork *work = data;
    BOOL active[3];
    int i;

    active[0] = BattleAnimUtil_UpdateSinMotionPokepic(&work->motion, work->x, work->y, work->pokepic);
    active[1] = BattleAnimUtil_UpdateSpriteSequence(&work->sequence);
    active[2] = FALSE;
    SpriteSystem_DrawSprites(work->spriteManager);
    for (i = 0; i < 3; i++) {
        if (active[i] == TRUE) {
            return;
        }
    }
    Pokepic_SetAttr(work->pokepic, POKEPIC_X, work->x);
    Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y - 8);
    BattleAnimSystem_DestroyTask(work->system, task);
    Heap_Free(work);
}

void BattleAnimFunc_PicOrbit(BattleAnimSystem *system) {
    BattleAnimPicOrbitWork *work;
    BattleAnimSinMotion motion;
    int sign;
    int i;

    work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimPicOrbitWork));
    work->system = system;
    work->spriteManager = BattleAnimSystem_GetBattlerSpriteManager(work->system);
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->shadowHeight = Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H);
    work->y += 8;
    BattleAnimUtil_InitCircleMotion(&work->motion, 1, 8);
    BattleAnimUtil_InitCircleMotion(&motion, 1, 10);
    sign = BattleAnimUtil_GetBattlerSideSignByMode(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->motion.ampX *= sign;
    motion.ampX *= sign;
    for (i = 0; i < 2; i++) {
        work->sprites[i] = BattleAnimSystem_GetBattlerSprite(work->system, i);
        ManagedSprite_SetDrawPriority(work->sprites[i], 100);
        ManagedSprite_SetPriority(work->sprites[i], 1);
        ManagedSprite_SetOamMode(work->sprites[i], GX_OAM_MODE_XLU);
    }
    BattleAnimUtil_InitSpriteSequence(&work->sequence, &motion, BattleAnimUtil_UpdateSinMotion, work->x, (s16)(work->y - work->shadowHeight), 2, 2, FALSE, work->sprites[0], work->sprites[1], NULL, NULL);
    BattleAnimUtil_SetBlendWithBattleBg(work->system, -1, -1);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_PicOrbit, work);
}

void BattleAnimTask_AttackerRecoilTilt(SysTask *task, void *data) {
    BattleAnimAttackerRecoilTiltWork *work = data;

    switch (work->state) {
    case 0:
        if (BattleAnimUtil_UpdateLinearMotion(&work->offset)) {
            BattleAnimUtil_SetPokepicPosWithOffset(&work->offset, work->pokepic, work->x, work->y);
            return;
        }
        BattleAnimUtil_InitLinearMotion(&work->offset, work->offset.x, 0, 0, 0, 2);
        work->state++;
        return;
    case 1:
        if (BattleAnimUtil_UpdateValueMotion(&work->rotation)) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_ZROT, work->rotation.value);
            return;
        }
        BattleAnimUtil_InitValueMotion(&work->rotation, work->rotation.value, 0, 4);
        work->state++;
        work->timer = 2;
        return;
    case 2:
        if (--work->timer < 0) {
            work->state++;
        }
        return;
    case 3:
        if (BattleAnimUtil_UpdateLinearMotion(&work->offset)) {
            BattleAnimUtil_SetPokepicPosWithOffset(&work->offset, work->pokepic, work->x, work->y);
            return;
        }
        work->state++;
        work->timer = 32;
        return;
    case 4:
        if (--work->timer < 0) {
            work->state++;
        }
        return;
    case 5:
        if (BattleAnimUtil_UpdateValueMotion(&work->rotation)) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_ZROT, work->rotation.value);
            return;
        }
        work->state++;
        return;
    case 6:
        Pokepic_SetAttr(work->pokepic, POKEPIC_X, work->x);
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y);
        Pokepic_SetAttr(work->pokepic, POKEPIC_ZROT, 0);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
}

void BattleAnimFunc_AttackerRecoilTilt(BattleAnimSystem *system) {
    BattleAnimAttackerRecoilTiltWork *work;
    int sign;
    int attackerSide;
    int defenderSide;

    work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimAttackerRecoilTiltWork));
    work->system = system;
    work->state = 0;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    BattleAnimUtil_InitLinearMotion(&work->offset, 0, -32, 0, 0, 3);
    BattleAnimUtil_InitValueMotion(&work->rotation, 0, 0xE38, 8);
    sign = BattleAnimUtil_GetBattlerSideSignByMode(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->offset.stepX *= sign;
    work->rotation.step *= sign;
    sign = BattleAnimUtil_GetBattlerSideSignByMode(work->system, BattleAnimSystem_GetDefender(work->system));
    attackerSide = BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetAttacker(work->system));
    defenderSide = BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetDefender(work->system));
    if (attackerSide == BATTLE_ANIM_SIDE_PLAYER && defenderSide == BATTLE_ANIM_SIDE_PLAYER) {
        Pokepic_SetAttr(work->pokepic, POKEPIC_XPIVOT, -40);
        Pokepic_SetAttr(work->pokepic, POKEPIC_YPIVOT, 40);
    } else if (attackerSide == BATTLE_ANIM_SIDE_ENEMY && defenderSide == BATTLE_ANIM_SIDE_ENEMY) {
        Pokepic_SetAttr(work->pokepic, POKEPIC_XPIVOT, 40);
        Pokepic_SetAttr(work->pokepic, POKEPIC_YPIVOT, 40);
    } else {
        Pokepic_SetAttr(work->pokepic, POKEPIC_XPIVOT, 40 * sign);
        Pokepic_SetAttr(work->pokepic, POKEPIC_YPIVOT, 40);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_AttackerRecoilTilt, work);
}

void BattleAnimMotionSprite_Init(BattleAnimMotionSprite *motionSprite, SpriteSystem *spriteSystem, SpriteManager *spriteManager, const ManagedSpriteTemplate *template, const BattleAnimMotionSprite *source) {
    motionSprite->sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, template);
    motionSprite->orbit = source->orbit;
    motionSprite->speed = source->speed;
    motionSprite->baseAngleStep = source->baseAngleStep;
    motionSprite->x = source->x;
    motionSprite->y = source->y;
    motionSprite->state = source->state;
    motionSprite->active = FALSE;
    ManagedSprite_SetDrawFlag(motionSprite->sprite, FALSE);
    ManagedSprite_SetDrawPriority(motionSprite->sprite, 100);
    ManagedSprite_SetPriority(motionSprite->sprite, 1);
}

void BattleAnimMotionSprite_Delete(BattleAnimMotionSprite *motionSprite) {
    Sprite_DeleteAndFreeResources(motionSprite->sprite);
}

void BattleAnimMotionSprite_SetDrawFlag(BattleAnimMotionSprite *motionSprite, BOOL draw) {
    ManagedSprite_SetDrawFlag(motionSprite->sprite, draw);
}

BOOL BattleAnimMotionSprite_Update(BattleAnimMotionSprite *motionSprite) {
    if (!motionSprite->active) {
        return TRUE;
    }
    switch (motionSprite->state) {
    case 0:
    case 1:
        if (BattleAnimUtil_UpdateSinMotion(&motionSprite->orbit)) {
            BattleAnimUtil_UpdateCosMotion(&motionSprite->speed);
            motionSprite->orbit.stepX = motionSprite->baseAngleStep + motionSprite->speed.value;
            ManagedSprite_SetPositionXY(motionSprite->sprite, motionSprite->x + motionSprite->orbit.x, motionSprite->y + motionSprite->orbit.y);
        } else {
            if (motionSprite->state < 1) {
                if (motionSprite->state == 0) {
                    BattleAnimUtil_InitSinMotionFromStepU16(&motionSprite->orbit, motionSprite->orbit.angleX, 0x7FFF, (u16)motionSprite->orbit.angleX, 0x7FFF, motionSprite->orbit.ampX, motionSprite->orbit.ampY, 0x71C);
                }
                BattleAnimUtil_InitCosMotion(&motionSprite->speed, 0, 0xFFFF, 0xFFFFF000, motionSprite->orbit.duration);
            }
            motionSprite->state++;
        }
        break;
    case 2:
        return FALSE;
    }
    return TRUE;
}

void BattleAnimTask_OrbitingSpriteTrail(SysTask *task, void *data) {
    int i;
    BOOL active;
    BattleAnimOrbitingSpriteTrailWork *work = data;

    switch (work->state) {
    case 0:
        BattleAnimMotionSprite_Update(&work->main);
        work->state++;
        break;
    case 1:
        work->blend += 10;
        reg_G2_BLDALPHA = ((31 - work->blend / 10) << 8) | (work->blend / 10);
        if (work->blend >= 310) {
            ManagedSprite_SetOamMode(work->main.sprite, GX_OAM_MODE_NORMAL);
            work->state++;
        }
        break;
    case 2:
        reg_G2_BLDALPHA = 0xFFFF;
        work->state++;
        break;
    case 3:
        if (work->timer <= 0) {
            work->trail[work->index].active = TRUE;
            ManagedSprite_SetDrawFlag(work->trail[work->index].sprite, TRUE);
            work->index++;
            work->timer = 8;
        }
        if (work->index < 2) {
            work->timer--;
        }
        active = BattleAnimMotionSprite_Update(&work->main);
        for (i = 0; i < 2; i++) {
            BOOL trailActive = BattleAnimMotionSprite_Update(&work->trail[i]);
            if (!trailActive) {
                BattleAnimMotionSprite_SetDrawFlag(&work->trail[i], FALSE);
            }
            active |= trailActive;
        }
        if (!active) {
            work->state++;
        }
        break;
    case 4:
        ManagedSprite_TickFrame(work->main.sprite);
        if (!Sprite_IsAnimated(work->main.sprite->sprite)) {
            ManagedSprite_SetOamMode(work->main.sprite, GX_OAM_MODE_XLU);
            work->state++;
        }
        break;
    case 5:
        work->blend -= 10;
        reg_G2_BLDALPHA = ((31 - work->blend / 10) << 8) | (work->blend / 10);
        if (work->blend <= 0) {
            work->state++;
        }
        break;
    case 6:
        for (i = 0; i < 2; i++) {
            BattleAnimMotionSprite_Delete(&work->trail[i]);
        }
        Sprite_DeleteAndFreeResources(work->main.sprite);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimSpriteFunc_OrbitingSpriteTrail(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    int i;
    int attacker;
    int sign;
    ManagedSpriteTemplate template;
    BattleAnimOrbitingSpriteTrailWork *work;

    work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimOrbitingSpriteTrailWork));
    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->timer = 8;
    work->index = 0;
    work->state = 0;
    attacker = BattleAnimSystem_GetAttacker(system);
    sign = BattleAnimUtil_GetBattlerSideSignByMode(system, attacker);
    work->main.sprite = sprite;
    work->main.state = 0;
    BattleAnimUtil_InitSinMotionFromStep(&work->main.orbit, 0x3FFF, 0xBFFF, 0x3FFF, 0xBFFF, -(sign << 17), -FX32_CONST(8), 0x71C);
    BattleAnimUtil_InitCosMotion(&work->main.speed, 0, 0xFFFF, 0xFFFFF000, work->main.orbit.duration);
    work->main.baseAngleStep = work->main.orbit.stepX;
    work->main.active = TRUE;
    work->main.x = BattleAnimSystem_GetBattlerScreenCoord(system, attacker, 0);
    work->main.y = BattleAnimSystem_GetBattlerScreenCoord(system, attacker, 1);
    ManagedSprite_SetPositionXY(work->main.sprite, work->main.x, work->main.y);
    ManagedSprite_SetDrawPriority(work->main.sprite, 100);
    ManagedSprite_SetPriority(work->main.sprite, 1);
    BattleAnimSystem_GetSpriteTemplate(&template, work->system);
    template.x = work->main.x;
    template.y = work->main.y;
    for (i = 0; i < 2; i++) {
        BattleAnimMotionSprite_Init(&work->trail[i], work->spriteSystem, work->spriteManager, &template, &work->main);
    }
    work->blend = 0;
    BattleAnimUtil_SetBlendWithBattleBg(work->system, 0, 31);
    ManagedSprite_SetOamMode(work->main.sprite, GX_OAM_MODE_XLU);
    for (i = 0; i < 2; i++) {
        ManagedSprite_SetOamMode(work->trail[i].sprite, GX_OAM_MODE_XLU);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_OrbitingSpriteTrail, work);
}

void BattleAnimTask_ShakeDefenderWithOverlayApply(BattleAnimShakeDefenderWithOverlayWork *work) {
    f32 scaleX;
    f32 scaleY;

    Pokepic_SetAttr(work->pokepic, POKEPIC_X, work->x + work->shake.x);
    Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y + work->shake.y);
    ManagedSprite_SetPositionXY(work->sprite, work->x + work->shake.x, work->y + work->shake.y - work->shadowHeight);
    BattleAnimUtil_GetScaleFloat(&work->scale, &scaleX, &scaleY);
    scaleX += (f32)work->jitter * 0.1f;
    ManagedSprite_SetAffineScale(work->sprite, scaleX, scaleY);
    work->jitter *= -1;
}

void BattleAnimTask_ShakeDefenderWithOverlay(SysTask *task, void *data) {
    BattleAnimShakeDefenderWithOverlayWork *work = data;

    switch (work->state) {
    case 0: {
        BOOL active;
        BattleAnimUtil_UpdateShake(&work->shake);
        active = BattleAnimUtil_UpdateScaleMotionXY(&work->scale);
        BattleAnimTask_ShakeDefenderWithOverlayApply(work);
        if (!active) {
            BattleAnimUtil_InitScaleMotionXY(&work->scale, 12, 10, 15, 10, 10, 4);
            work->state++;
        }
        break;
    }
    case 1: {
        BOOL active;
        BattleAnimUtil_UpdateShake(&work->shake);
        active = BattleAnimUtil_UpdateScaleMotionXY(&work->scale);
        BattleAnimTask_ShakeDefenderWithOverlayApply(work);
        if (!active) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_X, work->x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y);
            ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_NORMAL);
            ManagedSprite_SetPositionXY(work->sprite, work->x, work->y - work->shadowHeight);
            ManagedSprite_SetAffineScale(work->sprite, 1.0f, 1.0f);
            work->state++;
        }
        break;
    }
    case 2:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimFunc_ShakeDefenderWithOverlay(BattleAnimSystem *system) {
    BattleAnimShakeDefenderWithOverlayWork *work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimShakeDefenderWithOverlayWork));
    int defender;

    work->system = system;
    work->spriteManager = BattleAnimSystem_GetBattlerSpriteManager(system);
    work->state = 0;
    work->unk_0C = 0;
    defender = BattleAnimSystem_GetDefender(work->system);
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, defender);
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->shadowHeight = Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H);
    BattleAnimUtil_InitShake(&work->shake, 2, 0, 1, 6);
    work->sprite = BattleAnimSystem_GetBattlerSprite(work->system, 0);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE);
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    BattleAnimUtil_InitScaleMotionXY(&work->scale, 10, 12, 10, 15, 10, 7);
    work->jitter = 1;
    BattleAnimUtil_SetBlendWithBattleBg(work->system, 28, 15);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_ShakeDefenderWithOverlay, work);
}
