#pragma force_active on
#include "global.h"
#include "sprite.h"
#include "unk_02009D48.h"

typedef struct Ov27Pos {
    u16 value;
    u16 unk2;
} Ov27Pos;

typedef struct Ov27Unk {
    u8 unk0[0x218];
    SpriteList *spriteList;
    u8 unk21C[0x128];
    GF_2DGfxResMan *resMan[4];
    u8 unk354[0x10];
    SpriteResourcesHeader header;
    Sprite *sprite;
} Ov27Unk;

extern const Ov27Pos ov27_0225D118[];
extern const Ov27Pos ov27_0225D11A[];

void ov27_0225C80C(Ov27Unk *data, int index);

// @fn ov27_0225C80C
void ov27_0225C80C(Ov27Unk *data, int index) {
    SpriteTemplate tmpl;

    CreateSpriteResourcesHeader(&data->header, 10, 10, 10, 10, -1, -1, 0, 0, data->resMan[0], data->resMan[1], data->resMan[2], data->resMan[3], NULL, NULL);
    tmpl.spriteList = data->spriteList;
    tmpl.header = &data->header;
    tmpl.position.z = 0;
    tmpl.position.x = ov27_0225D118[index].value << FX32_SHIFT;
    tmpl.position.y = (ov27_0225D11A[index].value + 256) << FX32_SHIFT;
    tmpl.scale.x = FX32_ONE;
    tmpl.scale.y = FX32_ONE;
    tmpl.scale.z = FX32_ONE;
    tmpl.rotation = 0;
    tmpl.drawPriority = 1;
    tmpl.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    tmpl.heapID = HEAP_ID_8;
    data->sprite = Sprite_CreateAffine(&tmpl);
    Sprite_SetAnimCtrlSeq(data->sprite, 1);
    Sprite_SetAnimActiveFlag(data->sprite, TRUE);
    Sprite_SetDrawFlag(data->sprite, FALSE);
}
