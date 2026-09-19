#include "to52_r6_w06_overlay96_r12_private.h"
void ov96_021E8A44(Ov96R12Work *work, u32 count, void *spriteList) {
    int i;
    if (count > 16) {
        GF_AssertFail();
    }
    work->count = count;
    work->cursor = 0;
    work->task = NULL;
    work->unk08 = NULL;
    work->records = Heap_Alloc(work->heapId, count * 0x2c);
    if (spriteList == NULL) {
        work->spriteList = G2dRenderer_Init(work->count, work->renderer, work->heapId);
        work->renderer2 = work->spriteList;
        work->unk10 = 1;
    } else {
        work->renderer2 = spriteList;
        work->unk10 = 0;
    }
    for (i = 0; i < 4; i++) {
        work->managers[i] = Create2DGfxResObjMan(work->count, i, work->heapId);
    }
}
void ov96_021E8AAC(Ov96R12Work *work) {
    u8 i;
    for (i = 0; i < work->count; i++) {
        SpriteTransfer_DeleteCharTransferTask(*(void **)((u8 *)work->records + i * 0x2c));
        SpriteTransfer_DeletePlttTransferTask(*(void **)((u8 *)work->records + i * 0x2c + 4));
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(work->managers[i]);
    }
    Heap_Free(work->records);
    if (work->unk10 != 0) {
        SpriteList_Delete(work->spriteList);
    }
    Heap_Free(work);
}
void ov96_021E8B1C(Ov96R12Work *work, s32 count, struct Ov96R12Record *records, struct Ov96R12Template *template, u32 priority, void *unk04) {
    int i;
    Ov96R12Task *task = Heap_AllocAtEnd(work->heapId, 0x154);
    task->count = count;
    task->work = work;
    task->template = *template;
    task->unk08 = 0;
    task->unk04 = unk04;
    task->cursor = 0;
    work->task = NULL;
    for (i = 0; i < count; i++) {
        task->records[i] = records[i];
    }
    SysTask_CreateOnMainQueue(ov96_021E8FB4, task, priority);
}
