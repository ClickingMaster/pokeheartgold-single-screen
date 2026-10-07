#pragma force_active on
#include "global.h"
#include "bag_types_def.h"
typedef struct Work { u8 prefix[0x234]; BagView *view; } Work;
u16 ov15_021F9D60(Work *w, int index, BOOL quantity);
// @fn ov15_021F9D60
u16 ov15_021F9D60(Work *w, int index, BOOL quantity) {
    BagView *v = w->view;
    BagViewPocket *p = v->pockets;
    p += v->unk64;
    if (quantity == 0) {
        return p->slots[index].id;
    }
    return p->slots[index].quantity;
}
