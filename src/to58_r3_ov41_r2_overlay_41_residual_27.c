#include "global.h"

void ov41_02247FAC(void *work, void *a, void *b, void *c, void *d, void *e, int f);

void ov41_022495F0(void *, void *, void *, void *, int);
void ov41_02249700(void *, void *);
void ov41_0224971C(void *, void *, void *);

void ov41_02247FAC(void *work, void *a, void *b, void *c, void *d, void *e, int f)
{
    ov41_022495F0((u8 *)work + 0x78, *(void **)((u8 *)work + 0x3c), a, b, f);
    ov41_02249700((u8 *)work + 0x78, e);
    ov41_0224971C((u8 *)work + 0x78, c, d);
}
