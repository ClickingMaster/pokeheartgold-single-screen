
#include "constants/items.h"

#include "bag_cursor.h"
#include "bag_view.h"
#include "berry_pots_app.h"
#include "overlay_15.h"
#include "berry_pots.h"
#include "poke_overlay.h"

FS_EXTERN_OVERLAY(OVY_15);
FS_EXTERN_OVERLAY(OVY_17);

extern BOOL BerryPotsApp_Initialize(OverlayManager *, int *);
extern BOOL BerryPotsApp_Run(OverlayManager *, int *);
extern BOOL BerryPotsApp_Exit(OverlayManager *, int *);

static const u8 sBerryPotsBerryPockets[] = {
    POCKET_BERRIES, 0xFF
};

static const u8 sBerryPotsItemPockets[] = {
    POCKET_ITEMS, 0xFF
};

static const OverlayManagerTemplate sBerryPotsAppTemplate = {
    .init = BerryPotsApp_Initialize,
    .exec = BerryPotsApp_Run,
    .exit = BerryPotsApp_Exit,
    .ovy_id = FS_OVERLAY_ID_NONE,
};

static const OverlayManagerTemplate sBerryPotsBagTemplate = {
    .init = Bag_Init,
    .exec = Bag_Main,
    .exit = Bag_Exit,
    .ovy_id = FS_OVERLAY_ID(OVY_15),
};

static const u8 *unused_02201BA0;

typedef struct BerryPotsOverlayData {
    enum HeapID heapID;
    BerryPotsArgs *args;
    OverlayManager *ovyManager;
    u8 unkC[0x10 - 0xC];
    BagView *bagView;
    u8 itemAction;
    u16 itemId;
    BagCursor *cursor1;
    BagCursor *cursor2;
} BerryPotsOverlayData; // size: 0x20

static BOOL BerryPots_RunSubApp(OverlayManager **manager);
static void BerryPots_AllocBagCursors(BerryPotsOverlayData *unk);
static void BerryPots_FreeBagCursors(BerryPotsOverlayData *unk);
static u32 BerryPots_MainSeq_StartApp(BerryPotsOverlayData *unk);
static u32 BerryPots_MainSeq_RunApp(BerryPotsOverlayData *unk);
static u32 BerryPots_MainSeq_StartBag(BerryPotsOverlayData *unk);
static u32 BerryPots_MainSeq_RunBag(BerryPotsOverlayData *unk);

static BOOL BerryPots_RunSubApp(OverlayManager **manager) {
    if (*manager != NULL && OverlayManager_Run(*manager)) {
        OverlayManager_Delete(*manager);
        *manager = NULL;
        return TRUE;
    }

    return FALSE;
}

BOOL BerryPots_Init(OverlayManager *manager, int *state) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_17), OVY_LOAD_ASYNC);
    BerryPotsApp_ResetDisplay();

    Heap_Create(HEAP_ID_3, HEAP_ID_BERRY_POTS, 0x20000);

    BerryPotsOverlayData *unk = OverlayManager_CreateAndGetData(manager, sizeof(BerryPotsOverlayData), HEAP_ID_BERRY_POTS);
    MI_CpuFill8(unk, 0, sizeof(BerryPotsOverlayData));
    unk->heapID = HEAP_ID_BERRY_POTS;
    unk->args = OverlayManager_GetArgs(manager);
    BerryPots_AllocBagCursors(unk);

    return TRUE;
}

BOOL BerryPots_Main(OverlayManager *manager, int *state) {
    BerryPotsOverlayData *unk = OverlayManager_GetData(manager);

    switch (*state) {
    case 0:
        *state = BerryPots_MainSeq_StartApp(unk);
        break;
    case 1:
        *state = BerryPots_MainSeq_RunApp(unk);
        break;
    case 2:
        *state = BerryPots_MainSeq_StartBag(unk);
        break;
    case 3:
        *state = BerryPots_MainSeq_RunBag(unk);
        break;
    case 4:
        return TRUE;
    }

    return FALSE;
}

BOOL BerryPots_Exit(OverlayManager *manager, int *state) {
    BerryPotsOverlayData *unk = OverlayManager_GetData(manager);
    BerryPotsApp_ResetDisplay();

    BerryPots_FreeBagCursors(unk);

    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_BERRY_POTS);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_17));

    return TRUE;
}

static void BerryPots_AllocBagCursors(BerryPotsOverlayData *unk) {
    unk->cursor1 = BagCursor_New(unk->heapID);
    unk->cursor2 = BagCursor_New(unk->heapID);
}

static void BerryPots_FreeBagCursors(BerryPotsOverlayData *unk) {
    Heap_Free(unk->cursor2);
    Heap_Free(unk->cursor1);
}

static u32 BerryPots_MainSeq_StartApp(BerryPotsOverlayData *unk) {
    unk->ovyManager = OverlayManager_New(&sBerryPotsAppTemplate, unk, unk->heapID);
    return 1;
}

static u32 BerryPots_MainSeq_RunApp(BerryPotsOverlayData *unk) {
    if (!BerryPots_RunSubApp(&unk->ovyManager)) {
        return 1;
    }

    if ((u8)(unk->itemAction + 0xFF) <= 1) {
        return 2;
    }

    return 4;
}

static u32 BerryPots_MainSeq_StartBag(BerryPotsOverlayData *unk) {
    Bag *bag = Save_Bag_Get(unk->args->saveData);

    switch (unk->itemAction) {
    case 1:
        unused_02201BA0 = (u8 *)sBerryPotsItemPockets;

        unk->bagView = Bag_CreateView(bag, sBerryPotsItemPockets, unk->heapID);
        BagView_Init(unk->bagView, unk->args->saveData, 6, unk->cursor2, unk->args->menuInputStatePtr);
        break;
    case 2:
        unused_02201BA0 = (u8 *)sBerryPotsBerryPockets;

        unk->bagView = Bag_CreateView(bag, sBerryPotsBerryPockets, unk->heapID);
        BagView_Init(unk->bagView, unk->args->saveData, 6, unk->cursor1, unk->args->menuInputStatePtr);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    unk->ovyManager = OverlayManager_New(&sBerryPotsBagTemplate, unk->bagView, unk->heapID);
    return 3;
}

static u32 BerryPots_MainSeq_RunBag(BerryPotsOverlayData *unk) {
    if (!BerryPots_RunSubApp(&unk->ovyManager)) {
        return 3;
    }

    unk->itemId = ((u8)BagView_GetSelectedAction(unk->bagView) == 4) ? BagView_GetItemId(unk->bagView) : 0xFFFF;
    Heap_Free(unk->bagView);
    unk->bagView = NULL;

    return 0;
}
