#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"

#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

extern void GF_AssertFail(void);
extern void *ov47_02258CEC(void *, int, int);
extern void ov47_022592B4(void *, void *);
extern void ov47_02259404(void *, void *, void *);
extern int ov47_02259430(void *, void *);
extern void ov47_0225916C(void);
extern void *ov47_02259D40(void *, void *);
extern void ov47_02259318(void *, void *);
extern void ov47_022591F8(void *);
extern void ov47_0225921C(void *);
extern void PlaySE(u16);
extern void ov45_0222A7DC(void *, void *, int);
extern void ov45_0222F4D4(int);
extern void ov47_02259384(void *);
extern int ov45_0222F524(void);
extern void ov47_022593A0(void *);
extern void ov47_022593CC(void *);
extern int ov47_022593B4(void *);
extern const WindowTemplate ov47_02259E40;
extern void ov47_02259C8C(void *, int, int);

typedef struct Ov47State {
    u8 pad00[0x10];
    u32 selection;
    u8 pad14[0x14];
    u16 state;
    u16 returnState;
    u8 pad2C[4];
    void *value30;
    u32 flag34;
} Ov47State;

int ov47_02258F48(void *state, void *ctx, void *arg2, void *ui, void *owner, void *arg5);
void ov47_02258DD0(void *dst, void *unused, void *context, enum HeapID heapId);

void ov47_02258DD0(void *dst, void *unused, void *context, enum HeapID heapId) {
    SpriteResourcesHeader header;

    GfGfxLoader_GXLoadPalFromOpenNarc(*(NARC **)((u8 *)context + 0x140), 0xC7, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x80, heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(*(NARC **)((u8 *)context + 0x140), 0xC8, *(BgConfig **)context, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(*(NARC **)((u8 *)context + 0x140), 0xC9, *(BgConfig **)context, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapId);

    *(SpriteResource **)((u8 *)dst + 0x18) = AddCharResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)context + 0x130), *(NARC **)((u8 *)context + 0x140), 0xCD, FALSE, 0x14, 1, heapId);
    *(SpriteResource **)((u8 *)dst + 0x1C) = AddPlttResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)context + 0x134), *(NARC **)((u8 *)context + 0x140), 0xCC, FALSE, 0x14, 1, 3, heapId);
    if (!SpriteTransfer_CreateCharTransferTask_AllocAtEnd(*(SpriteResource **)((u8 *)dst + 0x18))) {
        GF_AssertFail();
    }
    if (!SpriteTransfer_CreatePlttTransferTask(*(SpriteResource **)((u8 *)dst + 0x1C))) {
        GF_AssertFail();
    }
    sub_0200A740(*(SpriteResource **)((u8 *)dst + 0x18));
    sub_0200A740(*(SpriteResource **)((u8 *)dst + 0x1C));
    *(SpriteResource **)((u8 *)dst + 0x20) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)context + 0x138), *(NARC **)((u8 *)context + 0x140), 0xCE, FALSE, 0x14, GF_GFX_RES_TYPE_CELL, heapId);
    *(SpriteResource **)((u8 *)dst + 0x24) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)context + 0x13C), *(NARC **)((u8 *)context + 0x140), 0xCF, FALSE, 0x14, GF_GFX_RES_TYPE_ANIM, heapId);

    {
        SimpleSpriteTemplate template = { 0 };
        CreateSpriteResourcesHeader(&header, 0x14, 0x14, 0x14, 0x14, -1, -1, 0, 0, *(GF_2DGfxResMan **)((u8 *)context + 0x130), *(GF_2DGfxResMan **)((u8 *)context + 0x134), *(GF_2DGfxResMan **)((u8 *)context + 0x138), *(GF_2DGfxResMan **)((u8 *)context + 0x13C), NULL, NULL);
        template.spriteList = *(SpriteList **)((u8 *)context + 4);
        template.header = &header;
        template.priority = 0;
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        template.heapID = heapId;
        *(Sprite **)((u8 *)dst + 0x14) = Sprite_Create(&template);
        Sprite_SetDrawFlag(*(Sprite **)((u8 *)dst + 0x14), FALSE);
        AddWindow(*(BgConfig **)context, (Window *)dst, &ov47_02259E40);
        ov47_02259C8C((u8 *)dst + 0x2C, 1, heapId);
    }
}
