#include "overlay74_thumb_w07_private.h"

void ov74_0222F688(void *data) {
    void *p = (u8 *)data + 0x30e8;
    if (U32AT(p, 0xc00) != 0) {
        sub_02014A38((void *)U32AT(p, 0xc00));
    }
    if (U32AT(p, 0xc04) != 0) {
        SysTask_Destroy((void *)U32AT(p, 0xc04));
    }
    U32AT(p, 0xc00) = 0;
    U32AT(p, 0xc04) = 0;
    sub_02014AA0();
}
