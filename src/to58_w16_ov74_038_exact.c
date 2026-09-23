#include "global.h"

extern u8 ov74_0223D48C[];
extern u8 ov74_0223D654[];
extern void G2dRenderer_SetSubSurfaceCoords(void *, int, int);
extern void ov74_022358C8(int);

void ov74_0223589C(int x, int y);
void ov74_022358BC(void);

void ov74_0223589C(int x, int y) {
    G2dRenderer_SetSubSurfaceCoords(ov74_0223D48C, x, y);
    *(int *)(ov74_0223D654 + 0x10) = y;
}

void ov74_022358BC(void) {
    ov74_022358C8(0);
}
