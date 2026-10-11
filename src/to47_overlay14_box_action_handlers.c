#include "global.h"

#include "to47_overlay14_box_action_handlers_private.h"

void ov14_021F2818(void *p) {
    PCBox_DrawReturnButton(p, 0);
    PCBox_SlideOutActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2F0));
    ov14_021E8328(PTR_AT(PTR_AT(p, 0x34), 0x2F0));
    ov14_021E884C(PTR_AT(p, 0x34));
    PCBox_SetSelectedIconOutlineVisible(p, 0);
    PCBox_WaitForUpdateFunc(p, ov14_021EA130, 0x70);
}

void ov14_021F2858(void *p) {
    PCBox_ClearMonInfoAndSelection(p);
    PCBox_SetIconsTranslucent(p, 0x82, 1);
    ov14_021F0AD8(p);
}

void ov14_021F2874(void *p) {
    PCBox_ClearMonInfoAndSelection(p);
    PCBox_SetIconsTranslucent(p, 0x82, 1);
    ov14_021F0C88(p);
}
