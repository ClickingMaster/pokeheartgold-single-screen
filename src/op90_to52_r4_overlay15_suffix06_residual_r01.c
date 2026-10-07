#pragma force_active on
#include "global.h"
#include "bag_types_def.h"
extern u8 ov15_022008C8[];
typedef struct S { ItemSlot *slots; } S;
int ov15_021FF320(S *s, int pocket, int n);
// @fn ov15_021FF320
int ov15_021FF320(S *s, int pocket, int n) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < ov15_022008C8[pocket]; i++) {
        if (s->slots[i].id != 0 && s->slots[i].quantity != 0) {
            count++;
            if (count == n + 1) {
                break;
            }
        }
    }
    return i;
}
