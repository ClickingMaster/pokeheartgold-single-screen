#ifndef POKEHEARTGOLD_SPIN_TRADE_LINK_H
#define POKEHEARTGOLD_SPIN_TRADE_LINK_H

#include "global.h"

#include "field_types_def.h"
#include "game_stats.h"
#include "heap.h"
#include "options.h"
#include "save.h"

typedef struct SpinTradeCommonArgs {
    u8 sendData[4];
    u32 frame;
    SaveData *saveData;
    struct UnionRoomObjects *communication;
    Options *options;
    GameStats *gameStats;
    FieldSystem *fieldSystem;
} SpinTradeCommonArgs;

typedef struct SpinTradeArgs {
    u32 unk0[4];
    u32 result;
    SpinTradeCommonArgs input;
    void *menu;
    struct SpinTradeApp *tradeApp;
    struct SpinTradeGroupApp *groupApp;
} SpinTradeArgs;

void *SpinTradeLink_New(SpinTradeArgs *work, enum HeapID heapID);
void SpinTradeLink_Close(void *ptr);
void SpinTradeLink_Free(void *ptr);

#endif
