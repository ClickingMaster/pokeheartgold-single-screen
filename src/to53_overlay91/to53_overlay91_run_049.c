#include "private_w19.h"

void ov91_0225F4F8(u8 *p) {
    extern void GF_3DVramMan_Delete(void *);
    ov91_0225FDC8(p);
    GF_3DVramMan_Delete(*(void **)(p + 0x18));
}
