#include "field_effect_scale_task_internal.h"

void *ov01_021FED9C(void *owner) {
    void *work = FieldEffectManager_HeapAllocAndFill(owner, 0x3c, 0, 0);
    *(void **)work = owner;
    ov01_021FEDC8(work);
    return work;
}

void ov01_021FEDB8(void *work) {
    ov01_021FEDF0(work);
    FieldEffectManager_HeapFree(work);
}

void ov01_021FEDC8(void *work) {
    u8 *p = work;
    ov01_021F19F4(*(void **)p, p + 4, 0, 0x1e, 0);
    ov01_021F1A18(*(void **)p, p + 0x18, 0, 0x87, 0);
}

void ov01_021FEDF0(void *work) {
    u8 *p = work;
    RenderModel_Free(p + 4);
    RenderAnimation_Free(p + 0x18);
}

void ov01_021FEE04(void *mapObject, s32 x, s32 offset, s32 z) {
    EffectContextPrivate context;
    VecFx32Private position;
    context.manager = FieldEffectManager_GetFromMapObject(mapObject);
    context.work = FieldEffectManager_GetRendererData(context.manager, 7);
    sub_020611C8(x, z, &position);
    position.z += offset * 8;
    position.y = MapObject_GetPreviousYCoord(mapObject) * 0x8000 - 0x8000;
    FieldEffectManager_CreateEffect(context.manager, ov01_02209124, &position, 0, &context, MapObject_GetPriorityPlusValue(mapObject, 2));
}

BOOL ov01_021FEE64(void *object, void *data) {
    EffectWorkPrivate *p = data;
    TwoWordsPrivate *source = sub_02068D98(object);
    p->source = *source;
    ov01_021F1A34(p->source.word0, p, p->source.word4 + 4, p->source.word4 + 0x18, 0);
    RenderObj_InitWithAnimation(p->fields + 0x24, p->source.word4 + 4, p);
    return TRUE;
}

void ov01_021FEE9C(void *unused, void *data) {
    RenderAnimation_Free(data);
}

void ov01_021FEEA8(void *task, void *data) {
    if (RenderAnimation_IsDone(data) == 1) {
        FieldEffectManager_DestroyEffect(task);
    } else {
        RenderAnimation_Advance(data, 0x1000, 0);
    }
}

void ov01_021FEED0(void *object, void *data) {
    VecFx32Private position;
    sub_02068DB8(object, &position);
    RenderObj_DrawAt((u8 *)data + 0x24, &position);
}
