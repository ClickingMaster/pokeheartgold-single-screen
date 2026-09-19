#include "private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define S32(p, o) (*(s32 *)((u8 *)(p) + (o)))

void ov96_021EAA24(void *task, Ov96R23Work *work) {
    int i;
    int base;
    if (ov96_021E8B88(work->resource) == 0) {
        return;
    }
    base = work->base;
    for (i = 0; i < work->count; i++) {
        work->entries[i].obj = ov96_021E8B8C(work->resource, (u8)i);
        ov96_021EABA8(&work->entries[i].obj, base);
        base++;
        work->entries[i].sprite = ov96_021E90FC(work->entries[i].obj);
    }
    work->done = 1;
    SysTask_Destroy(task);
}

Sprite *ov96_021EAA88(Ov96R23Work *work, int priority, NNS_G2D_VRAM_TYPE whichScreen, int resourceId) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;
    void *res0;
    void *res1;
    void *res2;
    void *res3;

    res0 = ov96_021E9D08(work->resources, 0);
    res1 = ov96_021E9D08(work->resources, 1);
    res2 = ov96_021E9D08(work->resources, 2);
    res3 = ov96_021E9D08(work->resources, 3);
    CreateSpriteResourcesHeader(&header, resourceId, 0x17, resourceId, resourceId, -1, -1, 0, priority, res0, res1, res2, res3, NULL, NULL);
    if (whichScreen == NNS_G2D_VRAM_TYPE_MAX) {
        whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    }
    template.spriteList = work->spriteList;
    template.header = &header;
    template.heapID = work->heapID;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 0;
    template.whichScreen = whichScreen;
    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, FALSE);
    return sprite;
}

void ov96_021EAB38(u8 *work, int draw) {
    void *sprite = ov96_021E8BAC(PTR(work, 0));
    Sprite_SetDrawFlag(sprite, draw);
    if (PTR(work, 4) != NULL) {
        if (S32(work, 0x40) != 0) {
            Sprite_SetDrawFlag(PTR(work, 4), 0);
        } else {
            Sprite_SetDrawFlag(PTR(work, 4), draw);
        }
    }
    if (PTR(work, 8) != NULL) {
        Sprite_SetDrawFlag(PTR(work, 8), draw);
    }
}

void ov96_021EAB74(u8 *work, int draw) {
    void *sprite = PTR(work, 4);
    if (sprite != NULL) {
        if (S32(work, 0x40) != 0) {
            Sprite_SetDrawFlag(sprite, 0);
        } else {
            Sprite_SetDrawFlag(sprite, draw);
        }
    }
}
