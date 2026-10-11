#pragma force_active on
#include "global.h"
#include "bag_types_def.h"
extern u8 sListPocketCapacities[];
typedef struct S { ItemSlot *slots; } S;
int BagApp_GetNthItemSlotIndex(S *s, int pocket, int n);
// @fn BagApp_GetNthItemSlotIndex
int BagApp_GetNthItemSlotIndex(S *s, int pocket, int n) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < sListPocketCapacities[pocket]; i++) {
        if (s->slots[i].id != 0 && s->slots[i].quantity != 0) {
            count++;
            if (count == n + 1) {
                break;
            }
        }
    }
    return i;
}
