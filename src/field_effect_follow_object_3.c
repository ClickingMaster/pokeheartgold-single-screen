#include "field_effect_follow_object_internal.h"

BOOL ov01_021FF418(void *a0, FieldEffectSimpleWork *work) {
    return BillboardObject_Delete(work->sprite);
}

void ov01_021FF424(void *a0, FieldEffectSimpleWork *work) {
    switch (work->state) {
    case 0:
        if (BillboardObject_AdvanceFrame(work->sprite, 1 << 12) == 1) {
            work->unk4 = TRUE;
            work->state++;
        }
        break;
    case 1:
        break;
    }
}

void ov01_021FF44C(void *a0, FieldEffectSimpleWork *work) {
    VecFx32 pos;

    sub_02068DB8(a0, &pos);
    BillboardObject_SetPosition(work->sprite, &pos);
}
