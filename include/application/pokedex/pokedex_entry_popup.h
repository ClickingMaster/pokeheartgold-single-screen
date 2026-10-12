#ifndef POKEHEARTGOLD_APPLICATION_POKEDEX_POKEDEX_ENTRY_POPUP_H
#define POKEHEARTGOLD_APPLICATION_POKEDEX_POKEDEX_ENTRY_POPUP_H

#include "application/pokedex/pokedex_text_util.h"
#include "battle/battle.h"

#include "bg_window.h"
#include "overlay_18.h"
#include "palette.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "unk_0200A090.h"

// Sprites of the popup: 0 banner, 1 and 2 the type icons, 3 the footprint
typedef struct PokedexEntryPopupSprite {
    Sprite *sprite;
    SpriteResource *resources[4]; // char, palette, cell, anim
} PokedexEntryPopupSprite;

// "Registered in the Pokedex" popup after a capture (msg_0802). Starts with a copy of UnkStruct_50C; created
// by PokedexEntryPopup_Create, run by PokedexEntryPopup_Task.
typedef struct PokedexEntryPopup {
    BgConfig *bgConfig;                  // 0x000
    PaletteData *paletteData;            // 0x004
    PokepicManager *pokepicManager;      // 0x008
    Pokemon *mon;                        // 0x00C
    BOOL natDexEnabled;                  // 0x010
    enum HeapID heapId;                  // 0x014
    SysTask *task;                       // 0x018
    NARC *gfxNarc;                       // 0x01C
    Pokepic *pokepic;                    // 0x020
    Window windows[9];                   // 0x024
    SpriteList *spriteList;              // 0x0B4
    G2dRenderer renderer;                // 0x0B8
    GF_2DGfxResMan *resourceManagers[4]; // 0x1E0
    PokedexEntryPopupSprite sprites[4];  // 0x1F0
    u16 state;                           // 0x240
    u16 bannerFlashTimer;                // 0x242
    u32 species;                         // 0x244
    u32 form;                            // 0x248
    u32 type1;                           // 0x24C
    u32 type2;                           // 0x250
    u32 done;                            // 0x254
} PokedexEntryPopup;

void PokedexEntryPopup_Task(SysTask *task, PokedexEntryPopup *popup);
void PokedexEntryPopup_ReadMonData(PokedexEntryPopup *popup);
void PokedexEntryPopup_InitBgs(PokedexEntryPopup *popup);
void PokedexEntryPopup_FreeBgs(PokedexEntryPopup *popup);
void PokedexEntryPopup_StartFadeIn(PokedexEntryPopup *popup);
BOOL PokedexEntryPopup_IsFadeDone(PokedexEntryPopup *popup);
void PokedexEntryPopup_FlashBanner(PokedexEntryPopup *popup);
void PokedexEntryPopup_DrawWindows(PokedexEntryPopup *popup);
void PokedexEntryPopup_RemoveWindows(PokedexEntryPopup *popup);
void PokedexEntryPopup_InitObjCharTransfer(PokedexEntryPopup *popup);
void PokedexEntryPopup_DestroyObjCharTransfer(void);
void PokedexEntryPopup_CreateResourceManagers(PokedexEntryPopup *popup);
void PokedexEntryPopup_DestroyResourceManagers(PokedexEntryPopup *popup);
void PokedexEntryPopup_InitSprites(PokedexEntryPopup *popup);
void PokedexEntryPopup_LoadBannerResources(PokedexEntryPopup *popup);
void PokedexEntryPopup_FreeBannerResources(PokedexEntryPopup *popup);
void PokedexEntryPopup_LoadTypeIconResources(PokedexEntryPopup *popup);
void PokedexEntryPopup_UnloadTypeIconResources(PokedexEntryPopup *popup);
void PokedexEntryPopup_LoadFootprintResources(PokedexEntryPopup *popup);
void PokedexEntryPopup_UnloadFootprintResources(PokedexEntryPopup *popup);
void PokedexEntryPopup_FreeSprites(PokedexEntryPopup *popup);
void PokedexEntryPopup_LoadSpriteResources(SpriteResource **resources, GF_2DGfxResMan **managers, enum HeapID heapId, NARC *narc, int charFileId, int plttFileId, int cellFileId, int animFileId, int plttNum, int charId, int plttId, int cellId, int animId);
SpriteResource *PokedexEntryPopup_LoadPaletteResource(GF_2DGfxResMan *manager, enum HeapID heapId, NARC *narc, int fileId, int plttNum, int id);
void PokedexEntryPopup_FreeSpriteResources(SpriteResource **resources, GF_2DGfxResMan **managers);
void PokedexEntryPopup_MakeResourcesHeader(SpriteResource **resources, GF_2DGfxResMan **managers, SpriteResourcesHeader *header, int priority);
void PokedexEntryPopup_CreateTypeIconSprites(PokedexEntryPopup *popup);
void PokedexEntryPopup_DeleteTypeIconSprites(PokedexEntryPopup *popup);
void PokedexEntryPopup_CreateBannerSprite(PokedexEntryPopup *popup);
void PokedexEntryPopup_DeleteBannerSprite(PokedexEntryPopup *popup);
void PokedexEntryPopup_CreateFootprintSprite(PokedexEntryPopup *popup);
void PokedexEntryPopup_DeleteFootprintSprite(PokedexEntryPopup *popup);
void PokedexEntryPopup_CreatePokepic(PokedexEntryPopup *popup);

#endif // POKEHEARTGOLD_APPLICATION_POKEDEX_POKEDEX_ENTRY_POPUP_H
