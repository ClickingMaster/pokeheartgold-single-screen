#include "overlay74_thumb_w07_private.h"

void ov74_0222F2D4(void *data, int member, int bgId, int destination) {
    NNSG2dScreenData *screen;
    void *raw = GfGfxLoader_LoadFromNarc(0x71, member, 1, U32AT(data, 0), 1);
    NNS_G2dGetUnpackedScreenData(raw, &screen);
    BG_LoadScreenTilemapData((void *)U32AT(data, 0x29fc), bgId, screen->rawData, destination);
    Heap_Free(raw);
}
