#include "slop_r1_module_05_private.h"

void ov18_021F94BC(SlopR1Module05Work *work) {
    SimpleSpriteTemplate template;
    SpriteResourcesHeader header;

    PokedexEntryPopup_MakeResourcesHeader(work->objs[0].res, work->resMan, &header, 1);
    template.spriteList = work->spriteList;
    template.header = &header;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.priority = 0;
    template.heapID = work->heapId;
    template.position.x = 112 * FX32_ONE;
    template.position.y = 32 * FX32_ONE;
    work->objs[0].sprite = Sprite_Create(&template);
}
