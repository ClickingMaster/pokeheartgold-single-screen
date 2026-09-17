#include "overlay14_lifecycle_private.h"

BOOL PCBox_Init(OverlayManager *manager, int *state) {
    PCBoxWork *work;

    Heap_Create(3, 9, 1 << 16);
    work = OverlayManager_CreateAndGetData(manager, sizeof(PCBoxWork), 9);
    MI_CpuFill8(work, 0, sizeof(PCBoxWork));
    work->args = OverlayManager_GetArgs(manager);
    work->storage = SaveArray_PCStorage_Get(work->args->saveData);
    work->activeBox = PCStorage_GetActiveBox(work->storage);
    work->party = SaveArray_Party_Get(work->args->saveData);
    work->bag = Save_Bag_Get(work->args->saveData);
    work->options = Save_PlayerData_GetOptionsAddr(work->args->saveData);
    work->unk21 = 0xff;
    work->unk2C = 0;
    work->unk30 = 11;
    return TRUE;
}

BOOL PCBox_Main(OverlayManager *manager, int *state) {
    PCBoxWork *work = OverlayManager_GetData(manager);

    if (ov14_021EAF8C(work, state) == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL PCBox_Exit(OverlayManager *manager, int *state) {
    PCBoxWork *work = OverlayManager_GetData(manager);

    PCStorage_SetActiveBox(work->storage, work->activeBox);
    OverlayManager_FreeData(manager);
    Heap_Destroy(9);
    return TRUE;
}

void ov14_021E59AC(void *task, PCBoxWork *work) {
    if (work->inner->callback != NULL && work->inner->callback(work) == 0) {
        work->inner->callback = NULL;
    }
    ov14_021E5A14(work);
    PaletteData_PushTransparentBuffers(work->inner->palette);
    DoScheduledBgGpuUpdates(work->inner->bgConfig);
    ov14_021F29C4(work->inner);
    if (work->inner->disableSpriteDraw == 0) {
        SpriteSystem_DrawSprites(((void **)work->inner)[0xBE]);
        SpriteSystem_TransferOam();
    }
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void ov14_021E5A14(PCBoxWork *work) {
    if (work->args->unk8 != 3) {
        sub_02019978(work->inner->spriteSystem, 0x10);
        return;
    }
    sub_02019978(work->inner->spriteSystem, 0x10);
    ov14_021F3BC0(work->inner);
}
