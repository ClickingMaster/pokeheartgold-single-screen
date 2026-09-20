#ifndef TO56_R3_W02_PRIVATE_H
#define TO56_R3_W02_PRIVATE_H
#include "global.h"

#include "heap.h"
void *BattleSystem_GetBag(void *);
void *BattleSystem_GetBagCursor(void *);
BOOL Bag_TakeItem(void *, u16, u16, enum HeapID);
void BagCursor_Battle_SetLastUsedItem(void *, u16, u8);
void ov08_0221DBCC(void *, u16, u8, enum HeapID);
#endif
#include "bg_window.h"
typedef struct Ov08Bank12Owner {
    u8 pad00[0xC];
    enum HeapID heapId;
} Ov08Bank12Owner;
typedef struct Ov08Bank12Work {
    Ov08Bank12Owner *owner;
    u8 pad0004[0x1E0];
    BgConfig *bg;
    u8 pad01E8[0x1E68];
    Window windows[2];
    Window *dynamicWindows;
    u8 dynamicCount;
    u8 pad2075[5];
    u8 windowType;
} Ov08Bank12Work;
extern const WindowTemplate ov08_02224FD0[];
void ov08_0221DC00(Ov08Bank12Work *);
void ov08_0221DC3C(Ov08Bank12Work *, u8);

extern const WindowTemplate ov08_02225084[];
extern const WindowTemplate ov08_0222500C[];
extern const WindowTemplate ov08_0222522C[];
extern const WindowTemplate ov08_022250EC[];
extern const WindowTemplate ov08_022251A4[];
extern const WindowTemplate ov08_0222502C[];
extern const WindowTemplate ov08_02225054[];
extern const WindowTemplate ov08_02225144[];
extern const WindowTemplate ov08_022250B4[];
