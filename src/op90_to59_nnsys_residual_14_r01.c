#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u32 freeList;
    u32 blockList;
    u32 szByte;
    void *pWork;
    u32 szWork;
} PlttMgr;

PlttMgr _021D8500;

extern u32 NNSi_GfdInitLnkVramBlockPool(void *, u32);
extern void NNSi_GfdInitLnkVramMan(void *);
extern void NNSi_GfdAddNewFreeBlock(void *, void *, u32, u32);
extern void NNSi_GfdMergeAllFreeBlocks(void *, void *);
void NNS_GfdResetLnkPlttVramState(void);

// @fn NNS_GfdResetLnkPlttVramState
void NNS_GfdResetLnkPlttVramState(void) {
    _021D8500.blockList = NNSi_GfdInitLnkVramBlockPool(_021D8500.pWork, _021D8500.szWork / 16);
    NNSi_GfdInitLnkVramMan(&_021D8500.freeList);
    NNSi_GfdAddNewFreeBlock(&_021D8500.freeList, &_021D8500.blockList, 0, _021D8500.szByte);
    NNSi_GfdMergeAllFreeBlocks(&_021D8500.freeList, &_021D8500.blockList);
}
