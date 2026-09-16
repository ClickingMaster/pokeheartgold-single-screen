#include "overlay02_slop_r3_private.h"

WIP_LOCAL BOOL ov02_0224BE24(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    void *env = TaskManager_GetEnvironment(taskManager);

    switch (*(u8 *)((u8 *)env + 2)) {
    case 0: {
        int outObj;
        NNSG3dResMdlSet *mdlSet = NNS_G3dGetMdlSet(*(NNSG3dResFileHeader **)ov01_021FB90C(0xd0, *(void **)((u8 *)fieldSystem + 0x34)));
        NNSG3dResMdl *mdl = NNS_G3dGetMdlByIdx(mdlSet, 0);
        void *v5;
        void *v6;
        if (sub_02054C20(fieldSystem, 0xd0, &outObj, NULL) == 0) {
            GF_AssertFail();
        }
        v5 = MapProp_GetRenderSurface((void *)outObj);
        v6 = ov01_021FB9E0(*(void **)((u8 *)fieldSystem + 0x34));
        MapPropOneShotAnimationManager_LoadPropAnimations(*(void **)((u8 *)fieldSystem + 0x54), *(void **)((u8 *)fieldSystem + 0x58), 1, (void *)0xd0, v5, mdl, v6, 2, *(u8 *)env, 0);
        (*(u8 *)((u8 *)env + 2))++;
        break;
    }
    case 1:
        GF_ASSERT(*(u8 *)((u8 *)env + 1) == 0 || *(u8 *)((u8 *)env + 1) == 1);
        MapPropOneShotAnimationManager_PlayAnimation(*(void **)((u8 *)fieldSystem + 0x58), 1, *(u8 *)((u8 *)env + 1));
        PlaySE(SEQ_SE_DP_ELEBETA2);
        (*(u8 *)((u8 *)env + 2))++;
        break;
    case 2:
        if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(*(void **)((u8 *)fieldSystem + 0x58), 1)) {
            StopSE(SEQ_SE_DP_ELEBETA2, 0);
            PlaySE(SEQ_SE_DP_PINPON);
            MapPropOneShotAnimationManager_UnloadAnimation(*(void **)((u8 *)fieldSystem + 0x54), *(void **)((u8 *)fieldSystem + 0x58), 1);
            (*(u8 *)((u8 *)env + 2))++;
        }
        break;
    case 3:
        if (!IsSEPlaying(SEQ_SE_DP_PINPON)) {
            (*(u8 *)((u8 *)env + 2))++;
        }
        break;
    case 4:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}
