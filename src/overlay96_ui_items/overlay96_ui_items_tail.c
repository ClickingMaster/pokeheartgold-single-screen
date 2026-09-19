#include "overlay96_ui_items_gap63_internal.h"

int ov96_021F8180(Ov96R63Item *item, int direction) {
    int x, y;
    int ret = 0;
    ov96_021F893C(item->handle, &x, &y);
    if (direction == 0) {
        if (x <= 0) {
            ov96_021F8948(item->handle, 0, y);
            ret = 1;
        }
    } else if (direction == 1) {
        if (y >= 0xc0) {
            ret = 1;
        }
    }
    return ret;
}
