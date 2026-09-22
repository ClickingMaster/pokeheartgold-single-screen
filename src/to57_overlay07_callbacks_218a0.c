#include "to57_overlay07_callbacks_218a0_private.h"

BOOL ov07_022218A0(SPLEmitter *emitter, Ov07ParticleCallbackWork *work) {
    Ov07ParticleParam param;
    VecFx32 value = { 0 };

    ov07_022217A4(work, &param);
    work->scale = ov07_02221664(work);
    if (param.forcePositive == 1) {
        work->scale = 1;
    }
    switch (param.mode) {
    case 0:
        sub_02015640(emitter, &value);
        value.x *= work->scale;
        value.y *= work->scale;
        value.z *= work->scale;
        break;
    case 1:
        value.x = param.value.x;
        value.y = param.value.y;
        value.z = param.value.z;
        value.x *= work->scale;
        value.y *= work->scale;
        value.z *= work->scale;
        break;
    case 2:
        ov07_02231B90(work->context, work->indexA, &value);
        break;
    case 3:
        ov07_02231B90(work->context, work->indexB, &value);
        break;
    case 0xff:
        break;
    }
    value.x -= work->baseX;
    value.y -= work->baseY;
    sub_02015628(emitter, &value);
    return TRUE;
}

BOOL ov07_02221968(SPLEmitter *emitter, Ov07ParticleCallbackWork *work) {
    s16 value = 0;
    Ov07ParticleParam param;

    ov07_022217A4(work, &param);
    work->scale = ov07_02221664(work);
    if (param.forcePositive == 1) {
        work->scale = 1;
    }
    switch (param.mode) {
    case 0:
        sub_0201568C(emitter, &value);
        value *= work->scale;
        break;
    case 1:
        value = param.value.x;
        value *= work->scale;
        break;
    case 2:
    case 3:
    case 0xff:
        break;
    }
    sub_02015674(emitter, &value);
    return TRUE;
}

BOOL ov07_022219EC(SPLEmitter *emitter, Ov07ParticleCallbackWork *work) {
    u16 value = 0;
    Ov07ParticleParam param;

    ov07_022217A4(work, &param);
    work->scale = ov07_02221664(work);
    if (param.forcePositive == 1) {
        work->scale = 1;
    }
    switch (param.mode) {
    case 0:
        sub_020156BC(emitter, &value);
        value *= work->scale;
        break;
    case 1:
        value = param.value.x;
        value *= work->scale;
        break;
    case 2:
    case 3:
    case 0xff:
        break;
    }
    sub_020156A8(emitter, &value);
    return TRUE;
}

BOOL ov07_02221A6C(SPLEmitter *emitter, Ov07ParticleCallbackWork *work) {
    u16 value = 0;
    Ov07ParticleParam param;

    ov07_022217A4(work, &param);
    work->scale = ov07_02221664(work);
    if (param.forcePositive == 1) {
        work->scale = 1;
    }
    switch (param.mode) {
    case 0:
        sub_020156EC(emitter, &value);
        break;
    case 1:
        value = param.value.x;
        break;
    case 2:
    case 3:
    case 0xff:
        break;
    }
    sub_020156D8(emitter, &value);
    return TRUE;
}
