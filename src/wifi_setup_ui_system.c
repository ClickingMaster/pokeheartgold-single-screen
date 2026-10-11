#include <string.h>

#include "global.h"

#include "wifi_setup_ui.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#pragma force_active on

typedef struct WifiSetupArchiveState {
    void *buf;
    u8 unk_04[0x80];
    WifiSetupPool *filePool;
    FSArchive arc;
    u8 unk_88[0xE4 - 0x88 - sizeof(FSArchive)];
    u16 lockId;
} WifiSetupArchiveState;

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
#define MODULE_BSS_ADDR(addr) ((void *)(ov13_0224F5C8 + ((addr) - 0x0224F5C8)))

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

extern char ov13_0224F5C8[];
extern WifiSetupArchiveState *ov13_0224F5CC;
extern WifiSetupOamBlockManager *ov13_0224F5D0;
extern void *ov13_0224F5D4[];
extern WifiSetupFade *ov13_0224F5DC;
extern WifiSetupTextState *ov13_0224F5E0;
extern NNSFndHeapHandle ov13_0224F5E4;
extern WifiSetupInput ov13_0224F5E8;
extern u8 ov13_0224F5F0[];
extern WifiSetupIrqSave ov13_0224F600;
extern WifiSetupOamBuffer *ov13_0224F608;
extern WifiSetupCharVramManager *ov13_0224F60C;
extern WifiSetupVramBanks ov13_0224F610;
extern WifiSetupTaskQueue *ov13_0224F644;
extern u8 ov13_02245940[];
extern const u16 ov13_0224594C[];
extern const u16 ov13_02245950[];
extern const u16 ov13_02245952[];
extern u8 ov13_02245960[];
extern char ov13_02247424[];
extern char ov13_0224742C[];
extern char *ov13_02247430[];
extern char *ov13_02247438[];

int ov13_0222B970(void);
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
int ov13_0223F954(void);
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
    FS_ChangeDir(ov13_02247424);
    FS_UnloadArchiveTables(&ov13_0224F5CC->arc);
    FS_UnloadArchive(&ov13_0224F5CC->arc);
    FS_ReleaseArchiveName(&ov13_0224F5CC->arc);
    OS_ReleaseLockID(ov13_0224F5CC->lockId);
    ov13_0224F5CC->lockId = 0;
    WifiSetupHeap_FreeAndClear(&ov13_0224F5CC->buf);
    ov13_0224F5CC->buf = NULL;
    WifiSetupHeap_FreeAndClear(MODULE_BSS_ADDR(0x0224F5CC));
}

int WifiSetupArchive_Proc(FSArchive *arc, int command) {
    switch (command) {
    case 9:
        CARD_LockRom(ov13_0224F5CC->lockId);
        return 0;
    case 10:
        CARD_UnlockRom(ov13_0224F5CC->lockId);
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

int ov13_0223F954(void) {
    return 1;
}

void *WifiSetupFile_Load(const char *path, u32 *outSize, int align) {
    void *buf;
    FSFile file;
    u32 size;
    int a;
    void *out;

    WifiSetupPool_Pop(ov13_0224F5CC->filePool);
    FS_InitFile(&file);
    if (!FS_OpenFile(&file, path)) {
        OS_Terminate();
    }
    size = FS_GetLength(&file);
    if (outSize != NULL) {
        *outSize = size;
    }
    // compressed files are loaded with a negative alignment (from the end of the heap)
    if (WifiSetup_HasSameSuffix(path, ov13_0224742C, 2)) {
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
    WifiSetupPool_Push(ov13_0224F5CC->filePool, data);
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
    ov13_0224F5D0 = WifiSetupHeap_AllocZeroed(0x450, 4);
    for (i = 0; i < 2; i++) {
        ov13_0224F5D0[i].pool = WifiSetupPool_NewWithItems(0x20, ov13_0224F5D0[i].blocks, 0x10);
        ov13_0224F5D0[i].list = WifiSetupList_New();
        ov13_0224F5D0[i].first.oam = WifiSetupOam_Get(i, 0x40);
        ov13_0224F5D0[i].last.oam = WifiSetupOam_Get(i, 0x7F) + 1;
        WifiSetupList_Prepend(ov13_0224F5D0[i].list, &ov13_0224F5D0[i].first);
        WifiSetupList_Append(ov13_0224F5D0[i].list, &ov13_0224F5D0[i].last);
    }
}

void WifiSetupOamBlock_FreeManager(void) {
    int i;
    for (i = 0; i < 2; i++) {
        WifiSetupList_Delete(ov13_0224F5D0[i].list);
        WifiSetupPool_Delete(ov13_0224F5D0[i].pool);
    }
    WifiSetupHeap_FreeAndClear(MODULE_BSS_ADDR(0x0224F5D0));
}

WifiSetupOamBlock *WifiSetupOamBlock_Alloc(int screen, int count, int fromFront) {
    WifiSetupOamBlock *block;
    WifiSetupOamBlock *cur;
    int irq;

    block = WifiSetupPool_Pop(ov13_0224F5D0[screen].pool);
    irq = OS_DisableIrqMask(1);
    if (fromFront) {
        cur = &ov13_0224F5D0[screen].first;
        while (cur != &ov13_0224F5D0[screen].last) {
            WifiSetupOamBlock *next = (WifiSetupOamBlock *)cur->node.next;
            GXOamAttr *start = cur->oam + cur->count;
            if (start + count <= next->oam) {
                block->oam = start;
                WifiSetupList_InsertBefore(next, block);
                break;
            }
            cur = next;
        }
        if (cur == &ov13_0224F5D0[screen].last) {
            OS_Terminate();
        }
    } else {
        cur = &ov13_0224F5D0[screen].last;
        while (cur != &ov13_0224F5D0[screen].first) {
            WifiSetupOamBlock *prev = (WifiSetupOamBlock *)cur->node.prev;
            GXOamAttr *start = cur->oam - count;
            if (start >= prev->oam + prev->count) {
                block->oam = start;
                WifiSetupList_InsertBefore(cur, block);
                break;
            }
            cur = prev;
        }
        if (cur == &ov13_0224F5D0[screen].first) {
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
    if ((u32)block >= (u32)&ov13_0224F5D0[1]) {
        screen = 1;
    }
    WifiSetupPool_Push(ov13_0224F5D0[screen].pool, block);
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
    ov13_0224F5D4[slot] = WifiSetupFile_Load(path, &size, 4);
}

void WifiSetupOamTemplate_Free(int slot) {
    WifiSetupFile_Free(ov13_0224F5D4[slot]);
    ov13_0224F5D4[slot] = NULL;
}

void WifiSetupOamTemplate_Copy(int slot, int index, GXOamAttr *dst) {
    u8 *data = ov13_0224F5D4[slot];
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
    WifiSetupOamBlock *block = WifiSetupOamBlock_Alloc(slot, ((WifiSetupOamTemplateHeader *)ov13_0224F5D4[slot])[index].count, fromFront);
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
    int irq = OS_DisableIrqMask(1);
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
    int irq = OS_DisableIrqMask(1);
    if (pool->head != pool->tail) {
        pool->tail = FX_ModS32(pool->tail + pool->size - 1, pool->size);
        ret = pool->items[pool->tail];
    }
    OS_EnableIrqMask(irq);
    return ret;
}

void WifiSetupFade_Init(void) {
    ov13_0224F5DC = WifiSetupHeap_AllocZeroed(0x18, 4);
    G2x_SetBlendBrightness_((volatile u16 *)0x04000050, 0x3F, 0x10);
    G2x_SetBlendBrightness_((volatile u16 *)0x04001050, 0x3F, 0x10);
}

void WifiSetupFade_Free(void) {
    WifiSetupHeap_FreeAndClear(&ov13_0224F5DC);
}

u8 WifiSetupFade_IsActive(int screen) {
    WifiSetupFade *fade;
    if (screen == 1) {
        fade = &ov13_0224F5DC[0];
    } else {
        fade = &ov13_0224F5DC[1];
    }
    return fade->active;
}

int WifiSetupFade_Start(int kind, int screen, int planes, int duration) {
    WifiSetupBrightnessTable start = *(WifiSetupBrightnessTable *)(ov13_02245940 + 8);
    WifiSetupFade *fade;
    if (screen == 1) {
        fade = &ov13_0224F5DC[0];
    } else {
        fade = &ov13_0224F5DC[1];
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
    WifiSetupBrightnessTable flags = *(WifiSetupBrightnessTable *)(ov13_02245940 + 4);
    WifiSetupBrightnessTable end = *(WifiSetupBrightnessTable *)(ov13_02245940 + 0);
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
    if (fade == ov13_0224F5DC) {
        G2x_ChangeBlendBrightness_((u16 *)0x04001050, brightness);
    } else {
        G2x_ChangeBlendBrightness_((u16 *)0x04000050, brightness);
    }
    if (fade->counter < fade->duration) {
        return;
    }
    if (fade == ov13_0224F5DC) {
        G2x_ChangeBlendBrightness_((u16 *)0x04001050, end.values[fade->kind]);
    } else {
        G2x_ChangeBlendBrightness_((u16 *)0x04000050, end.values[fade->kind]);
    }
    fade->active = 0;
    WifiSetupTask_DestroyLater(1, task);
}

int WifiSetupFade_StartDelay(int duration) {
    WifiSetupFade *fade = ov13_0224F5DC;
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
    ov13_0224F5E0 = text = WifiSetupHeap_AllocZeroed(0x680, 4);
    ov13_0224F5E0->objCanvasPool = WifiSetupPool_NewWithItems(0x20, text->objCanvases, 0x30);
    if (ov13_0222B970() == 6) {
        for (i = 0; i < 2; i++) {
            ov13_0224F5E0->fontData[i] = WifiSetupFile_Load(ov13_02247430[i], NULL, 4);
            NNS_G2dFontInitUTF16(&ov13_0224F5E0->fonts[i], ov13_0224F5E0->fontData[i]);
        }
    } else {
        for (i = 0; i < 2; i++) {
            ov13_0224F5E0->fontData[i] = WifiSetupFile_Load(ov13_02247438[i], NULL, 4);
            NNS_G2dFontInitUTF16(&ov13_0224F5E0->fonts[i], ov13_0224F5E0->fontData[i]);
        }
    }
}

void WifiSetupText_Free(void) {
    int i;
    for (i = 0; i < 2; i++) {
        WifiSetupFile_Free(ov13_0224F5E0->fontData[i]);
    }
    WifiSetupPool_Delete(ov13_0224F5E0->objCanvasPool);
    WifiSetupHeap_FreeAndClear(MODULE_BSS_ADDR(0x0224F5E0));
}

WifiSetupCanvas *WifiSetupText_AllocObjCanvas(int screen, int width, int height, int fromFront, u32 *numObjs, int font) {
    u32 charNo;
    WifiSetupCanvas *canvas = WifiSetupPool_Pop(ov13_0224F5E0->objCanvasPool);
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
        WifiSetupFont *fontPtr = &ov13_0224F5E0->fonts[font];
        canvas->textCanvas.charCanvas = canvas;
        canvas->textCanvas.font = fontPtr;
    }
    canvas->textCanvas.hSpace = 1;
    canvas->textCanvas.vSpace = 1;
    return canvas;
}

void WifiSetupText_FreeObjCanvas(WifiSetupCanvas *canvas) {
    WifiSetupCharVram_Free(canvas->charBuf);
    WifiSetupPool_Push(ov13_0224F5E0->objCanvasPool, canvas);
}

WifiSetupCanvas *WifiSetupText_InitBgCanvas(int screen, int font) {
    int height = ov13_02245952[screen * 2], width = ov13_02245950[screen * 2];
    WifiSetupCanvas *canvas = &ov13_0224F5E0->bgCanvases[screen];
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
        WifiSetupTextState *text = ov13_0224F5E0;
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
    NNS_G2dMapScrToCharText(screenBuf, width, height, 0, 0, 32, ov13_0224594C[screen], 15);
    WifiSetupText_Clear(canvas, 0);
    canvas->u.task = WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupText_BgTransferTask, &ov13_0224F5E0->bgDirty[screen], 200);
    return canvas;
}

void WifiSetupText_BgTransferTask(void *task, u8 *dirty) {
    if (*dirty == 0) {
        return;
    }
    if (dirty == &ov13_0224F5E0->bgDirty[0]) {
        DC_FlushRange(ov13_0224F5E0->bgCanvases[0].charBuf, 0x6000);
        GX_LoadBG0Char(ov13_0224F5E0->bgCanvases[0].charBuf, 0, 0x6000);
    } else {
        DC_FlushRange(ov13_0224F5E0->bgCanvases[1].charBuf, 0x3000);
        GXS_LoadBG0Char(ov13_0224F5E0->bgCanvases[1].charBuf, 0x3000, 0x3000);
    }
    *dirty = 0;
}

void WifiSetupText_FreeCanvas(WifiSetupCanvas *canvas) {
    WifiSetupTask_Destroy(1, canvas->u.task);
    if (canvas == &ov13_0224F5E0->bgCanvases[0]) {
        MIi_CpuClear16(0, G2_GetBG0CharPtr(), 0x6000);
    } else {
        MIi_CpuClear16(0, G2_GetBG0CharPtr(), 0x3000);
    }
    WifiSetupHeap_FreeAndClear(&canvas->charBuf);
}

void WifiSetupText_FreeBgCanvas(int screen) {
    WifiSetupText_FreeCanvas(&ov13_0224F5E0->bgCanvases[screen]);
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
    NNS_G2dCharCanvasDrawChar(charCanvas, &ov13_0224F5E0->fonts[font], x, y, color, ch);
}

// Draws every character centred in a cell of advance pixels.
void WifiSetupText_DrawMonospace(void *charCanvas, int x, int y, int color, int advance, const u16 *text, int font) {
    WifiSetupFont *fontPtr;
    u16 glyph;
    WifiSetupCharWidths *widths;
    while (*text != 0) {
        fontPtr = &ov13_0224F5E0->fonts[font];
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
    if (canvas == &ov13_0224F5E0->bgCanvases[0]) {
        ov13_0224F5E0->bgDirty[0] = 1;
    } else {
        ov13_0224F5E0->bgDirty[1] = 1;
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

void WifiSetup_RectFromPosSize(WifiSetupPoint *pos, WifiSetupPoint *size, WifiSetupRect *rect) {
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
    ov13_0224F5E4 = NNS_FndCreateExpHeapEx(mem, 0x40000, 0);
    if (ov13_0224F5E4 == NULL) {
        OS_Terminate();
    }
}

void WifiSetupHeap_Destroy(void) {
    NNS_FndDestroyExpHeap(ov13_0224F5E4);
    ov13_0224F5E4 = NULL;
}

void *WifiSetupHeap_Alloc(u32 size, int align) {
    void *mem;
    int irq;
    irq = OS_DisableIrqMask(1);
    mem = NNS_FndAllocFromExpHeapEx(ov13_0224F5E4, size, align);
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
    int irq = OS_DisableIrqMask(1);
    if (*(void **)ptr == NULL) {
        return;
    }
    NNS_FndFreeToExpHeap(ov13_0224F5E4, *(void **)ptr);
    OS_EnableIrqMask(irq);
    *(void **)ptr = NULL;
}

void WifiSetupHeap_Free(void *mem) {
    int irq = OS_DisableIrqMask(1);
    if (mem == NULL) {
        return;
    }
    NNS_FndFreeToExpHeap(ov13_0224F5E4, mem);
    OS_EnableIrqMask(irq);
}

void WifiSetupInput_Init(void) {
    TPCalibrateParam calibrate;
    ov13_0224F5E8.state = WifiSetupHeap_AllocZeroed(0x3A, 4);
    if (!TP_GetUserInfo(&calibrate)) {
        OS_Terminate();
    }
    TP_SetCalibrateParam(&calibrate);
    TP_RequestAutoSamplingStartAsync(0, 4, ov13_0224F5E8.state->samples, 5);
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
    WifiSetupHeap_FreeAndClear(&ov13_0224F5E8.state);
}

void WifiSetupInput_Update(void) {
    WifiSetupInput_UpdatePad();
    WifiSetupInput_UpdateTouch();
}

// Key repeat: first after 40 frames, then every 7 frames.
void WifiSetupInput_UpdatePad(void) {
    int i;
    u8 *holdFrames;
    WifiSetupInputState *state = ov13_0224F5E8.state;
    u16 pad = PAD_Read();
    ov13_0224F5E8.state->trig = (state->cont ^ pad) & pad;
    ov13_0224F5E8.state->release = state->cont & (state->cont ^ pad);
    ov13_0224F5E8.state->cont = pad;
    ov13_0224F5E8.state->repeat = ov13_0224F5E8.state->trig;
    holdFrames = ov13_0224F5F0;
    for (i = 0; i < 14; i++) {
        u16 bit = 1 << i;
        if ((pad & bit) == 0) {
            *holdFrames = 0;
        } else {
            (*holdFrames)++;
            if (*holdFrames == 40) {
                ov13_0224F5E8.state->repeat |= bit;
            } else if (*holdFrames == 47) {
                ov13_0224F5E8.state->repeat |= bit;
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

    wasTouching = ov13_0224F5E8.state->touching ? 1 : 0;
    touched = 0;
    index = TP_GetLatestIndexInAuto();
    ov13_0224F5E8.state->prevTouch = ov13_0224F5E8.state->touch;
    n = 0;
    do {
        TPData *sample = &ov13_0224F5E8.state->samples[index];
        if (sample->touch == 1 && sample->validity == 0) {
            touched = 1;
            TP_GetCalibratedPoint(&calibrated, sample);
            WifiSetup_SetPoint(calibrated.x, calibrated.y, &ov13_0224F5E8.state->touch);
            break;
        }
        n++;
        index = FX_ModS32(index + 4, 5);
    } while (n < 4);
    changed = touched ^ wasTouching;
    ov13_0224F5E8.state->touchTrig = touched & changed;
    ov13_0224F5E8.state->touchRelease = wasTouching & changed;
    ov13_0224F5E8.state->touching = touched;
    ov13_0224F5E8.state->touchRepeat = ov13_0224F5E8.state->touchTrig;
    if (!touched) {
        ov13_0224F5E8.touchHoldFrames = 0;
        return;
    }
    ov13_0224F5E8.touchHoldFrames++;
    if (ov13_0224F5E8.touchHoldFrames == 40) {
        ov13_0224F5E8.state->touchRepeat = 1;
    } else if (ov13_0224F5E8.touchHoldFrames == 47) {
        ov13_0224F5E8.state->touchRepeat = 1;
        ov13_0224F5E8.touchHoldFrames = 40;
    }
}

BOOL WifiSetupInput_IsKeyPressed(u16 mask) {
    if (PAD_DetectFold()) {
        return FALSE;
    }
    return mask == (mask & ov13_0224F5E8.state->trig);
}

BOOL WifiSetupInput_IsKeyRepeated(u16 mask) {
    if (PAD_DetectFold()) {
        return FALSE;
    }
    return mask == (mask & ov13_0224F5E8.state->repeat);
}

BOOL WifiSetupInput_IsKeyReleased(u16 mask) {
    if (PAD_DetectFold()) {
        return FALSE;
    }
    return mask == (mask & ov13_0224F5E8.state->release);
}

BOOL WifiSetupInput_IsTouchHeld(WifiSetupRect *rect) {
    WifiSetupInputState *state;
    if (PAD_DetectFold()) {
        return FALSE;
    }
    state = ov13_0224F5E8.state;
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
    state = ov13_0224F5E8.state;
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
    state = ov13_0224F5E8.state;
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
    state = ov13_0224F5E8.state;
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
    state = ov13_0224F5E8.state;
    if (!state->touching) {
        *out = state->prevTouch;
        return FALSE;
    }
    *out = state->touch;
    return TRUE;
}

// Turns the LCDs off while the lid is closed and back on when it opens.
void WifiSetupInput_UpdateLcdPower(void) {
    if (ov13_0224F5E8.lcdOff) {
        if (PAD_DetectFold()) {
            return;
        }
        if (!PM_SetLCDPower(1)) {
            return;
        }
        ov13_0224F5E8.lcdOff = 0;
    } else {
        if (!PAD_DetectFold()) {
            return;
        }
        if (PM_SetLCDPower(0)) {
            ov13_0224F5E8.lcdOff = 1;
        }
    }
}

void WifiSetup_InstallVBlankIntr(void) {
    ov13_0224F600.mask = reg_OS_IE;
    OS_SetIrqMask(0x40018);
    OS_EnableIrqMask(1);
    ov13_0224F600.vblankFunc = OS_GetIrqFunction(1);
    OS_SetIrqFunction(1, WifiSetup_VBlankIntr);
    OS_ResetRequestIrqMask(1);
    OS_EnableIrq();
    OS_EnableInterrupts();
}

void WifiSetup_RestoreVBlankIntr(void) {
    OS_DisableIrq();
    OS_SetIrqMask(ov13_0224F600.mask);
    OS_SetIrqFunction(1, ov13_0224F600.vblankFunc);
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
    int irq = OS_DisableIrqMask(1);
    n->prev->next = n->next;
    n->next->prev = n->prev;
    n->next = NULL;
    n->prev = NULL;
    OS_EnableIrqMask(irq);
}

void WifiSetupList_InsertBefore(void *pos, void *node) {
    WifiSetupListNode *p = pos;
    WifiSetupListNode *n = node;
    int irq = OS_DisableIrqMask(1);
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
    ov13_0224F608 = buffer = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupOamBuffer), 4);
    MIi_CpuClearFast(0x200, (void *)buffer, 0x800);
    for (i = 0; i < 2; i++) {
        ov13_0224F608->pools[i] = WifiSetupPool_NewWithItems(0x40, ov13_0224F608->oam[i], 8);
    }
    ov13_0224F608->task = WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupOam_TransferTask, NULL, 0xC8);
}

void WifiSetupOam_TransferTask(void) {
    DC_FlushRange(ov13_0224F608, 0x800);
    GX_LoadOAM(ov13_0224F608->oam[0], 0, 0x400);
    GXS_LoadOAM(ov13_0224F608->oam[1], 0, 0x400);
}

void WifiSetupOam_Free(void) {
    WifiSetupTask_Destroy(1, ov13_0224F608->task);
    WifiSetupHeap_FreeAndClear(MODULE_BSS_ADDR(0x0224F608));
}

GXOamAttr *WifiSetupOam_Alloc(int screen) {
    return WifiSetupPool_Pop(ov13_0224F608->pools[screen]);
}

void WifiSetupOam_Release(GXOamAttr *oam) {
    int screen;
    oam->attr01 = (oam->attr01 & 0xC1FFFCFF) | 0x200; // hide
    screen = 0;
    if ((u32)oam >= (u32)ov13_0224F608->oam[1]) {
        screen = 1;
    }
    WifiSetupPool_Push(ov13_0224F608->pools[screen], oam);
}

GXOamAttr *WifiSetupOam_Get(int screen, int index) {
    return &ov13_0224F608->oam[screen][index];
}

void WifiSetupCharVram_Init(void) {
    int i;
    ov13_0224F60C = WifiSetupHeap_AllocZeroed(0x340, 4);
    for (i = 0; i < 2; i++) {
        ov13_0224F60C[i].pool = WifiSetupPool_NewWithItems(0x20, ov13_0224F60C[i].blocks, 0xC);
        ov13_0224F60C[i].list = WifiSetupList_New();
        ov13_0224F60C[i].first.start = 0x300;
        ov13_0224F60C[i].last.start = 0x400;
        WifiSetupList_Prepend(ov13_0224F60C[i].list, &ov13_0224F60C[i].first);
        WifiSetupList_Append(ov13_0224F60C[i].list, &ov13_0224F60C[i].last);
    }
}

// size in bytes; allocated in units of 4 (tile numbers 0x300-0x3FF are managed).
void *WifiSetupCharVram_Alloc(int screen, int size, int fromFront, u32 *outCharNo) {
    WifiSetupCharBlock *block;
    WifiSetupCharBlock *cur;
    int irq;
    int n;

    block = WifiSetupPool_Pop(ov13_0224F60C[screen].pool);
    n = ((size + 3) & ~3) >> 2;
    block->size = n;
    irq = OS_DisableIrqMask(1);
    if (fromFront) {
        cur = &ov13_0224F60C[screen].first;
        while (cur != &ov13_0224F60C[screen].last) {
            WifiSetupCharBlock *next = (WifiSetupCharBlock *)cur->node.next;
            int start = cur->start + cur->size;
            if (start + n <= next->start) {
                block->start = start;
                WifiSetupList_InsertBefore(next, block);
                break;
            }
            cur = next;
        }
        if (cur == &ov13_0224F60C[screen].last) {
            OS_Terminate();
        }
    } else {
        cur = &ov13_0224F60C[screen].last;
        while (cur != &ov13_0224F60C[screen].first) {
            WifiSetupCharBlock *prev = (WifiSetupCharBlock *)cur->node.prev;
            int start = cur->start - n;
            if (start >= prev->start + prev->size) {
                block->start = start;
                WifiSetupList_InsertBefore(cur, block);
                break;
            }
            cur = prev;
        }
        if (cur == &ov13_0224F60C[screen].first) {
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
    if ((u32)block >= (u32)&ov13_0224F60C[1]) {
        screen = 1;
    }
    WifiSetupPool_Push(ov13_0224F60C[screen].pool, block);
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
    ov13_0224F610.bg = GX_DisableBankForBG();
    ov13_0224F610.obj = GX_DisableBankForOBJ();
    ov13_0224F610.bgExtPltt = GX_DisableBankForBGExtPltt();
    ov13_0224F610.objExtPltt = GX_DisableBankForOBJExtPltt();
    ov13_0224F610.tex = GX_DisableBankForTex();
    ov13_0224F610.texPltt = GX_DisableBankForTexPltt();
    ov13_0224F610.clearImage = GX_DisableBankForClearImage();
    ov13_0224F610.subBg = GX_DisableBankForSubBG();
    ov13_0224F610.subObj = GX_DisableBankForSubOBJ();
    ov13_0224F610.subBgExtPltt = GX_DisableBankForSubBGExtPltt();
    ov13_0224F610.subObjExtPltt = GX_DisableBankForSubOBJExtPltt();
    ov13_0224F610.arm7 = GX_DisableBankForARM7();
    ov13_0224F610.lcdc = GX_DisableBankForLCDC();
    GX_SetBankForARM7(ov13_0224F610.arm7);
    WifiSetup_ClearVram();
}

void WifiSetup_RestoreVramBanks(void) {
    OSIntrMode irq;
    GX_DisableBankForBG();
    GX_DisableBankForOBJ();
    GX_DisableBankForSubBG();
    GX_DisableBankForSubOBJ();
    WifiSetup_ClearVram();
    GX_SetBankForBG(ov13_0224F610.bg);
    GX_SetBankForOBJ(ov13_0224F610.obj);
    GX_SetBankForBGExtPltt(ov13_0224F610.bgExtPltt);
    GX_SetBankForOBJExtPltt(ov13_0224F610.objExtPltt);
    GX_SetBankForTex(ov13_0224F610.tex);
    GX_SetBankForTexPltt(ov13_0224F610.texPltt);
    GX_SetBankForClearImage(ov13_0224F610.clearImage);
    GX_SetBankForSubBG(ov13_0224F610.subBg);
    GX_SetBankForSubOBJ(ov13_0224F610.subObj);
    GX_SetBankForSubBGExtPltt(ov13_0224F610.subBgExtPltt);
    GX_SetBankForSubOBJExtPltt(ov13_0224F610.subObjExtPltt);
    GX_SetBankForLCDC(ov13_0224F610.lcdc);
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

void ov13_022422E0(void) {
    u32 pattern;
    if (PM_GetLEDPattern(&pattern) != 0) {
        return;
    }
    if (pattern == 0xF) {
        return;
    }
    PMi_SendLEDPatternCommand(0xF);
}

void ov13_0224230C(void) {
    PMi_SendLEDPatternCommand(1);
}

void WifiSetupTask_InitScheduler(void) {
    int i;
    u8 *capacity;
    ov13_0224F644 = WifiSetupHeap_AllocZeroed(0x80, 4);
    capacity = ov13_02245960;
    for (i = 0; i < 2; i++) {
        ov13_0224F644[i].capacity = *capacity;
        ov13_0224F644[i].taskBuf = WifiSetupHeap_AllocZeroed(*capacity * 0x14, 4);
        ov13_0224F644[i].taskPool = WifiSetupPool_NewWithItems(*capacity, ov13_0224F644[i].taskBuf, 0x14);
        ov13_0224F644[i].destroyQueue = WifiSetupPool_New(*capacity++);
        ov13_0224F644[i].list = WifiSetupList_New();
        ov13_0224F644[i].head.priority = 0;
        ov13_0224F644[i].tail.priority = 0xFF;
        WifiSetupList_Prepend(ov13_0224F644[i].list, &ov13_0224F644[i].head);
        WifiSetupList_Append(ov13_0224F644[i].list, &ov13_0224F644[i].tail);
        ov13_0224F644[i].enabled = 1;
    }
}

void WifiSetupTask_FreeScheduler(void) {
    int i;
    for (i = 0; i < 2; i++) {
        WifiSetupList_Delete(ov13_0224F644[i].list);
        WifiSetupPool_Delete(ov13_0224F644[i].taskPool);
    }
    WifiSetupHeap_FreeAndClear(MODULE_BSS_ADDR(0x0224F644));
}

// Runs every task of the queue in priority order, then destroys the tasks queued for destruction.
void WifiSetupTask_RunQueue(int queue) {
    WifiSetupTask *task;
    int i;
    if (!ov13_0224F644[queue].enabled) {
        return;
    }
    for (task = (WifiSetupTask *)ov13_0224F644[queue].head.node.next; task != &ov13_0224F644[queue].tail; task = (WifiSetupTask *)task->node.next) {
        task->func(task, task->arg);
    }
    for (i = 0; i < ov13_0224F644[queue].capacity; i++) {
        task = WifiSetupPool_Pop(ov13_0224F644[queue].destroyQueue);
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
    task = WifiSetupPool_Pop(ov13_0224F644[queue].taskPool);
    task->func = func;
    task->arg = arg;
    task->priority = priority;
    task->ownsArg = ownsArg;
    irq = OS_DisableIrqMask(1);
    for (cur = (WifiSetupTask *)ov13_0224F644[queue].head.node.next;; cur = (WifiSetupTask *)cur->node.next) {
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
    WifiSetupPool_Push(ov13_0224F644[queue].destroyQueue, task);
}

void WifiSetupTask_Destroy(int queue, void *task) {
    WifiSetupTask_DestroyNow(queue, task);
}

void WifiSetupTask_DestroyNow(int queue, WifiSetupTask *task) {
    if (task->ownsArg) {
        WifiSetupHeap_FreeAndClear(&task->arg);
    }
    WifiSetupList_Remove(task);
    WifiSetupPool_Push(ov13_0224F644[queue].taskPool, task);
}

void WifiSetupTask_SetQueueEnabled(int queue, u8 enabled) {
    ov13_0224F644[queue].enabled = enabled;
}
