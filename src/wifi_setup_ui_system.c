#include <string.h>

#include "global.h"

#include "wifi_setup_ui.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#pragma force_active on

typedef struct WifiSetupOamBlockManager {
    u8 blocks[0x20][0x10];
    WifiSetupOamBlock first;
    WifiSetupOamBlock last;
    WifiSetupList *list;
    WifiSetupPool *pool;
} WifiSetupOamBlockManager;

// Header of one template in an OAM template file: count entries of 6 bytes at data + offset.
typedef struct WifiSetupOamTemplateHeader {
    u16 count;
    u16 unk_02;
    u32 offset;
} WifiSetupOamTemplateHeader;

typedef struct WifiSetupFade {
    void *task;
    s16 counter;
    u16 duration;
    u8 kind;
    u8 active;
} WifiSetupFade;

typedef struct WifiSetupBrightnessTable {
    s8 values[4];
} WifiSetupBrightnessTable;

typedef struct WifiSetupTextDirection {
    s8 x;
    s8 y;
} WifiSetupTextDirection;

typedef struct WifiSetupTextState {
    WifiSetupFont fonts[2];
    WifiSetupCanvas objCanvases[0x20];
    WifiSetupCanvas bgCanvases[2];
    WifiSetupPool *objCanvasPool;
    void *fontData[2];
    u8 bgDirty[2];
} WifiSetupTextState;

typedef struct WifiSetupOamBuffer {
    GXOamAttr oam[2][0x80];
    WifiSetupPool *pools[2];
    void *task;
} WifiSetupOamBuffer;

typedef struct WifiSetupCharBlock {
    WifiSetupListNode node;
    u16 start;
    u16 size;
} WifiSetupCharBlock;

typedef struct WifiSetupCharVramManager {
    u8 blocks[0x20][0xC];
    WifiSetupCharBlock first;
    WifiSetupCharBlock last;
    WifiSetupList *list;
    WifiSetupPool *pool;
} WifiSetupCharVramManager;

typedef struct WifiSetupVramBanks {
    GXVRamBG bg;
    GXVRamOBJ obj;
    GXVRamBGExtPltt bgExtPltt;
    GXVRamOBJExtPltt objExtPltt;
    GXVRamTex tex;
    GXVRamTexPltt texPltt;
    GXVRamClearImage clearImage;
    GXVRamSubBG subBg;
    GXVRamSubOBJ subObj;
    GXVRamSubBGExtPltt subBgExtPltt;
    GXVRamSubOBJExtPltt subObjExtPltt;
    int arm7;
    GXVRamLCDC lcdc;
} WifiSetupVramBanks;

typedef struct WifiSetupTaskQueue {
    WifiSetupPool *taskPool;
    WifiSetupPool *destroyQueue;
    WifiSetupList *list;
    WifiSetupTask head;
    WifiSetupTask tail;
    int capacity;
    u8 enabled;
    void *taskBuf;
} WifiSetupTaskQueue;

typedef struct WifiSetupInputState {
    TPData samples[5];
    WifiSetupPoint touch;
    WifiSetupPoint prevTouch;
    u16 cont;
    u16 trig;
    u16 repeat;
    u16 release;
    u8 touching : 1;
    u8 touchTrig : 1;
    u8 touchRepeat : 1;
    u8 touchRelease : 1;
} WifiSetupInputState;

typedef struct WifiSetupInput {
    u8 lcdOff;
    u8 touchHoldFrames;
    u8 unk_02[2];
    WifiSetupInputState *state;
} WifiSetupInput;

typedef struct WifiSetupIrqSave {
    OSIrqFunction vblankFunc;
    OSIrqMask mask;
} WifiSetupIrqSave;

// The address of a global of this module, formed from the module's first .bss label so that it gets
// its own literal-pool slot, as in the original code.
#define MODULE_BSS_ADDR(addr) ((void *)(gWifiSetupShare + ((addr) - 0x0224F5C8)))

static inline int GX_GetVisiblePlane13(void) {
    return (int)((reg_GX_DISPCNT & 0x1F00) >> 8);
}

static inline int GXS_GetVisiblePlane13(void) {
    return (int)((reg_GXS_DB_DISPCNT & 0x1F00) >> 8);
}

static inline void OamSetPosition(GXOamAttr *oam, int x, int y) {
    oam->attr01 = (oam->attr01 & 0xFE00FF00) | (y & 0xFF) | ((x & 0x1FF) << 16);
}

static inline void OamGetPosition(const GXOamAttr *oam, s32 *x, s32 *y) {
    *x = (oam->attr01 & 0x01FF0000) >> 16;
    *y = oam->attr01 & 0xFF;
}

extern char gWifiSetupShare[];
extern WifiSetupArchiveState *gWifiSetupArchive;
extern WifiSetupOamBlockManager *gWifiSetupOamBlockManager;
extern void *gWifiSetupOamTemplateFiles[];
extern WifiSetupFade *gWifiSetupFades;
extern WifiSetupTextState *gWifiSetupText;
extern NNSFndHeapHandle gWifiSetupHeap;
extern WifiSetupInput gWifiSetupInput;
extern u8 gWifiSetupInputHoldFrames[];
extern WifiSetupIrqSave gWifiSetupIrqSave;
extern WifiSetupOamBuffer *gWifiSetupOamBuffer;
extern WifiSetupCharVramManager *gWifiSetupCharVram;
extern WifiSetupVramBanks gWifiSetupSavedVramBanks;
extern WifiSetupTaskQueue *gWifiSetupTaskQueues;
extern u8 gWifiSetupFadeTables[];
extern const u16 gWifiSetupTextScreenCharBases[];
extern const u16 gWifiSetupTextScreenWidths[];
extern const u16 gWifiSetupTextScreenHeights[];
extern u8 gWifiSetupTaskQueueCapacities[];
extern char gWifiSetupRomPrefix[];
extern char gWifiSetupCompressedSuffix[];
extern char *gWifiSetupFontPathsKorean[];
extern char *gWifiSetupFontPaths[];

int WifiSetupApp_GetLanguage(void);
fx32 FX_DivS32(fx32 a, fx32 b);
int GX_DisableBankForARM7(void);
void GX_SetBankForARM7(int bank);
int PM_GetLEDPattern(u32 *pattern);
void PMi_SendLEDPatternCommand(int pattern);
BOOL PM_SetLCDPower(int on);
void TP_RequestAutoSamplingStartAsync(u16 vcount, u16 freq, TPData *bufs, u16 size);
void TP_RequestAutoSamplingStopAsync(void);
void TP_WaitBusy(u32 flag);
u32 TP_CheckBusy(u32 flag);
u16 TP_GetLatestIndexInAuto(void);
void NNS_G2dFontInitUTF16(WifiSetupFont *font, void *data);
void NNSi_G2dTextCanvasDrawText(WifiSetupTextCanvas *textCanvas, int x, int y, int color, u32 flags, const u16 *text, WifiSetupTextDirection dir);
void NNSi_G2dTextCanvasDrawTextRect(WifiSetupTextCanvas *textCanvas, int x, int y, int width, int height, int color, u32 flags, const u16 *text, WifiSetupTextDirection dir);
void NNS_G2dCharCanvasDrawChar(void *charCanvas, WifiSetupFont *font, int x, int y, int color, u16 ch);
u16 NNS_G2dFontFindGlyphIndex(WifiSetupFont *font, u16 ch);
WifiSetupCharWidths *NNS_G2dFontGetCharWidthsFromIndex(WifiSetupFont *font, u16 index);
void NNS_G2dArrangeOBJ1D(GXOamAttr *oam, int width, int height, int x, int y, int color, int charNo, int vramMode);
int NNSi_G2dCalcRequiredOBJ(int width, int height);
void NNS_G2dCharCanvasInitForOBJ1D(void *charCanvas, void *charBase, int width, int height, int colorMode);
void NNS_G2dCharCanvasInitForBG(void *charCanvas, void *charBase, u32 width, u32 height, int colorMode);
void NNS_G2dMapScrToCharText(void *screen, u32 width, u32 height, u32 left, u32 top, u32 screenWidth, u32 charNo, u32 palette);

int WifiSetupArchive_Proc(FSArchive *arc, int command);
int WifiSetupArchive_Read(FSArchive *arc, void *dst, u32 pos, u32 len);
void WifiSetupArchive_ReadDone(void *arg);
int WifiSetupArchive_Write(void);
void WifiSetupFade_Task(void *task, WifiSetupFade *fade);
void WifiSetupFade_DelayTask(void *task, WifiSetupFade *fade);
void WifiSetupText_BgTransferTask(void *task, u8 *dirty);
void WifiSetupText_FreeCanvas(WifiSetupCanvas *canvas);
void WifiSetupInput_UpdatePad(void);
void WifiSetupInput_UpdateTouch(void);
void WifiSetup_VBlankIntr(void);
void WifiSetupOam_TransferTask(void);
void WifiSetupTask_DestroyNow(int queue, WifiSetupTask *task);

void WifiSetupArchive_Unload(void) {
    FS_ChangeDir(gWifiSetupRomPrefix);
    FS_UnloadArchiveTables(&gWifiSetupArchive->arc);
    FS_UnloadArchive(&gWifiSetupArchive->arc);
    FS_ReleaseArchiveName(&gWifiSetupArchive->arc);
    OS_ReleaseLockID(gWifiSetupArchive->lockId);
    gWifiSetupArchive->lockId = 0;
    WifiSetupHeap_FreeAndClear(&gWifiSetupArchive->tables);
    gWifiSetupArchive->tables = NULL;
    WifiSetupHeap_FreeAndClear(MODULE_BSS_ADDR(0x0224F5CC));
}

int WifiSetupArchive_Proc(FSArchive *arc, int command) {
    switch (command) {
    case 9:
        CARD_LockRom(gWifiSetupArchive->lockId);
        return 0;
    case 10:
        CARD_UnlockRom(gWifiSetupArchive->lockId);
        return 0;
    case 1:
        return 4;
    default:
        return 8;
    }
}

int WifiSetupArchive_Read(FSArchive *arc, void *dst, u32 pos, u32 len) {
    CARDi_ReadRom((u32)-1, (const void *)(pos + arc->base), dst, len, WifiSetupArchive_ReadDone, arc, TRUE);
    return 6;
}

void WifiSetupArchive_ReadDone(void *arg) {
    FS_NotifyArchiveAsyncEnd(arg, FS_RESULT_SUCCESS);
}

int WifiSetupArchive_Write(void) {
    return 1;
}

void *WifiSetupFile_Load(const char *path, u32 *outSize, int align) {
    void *buf;
    FSFile file;
    u32 size;
    int a;
    void *out;

    WifiSetupPool_Pop(gWifiSetupArchive->filePool);
    FS_InitFile(&file);
    if (!FS_OpenFile(&file, path)) {
        OS_Terminate();
    }
    size = FS_GetLength(&file);
    if (outSize != NULL) {
        *outSize = size;
    }
    // compressed files are loaded with a negative alignment (from the end of the heap)
    if (WifiSetup_HasSameSuffix(path, gWifiSetupCompressedSuffix, 2)) {
        a = -4;
    } else {
        a = align;
    }
    buf = WifiSetupHeap_AllocZeroed(size, a);
    FS_ReadFile(&file, buf, size);
    FS_CloseFile(&file);
    if (a > 0) {
        return buf;
    }
    size = *(u32 *)buf >> 8;
    if (outSize != NULL) {
        *outSize = size;
    }
    out = WifiSetupHeap_AllocZeroed(size, align);
    MI_UncompressLZ8(buf, out);
    WifiSetupHeap_FreeAndClear(&buf);
    return out;
}

void WifiSetupFile_Free(void *data) {
    WifiSetupHeap_FreeAndClear(&data);
    WifiSetupPool_Push(gWifiSetupArchive->filePool, data);
}

BOOL WifiSetup_HasSameSuffix(const char *a, const char *b, int len) {
    int lenA = strlen(a);
    int lenB = strlen(b);
    if (lenA < len || lenB < len) {
        return FALSE;
    }
    return memcmp(a + (lenA - len), b + (lenB - len), len) == 0;
}

void WifiSetupOamBlock_InitManager(void) {
    int i;
    gWifiSetupOamBlockManager = WifiSetupHeap_AllocZeroed(0x450, 4);
    for (i = 0; i < 2; i++) {
        gWifiSetupOamBlockManager[i].pool = WifiSetupPool_NewWithItems(0x20, gWifiSetupOamBlockManager[i].blocks, 0x10);
        gWifiSetupOamBlockManager[i].list = WifiSetupList_New();
        gWifiSetupOamBlockManager[i].first.oam = WifiSetupOam_Get(i, 0x40);
        gWifiSetupOamBlockManager[i].last.oam = WifiSetupOam_Get(i, 0x7F) + 1;
        WifiSetupList_Prepend(gWifiSetupOamBlockManager[i].list, &gWifiSetupOamBlockManager[i].first);
        WifiSetupList_Append(gWifiSetupOamBlockManager[i].list, &gWifiSetupOamBlockManager[i].last);
    }
}

void WifiSetupOamBlock_FreeManager(void) {
    int i;
    for (i = 0; i < 2; i++) {
        WifiSetupList_Delete(gWifiSetupOamBlockManager[i].list);
        WifiSetupPool_Delete(gWifiSetupOamBlockManager[i].pool);
    }
    WifiSetupHeap_FreeAndClear(MODULE_BSS_ADDR(0x0224F5D0));
}

WifiSetupOamBlock *WifiSetupOamBlock_Alloc(int screen, int count, int fromFront) {
    WifiSetupOamBlock *block;
    WifiSetupOamBlock *cur;
    int irq;

    block = WifiSetupPool_Pop(gWifiSetupOamBlockManager[screen].pool);
    irq = OS_DisableIrqMask(OS_IE_V_BLANK);
    if (fromFront) {
        cur = &gWifiSetupOamBlockManager[screen].first;
        while (cur != &gWifiSetupOamBlockManager[screen].last) {
            WifiSetupOamBlock *next = (WifiSetupOamBlock *)cur->node.next;
            GXOamAttr *start = cur->oam + cur->count;
            if (start + count <= next->oam) {
                block->oam = start;
                WifiSetupList_InsertBefore(next, block);
                break;
            }
            cur = next;
        }
        if (cur == &gWifiSetupOamBlockManager[screen].last) {
            OS_Terminate();
        }
    } else {
        cur = &gWifiSetupOamBlockManager[screen].last;
        while (cur != &gWifiSetupOamBlockManager[screen].first) {
            WifiSetupOamBlock *prev = (WifiSetupOamBlock *)cur->node.prev;
            GXOamAttr *start = cur->oam - count;
            if (start >= prev->oam + prev->count) {
                block->oam = start;
                WifiSetupList_InsertBefore(cur, block);
                break;
            }
            cur = prev;
        }
        if (cur == &gWifiSetupOamBlockManager[screen].first) {
            OS_Terminate();
        }
    }
    OS_EnableIrqMask(irq);
    block->count = count;
    return block;
}

void WifiSetupOamBlock_Free(WifiSetupOamBlock *block) {
    int screen = 0;
    GXOamAttr *oam = block->oam;
    int i;
    for (i = 0; i < block->count; i++) {
        oam->attr01 = (oam->attr01 & 0xC1FFFCFF) | 0x200; // hide
        oam++;
    }
    WifiSetupList_Remove(block);
    if ((u32)block >= (u32)&gWifiSetupOamBlockManager[1]) {
        screen = 1;
    }
    WifiSetupPool_Push(gWifiSetupOamBlockManager[screen].pool, block);
}

GXOamAttr *WifiSetupOamBlock_GetOam(WifiSetupOamBlock *block, int index) {
    return &block->oam[index];
}

u8 WifiSetupOamBlock_GetCount(WifiSetupOamBlock *block) {
    return block->count;
}

// index < 0: every entry of the block. Modes 0x100 and 0x300 (affine) also take the affine parameter.
void WifiSetupOamBlock_SetAffineMode(WifiSetupOamBlock *block, int index, u32 mode, u32 param) {
    GXOamAttr *oam = block->oam;
    if (index >= 0) {
        if (mode != 256 && mode != 768) {
            u32 value = oam[index].attr01 & 0xc1fffcff;
            value |= mode;
            oam[index].attr01 = value;
        } else {
            u32 value = oam[index].attr01 & 0xc1fffcff;
            value |= mode;
            value |= param << 25;
            oam[index].attr01 = value;
        }
        return;
    }
    for (index = 0; index < block->count; index++) {
        u32 value;
        if (mode != 256 && mode != 768) {
            value = oam[index].attr01 & 0xc1fffcff;
            value |= mode;
        } else {
            {
                value = oam[index].attr01 & 0xc1fffcff;
                value |= mode;
            }
            value |= param << 25;
        }
        oam[index].attr01 = value;
    }
}

void WifiSetupOamBlock_SetModeAndPalette(WifiSetupOamBlock *block, int index, u32 mode, u32 palette) {
    GXOamAttr *oam = block->oam;
    int i;
    if (index >= 0) {
        oam[index].attr01 = (oam[index].attr01 & ~0xC00) | (mode << 10);
        oam[index].attr2 = (oam[index].attr2 & ~0xF000) | (palette << 12);
        return;
    }
    for (i = 0; i < block->count; i++) {
        oam[i].attr01 = (oam[i].attr01 & ~0xC00) | (mode << 10);
        oam[i].attr2 = (oam[i].attr2 & ~0xF000) | (palette << 12);
    }
}

// index < 0: moves the whole block so that its first entry lands on (x, y).
void WifiSetupOamBlock_SetPosition(WifiSetupOamBlock *block, int index, int x, int y) {
    GXOamAttr *oam = block->oam;
    if (index >= 0) {
        OamSetPosition(&oam[index], x, y);
    } else {
        int oldX;
        int oldY;
        int curX;
        int curY;
        int i;
        int dx;
        int dy;
        OamGetPosition(&oam[0], (s32 *)&oldX, (s32 *)&oldY);
        OamSetPosition(&oam[0], x, y);
        dx = x - oldX;
        dy = y - oldY;
        for (i = 1; i < block->count; i++) {
            OamGetPosition(&oam[i], (s32 *)&curX, (s32 *)&curY);
            OamSetPosition(&oam[i], curX + dx, curY + dy);
        }
    }
}

void WifiSetupOamBlock_SetPriority(WifiSetupOamBlock *block, int index, u32 priority) {
    GXOamAttr *oam = block->oam;
    int i;
    if (index >= 0) {
        oam[index].attr2 = (oam[index].attr2 & ~0xC00) | (priority << 10);
        return;
    }
    for (i = 0; i < block->count; i++) {
        oam[i].attr2 = (oam[i].attr2 & ~0xC00) | (priority << 10);
    }
}

void WifiSetupOamBlock_GetPosition(WifiSetupOamBlock *block, int index, int *x, int *y) {
    GXOamAttr *oam = block->oam;
    *x = (oam[index].attr01 & 0x01FF0000) >> 16;
    *y = oam[index].attr01 & 0xFF;
}

void WifiSetupOamTemplate_Load(int slot, const char *path) {
    u32 size;
    gWifiSetupOamTemplateFiles[slot] = WifiSetupFile_Load(path, &size, 4);
}

void WifiSetupOamTemplate_Free(int slot) {
    WifiSetupFile_Free(gWifiSetupOamTemplateFiles[slot]);
    gWifiSetupOamTemplateFiles[slot] = NULL;
}

void WifiSetupOamTemplate_Copy(int slot, int index, GXOamAttr *dst) {
    u8 *data = gWifiSetupOamTemplateFiles[slot];
    u8 *src = data + ((WifiSetupOamTemplateHeader *)data)[index].offset;
    u16 count = ((WifiSetupOamTemplateHeader *)data)[index].count;
    GXOamAttr tmp;
    int i;
    MIi_CpuClear32(0, (void *)&tmp, sizeof(GXOamAttr));
    for (i = 0; i < count; i++) {
        MIi_CpuCopy16((void *)src, (void *)&tmp, 6);
        MIi_CpuCopy32((void *)&tmp, (void *)dst, sizeof(GXOamAttr));
        src += 6;
        dst++;
    }
}

GXOamAttr *WifiSetupOamTemplate_AllocOam(int slot, int index) {
    GXOamAttr *oam = WifiSetupOam_Alloc(slot);
    WifiSetupOamTemplate_Copy(slot, index, oam);
    return oam;
}

WifiSetupOamBlock *WifiSetupOamTemplate_AllocBlock(int slot, int index, int fromFront) {
    WifiSetupOamBlock *block = WifiSetupOamBlock_Alloc(slot, ((WifiSetupOamTemplateHeader *)gWifiSetupOamTemplateFiles[slot])[index].count, fromFront);
    WifiSetupOamTemplate_Copy(slot, index, WifiSetupOamBlock_GetOam(block, 0));
    return block;
}

WifiSetupPool *WifiSetupPool_New(int capacity) {
    WifiSetupPool *pool = WifiSetupHeap_AllocZeroed((capacity + 1) * 4 + 8, 4);
    pool->size = capacity + 1;
    pool->head = 0;
    pool->tail = 0;
    return pool;
}

WifiSetupPool *WifiSetupPool_NewWithItems(int count, void *items, int stride) {
    WifiSetupPool *pool = WifiSetupPool_New(count);
    int i;
    for (i = 0; i < count; i++) {
        pool->items[i] = items;
        items = (u8 *)items + stride;
    }
    pool->tail = count;
    return pool;
}

void WifiSetupPool_Delete(WifiSetupPool *pool) {
    WifiSetupHeap_FreeAndClear(&pool);
}

void *WifiSetupPool_Push(WifiSetupPool *pool, void *item) {
    int irq = OS_DisableIrqMask(OS_IE_V_BLANK);
    int next = FX_ModS32(pool->tail + 1, pool->size);
    if (next == pool->head) {
        OS_Terminate();
    }
    pool->items[pool->tail] = item;
    pool->tail = next;
    OS_EnableIrqMask(irq);
}

void *WifiSetupPool_Pop(WifiSetupPool *pool) {
    void *ret = NULL;
    int irq = OS_DisableIrqMask(OS_IE_V_BLANK);
    if (pool->head != pool->tail) {
        pool->tail = FX_ModS32(pool->tail + pool->size - 1, pool->size);
        ret = pool->items[pool->tail];
    }
    OS_EnableIrqMask(irq);
    return ret;
}

void WifiSetupFade_Init(void) {
    gWifiSetupFades = WifiSetupHeap_AllocZeroed(0x18, 4);
    G2x_SetBlendBrightness_((volatile u16 *)0x04000050, 0x3F, 0x10);
    G2x_SetBlendBrightness_((volatile u16 *)0x04001050, 0x3F, 0x10);
}

void WifiSetupFade_Free(void) {
    WifiSetupHeap_FreeAndClear(&gWifiSetupFades);
}

u8 WifiSetupFade_IsActive(int screen) {
    WifiSetupFade *fade;
    if (screen == 1) {
        fade = &gWifiSetupFades[0];
    } else {
        fade = &gWifiSetupFades[1];
    }
    return fade->active;
}

int WifiSetupFade_Start(int kind, int screen, int planes, int duration) {
    WifiSetupBrightnessTable start = *(WifiSetupBrightnessTable *)(gWifiSetupFadeTables + 8);
    WifiSetupFade *fade;
    if (screen == 1) {
        fade = &gWifiSetupFades[0];
    } else {
        fade = &gWifiSetupFades[1];
    }
    if (fade->active) {
        return 0;
    }
    if (screen == 1) {
        G2x_SetBlendBrightness_((volatile u16 *)0x04001050, planes, start.values[kind]);
    } else {
        G2x_SetBlendBrightness_((volatile u16 *)0x04000050, planes, start.values[kind]);
    }
    fade->task = WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupFade_Task, fade, 0xC8);
    fade->counter = 0;
    fade->kind = kind;
    fade->duration = duration;
    fade->active = 1;
    return 1;
}

void WifiSetupFade_Task(void *task, WifiSetupFade *fade) {
    WifiSetupBrightnessTable flags = *(WifiSetupBrightnessTable *)(gWifiSetupFadeTables + 4);
    WifiSetupBrightnessTable end = *(WifiSetupBrightnessTable *)(gWifiSetupFadeTables + 0);
    int brightness;
    u8 kindFlags;
    fade->counter++;
    brightness = FX_DivS32(fade->counter << 4, fade->duration);
    kindFlags = flags.values[fade->kind];
    if (kindFlags & 1) {
        brightness = 16 - brightness;
    }
    if (kindFlags & 0x10) {
        brightness = -brightness;
    }
    if (fade == gWifiSetupFades) {
        G2x_ChangeBlendBrightness_((u16 *)0x04001050, brightness);
    } else {
        G2x_ChangeBlendBrightness_((u16 *)0x04000050, brightness);
    }
    if (fade->counter < fade->duration) {
        return;
    }
    if (fade == gWifiSetupFades) {
        G2x_ChangeBlendBrightness_((u16 *)0x04001050, end.values[fade->kind]);
    } else {
        G2x_ChangeBlendBrightness_((u16 *)0x04000050, end.values[fade->kind]);
    }
    fade->active = 0;
    WifiSetupTask_DestroyLater(1, task);
}

int WifiSetupFade_StartDelay(int duration) {
    WifiSetupFade *fade = gWifiSetupFades;
    if (fade->active) {
        return 0;
    }
    fade->task = WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupFade_DelayTask, fade, 0xC8);
    fade->counter = 0;
    fade->duration = duration;
    fade->active = 1;
    return 1;
}

void WifiSetupFade_DelayTask(void *task, WifiSetupFade *fade) {
    fade->counter++;
    if (fade->counter < fade->duration) {
        return;
    }
    fade->active = 0;
    WifiSetupTask_DestroyLater(1, task);
}

void WifiSetupText_Init(void) {
    WifiSetupTextState *text;
    int i;
    gWifiSetupText = text = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupTextState), 4);
    gWifiSetupText->objCanvasPool = WifiSetupPool_NewWithItems(0x20, text->objCanvases, 0x30);
    if (WifiSetupApp_GetLanguage() == 6) {
        for (i = 0; i < 2; i++) {
            gWifiSetupText->fontData[i] = WifiSetupFile_Load(gWifiSetupFontPathsKorean[i], NULL, 4);
            NNS_G2dFontInitUTF16(&gWifiSetupText->fonts[i], gWifiSetupText->fontData[i]);
        }
    } else {
        for (i = 0; i < 2; i++) {
            gWifiSetupText->fontData[i] = WifiSetupFile_Load(gWifiSetupFontPaths[i], NULL, 4);
            NNS_G2dFontInitUTF16(&gWifiSetupText->fonts[i], gWifiSetupText->fontData[i]);
        }
    }
}

void WifiSetupText_Free(void) {
    int i;
    for (i = 0; i < 2; i++) {
        WifiSetupFile_Free(gWifiSetupText->fontData[i]);
    }
    WifiSetupPool_Delete(gWifiSetupText->objCanvasPool);
    WifiSetupHeap_FreeAndClear(MODULE_BSS_ADDR(0x0224F5E0));
}

WifiSetupCanvas *WifiSetupText_AllocObjCanvas(int screen, int width, int height, int fromFront, u32 *numObjs, int font) {
    u32 charNo;
    WifiSetupCanvas *canvas = WifiSetupPool_Pop(gWifiSetupText->objCanvasPool);
    canvas->u.obj.width = width;
    canvas->u.obj.height = height;
    canvas->charBuf = WifiSetupCharVram_Alloc(screen, width * height, fromFront, &charNo);
    canvas->u.obj.charNo = charNo;
    *numObjs = NNSi_G2dCalcRequiredOBJ(width, height);
    {
        u32 tile = charNo;
        u32 base = screen == 1 ? 0x6600000 : 0x6400000;
        NNS_G2dCharCanvasInitForOBJ1D(canvas, (void *)(base + (tile << 7)), width, height, 4);
    }
    canvas->vtable->clear(canvas, 0);
    {
        WifiSetupFont *fontPtr = &gWifiSetupText->fonts[font];
        canvas->textCanvas.charCanvas = canvas;
        canvas->textCanvas.font = fontPtr;
    }
    canvas->textCanvas.hSpace = 1;
    canvas->textCanvas.vSpace = 1;
    return canvas;
}

void WifiSetupText_FreeObjCanvas(WifiSetupCanvas *canvas) {
    WifiSetupCharVram_Free(canvas->charBuf);
    WifiSetupPool_Push(gWifiSetupText->objCanvasPool, canvas);
}

WifiSetupCanvas *WifiSetupText_InitBgCanvas(int screen, int font) {
    int height = gWifiSetupTextScreenHeights[screen * 2], width = gWifiSetupTextScreenWidths[screen * 2];
    WifiSetupCanvas *canvas = &gWifiSetupText->bgCanvases[screen];
    void *screenBuf;
    canvas->charBuf = WifiSetupHeap_AllocZeroed(width * height * 32, 32);
    if (screen == 1) {
        *(volatile u16 *)0x04001008 &= ~0x40;
        *(volatile u16 *)0x04001008 = (*(volatile u16 *)0x04001008 & 0x43) | 0xc00;
    } else {
        *(volatile u16 *)0x04000008 &= ~0x40;
        *(volatile u16 *)0x04000008 = (*(volatile u16 *)0x04000008 & 0x43) | 0xc00;
    }
    NNS_G2dCharCanvasInitForBG(canvas, canvas->charBuf, width, height, 4);
    {
        WifiSetupTextState *text = gWifiSetupText;
        int sub = screen == 1;
        canvas->textCanvas.charCanvas = canvas;
        canvas->textCanvas.font = &text->fonts[font];
        canvas->textCanvas.hSpace = 1;
        canvas->textCanvas.vSpace = 1;
        if (sub) {
            screenBuf = G2S_GetBG0ScrPtr();
        } else {
            screenBuf = G2_GetBG0ScrPtr();
        }
    }
    NNS_G2dMapScrToCharText(screenBuf, width, height, 0, 0, 32, gWifiSetupTextScreenCharBases[screen], 15);
    WifiSetupText_Clear(canvas, 0);
    canvas->u.task = WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupText_BgTransferTask, &gWifiSetupText->bgDirty[screen], 200);
    return canvas;
}

void WifiSetupText_BgTransferTask(void *task, u8 *dirty) {
    if (*dirty == 0) {
        return;
    }
    if (dirty == &gWifiSetupText->bgDirty[0]) {
        DC_FlushRange(gWifiSetupText->bgCanvases[0].charBuf, 0x6000);
        GX_LoadBG0Char(gWifiSetupText->bgCanvases[0].charBuf, 0, 0x6000);
    } else {
        DC_FlushRange(gWifiSetupText->bgCanvases[1].charBuf, 0x3000);
        GXS_LoadBG0Char(gWifiSetupText->bgCanvases[1].charBuf, 0x3000, 0x3000);
    }
    *dirty = 0;
}

void WifiSetupText_FreeCanvas(WifiSetupCanvas *canvas) {
    WifiSetupTask_Destroy(1, canvas->u.task);
    if (canvas == &gWifiSetupText->bgCanvases[0]) {
        MIi_CpuClear16(0, G2_GetBG0CharPtr(), 0x6000);
    } else {
        MIi_CpuClear16(0, G2_GetBG0CharPtr(), 0x3000);
    }
    WifiSetupHeap_FreeAndClear(&canvas->charBuf);
}

void WifiSetupText_FreeBgCanvas(int screen) {
    WifiSetupText_FreeCanvas(&gWifiSetupText->bgCanvases[screen]);
}

void WifiSetupText_Draw(WifiSetupCanvas *canvas, int x, int y, int color, u32 flags, const u16 *text) {
    WifiSetupFont *font = canvas->textCanvas.font;
    WifiSetupTextDirection dir = { 0, 0 };
    switch (font->res->info->flowDirection) {
    case 0:
    case 7:
        dir.x = 1;
        break;
    case 1:
    case 2:
        dir.y = 1;
        break;
    case 3:
    case 4:
        dir.x = -1;
        break;
    case 5:
    case 6:
        dir.y = -1;
        break;
    }
    NNSi_G2dTextCanvasDrawText(&canvas->textCanvas, x, y, color, flags, text, dir);
}

void WifiSetupText_DrawChar(void *charCanvas, int x, int y, int color, u16 ch, int font) {
    NNS_G2dCharCanvasDrawChar(charCanvas, &gWifiSetupText->fonts[font], x, y, color, ch);
}

// Draws every character centred in a cell of advance pixels.
void WifiSetupText_DrawMonospace(void *charCanvas, int x, int y, int color, int advance, const u16 *text, int font) {
    WifiSetupFont *fontPtr;
    u16 glyph;
    WifiSetupCharWidths *widths;
    while (*text != 0) {
        fontPtr = &gWifiSetupText->fonts[font];
        glyph = NNS_G2dFontFindGlyphIndex(fontPtr, *text);
        if (glyph == 0xFFFF) {
            glyph = fontPtr->res->alternateChar;
        }
        widths = NNS_G2dFontGetCharWidthsFromIndex(fontPtr, glyph);
        WifiSetupText_DrawChar(charCanvas, x + ((advance - widths->charWidth) >> 1), y, color, *text, font);
        text++;
        x += advance;
    }
}

void WifiSetupText_DrawInRect(WifiSetupCanvas *canvas, int x, int y, int width, int height, int color, u32 flags, const u16 *text) {
    WifiSetupFont *font = canvas->textCanvas.font;
    WifiSetupTextDirection dir = { 0, 0 };
    switch (font->res->info->flowDirection) {
    case 0:
    case 7:
        dir.x = 1;
        break;
    case 1:
    case 2:
        dir.y = 1;
        break;
    case 3:
    case 4:
        dir.x = -1;
        break;
    case 5:
    case 6:
        dir.y = -1;
        break;
    }
    NNSi_G2dTextCanvasDrawTextRect(&canvas->textCanvas, x, y, width, height, color, flags, text, dir);
}

void WifiSetupText_Clear(WifiSetupCanvas *canvas, int color) {
    canvas->vtable->clear(canvas, color);
}

void WifiSetupText_ShowObjCanvas(WifiSetupCanvas *canvas, int x, int y, WifiSetupOamBlock *block, u32 priority) {
    GXOamAttr *oam = WifiSetupOamBlock_GetOam(block, 0);
    WifiSetupOamBlock_SetAffineMode(block, -1, 0, 0);
    WifiSetupOamBlock_SetModeAndPalette(block, -1, 0, 0xF);
    WifiSetupOamBlock_SetPriority(block, -1, priority);
    NNS_G2dArrangeOBJ1D(oam, canvas->u.obj.width, canvas->u.obj.height, x, y, 0, canvas->u.obj.charNo, 2);
}

void WifiSetupText_MarkDirty(WifiSetupCanvas *canvas) {
    if (canvas == &gWifiSetupText->bgCanvases[0]) {
        gWifiSetupText->bgDirty[0] = 1;
    } else {
        gWifiSetupText->bgDirty[1] = 1;
    }
}

void WifiSetup_SetPoint(u16 x, u16 y, WifiSetupPoint *point) {
    point->x = x;
    point->y = y;
}

void WifiSetup_SetRect(u16 left, u16 top, u16 right, u16 bottom, WifiSetupRect *rect) {
    rect->left = left;
    rect->top = top;
    rect->right = right;
    rect->bottom = bottom;
}

void WifiSetup_RectFromPosSize(const WifiSetupPoint *pos, const WifiSetupPoint *size, WifiSetupRect *rect) {
    rect->left = pos->x;
    rect->top = pos->y;
    rect->right = pos->x + size->x;
    rect->bottom = pos->y + size->y;
}

void WifiSetupDisplay_ShowPlanes(int screen, int planes) {
    if (screen == 1) {
        GXS_SetVisiblePlane(planes | GXS_GetVisiblePlane13());
    } else {
        GX_SetVisiblePlane(planes | GX_GetVisiblePlane13());
    }
}

void WifiSetupDisplay_HidePlanes(int screen, int planes) {
    if (screen == 1) {
        GXS_SetVisiblePlane(~planes & GXS_GetVisiblePlane13());
    } else {
        GX_SetVisiblePlane(~planes & GX_GetVisiblePlane13());
    }
}

void WifiSetupDisplay_SetWindowRect(int screen, int window, WifiSetupRect *rect) {
    if (screen == 1) {
        if (window == 0) {
            G2S_SetWnd0Position(rect->left, rect->top, rect->right, rect->bottom);
        } else {
            G2S_SetWnd1Position(rect->left, rect->top, rect->right, rect->bottom);
        }
    } else {
        if (window == 0) {
            G2_SetWnd0Position(rect->left, rect->top, rect->right, rect->bottom);
        } else {
            G2_SetWnd1Position(rect->left, rect->top, rect->right, rect->bottom);
        }
    }
}

// window: 0/1 inside window 0/1, 2 inside the OBJ window, 3 outside all windows.
void WifiSetupDisplay_SetWindowPlanes(int screen, int window, int planes, BOOL effect) {
    switch (window) {
    case 0:
        if (screen == 1) {
            G2S_SetWnd0InsidePlane(planes, effect);
        } else {
            G2_SetWnd0InsidePlane(planes, effect);
        }
        break;
    case 1:
        if (screen == 1) {
            G2S_SetWnd1InsidePlane(planes, effect);
        } else {
            G2_SetWnd1InsidePlane(planes, effect);
        }
        break;
    case 2:
        if (screen == 1) {
            u32 tmp = (reg_G2S_DB_WINOUT & ~0x3F00) | (planes << 8);
            if (effect) {
                tmp |= 0x2000;
            }
            reg_G2S_DB_WINOUT = tmp;
        } else {
            u32 tmp = (reg_G2_WINOUT & ~0x3F00) | (planes << 8);
            if (effect) {
                tmp |= 0x2000;
            }
            reg_G2_WINOUT = tmp;
        }
        break;
    case 3:
        if (screen == 1) {
            G2S_SetWndOutsidePlane(planes, effect);
        } else {
            G2_SetWndOutsidePlane(planes, effect);
        }
        break;
    }
}

void WifiSetupHeap_Init(void *mem) {
    MI_CpuFill8(mem, 0, 0x40000);
    gWifiSetupHeap = NNS_FndCreateExpHeapEx(mem, 0x40000, 0);
    if (gWifiSetupHeap == NULL) {
        OS_Terminate();
    }
}

void WifiSetupHeap_Destroy(void) {
    NNS_FndDestroyExpHeap(gWifiSetupHeap);
    gWifiSetupHeap = NULL;
}

void *WifiSetupHeap_Alloc(u32 size, int align) {
    void *mem;
    int irq;
    irq = OS_DisableIrqMask(OS_IE_V_BLANK);
    mem = NNS_FndAllocFromExpHeapEx(gWifiSetupHeap, size, align);
    if (mem == NULL) {
        OS_Terminate();
    }
    OS_EnableIrqMask(irq);
    return mem;
}

void *WifiSetupHeap_AllocZeroed(u32 size, int align) {
    void *mem = WifiSetupHeap_Alloc(size, align);
    MI_CpuFill8(mem, 0, size);
    return mem;
}

// ptr points to the pointer to free; it is set to NULL.
void WifiSetupHeap_FreeAndClear(void *ptr) {
    int irq = OS_DisableIrqMask(OS_IE_V_BLANK);
    if (*(void **)ptr == NULL) {
        return;
    }
    NNS_FndFreeToExpHeap(gWifiSetupHeap, *(void **)ptr);
    OS_EnableIrqMask(irq);
    *(void **)ptr = NULL;
}

void WifiSetupHeap_Free(void *mem) {
    int irq = OS_DisableIrqMask(OS_IE_V_BLANK);
    if (mem == NULL) {
        return;
    }
    NNS_FndFreeToExpHeap(gWifiSetupHeap, mem);
    OS_EnableIrqMask(irq);
}

void WifiSetupInput_Init(void) {
    TPCalibrateParam calibrate;
    gWifiSetupInput.state = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupInputState), 4);
    if (!TP_GetUserInfo(&calibrate)) {
        OS_Terminate();
    }
    TP_SetCalibrateParam(&calibrate);
    TP_RequestAutoSamplingStartAsync(0, 4, gWifiSetupInput.state->samples, 5);
    TP_WaitBusy(2);
    if (TP_CheckBusy(2)) {
        OS_Terminate();
    }
    WifiSetupInput_Update();
}

void WifiSetupInput_Free(void) {
    do {
        TP_RequestAutoSamplingStopAsync();
        TP_WaitBusy(4);
    } while (TP_CheckBusy(4));
    WifiSetupHeap_FreeAndClear(&gWifiSetupInput.state);
}

void WifiSetupInput_Update(void) {
    WifiSetupInput_UpdatePad();
    WifiSetupInput_UpdateTouch();
}

// Key repeat: first after 40 frames, then every 7 frames.
void WifiSetupInput_UpdatePad(void) {
    int i;
    u8 *holdFrames;
    WifiSetupInputState *state = gWifiSetupInput.state;
    u16 pad = PAD_Read();
    gWifiSetupInput.state->trig = (state->cont ^ pad) & pad;
    gWifiSetupInput.state->release = state->cont & (state->cont ^ pad);
    gWifiSetupInput.state->cont = pad;
    gWifiSetupInput.state->repeat = gWifiSetupInput.state->trig;
    holdFrames = gWifiSetupInputHoldFrames;
    for (i = 0; i < 14; i++) {
        u16 bit = 1 << i;
        if ((pad & bit) == 0) {
            *holdFrames = 0;
        } else {
            (*holdFrames)++;
            if (*holdFrames == 40) {
                gWifiSetupInput.state->repeat |= bit;
            } else if (*holdFrames == 47) {
                gWifiSetupInput.state->repeat |= bit;
                *holdFrames = 40;
            }
        }
        holdFrames++;
    }
}

void WifiSetupInput_UpdateTouch(void) {
    int n;
    int index;
    int wasTouching;
    u8 touched;
    int changed;
    TPData calibrated;

    wasTouching = gWifiSetupInput.state->touching ? 1 : 0;
    touched = 0;
    index = TP_GetLatestIndexInAuto();
    gWifiSetupInput.state->prevTouch = gWifiSetupInput.state->touch;
    n = 0;
    do {
        TPData *sample = &gWifiSetupInput.state->samples[index];
        if (sample->touch == 1 && sample->validity == 0) {
            touched = 1;
            TP_GetCalibratedPoint(&calibrated, sample);
            WifiSetup_SetPoint(calibrated.x, calibrated.y, &gWifiSetupInput.state->touch);
            break;
        }
        n++;
        index = FX_ModS32(index + 4, 5);
    } while (n < 4);
    changed = touched ^ wasTouching;
    gWifiSetupInput.state->touchTrig = touched & changed;
    gWifiSetupInput.state->touchRelease = wasTouching & changed;
    gWifiSetupInput.state->touching = touched;
    gWifiSetupInput.state->touchRepeat = gWifiSetupInput.state->touchTrig;
    if (!touched) {
        gWifiSetupInput.touchHoldFrames = 0;
        return;
    }
    gWifiSetupInput.touchHoldFrames++;
    if (gWifiSetupInput.touchHoldFrames == 40) {
        gWifiSetupInput.state->touchRepeat = 1;
    } else if (gWifiSetupInput.touchHoldFrames == 47) {
        gWifiSetupInput.state->touchRepeat = 1;
        gWifiSetupInput.touchHoldFrames = 40;
    }
}

BOOL WifiSetupInput_IsKeyPressed(u16 mask) {
    if (PAD_DetectFold()) {
        return FALSE;
    }
    return mask == (mask & gWifiSetupInput.state->trig);
}

BOOL WifiSetupInput_IsKeyRepeated(u16 mask) {
    if (PAD_DetectFold()) {
        return FALSE;
    }
    return mask == (mask & gWifiSetupInput.state->repeat);
}

BOOL WifiSetupInput_IsKeyReleased(u16 mask) {
    if (PAD_DetectFold()) {
        return FALSE;
    }
    return mask == (mask & gWifiSetupInput.state->release);
}

BOOL WifiSetupInput_IsTouchHeld(WifiSetupRect *rect) {
    WifiSetupInputState *state;
    if (PAD_DetectFold()) {
        return FALSE;
    }
    state = gWifiSetupInput.state;
    if (!state->touching) {
        return FALSE;
    }
    if (rect->left > state->touch.x) {
        return FALSE;
    }
    if (rect->right < state->touch.x) {
        return FALSE;
    }
    if (rect->top > state->touch.y) {
        return FALSE;
    }
    return rect->bottom >= state->touch.y;
}

BOOL WifiSetupInput_IsTouchPressed(WifiSetupRect *rect) {
    WifiSetupInputState *state;
    if (PAD_DetectFold()) {
        return FALSE;
    }
    state = gWifiSetupInput.state;
    if (!state->touchTrig) {
        return FALSE;
    }
    if (rect->left > state->touch.x) {
        return FALSE;
    }
    if (rect->right < state->touch.x) {
        return FALSE;
    }
    if (rect->top > state->touch.y) {
        return FALSE;
    }
    return rect->bottom >= state->touch.y;
}

BOOL WifiSetupInput_IsTouchRepeated(WifiSetupRect *rect) {
    WifiSetupInputState *state;
    if (PAD_DetectFold()) {
        return FALSE;
    }
    state = gWifiSetupInput.state;
    if (!state->touchRepeat) {
        return FALSE;
    }
    if (rect->left > state->touch.x) {
        return FALSE;
    }
    if (rect->right < state->touch.x) {
        return FALSE;
    }
    if (rect->top > state->touch.y) {
        return FALSE;
    }
    return rect->bottom >= state->touch.y;
}

BOOL WifiSetupInput_IsTouchReleased(WifiSetupRect *rect) {
    WifiSetupInputState *state;
    if (PAD_DetectFold()) {
        return FALSE;
    }
    state = gWifiSetupInput.state;
    if (!state->touchRelease) {
        return FALSE;
    }
    if (rect->left > state->touch.x) {
        return FALSE;
    }
    if (rect->right < state->touch.x) {
        return FALSE;
    }
    if (rect->top > state->touch.y) {
        return FALSE;
    }
    return rect->bottom >= state->touch.y;
}

// box: left/top are the position, right/bottom the width and height.
BOOL WifiSetupInput_IsTouchPressedInBox(WifiSetupRect *box) {
    WifiSetupRect rect;
    if (PAD_DetectFold()) {
        return FALSE;
    }
    rect.left = box->left;
    rect.top = box->top;
    rect.right = box->left + box->right;
    rect.bottom = box->top + box->bottom;
    return WifiSetupInput_IsTouchPressed(&rect);
}

BOOL WifiSetupInput_GetTouchPos(WifiSetupPoint *out) {
    WifiSetupInputState *state;
    if (PAD_DetectFold()) {
        return FALSE;
    }
    state = gWifiSetupInput.state;
    if (!state->touching) {
        *out = state->prevTouch;
        return FALSE;
    }
    *out = state->touch;
    return TRUE;
}

// Turns the LCDs off while the lid is closed and back on when it opens.
void WifiSetupInput_UpdateLcdPower(void) {
    if (gWifiSetupInput.lcdOff) {
        if (PAD_DetectFold()) {
            return;
        }
        if (!PM_SetLCDPower(1)) {
            return;
        }
        gWifiSetupInput.lcdOff = 0;
    } else {
        if (!PAD_DetectFold()) {
            return;
        }
        if (PM_SetLCDPower(0)) {
            gWifiSetupInput.lcdOff = 1;
        }
    }
}

void WifiSetup_InstallVBlankIntr(void) {
    gWifiSetupIrqSave.mask = reg_OS_IE;
    OS_SetIrqMask(0x40018);
    OS_EnableIrqMask(OS_IE_V_BLANK);
    gWifiSetupIrqSave.vblankFunc = OS_GetIrqFunction(OS_IE_V_BLANK);
    OS_SetIrqFunction(OS_IE_V_BLANK, WifiSetup_VBlankIntr);
    OS_ResetRequestIrqMask(OS_IE_V_BLANK);
    OS_EnableIrq();
    OS_EnableInterrupts();
}

void WifiSetup_RestoreVBlankIntr(void) {
    OS_DisableIrq();
    OS_SetIrqMask(gWifiSetupIrqSave.mask);
    OS_SetIrqFunction(OS_IE_V_BLANK, gWifiSetupIrqSave.vblankFunc);
}

#pragma force_active off

static inline volatile u32 *WifiSetup_GetIrqCheckBase(void) {
    return (volatile u32 *)0x027E0000;
}

#pragma force_active on

// Runs the vblank task queue and sets the vblank bit of the interrupt check flags (DTCM + 0x3FF8).
void WifiSetup_VBlankIntr(void) {
    WifiSetupTask_RunQueue(1);
    {
        volatile u32 *base = WifiSetup_GetIrqCheckBase();
        base += 0x3000 / 4;
        {
            volatile u32 *flags = base + 0xff8 / 4;
            *flags |= 1;
        }
    }
}

WifiSetupList *WifiSetupList_New(void) {
    WifiSetupList *list = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupList), 4);
    list->head.prev = NULL;
    list->head.next = &list->tail;
    list->tail.prev = &list->head;
    list->tail.next = NULL;
    return list;
}

void WifiSetupList_Delete(WifiSetupList *list) {
    WifiSetupHeap_FreeAndClear(&list);
}

void WifiSetupList_Remove(void *node) {
    WifiSetupListNode *n = node;
    int irq = OS_DisableIrqMask(OS_IE_V_BLANK);
    n->prev->next = n->next;
    n->next->prev = n->prev;
    n->next = NULL;
    n->prev = NULL;
    OS_EnableIrqMask(irq);
}

void WifiSetupList_InsertBefore(void *pos, void *node) {
    WifiSetupListNode *p = pos;
    WifiSetupListNode *n = node;
    int irq = OS_DisableIrqMask(OS_IE_V_BLANK);
    p->prev->next = n;
    n->prev = p->prev;
    n->next = p;
    p->prev = n;
    OS_EnableIrqMask(irq);
}

void WifiSetupList_Append(WifiSetupList *list, void *node) {
    WifiSetupList_InsertBefore(&list->tail, node);
}

void WifiSetupList_Prepend(WifiSetupList *list, void *node) {
    WifiSetupList_InsertBefore(list->head.next, node);
}

void WifiSetupOam_Init(void) {
    WifiSetupOamBuffer *buffer;
    int i;
    gWifiSetupOamBuffer = buffer = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupOamBuffer), 4);
    MIi_CpuClearFast(0x200, (void *)buffer, 0x800);
    for (i = 0; i < 2; i++) {
        gWifiSetupOamBuffer->pools[i] = WifiSetupPool_NewWithItems(0x40, gWifiSetupOamBuffer->oam[i], 8);
    }
    gWifiSetupOamBuffer->task = WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupOam_TransferTask, NULL, 0xC8);
}

void WifiSetupOam_TransferTask(void) {
    DC_FlushRange(gWifiSetupOamBuffer, 0x800);
    GX_LoadOAM(gWifiSetupOamBuffer->oam[0], 0, 0x400);
    GXS_LoadOAM(gWifiSetupOamBuffer->oam[1], 0, 0x400);
}

void WifiSetupOam_Free(void) {
    WifiSetupTask_Destroy(1, gWifiSetupOamBuffer->task);
    WifiSetupHeap_FreeAndClear(MODULE_BSS_ADDR(0x0224F608));
}

GXOamAttr *WifiSetupOam_Alloc(int screen) {
    return WifiSetupPool_Pop(gWifiSetupOamBuffer->pools[screen]);
}

void WifiSetupOam_Release(GXOamAttr *oam) {
    int screen;
    oam->attr01 = (oam->attr01 & 0xC1FFFCFF) | 0x200; // hide
    screen = 0;
    if ((u32)oam >= (u32)gWifiSetupOamBuffer->oam[1]) {
        screen = 1;
    }
    WifiSetupPool_Push(gWifiSetupOamBuffer->pools[screen], oam);
}

GXOamAttr *WifiSetupOam_Get(int screen, int index) {
    return &gWifiSetupOamBuffer->oam[screen][index];
}

void WifiSetupCharVram_Init(void) {
    int i;
    gWifiSetupCharVram = WifiSetupHeap_AllocZeroed(0x340, 4);
    for (i = 0; i < 2; i++) {
        gWifiSetupCharVram[i].pool = WifiSetupPool_NewWithItems(0x20, gWifiSetupCharVram[i].blocks, 0xC);
        gWifiSetupCharVram[i].list = WifiSetupList_New();
        gWifiSetupCharVram[i].first.start = 0x300;
        gWifiSetupCharVram[i].last.start = 0x400;
        WifiSetupList_Prepend(gWifiSetupCharVram[i].list, &gWifiSetupCharVram[i].first);
        WifiSetupList_Append(gWifiSetupCharVram[i].list, &gWifiSetupCharVram[i].last);
    }
}

// size in bytes; allocated in units of 4 (tile numbers 0x300-0x3FF are managed).
void *WifiSetupCharVram_Alloc(int screen, int size, int fromFront, u32 *outCharNo) {
    WifiSetupCharBlock *block;
    WifiSetupCharBlock *cur;
    int irq;
    int n;

    block = WifiSetupPool_Pop(gWifiSetupCharVram[screen].pool);
    n = ((size + 3) & ~3) >> 2;
    block->size = n;
    irq = OS_DisableIrqMask(OS_IE_V_BLANK);
    if (fromFront) {
        cur = &gWifiSetupCharVram[screen].first;
        while (cur != &gWifiSetupCharVram[screen].last) {
            WifiSetupCharBlock *next = (WifiSetupCharBlock *)cur->node.next;
            int start = cur->start + cur->size;
            if (start + n <= next->start) {
                block->start = start;
                WifiSetupList_InsertBefore(next, block);
                break;
            }
            cur = next;
        }
        if (cur == &gWifiSetupCharVram[screen].last) {
            OS_Terminate();
        }
    } else {
        cur = &gWifiSetupCharVram[screen].last;
        while (cur != &gWifiSetupCharVram[screen].first) {
            WifiSetupCharBlock *prev = (WifiSetupCharBlock *)cur->node.prev;
            int start = cur->start - n;
            if (start >= prev->start + prev->size) {
                block->start = start;
                WifiSetupList_InsertBefore(cur, block);
                break;
            }
            cur = prev;
        }
        if (cur == &gWifiSetupCharVram[screen].first) {
            OS_Terminate();
        }
    }
    *outCharNo = block->start;
    OS_EnableIrqMask(irq);
    return block;
}

void WifiSetupCharVram_Free(void *block) {
    int screen = 0;
    WifiSetupList_Remove(block);
    if ((u32)block >= (u32)&gWifiSetupCharVram[1]) {
        screen = 1;
    }
    WifiSetupPool_Push(gWifiSetupCharVram[screen].pool, block);
}

int WifiSetup_StrNLen(const u8 *str, int max) {
    int i;
    for (i = 0; i < max; i++) {
        if (str[i] == 0) {
            break;
        }
    }
    return i;
}

void WifiSetup_SaveVramBanks(void) {
    gWifiSetupSavedVramBanks.bg = GX_DisableBankForBG();
    gWifiSetupSavedVramBanks.obj = GX_DisableBankForOBJ();
    gWifiSetupSavedVramBanks.bgExtPltt = GX_DisableBankForBGExtPltt();
    gWifiSetupSavedVramBanks.objExtPltt = GX_DisableBankForOBJExtPltt();
    gWifiSetupSavedVramBanks.tex = GX_DisableBankForTex();
    gWifiSetupSavedVramBanks.texPltt = GX_DisableBankForTexPltt();
    gWifiSetupSavedVramBanks.clearImage = GX_DisableBankForClearImage();
    gWifiSetupSavedVramBanks.subBg = GX_DisableBankForSubBG();
    gWifiSetupSavedVramBanks.subObj = GX_DisableBankForSubOBJ();
    gWifiSetupSavedVramBanks.subBgExtPltt = GX_DisableBankForSubBGExtPltt();
    gWifiSetupSavedVramBanks.subObjExtPltt = GX_DisableBankForSubOBJExtPltt();
    gWifiSetupSavedVramBanks.arm7 = GX_DisableBankForARM7();
    gWifiSetupSavedVramBanks.lcdc = GX_DisableBankForLCDC();
    GX_SetBankForARM7(gWifiSetupSavedVramBanks.arm7);
    WifiSetup_ClearVram();
}

void WifiSetup_RestoreVramBanks(void) {
    OSIntrMode irq;
    GX_DisableBankForBG();
    GX_DisableBankForOBJ();
    GX_DisableBankForSubBG();
    GX_DisableBankForSubOBJ();
    WifiSetup_ClearVram();
    GX_SetBankForBG(gWifiSetupSavedVramBanks.bg);
    GX_SetBankForOBJ(gWifiSetupSavedVramBanks.obj);
    GX_SetBankForBGExtPltt(gWifiSetupSavedVramBanks.bgExtPltt);
    GX_SetBankForOBJExtPltt(gWifiSetupSavedVramBanks.objExtPltt);
    GX_SetBankForTex(gWifiSetupSavedVramBanks.tex);
    GX_SetBankForTexPltt(gWifiSetupSavedVramBanks.texPltt);
    GX_SetBankForClearImage(gWifiSetupSavedVramBanks.clearImage);
    GX_SetBankForSubBG(gWifiSetupSavedVramBanks.subBg);
    GX_SetBankForSubOBJ(gWifiSetupSavedVramBanks.subObj);
    GX_SetBankForSubBGExtPltt(gWifiSetupSavedVramBanks.subBgExtPltt);
    GX_SetBankForSubOBJExtPltt(gWifiSetupSavedVramBanks.subObjExtPltt);
    GX_SetBankForLCDC(gWifiSetupSavedVramBanks.lcdc);
    *(vu16 *)0x04000050 = 0;
    *(vu16 *)0x04001050 = 0;
    *(vu32 *)0x04000010 = 0;
    *(vu32 *)0x04000014 = 0;
    *(vu32 *)0x04000018 = 0;
    *(vu32 *)0x0400001C = 0;
    *(vu32 *)0x04001010 = 0;
    *(vu32 *)0x04001014 = 0;
    *(vu32 *)0x04001018 = 0;
    *(vu32 *)0x0400101C = 0;
    irq = OS_DisableInterrupts();
    PM_SetLCDPower(1);
    OS_RestoreInterrupts(irq);
}

void WifiSetup_ClearVram(void) {
    GX_SetBankForLCDC((GXVRamLCDC)0x1F3);
    MIi_CpuClearFast(0, (void *)0x06800000, 0x40000);
    MIi_CpuClearFast(0, (void *)0x06880000, 0x24000);
    GX_DisableBankForLCDC();
    MIi_CpuClearFast(0x200, (void *)0x07000000, 0x400);
    MIi_CpuClearFast(0, (void *)0x05000000, 0x400);
    MIi_CpuClearFast(0x200, (void *)0x07000400, 0x400);
    MIi_CpuClearFast(0, (void *)0x05000400, 0x400);
}

void WifiSetupLed_SetWirelessPattern(void) {
    u32 pattern;
    if (PM_GetLEDPattern(&pattern) != 0) {
        return;
    }
    if (pattern == 0xF) {
        return;
    }
    PMi_SendLEDPatternCommand(0xF);
}

void WifiSetupLed_SetOn(void) {
    PMi_SendLEDPatternCommand(1);
}

void WifiSetupTask_InitScheduler(void) {
    int i;
    u8 *capacity;
    gWifiSetupTaskQueues = WifiSetupHeap_AllocZeroed(0x80, 4);
    capacity = gWifiSetupTaskQueueCapacities;
    for (i = 0; i < 2; i++) {
        gWifiSetupTaskQueues[i].capacity = *capacity;
        gWifiSetupTaskQueues[i].taskBuf = WifiSetupHeap_AllocZeroed(*capacity * 0x14, 4);
        gWifiSetupTaskQueues[i].taskPool = WifiSetupPool_NewWithItems(*capacity, gWifiSetupTaskQueues[i].taskBuf, 0x14);
        gWifiSetupTaskQueues[i].destroyQueue = WifiSetupPool_New(*capacity++);
        gWifiSetupTaskQueues[i].list = WifiSetupList_New();
        gWifiSetupTaskQueues[i].head.priority = 0;
        gWifiSetupTaskQueues[i].tail.priority = 0xFF;
        WifiSetupList_Prepend(gWifiSetupTaskQueues[i].list, &gWifiSetupTaskQueues[i].head);
        WifiSetupList_Append(gWifiSetupTaskQueues[i].list, &gWifiSetupTaskQueues[i].tail);
        gWifiSetupTaskQueues[i].enabled = 1;
    }
}

void WifiSetupTask_FreeScheduler(void) {
    int i;
    for (i = 0; i < 2; i++) {
        WifiSetupList_Delete(gWifiSetupTaskQueues[i].list);
        WifiSetupPool_Delete(gWifiSetupTaskQueues[i].taskPool);
    }
    WifiSetupHeap_FreeAndClear(MODULE_BSS_ADDR(0x0224F644));
}

// Runs every task of the queue in priority order, then destroys the tasks queued for destruction.
void WifiSetupTask_RunQueue(int queue) {
    WifiSetupTask *task;
    int i;
    if (!gWifiSetupTaskQueues[queue].enabled) {
        return;
    }
    for (task = (WifiSetupTask *)gWifiSetupTaskQueues[queue].head.node.next; task != &gWifiSetupTaskQueues[queue].tail; task = (WifiSetupTask *)task->node.next) {
        task->func(task, task->arg);
    }
    for (i = 0; i < gWifiSetupTaskQueues[queue].capacity; i++) {
        task = WifiSetupPool_Pop(gWifiSetupTaskQueues[queue].destroyQueue);
        if (task == NULL) {
            return;
        }
        WifiSetupTask_DestroyNow(queue, task);
    }
}

void *WifiSetupTask_Create(int queue, WifiSetupTaskFunc func, void *arg, u32 priority) {
    return WifiSetupTask_CreateEx(queue, func, arg, priority, 0);
}

WifiSetupTask *WifiSetupTask_CreateEx(int queue, WifiSetupTaskFunc func, void *arg, u32 priority, u8 ownsArg) {
    WifiSetupTask *task;
    WifiSetupTask *cur;
    int irq;
    task = WifiSetupPool_Pop(gWifiSetupTaskQueues[queue].taskPool);
    task->func = func;
    task->arg = arg;
    task->priority = priority;
    task->ownsArg = ownsArg;
    irq = OS_DisableIrqMask(OS_IE_V_BLANK);
    for (cur = (WifiSetupTask *)gWifiSetupTaskQueues[queue].head.node.next;; cur = (WifiSetupTask *)cur->node.next) {
        if (priority < cur->priority) {
            WifiSetupList_InsertBefore(cur, task);
            break;
        }
    }
    OS_EnableIrqMask(irq);
    return task;
}

void WifiSetupTask_SetFunc(WifiSetupTask *task, WifiSetupTaskFunc func) {
    task->func = func;
}

void WifiSetupTask_DestroyLater(int queue, void *task) {
    WifiSetupPool_Push(gWifiSetupTaskQueues[queue].destroyQueue, task);
}

void WifiSetupTask_Destroy(int queue, void *task) {
    WifiSetupTask_DestroyNow(queue, task);
}

void WifiSetupTask_DestroyNow(int queue, WifiSetupTask *task) {
    if (task->ownsArg) {
        WifiSetupHeap_FreeAndClear(&task->arg);
    }
    WifiSetupList_Remove(task);
    WifiSetupPool_Push(gWifiSetupTaskQueues[queue].taskPool, task);
}

void WifiSetupTask_SetQueueEnabled(int queue, u8 enabled) {
    gWifiSetupTaskQueues[queue].enabled = enabled;
}
