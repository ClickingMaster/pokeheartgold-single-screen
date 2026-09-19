#include "private_w19.h"

void ov91_0225FA60(u8 *p) {
    FreeBgTilemapBuffer(*(void **)p, 1);
    FreeBgTilemapBuffer(*(void **)p, 2);
    FreeBgTilemapBuffer(*(void **)p, 4);
    FreeBgTilemapBuffer(*(void **)p, 5);
    FreeBgTilemapBuffer(*(void **)p, 6);
    FreeBgTilemapBuffer(*(void **)p, 7);
    Heap_Free(*(void **)p);
}
