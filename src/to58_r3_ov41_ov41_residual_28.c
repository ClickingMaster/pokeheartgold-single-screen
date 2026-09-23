#include "global.h"

void ov41_02247FFC(void *work, void *a, void *b, void *c);

void ov41_02249604(void *, void *, void *, void *, void *, int);

void ov41_02247FFC(void *work, void *a, void *b, void *c)
{
    ov41_02249604((u8 *)work + 0x78, *(void **)((u8 *)work + 0x3c), a, b, c, 1);
}
