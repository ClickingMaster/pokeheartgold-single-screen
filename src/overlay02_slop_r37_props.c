#include "overlay02_slop_r3_private.h"

void ov02_0224CDB0(FieldSystem *fieldSystem, u8 a1) {
    int outObj;
    void *outHandle;
    VecFx32 tileCenter;
    VecFx32 local;
    void *data;
    if (sub_02054C20(fieldSystem, 0x26, &outObj, &outHandle)) {
        data = Heap_AllocAtEnd(HEAP_ID_FIELD1, 0x18);
        *(u8 *)((u8 *)data + 0xc) = a1;
        *(u8 *)((u8 *)data + 0xd) = 0;
        *(u8 *)((u8 *)data + 0xe) = 0;
        *(u8 *)((u8 *)data + 0xf) = 0;
        sub_02054DC8((int)outHandle, MapMatrix_GetWidth(fieldSystem->mapMatrix), &tileCenter);
        MapProp_GetTranslation(&local, (void *)outObj);
        *(VecFx32 *)data = local;
        ((VecFx32 *)data)->x += tileCenter.x;
        ((VecFx32 *)data)->z += tileCenter.z;
        TaskManager_Call(fieldSystem->taskman, ov02_0224CE28, data);
    } else {
        GF_AssertFail();
    }
}

WIP_LOCAL BOOL ov02_0224CE28(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    void *env = TaskManager_GetEnvironment(taskManager);

    switch (*(u8 *)((u8 *)env + 0xf)) {
    case 0: {
        NNSG3dResMdlSet *mdlSet = NNS_G3dGetMdlSet(*(NNSG3dResFileHeader **)ov01_021FB90C(0x6b, *(void **)((u8 *)fieldSystem + 0x34)));
        NNSG3dResMdl *mdl = NNS_G3dGetMdlByIdx(mdlSet, 0);
        void *v = ov01_021FB9E0(*(void **)((u8 *)fieldSystem + 0x34));
        MapPropOneShotAnimationManager_LoadPropAnimations(*(void **)((u8 *)fieldSystem + 0x54), *(void **)((u8 *)fieldSystem + 0x58), 0x10, (void *)0x6b, NULL, mdl, v, 1, 1, 0);
        (*(u8 *)((u8 *)env + 0xf))++;
        break;
    }
    case 1: {
        VecFx32 vec24;
        VecFx32 vec18;
        int *p = (int *)&vec18;
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
        vec24.x = ((VecFx32 *)env)->x + ov02_02253DD8[*(u8 *)((u8 *)env + 0xd)].x;
        vec24.y = ((VecFx32 *)env)->y + ov02_02253DD8[*(u8 *)((u8 *)env + 0xd)].y;
        vec24.z = ((VecFx32 *)env)->z + ov02_02253DD8[*(u8 *)((u8 *)env + 0xd)].z;
        PlaySE(SEQ_SE_DP_BOWA);
        *(u8 *)((u8 *)env + *(u8 *)((u8 *)env + 0xd) + 0x10) = MapPropManager_LoadOne(*(void **)((u8 *)fieldSystem + 0x9c), 0x6b, &vec24, &vec18, *(void **)((u8 *)fieldSystem + 0x54));
        (*(u8 *)((u8 *)env + 0xf))++;
        break;
    }
    case 2:
        if (*(u8 *)((u8 *)env + 0xe) < 0xf) {
            (*(u8 *)((u8 *)env + 0xe))++;
        } else {
            *(u8 *)((u8 *)env + 0xe) = 0;
            (*(u8 *)((u8 *)env + 0xd))++;
            if (*(u8 *)((u8 *)env + 0xd) < *(u8 *)((u8 *)env + 0xc)) {
                *(u8 *)((u8 *)env + 0xf) = 1;
            } else {
                MapPropOneShotAnimationManager_SetAnimationRenderObj(*(void **)((u8 *)fieldSystem + 0x58), 0x10, 0, MapProp_GetRenderSurface(MapPropManager_GetMapPropByIndex_Checked_RequireActive(*(void **)((u8 *)fieldSystem + 0x9c), *(u8 *)((u8 *)env + 0x10))));
                (*(u8 *)((u8 *)env + 0xf))++;
            }
        }
        break;
    case 3:
        MapPropOneShotAnimationManager_PlayAnimation(*(void **)((u8 *)fieldSystem + 0x58), 0x10, 0);
        (*(u8 *)((u8 *)env + 0xf))++;
        break;
    case 4:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(*(void **)((u8 *)fieldSystem + 0x58), 0x10)) {
            u8 i;
            MapPropOneShotAnimationManager_UnloadAnimation(*(void **)((u8 *)fieldSystem + 0x54), *(void **)((u8 *)fieldSystem + 0x58), 0x10);
            for (i = 0; i < *(u8 *)((u8 *)env + 0xc); i++) {
                MapPropManager_RemoveMapPropByIndex(*(u8 *)((u8 *)env + i + 0x10), *(void **)((u8 *)fieldSystem + 0x9c));
            }
            (*(u8 *)((u8 *)env + 0xf))++;
        }
        break;
    case 5:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}

WIP_LOCAL void ov02_0224CFD8(void *a0, int a1, void *data) {
    VecFx32 vec;
    LocalMapObject *obj = MapObjectManager_GetFirstActiveObjectByID(a0, a1);
    u32 x;
    u32 z;
    MapObject_CopyPositionVector(obj, &vec);
    x = MapObject_GetXCoord(obj);
    z = MapObject_GetZCoord(obj);
    switch (MapObject_GetFacingDirection(obj)) {
    case 0:
        z--;
        break;
    case 1:
        z++;
        break;
    case 2:
        x--;
        break;
    case 3:
        x++;
        break;
    default:
        GF_AssertFail();
        break;
    }
    Field3dObject_SetPosEx(data, (x << 0x10) + 0x8000, vec.y, (z << 0x10) + 0x8000);
}

WIP_LOCAL void ov02_0224D044(void *a0, void *data) {
    VecFx32 vec;
    u32 x;
    u32 z;
    PlayerAvatar_CopyPositionVector(a0, &vec);
    x = PlayerAvatar_GetXCoord(a0);
    z = PlayerAvatar_GetZCoord(a0);
    switch (PlayerAvatar_GetFacingDirection(a0)) {
    case 0:
        z--;
        break;
    case 1:
        z++;
        break;
    case 2:
        x--;
        break;
    case 3:
        x++;
        break;
    default:
        GF_AssertFail();
        break;
    }
    Field3dObject_SetPosEx(data, (x << 0x10) + 0x8000, vec.y, (z << 0x10) + 0x8000);
}
