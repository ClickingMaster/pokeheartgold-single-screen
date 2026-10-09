#pragma force_active on
#include "global.h"

void ov41_02248B48(int index, int *x, int *y);

// @fn ov41_02248B48
void ov41_02248B48(int index, int *x, int *y) {
    int pos;
    int row;
    int col;

    pos = index % 9;
    row = pos / 3;
    col = pos % 3;
    *y = (row + 1) * 8 + row * 32;
    *y += 16;
    *x = (col + 1) * 8 + col * 24;
    *x += 8;
}
