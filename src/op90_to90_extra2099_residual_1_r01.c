#pragma force_active on
#include "global.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "oam_manager.h"

typedef struct Ov74GfxWork {
    u32 unk00, unk04;
    enum HeapID heapId;
    u8 unk0C[0x28];
    SpriteList *spriteList;
    u8 unk38[0x128];
    GF_2DGfxResMan *resMan[6];
} Ov74GfxWork;

extern u32 ov74_0223D450;
extern Ov74GfxWork ov74_0223D454;
extern G2dRenderer ov74_0223D48C;
extern u8 ov74_0223D654[];

// The original file owned the work area, so its literal pools hold the address twice; the second entry is
// reached here through the label that precedes it.
#define GFX_WORK ((Ov74GfxWork *)(&ov74_0223D450 + 1))

void ov74_02235690(void);
SpriteList **ov74_02235708(void);

// @fn ov74_02235690
void ov74_02235690(void)
{
    Ov74GfxWork *work = GFX_WORK;
    int i;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, ov74_0223D454.heapId);
    ov74_0223D454.spriteList = G2dRenderer_Init(128, &ov74_0223D48C, ov74_0223D454.heapId);
    G2dRenderer_SetSubSurfaceCoords(&ov74_0223D48C, 0, 0x100000);
    *(fx32 *)(ov74_0223D654 + 0x10) = 0xC0000;
    for (i = 0; i < 6; i++) {
        work->resMan[i] = Create2DGfxResObjMan(32, (GfGfxResType)i, work->heapId);
    }
}

// @fn ov74_02235708
SpriteList **ov74_02235708(void)
{
    Ov74GfxWork *work = GFX_WORK;
    GF_ASSERT(ov74_0223D454.spriteList != NULL);
    return &work->spriteList;
}
