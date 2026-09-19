#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "pokemon.h"
#include "screen_fade.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))

typedef struct Ov70R14Work {
    u8 pad00[4];
    BgConfig *bgConfig;
    u8 pad08[0xBA0 - 8];
    void *msgData;
    u8 padBA4[0xF10 - 0xBA4];
    void *sprite;
    u8 padF14[0x1198 - 0xF14];
    Window window;
} Ov70R14Work;

void ov70_022450EC(Window *, void *, int);
void ov70_0223A1E4(void *);
void ov70_02239EAC(void *);
void ov70_02239FA4(void *);
void ov70_0223A0D4(void *);
void ov70_0223A06C(void *);
void ov70_0223A578(void *, void *, void *, void *, void *, void *);
void ov70_0223A72C(void *, void *, void *, void *, void *);
int ov70_0223F864(int, int, int);
void ov70_0223F470(void *, void *, void *, void *, int, int, int);
void ov70_0223A7E4(void *);

void ov70_02239C6C(Ov70R14Work *work);
void ov70_02239CF8(Ov70R14Work *work);
void ov70_02239D44(Ov70R14Work *work, int message);
int ov70_02239D8C(void *work);

void ov70_02239C6C(Ov70R14Work *work) {
    BgConfig *bgConfig = work->bgConfig;

    GfGfxLoader_GXLoadPal((NarcId)0x64, 5, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0, 0x200, HEAP_ID_61);
    GfGfxLoader_LoadCharData((NarcId)0x64, 0x11, bgConfig, GF_BG_LYR_SUB_1, 0, 0xA800, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadScrnData((NarcId)0x64, 0x28, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, TRUE, HEAP_ID_61);
    FillBgTilemapRect(bgConfig, GF_BG_LYR_SUB_1, 0, 0, 0x18, 0x20, 8, 0x11);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_SUB_1);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x20, HEAP_ID_61);
}

void ov70_02239CF8(Ov70R14Work *work) {
    GfGfxLoader_LoadCharData((NarcId)0x64, 0x12, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_61);
    GfGfxLoader_LoadScrnData((NarcId)0x64, 0x29, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_61);
    Sprite_SetDrawFlag(work->sprite, TRUE);
}

void ov70_02239D44(Ov70R14Work *work, int message) {
    AddWindowParameterized(work->bgConfig, &work->window, 4, 0xD, 0x13, 0x10, 4, 1, 1);
    FillWindowPixelBuffer(&work->window, 0);
    ov70_022450EC(&work->window, work->msgData, message);
}

int ov70_02239D8C(void *work) {
    int value;

    ov70_0223A1E4(work);
    ov70_02239EAC(PTR_AT(work, 4));
    ov70_02239FA4(work);
    ov70_0223A0D4(work);
    ov70_0223A06C(work);
    ov70_0223A578(PTR_AT(work, 0xBA0), PTR_AT(work, 0xBA4), PTR_AT(work, 0xB9C), (u8 *)work + 0x1058, Mon_GetBoxMon((Pokemon *)((u8 *)work + 0x13C)), (u8 *)work + 0x228);
    ov70_0223A72C(PTR_AT(work, 0xBA0), (u8 *)work + 0x10C8, (u8 *)work + 0x248, (u8 *)work + 0x13C, (u8 *)work + 0x1118);
    value = ov70_0223F864(S8_AT(work, 0x22F), S8_AT(work, 0x230), 0);
    ov70_0223F470(PTR_AT(work, 0xBA0), PTR_AT(work, 0xBA4), PTR_AT(work, 0xB9C), (u8 *)work + 0x10E8, S16_AT(work, 0x22C), S8_AT(work, 0x22E), value);
    ov70_0223A7E4((u8 *)work + 0x13C);
    BeginNormalPaletteFade((enum FadeMode)3, (enum FadeType)1, (enum FadeType)1, 0, 6, 1, HEAP_ID_61);
    *(u32 *)((u8 *)work + 0x2C) = 0;
    return 2;
}
