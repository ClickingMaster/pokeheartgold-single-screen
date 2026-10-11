#include "communication_player_movement_internal.h"

void sub_0205776C(int a, int b, const u8 *data, void *fieldSystem) {
    int slot = *data;
    if (_021D41C4 != NULL && slot != Comm_GetLocalNetId()) {
        sub_0205724C(slot, 0, 0);
        CommPlayerInfo_ClearPlayer(slot);
    }
}

int sub_02057798(void) {
    return 5;
}
