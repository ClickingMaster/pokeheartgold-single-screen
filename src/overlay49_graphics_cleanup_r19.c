#include "overlay49_graphics_cleanup_r19_private.h"

void ov49_0225A7D0(void *work) {
    int i;
    int j;
    const u32 *bgIds;
    u32 offset;

    GF_DestroyVramTransferManager();
    bgIds = ov49_02269734;
    for (i = 0; i < 4; i++, bgIds++) {
        FreeBgTilemapBuffer(PTR(work, 0), (u8)*bgIds);
    }
    Heap_Free(PTR(work, 0));
    sub_0202067C(PTR(work, 0x140));
    offset = 0x140;
    j = 0;
    PTR(work, offset) = NULL;
    bgIds = work;
    offset -= 0x10;
    do {
        Destroy2DGfxResObjMan(PTR(bgIds, offset));
        j++;
        bgIds++;
    } while (j < 4);
    SpriteList_Delete(PTR(work, 4));
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
    GF_3DVramMan_Delete(PTR(work, 0x144));
}
