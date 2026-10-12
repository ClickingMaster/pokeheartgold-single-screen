#ifndef POKEHEARTGOLD_FIELD_RESOURCE_BANK_H
#define POKEHEARTGOLD_FIELD_RESOURCE_BANK_H

#include "filesystem.h"
#include "heap.h"

// A private heap holding a fixed number of resource blocks, each looked up by a caller-chosen id.
// The field effect manager keeps its effect resources and billboard sequences in two of these.
typedef struct ResourceBank ResourceBank;

ResourceBank *ResourceBank_Create(enum HeapID parentHeapID, enum HeapID heapID, u32 heapSize, u32 capacity);
void ResourceBank_Destroy(ResourceBank *bank);
void *ResourceBank_Alloc(ResourceBank *bank, u32 id, u32 size, BOOL atEnd);
void ResourceBank_Free(ResourceBank *bank, u32 id);
void *ResourceBank_Get(ResourceBank *bank, u32 id);
BOOL ResourceBank_Contains(ResourceBank *bank, u32 id);
void *ResourceBank_LoadFromNarc(ResourceBank *bank, u32 id, NARC *narc, u32 fileId, BOOL atEnd);

#endif // POKEHEARTGOLD_FIELD_RESOURCE_BANK_H
