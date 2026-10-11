#include "overlay74_thumb_w07_private.h"

int ov74_0222F598(void *manager) {
    void *data;
    Heap_Create(3, 0x55, 3 << 16);
    data = OverlayManager_CreateAndGetData(manager, 0x3d0c, 0x55);
    memset(data, 0, 0x3d0c);
    U32AT(data, 0x29fc) = (u32)BgConfig_Alloc(0x55);
    U32AT(data, 0) = 0x55;
    SetMasterBrightnessByColor(0, 0);
    SetMasterBrightnessByColor(1, 0);
    U32AT(data, 0x2bf4) = 1;
    ov74_022352A0(0x55);
    Heap_Create(0, 0x59, 0x570);
    return 1;
}
void ov74_0222F600(void *task, void *data) {
    void *p;
    HBlankDma_Stop();
    p = DoubleBuffer_GetOther((void *)U32AT(data, 0xc00));
    HBlankDma_Start(p, (void *)0x04000010, 8, 1);
}
