#include "slop_r1_overlay83_m10_private.h"

void ov83_022479E4(void *param0, MsgData *msgData, s32 msgNo, int param3, int param4, int param5, int param6, int param7) {
    String *str = NewString_ReadMsgData(msgData, msgNo);
    ov83_02247998(param0, str, param3, param4, param5, param6, param7);
    String_Delete(str);
}
