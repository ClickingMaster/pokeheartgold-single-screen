#include "to52_r5_overlay71_r46_private.h"

void ov71_0224B720(Ov71WorkR46 *work) {
    vu16 *reg = (vu16 *)0x04000060;
    NNSGfdTexKey texKey;
    NNSGfdPlttKey plttKey;

    NNS_G3dInit();
    G3X_InitMtxStack();
    *reg &= 0xffffcffd;
    *reg = (*reg & 0xffffcfff) | 0x10;
    *reg = 0xcffb & *reg;
    *reg = (*reg & (0xffffcffd + 2)) | 8;
    G3X_SetClearColor(0, 0, 0x7fff, 0x3f, FALSE);
    *(vu32 *)0x04000540 = 2;
    *(vu32 *)0x04000580 = 0xbfff0000;
    GF_3DVramMan_InitFrameTexVramManager(1, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
    texKey = NNS_GfdDefaultFuncAllocTexVram(0x4000, FALSE, 0);
    plttKey = NNS_GfdDefaultFuncAllocPlttVram(0x80, FALSE, 1);
    PokepicManager_SetCharBaseAddrAndSize(work->pokepicManager, (texKey << 16) >> 13, ((texKey & 0x7fff0000) >> 16) << 4);
    PokepicManager_SetPlttBaseAddrAndSize(work->pokepicManager, (plttKey << 16) >> 13, ((plttKey & 0xffff0000) >> 16) << 3);
}
