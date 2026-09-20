#include "global.h"
extern void Sprite_Delete(void *p);
extern void SpriteTransfer_DeleteCharTransferTask(void *p);
extern void SpriteTransfer_DeletePlttTransferTask(void *p);
extern void DestroySingle2DGfxResObj(void *manager, void *obj);
void ov91_0225F1E8(void *a, void *b);

void ov91_0225F1E8(void *a, void *b) {
    u8 *p = a;
    u8 *q = b;
    Sprite_Delete(*(void **)(p + 0x34));
    SpriteTransfer_DeleteCharTransferTask(*(void **)(p + 0));
    SpriteTransfer_DeletePlttTransferTask(*(void **)(p + 4));
    DestroySingle2DGfxResObj(*(void **)(q + 0x148), *(void **)(p + 0));
    DestroySingle2DGfxResObj(*(void **)(q + 0x14c), *(void **)(p + 4));
    DestroySingle2DGfxResObj(*(void **)(q + 0x150), *(void **)(p + 8));
    DestroySingle2DGfxResObj(*(void **)(q + 0x154), *(void **)(p + 0xc));
    memset(p, 0, 0x40);
}
