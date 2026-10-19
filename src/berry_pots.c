
#include "constants/items.h"

#include "bag_cursor.h"
#include "bag_view.h"
#include "berry_pots_app.h"
#include "overlay_15.h"
#include "berry_pots.h"
#include "poke_overlay.h"

enum BerryPotsMainState {
    BERRY_POTS_MAIN_START_APP,
    BERRY_POTS_MAIN_RUN_APP,
    BERRY_POTS_MAIN_START_BAG,
    BERRY_POTS_MAIN_RUN_BAG,
    BERRY_POTS_MAIN_EXIT,
};

FS_EXTERN_OVERLAY(OVY_15);
FS_EXTERN_OVERLAY(OVY_17);

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

static const u8 *sBerryPotsBagPockets;

typedef struct BerryPotsOverlayData {
    enum HeapID heapID;
    BerryPotsArgs *args;
    OverlayManager *ovyManager;
    u8 padding_0C[0x10 - 0xC];
    BagView *bagView;
    u8 itemAction;
    u16 itemId;
    BagCursor *cursor1;
    BagCursor *cursor2;
} BerryPotsOverlayData; // size: 0x20

static BOOL BerryPots_RunSubApp(OverlayManager **manager);
static void BerryPots_AllocBagCursors(BerryPotsOverlayData *data);
static void BerryPots_FreeBagCursors(BerryPotsOverlayData *data);
static u32 BerryPots_MainSeq_StartApp(BerryPotsOverlayData *data);
static u32 BerryPots_MainSeq_RunApp(BerryPotsOverlayData *data);
static u32 BerryPots_MainSeq_StartBag(BerryPotsOverlayData *data);
static u32 BerryPots_MainSeq_RunBag(BerryPotsOverlayData *data);

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

    BerryPotsOverlayData *data = OverlayManager_CreateAndGetData(manager, sizeof(BerryPotsOverlayData), HEAP_ID_BERRY_POTS);
    MI_CpuFill8(data, 0, sizeof(BerryPotsOverlayData));
    data->heapID = HEAP_ID_BERRY_POTS;
    data->args = OverlayManager_GetArgs(manager);
    BerryPots_AllocBagCursors(data);

    return TRUE;
}

BOOL BerryPots_Main(OverlayManager *manager, int *state) {
    BerryPotsOverlayData *data = OverlayManager_GetData(manager);

    switch (*state) {
    case BERRY_POTS_MAIN_START_APP:
        *state = BerryPots_MainSeq_StartApp(data);
        break;
    case BERRY_POTS_MAIN_RUN_APP:
        *state = BerryPots_MainSeq_RunApp(data);
        break;
    case BERRY_POTS_MAIN_START_BAG:
        *state = BerryPots_MainSeq_StartBag(data);
        break;
    case BERRY_POTS_MAIN_RUN_BAG:
        *state = BerryPots_MainSeq_RunBag(data);
        break;
    case BERRY_POTS_MAIN_EXIT:
        return TRUE;
    }

    return FALSE;
}

BOOL BerryPots_Exit(OverlayManager *manager, int *state) {
    BerryPotsOverlayData *data = OverlayManager_GetData(manager);
    BerryPotsApp_ResetDisplay();

    BerryPots_FreeBagCursors(data);

    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_BERRY_POTS);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_17));

    return TRUE;
}

static void BerryPots_AllocBagCursors(BerryPotsOverlayData *data) {
    data->cursor1 = BagCursor_New(data->heapID);
    data->cursor2 = BagCursor_New(data->heapID);
}

static void BerryPots_FreeBagCursors(BerryPotsOverlayData *data) {
    Heap_Free(data->cursor2);
    Heap_Free(data->cursor1);
}

static u32 BerryPots_MainSeq_StartApp(BerryPotsOverlayData *data) {
    data->ovyManager = OverlayManager_New(&sBerryPotsAppTemplate, data, data->heapID);
    return BERRY_POTS_MAIN_RUN_APP;
}

static u32 BerryPots_MainSeq_RunApp(BerryPotsOverlayData *data) {
    if (!BerryPots_RunSubApp(&data->ovyManager)) {
        return BERRY_POTS_MAIN_RUN_APP;
    }

    if ((u8)(data->itemAction + 0xFF) <= 1) {
        return BERRY_POTS_MAIN_START_BAG;
    }

    return BERRY_POTS_MAIN_EXIT;
}

static u32 BerryPots_MainSeq_StartBag(BerryPotsOverlayData *data) {
    Bag *bag = Save_Bag_Get(data->args->saveData);

    switch (data->itemAction) {
    case 1:
        sBerryPotsBagPockets = (u8 *)sBerryPotsItemPockets;

        data->bagView = Bag_CreateView(bag, sBerryPotsItemPockets, data->heapID);
        BagView_Init(data->bagView, data->args->saveData, 6, data->cursor2, data->args->menuInputStatePtr);
        break;
    case 2:
        sBerryPotsBagPockets = (u8 *)sBerryPotsBerryPockets;

        data->bagView = Bag_CreateView(bag, sBerryPotsBerryPockets, data->heapID);
        BagView_Init(data->bagView, data->args->saveData, 6, data->cursor1, data->args->menuInputStatePtr);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    data->ovyManager = OverlayManager_New(&sBerryPotsBagTemplate, data->bagView, data->heapID);
    return BERRY_POTS_MAIN_RUN_BAG;
}

static u32 BerryPots_MainSeq_RunBag(BerryPotsOverlayData *data) {
    if (!BerryPots_RunSubApp(&data->ovyManager)) {
        return BERRY_POTS_MAIN_RUN_BAG;
    }

    data->itemId = ((u8)BagView_GetSelectedAction(data->bagView) == 4) ? BagView_GetItemId(data->bagView) : 0xFFFF;
    Heap_Free(data->bagView);
    data->bagView = NULL;

    return BERRY_POTS_MAIN_START_APP;
}
