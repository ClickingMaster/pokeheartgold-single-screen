#include "to50_overlay80_command_helper_r39_private.h"

void ov80_0222F458(void *c, const u16 *d, u32 narc) {
    void *args;

    args = Frontier_GetLaunchArgs(PTR(PTR(c, 0), 0));
    if (d[0] == 0xFFFF) {
        void *msgData = NewMsgDataFromNarc(1, 0x1B, narc, 0x20);
        ov80_0222E268(PTR(c, 0), msgData, d[1], 1, NULL);
        DestroyMsgData(msgData);
    } else {
        u32 delay = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(PTR(args, 8)));
        ov80_0222E3B8(PTR(c, 0), delay, d[0], d[1], d[2], (s16)d[3], 1);
    }
    FrontierScriptContext_Pause(c, ov80_0222F4D0);
}
