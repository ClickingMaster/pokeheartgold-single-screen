#include "to50_comm_player_position_flush_internal.h"

void sub_0205712C(void *unused) {
    int slot;

    for (slot = 0; slot < 8; slot++) {
        CommPlayerState *state = _021D41C4;

        if (state->raw[0x24 + slot] != 0) {
            u8 dirty = state->raw[0xCC + slot];
            u8 *positions = &state->raw[0x34];

            if (dirty != 0 || state->raw[0xEC] != 0) {
                state->raw[0xCC + slot] = 0;
                sub_0205704C(slot, positions + slot * 8);
            }
        }
    }
    _021D41C4->raw[0xEC] = 0;
}
