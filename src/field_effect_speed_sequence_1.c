#include "field_effect_speed_sequence_internal.h"
typedef struct LocalThreeWords {
    u32 v[3];
} LocalThreeWords;
typedef struct LocalFiveWords {
    fx32 v[5];
} LocalFiveWords;
extern const u32 ov01_02209084[], ov01_022090AC[];
LocalManager *ov01_021FE590(void *owner) {
    LocalManager *m = FieldEffectManager_HeapAllocAndFill(owner, 12, 0, 0);
    m->owner = owner;
    return m;
}
void ov01_021FE5A4(LocalManager *m) {
    ov01_021FE61C(m);
    FieldEffectManager_HeapFree(m);
}
void ov01_021FE5B4(LocalManager *m) {
    m->refs++;
}
void ov01_021FE5BC(LocalManager *m) {
    m->refs--;
    if (m->refs < 0) {
        GF_AssertFail();
    }
}
void ov01_021FE5CC(LocalManager *m) {
    if (m->active == 0) {
        m->active = 1;
        FieldEffectManager_LoadBillboardResource(m->owner, 11, 0x80);
        FieldEffectManager_LoadBillboardSequence(m->owner, 11, 0x95);
        FieldEffectManager_LoadTexResource(m->owner, 12, 0x1a, 1);
        FieldEffectManager_AddRecord(m->owner, 13, 11, 11, 12, 0, ov01_022090AC);
    }
}
void ov01_021FE61C(LocalManager *m) {
    if (m->active == 1) {
        m->active = 0;
        FieldEffectManager_FreeBillboardResource(m->owner, 11);
        FieldEffectManager_FreeBillboardSequence(m->owner, 11);
        FieldEffectManager_FreeTexResource(m->owner, 12);
        FieldEffectManager_ReleaseRecord(m->owner, 13);
    }
}
void ov01_021FE64C(LocalManager *m) {
    if (m->refs == 0) {
        ov01_021FE5CC(m);
    }
}
void ov01_021FE65C(LocalManager *m) {
    if (m->refs == 0) {
        ov01_021FE61C(m);
    }
}
void ov01_021FE66C(void *obj) {
    struct {
        LocalManager *manager;
        void *resource;
        u32 pad10;
        VecFx32 pos;
    } local;
    LocalManager *m = FieldEffectManager_GetFromMapObject(obj);
    ov01_021F93AC(obj, &local.pos);
    local.pos.z += 2 << 14;
    local.manager = m;
    local.resource = FieldEffectManager_GetRendererData(m, 16);
    FieldEffectManager_CreateEffect(m, ov01_02209084, &local.pos, 0, &local, 0xff);
}
BOOL ov01_021FE6B4(void *task, LocalState *s) {
    VecFx32 pos;
    *(LocalThreeWords *)&s->resource = *(LocalThreeWords *)FieldEffect_GetUserData(task);
    ov01_021FE64C(s->manager);
    FieldEffect_GetPosition(task, &pos);
    s->effect = FieldEffectManager_CreateBillboardFromRecord(s->resource, 13, &pos);
    ov01_021FE5B4(s->manager);
    return TRUE;
}
void ov01_021FE6F4(void *task, LocalState *s) {
    BillboardObject_Delete(s->effect);
    ov01_021FE5BC(s->manager);
    ov01_021FE65C(s->manager);
}
void ov01_021FE70C(void *task, LocalState *s) {
    static const LocalFiveWords init = {
        { FX32_ONE, FX32_ONE, 2 * FX32_ONE, 2 * FX32_ONE, FX32_ONE }
    };
    LocalFiveWords values = init;
    if (s->finished == 1) {
        s->finished = 0;
        s->index++;
        if (s->index >= 5) {
            FieldEffectManager_DestroyEffect(task);
            return;
        }
        BillboardObject_SetFrame(s->effect, 0);
    }
    if (BillboardObject_AdvanceFrame(s->effect, values.v[s->index]) == TRUE) {
        s->finished = 1;
    }
}
void ov01_021FE768(void *task, LocalState *s) {
    VecFx32 pos;
    FieldEffect_GetPosition(task, &pos);
    BillboardObject_SetPosition(s->effect, &pos);
}
