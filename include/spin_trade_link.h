#ifndef POKEHEARTGOLD_SPIN_TRADE_LINK_H
#define POKEHEARTGOLD_SPIN_TRADE_LINK_H

#include "global.h"

#include "heap.h"

typedef struct SpinTradeCommonArgs {
    u8 sendData[4];
    u32 frame;
    void *saveData;
    void *communication;
    void *options;
    void *gameStats;
    void *fieldSystem;
} SpinTradeCommonArgs;

typedef struct SpinTradeArgs {
    u32 unk0[4];
    u32 result;
    SpinTradeCommonArgs input;
    void *menu;
    void *tradeApp;
    void *groupApp;
} SpinTradeArgs;

void *SpinTradeLink_New(SpinTradeArgs *work, enum HeapID heapID);
void SpinTradeLink_Close(void *ptr);
void SpinTradeLink_Free(void *ptr);

#endif
