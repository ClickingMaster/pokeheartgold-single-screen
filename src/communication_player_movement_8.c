#include "communication_player_movement_internal.h"

int sub_02057A08(void) {
    return 4;
}

BOOL sub_02057A0C(void) {
    int slot;
    if (_021D41C4 == NULL) {
        return FALSE;
    }
    slot = Comm_GetLocalNetId();
    return Comm_IsNetIdConnected(slot) != 0;
}
