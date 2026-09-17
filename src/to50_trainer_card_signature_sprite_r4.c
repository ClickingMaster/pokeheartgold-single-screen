#include "global.h"

#include "gf_gfx_planes.h"
#include "sprite.h"

void ov52_021E888C(void *);
void ov52_021E888C(void *work) {
    SpriteTemplate t;

    CreateSpriteResourcesHeader((SpriteResourcesHeader *)((u8 *)work + 0x198), 0, 0, 0, 0, -1, -1, 0, 0, *(GF_2DGfxResMan **)((u8 *)work + 0x168), *(GF_2DGfxResMan **)((u8 *)work + 0x16c), *(GF_2DGfxResMan **)((u8 *)work + 0x170), *(GF_2DGfxResMan **)((u8 *)work + 0x174), NULL, NULL);
    CreateSpriteResourcesHeader((SpriteResourcesHeader *)((u8 *)work + 0x1bc), 1, 1, 1, 1, -1, -1, 0, 0, *(GF_2DGfxResMan **)((u8 *)work + 0x168), *(GF_2DGfxResMan **)((u8 *)work + 0x16c), *(GF_2DGfxResMan **)((u8 *)work + 0x170), *(GF_2DGfxResMan **)((u8 *)work + 0x174), NULL, NULL);
    t.spriteList = *(SpriteList **)((u8 *)work + 0x3c);
    t.header = (SpriteResourcesHeader *)((u8 *)work + 0x198);
    t.position.z = 0;
    t.scale.x = 0x1000;
    t.scale.y = 0x1000;
    t.scale.z = 0x1000;
    t.rotation = 0;

    t.drawPriority = 1;
    t.whichScreen = (NNS_G2D_VRAM_TYPE)1;
    t.heapID = (enum HeapID)0x27;
    t.header = (SpriteResourcesHeader *)((u8 *)work + 0x1bc);
    t.position.x = 0xe0000;
    t.position.y = 0xaf000;

    *(Sprite **)((u8 *)work + 0x250) = Sprite_CreateAffine(&t);
    Sprite_SetAnimActiveFlag(*(Sprite **)((u8 *)work + 0x250), 1);
    Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)work + 0x250), 0);
    Sprite_SetPaletteOverride(*(Sprite **)((u8 *)work + 0x250), 0);
    Sprite_SetPriority(*(Sprite **)((u8 *)work + 0x250), 2);
    GfGfx_EngineATogglePlanes(0x10, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
}
