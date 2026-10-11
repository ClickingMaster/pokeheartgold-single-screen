#ifndef TO46_OVERLAY15_GAP11_PRIVATE_H
#define TO46_OVERLAY15_GAP11_PRIVATE_H

#include "global.h"

#include "constants/heap.h"

#include "gf_gfx_loader.h"
#include "item.h"
#include "move.h"
#include "sprite_system.h"
#include "type_icon_resources.h"

typedef struct BagWorkRaw {
    u8 raw[0x900];
} BagWorkRaw;

extern int TypeIcon_GetPalette(u32);
extern int MoveCategoryIcon_GetCharFile(u32);
extern u32 MoveCategoryIcon_GetPalette(u32);
extern NarcId MoveCategoryIcon_GetNarc(void);
extern const SpriteResourceCountsListUnion sBagSpriteResourceCounts;
extern const OamManagerParam sBagOamManagerParam;
extern const OamCharTransferParam sBagOamCharTransferParam;
extern const ManagedSpriteTemplate sBagSpriteTemplates[];
extern void BagApp_ShowPocketTabSprites(BagWorkRaw *, BOOL);
void BagApp_ShowTMHMTypeIcons(BagWorkRaw *, u16, BOOL);
void BagApp_CreateSpriteSystem(BagWorkRaw *);
void BagApp_LoadSpriteResources(BagWorkRaw *);
void BagApp_CreateSprites(BagWorkRaw *);

#endif
