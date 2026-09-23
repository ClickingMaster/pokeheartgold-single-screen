#include "field_overlay_effects_internal.h"
void *AddCharResObjFromOpenNarcWithAtEndFlag(void *, void *, int, int, int, int, int, int);
void *AddPlttResObjFromOpenNarc(void *, void *, int, int, int, int, int, int);
void *AddCellOrAnimResObjFromOpenNarc(void *, void *, int, int, int, int, int);
void SpriteTransfer_CreateCharTransferTask_AllocAtEnd(void *);
void sub_0200A740(void *);
void SpriteTransfer_CreatePlttTransferTask(void *);
void CreateSpriteResourcesHeader(void *, int, int, int, int, int, int, int, int, void *, void *, void *, void *, void *, void *);
void ov01_021F0614(void *, void *, void *, int, int, int, int, int, int);
void ov01_021F0614(void *mgr, void *holder, void *arg, int plttFile, int plttSlot, int charFile, int cellFile, int animFile, int id) {
    void **out = arg;
    void **h = holder;
    out[0] = AddCharResObjFromOpenNarcWithAtEndFlag(h[75], mgr, charFile, 0, id, 1, 4, 1);
    out[1] = AddPlttResObjFromOpenNarc(h[76], mgr, plttFile, 0, id, 1, plttSlot, 4);
    out[2] = AddCellOrAnimResObjFromOpenNarc(h[77], mgr, cellFile, 0, id, 2, 4);
    out[3] = AddCellOrAnimResObjFromOpenNarc(h[78], mgr, animFile, 0, id, 3, 4);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(out[0]);
    sub_0200A740(out[0]);
    SpriteTransfer_CreatePlttTransferTask(out[1]);
    CreateSpriteResourcesHeader(out + 4, id, id, id, id, -1, -1, 0, 0, h[75], h[76], h[77], h[78], 0, 0);
}
