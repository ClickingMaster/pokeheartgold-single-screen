#include "global.h"
int ov07_02221C8C(int);
int ov07_02221CB4(void);

int ov07_02221CB4(void) {
    int index = 0;
    int count = 0;
    while (1) {
        int value = ov07_02221C8C(index);
        index++;
        if (value == 0xFEFE) break;
        count++;
        if (count >= 0xFF) break;
    }
    if (count >= 0xFF) return 0;
    return index;
}
