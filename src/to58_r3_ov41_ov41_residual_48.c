#include "global.h"

void ov41_0224971C(void *work, int x, int y);

void ov41_022497A0(void *, int *, int *);
void Pokepic_SetAttr(void *, int, int);
void ov41_02249978(void *, int, int, int, int);

void ov41_0224971C(void *work, int x, int y)
{
    int halfX;
    int halfY;
    ov41_022497A0(work, &halfX, &halfY);
    Pokepic_SetAttr(*(void **)work, 0, x);
    Pokepic_SetAttr(*(void **)work, 1, y);
    halfY /= 2;
    halfX /= 2;
    ov41_02249978((u8 *)work + 4, x, y, halfX, halfY);
}
