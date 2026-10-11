#include "field_map_object_motion_internal.h"

void ov01_021F7CBC(void *unused, void *obj, const s8 *state, int index) {
    if (state[3] != 9) {
        BillboardObject_SetSequence(obj, 4);
        BillboardObject_SetSequenceFrame(obj, 0);
    }
    BillboardObject_AdvanceFrame(obj, FX32_ONE);
}
