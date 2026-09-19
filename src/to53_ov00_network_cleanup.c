#include "to53_ov00_network_cleanup_private.h"
#define U32AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTRAT(p, o) (*(void **)((u8 *)(p) + (o)))
void ov00_021E5AE8(void) {
    if (_0221A680 != NULL) {
        ov00_021E7300();
        if (PTRAT(_0221A680, 0xF98) != NULL) {
            NNS_FndDestroyExpHeap((NNSFndHeapHandle)PTRAT(_0221A680, 0xF9C));
            Heap_Free(PTRAT(_0221A680, 0xF98));
            PTRAT(_0221A680, 0xF98) = NULL;
        }
        NNS_FndDestroyExpHeap((NNSFndHeapHandle)PTRAT(_0221A680, 0xF94));
        Heap_FreeExplicit((enum HeapID)U32AT(_0221A680, 0x1084), PTRAT(_0221A680, 0xF90));
        Heap_FreeExplicit((enum HeapID)U32AT(_0221A680, 0x1084), PTRAT(_0221A680, 0xF7C));
        _0221A680 = NULL;
    }
    ov00_021EC210();
}
