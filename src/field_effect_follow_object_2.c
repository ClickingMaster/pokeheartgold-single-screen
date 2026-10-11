#include "field_effect_follow_object_internal.h"

BOOL ov01_021FF174(void *a0, FieldEffectFollowWork *work) {
    VecFx32 pos;

    work->args = *(FieldEffectFollowArgs *)sub_02068D98(a0);
    work->spriteId = MapObject_GetSpriteID(work->args.object);
    work->objId = MapObject_GetID(work->args.object);
    work->mapId = MapObject_GetMapID(work->args.object);
    pos.x = work->args.x << 16;
    pos.z = work->args.z << 16;
    pos.y = MapObject_GetPositionVectorYCoord(work->args.object);
    work->unk14 = sub_0206121C(work->args.taskman, &pos);
    pos.x = pos.x + (2 << 14);
    pos.z = pos.z + (0x12 << 12);
    sub_02068DA8(a0, &pos);
    work->sprite = FieldEffectManager_CreateBillboardFromRecord(work->args.fieldSys, 0, &pos);
    if (MapObject_TestFlagsBits(work->args.object, MAPOBJECTFLAG_VISIBLE) == 1) {
        BillboardObject_SetVisible(work->sprite, 0);
    }
    if (sub_02068D90(a0) == 0) {
        BillboardObject_SetFrame(work->sprite, 3 << 14);
        work->state = 2;
    }
    return TRUE;
}

BOOL ov01_021FF228(void *a0, FieldEffectFollowWork *work) {
    return BillboardObject_Delete(work->sprite);
}

void ov01_021FF234(void *a0, FieldEffectFollowWork *work) {
    VecFx32 cur;
    VecFx32 pos;
    LocalMapObject *object = work->args.object;
    int x;
    int z;

    if (!sub_0205F0F8(object, work->spriteId, work->objId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)a0);
        return;
    }
    if (MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) == 1) {
        BillboardObject_SetVisible(work->sprite, 0);
    } else {
        BillboardObject_SetVisible(work->sprite, 1);
    }
    if (work->unk14 == 0) {
        sub_02068DB8(a0, &cur);
        pos.x = work->args.x << 16;
        pos.z = work->args.z << 16;
        pos.y = cur.y;
        work->unk14 = sub_0206121C(work->args.taskman, &pos);
        if (work->unk14 == 1) {
            cur.y = pos.y;
            sub_02068DA8(a0, &cur);
        }
    }
    switch (work->state) {
    case 0:
        if (BillboardObject_AdvanceFrame(work->sprite, 1 << 12) == 1) {
        }
        if (BillboardObject_GetSequenceFrame(work->sprite) / FX32_ONE >= 0xc) {
            work->state = 1;
        }
        return;
    case 1:
        BillboardObject_SetFrame(work->sprite, 3 << 14);
        work->state = 2;
    case 2:
        if (!sub_0205F0F8(object, work->spriteId, work->objId, work->mapId)) {
            FieldEffectManager_DestroyEffect((int)a0);
            return;
        }
        x = MapObject_GetXCoord(object) - work->args.dx;
        z = MapObject_GetZCoord(object) - work->args.dz;
        if (work->args.x != x || work->args.z != z) {
            FieldEffectManager_DestroyEffect((int)a0);
            return;
        }
        if (work->args.dir == -1) {
            return;
        }
        if (work->args.dir == MapObject_GetFacingDirection(object)) {
            return;
        }
        FieldEffectManager_DestroyEffect((int)a0);
        return;
    }
}

void ov01_021FF35C(void *a0, FieldEffectFollowWork *work) {
    VecFx32 pos;

    if (!sub_0205F0F8(work->args.object, work->spriteId, work->objId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)a0);
        return;
    }
    sub_02068DB8(a0, &pos);
    BillboardObject_SetPosition(work->sprite, &pos);
}
