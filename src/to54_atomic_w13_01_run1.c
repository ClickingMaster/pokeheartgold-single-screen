#include "to54_atomic_w13_private.h"

static inline void SetPosition(Ov07CallbackPosition *work, const VecFx32 *offset) {
    work->position.x = offset->x + (*work->base)->position.x;
    work->position.y = offset->y + (*work->base)->position.y;
    work->position.z = offset->z + (*work->base)->position.z;
}

void ov07_0221FF78(Ov07CallbackPosition *work) {
    CameraAngle angle;
    VecFx32 offset;
    VecFx32 unusedOffset;
    VecFx32 up;
    void *ctx = ParticleSystem_GetEmitterParam();
    int a = ov07_0221C468(ctx);
    ov07_0221C470(ctx);
    SPLEmitter *emitter = ov07_0221C478(ctx);
    if (BattleAnimUtil_GetBattlerSide(ctx, 1) == 3) {
        ov07_02231B90(ctx, 1, &offset);
    } else {
        int index = BattleAnimUtil_GetBattlerType(ctx, 1);
        int other = BattleAnimUtil_GetOpposingBattlerType(index);
        int d = ParticleSystem_GetPerspectiveType(emitter);
        void *b = ov07_0221BFC0(ctx);
        ov07_02231AD0(other, &offset, b, d);
        ov07_02231AD0(index, &unusedOffset, b, d);
    }
    SetPosition(work, &offset);
    BattleAnimUtil_GetCameraUp(1, &up);
    Camera *camera = ParticleSystem_GetCamera(emitter);
    angle = Camera_GetAngle(camera);
    Camera_SetAnglePos(&angle, camera);
}

void ov07_02220030(Ov07CallbackPosition *work) {
    VecFx32 offset;
    VecFx32 up;
    void *ctx = ParticleSystem_GetEmitterParam();
    int a = ov07_0221C470(ctx);
    SPLEmitter *emitter = ov07_0221C478(ctx);
    ov07_02231B90(ctx, 0, &offset);
    SetPosition(work, &offset);
    BattleAnimUtil_GetCameraUp(a, &up);
    ParticleSystem_SetCameraUp(emitter, &up);
}

void ov07_0222008C(Ov07CallbackPosition *work) {
    VecFx32 offset;
    void *ctx = ParticleSystem_GetEmitterParam();
    int a = ov07_0221C470(ctx);
    ov07_02231B90(ctx, a, &offset);
    SetPosition(work, &offset);
}

void ov07_022200D0(Ov07CallbackPosition *work) {
    VecFx32 offset;
    void *ctx = ParticleSystem_GetEmitterParam();
    int a = ov07_0221C468(ctx);
    ov07_02231B90(ctx, a, &offset);
    SetPosition(work, &offset);
}

void ov07_02220114(Ov07CallbackPosition *work) {
    VecFx32 offset;
    void *ctx = ParticleSystem_GetEmitterParam();
    int a = ov07_0221C468(ctx);
    int b = ov07_0221C470(ctx);
    ov07_02231B90(ctx, a, &offset);
    (void)b;
    SetPosition(work, &offset);
}

void ov07_02220160(Ov07CallbackPosition *work) {
    VecFx32 offset;
    void *ctx = ParticleSystem_GetEmitterParam();
    SPLEmitter *emitter = ov07_0221C478(ctx);
    int d = ParticleSystem_GetPerspectiveType(emitter);
    int a = ov07_0221C468(ctx);
    int mode = BattleAnimUtil_GetBattlerSide(ctx, a) == 3 ? 0 : 1;
    void *b = ov07_0221BFC0(ctx);
    ov07_02231AD0(mode, &offset, b, d);
    SetPosition(work, &offset);
}

void ov07_022201C8(Ov07CallbackPosition *work) {
    VecFx32 offset;
    void *ctx = ParticleSystem_GetEmitterParam();
    SPLEmitter *emitter = ov07_0221C478(ctx);
    int d = ParticleSystem_GetPerspectiveType(emitter);
    int a = ov07_0221C470(ctx);
    int mode = BattleAnimUtil_GetBattlerSide(ctx, a) == 3 ? 0 : 1;
    void *b = ov07_0221BFC0(ctx);
    ov07_02231AD0(mode, &offset, b, d);
    SetPosition(work, &offset);
}
