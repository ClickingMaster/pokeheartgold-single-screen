#include "field_map_object_motion_internal.h"

void ov01_021F8400(void *owner, void *obj, const s8 *s, int i) {
    u32 value = ov01_021FA464(i);
    if (i != s[0] || s[2] != 0) {
        BillboardObject_SetSequence(obj, value);
        BillboardObject_SetSequenceFrame(obj, 0);
    } else if (s[2] != 10) {
        BillboardObject_SetSequence(obj, value);
        BillboardObject_SetSequenceFrame(obj, 0);
    } else if (BillboardObject_GetSequence(obj) != value) {
        BillboardObject_SetSequence(obj, value);
        BillboardObject_SetSequenceFrame(obj, 0);
    }
    if (!ov01_021F9344(owner)) {
        BillboardObject_AdvanceFrame(obj, 2 << 10);
    }
}

void ov01_021F847C(void *owner, void *obj, const s8 *s, int i) {
    u32 value = ov01_021FA464(i);
    if (i != s[0]) {
        BillboardObject_SetSequence(obj, value);
        BillboardObject_SetSequenceFrame(obj, 0);
    } else if (s[2] != 11) {
        BillboardObject_SetSequence(obj, value);
        BillboardObject_SetSequenceFrame(obj, 0);
    } else if (BillboardObject_GetSequence(obj) != value) {
        BillboardObject_SetSequence(obj, value);
        BillboardObject_SetSequenceFrame(obj, 0);
    }
    if (!ov01_021F9344(owner)) {
        BillboardObject_AdvanceFrame(obj, FX32_ONE);
    }
}
