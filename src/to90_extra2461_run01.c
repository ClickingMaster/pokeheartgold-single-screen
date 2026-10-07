#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
typedef void (*W17HeapCallback)(void *,u32,void *,int);
// clang-format on
#pragma force_active on
typedef struct {void *prev,*next;u32 size;W17HeapCallback callback;void *a;int b;u8 pad[8];} Block;
typedef struct {void *heap;NNSFndList sections;} Heap;
extern void *NNS_FndGetPrevListObject(NNSFndList *,void *);
extern void NNS_FndRemoveListObject(NNSFndList *,void *);
extern void NNS_FndFreeToFrmHeap(void *,int);
extern BOOL NNS_FndFreeByStateToFrmHeap(void *,u32),NNS_FndRecordStateForFrmHeap(void *,u32);
extern void EraseSync(void);extern u32 NewSection(void *);
u32 NNS_SndHeapClear(void *);
u32 NNS_SndHeapClear(void *memory){Heap *h=memory;NNSFndList *section;BOOL sync=FALSE;Block *block;section=NNS_FndGetPrevListObject(&h->sections,NULL);while(section){block=NNS_FndGetPrevListObject(section,NULL);while(block){if(block->callback){block->callback(block+1,block->size,block->a,block->b);sync=TRUE;}block=NNS_FndGetPrevListObject(section,block);}NNS_FndRemoveListObject(&h->sections,section);section=NNS_FndGetPrevListObject(&h->sections,NULL);}NNS_FndFreeToFrmHeap(h->heap,3);if(sync)EraseSync();return NewSection(h);}
