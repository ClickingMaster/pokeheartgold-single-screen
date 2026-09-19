#include "to53_overlay112_w01_private.h"


void ov112_021E7C6C(void *state) {
    String_Delete(*(void **)((u8 *)state + 0x1e454));
    DestroyMsgData(*(void **)((u8 *)state + 0x1e450));
    DestroyMsgData(*(void **)((u8 *)state + 0x1e44c));
    MessageFormat_Delete(*(void **)((u8 *)state + 0x1e448));
}

void ov112_021E7CA4(void *state, int bgId, int member) {
    GfGfxLoader_LoadScrnData(0xf9, member, *(void **)((u8 *)state + 0x18), bgId, 0, 0, 1, 0x9a);
}
