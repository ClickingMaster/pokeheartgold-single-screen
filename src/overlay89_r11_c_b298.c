#include "overlay89_r11_private.h"

void ov89_0225B298(void *unused, u8 *work) {
    int i;
    u8 *p;

    p = work;
    i = 0;
    p += 0x94;
    do {
        ModelInstance_Draw(p);
        i++;
        p += 0x78;
    } while (i < 3);
    ModelInstance_Draw(work + 0x1C);
}
