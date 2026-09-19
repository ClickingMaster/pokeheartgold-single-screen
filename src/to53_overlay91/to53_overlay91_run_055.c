#include "private_w19.h"

void ov91_0225FDC8(u8 *p) {
    Camera_Delete(*(void **)(p + 0x190));
}


void ov91_0225FDD8(void) {
    Camera_PushLookAtToNNSGlb();
}
