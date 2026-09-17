#include "overlay83_party_ui_gap37_internal.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov83_02245C80(u8 *work) {
    int i;
    int count = ov80_02237B58(work[9], 1);
    for (i = 0; i < count; i++) {
        ov83_02245CA8(work, (u8)i);
    }
}
