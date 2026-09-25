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

u32 NNS_GfdFreeFrmTexVram(u32 key) {
    return 0;
}

void NNS_GfdGetFrmTexVramState(u32 *out) {
    int i = 0;
    u8 *region = &_02110950;
    int j = 0;
    do {
        out[j] = *(u32 *)(region + 0);
        out[j + 1] = *(u32 *)(region + 4);
        i++;
        region += 0x18;
        j += 2;
    } while (i < 5);
}

void NNS_GfdSetFrmTexVramState(const u32 *in) {
    int i = 0;
    int j;
    u8 *region = &_02110950;
    j = i;
    do {
        *(u32 *)(region + 0) = in[j];
        *(u32 *)(region + 4) = in[j + 1];
        i++;
        j += 2;
        region += 0x18;
    } while (i < 5);
}
