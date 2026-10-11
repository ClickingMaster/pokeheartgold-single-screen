#include "overlay18_resource_groups_private.h"
#define PTR(p, o)    (*(void **)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

void PokedexEntryPopup_InitSprites(void *p) {
    PTR(p, 0xB4) = G2dRenderer_Init(0x20, (u8 *)p + 0xB8, *(int *)((u8 *)p + 0x14));
    ClearMainOAM(*(int *)((u8 *)p + 0x14));
    PokedexEntryPopup_InitObjCharTransfer(p);
    PokedexEntryPopup_CreateResourceManagers(p);
    ov18_021F8FF8(p);
    PokedexEntryPopup_LoadTypeIconResources(p);
    PokedexEntryPopup_LoadFootprintResources(p);
    ov18_021F94BC(p);
    PokedexEntryPopup_CreateTypeIconSprites(p);
    PokedexEntryPopup_CreateFootprintSprite(p);
    GfGfx_EngineBTogglePlanes(0x10, 1);
}

void ov18_021F8FF8(void *p) {
    PokedexEntryPopup_LoadSpriteResources((u8 *)p + 0x1F4, (u8 *)p + 0x1E0, *(u32 *)((u8 *)p + 0x14), *(void **)((u8 *)p + 0x1C), 0x1D, 0x20, 0x1E, 0x1F, 2, 0xC618, 0xC618, 0xC618, 0xC618);
    PaletteData_LoadPaletteSlotFromHardware(PTR(p, 4), 2, SpriteTransfer_GetPlttOffset(PTR(p, 0x1F8), 1) << 4, 0x40);
}

void ov18_021F9054(void *p) {
    PokedexEntryPopup_FreeSpriteResources((u8 *)p + 0x1F4, (u8 *)p + 0x1E0);
}

void PokedexEntryPopup_LoadTypeIconResources(void *p) {
    PokedexEntryPopup_LoadSpriteResources((u8 *)p + 0x208, (u8 *)p + 0x1E0, *(u32 *)((u8 *)p + 0x14), *(void **)((u8 *)p + 0x1C), PokedexApp_GetTypeIconCharFile(U32_AT(p, 0x24C)), 0x23, 0x21, 0x22, 4, 0xC619, 0xC619, 0xC619, 0xC619);
    PokedexEntryPopup_LoadSpriteResources((u8 *)p + 0x21C, (u8 *)p + 0x1E0, *(u32 *)((u8 *)p + 0x14), *(void **)((u8 *)p + 0x1C), PokedexApp_GetTypeIconCharFile(U32_AT(p, 0x250)), -1, 0x21, 0x22, 4, 0xC61A, 0xC61A, 0xC61A, 0xC61A);
    PaletteData_LoadPaletteSlotFromHardware(PTR(p, 4), 2, SpriteTransfer_GetPlttOffset(PTR(p, 0x20C), 1) << 4, 0x80);
}

void PokedexEntryPopup_UnloadTypeIconResources(void *p) {
    PokedexEntryPopup_FreeSpriteResources((u8 *)p + 0x208, (u8 *)p + 0x1E0);
    SpriteTransfer_DeleteCharTransferTask(PTR(p, 0x21C));
    DestroySingle2DGfxResObj(PTR(p, 0x1E0), PTR(p, 0x21C));
    DestroySingle2DGfxResObj(PTR(p, 0x1E8), PTR(p, 0x224));
    DestroySingle2DGfxResObj(PTR(p, 0x1EC), PTR(p, 0x228));
}

void PokedexEntryPopup_LoadFootprintResources(void *p) {
    void *narc = NARC_New(ov18_021E5900(), *(u32 *)((u8 *)p + 0x14));
    PokedexEntryPopup_LoadSpriteResources((u8 *)p + 0x230, (u8 *)p + 0x1E0, *(u32 *)((u8 *)p + 0x14), *(void **)((u8 *)p + 0x1C), 0x4D, -1, 0x4E, 0x4F, 1, 0xC61B, 0xC61B, 0xC61B, 0xC61B);
    PTR(p, 0x234) = PokedexEntryPopup_LoadPaletteResource(PTR(p, 0x1E4), *(u32 *)((u8 *)p + 0x14), narc, ov18_021E5908(), 1, 0xC61B);
    PaletteData_LoadPaletteSlotFromHardware(PTR(p, 4), 2, SpriteTransfer_GetPlttOffset(PTR(p, 0x234), 1) << 4, 0x20);
    NARC_Delete(narc);
}

void PokedexEntryPopup_UnloadFootprintResources(void *p) {
    PokedexEntryPopup_FreeSpriteResources((u8 *)p + 0x230, (u8 *)p + 0x1E0);
}
