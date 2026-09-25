#pragma force_active on

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct UnitHeap {
    u8 pad00[0x20];
    volatile u32 flags;
    void *head;
    u32 blockSize;
} UnitHeap;
typedef struct NNSAllocator {
    void *vtable;
    void *heap;
    u32 arg1;
    u32 arg2;
} NNSAllocator;

void MIi_CpuClear32(u32, void *, u32);
void *NNS_FndAllocFromExpHeapEx(void *, u32, int);
void NNS_FndFreeToExpHeap(void *, void *);
void *NNS_FndAllocFromFrmHeapEx(void *, u32, int);
void *OS_AllocFromHeap(u32, u32, u32);
void OS_FreeToHeap(u32, u32, void *);
extern u32 _02109200;

void *PopMBlock__UnitHeap(void **head);
void *NNS_FndAllocFromUnitHeap(UnitHeap *heap);
void NNS_FndFreeToUnitHeap(UnitHeap *heap, void *block);
void *AllocatorAllocForExpHeap(NNSAllocator *, u32);
void AllocatorFreeForExpHeap(NNSAllocator *, void *);
void *AllocatorAllocForFrmHeap(NNSAllocator *, u32);
void AllocatorFreeForFrmHeap(NNSAllocator *, void *);
void *AllocatorAllocForUnitHeap(NNSAllocator *, u32);
void AllocatorFreeForUnitHeap(NNSAllocator *, void *);
void *AllocatorAllocForSDKHeap(NNSAllocator *, u32);
void AllocatorFreeForSDKHeap(NNSAllocator *, void *);
void *NNS_FndAllocFromAllocator(NNSAllocator *, u32);
void NNS_FndFreeToAllocator(NNSAllocator *, void *);
void NNS_FndInitAllocatorForExpHeap(NNSAllocator *, void *, u32, u32);
u32 AllocTexVram_(void);
u32 FreeTexVram_(void);
u32 AllocPlttVram_(void);
u32 FreePlttVram_(void);
void NNSi_GfdSetTexNrmSearchArray(u32, u32, u32, u32, u32);
u32 NNS_GfdFreeFrmTexVram(u32);
void NNS_GfdGetFrmTexVramState(u32 *);
void NNS_GfdSetFrmTexVramState(const u32 *);
extern u8 _02110950;
typedef struct TexRegion {
    u32 start, end, enabled, mode, kind, offset;
} TexRegion;
extern u8 _02110934;
extern u16 _021D84C0;
extern void *NNS_GfdDefaultFuncAllocTexVram;
extern void *NNS_GfdDefaultFuncFreeTexVram;
void NNS_GfdInitFrmTexVramManager(u32, u32);
void NNS_GfdResetFrmTexVramState(void);
u32 NNS_GfdAllocFrmTexVram(u32, u32);

void *PopMBlock__UnitHeap(void **head) {
    void *block = *head;
    if (block != 0) {
        *head = *(void **)block;
    }
    return block;
}

void *NNS_FndAllocFromUnitHeap(UnitHeap *heap) {
    void *block = PopMBlock__UnitHeap(&heap->head);
    if (block != 0) {
        u32 flags = heap->flags & 0xff;
        u32 blockSize = heap->blockSize;
        if (flags & 1) {
            MIi_CpuClear32(0, block, blockSize);
        }
    }
    return block;
}

void NNS_FndFreeToUnitHeap(UnitHeap *heap, void *block) {
    *(void **)block = heap->head;
    heap->head = block;
}

void *AllocatorAllocForExpHeap(NNSAllocator *allocator, u32 size) {
    return NNS_FndAllocFromExpHeapEx(allocator->heap, size, allocator->arg1);
}
void AllocatorFreeForExpHeap(NNSAllocator *allocator, void *block) {
    NNS_FndFreeToExpHeap(allocator->heap, block);
}
void *AllocatorAllocForFrmHeap(NNSAllocator *allocator, u32 size) {
    return NNS_FndAllocFromFrmHeapEx(allocator->heap, size, allocator->arg1);
}
void AllocatorFreeForFrmHeap(NNSAllocator *allocator, void *block) {
}

void *AllocatorAllocForUnitHeap(NNSAllocator *allocator, u32 size) {
    UnitHeap *heap = allocator->heap;
    if (size > heap->blockSize) {
        return 0;
    }
    return NNS_FndAllocFromUnitHeap(heap);
}
void AllocatorFreeForUnitHeap(NNSAllocator *allocator, void *block) {
    NNS_FndFreeToUnitHeap(allocator->heap, block);
}
void *AllocatorAllocForSDKHeap(NNSAllocator *allocator, u32 size) {
    return OS_AllocFromHeap(allocator->arg1, (u32)allocator->heap, size);
}
void AllocatorFreeForSDKHeap(NNSAllocator *allocator, void *block) {
    OS_FreeToHeap(allocator->arg1, (u32)allocator->heap, block);
}
void *NNS_FndAllocFromAllocator(NNSAllocator *allocator, u32 size) {
    return ((void *(*)(NNSAllocator *, u32))((void **)allocator->vtable)[0])(allocator, size);
}
void NNS_FndFreeToAllocator(NNSAllocator *allocator, void *block) {
    ((void (*)(NNSAllocator *, void *))((void **)allocator->vtable)[1])(allocator, block);
}
void NNS_FndInitAllocatorForExpHeap(NNSAllocator *allocator, void *heap, u32 a, u32 b) {
    allocator->vtable = &_02109200;
    allocator->heap = heap;
    allocator->arg1 = a;
    allocator->arg2 = 0;
}

u32 AllocTexVram_(void) {
    return 0;
}
u32 FreeTexVram_(void) {
    return ~0U;
}
u32 AllocPlttVram_(void) {
    return 0;
}
u32 FreePlttVram_(void) {
    return ~0U;
}

void NNSi_GfdSetTexNrmSearchArray(u32 a, u32 b, u32 c, u32 d, u32 e) {
    u8 *regions = &_02110950;
    ((u8 **)((u8 *)&_02110934 + 8))[0] = regions + a * 0x18;
    ((u8 **)((u8 *)&_02110934 + 8))[1] = regions + b * 0x18;
    ((u8 **)((u8 *)&_02110934 + 8))[2] = regions + c * 0x18;
    ((u8 **)((u8 *)&_02110934 + 8))[3] = regions + d * 0x18;
    ((u8 **)((u8 *)&_02110934 + 8))[4] = regions + e * 0x18;
}

void NNS_GfdInitFrmTexVramManager(u32 mode, u32 install) {
    if (mode <= 2) {
        NNSi_GfdSetTexNrmSearchArray(4, 3, 2, 0, 1);
    } else {
        NNSi_GfdSetTexNrmSearchArray(4, 3, 0, 2, 1);
    }
    _021D84C0 = mode;
    NNS_GfdResetFrmTexVramState();
    if (install != 0) {
        NNS_GfdDefaultFuncAllocTexVram = NNS_GfdAllocFrmTexVram;
        NNS_GfdDefaultFuncFreeTexVram = NNS_GfdFreeFrmTexVram;
    }
}
