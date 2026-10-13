#ifndef POKEHEARTGOLD_IGT_COUNTER_H
#define POKEHEARTGOLD_IGT_COUNTER_H

#include "igt.h"

typedef struct IGTCounter {
    u32 active;
    IGT *igt;
    u64 countedSeconds;
    u32 unk10;
    u32 unk14;
    u64 startTicks;
} IGTCounter;

void IGTCounter_Init(void);
void IGTCounter_Update(void);
void IGTCounter_Start(IGT *igt);

#endif // POKEHEARTGOLD_IGT_COUNTER_H
