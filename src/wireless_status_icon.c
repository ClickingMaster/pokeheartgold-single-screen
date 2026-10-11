#include "wireless_status_icon.h"

#include "global.h"

#include "assert.h"
#include "heap.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"

typedef struct WirelessStatusIconOam {
    u32 attr01;
    u16 attr2;
} WirelessStatusIconOam;

typedef struct WirelessStatusIcon {
    u32 unused;
    s32 level; // 0-3, selects the icon's tile
    u32 animTimer;
    u16 x;
    u16 y;
    u8 isWifi;        // Wi-Fi (pl_wifi) or local wireless (pl_wm) graphics
    u8 screenMode;    // 0 = fixed screen, 1/2 = top/bottom screen, following the LCD swap
    u8 onBothScreens; // the sub-screen OAM slot is cleared too
    const u8 **anims;
    SysTask *task;
    WirelessStatusIconOam *oam; // the OAM entry written last
    u8 palette;
    u8 screen; // 1 = main engine, 2 = sub engine
    u8 priority;
} WirelessStatusIcon;

extern const char gWirelessStatusIconNclr[];
extern const char gWirelessStatusIconWifiNcgr[];
extern const char gWirelessStatusIconWmNcgr[];
extern const char gWirelessStatusIconWm256kNcgr[];
extern const u8 *gWirelessStatusIconAnims[];
extern WirelessStatusIcon *gWirelessStatusIcon;

int sub_02034DCC(void);

WirelessStatusIcon *WirelessStatusIcon_New(int a0, enum HeapID heapId, int x, int y, int isWifi, void *anims, int screen, int plttBase);
void WirelessStatusIcon_Task(SysTask *task, void *data);
void WirelessStatusIcon_SetLevelOf(WirelessStatusIcon *p, s32 level);
void WirelessStatusIcon_Delete(WirelessStatusIcon *p);
void WirelessStatusIcon_ShowOnBothScreensOf(WirelessStatusIcon *p, int topFirst, enum HeapID heapId);
void *WirelessStatusIcon_LoadPaletteFile(enum HeapID heapId);
void WirelessStatusIcon_LoadPalette(int engine, int isWifi, u32 plttBase, enum HeapID heapId);
NNSG2dCharacterData *WirelessStatusIcon_GetCharData(int isWifi, u32 mapping, u32 sz, void *buf);
void WirelessStatusIcon_LoadChar(int engine, int isWifi, enum HeapID heapId);
void WirelessStatusIcon_WriteOam(WirelessStatusIcon *p);
void WirelessStatusIcon_Refresh(void);
void WirelessStatusIcon_CreateOnScreen(int screen);
void WirelessStatusIcon_CreateAt(int x, int y, int isWifi, int screen, u32 plttBase);
void WirelessStatusIcon_SetLevel(s32 level);
void WirelessStatusIcon_ShowOnBothScreens(int topFirst, enum HeapID heapId);
void WirelessStatusIcon_CreateOnScreenIfConnected(int screen);
void WirelessStatusIcon_SetBackPriority(void);

WirelessStatusIcon *WirelessStatusIcon_New(int a0, enum HeapID heapId, int x, int y, int isWifi, void *anims, int screen, int plttBase) {
    WirelessStatusIcon *p;

    WirelessStatusIcon_LoadPalette(screen, isWifi, plttBase, heapId);
    WirelessStatusIcon_LoadChar(screen, isWifi, heapId);
    p = Heap_AllocAtEnd(heapId, sizeof(WirelessStatusIcon));
    p->task = SysTask_CreateOnVWaitQueue(WirelessStatusIcon_Task, p, 5);
    p->x = x;
    p->y = y;
    p->unused = 0;
    p->palette = plttBase / 32;
    p->screen = screen;
    p->priority = 0;
    p->level = 3;
    p->anims = anims;
    p->animTimer = 0;
    p->isWifi = isWifi;
    p->onBothScreens = 0;
    p->screenMode = 0;
    p->oam = (WirelessStatusIconOam *)0x07000000;
    return p;
}

void WirelessStatusIcon_Task(SysTask *task, void *data) {
    WirelessStatusIcon_WriteOam(data);
}

void WirelessStatusIcon_SetLevelOf(WirelessStatusIcon *p, s32 level) {
    if (level < 4) {
        p->level = level;
        p->animTimer = 0;
    }
}

void WirelessStatusIcon_Delete(WirelessStatusIcon *p) {
    SysTask_Destroy(p->task);
    ((WirelessStatusIconOam *)0x07000000)->attr01 = 0x40000200;
    ((WirelessStatusIconOam *)0x07000000)->attr2 = 0;
    if (p->onBothScreens != 0) {
        ((WirelessStatusIconOam *)0x07000400)->attr01 = 0x40000200;
        ((WirelessStatusIconOam *)0x07000400)->attr2 = 0;
    }
    Heap_Free(p);
}

void WirelessStatusIcon_ShowOnBothScreensOf(WirelessStatusIcon *p, int topFirst, enum HeapID heapId) {
    WirelessStatusIcon_LoadPalette(2, p->isWifi, 0x1C0, heapId);
    WirelessStatusIcon_LoadChar(2, p->isWifi, heapId);
    if (topFirst != 0) {
        p->screenMode = 1;
    } else {
        p->screenMode = 2;
    }
    p->onBothScreens = 1;
}

void *WirelessStatusIcon_LoadPaletteFile(enum HeapID heapId) {
    void *buf;

    buf = Heap_AllocAtEnd(heapId, 0x258);
    sub_0201A3F8(gWirelessStatusIconNclr, &buf);
    DC_FlushRange(buf, 0x258);
    return buf;
}

void WirelessStatusIcon_LoadPalette(int engine, int isWifi, u32 plttBase, enum HeapID heapId) {
    void *buf;
    NNSG2dPaletteData *plttData;

    buf = Heap_AllocAtEnd(heapId, 0x258);
    if (buf != NULL) {
        sub_0201A3F8(gWirelessStatusIconNclr, &buf);
        DC_FlushRange(buf, 0x258);
        NNS_G2dGetUnpackedPaletteData(buf, &plttData);
        if (engine == 1) {
            GX_LoadOBJPltt(plttData->pRawData, plttBase, 0x20);
        } else {
            GXS_LoadOBJPltt(plttData->pRawData, plttBase, 0x20);
        }
        Heap_Free(buf);
    }
}

NNSG2dCharacterData *WirelessStatusIcon_GetCharData(int isWifi, u32 mapping, u32 sz, void *buf) {
    NNSG2dCharacterData *charData;

    if (isWifi != 0) {
        GF_ASSERT(mapping != 0x00300010);
        sub_0201A3F8(gWirelessStatusIconWifiNcgr, &buf);
    } else if (mapping != 0x00300010) {
        sub_0201A3F8(gWirelessStatusIconWmNcgr, &buf);
        DC_FlushRange(buf, 0x258);
    } else {
        sub_0201A3F8(gWirelessStatusIconWm256kNcgr, &buf);
        DC_FlushRange(buf, 0x4C0);
    }
    NNS_G2dGetUnpackedBGCharacterData(buf, &charData);
    return charData;
}

void WirelessStatusIcon_LoadChar(int engine, int isWifi, enum HeapID heapId) {
    s32 mapping;
    u32 offs;
    NNSG2dCharacterData *charData;
    void *buf;

    if (engine == 1) {
        mapping = *(u32 *)0x04000000 & 0x00300010;
        offs = GX_GetBankForOBJ();
    } else {
        mapping = *(u32 *)0x04001000 & 0x00300010;
        offs = GX_GetBankForSubOBJ();
    }
    if (mapping == 0x00300010) {
        buf = Heap_AllocAtEnd(heapId, 0x4C0);
    } else {
        buf = Heap_AllocAtEnd(heapId, 0x258);
    }
    if (buf == NULL) {
        return;
    }
    charData = WirelessStatusIcon_GetCharData(isWifi, mapping, heapId, buf);
    switch (mapping) {
    case 0x10:
        if (offs == 0x40 || offs == 0x20) {
            offs = 0x3E00;
        } else {
            offs = 0x7E00;
        }
        break;
    case 0x00100010:
        if (offs == 0x40 || offs == 0x20 || offs == (0x00100010 >> 12)) {
            offs = 0x3E00;
        } else {
            offs = 0xFE00;
        }
        break;
    case 0x00200010:
        if (offs == 0x30 || offs == 0x50) {
            offs = 0x00013E00;
        } else if (offs == 0x10) {
            offs = 0xFE00;
        } else {
            offs = 0x0001FE00;
        }
        break;
    case 0x00300010:
        offs = 0x3FC00;
        break;
    default:
        offs = 0xFE00;
        break;
    }
    if (engine == 1) {
        if (mapping != 0x00300010) {
            DC_FlushRange(charData->pRawData, 0x200);
            GX_LoadOBJ(charData->pRawData, offs, 0x200);
        } else {
            DC_FlushRange(charData->pRawData, 0x400);
            GX_LoadOBJ(charData->pRawData, offs, 0x400);
        }
    } else {
        if (mapping != 0x00300010) {
            DC_FlushRange(charData->pRawData, 0x200);
            GXS_LoadOBJ(charData->pRawData, offs, 0x200);
        } else {
            DC_FlushRange(charData->pRawData, 0x400);
            GXS_LoadOBJ(charData->pRawData, 0x3C00, 0x400);
        }
    }
    Heap_Free(buf);
}

#pragma force_active on
#pragma push
#pragma opt_propagation off

void WirelessStatusIcon_WriteOam(WirelessStatusIcon *p) {
    int bank;
    s32 tile;
    s32 screen;
    s32 index;
    volatile WirelessStatusIconOam *oam;
    s32 mode;
    u32 pal;
    volatile WirelessStatusIconOam *old;
    switch (p->screenMode) {
    case 1:
        if (((*(volatile u16 *)0x4000304 & 0x8000) >> 15) == 1) {
            screen = 1;
        } else {
            screen = 2;
        }
        break;
    case 2:
        if (((*(volatile u16 *)0x4000304 & 0x8000) >> 15) == 1) {
            screen = 2;
        } else {
            screen = 1;
        }
        break;
    default:
        screen = p->screen;
        break;
    }
    index = p->level;
    if (screen == 1) {
        mode = GX_GetOBJVRamModeChar();
        bank = GX_GetBankForOBJ();
    } else {
        mode = GXS_GetOBJVRamModeChar();
        bank = GX_GetBankForSubOBJ();
    }
    switch (mode) {
    case 0x10:
        if (bank == 64 || bank == 32) {
            tile = 4 * index + 496;
        } else {
            tile = 4 * index + 1008;
        }
        break;
    case 0x100010:
        if (bank == 64 || bank == 32 || bank == 256) {
            tile = 2 * index + 504;
        } else {
            goto fallback;
        }
        break;
    case 0x200010:
        if (bank == 48 || bank == 80) {
            tile = index + 636;
        } else {
            if (bank == 16) {
                tile = index + 508;
            } else {
                tile = index + 1020;
            }
        }
        break;
    case 0x300010:
        if (bank == 3) {
            tile = 1020 + index;
        } else {
            tile = index + 508;
        }
        break;
    default:
fallback:
        tile = 2 * index + 1016;
        break;
    }
    if (screen == 1) {
        oam = (volatile WirelessStatusIconOam *)0x7000000;
    } else {
        oam = (volatile WirelessStatusIconOam *)0x7000400;
    }
    pal = p->palette;
    {
        u8 prio = p->priority;
        oam->attr01 = ((p->x & 511) << 16) | (0x40000000 | ((p->y & 255) | (prio << 10)));
    }
    oam->attr2 = (pal << 12) | tile;
    old = p->oam;
    if (oam != old) {
        old->attr01 = 0x40000200 | (p->priority << 10);
        old->attr2 = 0;
        p->oam = (WirelessStatusIconOam *)oam;
    }
}

#pragma pop
#pragma force_active off

void WirelessStatusIcon_Refresh(void) {
    if (gWirelessStatusIcon != NULL) {
        WirelessStatusIcon_WriteOam(gWirelessStatusIcon);
    }
}

void WirelessStatusIcon_Create(void) {
    int isWifi = 0;

    if (sub_02039998() != 0) {
        isWifi = 1;
    }
    WirelessStatusIcon_CreateAt(0xF0, 0, isWifi, 1, 0x1C0);
}

void WirelessStatusIcon_CreateOnScreen(int screen) {
    int isWifi = 0;

    if (sub_02039998() != 0) {
        isWifi = 1;
    }
    WirelessStatusIcon_CreateAt(0xF0, 0, isWifi, screen, 0x1E0);
}

void WirelessStatusIcon_CreateAt(int x, int y, int isWifi, int screen, u32 plttBase) {
    if (sub_02034DCC() != 0) {
        if (gWirelessStatusIcon != NULL) {
            WirelessStatusIcon_Destroy();
        }
        gWirelessStatusIcon = WirelessStatusIcon_New(0, (enum HeapID)0x59, x, y, isWifi, gWirelessStatusIconAnims, screen, plttBase);
    }
}

void WirelessStatusIcon_Destroy(void) {
    if (gWirelessStatusIcon != NULL) {
        WirelessStatusIcon_Delete(gWirelessStatusIcon);
        gWirelessStatusIcon = NULL;
    }
}

void WirelessStatusIcon_SetLevel(s32 level) {
    if (gWirelessStatusIcon != NULL) {
        WirelessStatusIcon_SetLevelOf(gWirelessStatusIcon, level);
    }
}

void WirelessStatusIcon_ShowOnBothScreens(int topFirst, enum HeapID heapId) {
    if (gWirelessStatusIcon != NULL) {
        WirelessStatusIcon_ShowOnBothScreensOf(gWirelessStatusIcon, topFirst, heapId);
    }
}

void WirelessStatusIcon_CreateIfConnected(void) {
    if (sub_02037474()) {
        if (Comm_CountConnected() > 1 || sub_02039998() != 0) {
            WirelessStatusIcon_Create();
        }
    } else if (sub_02039998() != 0) {
        WirelessStatusIcon_Create();
    }
}

void WirelessStatusIcon_CreateOnScreenIfConnected(int screen) {
    if (sub_02037474()) {
        if (Comm_CountConnected() > 1 || sub_02039998() != 0) {
            WirelessStatusIcon_CreateOnScreen(screen);
        }
    } else if (sub_02039998() != 0) {
        WirelessStatusIcon_CreateOnScreen(screen);
    }
}

void WirelessStatusIcon_SetBackPriority(void) {
    if (gWirelessStatusIcon != NULL) {
        gWirelessStatusIcon->priority = 1;
    }
}
