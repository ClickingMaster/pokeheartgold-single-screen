#include "battle/battle_anim_funcs.h"

#include "global.h"

#include "constants/battle.h"

#include "library/spl_emitter.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "obj_pltt_transfer.h"
#include "palette.h"
#include "particle_system_internal.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"

// Battler roles used by the effect arguments
#define BATTLE_ANIM_ROLE_ATTACKER         2
#define BATTLE_ANIM_ROLE_ATTACKER_PARTNER 4
#define BATTLE_ANIM_ROLE_DEFENDER         8
#define BATTLE_ANIM_ROLE_DEFENDER_PARTNER 16

// BattleAnimUtil_CollectPicsByFlags target flags
#define BATTLE_ANIM_TARGET_PICS    0x100
#define BATTLE_ANIM_TARGET_SPRITES 0x200
#define BATTLE_ANIM_TARGET_BG      0x400

// Emitter position units per screen pixel
#define BATTLE_ANIM_EMITTER_PIXEL 172

// Starts the effect task and runs it once right away: the task argument is what BattleAnimSystem_CreateTaskDefaultPriority (declared void)
// leaves in r0, the new task
typedef SysTask *(*BattleAnimCreateTaskReturningFunc)(BattleAnimSystem *system, SysTaskFunc func, void *data);
#define BattleAnim_CreateTaskAndRun(system, func, work) func(((BattleAnimCreateTaskReturningFunc)BattleAnimSystem_CreateTaskDefaultPriority)(system, func, work), work)

typedef struct BattleAnimOneFrameWork {
    u8 state;
    BattleAnimSystem *system;
} BattleAnimOneFrameWork;

typedef struct BattleAnimSpriteCopiesWork {
    u8 state;
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprite;
    ManagedSprite *sprites[3]; // [0]: the original sprite
} BattleAnimSpriteCopiesWork;

typedef struct BattleAnimShakeTargetsWork {
    int targetCount;
    BattleAnimCommonWork common; // 0x04
    s16 ampX;
    s16 ampY;
    s16 interval;
    s16 count;
    u32 flags;
    BattleAnimTarget targets[4]; // 0x2C
    BattleAnimShake shake;       // 0x7C
} BattleAnimShakeTargetsWork;    // size: 0xA0

typedef struct BattleAnimSlidingBgWork {
    BOOL mirror;
    BOOL offscreen;
    u16 eva;
    s16 x;
    s16 y;
    s16 dx;
    s16 dy;
    u16 slowdownInterval;
    u16 slowdownTimer;
    u16 curEva;
    u16 curEvb;
    BattleAnimCommonWork common; // 0x1C
    u8 unk_38[0x80];
} BattleAnimSlidingBgWork; // size: 0xB8

typedef struct BattleAnimRotateBattlerWork {
    u8 state;
    BattleAnimSystem *system;
    Pokepic *pokepic;
    BattleAnimValueMotion rotation; // 0x0C
    int startAngle;
    int duration;
    s16 pivotX;
    s16 pivotY;
} BattleAnimRotateBattlerWork; // size: 0x2C

typedef struct BattleAnimCircleBattlerWork {
    BattleAnimSystem *system;
    BattleAnimSinMotion motion; // 0x04
    Pokepic *pokepic;
    s16 x;
    s16 y;
} BattleAnimCircleBattlerWork; // size: 0x30

// A sprite that pops up (scales up then down) beside the defender, twice
typedef struct BattleAnimSpritePopWork {
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int state;
    u8 unk_10[4];
    ManagedSprite *sprite;
    BattleAnimLinearMotion scale; // 0x18
    int side;
    int shrinking;
    int delay;
    int sequenceState;
    s16 x;
    s16 y;
} BattleAnimSpritePopWork; // size: 0x50

typedef struct BattleAnimPaletteFadeWork {
    BattleAnimSystem *system;
    PaletteData *paletteData;
} BattleAnimPaletteFadeWork;

typedef struct BattleAnimBattlerPaletteFadeWork {
    BattleAnimSystem *system;
    Pokepic *pokepic;
    int state;
    int count;
    int framesPer;
    int color;
    int strength;
    int delay;
} BattleAnimBattlerPaletteFadeWork; // size: 0x20

typedef struct BattleAnimPulseBattlerSpriteWork {
    BattleAnimSystem *system;
    SpriteManager *spriteManager;
    int state;
    s16 y;
    s16 height;
    ManagedSprite *sprite;
    BattleAnimLinearMotion scale; // 0x14
    int eva;
    int start;
    int end;
    int divisor;
    int count;
    int durations; // high half: growing, low half: shrinking
    BOOL flipX;
} BattleAnimPulseBattlerSpriteWork; // size: 0x54

typedef struct BattleAnimStretchBattlerWork {
    BattleAnimSystem *system;
    int state;
    s16 y;
    s16 height;
    Pokepic *pokepic;
    BattleAnimLinearMotion scale; // 0x10
    int startX;
    int endX;
    int startY;
    int endY;
    int divisor;
    int count;
    int durations; // high half: stretching, low half: back
    int holdFrames;
} BattleAnimStretchBattlerWork; // size: 0x54

typedef struct BattleAnimBlendBattlerSpritesWork {
    BattleAnimSystem *system;
    SpriteManager *spriteManager;
    BattleAnimBlendAlphaMotion blend;
} BattleAnimBlendBattlerSpritesWork; // size: 0x30

typedef struct BattleAnimBlinkBattlerWork {
    int count;
    int timer;
    int interval;
    u8 unk_0C[8];
    Pokepic *pokepic;
    u8 unk_18[8];
    BattleAnimCommonWork common; // 0x20
    BattleAnimSystem *system;
} BattleAnimBlinkBattlerWork; // size: 0x40

typedef struct BattleAnimSlideBattlerWork {
    int unk_00;
    u8 unk_04[4];
    int duration;
    s16 dx;
    u8 unk_0E[2];
    BattleAnimTarget target;       // 0x10
    BattleAnimCommonWork common;   // 0x24
    BattleAnimLinearMotion motion; // 0x40
    BattleAnimSystem *system;
} BattleAnimSlideBattlerWork; // size: 0x68

typedef struct BattleAnimMoveBattlerWork {
    int unk_00;
    u8 unk_04[4];
    int duration;
    BattleAnimTarget target;       // 0x0C
    BattleAnimCommonWork common;   // 0x20
    BattleAnimLinearMotion motion; // 0x3C
    BattleAnimSystem *system;
    s16 dx;
    s16 dy;
} BattleAnimMoveBattlerWork; // size: 0x68

typedef struct BattleAnimShakeAndScaleWork {
    u8 state;
    u8 unk_01;
    s16 baseY;
    int height;
    u8 unk_08;
    u8 timer;
    u8 delay;
    u8 unk_0B;
    BattleAnimSystem *system;
    Pokepic *pokepic;
    BattleAnimLinearMotion scale; // 0x14
    BattleAnimShake shake;        // 0x38
} BattleAnimShakeAndScaleWork;    // size: 0x5C

typedef struct BattleAnimShakeAndScaleXYWork {
    u8 state;
    u8 unk_01;
    s16 baseY;
    int height;
    u8 unk_08;
    u8 timer;
    u8 delay;
    u8 unk_0B;
    int scaleX; // high half: start, low byte: end
    int scaleY;
    int duration;
    BattleAnimSystem *system;
    Pokepic *pokepic;
    BattleAnimLinearMotion scale; // 0x20
    BattleAnimShake shake;        // 0x44
} BattleAnimShakeAndScaleXYWork;  // size: 0x68

typedef struct BattleAnimObjWindowWork {
    u32 unk_00;
    BattleAnimCommonWork common; // 0x04
    u32 unk_20;
    ManagedSprite *sprite;
} BattleAnimObjWindowWork; // size: 0x28

typedef struct BattleAnimMoveBattlerSinWork {
    BattleAnimCommonWork common;
    BattleAnimTarget target;    // 0x1C
    BattleAnimSinMotion motion; // 0x30
} BattleAnimMoveBattlerSinWork; // size: 0x54

typedef struct BattleAnimMoveBattlerLinearWork {
    BattleAnimCommonWork common;
    BattleAnimTarget target;       // 0x1C
    BattleAnimLinearMotion motion; // 0x30
    BattleAnimPosition home;
    s16 offscreenX;
    s16 unk_5A;
} BattleAnimMoveBattlerLinearWork; // size: 0x5C

typedef struct BattleAnimBlendSpritePaletteWork {
    BattleAnimCommonWork common;
    BattleAnimTarget target; // 0x1C
    BattleAnimPaletteBlend *blend;
} BattleAnimBlendSpritePaletteWork; // size: 0x34

typedef struct BattleAnimBattlerHeightWork {
    BattleAnimCommonWork common;
    BattleAnimTarget target; // 0x1C
    int visibleX;
    int visibleY;
    int visibleWidth;
    int visibleHeight;
    s16 unk_40;
    s16 step;
    int interval;
    int growing;
    int timer;
    int y;
    int height;
} BattleAnimBattlerHeightWork; // size: 0x58

typedef struct BattleAnimRevealBattlerWork {
    BattleAnimCommonWork common;
    s16 step;
    int interval;
    int timer;
    BattleAnimTarget target; // 0x28
    int height;
    int maxHeight;
    int y;
    int baseY;
    u8 unk_4C[4];
} BattleAnimRevealBattlerWork; // size: 0x50

typedef struct BattleAnimShakeBgWork {
    u8 state;
    u8 count;
    int ampX;
    int ampY;
    int interval;
    int shakeCount;
    u8 unk_14[4];
    int repeatCount;
    int bgLayer;
    BOOL movedX;
    BOOL movedY;
    BgConfig *bgConfig;
    BattleAnimSystem *system;
    SpriteManager *spriteManager;
    u8 unk_34[0xA0];
    BattleAnimShake shake; // 0xD4
} BattleAnimShakeBgWork;   // size: 0xF8

typedef struct BattleAnimSpriteMosaicWork {
    BattleAnimCommonWork common;
    ManagedSprite *sprite;
    s16 step;
    int sizeH;
    int sizeV;
    int target;
} BattleAnimSpriteMosaicWork; // size: 0x30

// An emitter moved between two battlers (linear, optionally on an arc or with a vertical sine wave)
typedef struct BattleAnimEmitterMotionWork {
    int emitterIndex;
    s16 delay;
    u16 duration;
    int amplitude;
    s16 offsetX;
    s16 offsetY;
    int direction;
    int fromBattler;
    int toBattler;
    int frame;
    int startFrame;
    int endFrame;
    int counter;
    int angle;
    BOOL wave;
    ParticleSystem *particleSystem; // 0x34
    SPLEmitter *emitter;
    BattleAnimCommonWork common; // 0x3C
    u8 unk_58[0x50];
    BattleAnimLinearMotion linear; // 0xA8
    BattleAnimSinMotion sin;       // 0xCC
    BattleAnimValueMotion value;   // 0xF0
} BattleAnimEmitterMotionWork;     // size: 0x104

typedef struct BattleAnimEmitterSinWork {
    int emitterIndex;
    int startAngleX;
    int startAngleY;
    int endAngleX;
    int endAngleY;
    int ampX;
    int ampY;
    int duration;
    int useDefender;
    int battler;
    u8 unk_28[0xC];
    ParticleSystem *particleSystem; // 0x34
    SPLEmitter *emitter;
    BattleAnimCommonWork common; // 0x3C
    BattleAnimSinMotion motion;  // 0x58
    u8 unk_7C[0xC];
    VecFx32 base;           // 0x88
} BattleAnimEmitterSinWork; // size: 0x94

typedef struct BattleAnimSpriteWork {
    BattleAnimCommonWork common;
    ManagedSprite *sprite; // 0x1C
} BattleAnimSpriteWork;    // size: 0x20

typedef struct BattleAnimSpriteOrbitWork {
    BattleAnimCommonWork common;
    ManagedSprite *sprite;       // 0x1C
    BattleAnimValueMotion angle; // 0x20
    int step;
} BattleAnimSpriteOrbitWork; // size: 0x38

typedef struct BattleAnimSpriteArcWork {
    BattleAnimCommonWork common;
    ManagedSprite *sprite; // 0x1C
    u8 eva;
    u8 evb;
    u8 unk_22[2];
    BattleAnimLinearMotion linear; // 0x24
    BattleAnimSinMotion sin;       // 0x48
} BattleAnimSpriteArcWork;         // size: 0x6C

// X steps of the sprite of BattleAnimTask_SpriteOrbit (0: switch its priority, 255: end)
extern const s16 sBattleAnimSpriteOrbitStepX[];

void BattleAnimTask_OneFrame(SysTask *task, void *data) {
    BattleAnimOneFrameWork *work = data;
    switch (work->state) {
    case 0:
        work->state++;
        break;
    case 1:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

void BattleAnimFunc_OneFrameTask(BattleAnimSystem *system) {
    BattleAnimOneFrameWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleAnimOneFrameWork));
    work->state = 0;
    work->system = system;
    BattleAnimSystem_CreateTask(system, BattleAnimTask_OneFrame, work, 0x1000);
}

void BattleAnimTask_OneFrameSound(SysTask *task, void *data) {
    BattleAnimOneFrameWork *work = data;
    switch (work->state) {
    case 0:
        work->state++;
        break;
    case 1:
        BattleAnimSystem_DestroySoundTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

void BattleAnimFunc_OneFrameSoundTask(BattleAnimSystem *system) {
    BattleAnimOneFrameWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleAnimOneFrameWork));
    work->state = 0;
    work->system = system;
    BattleAnimSystem_CreateSoundTask(system, BattleAnimTask_OneFrameSound, work, 0x1000);
}

void BattleAnimTask_OneFrameUncounted(SysTask *task, void *data) {
    BattleAnimOneFrameWork *work = data;
    switch (work->state) {
    case 0:
        work->state++;
        break;
    case 1:
        SysTask_Destroy(task);
        Heap_Free(work);
        break;
    }
}

void BattleAnimFunc_OneFrameUncountedTask(BattleAnimSystem *system) {
    BattleAnimOneFrameWork *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleAnimOneFrameWork));
    work->state = 0;
    work->system = system;
    SysTask_CreateOnMainQueue(BattleAnimTask_OneFrameUncounted, work, 0x1000);
}

#pragma force_active on

void ov07_0222314C(SysTask *task, void *data) {
    BattleAnimSpriteCopiesWork *work = data;
    switch (work->state) {
    case 0:
        Sprite_TickFrame(work->sprites[0]->sprite);
        Sprite_TickFrame(work->sprites[1]->sprite);
        Sprite_TickFrame(work->sprites[2]->sprite);
        Sprite_OffsetPositionXY(work->sprites[0]->sprite, 1, 0);
        Sprite_OffsetPositionXY(work->sprites[1]->sprite, -1, 0);
        Sprite_OffsetPositionXY(work->sprites[2]->sprite, 1, 1);
        SpriteSystem_DrawSprites(work->spriteManager);
        break;
    case 1: {
        BattleAnimSystem *system = work->system;
        Sprite_DeleteAndFreeResources(work->sprite);
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(system, task);
        break;
    }
    }
}

void ov07_022231BC(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimSpriteCopiesWork *work = Heap_Alloc(HEAP_ID_BATTLE, 0x1CC);
    ManagedSpriteTemplate template;
    int i;
    GF_ASSERT(work != NULL);
    work->state = 0;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->sprite = sprite;
    work->system = system;
    BattleAnimSystem_GetSpriteTemplate(&template, system);
    work->sprites[0] = sprite;
    for (i = 1; i < 3; i++) {
        work->sprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
    }
    BattleAnimSystem_CreateTask(system, ov07_0222314C, work, 0x1000);
}

#pragma force_active reset

void BattleAnimUtil_ResetPokepicScale(Pokepic *pokepic) {
    Pokepic_SetAttr(pokepic, POKEPIC_AFFINEW, 0x100);
    Pokepic_SetAttr(pokepic, POKEPIC_AFFINEH, 0x100);
}

void BattleAnimTask_ShakeTargets(SysTask *task, void *data) {
    BattleAnimShakeTargetsWork *work = data;
    int i;
    switch (work->common.state[0]) {
    case 0:
        BattleAnimUtil_InitShake(&work->shake, work->ampX, work->ampY, work->interval, work->count);
        work->common.state[0]++;
        return;
    case 1:
        if (BattleAnimUtil_UpdateShake(&work->shake) == FALSE) {
            work->common.state[0]++;
            return;
        }
        if ((work->flags & BATTLE_ANIM_TARGET_PICS) == BATTLE_ANIM_TARGET_PICS) {
            for (i = 0; i < work->targetCount; i++) {
                if (work->targets[i].pokepic != NULL) {
                    Pokepic_SetAttr(work->targets[i].pokepic, POKEPIC_X, work->shake.x + work->targets[i].pos[0]);
                    Pokepic_SetAttr(work->targets[i].pokepic, POKEPIC_Y, work->shake.y + work->targets[i].pos[1]);
                }
            }
        } else if ((work->flags & BATTLE_ANIM_TARGET_SPRITES) == BATTLE_ANIM_TARGET_SPRITES) {
            for (i = 0; i < work->targetCount; i++) {
                if (work->targets[i].sprite != NULL) {
                    ManagedSprite_SetPositionXY(work->targets[i].sprite, work->shake.x + work->targets[i].pos[0], work->shake.y + work->targets[i].pos[1]);
                    ManagedSprite_TickFrame(work->targets[i].sprite);
                }
            }
            SpriteSystem_DrawSprites(work->common.battlerSpriteManager);
        }
        if ((work->flags & BATTLE_ANIM_TARGET_BG) == BATTLE_ANIM_TARGET_BG) {
            BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, work->shake.x);
        }
        return;
    default:
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        return;
    }
}

void BattleAnimFunc_ShakeTargets(BattleAnimSystem *system) {
    BattleAnimShakeTargetsWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimShakeTargetsWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->ampX = BattleAnimSystem_GetArg(system, 0);
    work->ampY = BattleAnimSystem_GetArg(system, 1);
    work->interval = BattleAnimSystem_GetArg(system, 2);
    work->count = BattleAnimSystem_GetArg(system, 3);
    work->flags = BattleAnimSystem_GetArg(system, 4);
    if ((work->flags & BATTLE_ANIM_TARGET_PICS) == BATTLE_ANIM_TARGET_PICS) {
        BattleAnimUtil_CollectPicsByFlags(system, work->flags, work->targets, &work->targetCount);
    } else if ((work->flags & BATTLE_ANIM_TARGET_SPRITES) == BATTLE_ANIM_TARGET_SPRITES) {
        BattleAnimUtil_CollectSpritesByFlags(system, work->flags, work->targets, &work->targetCount);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_ShakeTargets, work);
}

// Moves BG2 by (dx, dy) each frame (the speeds decay towards 0), blending it in; blends it out once it has left
// the screen vertically
void BattleAnimTask_SlidingBg(SysTask *task, void *data) {
    BattleAnimSlidingBgWork *work = data;
    int done;
    switch (work->common.state[0]) {
    case 0:
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, work->curEva, work->curEvb);
        ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_ON);
        work->common.state[0]++;
    case 1:
        done = 0;
        if (work->curEva < work->eva - 2) {
            work->curEva += 2;
        } else {
            done++;
        }
        if (work->curEvb > 9) {
            work->curEvb -= 2;
        } else {
            done++;
        }
        if (done == 2) {
            work->curEva = work->eva;
            work->curEvb = 7;
            work->common.state[0]++;
        }
        reg_G2_BLDALPHA = work->curEva | (work->curEvb << 8);
        break;
    case 2:
        if (work->offscreen) {
            work->common.state[0]++;
        }
        break;
    case 3:
        done = 0;
        if (work->curEva > 2) {
            work->curEva -= 2;
        } else {
            done++;
        }
        if (work->curEvb < 29) {
            work->curEvb += 2;
        } else {
            done++;
        }
        if (done == 2) {
            work->curEva = 0;
            work->curEvb = 31;
            work->common.state[0]++;
        }
        reg_G2_BLDALPHA = work->curEva | (work->curEvb << 8);
        break;
    default:
        ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_OFF);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        return;
    }

    if ((work->dy > 0 && work->y >= 512) || (work->dy < 0 && work->y <= -412)) {
        work->offscreen = TRUE;
    }
    work->x += work->dx;
    work->y += work->dy;
    if (work->slowdownInterval < work->slowdownTimer) {
        if (work->dy < 0) {
            work->dy++;
        } else {
            work->dy = 0;
        }
        if (work->dx < 0) {
            work->dx++;
        } else {
            work->dx = 0;
        }
        work->slowdownTimer = 0;
    } else {
        work->slowdownTimer++;
    }
    BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, work->x);
    BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, work->y);
}

void BattleAnimFunc_SlidingBg(BattleAnimSystem *system) {
    int bgId;
    int file;
    BattleAnimSlidingBgWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSlidingBgWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->x = BattleAnimSystem_GetArg(system, 1);
    work->y = BattleAnimSystem_GetArg(system, 2);
    work->dx = BattleAnimSystem_GetArg(system, 3);
    work->dy = BattleAnimSystem_GetArg(system, 4);
    work->mirror = BattleAnimSystem_GetArg(system, 5);
    work->eva = BattleAnimSystem_GetArg(system, 6);
    work->slowdownInterval = BattleAnimSystem_GetArg(system, 7);
    work->slowdownTimer = 0;
    if (work->mirror && BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetAttacker(system)) == BATTLE_ANIM_SIDE_ENEMY) {
        work->x *= -1;
        work->y *= -1;
        work->dx *= -1;
        work->dy *= -1;
        work->y -= 84;
    } else {
        work->y += 84;
    }
    work->curEva = 0;
    work->curEvb = 31;
    work->offscreen = FALSE;
    ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_OFF);
    bgId = BattleAnimSystem_GetArg(system, 0);
    file = BattleAnimSystem_GetBackgroundFile(bgId, 0);
    GfGfxLoader_LoadCharData(NARC_a_0_0_7, file, work->common.bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, BattleAnimSystem_GetHeapID(system));
    file = BattleAnimSystem_GetBackgroundFile(bgId, 1);
    PaletteData_LoadNarc(work->common.paletteData, NARC_a_0_0_7, file, BattleAnimSystem_GetHeapID(system), PLTTBUF_MAIN_BG, 0x20, 0x90);
    BgClearTilemapBufferAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2);
    file = 2;
    if (ov07_0221BFC0(system) == 1) {
        file = 4;
    } else if (BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetAttacker(system)) == BATTLE_ANIM_SIDE_ENEMY) {
        file = 3;
    }
    file = BattleAnimSystem_GetBackgroundFile(bgId, file);
    GfGfxLoader_LoadScrnData(NARC_a_0_0_7, file, work->common.bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, BattleAnimSystem_GetHeapID(system));
    BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, work->x);
    BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, work->y);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_SlidingBg, work);
}

// As BattleAnimTask_SlidingBg with fixed blend levels
void BattleAnimTask_SlidingBg2(SysTask *task, void *data) {
    BattleAnimSlidingBgWork *work = data;
    int done;
    switch (work->common.state[0]) {
    case 0:
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, work->curEva, work->curEvb);
        ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_ON);
        work->common.state[0]++;
    case 1:
        done = 0;
        if (work->curEva < 16) {
            work->curEva += 2;
        } else {
            done++;
        }
        if (work->curEvb > 4) {
            work->curEvb -= 2;
        } else {
            done++;
        }
        if (done == 2) {
            work->curEva = 16;
            work->curEvb = 4;
            work->common.state[0]++;
        }
        reg_G2_BLDALPHA = work->curEva | (work->curEvb << 8);
        break;
    case 2:
        if (work->offscreen) {
            work->common.state[0]++;
        }
        break;
    case 3:
        done = 0;
        if (work->curEva > 2) {
            work->curEva -= 2;
        } else {
            done++;
        }
        if (work->curEvb < 16) {
            work->curEvb += 2;
        } else {
            done++;
        }
        if (done == 2) {
            work->curEva = 0;
            work->curEvb = 31;
            work->common.state[0]++;
        }
        reg_G2_BLDALPHA = work->curEva | (work->curEvb << 8);
        break;
    default:
        ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_OFF);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        return;
    }

    if ((work->dy > 0 && work->y >= 512) || (work->dy < 0 && work->y <= -412)) {
        work->offscreen = TRUE;
    }
    work->x += work->dx;
    work->y += work->dy;
    if (work->slowdownInterval < work->slowdownTimer) {
        if (work->dy < 0) {
            work->dy++;
        } else {
            work->dy = 0;
        }
        if (work->dx < 0) {
            work->dx++;
        } else {
            work->dx = 0;
        }
        work->slowdownTimer = 0;
    } else {
        work->slowdownTimer++;
    }
    BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, work->x);
    BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, work->y);
}

void BattleAnimFunc_SlidingBg2(BattleAnimSystem *system) {
    int bgId;
    int file;
    BattleAnimSlidingBgWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSlidingBgWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->x = BattleAnimSystem_GetArg(system, 1);
    work->y = BattleAnimSystem_GetArg(system, 2);
    work->dx = BattleAnimSystem_GetArg(system, 3);
    work->dy = BattleAnimSystem_GetArg(system, 4);
    work->mirror = BattleAnimSystem_GetArg(system, 5);
    work->eva = BattleAnimSystem_GetArg(system, 6);
    work->slowdownInterval = BattleAnimSystem_GetArg(system, 7);
    work->slowdownTimer = 0;
    if (work->mirror && BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetAttacker(system)) == BATTLE_ANIM_SIDE_ENEMY) {
        work->x *= -1;
        work->y *= -1;
        work->dx *= -1;
        work->dy *= -1;
        work->y -= 84;
    } else {
        work->y += 84;
    }
    if (ov07_0221BFC0(system) == 1) {
        work->dx *= -1;
    }
    work->curEva = 4;
    work->curEvb = 16;
    work->curEva = 0;
    work->curEvb = 16;
    work->offscreen = FALSE;
    ToggleBgLayer(GF_BG_LYR_MAIN_2, GF_PLANE_TOGGLE_OFF);
    bgId = BattleAnimSystem_GetArg(system, 0);
    file = BattleAnimSystem_GetBackgroundFile(bgId, 0);
    GfGfxLoader_LoadCharData(NARC_a_0_0_7, file, work->common.bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, BattleAnimSystem_GetHeapID(system));
    file = BattleAnimSystem_GetBackgroundFile(bgId, 1);
    PaletteData_LoadNarc(work->common.paletteData, NARC_a_0_0_7, file, BattleAnimSystem_GetHeapID(system), PLTTBUF_MAIN_BG, 0x20, 0x90);
    BgClearTilemapBufferAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2);
    file = 2;
    if (ov07_0221BFC0(system) == 1) {
        file = 4;
    } else if (BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetAttacker(system)) == BATTLE_ANIM_SIDE_ENEMY) {
        file = 3;
    }
    file = BattleAnimSystem_GetBackgroundFile(bgId, file);
    GfGfxLoader_LoadScrnData(NARC_a_0_0_7, file, work->common.bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, BattleAnimSystem_GetHeapID(system));
    BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, work->x);
    BgSetPosTextAndCommit(work->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, work->y);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_SlidingBg2, work);
}

// Rotates a battler to an angle and back
void BattleAnimTask_RotateBattler(SysTask *task, void *data) {
    BattleAnimRotateBattlerWork *work = data;
    switch (work->state) {
    case 0:
    case 1:
        if (BattleAnimUtil_UpdateValueMotion(&work->rotation) == TRUE) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_ZROT, (u16)work->rotation.value);
            return;
        }
        BattleAnimUtil_InitValueMotion(&work->rotation, work->rotation.value, work->startAngle, work->duration);
        work->state++;
        return;
    case 2:
        Pokepic_SetAttr(work->pokepic, POKEPIC_XPIVOT, 0);
        Pokepic_SetAttr(work->pokepic, POKEPIC_YPIVOT, 0);
        Pokepic_SetAttr(work->pokepic, POKEPIC_ZROT, 0);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
}

#pragma force_active on

void BattleAnimFunc_RotateBattler(BattleAnimSystem *system) {
    BattleAnimRotateBattlerWork *work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimRotateBattlerWork));
    int mode;
    int battler;
    int sign;
    int pivotY;
    work->state = 0;
    work->system = system;
    work->duration = BattleAnimSystem_GetArg(system, 2);
    work->startAngle = BattleAnimSystem_GetArg(system, 0);
    mode = BattleAnimSystem_GetArg(system, 3);
    if (mode == 0 || mode == 2) {
        battler = BattleAnimSystem_GetDefender(work->system);
    } else {
        battler = BattleAnimSystem_GetAttacker(work->system);
    }
    BattleAnimUtil_InitValueMotion(&work->rotation, work->startAngle, BattleAnimSystem_GetArg(system, 1), work->duration);
    sign = ov07_02222004(system, battler);
    work->rotation.step *= sign;
    if (mode == 2) {
        sign *= -1;
    }
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, battler);
    if (BattleAnimSystem_GetArg(system, 3) == 1) {
        work->pivotX = BattleAnimSystem_GetArg(system, 4);
        pivotY = BattleAnimSystem_GetArg(system, 5);
    } else {
        pivotY = 40;
        work->pivotX = pivotY * sign;
    }
    work->pivotY = pivotY;
    Pokepic_SetAttr(work->pokepic, POKEPIC_XPIVOT, work->pivotX);
    Pokepic_SetAttr(work->pokepic, POKEPIC_YPIVOT, work->pivotY);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_RotateBattler, work);
}

#pragma force_active reset

// Moves a battler around an ellipse
void BattleAnimTask_CircleBattler(SysTask *task, void *data) {
    BattleAnimCircleBattlerWork *work = data;
    if (BattleAnimUtil_UpdateSinMotionPokepic(&work->motion, work->x, work->y, work->pokepic) == FALSE) {
        Pokepic_SetAttr(work->pokepic, POKEPIC_X, work->x);
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->y - 8);
        Pokepic_SetAttr(work->pokepic, POKEPIC_ZROT, 0);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
    }
}

void BattleAnimFunc_CircleBattler(BattleAnimSystem *system) {
    int role;
    int battler;
    BattleAnimCircleBattlerWork *work;
    work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimCircleBattlerWork));
    work->system = system;
    ov07_02222A44(&work->motion, BattleAnimSystem_GetArg(system, 0), BattleAnimSystem_GetArg(system, 1));
    role = BattleAnimSystem_GetArg(system, 2);
    switch (role) {
    case BATTLE_ANIM_ROLE_ATTACKER:
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
        work->motion.ampX *= -1;
        break;
    case BATTLE_ANIM_ROLE_ATTACKER_PARTNER:
        battler = BattleAnimUtil_GetPartnerBattler(work->system, BattleAnimSystem_GetAttacker(work->system));
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, battler);
        work->motion.ampX *= -1;
        break;
    case BATTLE_ANIM_ROLE_DEFENDER:
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(system));
        break;
    case BATTLE_ANIM_ROLE_DEFENDER_PARTNER:
        battler = BattleAnimUtil_GetPartnerBattler(work->system, BattleAnimSystem_GetDefender(work->system));
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, battler);
        break;
    default:
        GF_AssertFail();
        break;
    }
    work->x = Pokepic_GetAttr(work->pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->y += 8;
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_CircleBattler, work);
}

void BattleAnimSpritePop_Start(void *data, s16 x, s16 y) {
    BattleAnimSpritePopWork *work = data;
    ManagedSprite_SetDrawFlag(work->sprite, TRUE);
    ManagedSprite_SetPositionXY(work->sprite, x, y);
    work->shrinking = FALSE;
    BattleAnimUtil_InitScaleMotion(&work->scale, 10, 10, 14, 4);
}

BOOL BattleAnimSpritePop_Update(void *data) {
    BattleAnimSpritePopWork *work = data;
    if (BattleAnimUtil_UpdateScaleMotionSprite(&work->scale, work->sprite) == FALSE) {
        if (work->shrinking) {
            ManagedSprite_SetDrawFlag(work->sprite, FALSE);
            return TRUE;
        }
        work->shrinking = TRUE;
        BattleAnimUtil_InitScaleMotion(&work->scale, 14, 10, 12, 2);
    }
    return FALSE;
}

BOOL BattleAnimSpritePop_RunSequence(void *data) {
    BattleAnimSpritePopWork *work = data;
    BOOL done = FALSE;
    switch (work->sequenceState) {
    case 0:
        BattleAnimSpritePop_Start(work, work->x + work->side * 24, work->y - 16);
        work->sequenceState++;
        break;
    case 1:
        if (BattleAnimSpritePop_Update(work)) {
            work->sequenceState++;
        }
        break;
    case 2:
        if (--work->delay < 0) {
            work->sequenceState++;
            BattleAnimSpritePop_Start(work, work->x - work->side * 24, work->y - 24);
        }
        break;
    case 3:
        if (BattleAnimSpritePop_Update(work)) {
            work->sequenceState++;
        }
        break;
    case 4:
        done = TRUE;
        break;
    }
    return done;
}

void BattleAnimTask_SpritePops(SysTask *task, void *data) {
    BattleAnimSpritePopWork *work = data;
    switch (work->state) {
    case 0:
        if (BattleAnimSpritePop_RunSequence(work)) {
            work->state++;
        }
        break;
    case 1:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

#pragma force_active on

void BattleAnimSpriteFunc_SpritePops(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimSpritePopWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSpritePopWork));
    Pokepic *pokepic;
    work->system = system;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->sprite = sprite;
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, 2);
    ManagedSprite_SetDrawFlag(work->sprite, FALSE);
    work->delay = 4;
    work->side = ov07_02222004(work->system, BattleAnimSystem_GetAttacker(work->system));
    pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(system));
    work->x = Pokepic_GetAttr(pokepic, POKEPIC_X);
    work->y = Pokepic_GetAttr(pokepic, POKEPIC_Y);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_SpritePops, work);
}

#pragma force_active reset

void BattleAnimTask_WaitPaletteFade(SysTask *task, void *data) {
    BattleAnimPaletteFadeWork *work = data;
    if (PaletteData_GetSelectedBuffersBitmask(work->paletteData) == 0) {
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task); // reads the freed work
    }
}

void BattleAnimFunc_FadeBgPalette(BattleAnimSystem *system) {
    BattleAnimPaletteFadeWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimPaletteFadeWork));
    work->system = system;
    work->paletteData = BattleAnimSystem_GetPaletteData(system);
    u16 opaqueBits;
    switch (BattleAnimSystem_GetArg(system, 0)) {
    case 0:
        opaqueBits = BattleAnimSystem_GetBattleBgPaletteMask(system);
        break;
    case 1:
        opaqueBits = 0x100;
        break;
    case 2:
        opaqueBits = 0x200;
        break;
    default:
        GF_AssertFail();
        break;
    }
    int wait = BattleAnimSystem_GetArg(system, 1);
    int cur = BattleAnimSystem_GetArg(system, 2);
    int end = BattleAnimSystem_GetArg(system, 3);
    int color = BattleAnimSystem_GetArg(system, 4);
    PaletteData_BeginPaletteFade(work->paletteData, 1 << PLTTBUF_MAIN_BG, opaqueBits, (s8)wait, (u8)cur, (u8)end, (u16)color);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_WaitPaletteFade, work);
}

// Fades a battler's palette to a color and back, count times
void BattleAnimTask_BattlerPaletteFade(SysTask *task, void *data) {
    BattleAnimBattlerPaletteFadeWork *work = data;
    switch (work->state) {
    case 0:
        Pokepic_StartPaletteFade(work->pokepic, 0, work->strength, work->framesPer, work->color);
        work->state++;
        break;
    case 1:
        if (!Pokepic_ResumePaletteFade(work->pokepic)) {
            work->state++;
        }
        break;
    case 2:
        if (work->delay == 0) {
            work->state++;
        } else {
            work->delay--;
            break;
        }
    case 3:
        Pokepic_StartPaletteFade(work->pokepic, work->strength, 0, work->framesPer, work->color);
        work->state++;
        break;
    case 4:
        if (!Pokepic_ResumePaletteFade(work->pokepic)) {
            if (--work->count <= 0) {
                work->state++;
            } else {
                work->state = 0;
            }
        }
        break;
    case 5:
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task); // reads the freed work
        break;
    }
}

void BattleAnimFunc_BattlerPaletteFade(BattleAnimSystem *system) {
    BattleAnimBattlerPaletteFadeWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimBattlerPaletteFadeWork));
    work->system = system;
    work->pokepic = NULL;
    int role = BattleAnimSystem_GetArg(system, 0);
    int i, battler;
    switch (role) {
    case BATTLE_ANIM_ROLE_ATTACKER:
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
        break;
    case BATTLE_ANIM_ROLE_ATTACKER_PARTNER:
        if (BattleAnimSystem_IsDoubleBattle(system) == TRUE) {
            work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimUtil_GetPartnerBattler(work->system, BattleAnimSystem_GetAttacker(work->system)));
        }
        break;
    case BATTLE_ANIM_ROLE_DEFENDER:
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(system));
        break;
    case BATTLE_ANIM_ROLE_DEFENDER_PARTNER:
        if (BattleAnimSystem_IsDoubleBattle(system) == TRUE) {
            work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimUtil_GetPartnerBattler(work->system, BattleAnimSystem_GetDefender(work->system)));
        }
        break;
    case 0x800 | BATTLE_ANIM_ROLE_ATTACKER:
        battler = 0xFF;
        for (i = 0; i < 4; i++) {
            int type = BattleAnimSystem_GetBattlerType(system, i);
            if (type == BATTLER_TYPE_SOLO_PLAYER || type == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                battler = i;
                break;
            }
        }
        if (battler == 0xFF) {
            battler = 0;
        }
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, battler);
        break;
    case 0x800 | BATTLE_ANIM_ROLE_ATTACKER_PARTNER:
        battler = 0xFF;
        for (i = 0; i < 4; i++) {
            if (BattleAnimSystem_GetBattlerType(system, i) == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
                battler = i;
                break;
            }
        }
        if (battler == 0xFF) {
            battler = 0;
        }
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, battler);
        break;
    case 0x800 | BATTLE_ANIM_ROLE_DEFENDER:
        battler = 0xFF;
        for (i = 0; i < 4; i++) {
            int type = BattleAnimSystem_GetBattlerType(system, i);
            if (type == BATTLER_TYPE_SOLO_ENEMY || type == BATTLER_TYPE_ENEMY_SIDE_SLOT_1) {
                battler = i;
                break;
            }
        }
        if (battler == 0xFF) {
            battler = 0;
        }
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, battler);
        break;
    case 0x800 | BATTLE_ANIM_ROLE_DEFENDER_PARTNER:
        battler = 0xFF;
        for (i = 0; i < 4; i++) {
            if (BattleAnimSystem_GetBattlerType(system, i) == BATTLER_TYPE_ENEMY_SIDE_SLOT_2) {
                battler = i;
                break;
            }
        }
        if (battler == 0xFF) {
            battler = 0;
        }
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, battler);
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (work->pokepic == NULL) {
        Heap_Free(work);
        return;
    }
    work->framesPer = BattleAnimSystem_GetArg(system, 1);
    work->count = BattleAnimSystem_GetArg(system, 2);
    work->color = BattleAnimSystem_GetArg(system, 3);
    work->strength = BattleAnimSystem_GetArg(system, 4);
    work->delay = BattleAnimSystem_GetArg(system, 5);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_BattlerPaletteFade, work);
}

// Scales a battler sprite copy from start to end and back, count times, keeping its bottom in place
void BattleAnimTask_PulseBattlerSprite(SysTask *task, void *data) {
    BattleAnimPulseBattlerSpriteWork *work = data;
    f32 scaleX, scaleY;
    BOOL active;
    switch (work->state) {
    case 0:
        BattleAnimUtil_InitScaleMotion(&work->scale, (s16)work->start, (s16)work->divisor, (s16)work->end, work->durations >> 16);
        BattleAnimUtil_GetScaleFloat(&work->scale, &scaleX, &scaleY);
        if (work->flipX == TRUE) {
            scaleX = 0.0f - scaleX;
        }
        ManagedSprite_SetAffineScale(work->sprite, scaleX, scaleY);
        work->state++;
        break;
    case 1:
        active = BattleAnimUtil_UpdateScaleMotion(&work->scale);
        BattleAnimUtil_GetScaleFloat(&work->scale, &scaleX, &scaleY);
        if (work->flipX == TRUE) {
            scaleX = 0.0f - scaleX;
        }
        ManagedSprite_SetAffineScale(work->sprite, scaleX, scaleY);
        if (active) {
            BattleAnimUtil_AlignSpriteToScale(work->sprite, work->y, work->height, work->scale.curY, 0);
        } else {
            work->state++;
        }
        break;
    case 2:
        BattleAnimUtil_InitScaleMotion(&work->scale, (s16)work->end, (s16)work->divisor, (s16)work->start, (u16)work->durations);
        work->state++;
        break;
    case 3:
        active = BattleAnimUtil_UpdateScaleMotion(&work->scale);
        BattleAnimUtil_GetScaleFloat(&work->scale, &scaleX, &scaleY);
        if (work->flipX == TRUE) {
            scaleX = 0.0f - scaleX;
        }
        ManagedSprite_SetAffineScale(work->sprite, scaleX, scaleY);
        if (active) {
            BattleAnimUtil_AlignSpriteToScale(work->sprite, work->y, work->height, work->scale.curY, 0);
        } else if (--work->count <= 0) {
            work->state++;
        } else {
            work->state = 0;
        }
        break;
    case 4:
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task); // reads the freed work
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void BattleAnimFunc_PulseBattlerSprite(BattleAnimSystem *system) {
    BattleAnimPulseBattlerSpriteWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimPulseBattlerSpriteWork));
    work->system = system;
    work->spriteManager = BattleAnimSystem_GetBattlerSpriteManager(system);
    Pokepic *pokepic;
    if (BattleAnimSystem_GetArg(system, 0) == 0) {
        pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
        work->height = ov07_0221FAA0(work->system, BattleAnimSystem_GetAttacker(work->system));
    } else {
        pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetDefender(work->system));
        work->height = ov07_0221FAA0(work->system, BattleAnimSystem_GetDefender(work->system));
    }
    work->y = Pokepic_GetAttr(pokepic, POKEPIC_Y);
    work->y -= Pokepic_GetAttr(pokepic, POKEPIC_SHADOW_H);
    work->eva = BattleAnimSystem_GetArg(system, 1);
    work->start = BattleAnimSystem_GetArg(system, 2);
    work->end = BattleAnimSystem_GetArg(system, 3);
    work->divisor = BattleAnimSystem_GetArg(system, 4);
    work->count = BattleAnimSystem_GetArg(system, 5);
    work->durations = BattleAnimSystem_GetArg(system, 6);
    work->sprite = BattleAnimSystem_GetBattlerSprite(work->system, BattleAnimSystem_GetArg(system, 7));
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, 2);
    BattleAnimUtil_SetBlendWithBattleBg(work->system, work->eva, 16 - work->eva);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_PulseBattlerSprite, work);
    int side = BattleAnimSystem_GetArg(system, 0) != 0;
    if (ov07_0221FB78(work->system, side) == TRUE) {
        work->flipX = TRUE;
    }
}

// Stretches a battler (separate X and Y scale) and back, count times, keeping its bottom in place
void BattleAnimTask_StretchBattler(SysTask *task, void *data) {
    BattleAnimStretchBattlerWork *work = data;
    switch (work->state) {
    case 0:
        BattleAnimUtil_InitScaleMotionXY(&work->scale, (s16)work->startX, (s16)work->endX, (s16)work->startY, (s16)work->endY, (s16)work->divisor, work->durations >> 16);
        BattleAnimUtil_UpdateScaleMotionPokepic(&work->scale, work->pokepic);
        work->state++;
        break;
    case 1:
        if (BattleAnimUtil_UpdateScaleMotionXYPokepic(&work->scale, work->pokepic)) {
            BattleAnimUtil_AlignPokepicToScale(work->pokepic, work->y, work->height, work->scale.curY, 0);
        } else {
            work->state++;
        }
        break;
    case 2:
        if (work->holdFrames == 0) {
            work->state++;
        } else {
            work->holdFrames--;
            break;
        }
    case 3:
        BattleAnimUtil_InitScaleMotionXY(&work->scale, (s16)work->endX, (s16)work->startX, (s16)work->endY, (s16)work->startY, (s16)work->divisor, (u16)work->durations);
        BattleAnimUtil_UpdateScaleMotionXYPokepic(&work->scale, work->pokepic);
        work->state++;
        break;
    case 4:
        if (BattleAnimUtil_UpdateScaleMotionXYPokepic(&work->scale, work->pokepic)) {
            BattleAnimUtil_AlignPokepicToScale(work->pokepic, work->y, work->height, work->scale.curY, 0);
        } else if (--work->count <= 0) {
            work->state++;
        } else {
            work->state = 0;
        }
        break;
    case 5:
        BattleAnimUtil_ResetPokepicScale(work->pokepic);
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task); // reads the freed work
        break;
    }
}

#pragma force_active on

void BattleAnimFunc_StretchBattler(BattleAnimSystem *system) {
    BattleAnimTarget targets[4];
    int count;
    BattleAnimStretchBattlerWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimStretchBattlerWork));
    int i, battler;
    work->system = system;
    switch ((int)BattleAnimSystem_GetArg(system, 0)) {
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_ATTACKER:
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_ATTACKER_PARTNER:
        work->height = ov07_0221FAA0(work->system, BattleAnimSystem_GetAttacker(work->system));
        BattleAnimUtil_CollectPicsByFlags(work->system, BattleAnimSystem_GetArg(system, 0), targets, &count);
        work->pokepic = targets[0].pokepic;
        break;
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_DEFENDER:
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_DEFENDER_PARTNER:
        work->height = ov07_0221FAA0(work->system, BattleAnimSystem_GetDefender(work->system));
        BattleAnimUtil_CollectPicsByFlags(work->system, BattleAnimSystem_GetArg(system, 0), targets, &count);
        work->pokepic = targets[0].pokepic;
        break;
    case 0x800 | BATTLE_ANIM_ROLE_ATTACKER:
        battler = 0xFF;
        for (i = 0; i < 4; i++) {
            int type = BattleAnimSystem_GetBattlerType(system, i);
            if (type == BATTLER_TYPE_SOLO_PLAYER || type == BATTLER_TYPE_PLAYER_SIDE_SLOT_1) {
                battler = i;
                break;
            }
        }
        if (battler == 0xFF) {
            battler = 0;
        }
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, battler);
        break;
    case 0x800 | BATTLE_ANIM_ROLE_ATTACKER_PARTNER:
        battler = 0xFF;
        for (i = 0; i < 4; i++) {
            if (BattleAnimSystem_GetBattlerType(system, i) == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
                battler = i;
                break;
            }
        }
        if (battler == 0xFF) {
            battler = 0;
        }
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, battler);
        break;
    case 0x800 | BATTLE_ANIM_ROLE_DEFENDER:
        battler = 0xFF;
        for (i = 0; i < 4; i++) {
            int type = BattleAnimSystem_GetBattlerType(system, i);
            if (type == BATTLER_TYPE_SOLO_ENEMY || type == BATTLER_TYPE_ENEMY_SIDE_SLOT_1) {
                battler = i;
                break;
            }
        }
        if (battler == 0xFF) {
            battler = 0;
        }
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, battler);
        break;
    case 0x800 | BATTLE_ANIM_ROLE_DEFENDER_PARTNER:
        battler = 0xFF;
        for (i = 0; i < 4; i++) {
            if (BattleAnimSystem_GetBattlerType(system, i) == BATTLER_TYPE_ENEMY_SIDE_SLOT_2) {
                battler = i;
                break;
            }
        }
        if (battler == 0xFF) {
            battler = 0;
        }
        work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, battler);
        break;
    }
    if (work->pokepic == NULL) {
        Heap_Free(work);
        return;
    }
    work->y = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->height = Pokepic_GetAttr(work->pokepic, POKEPIC_SHADOW_H);
    work->height += 16;
    work->startX = BattleAnimSystem_GetArg(system, 1);
    work->endX = BattleAnimSystem_GetArg(system, 2);
    work->startY = BattleAnimSystem_GetArg(system, 3);
    work->endY = BattleAnimSystem_GetArg(system, 4);
    work->divisor = BattleAnimSystem_GetArg(system, 5);
    work->count = BattleAnimSystem_GetArg(system, 6);
    work->durations = BattleAnimSystem_GetArg(system, 7);
    work->count = (u16)BattleAnimSystem_GetArg(system, 6);
    work->holdFrames = (int)BattleAnimSystem_GetArg(system, 6) >> 16;
    BattleAnim_CreateTaskAndRun(work->system, BattleAnimTask_StretchBattler, work);
}

#pragma force_active reset

void BattleAnimTask_BlendBattlerSprites(SysTask *task, void *data) {
    BattleAnimBlendBattlerSpritesWork *work = data;
    if (BattleAnimUtil_IsBlendAlphaMotionDone(&work->blend)) {
        Heap_Free(work);
        BattleAnimSystem_DestroyTask(work->system, task); // reads the freed work
    } else {
        SpriteSystem_DrawSprites(work->spriteManager);
    }
}

#pragma force_active on

void BattleAnimFunc_BlendBattlerSprites(BattleAnimSystem *system) {
    BattleAnimBlendBattlerSpritesWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimBlendBattlerSpritesWork));
    int mask, i, startEva, endEva, startEvb, endEvb, duration;
    work->system = system;
    work->spriteManager = BattleAnimSystem_GetBattlerSpriteManager(system);
    mask = BattleAnimSystem_GetArg(system, 0);
    for (i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            ManagedSprite_SetOamMode(BattleAnimSystem_GetBattlerSprite(work->system, i), GX_OAM_MODE_XLU);
        }
    }
    startEva = BattleAnimSystem_GetArg(system, 1);
    startEvb = BattleAnimSystem_GetArg(system, 3);
    BattleAnimUtil_SetBlendWithBattleBg(work->system, startEva, startEvb);
    endEva = BattleAnimSystem_GetArg(system, 2);
    endEvb = BattleAnimSystem_GetArg(system, 4);
    duration = BattleAnimSystem_GetArg(system, 5);
    BattleAnimUtil_StartBlendAlphaMotion(&work->blend, (s16)startEva, (s16)endEva, (s16)startEvb, (s16)endEvb, duration);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_BlendBattlerSprites, work);
}

// Shows (0) or hides (1) the battlers selected by flags; a semi-invulnerable battler is not shown
void BattleAnimFunc_SetBattlerVanished(BattleAnimSystem *system) {
    int i;
    int vanished = BattleAnimSystem_GetArg(system, 1);
    int count;
    BattleAnimTarget target;
    BattleAnimUtil_CollectPicsByFlags(system, BattleAnimSystem_GetArg(system, 0), &target, &count);
    for (i = 0; i < count; i++) {
        if (BattleAnimSystem_IsBattlerSemiInvulnerable(system, target.battler) != TRUE || vanished) {
            Pokepic_SetAttr(target.pokepic, POKEPIC_VANISHED, vanished);
        }
    }
}

#pragma force_active reset

void BattleAnimTask_BlinkBattler(SysTask *task, void *data) {
    BattleAnimBlinkBattlerWork *work = data;
    if (work->timer >= work->interval) {
        if (work->count == 0) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, FALSE);
            Heap_Free(work);
            BattleAnimSystem_DestroyTask(work->system, task); // reads the freed work
        } else {
            Pokepic_SetAttr(work->pokepic, POKEPIC_VANISHED, 1 ^ Pokepic_GetAttr(work->pokepic, POKEPIC_VANISHED));
            work->count--;
            work->timer = 0;
        }
    } else {
        work->timer++;
    }
}

void BattleAnimFunc_BlinkBattler(BattleAnimSystem *system) {
    BattleAnimBlinkBattlerWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimBlinkBattlerWork));
    work->system = system;
    work->count = BattleAnimSystem_GetArg(system, 0) * 2;
    work->interval = BattleAnimSystem_GetArg(system, 1);
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(system, BattleAnimSystem_GetAttacker(system));
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_BlinkBattler, work);
}

void BattleAnimTask_SlideBattler(SysTask *task, void *data) {
    BattleAnimSlideBattlerWork *work = data;
    switch (work->common.state[0]) {
    case 0:
        BattleAnimUtil_InitLinearMotion(&work->motion, work->target.pos[0], work->target.pos[0] + work->dx, work->target.pos[1], work->target.pos[1], (u16)work->duration);
        work->common.state[0]++;
        break;
    case 1:
        if (BattleAnimUtil_UpdateLinearMotion(&work->motion) == FALSE) {
            work->common.state[0]++;
        }
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_X, work->motion.x);
        break;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

// Slides a battler horizontally by dx (mirrored on the enemy side)
void BattleAnimFunc_SlideBattler(BattleAnimSystem *system) {
    BattleAnimSlideBattlerWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSlideBattlerWork));
    int role, battler;
    work->system = system;
    work->duration = BattleAnimSystem_GetArg(system, 0);
    work->dx = BattleAnimSystem_GetArg(system, 1);
    role = BattleAnimSystem_GetArg(system, 2);
    battler = 0xFF;
    switch (role) {
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_ATTACKER:
        battler = BattleAnimSystem_GetAttacker(system);
        break;
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_ATTACKER_PARTNER:
        if (BattleAnimSystem_IsDoubleBattle(system) == TRUE) {
            battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetAttacker(system));
        }
        break;
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_DEFENDER:
        battler = BattleAnimSystem_GetDefender(system);
        break;
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_DEFENDER_PARTNER:
        if (BattleAnimSystem_IsDoubleBattle(system) == TRUE) {
            battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetDefender(system));
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (battler == 0xFF) {
        Heap_Free(work);
        return;
    }
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->target.pokepic = BattleAnimSystem_GetBattlerPokepic(system, battler);
    BattleAnimUtil_GetPicPosition(work->target.pokepic, work->target.pos);
    if (ov07_02222004(system, battler) <= 0) {
        work->dx *= -1;
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_SlideBattler, work);
}

void BattleAnimTask_SlideBattler2(SysTask *task, void *data) {
    BattleAnimMoveBattlerWork *work = data;
    switch (work->common.state[0]) {
    case 0:
        if (work->target.pokepic == NULL) {
            work->common.state[0] = 0xFF;
            return;
        }
        BattleAnimUtil_InitLinearMotion(&work->motion, work->target.pos[0], work->target.pos[0] + work->dx, work->target.pos[1], work->target.pos[1], (u16)work->duration);
        work->common.state[0]++;
    case 1:
        if (BattleAnimUtil_UpdateLinearMotion(&work->motion) == FALSE) {
            work->common.state[0]++;
        }
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_X, work->motion.x);
        break;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

// As BattleAnimFunc_SlideBattler; the first step runs right away
void BattleAnimFunc_SlideBattler2(BattleAnimSystem *system) {
    BattleAnimMoveBattlerWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimMoveBattlerWork));
    int role, battler;
    work->system = system;
    work->duration = BattleAnimSystem_GetArg(system, 0);
    work->dx = BattleAnimSystem_GetArg(system, 1);
    role = BattleAnimSystem_GetArg(system, 2);
    BattleAnimUtil_InitCommonWork(system, &work->common);
    battler = 0xFF;
    switch (role) {
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_ATTACKER:
        battler = BattleAnimSystem_GetAttacker(system);
        break;
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_ATTACKER_PARTNER:
        if (BattleAnimSystem_IsDoubleBattle(system) == TRUE) {
            battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetAttacker(system));
        }
        break;
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_DEFENDER:
        battler = BattleAnimSystem_GetDefender(system);
        break;
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_DEFENDER_PARTNER:
        if (BattleAnimSystem_IsDoubleBattle(system) == TRUE) {
            battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetDefender(system));
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (battler == 0xFF) {
        Heap_Free(work);
        return;
    }
    work->target.pokepic = BattleAnimSystem_GetBattlerPokepic(system, battler);
    BattleAnimUtil_GetPicPosition(work->target.pokepic, work->target.pos);
    if (ov07_02222004(system, battler) <= 0) {
        work->dx *= -1;
    }
    ov07_0221BFC0(system);
    BattleAnim_CreateTaskAndRun(work->system, BattleAnimTask_SlideBattler2, work);
}

void BattleAnimTask_ShakeAndScaleBattler(SysTask *task, void *data) {
    BattleAnimShakeAndScaleWork *work = data;
    int start, end, duration;
    switch (work->state) {
    case 0:
        if (BattleAnimUtil_UpdateScaleMotion(&work->scale) == TRUE) {
            BattleAnimUtil_UpdateShake(&work->shake);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
            Pokepic_AddAttr(work->pokepic, POKEPIC_X, work->shake.x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->baseY + BattleAnimUtil_CalcScaleYOffset(work->baseY, work->height, work->scale.curY));
            return;
        }
        work->state++;
        return;
    case 1:
        if (++work->timer <= work->delay) {
            return;
        }
        start = BattleAnimSystem_GetArg(work->system, 0);
        end = BattleAnimSystem_GetArg(work->system, 1);
        duration = BattleAnimSystem_GetArg(work->system, 3);
        BattleAnimUtil_InitScaleMotion(&work->scale, (s16)start, 100, (s16)end, duration);
        work->state++;
        return;
    case 2:
        if (BattleAnimUtil_UpdateScaleMotion(&work->scale) == TRUE) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->baseY + BattleAnimUtil_CalcScaleYOffset(work->baseY, work->height, work->scale.curY));
            return;
        }
        work->state++;
        return;
    case 3:
        if (BattleAnimUtil_UpdateScaleMotion(&work->scale) == TRUE) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->baseY + BattleAnimUtil_CalcScaleYOffset(work->baseY, work->height, work->scale.curY));
            return;
        }
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, 256);
        Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, 256);
        Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->baseY);
        work->state++;
        return;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        return;
    }
}

// Scales the attacker to arg 0 % while it shakes, waits, then scales it on to arg 1 % and back to normal
void BattleAnimFunc_ShakeAndScaleBattler(BattleAnimSystem *system) {
    BattleAnimShakeAndScaleWork *work;
    int end;
    work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimShakeAndScaleWork));
    work->unk_08 = 0;
    work->state = 0;
    work->system = system;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(system));
    work->timer = 0;
    work->delay = BattleAnimSystem_GetArg(system, 4);
    work->baseY = Pokepic_GetAttr(work->pokepic, POKEPIC_Y);
    work->height = ov07_0221FAA0(work->system, BattleAnimSystem_GetAttacker(work->system));
    end = BattleAnimSystem_GetArg(system, 0);
    BattleAnimUtil_InitScaleMotion(&work->scale, 100, 100, (s16)end, BattleAnimSystem_GetArg(system, 2));
    BattleAnimUtil_InitShake(&work->shake, 2, 0, 0, 10);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_ShakeAndScaleBattler, work);
}

#pragma force_active on

void BattleAnimTask_ShakeAndScaleBattlerXY(SysTask *task, void *data) {
    BattleAnimShakeAndScaleXYWork *work = data;
    switch (work->state) {
    case 0:
        if (BattleAnimUtil_UpdateScaleMotionXY(&work->scale) == TRUE) {
            BattleAnimUtil_UpdateShake(&work->shake);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
            Pokepic_AddAttr(work->pokepic, POKEPIC_X, work->shake.x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->baseY + BattleAnimUtil_CalcScaleYOffset(work->baseY, work->height, work->scale.curY));
        } else {
            work->state++;
        }
        break;
    case 1:
        work->timer++;
        if (work->timer > work->delay) {
            BattleAnimUtil_InitScaleMotionXY(&work->scale, (s16)((work->scaleX >> 16) & 0xFFFF), (s16)(work->scaleX & 0xFF), (s16)((work->scaleY >> 16) & 0xFFFF), (s16)(work->scaleY & 0xFF), 100, work->duration);
            work->state++;
        }
        break;
    case 2:
        if (BattleAnimUtil_UpdateScaleMotionXY(&work->scale) == TRUE) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->baseY + BattleAnimUtil_CalcScaleYOffset(work->baseY, work->height, work->scale.curY));
        } else {
            work->state++;
        }
        break;
    case 3:
        if (BattleAnimUtil_UpdateScaleMotionXY(&work->scale) == TRUE) {
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, work->scale.x);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, work->scale.y);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->baseY + BattleAnimUtil_CalcScaleYOffset(work->baseY, work->height, work->scale.curY));
        } else {
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEW, 256);
            Pokepic_SetAttr(work->pokepic, POKEPIC_AFFINEH, 256);
            Pokepic_SetAttr(work->pokepic, POKEPIC_Y, work->baseY);
            work->state++;
        }
        break;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

// As BattleAnimFunc_ShakeAndScaleBattler with separate X and Y scales (args packed start << 16 | end)
void BattleAnimFunc_ShakeAndScaleBattlerXY(BattleAnimSystem *system) {
    BattleAnimShakeAndScaleXYWork *work;
    s16 startX, startY, endX, endY;
    work = Heap_Alloc(BattleAnimSystem_GetHeapID(system), sizeof(BattleAnimShakeAndScaleXYWork));
    work->unk_08 = 0;
    work->state = 0;
    work->system = system;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->timer = 0;
    work->delay = BattleAnimSystem_GetArg(system, 6);
    work->scaleX = BattleAnimSystem_GetArg(work->system, 3);
    work->scaleY = BattleAnimSystem_GetArg(work->system, 4);
    work->duration = BattleAnimSystem_GetArg(work->system, 5);
    BattleAnimSystem_GetBattlerScreenPos(system, BattleAnimSystem_GetAttacker(work->system), NULL, &work->baseY);
    work->height = ov07_0221FAA0(work->system, BattleAnimSystem_GetAttacker(work->system));
    work->baseY += work->height;
    startX = (s32)BattleAnimSystem_GetArg(system, 0) >> 16;
    endX = BattleAnimSystem_GetArg(system, 0) & 0xFF;
    startY = (s32)BattleAnimSystem_GetArg(system, 1) >> 16;
    endY = BattleAnimSystem_GetArg(system, 1) & 0xFF;
    BattleAnimUtil_InitScaleMotionXY(&work->scale, startX, endX, startY, endY, 100, BattleAnimSystem_GetArg(system, 2));
    BattleAnimUtil_InitShake(&work->shake, 2, 0, 0, 10);
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_ShakeAndScaleBattlerXY, work);
}

#pragma force_active reset

void ov07_0222519C(SysTask *task, void *data) {
    BattleAnimObjWindowWork *work = data;
    reg_GX_DISPCNT &= ~(REG_GX_DISPCNT_W0_MASK | REG_GX_DISPCNT_W1_MASK | REG_GX_DISPCNT_OW_MASK);
    BattleAnimBackground_ResetLayer(work->common.system, GF_BG_LYR_MAIN_2);
    BattleAnimSystem_DestroyTask(work->common.system, task);
    Heap_Free(work);
}

#pragma force_active on

// Shows the battle background on BG2 through an OBJ window made of battler sprite 0 at 1.2 scale
void ov07_022251CC(BattleAnimSystem *system) {
    BattleAnimObjWindowWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimObjWindowWork));
    BattleAnimSystem_GetArg(system, 0);
    BattleAnimUtil_InitCommonWork(system, &work->common);
    BattleAnimBackground_LoadBattleBgToLayerDefault(work->common.system, GF_BG_LYR_MAIN_2);
    work->sprite = BattleAnimSystem_GetBattlerSprite(work->common.system, 0);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_OBJWND);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, 2);
    ManagedSprite_SetAffineScale(work->sprite, 1.2f, 1.2f);
    reg_GX_DISPCNT = (reg_GX_DISPCNT & ~(REG_GX_DISPCNT_W0_MASK | REG_GX_DISPCNT_W1_MASK | REG_GX_DISPCNT_OW_MASK)) | REG_GX_DISPCNT_OW_MASK;
    reg_G2_WINOUT = (reg_G2_WINOUT & ~0x3F) | (GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ);
    reg_G2_WINOUT = (reg_G2_WINOUT & 0xFFFFC0FF) | ((GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2) << 8);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, ov07_0222519C, work);
}

void BattleAnimTask_MoveBattlerBy(SysTask *task, void *data) {
    BattleAnimMoveBattlerWork *work = data;
    switch (work->common.state[0]) {
    case 0:
        BattleAnimUtil_InitLinearMotion(&work->motion, work->target.pos[0], (s16)(work->target.pos[0] + work->dx), work->target.pos[1], (s16)(work->target.pos[1] + work->dy), (u16)work->duration);
        work->common.state[0]++;
    case 1:
        if (!BattleAnimUtil_UpdateLinearMotion(&work->motion)) {
            work->common.state[0]++;
        }
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_X, work->motion.x);
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_Y, work->motion.y);
        break;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

// Moves a battler by (dx, dy) (mirrored on the enemy side)
void BattleAnimFunc_MoveBattlerBy(BattleAnimSystem *system) {
    BattleAnimMoveBattlerWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimMoveBattlerWork));
    int role, battler;
    work->system = system;
    work->duration = BattleAnimSystem_GetArg(system, 0);
    work->dx = BattleAnimSystem_GetArg(system, 1);
    work->dy = BattleAnimSystem_GetArg(system, 2);
    role = BattleAnimSystem_GetArg(system, 3);
    BattleAnimUtil_InitCommonWork(system, &work->common);
    battler = 0xFF;
    switch (role) {
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_ATTACKER:
        battler = BattleAnimSystem_GetAttacker(system);
        break;
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_ATTACKER_PARTNER:
        if (BattleAnimSystem_IsDoubleBattle(system) == TRUE) {
            battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetAttacker(system));
        }
        break;
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_DEFENDER:
        battler = BattleAnimSystem_GetDefender(system);
        break;
    case BATTLE_ANIM_TARGET_PICS | BATTLE_ANIM_ROLE_DEFENDER_PARTNER:
        if (BattleAnimSystem_IsDoubleBattle(system) == TRUE) {
            battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetDefender(system));
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (battler == 0xFF) {
        Heap_Free(work);
        return;
    }
    work->target.pokepic = BattleAnimSystem_GetBattlerPokepic(system, battler);
    if (work->target.pokepic == NULL) {
        Heap_Free(work);
        return;
    }
    BattleAnimUtil_GetPicPosition(work->target.pokepic, work->target.pos);
    if (ov07_02222004(system, battler) <= 0) {
        work->dx *= -1;
        work->dy *= -1;
    }
    if (ov07_0221BFC0(system) == 1) {
        work->dy *= -1;
    }
    BattleAnim_CreateTaskAndRun(work->system, BattleAnimTask_MoveBattlerBy, work);
}

#pragma force_active reset

void BattleAnimTask_MoveBattlerSin(SysTask *task, void *data) {
    BattleAnimMoveBattlerSinWork *work = data;
    if (BattleAnimUtil_UpdateSinMotionPokepic(&work->motion, work->target.pos[0], work->target.pos[1], work->target.pokepic) == FALSE) {
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_X, work->target.pos[0]);
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_Y, work->target.pos[1] - 8);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
    }
}

#pragma force_active on

// As BattleAnimFunc_CircleBattler with half the amplitude, on the battlers selected by flags
void BattleAnimFunc_MoveBattlerSin(BattleAnimSystem *system) {
    int count;
    int multiplier;
    BattleAnimMoveBattlerSinWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimMoveBattlerSinWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    BattleAnimUtil_CollectPicsByFlags(system, BattleAnimSystem_GetArg(system, 0), &work->target, &count);
    work->target.pos[1] += 8;
    multiplier = BattleAnimSystem_GetArg(system, 1);
    ov07_02222A44(&work->motion, multiplier, BattleAnimSystem_GetArg(system, 2));
    work->motion.ampX /= 2;
    work->motion.ampY /= 2;
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_MoveBattlerSin, work);
}

#pragma force_active reset

void BattleAnimTask_MoveBattlerLinear(SysTask *task, void *data) {
    BattleAnimMoveBattlerLinearWork *work = data;
    if (work->common.state[0] == 0) {
        if (BattleAnimUtil_UpdateLinearMotion(&work->motion) == FALSE) {
            work->common.state[0]++;
        }
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_X, work->motion.x);
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_Y, work->motion.y);
    } else {
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
    }
}

// Moves a battler off the screen on its side
void BattleAnimFunc_MoveBattlerLinear(BattleAnimSystem *system) {
    int battler;
    int count;
    int role;
    BattleAnimMoveBattlerLinearWork *work;
    int duration;

    work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimMoveBattlerLinearWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    role = BattleAnimSystem_GetArg(system, 0);
    duration = BattleAnimSystem_GetArg(system, 1);
    switch (role) {
    case BATTLE_ANIM_ROLE_ATTACKER:
        battler = BattleAnimSystem_GetAttacker(system);
        break;
    case BATTLE_ANIM_ROLE_ATTACKER_PARTNER:
        battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetAttacker(system));
        break;
    case BATTLE_ANIM_ROLE_DEFENDER:
        battler = BattleAnimSystem_GetDefender(system);
        break;
    case BATTLE_ANIM_ROLE_DEFENDER_PARTNER:
        battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetDefender(system));
        break;
    default:
        GF_AssertFail();
        break;
    }
    BattleAnimUtil_GetBattlerPosition(system, battler, &work->home);
    BattleAnimUtil_CollectPicsByFlags(system, role, &work->target, &count);
    work->offscreenX = 0;
    work->unk_5A = 0;
    work->offscreenX = BattleAnimUtil_GetBattlerSide(work->common.system, battler) == BATTLE_ANIM_SIDE_PLAYER ? -80 : 336;
    BattleAnimUtil_InitLinearMotion(&work->motion, work->target.pos[0], work->offscreenX, work->target.pos[1], work->target.pos[1], (u16)duration);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_MoveBattlerLinear, work);
}

// Moves a battler off the screen (arg 0 == 0) or from off the screen back to its place
void ov07_02225640(BattleAnimSystem *system) {
    int toHome;
    int battler;
    int count;
    BattleAnimMoveBattlerLinearWork *work;
    int role;
    int duration;

    work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimMoveBattlerLinearWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    toHome = BattleAnimSystem_GetArg(system, 0);
    role = BattleAnimSystem_GetArg(system, 1);
    duration = BattleAnimSystem_GetArg(system, 2);
    switch (role) {
    case BATTLE_ANIM_ROLE_ATTACKER:
        battler = BattleAnimSystem_GetAttacker(system);
        break;
    case BATTLE_ANIM_ROLE_ATTACKER_PARTNER:
        battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetAttacker(system));
        break;
    case BATTLE_ANIM_ROLE_DEFENDER:
        battler = BattleAnimSystem_GetDefender(system);
        break;
    case BATTLE_ANIM_ROLE_DEFENDER_PARTNER:
        battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetDefender(system));
        break;
    default:
        GF_AssertFail();
        break;
    }
    BattleAnimUtil_GetBattlerPosition(system, battler, &work->home);
    BattleAnimUtil_CollectPicsByFlags(system, role, &work->target, &count);
    work->offscreenX = 0;
    work->unk_5A = 0;
    work->offscreenX = BattleAnimUtil_GetBattlerSide(work->common.system, battler) == BATTLE_ANIM_SIDE_PLAYER ? -80 : 336;
    if (toHome == 0) {
        BattleAnimUtil_InitLinearMotion(&work->motion, work->target.pos[0], work->offscreenX, work->target.pos[1], work->target.pos[1], (u16)duration);
    } else {
        s16 homeX = BattleAnimSystem_GetBattlerScreenCoord(system, battler, 0);
        BattleAnimUtil_InitLinearMotion(&work->motion, work->offscreenX, homeX, work->target.pos[1], work->target.pos[1], (u16)duration);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_MoveBattlerLinear, work);
}

void BattleAnimFunc_ResetBattlerPosition(BattleAnimSystem *system) {
    int count;
    BattleAnimPosition home;
    BattleAnimTarget target;
    int role = BattleAnimSystem_GetArg(system, 0);
    int battler;
    switch (role) {
    case BATTLE_ANIM_ROLE_ATTACKER:
        battler = BattleAnimSystem_GetAttacker(system);
        break;
    case BATTLE_ANIM_ROLE_ATTACKER_PARTNER:
        battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetAttacker(system));
        break;
    case BATTLE_ANIM_ROLE_DEFENDER:
        battler = BattleAnimSystem_GetDefender(system);
        break;
    case BATTLE_ANIM_ROLE_DEFENDER_PARTNER:
        battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetDefender(system));
        break;
    default:
        GF_AssertFail();
        break;
    }
    int height;

    BattleAnimUtil_GetBattlerPosition(system, battler, &home);
    BattleAnimUtil_CollectPicsByFlags(system, role, &target, &count);
    height = (s16)ov07_0221FAA0(system, battler);
    Pokepic_SetAttr(target.pokepic, POKEPIC_X, home.x);
    Pokepic_SetAttr(target.pokepic, POKEPIC_Y, home.y + height);
}

void BattleAnimTask_WaitPaletteBlend(SysTask *task, void *data) {
    BattleAnimBlendSpritePaletteWork *work = data;
    if (BattleAnimUtil_IsPaletteBlendActive(work->blend) == FALSE) {
        BattleAnimUtil_FreePaletteBlend(work->blend);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
    }
}

#pragma force_active on

// Blends the OBJ palette of the sprite selected by flags towards a color
void BattleAnimFunc_BlendSpritePalette(BattleAnimSystem *system) {
    BattleAnimBlendSpritePaletteWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimBlendSpritePaletteWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    int color, end, start, step;
    int flags = BattleAnimSystem_GetArg(system, 0);
    int delay = BattleAnimSystem_GetArg(system, 1);
    step = BattleAnimSystem_GetArg(system, 2);
    start = BattleAnimSystem_GetArg(system, 3);
    end = BattleAnimSystem_GetArg(system, 4);
    color = BattleAnimSystem_GetArg(system, 5);
    int count;
    BattleAnimUtil_CollectSpritesByFlags(system, flags, &work->target, &count);
    u32 palette = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(work->target.sprite->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    enum HeapID heapId = BattleAnimSystem_GetHeapID(system);
    work->blend = BattleAnimUtil_StartPaletteBlend(work->common.paletteData, heapId, PLTTBUF_MAIN_OBJ, (u16)(palette << 4), 16, (s8)delay, (s8)step, (u8)start, (u8)end, (u16)color, 1100);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_WaitPaletteBlend, work);
}

void ov07_02225904(SysTask *task, void *data) {
    BattleAnimBattlerHeightWork *work = data;
    if (work->common.state[0] == 0) {
        work->timer++;
        if (work->timer >= work->interval) {
            work->timer = 0;
            if (work->growing == 0) {
                if (work->height < 0) {
                    work->height = 0;
                }
                if (work->height == 0) {
                    work->common.state[0]++;
                }
            } else {
                if (work->height > 80) {
                    work->height = 80;
                }
                if (work->height == 80) {
                    work->common.state[0]++;
                }
            }
            Pokepic_SetAttr(work->target.pokepic, POKEPIC_H, work->height);
            Pokepic_SetAttr(work->target.pokepic, POKEPIC_Y, work->y);
            work->y = work->y - work->step;
            work->height = work->height + work->step;
        }
    } else {
        BattleAnimUtil_ResetPokepicScale(work->target.pokepic);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
    }
}

// Shrinks (arg 1 == 0) or grows a battler's visible height while moving it down or up
void ov07_02225990(BattleAnimSystem *system) {
    BattleAnimBattlerHeightWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimBattlerHeightWork));
    int role, battler, count;
    BattleAnimUtil_InitCommonWork(system, &work->common);
    role = BattleAnimSystem_GetArg(system, 0);
    BattleAnimUtil_CollectPicsByFlags(system, role, &work->target, &count);
    switch (role) {
    case BATTLE_ANIM_ROLE_ATTACKER:
        battler = BattleAnimSystem_GetAttacker(system);
        break;
    case BATTLE_ANIM_ROLE_ATTACKER_PARTNER:
        battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetAttacker(system));
        break;
    case BATTLE_ANIM_ROLE_DEFENDER:
        battler = BattleAnimSystem_GetDefender(system);
        break;
    case BATTLE_ANIM_ROLE_DEFENDER_PARTNER:
        battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetDefender(system));
        break;
    default:
        GF_AssertFail();
        break;
    }
    work->growing = BattleAnimSystem_GetArg(system, 1);
    if (work->growing == 0) {
        work->y = Pokepic_GetAttr(work->target.pokepic, POKEPIC_Y);
        work->height = 80 - Pokepic_GetAttr(work->target.pokepic, POKEPIC_H);
        work->unk_40 = BattleAnimSystem_GetArg(system, 2);
        work->step = BattleAnimSystem_GetArg(system, 3);
        work->step *= -1;
    } else {
        work->y = Pokepic_GetAttr(work->target.pokepic, POKEPIC_Y);
        work->height = Pokepic_GetAttr(work->target.pokepic, POKEPIC_H);
        work->unk_40 = BattleAnimSystem_GetArg(system, 2);
        work->step = BattleAnimSystem_GetArg(system, 3);
    }
    work->visibleX = 0;
    work->visibleY = 0;
    work->visibleWidth = 80;
    work->visibleHeight = 80 - ov07_0221FAA0(system, battler);
    work->interval = BattleAnimSystem_GetArg(system, 4);
    work->timer = 0;
    Pokepic_SetVisible(work->target.pokepic, work->visibleX, work->visibleY, work->visibleWidth, work->visibleHeight);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, ov07_02225904, work);
}

#pragma force_active reset

int BattleAnimUtil_GetRoleBattler(BattleAnimSystem *system, int role) {
    int battler = 0;

    switch (role) {
    case BATTLE_ANIM_ROLE_ATTACKER:
        battler = BattleAnimSystem_GetAttacker(system);
        break;
    case BATTLE_ANIM_ROLE_ATTACKER_PARTNER:
        battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetAttacker(system));
        break;
    case BATTLE_ANIM_ROLE_DEFENDER:
        battler = BattleAnimSystem_GetDefender(system);
        break;
    case BATTLE_ANIM_ROLE_DEFENDER_PARTNER:
        battler = BattleAnimUtil_GetPartnerBattler(system, BattleAnimSystem_GetDefender(system));
        break;
    default:
        GF_AssertFail();
        break;
    }
    return battler;
}

void ov07_02225B38(SysTask *task, void *data) {
    BattleAnimRevealBattlerWork *work = data;
    switch (work->common.state[0]) {
    case 0:
        work->timer++;
        if (work->timer < work->interval) {
            break;
        }
        work->timer = 0;
        work->y += work->step;
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_Y, work->y);
        work->height -= work->step;
        if (work->height > work->maxHeight) {
            work->height = work->maxHeight;
            work->common.state[0]++;
        }
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_H, work->height);
        break;
    case 1:
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_VISIBLE, 0);
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_Y, work->baseY);
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_H, work->maxHeight);
        BattleAnimUtil_ResetPokepicScale(work->target.pokepic);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        break;
    }
}

void ov07_02225BC4(SysTask *task, void *data) {
    BattleAnimRevealBattlerWork *work = data;
    switch (work->common.state[0]) {
    case 0:
        work->timer++;
        if (work->timer < work->interval) {
            break;
        }
        work->timer = 0;
        work->y += work->step;
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_Y, work->y);
        work->height -= work->step;
        if (work->height < 0) {
            work->height = 0;
            work->common.state[0]++;
        }
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_H, work->height);
        break;
    case 1:
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_VISIBLE, 0);
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_Y, work->baseY);
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_H, work->maxHeight);
        BattleAnimUtil_ResetPokepicScale(work->target.pokepic);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        break;
    }
}

void ov07_02225C50(SysTask *task, void *data) {
    BattleAnimRevealBattlerWork *work = data;
    switch (work->common.state[0]) {
    case 0:
        work->timer++;
        if (work->timer < work->interval) {
            break;
        }
        work->timer = 0;
        work->height -= work->step;
        work->maxHeight += work->step;
        if (work->height < 0) {
            work->height = 0;
            work->common.state[0]++;
        }
        Pokepic_SetVisible(work->target.pokepic, 0, work->height, 80, work->maxHeight);
        break;
    case 1:
        Pokepic_SetAttr(work->target.pokepic, POKEPIC_VISIBLE, 0);
        BattleAnimUtil_ResetPokepicScale(work->target.pokepic);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        break;
    }
}

#pragma force_active on

void ov07_02225CC4(BattleAnimSystem *system) {
    BattleAnimRevealBattlerWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimRevealBattlerWork));
    int flags, count;
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->step = BattleAnimSystem_GetArg(system, 3);
    work->interval = BattleAnimSystem_GetArg(system, 4);
    flags = BattleAnimSystem_GetArg(system, 0);
    BattleAnimUtil_CollectPicsByFlags(system, flags, &work->target, &count);
    flags = BattleAnimUtil_GetRoleBattler(system, flags);
    if (BattleAnimSystem_GetArg(system, 5)) {
        work->height = 80 - ov07_0221FAA0(system, flags);
        work->maxHeight = 0;
        Pokepic_SetVisible(work->target.pokepic, 0, work->height, 80, 0);
        BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, ov07_02225C50, work);
    } else {
        work->y = Pokepic_GetAttr(work->target.pokepic, POKEPIC_Y);
        work->baseY = work->y;
        work->height = 80 - ov07_0221FAA0(system, flags);
        work->maxHeight = work->height;
        if (work->step > 0) {
            Pokepic_SetVisible(work->target.pokepic, 0, 0, 80, work->height);
            BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, ov07_02225BC4, work);
        } else {
            work->height = 0;
            work->y += work->maxHeight;
            Pokepic_SetAttr(work->target.pokepic, POKEPIC_Y, work->y);
            Pokepic_SetVisible(work->target.pokepic, 0, 0, 80, work->height);
            BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, ov07_02225B38, work);
        }
    }
}

// Shakes a BG layer (args: amplitudes, interval, shake count), repeatCount + 1 times
void BattleAnimTask_ShakeBg(SysTask *task, void *data) {
    BattleAnimShakeBgWork *work = data;
    switch (work->state) {
    case 0:
        BattleAnimUtil_InitShake(&work->shake, (s16)work->ampX, (s16)work->ampY, work->interval, work->shakeCount);
        work->state++;
    case 1:
        if (!BattleAnimUtil_UpdateShake(&work->shake)) {
            if (work->count >= work->repeatCount) {
                work->state++;
            } else {
                work->state--;
            }
            if (work->movedX) {
                BgSetPosTextAndCommit(work->bgConfig, (u8)work->bgLayer, BG_POS_OP_SET_X, 0);
            }
            if (work->movedY) {
                BgSetPosTextAndCommit(work->bgConfig, (u8)work->bgLayer, BG_POS_OP_SET_Y, 0);
            }
            work->count++;
        } else {
            if (work->shake.x) {
                work->movedX = TRUE;
                BgSetPosTextAndCommit(work->bgConfig, (u8)work->bgLayer, BG_POS_OP_SET_X, work->shake.x);
            }
            if (work->shake.y) {
                work->movedY = TRUE;
                BgSetPosTextAndCommit(work->bgConfig, (u8)work->bgLayer, BG_POS_OP_SET_Y, work->shake.y);
            }
        }
        break;
    default:
        BattleAnimSystem_DestroyTask(work->system, task);
        Heap_Free(work);
        break;
    }
}

#pragma force_active reset

void BattleAnimFunc_ShakeBg(BattleAnimSystem *system) {
    BattleAnimShakeBgWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimShakeBgWork));

    work->state = 0;
    work->count = 0;
    work->system = system;
    work->spriteManager = BattleAnimSystem_GetBattlerSpriteManager(system);
    work->bgConfig = BattleAnimSystem_GetBgConfig(work->system);
    work->ampX = BattleAnimSystem_GetArg(system, 0);
    work->ampY = BattleAnimSystem_GetArg(system, 1);
    work->interval = BattleAnimSystem_GetArg(system, 2);
    work->shakeCount = BattleAnimSystem_GetArg(system, 3);
    work->repeatCount = BattleAnimSystem_GetArg(system, 4);
    work->bgLayer = BattleAnimSystem_GetArg(system, 5) == 0 ? GF_BG_LYR_MAIN_3 : GF_BG_LYR_MAIN_2;
    BattleAnimSystem_CreateTaskDefaultPriority(work->system, BattleAnimTask_ShakeBg, work);
}

#pragma force_active on

void BattleAnimTask_SpriteMosaic(SysTask *task, void *data) {
    BattleAnimSpriteMosaicWork *work = data;
    if (work->sizeH == work->target || work->sizeV == work->target) {
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        return;
    }
    if (work->step < 0) {
        if (work->sizeH + work->step > 0) {
            work->sizeH += work->step;
            work->sizeV += work->step;
        } else {
            work->sizeH = work->target;
            work->sizeV = work->target;
        }
    } else {
        if (work->sizeH + work->step < 15) {
            work->sizeH += work->step;
            work->sizeV += work->step;
        } else {
            work->sizeH = work->target;
            work->sizeV = work->target;
        }
    }
    ManagedSprite_TickFrame(work->sprite);
    *(vu8 *)(REG_MOSAIC_ADDR + 1) = work->sizeH | (work->sizeV << 4); // OBJ mosaic size
    SpriteSystem_DrawSprites(work->common.battlerSpriteManager);
}

#pragma force_active reset

// Changes the mosaic size of a battler sprite step by step until it reaches 0 (step < 0) or 15
void BattleAnimFunc_SpriteMosaic(BattleAnimSystem *system) {
    BattleAnimSpriteMosaicWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSpriteMosaicWork));
    int index;

    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->step = BattleAnimSystem_GetArg(work->common.system, 1);
    work->sizeH = BattleAnimSystem_GetArg(work->common.system, 2);
    work->sizeV = BattleAnimSystem_GetArg(work->common.system, 3);
    index = BattleAnimSystem_GetArg(work->common.system, 0);
    work->sprite = BattleAnimSystem_GetBattlerSprite(work->common.system, index);
    ManagedSprite_SetDrawFlag(work->sprite, TRUE);
    work->target = work->step < 0 ? 0 : 15;
    ManagedSprite_SetMosaicFlag(work->sprite, TRUE);
    BattleAnim_CreateTaskAndRun(work->common.system, BattleAnimTask_SpriteMosaic, work);
}

#pragma force_active on

void BattleAnimEmitterMotion_Init(BattleAnimSystem *system, void *data) {
    BattleAnimEmitterMotionWork *work = data;
    u32 frames;
    work->emitterIndex = BattleAnimSystem_GetArg(system, 0);
    work->offsetX = BattleAnimSystem_GetArg(system, 1);
    work->offsetY = BattleAnimSystem_GetArg(system, 2);
    work->delay = BattleAnimSystem_GetArg(system, 3);
    work->duration = BattleAnimSystem_GetArg(system, 4);
    work->amplitude = BattleAnimSystem_GetArg(system, 5);
    work->direction = BattleAnimSystem_GetArg(system, 6);
    work->wave = BattleAnimSystem_GetArg(system, 8);
    frames = BattleAnimSystem_GetArg(system, 7);
    work->frame = 0;
    work->startFrame = (frames & 0xFFFF0000) >> 16;
    work->endFrame = (u16)frames;
    if (work->startFrame == 0) {
        work->startFrame = 0;
    }
    if (work->endFrame == 0) {
        work->endFrame = 255;
    }
    work->emitter = BattleAnimSystem_GetEmitter(system, work->emitterIndex);
    work->particleSystem = (ParticleSystem *)BattleAnimSystem_GetCurrentParticleSystem(system);
    if (work->direction == 0) {
        work->fromBattler = BattleAnimSystem_GetAttacker(system);
        work->toBattler = BattleAnimSystem_GetDefender(system);
    } else if (work->direction == 1) {
        work->fromBattler = BattleAnimSystem_GetDefender(system);
        work->toBattler = BattleAnimSystem_GetAttacker(system);
    }
    if (work->emitter == NULL) {
        GF_AssertFail();
    }
}

#pragma force_active reset

// TRUE unless the emitter's last particle has reached the end of its life
BOOL BattleAnimUtil_IsEmitterActive(SPLEmitter *emitter) {
    BOOL ret = FALSE;
    SPLParticle *particle = emitter->act_ptcl_list.p_begin;
    u16 age;
    u16 life;

    while (particle != NULL) {
        age = particle->age;
        life = particle->life;
        particle = particle->p_next;
    }
    if (age != life) {
        ret = TRUE;
    }
    return ret;
}

#pragma force_active on

void BattleAnimTask_MoveEmitter(SysTask *task, void *data) {
    BattleAnimEmitterMotionWork *work = data;
    BOOL active = BattleAnimUtil_IsEmitterActive(work->emitter);
    if (work->counter < work->delay) {
        work->counter++;
        return;
    }
    if (!BattleAnimUtil_UpdateLinearMotion(&work->linear) && !active) {
        ParticleSystem_DeleteEmitter(work->particleSystem, work->emitter);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        BattleAnimUtil_FreeWork(work);
        return;
    }
    if ((work->startFrame <= work->frame && work->endFrame >= work->frame) || work->endFrame == 255) {
        SPL_SetEmitterPositionX(work->emitter, work->linear.x * BATTLE_ANIM_EMITTER_PIXEL);
        SPL_SetEmitterPositionY(work->emitter, work->linear.y * BATTLE_ANIM_EMITTER_PIXEL);
        if (work->wave) {
            work->angle += 360 / (s16)work->duration;
            SPL_SetEmitterPositionY(work->emitter, work->linear.y * BATTLE_ANIM_EMITTER_PIXEL + GF_SinDeg((u16)work->angle));
        }
    }
}

#pragma force_active reset

// Moves an emitter from one battler to the other (offsets mirrored by side)
void BattleAnimFunc_MoveEmitter(BattleAnimSystem *system) {
    BattleAnimEmitterMotionWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimEmitterMotionWork));
    VecFx32 from;
    VecFx32 to;
    s32 side;
    s32 fromX;
    s32 toX;
    s32 fromY;
    s32 toY;
    int i;

    BattleAnimUtil_InitCommonWork(system, &work->common);
    BattleAnimEmitterMotion_Init(system, work);
    side = ov07_02222004(system, BattleAnimSystem_GetAttacker(system));
    ov07_02231B90(system, work->fromBattler, &from);
    ov07_02231B90(system, work->toBattler, &to);
    fromX = from.x / BATTLE_ANIM_EMITTER_PIXEL;
    toX = to.x / BATTLE_ANIM_EMITTER_PIXEL;
    fromY = from.y / BATTLE_ANIM_EMITTER_PIXEL;
    toY = to.y / BATTLE_ANIM_EMITTER_PIXEL;
    BattleAnimUtil_InitLinearMotion(&work->linear, (s16)fromX, (s16)(toX + work->offsetX * side), (s16)fromY, (s16)(toY + work->offsetY * side), work->duration);
    if (side > 0) {
        BattleAnimUtil_InitValueMotion(&work->value, 0xE38 * side, 0x5C71 * side, 10);
    } else {
        BattleAnimUtil_InitValueMotion(&work->value, 0x3FFF * side, 0x5C71 * side, 10);
    }
    for (i = 0; i < work->startFrame; i++) {
        BattleAnimUtil_UpdateLinearMotion(&work->linear);
    }
    if (work->endFrame != 255) {
        work->frame = work->endFrame + 1;
    }
    SPL_SetEmitterPositionX(work->emitter, work->linear.x * BATTLE_ANIM_EMITTER_PIXEL);
    SPL_SetEmitterPositionY(work->emitter, work->linear.y * BATTLE_ANIM_EMITTER_PIXEL);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_MoveEmitter, work);
}

void ov07_022262E8(BattleAnimSystem *system) {
    BattleAnimEmitterMotionWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimEmitterMotionWork));
    VecFx32 from;
    VecFx32 to;
    s32 mode;
    u32 frames;
    s32 fromX;
    s32 toX;
    s32 fromY;
    s32 toY;
    int i;

    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->emitterIndex = BattleAnimSystem_GetArg(system, 0);
    work->direction = BattleAnimSystem_GetArg(system, 1);
    mode = BattleAnimSystem_GetArg(system, 2);
    work->duration = BattleAnimSystem_GetArg(system, 3);
    work->delay = BattleAnimSystem_GetArg(system, 4);
    work->counter = 0;
    frames = BattleAnimSystem_GetArg(system, 5);
    work->frame = 0;
    work->startFrame = (frames & 0xFFFF0000) >> 16;
    work->endFrame = (u16)frames;
    if (work->startFrame == 0) {
        work->startFrame = 0;
    }
    if (work->endFrame == 0) {
        work->endFrame = 255;
    }
    work->emitter = BattleAnimSystem_GetEmitter(system, work->emitterIndex);
    work->particleSystem = (ParticleSystem *)BattleAnimSystem_GetCurrentParticleSystem(system);
    if (work->direction == 0) {
        work->fromBattler = BattleAnimSystem_GetAttacker(system);
        work->toBattler = BattleAnimSystem_GetAttacker(system);
    } else {
        work->fromBattler = BattleAnimSystem_GetDefender(system);
        work->toBattler = BattleAnimSystem_GetDefender(system);
    }
    if (work->emitter == NULL) {
        GF_AssertFail();
    }
    ov07_02231B90(system, work->fromBattler, &from);
    ov07_02231B90(system, work->toBattler, &to);
    if (mode == 0) {
        ov07_02231DD0(&from);
        from.x = to.x;
    } else {
        ov07_02231DD0(&to);
        to.x = from.x;
    }
    fromX = from.x / BATTLE_ANIM_EMITTER_PIXEL;
    toX = to.x / BATTLE_ANIM_EMITTER_PIXEL;
    fromY = from.y / BATTLE_ANIM_EMITTER_PIXEL;
    toY = to.y / BATTLE_ANIM_EMITTER_PIXEL;
    BattleAnimUtil_InitLinearMotion(&work->linear, (s16)fromX, (s16)toX, (s16)fromY, (s16)toY, work->duration);
    for (i = 0; i < work->startFrame; i++) {
        BattleAnimUtil_UpdateLinearMotion(&work->linear);
    }
    if (work->endFrame != 255) {
        work->frame = work->endFrame + 1;
    }
    SPL_SetEmitterPositionX(work->emitter, work->linear.x * BATTLE_ANIM_EMITTER_PIXEL);
    SPL_SetEmitterPositionY(work->emitter, work->linear.y * BATTLE_ANIM_EMITTER_PIXEL);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_MoveEmitter, work);
}

void BattleAnimTask_MoveEmitterArc(SysTask *task, void *data) {
    BattleAnimEmitterMotionWork *work = data;
    BOOL active = BattleAnimUtil_IsEmitterActive(work->emitter);

    if (!BattleAnimUtil_UpdateLinearSinMotion(&work->linear, &work->sin) && !active) {
        ParticleSystem_DeleteEmitter(work->particleSystem, work->emitter);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        BattleAnimUtil_FreeWork(work);
        return;
    }
    SPL_SetEmitterPositionX(work->emitter, work->linear.x * BATTLE_ANIM_EMITTER_PIXEL);
    SPL_SetEmitterPositionY(work->emitter, work->linear.y * BATTLE_ANIM_EMITTER_PIXEL);
}

// As BattleAnimFunc_MoveEmitter along an arc
void BattleAnimFunc_MoveEmitterArc(BattleAnimSystem *system) {
    BattleAnimEmitterMotionWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimEmitterMotionWork));
    VecFx32 from;
    VecFx32 to;
    s32 side;
    s32 fromX;
    s32 toX;
    s32 fromY;
    s32 toY;
    int i;

    BattleAnimUtil_InitCommonWork(system, &work->common);
    BattleAnimEmitterMotion_Init(system, work);
    side = ov07_02222004(system, BattleAnimSystem_GetAttacker(system));
    ov07_02231B90(system, work->fromBattler, &from);
    ov07_02231B90(system, work->toBattler, &to);
    fromX = from.x / BATTLE_ANIM_EMITTER_PIXEL;
    toX = to.x / BATTLE_ANIM_EMITTER_PIXEL;
    fromY = from.y / BATTLE_ANIM_EMITTER_PIXEL;
    toY = to.y / BATTLE_ANIM_EMITTER_PIXEL;
    BattleAnimUtil_InitLinearSinMotion(&work->linear, &work->sin, (s16)fromX, (s16)(toX + work->offsetX * side), (s16)fromY, (s16)(toY + work->offsetY * side), work->duration, -(work->amplitude << FX32_SHIFT));
    if (side > 0) {
        BattleAnimUtil_InitValueMotion(&work->value, 0xE38 * side, 0x5C71 * side, 10);
    } else {
        BattleAnimUtil_InitValueMotion(&work->value, 0x3FFF * side, 0x5C71 * side, 10);
    }
    for (i = 0; i < work->startFrame; i++) {
        BattleAnimUtil_UpdateLinearSinMotion(&work->linear, &work->sin);
    }
    if (work->endFrame != 255) {
        work->frame = work->endFrame + 1;
    }
    SPL_SetEmitterPositionX(work->emitter, work->linear.x * BATTLE_ANIM_EMITTER_PIXEL);
    SPL_SetEmitterPositionY(work->emitter, work->linear.y * BATTLE_ANIM_EMITTER_PIXEL);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_MoveEmitterArc, work);
}

void BattleAnimTask_EmitterSinMotion(SysTask *task, void *data) {
    BattleAnimEmitterSinWork *work = data;
    BOOL active = BattleAnimUtil_IsEmitterActive(work->emitter);

    if (!BattleAnimUtil_UpdateSinMotion(&work->motion) && !active) {
        ParticleSystem_DeleteEmitter(work->particleSystem, work->emitter);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        return;
    }
    {
        s32 y = work->base.y + work->motion.y * BATTLE_ANIM_EMITTER_PIXEL;
        s32 x = work->base.x + work->motion.x * BATTLE_ANIM_EMITTER_PIXEL;
        SPL_SetEmitterPositionX(work->emitter, x);
        SPL_SetEmitterPositionY(work->emitter, y);
    }
}

// Moves an emitter around a battler on a sin/cos path (angles in degrees)
void BattleAnimFunc_EmitterSinMotion(BattleAnimSystem *system) {
    BattleAnimEmitterSinWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimEmitterSinWork));
    s32 startAngleX;
    s32 endAngleX;
    s32 startAngleY;
    s32 endAngleY;

    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->emitterIndex = BattleAnimSystem_GetArg(system, 0);
    work->startAngleX = BattleAnimSystem_GetArg(system, 1);
    work->endAngleX = BattleAnimSystem_GetArg(system, 2);
    work->startAngleY = BattleAnimSystem_GetArg(system, 3);
    work->endAngleY = BattleAnimSystem_GetArg(system, 4);
    work->ampX = BattleAnimSystem_GetArg(system, 5);
    work->ampY = BattleAnimSystem_GetArg(system, 6);
    work->duration = BattleAnimSystem_GetArg(system, 7);
    work->useDefender = BattleAnimSystem_GetArg(system, 8);
    if (work->useDefender == 0) {
        work->battler = BattleAnimSystem_GetAttacker(system);
    } else {
        work->battler = BattleAnimSystem_GetDefender(system);
    }
    ov07_02231B90(system, work->battler, &work->base);
    work->emitter = BattleAnimSystem_GetEmitter(system, work->emitterIndex);
    work->particleSystem = (ParticleSystem *)BattleAnimSystem_GetParticleSystem(system, BattleAnimSystem_GetArg(system, 9));
    startAngleX = work->startAngleX * 0xFFFF / 360;
    endAngleX = work->endAngleX * 0xFFFF / 360;
    startAngleY = work->startAngleY * 0xFFFF / 360;
    endAngleY = work->endAngleY * 0xFFFF / 360;
    BattleAnimUtil_InitSinMotion(&work->motion, (u16)startAngleX, (u16)endAngleX, (u16)startAngleY, (u16)endAngleY, work->ampX << FX32_SHIFT, work->ampY << FX32_SHIFT, work->duration);
    BattleAnimUtil_UpdateSinMotion(&work->motion);
    {
        s32 y = work->base.y + work->motion.y * BATTLE_ANIM_EMITTER_PIXEL;
        s32 x = work->base.x + work->motion.x * BATTLE_ANIM_EMITTER_PIXEL;
        SPL_SetEmitterPositionX(work->emitter, x);
        SPL_SetEmitterPositionY(work->emitter, y);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_EmitterSinMotion, work);
}

void BattleAnimTask_PlaySpriteAnimOnce(SysTask *task, void *data) {
    BattleAnimSpriteWork *work = data;
    if (ManagedSprite_IsAnimated(work->sprite) == FALSE) {
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
    } else {
        ManagedSprite_TickFrame(work->sprite);
        SpriteSystem_DrawSprites(work->common.unk_10);
    }
}

// Moves the sprite by (arg 0, arg 1) and plays its animation once
void ov07_02226838(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimSpriteWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSpriteWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->sprite = sprite;
    s16 dx = BattleAnimSystem_GetArg(system, 0);
    s16 dy = BattleAnimSystem_GetArg(system, 1);
    ManagedSprite_OffsetPositionXY(sprite, dx, dy);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_PlaySpriteAnimOnce, work);
}

#pragma force_active on

// Moves the sprite sideways by the steps of sBattleAnimSpriteOrbitStepX (passing behind the battler), then rocks it
void BattleAnimTask_SpriteOrbit(SysTask *task, void *data) {
    BattleAnimSpriteOrbitWork *work = data;
    switch (work->common.state[0]) {
    case 0: {
        s16 dx = sBattleAnimSpriteOrbitStepX[work->step];
        work->step++;
        if (dx != 255) {
            if (dx == 0) {
                int priority = ManagedSprite_GetPriority(work->sprite);
                if (priority == ov07_0221FAE8(work->common.system)) {
                    ManagedSprite_SetPriority(work->sprite, BattleAnimSystem_GetBgPriority(work->common.system, 2));
                } else {
                    ManagedSprite_SetPriority(work->sprite, ov07_0221FAE8(work->common.system));
                }
            }
            ManagedSprite_OffsetPositionXY(work->sprite, dx, 0);
        } else {
            ManagedSprite_SetAffineOverwriteMode(work->sprite, 2);
            BattleAnimUtil_InitValueMotion(&work->angle, 0, 0xE38, 4);
            ManagedSprite_SetAffineTranslation(work->sprite, -8, 16);
            work->step = 0;
            work->common.state[0]++;
        }
        break;
    }
    case 1:
        if (BattleAnimUtil_UpdateValueMotion(&work->angle) == TRUE) {
            ManagedSprite_SetAffineZRotation(work->sprite, (u16)work->angle.value);
        } else if (work->step > 5) {
            work->common.state[0]++;
        } else {
            switch (++work->step) {
            case 1:
                BattleAnimUtil_InitValueMotion(&work->angle, 0xE38, -0xE38, 4);
                break;
            case 2:
                BattleAnimUtil_InitValueMotion(&work->angle, -0xE38, 0xE38, 4);
                break;
            case 3:
                BattleAnimUtil_InitValueMotion(&work->angle, 0xE38, -0xE38, 4);
                break;
            case 4:
                BattleAnimUtil_InitValueMotion(&work->angle, -0xE38, 0xE38, 4);
                break;
            case 5:
                BattleAnimUtil_InitValueMotion(&work->angle, 0xE38, 0, 2);
                break;
            }
        }
        break;
    default:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        return;
    }
    ManagedSprite_TickFrame(work->sprite);
    SpriteSystem_DrawSprites(work->common.unk_10);
}

#pragma force_active reset

#pragma force_active on

void BattleAnimSpriteFunc_SpriteOrbit(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimSpriteOrbitWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSpriteOrbitWork));
    int battler;
    s16 x, y;
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->sprite = sprite;
    work->step = 0;
    battler = BattleAnimSystem_GetDefender(system);
    x = BattleAnimSystem_GetBattlerScreenCoord(system, battler, 0);
    y = BattleAnimSystem_GetBattlerScreenCoord(system, battler, 1);
    ManagedSprite_SetPositionXY(work->sprite, x, y);
    if (BattleAnimUtil_GetBattlerSide(system, BattleAnimSystem_GetAttacker(system)) == BATTLE_ANIM_SIDE_ENEMY) {
        ManagedSprite_SetAnim(work->sprite, 1);
    } else {
        ManagedSprite_SetAnim(work->sprite, 0);
    }
    ManagedSprite_SetPriority(work->sprite, ov07_0221FAE8(work->common.system));
    ManagedSprite_OffsetPositionXY(work->sprite, 0, 12);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_SpriteOrbit, work);
}

#pragma force_active reset

void BattleAnimTask_PlaySpriteAnimOnce2(SysTask *task, void *data) {
    BattleAnimSpriteWork *work = data;
    if (ManagedSprite_IsAnimated(work->sprite) == FALSE) {
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
    } else {
        ManagedSprite_TickFrame(work->sprite);
        SpriteSystem_DrawSprites(work->common.unk_10);
    }
}

// Places the sprite above the defender (below it when the defender is on the player side) and plays its
// animation once
void ov07_02226A9C(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimSpriteOrbitWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSpriteOrbitWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->sprite = sprite;
    int battler = BattleAnimSystem_GetDefender(system);
    s16 x = BattleAnimSystem_GetBattlerScreenCoord(system, battler, 0);
    BattleAnimSystem_GetBattlerScreenCoord(system, battler, 1);
    int anim;
    s16 y;
    if (ov07_0221BFC0(system) == 1) {
        x = 72;
        y = 32;
        anim = 0;
    } else if (BattleAnimUtil_GetBattlerSide(work->common.system, battler) == BATTLE_ANIM_SIDE_PLAYER) {
        y = 126;
        anim = 1;
    } else {
        y = 32;
        anim = 0;
    }
    ManagedSprite_SetPositionXY(work->sprite, x, y);
    ManagedSprite_SetAnim(work->sprite, anim);
    int priority = BattleAnimSystem_GetBgPriority(work->common.system, 2);
    ManagedSprite_SetPriority(work->sprite, priority);
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, BattleAnimTask_PlaySpriteAnimOnce2, work);
}

#pragma force_active on

// Moves the sprite along an arc from the lower left, waits for its animation, then blends it out
void ov07_02226B2C(SysTask *task, void *data) {
    BattleAnimSpriteArcWork *work = data;
    switch (work->common.state[0]) {
    case 0:
        BattleAnimUtil_InitLinearSinMotion(&work->linear, &work->sin, -30, 112, 160, 112, 21, 64 * FX32_ONE);
        work->common.state[0]++;
        break;
    case 1:
        if (!BattleAnimUtil_UpdateLinearSinMotionSprite(&work->linear, &work->sin, work->sprite)) {
            work->common.state[0]++;
        }
        break;
    case 2:
        if (!ManagedSprite_IsAnimated(work->sprite)) {
            ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
            work->eva = 16;
            work->evb = 0;
            BattleAnimUtil_SetBlendWithBattleBg(work->common.system, work->eva, work->evb);
            work->common.state[0]++;
        }
        break;
    case 3:
        if (work->eva) {
            work->eva--;
        }
        if (work->evb < 16) {
            work->evb++;
        }
        reg_G2_BLDALPHA = work->eva | (work->evb << 8);
        if (!work->eva) {
            work->common.state[0]++;
        }
        break;
    default:
        Sprite_DeleteAndFreeResources(work->sprite);
        BattleAnimSystem_DestroyTask(work->common.system, task);
        Heap_Free(work);
        return;
    }
    ManagedSprite_TickFrame(work->sprite);
    SpriteSystem_DrawSprites(work->common.unk_10);
}

void ov07_02226C4C(BattleAnimSystem *system, SpriteSystem *spriteSystem, SpriteManager *spriteManager, ManagedSprite *sprite) {
    BattleAnimSpriteArcWork *work = BattleAnimUtil_AllocWork(system, sizeof(BattleAnimSpriteArcWork));
    BattleAnimUtil_InitCommonWork(system, &work->common);
    work->sprite = sprite;
    ManagedSprite_SetPositionXY(work->sprite, -30, 160);
    ManagedSprite_SetPriority(work->sprite, BattleAnimSystem_GetBgPriority(work->common.system, 2));
    BattleAnimSystem_CreateTaskDefaultPriority(work->common.system, ov07_02226B2C, work);
}

#pragma force_active reset
