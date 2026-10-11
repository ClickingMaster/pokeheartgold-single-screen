#include "to55_overlay91_run062_private.h"

void ov91_0226066C(void *work) {
    T11Work *state = work;
    u8 *entry;
    int i;

    for (i = 0; i < 0x60; i++) {
        ov91_02260830(work, &state->entries[i]);
    }

    SpriteTransfer_DeleteCharTransferTask(PTR_AT(work, 0x75c));
    SpriteTransfer_DeletePlttTransferTask(PTR_AT(work, 0x760));
    DestroySingle2DGfxResObj(PTR_AT(work, 0x148), PTR_AT(work, 0x75c));
    DestroySingle2DGfxResObj(PTR_AT(work, 0x14c), PTR_AT(work, 0x760));
    DestroySingle2DGfxResObj(PTR_AT(work, 0x150), PTR_AT(work, 0x764));
    DestroySingle2DGfxResObj(PTR_AT(work, 0x154), PTR_AT(work, 0x768));

    entry = (u8 *)work + 0x6fc;
    for (i = 0; i < 5; i++, entry += 0x10) {
        ModelResource_Free(entry);
    }
    ModelResource_Free((u8 *)work + 0x74c);
    memset((u8 *)work + 0x6fc, 0, 0x94);
    memset((u8 *)work + 0x790, 0, 0x6300);
}

void ov91_02260728(void *a, void *entry, void *arg, u16 value) {
    void *owner = ov91_02260A50(a);
    if (*(u16 *)((u8 *)entry + 4) == value) {
        ov91_02260848(a, owner, entry, arg);
    } else {
        ov91_02260884(a, owner, entry);
    }
}

void ov91_02260754(void *work, void *target) {
    int i;
    u8 *entry;

    i = 0;
    entry = work;
    while (i < 0x60) {
        if (PTR_AT(entry, 0x888) == target) {
            ov91_02260A2C((u8 *)work + 0x790 + i * 0x108);
            return;
        }
        i++;
        entry += 0x108;
    }
    GF_AssertFail();
}

void ov91_0226078C(void *work, void *arg) {
    T11Work *state = work;
    int i;

    for (i = 0; i < 0x60; i++) {
        if (state->entries[i].target != NULL) {
            ov91_022608A8(&state->entries[i], work, arg);
        }
    }
}

void ov91_022607C4(void *work, void *entry, enum HeapID heapId) {
    SpriteTemplate template = { 0 };
    Sprite *sprite;

    template.spriteList = PTR_AT(work, 0x1c);
    template.header = (SpriteResourcesHeader *)((u8 *)work + 0x76c);
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.drawPriority = 0x20;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = heapId;
    sprite = Sprite_CreateAffine(&template);
    PTR_AT(entry, 0) = sprite;
    Sprite_SetDrawFlag(sprite, FALSE);
    ModelInstance_Init((u8 *)entry + 0x7c, (u8 *)work + 0x74c);
    ModelInstance_SetVisible((u8 *)entry + 0x7c, 0);
}
