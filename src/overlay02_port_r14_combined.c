#include "overlay02_port_private.h"

WIP_LOCAL Sprite *ov02_0224A33C(void *mgr, VecFx32 *pos, int charId, int plttId, int cellId, int mode, int priority, int drawPriority) {
    SpriteResourcesHeader header;
    SimpleSpriteTemplate template;
    Sprite *sprite;

    if (mode == 4) {
        CreateSpriteResourcesHeader(&header, charId, plttId, cellId, -1, -1, -1, 0, priority, *(GF_2DGfxResMan **)((u8 *)mgr + 0x19c), *(GF_2DGfxResMan **)((u8 *)mgr + 0x1a0), *(GF_2DGfxResMan **)((u8 *)mgr + 0x1a4), NULL, NULL, NULL);
    } else {
        CreateSpriteResourcesHeader(&header, charId, plttId, cellId, mode, -1, -1, 0, priority, *(GF_2DGfxResMan **)((u8 *)mgr + 0x19c), *(GF_2DGfxResMan **)((u8 *)mgr + 0x1a0), *(GF_2DGfxResMan **)((u8 *)mgr + 0x1a4), *(GF_2DGfxResMan **)((u8 *)mgr + 0x1a8), NULL, NULL);
    }
    template.spriteList = *(SpriteList **)((u8 *)mgr + 0x70);
    template.header = &header;
    template.position = *pos;
    template.priority = drawPriority;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_FIELD1;
    sprite = Sprite_Create(&template);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    return sprite;
}

WIP_LOCAL Sprite *ov02_0224A3F0(void *mgr, VecFx32 *pos, int drawPriority, int seq) {
    Sprite *sprite = ov02_0224A33C(mgr, pos, 1, 0, 1, 0, 0, drawPriority);
    Sprite_SetAnimCtrlSeq(sprite, seq);
    return sprite;
}

WIP_LOCAL Sprite *ov02_0224A418(void *mgr, VecFx32 *pos) {
    int plttId = 0;
    Sprite *sprite;
    if (*(u16 *)((u8 *)mgr + 0xe) != 0) {
        plttId = 1;
    }
    sprite = ov02_0224A33C(mgr, pos, 2, plttId, 2, 1, 0, 0x83);
    Sprite_SetDrawFlag(sprite, 0);
    Sprite_SetAnimCtrlSeq(sprite, 6);
    return sprite;
}
