#include "overlay96_ui_items_gap63_internal.h"

void ov96_021F8128(Ov96R63Item *item, int unused1, int unused2) {
    if (item->active != 0) {
        if (item->delay == 0) {
            ov96_021F895C(item->handle);
            if (item->played == 0) {
                ov96_021F8978(item->handle);
                PlaySE(0x8d7);
                item->played = 1;
            }
        } else {
            item->delay--;
        }
    }
}
