#include "link_lobby_internal.h"

void sub_02058304(void) {
    Comm_GetLocalNetId();
    if (((BOOL (*)(void))Comm_IsNetIdConnected)()) {
        ov03_02255C54();
        CommPlayerInfo_SendLocal();
        sub_020582F4(sub_02058328, 0);
    }
}
