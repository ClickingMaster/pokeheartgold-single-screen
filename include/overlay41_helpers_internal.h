#ifndef OV41_PRIVATE_H
#define OV41_PRIVATE_H
#include "global.h"

#include "system.h"
#include "touchscreen.h"

/* Placeholder work struct: word-indexed. Codegen-identical to a named-field
   struct for the word-sized accesses this module makes. */
typedef struct Ov41Work {
    u32 f[512];
} Ov41Work;
#define W(w, off) ((w)->f[(off) / 4])
typedef struct Ov41E12 {
    u32 a;
    u32 b;
    u32 c;
} Ov41E12;
#define AT(w, off) ((void *)((u8 *)(w) + (off)))

/* --- untracked externals: prototypes only (mwcc emits bl + relocation) --- */
void Heap_Free(void *ptr);
void GF_AssertFail(void);
void *memset(void *, int, u32);

void GraphicsObject_SetPosition(void *);
void GraphicsObject_SetVisible(void *);
void GraphicsObject_SetDepth(void *);
void GraphicsManager_FreeImages(void *);
void GraphicsManager_FreePalettes(void *);
void GraphicsManager_Delete(void *);
void GraphicsManager_DrawObjects(void *);

void *SpriteResourceCollection_Find(void *mgr, int id);
void DestroySingle2DGfxResObj(void *mgr, void *res);
void *AddCellOrAnimResObjFromOpenNarc(void *resMan, void *narc, int fileId, BOOL compressed, int id, int type, int heapId);
void *AddCharResObjFromOpenNarc(void *resMan, void *narc, int fileId, BOOL compressed, int a4, int a5, int heapId);
void SpriteTransfer_CreateCharTransferTask_AllocAtEnd(void *res);
void *GfGfxLoader_LoadFromOpenNarc(void *narc, int fileId, BOOL compressed, int a3, int heapId);
void DoScheduledBgGpuUpdates(void *bgConfig);
void PokepicManager_HandleLoadImgAndOrPltt(void *mgr);
void OamManager_ApplyAndResetBuffers(void);
void FreeBgTilemapBuffer(void *bgConfig, int layer);
void BgClearTilemapBufferAndCommit(void *bgConfig, int layer);
void NNS_GfdResetFrmTexVramState(void);
void NNS_GfdResetFrmPlttVramState(void);
void GfGfx_DisableEngineAPlanes(void);
void GfGfx_DisableEngineBPlanes(void);
void GfGfx_SwapDisplay(void);
void NNS_G2dInitOamManagerModule(void);
void GF2dGfxRawResObj_Destroy(void *man);
void SpriteList_RenderAndAnimateSprites(void *list);
void ClearWindowTilemapAndCopyToVram(void *window);
void RemoveWindow(void *window);
void YesNoPrompt_Reset(void *prompt);

/* --- in-module callees not transcribed in this candidate --- */
void *DressUpGfx_FindFreeSlot(void *);
void *DressUpGfx_CreateSlotGraphicsObject(void *);
void DressUpGfx_GetSlotHitbox(void *, TouchscreenHitbox *);
void DressUp_FreeGfxCore(void *);
void DressUp_DrawFrame3D(void *);
void DressUpGfx_AddImages(void *, u32, u32);
void DressUpGfx_AddPalettes(void *, u32, u32);
void DressUpGfx_SetVramBanks(void);
void DressUpGfx_Init3D(void);
void DressUpGfx_Init2DModes(void);
void DressUpGfx_AllocRawResTables(void *, int, int, void *);
void DressUpGfx_FreeRawResTables(void *);
void DressUp_CreateMsgWindow(void *, void *, int, int, int, int);
void DressUp_PrintMsgWindow(void *);
void ov41_02248164(void *, int);
void DressUpCanvas_LinkByMonOverlap(void *, void *, int, void *);
void DressUpTray_CyclePage(void *, void *, int);
void DressUpTray_Destroy(void *);
void DressUpTray_ClearObjects(void *);
void *DressUpTray_GetCurPageHead(void *);
void *DressUpTray_GetPageHead(void *, int, int);
void DressUpTray_SetListVisible(void *, void *);
void DressUpMon_Delete(void *);
void *DressUpList_InsertAfter(void *, void *);
void DressUpList_Remove(void *);
void DressUpList_Clear(void *);
void DressUpObj_MoveListBy(void *, void *, void *);
void DressUpBgScreen_Unload(void *);
void DressUpButtonPanel_Destroy(void *);
void DressUpButtonPanel_Update(void *);
void ov41_0224A580(void *);
void DressUpHud_Free(void *);
void DressUpHud_SetAccessoryCount(void *, u32);

void *DressUp_FindFreeSlideAnim(void *, void *);
void DressUp_TickSlideAnim(void *);
void DressUpTouch_Reset(void *);
void DressUpObj_GetVisibleRect(void *, void *, void *, void *, void *);
void DressUpMon_CreateEx(void *, void *, void *, void *, void *, int);
void Pokepic_SetAttr(void *, int, u32);
u32 Pokepic_GetAttr(void *, int);

void GF2dGfxRawResMan_AllocObj(void *);
void NARC_Delete(void *);
void DressUpGfx_FreeBgs(void *);
void DressUpGfx_FreeSpriteSystem(void *);

#endif
