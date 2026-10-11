#ifndef POKEHEARTGOLD_APPLICATION_DRESS_UP_ACCESSORY_PORTRAIT_H
#define POKEHEARTGOLD_APPLICATION_DRESS_UP_ACCESSORY_PORTRAIT_H

#include "global.h"

#include "bg_window.h"
#include "fashion_case.h"
#include "heap.h"

// Renderer of a saved dress-up design (overlay 41): used by the Accessory Portrait app and by the VS Recorder's
// dress-up photo screens. The overlay must stay loaded while a portrait exists.

typedef struct AccessoryPortrait AccessoryPortrait;

typedef struct AccessoryPortraitConfig {
    BgConfig *bgConfig; // its BG2 shows the backdrop
    int x;
    int y;
    enum HeapID heapID;
} AccessoryPortraitConfig;

AccessoryPortrait *AccessoryPortrait_Create(const AccessoryPortraitConfig *config, SaveFashionDataSub *fashionData);
void AccessoryPortrait_Draw(AccessoryPortrait *portrait);
void AccessoryPortrait_Destroy(AccessoryPortrait *portrait);
void AccessoryPortrait_VBlank(AccessoryPortrait *portrait);
void AccessoryPortrait_SetMonVisible(AccessoryPortrait *portrait, BOOL visible);

#endif // POKEHEARTGOLD_APPLICATION_DRESS_UP_ACCESSORY_PORTRAIT_H
