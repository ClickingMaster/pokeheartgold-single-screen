#ifndef TO52_SOL_R6_W06_PRIVATE_H
#define TO52_SOL_R6_W06_PRIVATE_H
#include "global.h"
typedef struct Ov96R12Work {
    u32 heapId;
    u16 count;
    u16 cursor;
    void *unk08;
    void *task;
    u32 unk10;
    void *spriteList;
    void *renderer2;
    u8 renderer[0x128];
    void *managers[4];
    void *records;
} Ov96R12Work;
typedef struct Ov96R12Task {
    u16 count;
    u16 cursor;
    void *unk04;
    u32 unk08;
    Ov96R12Work *work;
    struct Ov96R12Template {
        u32 words[17];
    } template;
    struct Ov96R12Record {
        u32 words[4];
    } records[];
} Ov96R12Task;
void GF_AssertFail(void);
void *Heap_Alloc(u32, u32);
void *Heap_AllocAtEnd(u32, u32);
void Heap_Free(void *);
void *G2dRenderer_Init(u32, void *, u32);
void *Create2DGfxResObjMan(u32, u32, u32);
void Destroy2DGfxResObjMan(void *);
void SpriteTransfer_DeleteCharTransferTask(void *);
void SpriteTransfer_DeletePlttTransferTask(void *);
void SpriteList_Delete(void *);
void *SysTask_CreateOnMainQueue(void (*)(void *), void *, u32);
void ov96_021E8FB4(void *);
void ov96_021E8A44(Ov96R12Work *, u32, void *);
void ov96_021E8AAC(Ov96R12Work *);
void ov96_021E8B1C(Ov96R12Work *, s32, struct Ov96R12Record *, struct Ov96R12Template *, u32, void *);
#endif
