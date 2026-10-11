#ifndef TO47_OVERLAY15_WINDOWS_R23_PRIVATE_H
#define TO47_OVERLAY15_WINDOWS_R23_PRIVATE_H

#include "global.h"
#include "bg_window.h"
#include "msgdata.h"

typedef struct Ov15WindowConfigR23 {
    u32 x;
    u32 y;
    u32 baseTile;
} Ov15WindowConfigR23;

typedef struct Ov15PositionR23 {
    u32 x;
    u32 y;
} Ov15PositionR23;

typedef struct Ov15BagWorkR23 {
    BgConfig *bgConfig;
    u8 pad_004[0xB0];
    Window windows[24];
    u8 pad_234[0x3C0];
    String *strings[8];
} Ov15BagWorkR23;

extern const Ov15WindowConfigR23 sItemListWindowTemplates[];
extern const Ov15PositionR23 sItemMenuWindowPositions[];
extern const Ov15PositionR23 sQuantityDigitWindowPositions[];

void BagApp_AddItemListWindows(Ov15BagWorkR23 *work);
void BagApp_RemoveItemListWindows(Ov15BagWorkR23 *work);
void BagApp_AddItemActionWindows(Ov15BagWorkR23 *work);
void BagApp_RemoveItemActionWindows(Ov15BagWorkR23 *work);
void ov15_021FE4C8(Ov15BagWorkR23 *work);

#endif
