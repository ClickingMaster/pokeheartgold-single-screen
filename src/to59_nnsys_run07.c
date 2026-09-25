typedef unsigned char u8;
typedef unsigned int u32;
static inline u8 HeapFlags(void **p) {
    return ((u32 *)p)[-1];
}

typedef struct FrameState {
    u32 tag;
    void *head;
    void *tail;
    struct FrameState *prev;
} FrameState;

typedef struct FrameHeap {
    u8 pad00[0x18];
    void *heapStart;
    void *heapEnd;
    u32 attribute;
    void *head;
    void *tail;
    FrameState *state;
} FrameHeap;

void NNSi_FndInitHeapHead(FrameHeap *, u32, void *, void *, u32);
void NNSi_FndFinalizeHeap(FrameHeap *);
void MIi_CpuClear32(u32, void *, u32);
int abs(int);

FrameHeap *InitFrameHeap(FrameHeap *heap, void *end, u32 attribute);
void *AllocFromHead__FrameHeap(void **head, u32 size, u32 alignment);
void *AllocFromTail__FrameHeap(void **head, u32 size, u32 alignment);
void FreeHead__FrameHeap(FrameHeap *heap);
void FreeTail__FrameHeap(FrameHeap *heap);
FrameHeap *NNS_FndCreateFrmHeapEx(void *start, u32 size, u32 attribute);
void NNS_FndDestroyFrmHeap(FrameHeap *heap);
void *NNS_FndAllocFromFrmHeapEx(FrameHeap *heap, u32 size, int alignment);
void NNS_FndFreeToFrmHeap(FrameHeap *heap, u32 mode);
u32 NNS_FndGetAllocatableSizeForFrmHeapEx(FrameHeap *heap, int alignment);
int NNS_FndRecordStateForFrmHeap(FrameHeap *heap, u32 tag);
int NNS_FndFreeByStateToFrmHeap(FrameHeap *heap, u32 tag);

FrameHeap *InitFrameHeap(FrameHeap *heap, void *end, u32 attribute) {
    NNSi_FndInitHeapHead(heap, 0x46524d48, (u8 *)heap + 0x30, end, attribute);
    heap->head = heap->heapStart;
    heap->tail = heap->heapEnd;
    heap->state = 0;
    return heap;
}

void *AllocFromHead__FrameHeap(void **head, u32 size, u32 alignment) {
    u8 *old = head[0];
    u32 mask = alignment - 1;
    u8 *next = (u8 *)((mask + (u32)old) & ~mask);
    u8 *end = (u8 *)(size + (u32)next);
    if (end > (u8 *)head[1]) {
        return 0;
    }
    u32 actual = end - old;
    if (HeapFlags(head) & 1) {
        MIi_CpuClear32(0, old, actual);
    }
    head[0] = end;
    return next;
}

void *AllocFromTail__FrameHeap(void **head, u32 size, u32 alignment) {
    u8 *old = head[1];
    u32 mask = alignment - 1;
    u8 *next = (u8 *)(((u32)(old - size)) & ~mask);
    if (next < (u8 *)head[0]) {
        return 0;
    }
    u32 actual = old - next;
    if (HeapFlags(head) & 1) {
        MIi_CpuClear32(0, next, actual);
    }
    head[1] = next;
    return next;
}

void FreeHead__FrameHeap(FrameHeap *heap) {
    heap->head = heap->heapStart;
    heap->state = 0;
}

void FreeTail__FrameHeap(FrameHeap *heap) {
    FrameState *state = heap->state;
    while (state != 0) {
        state->tail = heap->heapEnd;
        state = state->prev;
    }
    heap->tail = heap->heapEnd;
}

FrameHeap *NNS_FndCreateFrmHeapEx(void *start, u32 size, u32 attribute) {
    u8 *end = (u8 *)((size + (u32)start) & ~3);
    u8 *begin = (u8 *)(((u32)start + 3) & ~3);
    if (begin > end || (u32)(end - begin) < 0x30) {
        return 0;
    }
    return InitFrameHeap((FrameHeap *)begin, end, attribute);
}

void NNS_FndDestroyFrmHeap(FrameHeap *heap) {
    NNSi_FndFinalizeHeap(heap);
}

void *NNS_FndAllocFromFrmHeapEx(FrameHeap *heap, u32 size, int alignment) {
    if (size == 0) {
        size = 1;
    }
    size = (size + 3) & ~3;
    if (alignment >= 0) {
        return AllocFromHead__FrameHeap(&heap->head, size, alignment);
    }
    return AllocFromTail__FrameHeap(&heap->head, size, -alignment);
}

void NNS_FndFreeToFrmHeap(FrameHeap *heap, u32 mode) {
    if (mode & 1) {
        FreeHead__FrameHeap(heap);
    }
    if (mode & 2) {
        FreeTail__FrameHeap(heap);
    }
}
