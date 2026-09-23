#include "global.h"
void GF_AssertFail(void);
int ov07_02222040(int, int, int);
void ov07_02222508(u8 *, int, int, int, int);

void ov07_02222508(u8 *state, int x, int divisor, int y, int scale) {
    if (state == NULL) GF_AssertFail();
    *(int *)(state + 4) = scale;
    int dx = (x << 8) / divisor;
    int dy = (y << 8) / divisor;
    *(int *)(state + 8) = ov07_02222040(dx << 12, dy << 12, scale);
    *(s16 *)state = dx;
    *(s16 *)(state + 2) = dx;
    *(int *)(state + 0x10) = (s16)*(s16 *)state << 12;
    *(int *)(state + 0x14) = (s16)*(s16 *)(state + 2) << 12;
}
