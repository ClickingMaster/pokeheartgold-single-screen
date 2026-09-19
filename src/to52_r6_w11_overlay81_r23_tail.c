#include "global.h"

#include "bg_window.h"

typedef struct LocalScreenData {
    u32 width;
    volatile u32 height;
    u32 size;
    void *rawData;
} LocalScreenData;

void GfGfxLoader_LoadCharDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void GfGfxLoader_GXLoadPalFromOpenNarc(void *, int, int, int, int, int);
void *GfGfxLoader_GetScrnDataFromOpenNarc(void *, int, int, void *, int);
void Heap_Free(void *);

void ov81_02243100(BgConfig *bgConfig, void *narc);
void ov81_02243140(BgConfig *bgConfig, void *narc, int kind);
void ov81_022431E0(BgConfig *bgConfig, int kind);

void ov81_022431E0(BgConfig *bgConfig, int kind) {
    int x;
    int height;

    if (kind == 0) {
        x = 11;
        height = 7;
    } else {
        x = 14;
        height = 4;
    }
    FillBgTilemapRect(bgConfig, GF_BG_LYR_SUB_1, 0, 0, (u8)x, 32, (u8)height, 9);
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_SUB_1);
}
