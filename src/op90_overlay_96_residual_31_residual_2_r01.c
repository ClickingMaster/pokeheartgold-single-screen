#pragma force_active on
#include "global.h"
#include "sprite.h"
#include "unk_02009D48.h"
typedef struct {
    u16 x0;
    u16 x2;
    Sprite *sprite;
    u16 x8;
    u16 resId;
} Entry;
typedef struct {
    enum HeapID heapId;
    int count;
    int max;
    u8 padc[8];
    SpriteList *spriteList;
    u8 pad18[0x140 - 0x18];
    GF_2DGfxResMan *resMan[4];
    u8 pad150[4];
    Entry *entries;
} Mgr;
Sprite *ov96_021EB408(Mgr *mgr, int priority, NNS_G2D_VRAM_TYPE screen, u16 resId, u8 x8);
// @fn ov96_021EB408
Sprite *ov96_021EB408(Mgr *mgr, int priority, NNS_G2D_VRAM_TYPE screen, u16 resId, u8 x8) {
    SpriteResourcesHeader header;
    SpriteTemplate tmpl;
    Sprite *sprite;
    GF_ASSERT(mgr->count < mgr->max);
    CreateSpriteResourcesHeader(&header, resId, resId, resId, resId, -1, -1, 0, priority, mgr->resMan[0], mgr->resMan[1], mgr->resMan[2], mgr->resMan[3], NULL, NULL);
    tmpl.spriteList = mgr->spriteList;
    tmpl.header = &header;
    tmpl.heapID = mgr->heapId;
    tmpl.position.x = 0;
    tmpl.position.y = 0;
    tmpl.position.z = 0;
    tmpl.scale.x = FX32_ONE;
    tmpl.scale.y = FX32_ONE;
    tmpl.scale.z = FX32_ONE;
    tmpl.rotation = 0;
    tmpl.drawPriority = 0;
    tmpl.whichScreen = screen;
    sprite = Sprite_CreateAffine(&tmpl);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, FALSE);
    mgr->entries[mgr->count].x0 = 0;
    mgr->entries[mgr->count].x2 = 0;
    mgr->entries[mgr->count].sprite = sprite;
    mgr->entries[mgr->count].x8 = x8;
    mgr->entries[mgr->count].resId = resId;
    mgr->count++;
    return sprite;
}
