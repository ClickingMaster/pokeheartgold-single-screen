#include "to53_overlay40_transition_private.h"

void ov40_0222CCAC(void *w);
void ov40_0222CE7C(void *w);

void ov40_0222CBE0(void *w) {
    s16 x;
    s16 y;
    int i;
    u8 *a;
    u8 *b;
    u8 *c;
    int j;

    for (i = 0, a = (u8 *)w + 0x534, b = w; i < 5; i++, a += 0x28, b += 0x28) {
        ov40_0222D288(PTR(a, 0), 0x32, (5 - SWORD(w, 0x6E0)) * 16 + 0xD9);
        TextOBJ_SetPosition(PTR(b, 0x548), 0x24, 0x24 - 0x2C);
    }

    j = 0;
    c = w;
    do {
        if (SWORD(w, 0x6D8) == 0) {
            break;
        }
        ov40_0222D294(PTR(c, 0x534), &x, &y);
        if (SWORD(w, 0x6D8) - 1 == j) {
            y = 0xA9;
            ManagedSprite_SetPaletteOverrideOffset(PTR(c, 0x534), 1);
        } else {
            x = x - (SWORD(w, 0x6D8) - j) * 4;
            y = (SWORD(w, 0x6D8) - j - 1) * 16 + (0x24 * (5 - (SWORD(w, 0x6D8) - j)) + 25);
            ManagedSprite_SetPaletteOverrideOffset(PTR(c, 0x534), 2);
        }
        ov40_0222D288(PTR(c, 0x534), x, y);
        j++;
        c += 0x28;
    } while (j < SWORD(w, 0x6D8));
}
