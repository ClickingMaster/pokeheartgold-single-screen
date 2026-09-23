#include "../include/to57_overlay08_bank14_private.h"
extern void ov08_0221DDCC(void *, int, int, int, int, int);
extern void ov08_0221EAD4(void *, int);
extern void ov08_0221E408(void *, int);
extern void ov08_0221E5DC(void *, int);
extern void ov08_0221E6D8(void *, int);
extern void ov08_0221E7D4(void *, int);
extern void ov08_0221E8D4(void *, int);
extern void ov08_0221E9D4(void *, int);
extern void ov08_0221E120(void *, int, int);
extern void ov08_0221E1A8(void *, int, int);
extern void ov08_0221EC6C(void *, int);
typedef struct Work { u8 pad[0x2070]; Window *windows; u8 count; u8 page; } Work;
#define CLEAR(n) FillWindowPixelBuffer(&w->windows[(n)], 0)

void ov08_0221F900(void *arg) {
    Work *w = arg;
    u32 base = (u32)w->page * 22;
    CLEAR(0x13);
    CLEAR(0x14);
    CLEAR(0xE);
    CLEAR(0xF);
    CLEAR(0x10);
    CLEAR(0x11);
    CLEAR(0x12);
    CLEAR(0xD);
    CLEAR(0x15);
    CLEAR(base);
    CLEAR(base + 10);
    CLEAR(base + 11);
    CLEAR(base + 12);
    CLEAR(base + 5);
    CLEAR(base + 6);
    CLEAR(base + 7);
    CLEAR(base + 8);
    CLEAR(base + 9);
    CLEAR(base + 4);
    CLEAR(base + 1);
    CLEAR(base + 2);
    CLEAR(base + 3);
    ov08_0221DDCC(w, base, 0, *((u8 *)*(void **)w + 0x11), 0, 0);
    ov08_0221EAD4(w, *((u8 *)*(void **)w + 0x11));
    ov08_0221E048(w, base + 10, *((u8 *)*(void **)w + 0x11), 0, 0);
    ov08_0221E408(w, *((u8 *)*(void **)w + 0x11));
    ov08_0221E5DC(w, *((u8 *)*(void **)w + 0x11));
    ov08_0221E6D8(w, *((u8 *)*(void **)w + 0x11));
    ov08_0221E7D4(w, *((u8 *)*(void **)w + 0x11));
    ov08_0221E8D4(w, *((u8 *)*(void **)w + 0x11));
    ov08_0221E9D4(w, *((u8 *)*(void **)w + 0x11));
    ov08_0221E120(w, base + 1, *((u8 *)*(void **)w + 0x11));
    ov08_0221E1A8(w, base + 3, *((u8 *)*(void **)w + 0x11));
    ov08_0221EC6C(w, *((u8 *)*(void **)w + 0x11));
    ov08_0221E3A4(w, 21, 19);
    w->page ^= 1;
}
