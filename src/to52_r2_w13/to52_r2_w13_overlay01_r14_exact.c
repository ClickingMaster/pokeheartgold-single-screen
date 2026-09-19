#include "private.h"

void ov01_021FAB9C(void *task, W13Work *work) {
    void *obj = work->obj;
    switch (work->state) {
    case 0:
        if (!MapObject_CheckActive(obj) || !ov01_021FA2D4(obj)) {
            if (work->reflection != NULL) {
                sub_02068B48();
            }
            sub_02023DA4(work->unk24);
            Heap_Free(work->mdl1);
            Heap_Free(work->mdl2);
            Heap_Free(work->mdl3);
            Heap_Free(work);
            SysTask_Destroy(task);
            return;
        }
        if (work->spriteId != MapObject_GetSpriteID(obj)) {
            work->state++;
        }
        break;
    case 1:
        if (!ov01_021FA2D4(obj)) {
            if (work->reflection != NULL) {
                sub_02068B48();
            }
            sub_02023DA4(work->unk24);
            Heap_Free(work->mdl1);
            Heap_Free(work->mdl2);
            Heap_Free(work->mdl3);
            Heap_Free(work);
            SysTask_Destroy(task);
        }
        break;
    }
}
