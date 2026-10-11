#include "to54_atomic_x03_private.h"

void ov91_0225F508(Ov91Work *p, enum HeapID heapId) {
    HeapExp_FndInitAllocator((NNSFndAllocator *)&p->f86CC, heapId, 0x20);
    ov91_0225F414();
    ov91_0225F4C0(&p->f1AB4, p->unk10, p->unk14, heapId);
    ov91_0225F878((BgConfig **)&p->f1AB4, heapId);
    ov91_0225FA9C(&p->f1AB4, heapId);
    ov91_0225FCD8(&p->f1AB4, heapId);
    void *narc = NARC_New((NarcId)0xBD, heapId);
    ov91_0225FB80(&p->f1AB4, narc, heapId);
    ov91_02260F50(&p->f1C54, &p->f1AB4, narc, p->unk10, p->unk14, heapId, &p->f86CC);
    ov91_0225FF00(&p->f1F74, narc, heapId, &p->f86CC);
    ov91_022604F4(&p->f1AB4, narc, heapId);
    ov91_02260C48(&p->f1AB4, &p->f8544, p->unk14, heapId);
    ov91_02260D14(&p->f1AB4, &p->f854C, p->unk14, heapId);
    ov91_0225FDE0(&p->f1AB4, narc, p->unk14, heapId);
    ov91_0225D8F0(&p->f1F38, &p->f1AB4, p->unk877C, p->unk10, p->unk14, narc, heapId);
    ov91_0225DBD0(&p->f1AB4, p->unk10, p->unk14, heapId);
    ov91_0225D628(&p->f1E38, &p->f1AB4, p->unk10, p->unk14, heapId);
    ov91_022613AC(&p->f21A8, (u8)(u32)p->unk14);
    ov91_02261580(&p->f862C, &p->f1AB4, narc, heapId);
    NARC_Delete(narc);
    p->unk21A4 = PlazaGameSign_New(p->unk1AD0, heapId);
}

void ov91_0225F66C(Ov91Work *p) {
    ov91_02261790(&p->f862C, &p->f1AB4);
    ov91_0225D67C(&p->f1E38, &p->f1AB4);
    ov91_0225DBE0(&p->f1AB4, p->unk10);
    ov91_0225DB0C(&p->f1F38, &p->f1AB4);
    ov91_0225FEFC(&p->f1AB4);
    ov91_02260D80(&p->f1AB4, &p->f854C);
    ov91_02260CA8(&p->f8544);
    ov91_0226066C(&p->f1AB4);
    ov91_022600FC(&p->f1F74, &p->f86CC);
    ov91_02261070(&p->f1C54, &p->f1AB4, &p->f86CC);
    ov91_0225FC84(&p->f1AB4);
    PlazaGameSign_Free(p->unk21A4);
    ov91_0225FA60(&p->f1AB4);
    ov91_0225FB48(&p->f1AB4);
    ov91_0225FD0C(&p->f1AB4);
    ov91_0225F4F8(&p->f1AB4);
    *(vu16 *)0x04000050 = 0;
    *(vu16 *)0x04001050 = 0;
}

void ov91_0225F74C(Ov91Work *a, Ov91Work *b) {
    ov91_0226014C(&b->f04C0, &a->f19CC, a->root);
    ov91_022601AC(&b->f04C0, &a->f19CC, a->root);
    ov91_02260CC8(&b->f6A90);
    ov91_0225F7A8(a, b);
    ov91_022613C8(&b->f06F4, b);
    ov91_02261928(&b->f6B78);
}

void ov91_0225F7A8(Ov91Work *a, Ov91Work *b) {
    u32 one0;
    u32 one1;
    u32 one2;
    ov91_02260D98((u8 *)b + 0x6A98);
    ov91_0225D6A0(&b->f0384);
    Thunk_G3X_Reset();
    ov91_0225FDD8(b);
    NNS_G3dGlbLightVector(GX_LIGHTID_0, 0, -0x1000, 0);
    NNS_G3dGlbLightColor(GX_LIGHTID_0, 0x7FFF);
    NNS_G3dGlbMaterialColorDiffAmb(0x7FFF, 0x7FFF, FALSE);
    NNS_G3dGlbMaterialColorSpecEmi(0x7FFF, 0x7FFF, FALSE);
    NNS_G3dGeBufferOP_N(0x11, NULL, 0);
    ov91_022610A8(&b->f01A0);
    one0 = 1;
    NNS_G3dGeBufferOP_N(0x12, &one0, 1);
    NNS_G3dGeBufferOP_N(0x11, NULL, 0);
    ov91_0226023C(&b->f04C0, &a->f19CC);
    one1 = 1;
    NNS_G3dGeBufferOP_N(0x12, &one1, 1);
    NNS_G3dGeBufferOP_N(0x11, NULL, 0);
    ov91_0226078C(b, a->unk10);
    one2 = 1;
    NNS_G3dGeBufferOP_N(0x12, &one2, 1);
    RequestSwap3DBuffers(0, 0);
    SpriteList_RenderAndAnimateSprites(b->spriteList);
}

void ov91_0225F878(BgConfig **out, enum HeapID heapId) {
    *out = BgConfig_Alloc(heapId);
    GraphicsModes modes = ov91_02261C74;
    SetBothScreensModesAndDisable(&modes);
    BgTemplate bg1 = ov91_02261C9C;
    InitBgFromTemplate(*out, 1, &bg1, 0);
    BG_ClearCharDataRange(1, 0x20, 0, heapId);
    BgClearTilemapBufferAndCommit(*out, 1);
    BgTemplate bg2 = ov91_02261CB8;
    InitBgFromTemplate(*out, 2, &bg2, 0);
    BG_ClearCharDataRange(2, 0x20, 0, heapId);
    BgClearTilemapBufferAndCommit(*out, 2);
    BgTemplate bg4 = ov91_02261CD4;
    InitBgFromTemplate(*out, 4, &bg4, 0);
    BG_ClearCharDataRange(4, 0x20, 0, heapId);
    BgClearTilemapBufferAndCommit(*out, 4);
    BgTemplate bg5 = ov91_02261CF0;
    InitBgFromTemplate(*out, 5, &bg5, 0);
    BG_ClearCharDataRange(5, 0x20, 0, heapId);
    BgClearTilemapBufferAndCommit(*out, 5);
    BgTemplate bg6 = ov91_02261D0C;
    InitBgFromTemplate(*out, 6, &bg6, 0);
    BG_ClearCharDataRange(6, 0x20, 0, heapId);
    BgClearTilemapBufferAndCommit(*out, 6);
    BgTemplate bg7 = ov91_02261D28;
    InitBgFromTemplate(*out, 7, &bg7, 0);
    BG_ClearCharDataRange(7, 0x20, 0, heapId);
    BgClearTilemapBufferAndCommit(*out, 7);
    GfGfx_EngineATogglePlanes(1, TRUE);
    GfGfx_EngineATogglePlanes(2, FALSE);
    GfGfx_EngineATogglePlanes(4, TRUE);
    GfGfx_EngineATogglePlanes(8, FALSE);
    GfGfx_EngineATogglePlanes(0x10, TRUE);
    GfGfx_EngineBTogglePlanes(1, TRUE);
    GfGfx_EngineBTogglePlanes(2, TRUE);
    GfGfx_EngineBTogglePlanes(4, TRUE);
    GfGfx_EngineBTogglePlanes(8, TRUE);
    GfGfx_EngineBTogglePlanes(0x10, TRUE);
    LoadFontPal1(0, 0xA0, heapId);
    LoadUserFrameGfx1(*out, GF_BG_LYR_MAIN_1, 1, 4, 0, heapId);
}
