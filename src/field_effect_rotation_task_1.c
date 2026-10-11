#include "field_effect_rotation_task_internal.h"

void *ov01_021FEC38(u32 context) {
    Ov01EffectManager *manager = FieldEffectManager_HeapAllocAndFill(context, 0x3C, 0, 0);
    manager->context = (void *)context;
    ov01_021FEC64(manager);
    return manager;
}

void ov01_021FEC54(void *manager) {
    ov01_021FEC8C(manager);
    FieldEffectManager_HeapFree(manager);
}

void ov01_021FEC64(Ov01EffectManager *manager) {
    ov01_021F19F4(manager->context, manager->primary, 0, 0x1D, 0);
    ov01_021F1A18(manager->context, manager->secondary, 0, 0x86, 0);
}

void ov01_021FEC8C(Ov01EffectManager *manager) {
    RenderModel_Free(manager->primary);
    RenderAnimation_Free(manager->secondary);
}

void ov01_021FECA0(void *mapObject, void *arg, u32 variant, u32 direction) {
    Ov01SpawnArgs args;
    Ov01Vec3 position;

    if (MapObject_TestFlagsBits(mapObject, 0x200) == TRUE) {
        return;
    }
    args.context = FieldEffectManager_GetFromMapObject(mapObject);
    args.renderer = FieldEffectManager_GetRendererData(args.context, 6);
    sub_020611C8(arg, direction, &position);
    position.z += variant * 8 + 0x1000;
    position.y = (MapObject_GetPreviousYCoord(mapObject) << 15) - 0x8000;
    FieldEffectManager_CreateEffect(args.context, ov01_02209110, &position, 0, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
}

BOOL ov01_021FED14(void *task, Ov01EffectState *state) {
    Ov01TaskArgs *args = sub_02068D98(task);
    state->args = *args;
    ov01_021F1A34(state->args.context, state, (u8 *)state->args.resource + 4, (u8 *)state->args.resource + 0x18, 0);
    RenderObj_InitWithAnimation((u8 *)state + 0x24, (u8 *)state->args.resource + 4, state);
    return TRUE;
}

void ov01_021FED4C(void *unused, void *state) {
    RenderAnimation_Free(state);
}

void ov01_021FED58(void *task, void *state) {
    if (RenderAnimation_IsDone(state) == TRUE) {
        FieldEffectManager_DestroyEffect(task);
        return;
    }
    RenderAnimation_Advance(state, 0x1000, 0);
}

void ov01_021FED80(void *task, void *state) {
    Ov01Vec3 position;
    sub_02068DB8(task, &position);
    RenderObj_DrawAt((u8 *)state + 0x24, &position);
}
