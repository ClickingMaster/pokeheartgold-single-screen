#include "global.h"

void *sub_02015DDC(void *);
void *Heap_Alloc(int, int);

typedef struct {
    void *a;
    void *b;
    int c;
    int d;
    void *e;
    int f;
    int g;
    int h;
} Ov41R13;

void ov41_022467E4(Ov41R13 *out, void *source);

void ov41_022467E4(Ov41R13 *out, void *source)
{
    out->a = sub_02015DDC(source);
    out->b = Heap_Alloc(*(int *)((u8 *)source + 0xc), 0x1d8);
    out->c = 0x76;
    out->d = 0;
    out->e = Heap_Alloc(*(int *)((u8 *)source + 0xc), 0x4c);
    out->f = 0x13;
    out->g = 0;
    out->h = 1;
}
