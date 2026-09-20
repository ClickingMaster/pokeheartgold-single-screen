#include <nitro/gx/gxcommon.h>

#include "global.h"

#include "application/main_menu/main_menu.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "main.h"
#include "mystery_gift.h"
#include "overlay_manager.h"
#include "save.h"
#include "save_arrays.h"

typedef struct Ov74CardInfo {
    u8 pad00[0x48];
    u32 flags;
    u16 id;
    union {
        u8 raw;
        struct {
            u8 bit0 : 1;
            u8 bit1 : 1;
            u8 bit2 : 1;
            u8 bit3 : 1;
            u8 bit4 : 1;
            u8 bit5 : 1;
            u8 bit6 : 1;
            u8 bit7 : 1;
        } bits;
    } attr;
} Ov74CardInfo;

typedef struct Ov74TempWork {
    int heapId;
    u8 pad004[0x29F8];
    BgConfig *bgConfig;
    u8 pad2A00[0x1B4];
    void *card;
    u8 pad2BB8[0x1154];
} Ov74TempWork;

typedef struct Ov74ExitWork {
    u8 pad0000[0x29FC];
    BgConfig *bgConfig;
    u8 pad2A00[8];
    Window windows[11];
    u8 pad2AB8[0x10C];
    Window windowA;
    Window windowB;
} Ov74ExitWork;

extern const OverlayManagerTemplate gApp_MainMenu_SelectOption_MysteryGift;

int ov74_0222FB8C(OverlayManager *manager);
extern const GraphicsModes _0223B430;
extern const BgTemplate _0223B440;
extern const BgTemplate _0223B45C;
extern const BgTemplate _0223B478;
extern const BgTemplate _0223B494;
extern const GraphicsBanks _0223B4B0;

extern void SetBothScreensModesAndDisable(const GraphicsModes *);
extern void *memset(void *, int, u32);
extern void LoadFontPal0(int, int, int);
extern void ov74_0222F024(void *, int, int);

void ov74_0222FC50(BgConfig *bgConfig, void *card, int heapId);
void ov74_0222FCA4(void);
void ov74_0222FCC4(BgConfig *bgConfig);
int ov74_0222FD98(SaveData *saveData, Ov74CardInfo *info);

void ov74_0222FCA4(void) {
    GraphicsBanks banks = _0223B4B0;
    GfGfx_SetBanks(&banks);
}
void ov74_0222FCC4(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg2;
    BgTemplate bg3;

    modes = _0223B430;
    SetBothScreensModesAndDisable(&modes);
    bg0 = _0223B45C;
    InitBgFromTemplate(bgConfig, 0, &bg0, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 0);
    bg1 = _0223B494;
    InitBgFromTemplate(bgConfig, 1, &bg1, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 1);
    bg2 = _0223B478;
    InitBgFromTemplate(bgConfig, 2, &bg2, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 2);
    bg3 = _0223B440;
    InitBgFromTemplate(bgConfig, 3, &bg3, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 3);
}
