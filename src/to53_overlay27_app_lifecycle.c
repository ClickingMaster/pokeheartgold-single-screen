#include "global.h"

#include "poke_overlay.h"

FS_EXTERN_OVERLAY(ds_protect);

typedef struct Ov27Work {
    u8 d[0x530];
} Ov27Work;

typedef struct Ov27FieldSystem {
    u8 pad00[0xd2];
    u8 state : 6;
    u8 stateBit6 : 1;
    u8 stateBit7 : 1;
} Ov27FieldSystem;

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

extern void *SysTask_GetData(void *);
extern int DSProt_DetectFlashcart(void *);
extern int DSProt_DetectNotEmulator(void *);
extern int DSProt_DetectNotDummy(void *);
extern void *Heap_AllocAtEnd(int, u32);
extern void ov27_0225BEB0(void *);
extern void DestroyMsgData(void *);
extern void MessageFormat_Delete(void *);
extern void SpriteTransfer_DeleteCharTransferTask(void *);
extern void SpriteTransfer_DeletePlttTransferTask(void *);
extern void Destroy2DGfxResObjMan(void *);
extern void SpriteList_Delete(void *);
extern void RemoveWindow(void *);
extern void ov27_0225BC34(void *);
extern void FontID_Release(int);
extern void DestroySysTaskAndEnvironment(void *);
extern void FreeBgTilemapBuffer(void *, int);
extern void Heap_Destroy(int);
extern void ov27_0225C238(void);
extern void ov27_0225C248(void);
extern void ov27_0225C24C(void);
extern void Heap_Create(int, int, u32);
extern void InitBgFromTemplate(void *, int, const void *, int);
extern void *CreateSysTaskAndEnvironment(void *, u32, int, int);
extern u32 ov27_0225BD50(void *);
extern void ov27_0225AC00(void *, u32, void *, void *, void *);
extern void FontID_Alloc(int, int);
extern void *MessageFormat_New(int);
extern void *NewMsgDataFromNarc(int, int, int, int);
extern void ov27_0225C10C(void *);
extern void *ov27_0225C1AC(void *, int);
extern void ov27_0225C1EC(void *);
extern void ov27_0225AD0C(void *);
extern void ov27_0225B010(void *);
extern void ov27_0225BB6C(void *, int);
extern void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, int);
extern void ov27_0225BCE8(void *);
extern void ov27_0225BC84(void *);
extern void ov27_0225A690(void *, int);
extern void ov27_0225C0E0(void *);
extern void ov27_0225BDDC(void *, void *);
extern int MenuInputStateMgr_GetState(void *);
extern int FieldSystem_TaskIsRunning(void *);
extern int ov27_0225A714(void *);
extern void ov27_0225A7FC(void *);
extern void SpriteList_RenderAndAnimateSprites(void *);
extern void GfGfx_EngineBTogglePlanes(int, int);
extern void ov27_0225A320(void);
extern const u8 ov27_0225D000[];
extern const u8 ov27_0225D01C[];

void *ov27_02259F80(void *, void *, void *, void *);

void ov27_0225A19C(void *, void *);

void *ov27_02259F80(void *bgConfig, void *arg1, void *fieldSystem, void *arg3) {
    void *task;
    void *format;
    u8 *work;
    u32 value;
    int message;

    Heap_Create(3, 8, 0x18d00);
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & 0xFFCFFFEF) | 0x10;
    InitBgFromTemplate(bgConfig, 4, ov27_0225D000, 0);
    InitBgFromTemplate(bgConfig, 5, ov27_0225D01C, 0);
    task = CreateSysTaskAndEnvironment(ov27_0225A320, 0x540, 10, 8);
    work = SysTask_GetData(task);
    PTR(work, 8) = task;
    *(u32 *)(work + 0) = 0;
    PTR(work, 4) = bgConfig;
    PTR(work, 0xc) = arg1;
    PTR(work, 0x10) = fieldSystem;
    value = ov27_0225BD50(fieldSystem);
    *(u32 *)(work + 0x51c) = (*(u32 *)(work + 0x51c) & ~0x1e) | ((value << 28) >> 27);
    *(u32 *)(work + 0x51c) &= ~0x20;
    ov27_0225AC00(bgConfig, *(u32 *)work, work + 0x3d0, work + 0x3e0, work + 0x3f0);
    FontID_Alloc(4, 8);
    format = MessageFormat_New(8);
    message = 0xc4;
    PTR(work, 0x4ac) = format;
    PTR(work, 0x4a8) = NewMsgDataFromNarc(0, 0x1b, message, 8);
    ov27_0225C10C(work);
    PTR(work, 0x14) = ov27_0225C1AC(work, *(u8 *)((u8 *)fieldSystem + 0xd3));
    ov27_0225C1EC(work);
    ov27_0225AD0C(work);
    ov27_0225B010(work);
    ov27_0225BB6C(work, ((Ov27FieldSystem *)PTR(work, 0x10))->state);
    AddTextPrinterParameterizedWithColor(work + 0x3d0, 4, PTR(work, 0x4b4), 0, 0, 0, 0xf0100, 0);
    AddTextPrinterParameterizedWithColor(work + 0x3e0, 0, PTR(work, 0x4c4), 0, 0, 0xff, 0xf0100, 0);
    ov27_0225BCE8(work);
    ov27_0225BC84(work);
    ov27_0225A690(work, 1);
    ov27_0225C0E0(work);
    ov27_0225BDDC(work + 0x520, work);
    if (MenuInputStateMgr_GetState((u8 *)fieldSystem + 0x10c) == 0) {
        *(u8 *)((u8 *)fieldSystem + 0xd2) &= ~0x80;
    } else if (!FieldSystem_TaskIsRunning(fieldSystem)) {
        *(u8 *)((u8 *)fieldSystem + 0xd2) |= 0x80;
    }
    if (!ov27_0225A714(work)) {
        return task;
    }
    ov27_0225A7FC(work);
    SpriteList_RenderAndAnimateSprites(PTR(work, 0x18));
    *(vu32 *)0x04001000 &= 0xffff1fff;
    GfGfx_EngineBTogglePlanes(1, 1);
    GfGfx_EngineBTogglePlanes(2, 1);
    GfGfx_EngineBTogglePlanes(4, 0);
    GfGfx_EngineBTogglePlanes(8, 0);
    GfGfx_EngineBTogglePlanes(0x10, 1);
    return task;
}

void ov27_0225A19C(void *bgConfig, void *task) {
    Ov27Work *work = SysTask_GetData(task);

    FS_LoadOverlay(MI_PROCESSOR_ARM9, FS_OVERLAY_ID(ds_protect));
    if (DSProt_DetectFlashcart(ov27_0225C238)) {
        Heap_AllocAtEnd(3, 1000);
    }
    ov27_0225BEB0((u8 *)work + 0x520);
    DestroyMsgData(PTR(work, 0x4a8));
    MessageFormat_Delete(PTR(work, 0x4ac));
    {
        int i = 0;
        u8 *scan = (u8 *)work;
        for (; i < 11; i++, scan += 0x10) {
            SpriteTransfer_DeleteCharTransferTask(PTR(scan, 0x154));
        }
    }
    {
        u8 *scan;
        int i = 0;
        scan = (u8 *)work;
        for (; i < 11; i++, scan += 0x10) {
            SpriteTransfer_DeletePlttTransferTask(PTR(scan, 0x158));
        }
    }
    {
        u8 *scan;
        int i = 0;
        scan = (u8 *)work;
        for (; i < 4; i++, scan += 4) {
            Destroy2DGfxResObjMan(PTR(scan, 0x144));
        }
    }
    if (!DSProt_DetectNotEmulator(ov27_0225C248)) {
        Heap_AllocAtEnd(3, 1000);
    }
    SpriteList_Delete(PTR(work, 0x18));
    {
        u8 *scan;
        int i = 0;
        scan = (u8 *)work + 0x3f0;
        for (; i < 8; i++, scan += 0x10) {
            RemoveWindow(scan);
        }
    }
    RemoveWindow((u8 *)work + 0x3e0);
    RemoveWindow((u8 *)work + 0x3d0);
    ov27_0225BC34(work);
    FontID_Release(4);
    DestroySysTaskAndEnvironment(task);
    FreeBgTilemapBuffer(bgConfig, 5);
    FreeBgTilemapBuffer(bgConfig, 4);
    Heap_Destroy(8);
    if (!DSProt_DetectNotDummy(ov27_0225C24C)) {
        Heap_AllocAtEnd(3, 1000);
    }
    FS_UnloadOverlay(MI_PROCESSOR_ARM9, FS_OVERLAY_ID(ds_protect));
}
