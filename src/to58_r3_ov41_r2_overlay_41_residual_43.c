#include "global.h"

int ov41_02248EF4(void *work, int value);
void ov41_02248F18(void *work, void *a, void *b, void *c, void *d, void *e);
void ov41_02248E28(void *);
void *Heap_Alloc(int, u32);
void *memset(void *, int, u32);
void ov41_02248F80(void);
void ov41_022490F0(void);
void ov41_02249280(void);
void ov41_02248F6C(void);

int ov41_02248EF4(void *work, int value)
{
    int i;
    i = 0;
    do {
        if (value == *(int *)((u8 *)work + 0x190)) {
            return i;
        }
        i++;
        work = (u8 *)work + 4;
    } while (i < 18);
    return i;
}

void ov41_02248F18(void *work, void *a, void *b, void *c, void *d, void *e)
{
    u8 *state;
    ov41_02248E28(work);
    state = Heap_Alloc(0xd, 0x34);
    *(void **)work = state;
    memset(state, 0, 0x34);
    state = *(u8 **)work;
    *(void **)(state + 4) = a;
    *(void **)(state + 8) = b;
    *(void **)(state + 0) = c;
    *(void **)(state + 0xc) = d;
    *(void **)(state + 0x2c) = e;
    *(void (**)(void))((u8 *)work + 4) = ov41_02248F80;
    *(void (**)(void))((u8 *)work + 8) = ov41_022490F0;
    *(void (**)(void))((u8 *)work + 0xc) = ov41_02249280;
    *(void (**)(void))((u8 *)work + 0x10) = ov41_02248F6C;
}
