#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

struct NNSSndHeap {
    void *heap;
    NNSFndList sections;
};

typedef struct Block {
    NNSFndLink link;
    u32 size;
    void (*callback)(void *, u32, void *, int);
    void *arg;
    int tag;
    u8 reserved[8];
} Block;

typedef struct Section {
    NNSFndList blocks;
    NNSFndLink sectionLink;
} Section;

extern void *NNS_FndGetPrevListObject(NNSFndList *, void *);
extern void NNS_FndRemoveListObject(NNSFndList *, void *);
extern int NNS_FndFreeByStateToFrmHeap(void *, u32);
extern BOOL NNS_FndRecordStateForFrmHeap(void *, u32);
extern void EraseSync(void);
extern u32 NewSection(void *);
extern u32 NNS_SndHeapClear(void *);

int NNS_SndHeapLoadState(NNSSndHeapHandle heap, int level) {
    Section *section;
    BOOL sync;
    Block *block;
    block = NULL;
    sync = FALSE;
    if (level == 0) {
        return (int)NNS_SndHeapClear(heap);
    }

    while (level < heap->sections.numObjects) {
        section = NNS_FndGetPrevListObject(&heap->sections, NULL);
        block = NNS_FndGetPrevListObject(&section->blocks, block);
        while (block) {
            if (block->callback) {
                block->callback(block + 1, block->size, block->arg, block->tag);
                sync = TRUE;
            }
            block = NNS_FndGetPrevListObject(&section->blocks, block);
        }
        NNS_FndRemoveListObject(&heap->sections, section);
    }

    NNS_FndFreeByStateToFrmHeap(heap->heap, level);
    if (sync) {
        EraseSync();
    }
    NNS_FndRecordStateForFrmHeap(heap->heap, heap->sections.numObjects);
    return (int)NewSection(heap);
}
