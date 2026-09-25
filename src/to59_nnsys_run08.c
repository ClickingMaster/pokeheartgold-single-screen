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

int NNS_FndRecordStateForFrmHeap(FrameHeap *heap, u32 tag) {
    void *head = heap->head;
    FrameState *state = AllocFromHead__FrameHeap(&heap->head, 0x10, 4);
    if (!state) {
        return 0;
    }
    state->tag = tag;
    state->head = head;
    state->tail = heap->tail;
    state->prev = heap->state;
    heap->state = state;
    return 1;
}

int NNS_FndFreeByStateToFrmHeap(FrameHeap *heap, u32 tag) {
    FrameState *state = heap->state;
    if (tag != 0 && state != 0) {
        do {
            if (state->tag == tag) {
                break;
            }
            state = state->prev;
        } while (state != 0);
    }
    if (state == 0) {
        return 0;
    }
    heap->head = state->head;
    heap->tail = state->tail;
    heap->state = state->prev;
    return 1;
}
