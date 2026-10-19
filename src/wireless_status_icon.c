#include "wireless_status_icon.h"

#include "global.h"

#include "assert.h"
#include "comm_state.h"
#include "comm_wireless.h"
#include "communication_core.h"
#include "heap.h"
#include "sys_task_api.h"
#include "system.h"

typedef struct WirelessStatusIconOam {
    u32 attr01;
    u16 attr2;
} WirelessStatusIconOam;

typedef struct WirelessStatusIconAnimFrame {
    u16 tile;
    u16 duration;
} WirelessStatusIconAnimFrame;

typedef struct WirelessStatusIcon {
    u32 unused;
    s32 level; // 0-3, selects the icon's tile
    u32 animTimer;
    u16 x;
    u16 y;
    u8 isWifi;        // Wi-Fi (pl_wifi) or local wireless (pl_wm) graphics
    u8 screenMode;    // 0 = fixed screen, 1/2 = top/bottom screen, following the LCD swap
    u8 onBothScreens; // the sub-screen OAM slot is cleared too
    const WirelessStatusIconAnimFrame **anims;
    SysTask *task;
    WirelessStatusIconOam *oam; // the OAM entry written last
    u8 palette;
    u8 screen; // 1 = main engine, 2 = sub engine
    u8 priority;
} WirelessStatusIcon;

static WirelessStatusIcon *WirelessStatusIcon_New(int a0, enum HeapID heapId, int x, int y, int isWifi, const WirelessStatusIconAnimFrame **anims, int screen, int plttBase);
static void WirelessStatusIcon_Task(SysTask *task, void *data);
static void WirelessStatusIcon_SetLevelOf(WirelessStatusIcon *p, int level);
static void WirelessStatusIcon_Delete(WirelessStatusIcon *p);
static void WirelessStatusIcon_ShowOnBothScreensOf(WirelessStatusIcon *p, int topFirst, enum HeapID heapId);
static void WirelessStatusIcon_LoadPalette(int engine, int isWifi, u32 plttBase, enum HeapID heapId);
static NNSG2dCharacterData *WirelessStatusIcon_GetCharData(int isWifi, u32 mapping, u32 sz, void *buf);
static void WirelessStatusIcon_LoadChar(int engine, int isWifi, enum HeapID heapId);
static void WirelessStatusIcon_WriteOam(WirelessStatusIcon *p);
static void WirelessStatusIcon_CreateOnScreen(int screen);
static void WirelessStatusIcon_CreateAt(int x, int y, int isWifi, int screen, u32 plttBase);

static const WirelessStatusIconAnimFrame sWirelessStatusIconLevel3Anim[] = {
    { 0x1FC,  20 },
    { 0xFFFF, 0  },
};

static const WirelessStatusIconAnimFrame sWirelessStatusIconLevel0Anim[] = {
    { 0x1F0,  20 },
    { 0xFFFF, 0  },
};

static const WirelessStatusIconAnimFrame sWirelessStatusIconLevel1Anim[] = {
    { 0x1F4,  20 },
    { 0xFFFF, 0  },
};

static const WirelessStatusIconAnimFrame sWirelessStatusIconLevel2Anim[] = {
    { 0x1F8,  20 },
    { 0xFFFF, 0  },
};

static const WirelessStatusIconAnimFrame sWirelessStatusIconCycleAnim[] = {
    { 0x1F0,  20 },
    { 0x1F4,  20 },
    { 0x1F8,  20 },
    { 0x1FC,  20 },
    { 0xFFFF, 0  },
};

static const WirelessStatusIconAnimFrame *sWirelessStatusIconAnims[] = {
    sWirelessStatusIconLevel0Anim,
    sWirelessStatusIconLevel1Anim,
    sWirelessStatusIconLevel2Anim,
    sWirelessStatusIconLevel3Anim,
    sWirelessStatusIconCycleAnim,
};

static WirelessStatusIcon *sWirelessStatusIcon;

static WirelessStatusIcon *WirelessStatusIcon_New(int a0, enum HeapID heapId, int x, int y, int isWifi, const WirelessStatusIconAnimFrame **anims, int screen, int plttBase) {
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

static void WirelessStatusIcon_Task(SysTask *task, void *data) {
    WirelessStatusIcon_WriteOam(data);
}

static void WirelessStatusIcon_SetLevelOf(WirelessStatusIcon *p, int level) {
    if (level < 4) {
        p->level = level;
        p->animTimer = 0;
    }
}

static void WirelessStatusIcon_Delete(WirelessStatusIcon *p) {
    SysTask_Destroy(p->task);
    ((WirelessStatusIconOam *)0x07000000)->attr01 = 0x40000200;
    ((WirelessStatusIconOam *)0x07000000)->attr2 = 0;
    if (p->onBothScreens != 0) {
        ((WirelessStatusIconOam *)0x07000400)->attr01 = 0x40000200;
        ((WirelessStatusIconOam *)0x07000400)->attr2 = 0;
    }
    Heap_Free(p);
}

static void WirelessStatusIcon_ShowOnBothScreensOf(WirelessStatusIcon *p, int topFirst, enum HeapID heapId) {
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
    Sys_ReadFileIntoBuffer("data/pl_wm.NCLR", &buf);
    DC_FlushRange(buf, 0x258);
    return buf;
}

static void WirelessStatusIcon_LoadPalette(int engine, int isWifi, u32 plttBase, enum HeapID heapId) {
    void *buf;
    NNSG2dPaletteData *plttData;

    buf = Heap_AllocAtEnd(heapId, 0x258);
    if (buf != NULL) {
        Sys_ReadFileIntoBuffer("data/pl_wm.NCLR", &buf);
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

static NNSG2dCharacterData *WirelessStatusIcon_GetCharData(int isWifi, u32 mapping, u32 sz, void *buf) {
    NNSG2dCharacterData *charData;

    if (isWifi != 0) {
        GF_ASSERT(mapping != 0x00300010);
        Sys_ReadFileIntoBuffer("data/pl_wifi.NCGR", &buf);
    } else if (mapping != 0x00300010) {
        Sys_ReadFileIntoBuffer("data/pl_wm.NCGR", &buf);
        DC_FlushRange(buf, 0x258);
    } else {
        Sys_ReadFileIntoBuffer("data/wm256k.NCGR", &buf);
        DC_FlushRange(buf, 0x4C0);
    }
    NNS_G2dGetUnpackedBGCharacterData(buf, &charData);
    return charData;
}

static void WirelessStatusIcon_LoadChar(int engine, int isWifi, enum HeapID heapId) {
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

#pragma push
#pragma opt_propagation off

static void WirelessStatusIcon_WriteOam(WirelessStatusIcon *p) {
    int bank;
    s32 tile;
    s32 screen;
    s32 index;
    volatile WirelessStatusIconOam *oam;
    s32 mode;
    u32 pal;
    u8 prio;
    volatile WirelessStatusIconOam *old;
    switch (p->screenMode) {
    case 1:
        if (((reg_GX_POWCNT & REG_GX_POWCNT_DSEL_MASK) >> REG_GX_POWCNT_DSEL_SHIFT) == 1) {
            screen = 1;
        } else {
            screen = 2;
        }
        break;
    case 2:
        if (((reg_GX_POWCNT & REG_GX_POWCNT_DSEL_MASK) >> REG_GX_POWCNT_DSEL_SHIFT) == 1) {
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
        oam = (volatile WirelessStatusIconOam *)HW_OAM;
    } else {
        oam = (volatile WirelessStatusIconOam *)HW_DB_OAM;
    }
    pal = p->palette;
    prio = p->priority;
    oam->attr01 = ((p->x & 511) << 16) | (0x40000000 | ((p->y & 255) | (prio << 10)));
    oam->attr2 = (pal << 12) | tile;
    old = p->oam;
    if (oam != old) {
        old->attr01 = 0x40000200 | (p->priority << 10);
        old->attr2 = 0;
        p->oam = (WirelessStatusIconOam *)oam;
    }
}

#pragma pop

void WirelessStatusIcon_Refresh(void) {
    if (sWirelessStatusIcon != NULL) {
        WirelessStatusIcon_WriteOam(sWirelessStatusIcon);
    }
}

void WirelessStatusIcon_Create(void) {
    int isWifi = 0;

    if (CommState_IsWifi() != 0) {
        isWifi = 1;
    }
    WirelessStatusIcon_CreateAt(0xF0, 0, isWifi, 1, 0x1C0);
}

static void WirelessStatusIcon_CreateOnScreen(int screen) {
    int isWifi = 0;

    if (CommState_IsWifi() != 0) {
        isWifi = 1;
    }
    WirelessStatusIcon_CreateAt(0xF0, 0, isWifi, screen, 0x1E0);
}

static void WirelessStatusIcon_CreateAt(int x, int y, int isWifi, int screen, u32 plttBase) {
    if (CommWireless_IsOn() != 0) {
        if (sWirelessStatusIcon != NULL) {
            WirelessStatusIcon_Destroy();
        }
        sWirelessStatusIcon = WirelessStatusIcon_New(0, (enum HeapID)0x59, x, y, isWifi, sWirelessStatusIconAnims, screen, plttBase);
    }
}

void WirelessStatusIcon_Destroy(void) {
    if (sWirelessStatusIcon != NULL) {
        WirelessStatusIcon_Delete(sWirelessStatusIcon);
        sWirelessStatusIcon = NULL;
    }
}

void WirelessStatusIcon_SetLevel(int level) {
    if (sWirelessStatusIcon != NULL) {
        WirelessStatusIcon_SetLevelOf(sWirelessStatusIcon, level);
    }
}

void WirelessStatusIcon_ShowOnBothScreens(int topFirst, enum HeapID heapId) {
    if (sWirelessStatusIcon != NULL) {
        WirelessStatusIcon_ShowOnBothScreensOf(sWirelessStatusIcon, topFirst, heapId);
    }
}

void WirelessStatusIcon_CreateIfConnected(void) {
    if (Comm_IsActive()) {
        if (Comm_CountConnected() > 1 || CommState_IsWifi() != 0) {
            WirelessStatusIcon_Create();
        }
    } else if (CommState_IsWifi() != 0) {
        WirelessStatusIcon_Create();
    }
}

void WirelessStatusIcon_CreateOnScreenIfConnected(int screen) {
    if (Comm_IsActive()) {
        if (Comm_CountConnected() > 1 || CommState_IsWifi() != 0) {
            WirelessStatusIcon_CreateOnScreen(screen);
        }
    } else if (CommState_IsWifi() != 0) {
        WirelessStatusIcon_CreateOnScreen(screen);
    }
}

void WirelessStatusIcon_SetBackPriority(void) {
    if (sWirelessStatusIcon != NULL) {
        sWirelessStatusIcon->priority = 1;
    }
}
