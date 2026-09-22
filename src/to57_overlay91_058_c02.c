#include "to57_overlay91_058_private.h"

void ov91_0226014C(u8 *p, u8 *b, int mode) {
    ov91_022601C8(p, b);
    ov91_022601F4(p, b);
    ov91_02260218(p, b);
    switch (mode) {
    case 0:
    case 2:
        ov91_02260298(p, 1);
        ov91_02260254(p, *(u16 *)(b + 4));
        ov91_02260334(p, 1);
        break;
    case 1:
        ov91_02260298(p, 3);
        ov91_02260334(p, 0);
        ov91_0226031C(p, 0x1000);
        break;
    }
}
