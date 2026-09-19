#include "private_w10.h"

void ov13_02222BE4(void *dst, const void *src, u32 value) {
    *(u32 *)dst = value;
    ov13_02222B34(src, (u8 *)dst + 4);
}
BOOL ov13_02222BF4(void *message) {
    return OS_SendMessage((OSMessageQueue *)ov13_0224DDA0, message, OS_MESSAGE_NOBLOCK);
}
BOOL ov13_02222C08(void *message) {
    return OS_SendMessage((OSMessageQueue *)ov13_0224DDA0, message, OS_MESSAGE_NOBLOCK);
}
