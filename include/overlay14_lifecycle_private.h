#ifndef OVERLAY14_LIFECYCLE_PRIVATE_H
#define OVERLAY14_LIFECYCLE_PRIVATE_H

#include "global.h"

typedef struct OverlayManager OverlayManager;
typedef struct SaveData SaveData;

typedef struct PCBoxArgs {
    SaveData *saveData;
    void *menuInputStatePtr;
    int unk8;
} PCBoxArgs;

typedef struct PCBoxInner {
    u8 pad00[4];
    BOOL (*callback)(void *);
    u8 pad08[0x0C];
    void *bgConfig;
    void *palette;
    u8 pad1C[0x2D4];
    void *spriteSystem;
    u8 pad2F4[0x85DE];
    u16 disableSpriteDraw;
} PCBoxInner;

typedef struct PCBoxWork {
    PCBoxArgs *args;
    void *storage;
    void *party;
    void *bag;
    void *options;
    u8 pad14[0x0B];
    u8 activeBox;
    u8 pad20;
    u8 unk21;
    u8 pad22[0x0A];
    u32 unk2C;
    u32 unk30;
    PCBoxInner *inner;
} PCBoxWork;

extern void Heap_Create(u32, u32, u32);
extern void Heap_Destroy(u32);
extern void *OverlayManager_CreateAndGetData(OverlayManager *, u32, u32);
extern void *OverlayManager_GetArgs(OverlayManager *);
extern void *OverlayManager_GetData(OverlayManager *);
extern void OverlayManager_FreeData(OverlayManager *);
extern void MI_CpuFill8(void *, u8, u32);
extern void *SaveArray_PCStorage_Get(SaveData *);
extern u32 PCStorage_GetActiveBox(void *);
extern void PCStorage_SetActiveBox(void *, u32);
extern void *SaveArray_Party_Get(SaveData *);
extern void *Save_Bag_Get(SaveData *);
extern void *Save_PlayerData_GetOptionsAddr(SaveData *);
extern BOOL ov14_021EAF8C(PCBoxWork *, int *);
extern void ov14_021E5A14(PCBoxWork *);
extern void PaletteData_PushTransparentBuffers(void *);
extern void DoScheduledBgGpuUpdates(void *);
extern void ov14_021F29C4(PCBoxInner *);
extern void SpriteSystem_DrawSprites(void *);
extern void SpriteSystem_TransferOam(void);
extern void sub_02019978(void *, u32);
extern void ov14_021F3BC0(PCBoxInner *);

BOOL PCBox_Init(OverlayManager *, int *);
BOOL PCBox_Main(OverlayManager *, int *);
BOOL PCBox_Exit(OverlayManager *, int *);
void ov14_021E59AC(void *, PCBoxWork *);

#endif
