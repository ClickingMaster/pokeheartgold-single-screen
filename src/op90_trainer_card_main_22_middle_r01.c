#pragma force_active on
#include "global.h"
#include "sprite.h"
#include "sprite_system.h"
typedef struct { u8 x, y; } Pos8;
typedef struct Work {
    SpriteList *spriteList;
    u8 pad4[0x13C - 4];
    void *res[4];
    u8 pad14C[0x1F0 - 0x14C];
    Sprite *sprites[2];
} Work;
extern const Pos8 ov51_021E80A4[];
extern const u8 _021E80A0[];
void ov51_021E7BD0(Work *w);
// @fn ov51_021E7BD0
void ov51_021E7BD0(Work *w){SpriteResourcesHeader header; SpriteTemplate tmpl; int i; CreateSpriteResourcesHeader(&header, 1, 1, 1, 1, -1, -1, 0, 1, w->res[0], w->res[1], w->res[2], w->res[3], NULL, NULL); tmpl.spriteList = w->spriteList; tmpl.header = &header; tmpl.position.x = 0; tmpl.position.y = 0; tmpl.position.z = 0; tmpl.scale.x = FX32_ONE; tmpl.scale.y = FX32_ONE; tmpl.scale.z = FX32_ONE; tmpl.rotation = 0; tmpl.drawPriority = 0; tmpl.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB; tmpl.heapID = (enum HeapID)0x19; for (i = 0; i < 2; i++) { tmpl.position.x = ov51_021E80A4[i].x * FX32_ONE; tmpl.position.y = ov51_021E80A4[i].y * FX32_ONE + FX32_CONST(224); w->sprites[i] = Sprite_CreateAffine(&tmpl); Sprite_SetAnimActiveFlag(w->sprites[i], TRUE); Sprite_SetAnimCtrlSeq(w->sprites[i], _021E80A0[i]); Sprite_SetDrawPriority(w->sprites[i], 2 - i); Sprite_SetDrawFlag(w->sprites[i], FALSE); }}
