#ifndef POKEHEARTGOLD_FIELD_HBLANK_SYSTEM_H
#define POKEHEARTGOLD_FIELD_HBLANK_SYSTEM_H

#include "constants/heap.h"

typedef struct HBlankSystem HBlankSystem;
typedef struct HBlankTask HBlankTask;
typedef void (*HBlankTaskCallback)(HBlankTask *task, void *arg);

HBlankSystem *HBlankSystem_New(enum HeapID heapID);
void HBlankSystem_Delete(HBlankSystem *hBlankSystem);
void HBlankSystem_Start(HBlankSystem *hBlankSystem);
void HBlankSystem_Stop(HBlankSystem *hBlankSystem);
HBlankTask *HBlankSystem_StartTask(HBlankSystem *hBlankSystem, HBlankTaskCallback callback, void *arg);
void HBlankTask_Delete(HBlankTask *task);

#endif // POKEHEARTGOLD_FIELD_HBLANK_SYSTEM_H
