#pragma force_active on
#include "global.h"
#include "assert.h"
#include "bg_window.h"
#include "obj_char_transfer.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02013534.h"

typedef struct Ov41SpriteTmpl {
    SpriteList *spriteList;
    const SpriteResourcesHeader *header;
    VecFx32 position;
    VecFx32 scale;
    u16 rotation;
    u32 drawPriority;
    NNS_G2D_VRAM_TYPE whichScreen;
    enum HeapID heapID;
} Ov41SpriteTmpl;

typedef struct Ov41TextTmpl {
    Ov41SpriteTmpl *spriteTmpl;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    Window *window;
    u32 unk14;
    NNSG2dImagePaletteProxy *plttProxy;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
} Ov41TextTmpl;

typedef struct Ov41Res {
    u8 unk0[0x44];
    SpriteList *spriteList;
    GF_2DGfxResMan *resMan[4];
} Ov41Res;

typedef struct Ov41Work {
    u8 unk0[0x14];
    UnkStruct_02021AC8 charData;
} Ov41Work;

void ov41_0224A15C(Ov41Work *work, Ov41TextTmpl *tmpl);
void ov41_0224A734(Ov41Work *work, int id, Ov41Res *res, u32 a3, Window *window, int x, int y);

// @fn ov41_0224A734
void ov41_0224A734(Ov41Work *work, int id, Ov41Res *res, u32 a3, Window *window, int x, int y) {
    Ov41TextTmpl textTmpl;
    SpriteResourcesHeader header;
    Ov41SpriteTmpl spriteTmpl;

    CreateSpriteResourcesHeader(&header, id, 0, id, id, -1, -1, 0, 0, res->resMan[0], res->resMan[1], res->resMan[2], res->resMan[3], NULL, NULL);
    spriteTmpl.spriteList = res->spriteList;
    spriteTmpl.header = &header;
    spriteTmpl.position.x = x * FX32_ONE;
    spriteTmpl.position.y = y * FX32_ONE;
    spriteTmpl.position.z = 0;
    spriteTmpl.drawPriority = 2;
    spriteTmpl.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteTmpl.heapID = HEAP_ID_14;
    textTmpl.spriteTmpl = &spriteTmpl;
    textTmpl.unk4 = 0;
    textTmpl.unk8 = 0;
    textTmpl.unkC = 1;
    textTmpl.unk14 = a3;
    textTmpl.window = window;
    textTmpl.unk1C = 0;
    textTmpl.unk20 = 19;
    textTmpl.plttProxy = SpriteTransfer_GetPaletteProxy(SpriteResourceCollection_Find(res->resMan[1], 1), NULL);
    GF_ASSERT(sub_02021AC8(sub_02013688(window, NNS_G2D_VRAM_TYPE_2DMAIN, 13), TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &work->charData));
    textTmpl.unk24 = work->charData.offset;
    ov41_0224A15C(work, &textTmpl);
}
