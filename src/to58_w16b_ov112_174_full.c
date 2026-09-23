#include "global.h"
extern void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, int, int, int, int, int, int, int);
extern void GF_AssertFail(void);
extern void GfGfxLoader_GXLoadPalFromOpenNarc(void *, int, int, int, int, int);
extern void GfGfxLoader_LoadCharDataFromOpenNarc(void *, int, int, int, int, int, int, int);
void ov112_021F1838(void *);
void ov112_021F1838(void *s) {
    if (!*(int *)((u8 *)s + 0x14)) GF_AssertFail();
    GfGfxLoader_GXLoadPalFromOpenNarc(*(void **)((u8 *)s + 0x10), 0, 0, 0, 0, *(int *)((u8 *)s + 4));
    GfGfxLoader_LoadCharDataFromOpenNarc(*(void **)((u8 *)s + 0x10), 1, *(int *)((u8 *)s + 0x14), 0, 0, 0, 1, *(int *)((u8 *)s + 4));
    GfGfxLoader_LoadScrnDataFromOpenNarc(*(void **)((u8 *)s + 0x10), 2, *(int *)((u8 *)s + 0x14), 0, 0, 0, 1, *(int *)((u8 *)s + 4));
    GfGfxLoader_LoadCharDataFromOpenNarc(*(void **)((u8 *)s + 0x10), 1, *(int *)((u8 *)s + 0x14), 1, 0, 0, 1, *(int *)((u8 *)s + 4));
    GfGfxLoader_LoadScrnDataFromOpenNarc(*(void **)((u8 *)s + 0x10), 3, *(int *)((u8 *)s + 0x14), 1, 0, 0, 1, *(int *)((u8 *)s + 4));
    GfGfxLoader_GXLoadPalFromOpenNarc(*(void **)((u8 *)s + 0x10), 9, 4, 0, 0, *(int *)((u8 *)s + 4));
    GfGfxLoader_LoadCharDataFromOpenNarc(*(void **)((u8 *)s + 0x10), 10, *(int *)((u8 *)s + 0x14), 4, 0, 0, 1, *(int *)((u8 *)s + 4));
    GfGfxLoader_LoadScrnDataFromOpenNarc(*(void **)((u8 *)s + 0x10), 11, *(int *)((u8 *)s + 0x14), 4, 0, 0, 1, *(int *)((u8 *)s + 4));
}

void ov112_021F1904(void *, unsigned, int);
void ov112_021F1904(void *s, unsigned value, int mode) {
    int id;
    if (value >= 6 && value < 16) id = 0;
    else if (value >= 16 && value < 19) id = 1;
    else id = 2;
    if (mode == 5) id = (u8)(id + 3);
    GfGfxLoader_LoadScrnDataFromOpenNarc(*(void **)((u8 *)s+0x10), id+3, *(int *)((u8 *)s+0x14), 1, 0, 0, 1, *(int *)((u8 *)s+4));
}
