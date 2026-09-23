#include "global.h"

void ov41_0224B530(void *source, void *arg);
void ov41_0224B554(void *work);
void ov41_0224B57C(void *work);

void ov41_0224B8DC(void *, void *);
void ov41_0224B8F0(void *, void *);
void ov41_0224B630(void *);
void ov41_0224B50C(void *);
void ov41_0224B720(void *);
void ov41_0224B878(void *);
void ov41_0224B85C(void *);
void ov41_02245ECC(void *);
void ov41_022499DC(void *);
void ov41_0224B754(void *);
void Heap_Free(void *);

void ov41_0224B530(void *source, void *arg)
{
    u32 local[27];
    ov41_0224B8DC(local, source);
    ov41_0224B8F0(local, arg);
    ov41_0224B630(local);
}

void ov41_0224B554(void *work)
{
    if (*(u32 *)((u8 *)work + 0x260) == 1) {
        ov41_0224B50C((u8 *)work + 0x218);
    }
    if (*(u32 *)((u8 *)work + 0x264) == 1) {
        ov41_0224B720(work);
    }
}

void ov41_0224B57C(void *work)
{
    if (*(u32 *)((u8 *)work + 0x260) != 0) {
        ov41_0224B878(work);
    }
    ov41_0224B85C(work);
    ov41_02245ECC(*(void **)((u8 *)work + 0x18c));
    *(u32 *)((u8 *)work + 0x18c) = 0;
    ov41_022499DC((u8 *)work + 0x184);
    ov41_0224B754(work);
    Heap_Free(*(void **)((u8 *)work + 0x234));
    Heap_Free(work);
}
