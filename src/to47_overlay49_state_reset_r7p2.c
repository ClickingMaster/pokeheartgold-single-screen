#include "to47_overlay49_state_reset_r7p2_private.h"

void ov49_02258EEC(void *arg0, void *opaque, int index)
{
    Ov49Res7P2State *state=opaque;
    if (index >= 10) {
        GF_AssertFail();
    }
    if (ov49_02269698[state->storage.fields.state] != NULL) {
        ov49_02269698[state->storage.fields.state](state, arg0);
    }
    state->storage.fields.counter = 0;
    state->storage.fields.mode = 0;
    state->storage.fields.state = index;
    memset(state->storage.fields.payload, 0, sizeof(state->storage.fields.payload));
    state->tail.callback = ov49_022696C0[index];
}
