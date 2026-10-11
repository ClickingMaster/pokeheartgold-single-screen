#ifndef SOL_R40_NIGHT_OVERLAY18_RESIDUAL40_PRIVATE_H
#define SOL_R40_NIGHT_OVERLAY18_RESIDUAL40_PRIVATE_H
#include "global.h"
void *G2dRenderer_Init(int, void *, int);
void ClearMainOAM(int);
void GfGfx_EngineBTogglePlanes(u32, u32);
void SpriteTransfer_DeleteCharTransferTask(void *);
void DestroySingle2DGfxResObj(void *, void *);
void PokedexEntryPopup_InitObjCharTransfer(void *);
void PokedexEntryPopup_CreateResourceManagers(void *);
void ov18_021F8FF8(void *);
void PokedexEntryPopup_LoadTypeIconResources(void *);
void PokedexEntryPopup_LoadFootprintResources(void *);
void ov18_021F94BC(void *);
void PokedexEntryPopup_CreateTypeIconSprites(void *);
void PokedexEntryPopup_CreateFootprintSprite(void *);
void PokedexEntryPopup_FreeSpriteResources(void *, void *);
void PokedexEntryPopup_LoadSpriteResources(void *, void *, u32, void *, s32, s32, s32, s32, u32, u32, u32, u32, u32);
void *PokedexEntryPopup_LoadPaletteResource(void *, u32, void *, s32, u32, u32);
u32 PokedexApp_GetTypeIconCharFile(u32);
u32 ov18_021E5900(void);
s32 ov18_021E5908(void);
void *NARC_New(u32, u32);
void NARC_Delete(void *);
u32 SpriteTransfer_GetPlttOffset(void *, u32);
void PaletteData_LoadPaletteSlotFromHardware(void *, u32, u16, u32);
void PokedexEntryPopup_InitSprites(void *);
void ov18_021F8FF8(void *);
void ov18_021F9054(void *);
void PokedexEntryPopup_LoadTypeIconResources(void *);
void PokedexEntryPopup_UnloadTypeIconResources(void *);
void PokedexEntryPopup_LoadFootprintResources(void *);
void PokedexEntryPopup_UnloadFootprintResources(void *);
#endif
