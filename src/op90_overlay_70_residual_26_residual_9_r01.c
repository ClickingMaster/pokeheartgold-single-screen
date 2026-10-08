#pragma force_active on
#include "global.h"
#include "list_menu_items.h"
#include "msgdata.h"
extern u16 ov70_02245B5C[];
int ov70_0223F8D0(int kind, int *start);
int ov70_0223F904(ListMenuItem **items, MsgData *msgData, int kind);

// @fn ov70_0223F904
#pragma opt_rotateloops off
#pragma opt_common_subs off
int ov70_0223F904(ListMenuItem **items, MsgData *msgData, int kind) {
    int start;
    int count = ov70_0223F8D0(kind, &start);
    int i;
    *items = ListMenuItems_New(count, HEAP_ID_61);
    for (i = 0; i < count; i++) {
        ListMenuItems_AppendFromMsgData(*items, msgData, ov70_02245B5C[start + i], start + i + 1);
    }
    return count;
}
#pragma opt_rotateloops reset
#pragma opt_common_subs reset
