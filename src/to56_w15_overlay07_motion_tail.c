#include "global.h"

#include "library/spl_emitter.h"
#include "w19_particle_motion_owner.h"

#pragma require_prototypes off

typedef struct Ov07Interp {
    s16 x;
    s16 y;
    u8 filler_04[0x20];
} Ov07Interp;


typedef struct Ov07Work94 {
    u8 filler_00[0x34];
    void *resource;
    SPLEmitter *node;
    u8 filler_3C[4];
    void *owner;
    u8 filler_44[0x14];
    Ov07Interp interp;
    u8 filler_7C[0xC];
    s32 baseX;
    s32 baseY;
} Ov07Work94;

extern BOOL ov07_022260DC(void *);
extern BOOL ov07_022222B4(W02LinearMotion *);
extern BOOL ov07_02222384(W02LinearMotion *, W02SinMotion *);
extern BOOL ov07_02222180(Ov07Interp *);
extern void BattleAnimUtil_FreeWork(void *);
extern void ov07_0221C448(void *, void *);
extern void ParticleSystem_DeleteEmitter(void *, void *);
extern s32 GF_SinDeg(u16);

extern void *BattleAnimUtil_AllocWork(void *, u32);
extern void BattleAnimUtil_InitCommonWork(void *, void *);

extern u32 ov07_0221C4A8(void *p, int i);
extern u16 ov07_0221C468(void *);
extern u16 ov07_0221C470(void *);
extern void ov07_02231B90(void *, int, int);
extern SPLEmitter *ov07_0221C494(void *, int);
extern void *ov07_0221C488(void *, int);
extern void ov07_022220FC(void *, u16, u16, u16, u16, s32, s32, s32);
extern void *ov07_0221C410(void *, void (*)(void *, void *), void *);

typedef struct Ov07Work94Init {
    s32 arg0;
    s32 arg1;
    s32 arg3;
    s32 arg2;
    s32 arg4;
    s32 arg5;
    s32 arg6;
    s32 arg7;
    s32 arg8;
    u32 handle;
    u8 filler_28[0xC];
    void *resource;
    SPLEmitter *node;
    u8 filler_3C[4];
    void *owner;
    u8 filler_44[0x14];
    Ov07Interp interp;
    u8 filler_7C[0xC];
    VecFx32 base;
} Ov07Work94Init;

extern void ov07_0222600C(void *, Ov07Work104 *);
extern s32 ov07_02222004(void *, int);
extern void ov07_02222268(W02LinearMotion *, s16, s16, s16, s16, u16);
extern void ov07_022223F0(W02ScalarMotion *, s32, s32, s32);

extern void ov07_02222338(W02LinearMotion *, W02SinMotion *, s16, s16, s16, s16, u16, s32);

extern void *ov07_0221C478(void *);
extern void ov07_02231DD0(VecFx32 *);

extern void ov07_022260F8(void *, void *);

void ov07_022261B4(void *owner) {
    Ov07Work104 *work = BattleAnimUtil_AllocWork(owner, sizeof(Ov07Work104));
    VecFx32 a;
    VecFx32 b;
    s32 direction;
    s32 x1;
    s32 x2;
    s32 y1;
    s32 y2;
    int i;

    BattleAnimUtil_InitCommonWork(owner, &work->header);
    ov07_0222600C(owner, work);
    direction = ov07_02222004(owner, ov07_0221C468(owner));
    ov07_02231B90(owner, work->handleA, (int)&a);
    ov07_02231B90(owner, work->handleB, (int)&b);
    x1 = a.x / 0xAC;
    x2 = b.x / 0xAC;
    y1 = a.y / 0xAC;
    y2 = b.y / 0xAC;
    ov07_02222268(&work->interpA.motion, (s16)x1, (s16)(x2 + work->argC * direction), (s16)y1, (s16)(y2 + work->argE * direction), work->arg6);
    if (direction > 0) {
        ov07_022223F0(&work->interpC.motion, 0xE38 * direction, 0x5C71 * direction, 10);
    } else {
        ov07_022223F0(&work->interpC.motion, 0x3FFF * direction, 0x5C71 * direction, 10);
    }
    for (i = 0; i < work->unk20; i++) {
        ov07_022222B4(&work->interpA.motion);
    }
    if (work->unk24 != 0xFF) {
        work->unk1C = work->unk24 + 1;
    }
    SPL_SetEmitterPositionX(work->node, work->interpA.motion.x * 0xAC);
    SPL_SetEmitterPositionY(work->node, work->interpA.motion.y * 0xAC);
    ov07_0221C410(work->header.owner, ov07_022260F8, work);
}

void ov07_022262E8(void *owner) {
    Ov07Work104 *work = BattleAnimUtil_AllocWork(owner, sizeof(Ov07Work104));
    VecFx32 a;
    VecFx32 b;
    s32 mode;
    u32 packed;
    s32 x1;
    s32 x2;
    s32 y1;
    s32 y2;
    int i;

    BattleAnimUtil_InitCommonWork(owner, &work->header);
    work->arg0 = ((s32)ov07_0221C4A8(owner, 0));
    work->arg10 = ((s32)ov07_0221C4A8(owner, 1));
    mode = ((s32)ov07_0221C4A8(owner, 2));
    work->arg6 = ((s32)ov07_0221C4A8(owner, 3));
    work->delay = ((s32)ov07_0221C4A8(owner, 4));
    work->counter = 0;
    packed = ((s32)ov07_0221C4A8(owner, 5));
    work->unk1C = 0;
    work->unk20 = (packed & 0xFFFF0000) >> 16;
    work->unk24 = (u16)packed;
    if (work->unk20 == 0) {
        work->unk20 = 0;
    }
    if (work->unk24 == 0) {
        work->unk24 = 0xFF;
    }
    work->node = ov07_0221C494(owner, work->arg0);
    work->resource = ov07_0221C478(owner);
    if (work->arg10 == 0) {
        work->handleA = ov07_0221C468(owner);
        work->handleB = ov07_0221C468(owner);
    } else {
        work->handleA = ov07_0221C470(owner);
        work->handleB = ov07_0221C470(owner);
    }
    if (work->node == NULL) {
        GF_AssertFail();
    }
    ov07_02231B90(owner, work->handleA, (int)&a);
    ov07_02231B90(owner, work->handleB, (int)&b);
    if (mode == 0) {
        ov07_02231DD0(&a);
        a.x = b.x;
    } else {
        ov07_02231DD0(&b);
        b.x = a.x;
    }
    x1 = a.x / 0xAC;
    x2 = b.x / 0xAC;
    y1 = a.y / 0xAC;
    y2 = b.y / 0xAC;
    ov07_02222268(&work->interpA.motion, (s16)x1, (s16)x2, (s16)y1, (s16)y2, work->arg6);
    for (i = 0; i < work->unk20; i++) {
        ov07_022222B4(&work->interpA.motion);
    }
    if (work->unk24 != 0xFF) {
        work->unk1C = work->unk24 + 1;
    }
    SPL_SetEmitterPositionX(work->node, work->interpA.motion.x * 0xAC);
    SPL_SetEmitterPositionY(work->node, work->interpA.motion.y * 0xAC);
    ov07_0221C410(work->header.owner, ov07_022260F8, work);
}

void ov07_0222647C(void *task, Ov07Work104 *work) {
    BOOL active = ov07_022260DC(work->node);

    if (!ov07_02222384(&work->interpA.motion, &work->interpB.motion) && !active) {
        ParticleSystem_DeleteEmitter(work->resource, work->node);
        ov07_0221C448(work->header.owner, task);
        BattleAnimUtil_FreeWork(work);
        return;
    }
    SPL_SetEmitterPositionX(work->node, work->interpA.motion.x * 0xAC);
    SPL_SetEmitterPositionY(work->node, work->interpA.motion.y * 0xAC);
}

void ov07_022264E0(void *owner) {
    Ov07Work104 *work = BattleAnimUtil_AllocWork(owner, sizeof(Ov07Work104));
    VecFx32 a;
    VecFx32 b;
    s32 direction;
    s32 x1;
    s32 x2;
    s32 y1;
    s32 y2;
    int i;

    BattleAnimUtil_InitCommonWork(owner, &work->header);
    ov07_0222600C(owner, work);
    direction = ov07_02222004(owner, ov07_0221C468(owner));
    ov07_02231B90(owner, work->handleA, (int)&a);
    ov07_02231B90(owner, work->handleB, (int)&b);
    x1 = a.x / 0xAC;
    x2 = b.x / 0xAC;
    y1 = a.y / 0xAC;
    y2 = b.y / 0xAC;
    ov07_02222338(&work->interpA.motion, &work->interpB.motion, (s16)x1, (s16)(x2 + work->argC * direction), (s16)y1, (s16)(y2 + work->argE * direction), work->arg6, -(work->arg8 << 12));
    if (direction > 0) {
        ov07_022223F0(&work->interpC.motion, 0xE38 * direction, 0x5C71 * direction, 10);
    } else {
        ov07_022223F0(&work->interpC.motion, 0x3FFF * direction, 0x5C71 * direction, 10);
    }
    for (i = 0; i < work->unk20; i++) {
        ov07_02222384(&work->interpA.motion, &work->interpB.motion);
    }
    if (work->unk24 != 0xFF) {
        work->unk1C = work->unk24 + 1;
    }
    SPL_SetEmitterPositionX(work->node, work->interpA.motion.x * 0xAC);
    SPL_SetEmitterPositionY(work->node, work->interpA.motion.y * 0xAC);
    ov07_0221C410(work->header.owner, (void (*)(void *, void *))ov07_0222647C, work);
}

void ov07_02226628(void *task, Ov07Work94 *work) {
    BOOL active = ov07_022260DC(work->node);

    if (!ov07_02222180(&work->interp) && !active) {
        ParticleSystem_DeleteEmitter(work->resource, work->node);
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
        return;
    }
    {
        s32 y = work->baseY + work->interp.y * 0xAC;
        s32 x = work->baseX + work->interp.x * 0xAC;
        SPL_SetEmitterPositionX(work->node, x);
        SPL_SetEmitterPositionY(work->node, y);
    }
}

void ov07_02226698(void *owner) {
    Ov07Work94Init *work = BattleAnimUtil_AllocWork(owner, sizeof(Ov07Work94Init));
    s32 a;
    s32 b;
    s32 c;
    s32 d;

    BattleAnimUtil_InitCommonWork(owner, (u8 *)work + 0x3C);
    work->arg0 = ((s32)ov07_0221C4A8(owner, 0));
    work->arg1 = ((s32)ov07_0221C4A8(owner, 1));
    work->arg2 = ((s32)ov07_0221C4A8(owner, 2));
    work->arg3 = ((s32)ov07_0221C4A8(owner, 3));
    work->arg4 = ((s32)ov07_0221C4A8(owner, 4));
    work->arg5 = ((s32)ov07_0221C4A8(owner, 5));
    work->arg6 = ((s32)ov07_0221C4A8(owner, 6));
    work->arg7 = ((s32)ov07_0221C4A8(owner, 7));
    work->arg8 = ((s32)ov07_0221C4A8(owner, 8));
    if (work->arg8 == 0) {
        work->handle = ov07_0221C468(owner);
    } else {
        work->handle = ov07_0221C470(owner);
    }
    ov07_02231B90(owner, work->handle, (int)&work->base);
    work->node = ov07_0221C494(owner, work->arg0);
    work->resource = ov07_0221C488(owner, ((s32)ov07_0221C4A8(owner, 9)));
    a = work->arg1 * 0xFFFF / 360;
    b = work->arg2 * 0xFFFF / 360;
    c = work->arg3 * 0xFFFF / 360;
    d = work->arg4 * 0xFFFF / 360;
    ov07_022220FC(&work->interp, (u16)a, (u16)b, (u16)c, (u16)d, work->arg5 << 12, work->arg6 << 12, work->arg7);
    ov07_02222180(&work->interp);
    {
        s32 y = work->base.y + work->interp.y * 0xAC;
        s32 x = work->base.x + work->interp.x * 0xAC;
        SPL_SetEmitterPositionX(work->node, x);
        SPL_SetEmitterPositionY(work->node, y);
    }
    ov07_0221C410(work->owner, (void (*)(void *, void *))ov07_02226628, work);
}
