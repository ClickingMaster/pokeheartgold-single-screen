#include "overlay27_subapp_lifecycle_private.h"

void *ov27_0225C250(BgConfig *bgConfig, void *arg1, Ov27FieldFlagsW17 *fieldSystem, void *args) {
    SysTask *task;
    Ov27WorkW17 *work;
    vu32 *dispCnt = (vu32 *)0x04001000;

    Heap_Create(HEAP_ID_3, HEAP_ID_8, 3 << 15);
    GXS_SetGraphicsMode((GXBGMode)0);
    GX_SetBankForSubBG((GXVRamSubBG)0x80);
    GX_SetBankForSubOBJ((GXVRamSubOBJ)0x100);
    *dispCnt = (*dispCnt & 0xFFCFFFEF) | 0x10;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &ov27_0225D370, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &ov27_0225D38C, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &ov27_0225D3A8, 0);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, (enum HeapID)4);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, (enum HeapID)4);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, (enum HeapID)4);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);

    task = CreateSysTaskAndEnvironment((SysTaskFunc)ov27_0225C434, 0x3A4, 10, HEAP_ID_8);
    work = SysTask_GetData(task);
    work->task = task;
    work->selection = 0;
    work->bgConfig = bgConfig;
    work->arg1 = arg1;
    work->fieldSystem = fieldSystem;
    work->unk48 = 0;
    work->state = 0;
    work->args = args;
    work->unk39C = 0;
    work->unk3A0 = 0;
    work->unk34 = 0;
    work->unk44 = 0;

    FontID_Alloc(4, HEAP_ID_8);
    ov27_0225C914(work, work->fieldSystem->mode);
    ov27_0225C4AC(work);
    ov27_0225C72C(work);
    ov27_0225C80C(work, 0);

    *dispCnt &= 0xFFFF1FFF;
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    return task;
}

void ov27_0225C398(BgConfig *bgConfig, SysTask *task) {
    Ov27WorkW17 *work = SysTask_GetData(task);
    int i;

    SpriteTransfer_DeleteCharTransferTask(work->charResource);
    SpriteTransfer_DeletePlttTransferTask(work->plttResource);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(work->resManagers[i]);
    }
    SpriteList_Delete(work->spriteList);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    ov27_0225C930(work);
    FontID_Release(4);
    DestroySysTaskAndEnvironment(task);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    Heap_Destroy(HEAP_ID_8);
}
