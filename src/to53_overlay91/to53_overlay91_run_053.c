#include "private_w19.h"

void ov91_0225FCD8(u8 *p, u32 heapId) {
    *(void **)(p + 4) = MessageFormat_New(heapId);
    *(void **)(p + 8) = NewMsgDataFromNarc(0, 0x1b, 0xe6, heapId);
    *(void **)(p + 0xc) = String_New(0x80, heapId);
    *(void **)(p + 0x10) = String_New(0x80, heapId);
}


void ov91_0225FD0C(u8 *p) {
    String_Delete(*(void **)(p + 0x10));
    String_Delete(*(void **)(p + 0xc));
    DestroyMsgData(*(void **)(p + 8));
    MessageFormat_Delete(*(void **)(p + 4));
}
