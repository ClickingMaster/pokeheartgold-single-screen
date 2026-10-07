#ifndef OVERLAY_37_LIFECYCLE_PRIVATE_H
#define OVERLAY_37_LIFECYCLE_PRIVATE_H

#include "global.h"

#include "overlay_37_work.h"


typedef struct Ov37GraphicsBanks {
    u32 bg;
    u32 bgextpltt;
    u32 subbg;
    u32 subbgextpltt;
    u32 obj;
    u32 objextpltt;
    u32 subobj;
    u32 subobjextpltt;
    u32 tex;
    u32 texpltt;
} Ov37GraphicsBanks;

typedef struct Ov37GraphicsModes {
    u32 dispMode;
    u32 bgMode;
    u32 subMode;
    u32 _2d3dMode;
} Ov37GraphicsModes;

typedef struct Ov37BgTemplate {
    u32 x;
    u32 y;
    u32 bufferSize;
    u32 baseTile;
    u8 size;
    u8 colorMode;
    u8 screenBase;
    u8 charBase;
    u8 bgExtPltt;
    u8 priority;
    u8 areaOver;
    u8 dummy;
    u32 mosaic;
} Ov37BgTemplate;

/* module-local blobs, left in the assembly */
extern const Ov37StateEntry ov37_021E7D20[];
extern const Ov37GraphicsBanks ov37_021E7A24;
extern const Ov37GraphicsModes ov37_021E7978;
extern const Ov37BgTemplate ov37_021E7998;
extern const Ov37BgTemplate ov37_021E79B4;
extern const Ov37BgTemplate ov37_021E79D0;
extern const Ov37BgTemplate ov37_021E79EC;
extern const Ov37BgTemplate ov37_021E7A08;

/* external game functions */
extern void GF_RunVramTransferTasks(void);
extern void OamManager_ApplyAndResetBuffers(void);
extern void DoScheduledBgGpuUpdates(void *bgConfig);
extern void Main_SetVBlankIntrCB(void (*cb)(void *), void *arg);
extern void HBlankInterruptDisable(void);
extern void GfGfx_DisableEngineAPlanes(void);
extern void GfGfx_DisableEngineBPlanes(void);
extern void GfGfx_SetBanks(const Ov37GraphicsBanks *banks);
extern void SetBothScreensModesAndDisable(const Ov37GraphicsModes *modes);
extern void InitBgFromTemplate(void *bgConfig, u32 bgId, const Ov37BgTemplate *tmpl, u32 bgType);
extern void BgClearTilemapBufferAndCommit(void *bgConfig, u32 bgId);
extern void BG_ClearCharDataRange(u32 bgId, u32 size, u32 offset, u32 heapId);
extern void Heap_Create(u32 parent, u32 child, u32 size);
extern void Heap_Destroy(u32 heapId);
extern void *Heap_Alloc(u32 heapId, u32 size);
extern void Heap_Free(void *ptr);
extern void *OverlayManager_CreateAndGetData(void *manager, u32 size, u32 heapId);
extern void *OverlayManager_GetData(void *manager);
extern void *OverlayManager_GetArgs(void *manager);
extern void OverlayManager_FreeData(void *manager);
extern void *BgConfig_Alloc(u32 heapId);
extern void *MessageFormat_New(u32 heapId);
extern void MessageFormat_Delete(void *msgFormat);
extern void *NewMsgDataFromNarc(u32 lazy, u32 narcId, u32 fileId, u32 heapId);
extern void DestroyMsgData(void *msgData);
extern void ReadMsgDataIntoString(void *msgData, u32 entry, void *dest);
extern void SetKeyRepeatTimers(u32 a, u32 b);
extern void *NARC_New(u32 narcId, u32 heapId);
extern void NARC_Delete(void *narc);
extern void *String_New(u32 len, u32 heapId);
extern void String_Delete(void *str);
extern void BeginNormalPaletteFade(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 heapId);
extern u32 IsPaletteFadeFinished(void);
extern void Sound_SetSceneAndPlayBGM(u32 scene, u32 a1, u32 a2);
extern void SpriteList_RenderAndAnimateSprites(void *list);
extern void SpriteList_Delete(void *list);
extern void SpriteTransfer_DeleteCharTransferTask(void *task);
extern void SpriteTransfer_DeletePlttTransferTask(void *task);
extern void Destroy2DGfxResObjMan(void *man);
extern void OamManager_Free(void);
extern void ObjCharTransfer_Destroy(void);
extern void ObjPlttTransfer_Destroy(void);
extern void *YesNoPrompt_Create(u32 heapId);
extern u32 sub_02033250(void);
extern u32 sub_0203769C(void);
extern void sub_02037030(u32 a0, u32 a1, u32 a2);
extern void sub_02037FF0(void);
extern void sub_02038C1C(u32 a0);
extern void sub_020398D4(u32 a0, u32 a1);
extern void sub_0203A880(void);
extern void sub_020356EC(u32 a0);
extern void sub_0205A904(u32 a0);
extern void sub_0205AD24(u32 a0);
extern void sub_0200FBF4(u32 a0, u32 a1);
extern void sub_020210BC(void);
extern void sub_02021148(u32 a0);
extern void sub_02021238(void);
extern void sub_0208F814(void *work);

/* module functions declared elsewhere in overlay_37 */
extern void ov37_021E5F20(Ov37Work *work);
extern void ov37_021E5F5C(void *bgConfig);
extern void ov37_021E5F98(Ov37Work *work, void *narc);
extern void ov37_021E6090(void);
extern void ov37_021E60C0(Ov37Work *work, void *narc);
extern void ov37_021E6244(Ov37Work *work);
extern void ov37_021E6418(Ov37Work *work, void *manager);
extern void ov37_021E6540(Ov37Work *work);
extern void ov37_021E741C(u16 *arg0);
extern void ov37_021E7478(Ov37Window *windows, u32 a1, u32 a2, Ov37Work *work);
extern s32 ov37_021E75C4(void);
extern u32 ov37_021E76F0(Ov37Work *work);
extern void ov37_021E7844(Ov37Work *work, u32 state);
extern void ov37_021E784C(Ov37Work *work, u32 arg1);

/* functions defined in this file */
void ov37_021E5CC8(void *bgConfig);
void ov37_021E5CF0(void);
void ov37_021E5D10(void *bgConfig);
void ov37_021E5E30(Ov37Work *work);
u32 ov37_021E5900(void *manager, u32 *state);
u32 ov37_021E5A84(void *manager, u32 *state);
u32 ov37_021E5B94(void *manager, u32 *state);

#endif
