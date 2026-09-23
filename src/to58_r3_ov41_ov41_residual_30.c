#include "global.h"

void ov41_02248120(void *work, int x, int y, int z, int w);

void ov41_02249BE8(void *, int, int);

void ov41_02248120(void *work, int x, int y, int z, int w)
{
    int negY = -y;
    int negX = -x;
    ov41_02249BE8((u8 *)work + 4, negX, negY);
    ov41_02249BE8((u8 *)work + 4, z, w);
    ov41_02249BE8((u8 *)work + 0x14, negX, negY);
    ov41_02249BE8((u8 *)work + 0x14, z, w);
}
