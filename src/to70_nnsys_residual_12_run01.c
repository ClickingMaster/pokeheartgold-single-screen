#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Block { u32 address, size; struct Block *prev, *next; } Block;
int TryToMergeBlockRegion_(Block **, Block **, u32 *);
int NNSi_GfdAllocLnkVramAligned(Block **, Block **, u32 *, u32, u32);
void NNSi_GfdMergeAllFreeBlocks(Block **, Block **);
int NNSi_GfdFreeLnkVram(Block **, Block **, u32, u32);
void NNSi_GfdMergeAllFreeBlocks(Block **freeList, Block **pool) {
 Block *it = *freeList; u32 region[2];
 if (!it) return;
 while (it) {
  region[0] = it->address; region[1] = it->address + it->size;
  if (!TryToMergeBlockRegion_(freeList, pool, region)) it = it->next;
  else { it->address = region[0]; it->size = region[1] - region[0]; it = *freeList; }
 }
}
int NNSi_GfdFreeLnkVram(Block **freeList, Block **pool, u32 address, u32 size) {
 u32 region[2]; Block *extra;
 region[0] = address; region[1] = address + size;
 TryToMergeBlockRegion_(freeList, pool, region);
 extra = *pool;
 if (extra) *pool = extra->next;
 if (!extra) return 0;
 extra->address = region[0]; extra->size = region[1] - region[0]; extra->prev = 0; extra->next = 0;
 if (*freeList) (*freeList)->prev = extra;
 extra->next = *freeList; extra->prev = 0; *freeList = extra;
 return 1;
}
