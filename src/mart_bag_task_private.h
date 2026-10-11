#ifndef POKEHEARTGOLD_MART_BAG_TASK_PRIVATE_H
#define POKEHEARTGOLD_MART_BAG_TASK_PRIVATE_H
#include "global.h"
#include "task.h"
#include "bag_view.h"
#include "heap.h"
#include "field/fieldmap.h"
#include "overlay_03.h"
#include "mart_bag_task.h"
typedef struct MartReturnEnv {
    u8 pad00[4];
    BagView *bagView;
    u8 pad08[0x26A];
    u8 state;
    u8 pad273[0x2D];
    u16 *result;
} MartReturnEnv;
BOOL MartBag_WaitBagViewExit(FieldSystem *fieldSystem, MartReturnEnv *env);
void MartBag_ReturnToMart(TaskManager *taskManager);
#endif
