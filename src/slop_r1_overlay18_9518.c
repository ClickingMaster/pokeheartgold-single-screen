#include "slop_r1_module_05_private.h"

void ov18_021F9518(SlopR1Module05Work *work) {
    SimpleSpriteTemplate template;
    SpriteResourcesHeader header;
    void *footprint;
    u32 location;

    ov18_021F9310(work->objs[3].res, work->resMan, &header, 1);
    template.spriteList = work->spriteList;
    template.header = &header;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.priority = 0;
    template.heapID = work->heapId;
    template.position.x = 120 * FX32_ONE;
    template.position.y = 80 * FX32_ONE;
    work->objs[3].sprite = Sprite_Create(&template);

    footprint = ov18_021F9694(work->species, work->heapId);
    location = NNS_G2dGetImageLocation(Sprite_GetImageProxy(work->objs[3].sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    DC_FlushRange(footprint, 0x80);
    GX_LoadOBJ(footprint, location, 0x80);
    Heap_Free(footprint);
}
