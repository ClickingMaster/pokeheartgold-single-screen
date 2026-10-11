#include "global.h"

#include "frontier/frontier_map.h"

#include "bg_window.h"
#include "heap.h"
#include "sys_task_api.h"

// Map effect of some frontier maps: two 2x2 tile blocks of BG3 alternate every third frame.

typedef struct FrontierMapTileBlinkTask {
    SysTask *task;
    BgConfig *bgConfig;
    u16 state;
    u16 timer;
} FrontierMapTileBlinkTask;

struct FrontierMapTileBlink {
    u32 unused;
    FrontierMapTileBlinkTask *task;
};

FrontierMapTileBlinkTask *FrontierMapTileBlink_CreateTask(BgConfig *bgConfig);
void FrontierMapTileBlink_Task(SysTask *task, void *data);
void FrontierMapTileBlink_GetTiles(u16 *tiles, u8 state);
void FrontierMapTileBlink_DestroyTask(FrontierMapTileBlinkTask *blinkTask);

FrontierMapTileBlink *FrontierMapTileBlink_Create(BgConfig *bgConfig) {
    FrontierMapTileBlink *blink;

    blink = Heap_Alloc(HEAP_ID_101, sizeof(FrontierMapTileBlink));
    MI_CpuFill8(blink, 0, sizeof(FrontierMapTileBlink));
    blink->task = FrontierMapTileBlink_CreateTask(bgConfig);
    return blink;
}

FrontierMapTileBlinkTask *FrontierMapTileBlink_CreateTask(BgConfig *bgConfig) {
    FrontierMapTileBlinkTask *blinkTask = Heap_Alloc(HEAP_ID_101, sizeof(FrontierMapTileBlinkTask));

    MI_CpuFill8(blinkTask, 0, sizeof(FrontierMapTileBlinkTask));
    blinkTask->bgConfig = bgConfig;
    blinkTask->state = 0;
    blinkTask->task = SysTask_CreateOnMainQueue(FrontierMapTileBlink_Task, blinkTask, 79500);
    return blinkTask;
}

void FrontierMapTileBlink_Task(SysTask *task, void *data) {
    FrontierMapTileBlinkTask *blinkTask = data;
    u16 tiles[4];

    if (blinkTask->timer < 2) {
        blinkTask->timer++;
        return;
    }
    blinkTask->timer = 0;
    blinkTask->state ^= 1;
    FrontierMapTileBlink_GetTiles(tiles, (u8)blinkTask->state);
    LoadRectToBgTilemapRect(blinkTask->bgConfig, GF_BG_LYR_MAIN_3, tiles, 14, 2, 2, 2);
    ScheduleBgTilemapBufferTransfer(blinkTask->bgConfig, GF_BG_LYR_MAIN_3);
}

void FrontierMapTileBlink_GetTiles(u16 *tiles, u8 state) {
    int base;
    int i;
    int j;
    int rowBase;

    if (state == 0) {
        base = 0xC;
    } else {
        base = 0xE;
    }
    rowBase = 0x60;
    for (i = 0; i < 2u; i++) {
        for (j = 0; j < 2u; j++) {
            tiles[i * 2 + j] = base + j + rowBase;
        }
        rowBase += 0x10;
    }
}

void FrontierMapTileBlink_Free(FrontierMapTileBlink *blink) {
    FrontierMapTileBlink_DestroyTask(blink->task);
    Heap_Free(blink);
}

void FrontierMapTileBlink_DestroyTask(FrontierMapTileBlinkTask *blinkTask) {
    SysTask_Destroy(blinkTask->task);
    Heap_Free(blinkTask);
}

u32 ov80_022385D8(u32 value) {
    switch (value) {
    case 2:
        return 0x73;
    case 3:
        return 0x77;
    case 4:
        return 0x87;
    case 5:
        return 0x7B;
    case 6:
        return 0x8F;
    case 1:
        return 0x71;
    default:
        return value;
    }
}

u32 ov80_02238610(u32 value) {
    switch (value) {
    case 2:
        return 0x66;
    case 3:
        return 0x68;
    case 4:
        return 0x6C;
    case 5:
        return 0x6A;
    case 6:
        return 0x6E;
    case 1:
        return 0x64;
    default:
        return value;
    }
}
