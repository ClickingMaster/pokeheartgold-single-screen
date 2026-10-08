#pragma force_active on
#include "global.h"
#include "sprite.h"
#include "unk_02009D48.h"

typedef struct {
    u16 x;
    u16 y;
    u16 anim;
} Ov37SpritePos;

typedef struct {
    u8 unk0[0x34];
    SpriteList *spriteList;
    u8 unk38[0x160 - 0x38];
    GF_2DGfxResMan *resMan[4];
    u8 unk170[0x190 - 0x170];
    SpriteResourcesHeader header1;
    SpriteResourcesHeader header2;
    u8 unk1D8[0];
    Sprite *sprites[5];
    u8 unk1EC[0x210 - 0x1EC];
    Sprite *sprites3[5];
    u8 unk224[0x248 - 0x224];
    Sprite *sprites2[12];
} Ov37Work;

extern const Ov37SpritePos ov37_021E7A80[];
void GfGfx_EngineATogglePlanes(int plane, int enable);
void GfGfx_EngineBTogglePlanes(int plane, int enable);
void ov37_021E6244(Ov37Work *work);

// @fn ov37_021E6244
void ov37_021E6244(Ov37Work *work) {
    SpriteTemplate template;
    int i;

    CreateSpriteResourcesHeader(&work->header1, 0, 0, 0, 0, -1, -1, 0, 0, work->resMan[0], work->resMan[1], work->resMan[2], work->resMan[3], NULL, NULL);
    CreateSpriteResourcesHeader(&work->header2, 1, 1, 1, 1, -1, -1, 0, 0, work->resMan[0], work->resMan[1], work->resMan[2], work->resMan[3], NULL, NULL);
    template.spriteList = work->spriteList;
    template.header = &work->header1;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 1;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_39;
    for (i = 0; i < 5; i++) {
        template.header = &work->header2;
        template.position.x = (24 + i * 40) * FX32_ONE;
        template.position.y = 64 * FX32_ONE;
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
        work->sprites[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(work->sprites[i], TRUE);
        Sprite_SetAnimCtrlSeq(work->sprites[i], i);
        Sprite_SetDrawFlag(work->sprites[i], FALSE);
    }
    for (i = 0; i < 12; i++) {
        template.header = &work->header2;
        template.position.x = ov37_021E7A80[i].x * FX32_ONE;
        template.position.y = ov37_021E7A80[i].y * FX32_ONE;
        work->sprites2[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(work->sprites2[i], TRUE);
        Sprite_SetAnimCtrlSeq(work->sprites2[i], ov37_021E7A80[i].anim);
        if (i >= 8) {
            Sprite_SetPriority(work->sprites2[i], 2);
        }
    }
    Sprite_SetAnimCtrlSeq(work->sprites2[0], 6);
    for (i = 0; i < 5; i++) {
        template.position.x = 24 * FX32_ONE;
        template.position.y = (32 + i * 32) * FX32_ONE + 256 * FX32_ONE;
        work->sprites3[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(work->sprites3[i], TRUE);
        Sprite_SetAnimCtrlSeq(work->sprites3[i], i);
        Sprite_SetDrawPriority(work->sprites3[i], 1);
        Sprite_SetDrawFlag(work->sprites3[i], FALSE);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
}
