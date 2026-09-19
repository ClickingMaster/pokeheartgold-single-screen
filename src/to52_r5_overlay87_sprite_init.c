#include "to52_r5_overlay87_sprite_init_private.h"

void ov87_021E7AF8(u8 *work) {
    const u8 *counts;
    int i;
    u8 *cursor;
    u32 charOffset;
    u32 plttOffset;

    GF_CreateVramTransferManager(0x20, 0x7A);
    ov87_021E7EF0();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x80, 0, 0x20, 0, 0x80, 0, 0x20, 0x7A);
    PTR(work, 0) = G2dRenderer_Init(0x28, work + 4, 0x7A);
    G2dRenderer_SetSubSurfaceCoords(work + 4, 0, 2 << 20);

    counts = ov87_021E8394;
    for (i = 0, cursor = work; i < 4; i++) {
        PTR(cursor, 0x12C) = Create2DGfxResObjMan(*counts, i, 0x7A);
        counts++;
        cursor += 4;
    }

    ov87_021E7BC4(work, 1);
    ov87_021E7C50(work, 1, 1, 1);
    ov87_021E7CE0(work, 1);
    ov87_021E7D6C(work, 1);
    ov87_021E7C50(work, 0, 0, 2);

    charOffset = 0x13C;
    plttOffset = charOffset + 4;
    for (i = 0; i < 5; i++) {
        SpriteTransfer_CreateCharTransferTask(PTR(work, charOffset));
        SpriteTransfer_CreateExtPlttTransferTask(PTR(work, plttOffset));
        work += 0x10;
    }

    GfGfx_EngineBTogglePlanes(0x10, 1);
    GfGfx_EngineATogglePlanes(0x10, 1);
}
