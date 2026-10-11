#include "global.h"

#include "palette.h"

void PCStorage_SetBoxWallpaper(void *storage, u32 box, u32 wallpaper);
void ov14_021E78AC(void *p, s32 value);
void PCBox_UpdateCurrentWallpaperMarker(void *p);
void PCBox_DrawBoxThumbnail(void *p, u32 value);
void PCBox_SetSpriteVisible(void *p, u32 value, u32 enabled);
u32 PCBox_State_ApplyWallpaper(u8 *p);
u32 PCBox_State_WallpaperFadeOut(u8 *p);
u32 PCBox_State_WallpaperFadeIn(u8 *p);
u32 ov14_021EDA1C(u8 *p);

u32 PCBox_State_ApplyWallpaper(u8 *p) {
    u8 wallpaper;

    wallpaper = *(u8 *)(*(u8 **)(p + 0x34) + 0x44D);
    ov14_021E78AC(p, wallpaper);
    wallpaper = *(u8 *)(*(u8 **)(p + 0x34) + 0x44D);
    if (wallpaper < 0x10) {
        PCStorage_SetBoxWallpaper(*(void **)(p + 4), p[0x1F], wallpaper);
    } else {
        PCStorage_SetBoxWallpaper(*(void **)(p + 4), p[0x1F], wallpaper + 0x10);
    }
    PCBox_UpdateCurrentWallpaperMarker(p);
    PCBox_DrawBoxThumbnail(p, p[0x1F]);
    return 0x48;
}

u32 PCBox_State_WallpaperFadeOut(u8 *p) {
    PaletteData_LoadPaletteSlotFromHardware(*(void **)(*(u8 **)(p + 0x34) + 0x18), (PaletteBufferId)0, 0, 0x200);
    PaletteData_BeginPaletteFade(*(void **)(*(u8 **)(p + 0x34) + 0x18), 1, 0xC000, 0, 0, 0x10, 0x7FFF);
    *(u32 *)(p + 0x30) = 0x46;
    return 3;
}

u32 PCBox_State_WallpaperFadeIn(u8 *p) {
    PaletteData_BeginPaletteFade(*(void **)(*(u8 **)(p + 0x34) + 0x18), 1, 0xC000, 0, 0x10, 0, 0x7FFF);
    *(u32 *)(p + 0x30) = 0x42;
    return 3;
}

u32 ov14_021EDA1C(u8 *p) {
    PCBox_SetSpriteVisible(*(void **)(p + 0x34), 9, 1);
    if (*(u32 *)(*(u8 **)p + 8) == 3) {
        return 0x75;
    }
    return 0xC;
}
