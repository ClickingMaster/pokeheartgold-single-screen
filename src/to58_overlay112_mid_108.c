#include "to58_overlay112_mid_private.h"
typedef struct {u8 a[0x10f0]; u32 limit; u8 b[0x9d7a-0x10f4]; u16 id; u8 c[0x1d758-0x9d7c]; u32 count; u32 pos; u8 d[0x1e440-0x1d760]; void *walker;} Ov112W;

void ov112_021ED5DC(Ov112W *p) {
    sub_02032674(p->walker,&p->count,&p->pos);
    sub_02032650(p->walker,p->count,p->id);
}
