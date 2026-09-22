#include "to55_overlay91_run062_private.h"

extern const u8 ov91_02261BF0[];
void sub_02018030(void *, NARC *, int, enum HeapID);
void ov91_022604F4(void *, NARC *, enum HeapID);

typedef struct T11InitWork {
    u8 pad000[0x6FC];
    u8 modelResources[5][0x10];
    u8 finalModelResource[0x10];
    void *resourceObjects[4];
    SpriteResourcesHeader resourceHeader;
    T11Entry entries[0x60];
} T11InitWork;

void ov91_022604F4(void *work, NARC *narc, enum HeapID heapId) {
    int i;
    T11InitWork *init = work;
    T11Work *state = work;

    for (i = 0; i < 5; i++) {
        sub_02018030(init->modelResources[i], narc, ov91_02261BF0[i], heapId);
    }
    sub_02018030((u8 *)work + 0x74c, narc, 0x2e, heapId);
    NNSi_G3dModifyPolygonAttrMask(PTR_AT(work, 0x754), 0, 0x1f << 16);

    PTR_AT(work, 0x75c) = AddCharResObjFromOpenNarc(PTR_AT(work, 0x148), narc, 7, FALSE, 0x64, 2, heapId);
    PTR_AT(work, 0x760) = AddPlttResObjFromOpenNarc(PTR_AT(work, 0x14c), narc, 4, FALSE, 0x64, 2, 4, heapId);
    PTR_AT(work, 0x764) = AddCellOrAnimResObjFromOpenNarc(PTR_AT(work, 0x150), narc, 6, FALSE, 0x64, GF_GFX_RES_TYPE_CELL, heapId);
    PTR_AT(work, 0x768) = AddCellOrAnimResObjFromOpenNarc(PTR_AT(work, 0x154), narc, 5, FALSE, 0x64, GF_GFX_RES_TYPE_ANIM, heapId);

    if (!SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(PTR_AT(work, 0x75c))) {
        GF_AssertFail();
    }
    if (!SpriteTransfer_CreatePlttTransferTask(PTR_AT(work, 0x760))) {
        GF_AssertFail();
    }
    sub_0200A740(PTR_AT(work, 0x75c));
    sub_0200A740(PTR_AT(work, 0x760));

    CreateSpriteResourcesHeader((SpriteResourcesHeader *)((u8 *)work + 0x76c), 0x64, 0x64, 0x64, 0x64, -1, -1, 0, 0, PTR_AT(work, 0x148), PTR_AT(work, 0x14c), PTR_AT(work, 0x150), PTR_AT(work, 0x154), NULL, NULL);

    for (i = 0; i < 0x60; i++) {
        ov91_022607C4(work, &state->entries[i], heapId);
    }
}
