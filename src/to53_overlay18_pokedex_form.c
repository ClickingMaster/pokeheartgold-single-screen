#include "global.h"

#include "bg_window.h"

typedef struct W09Pokedex {
    u8 pad0000[8];
    void *grid;
    Window windows[101];
    void *msgData;
    u8 pad0660[0x18CA - 0x660];
    s8 current;
    u8 pad18CB[0x1900 - 0x18CB];
    int count;
} W09Pokedex;

extern void sub_02019B1C(void *, int, s8 *, s8 *);
extern void sub_020196E8(void *, int, int, int);
extern u32 ov18_021F04C0(void *, int);
extern void ov18_021EE3AC(void *, void *, u32, u32, s32, u32, u32, u32, u32);
void ov18_021F0504(W09Pokedex *p, int direction);

void ov18_021F0504(W09Pokedex *p, int direction) {
    u32 i = 0;
    s8 second;
    s8 first;
    W09Pokedex *work = p;

    do {
        sub_02019B1C(work->grid, i + 0x11, &second, &first);
        if (first == 2 || first == 0x14) {
            break;
        }
        i++;
    } while (i < 6);

    FillWindowPixelBuffer(&work->windows[i + 8], 0);
    if (direction < 0) {
        int next;
        sub_020196E8(work->grid, i + 0x11, 10, 0x14);
        next = work->current + 2;
        if (next < work->count) {
            ov18_021EE3AC(work, work->msgData, i + 8, ov18_021F04C0(work, next), 0x48, 0, 4, 0xF0C00, 2);
        }
    } else {
        int previous;
        sub_020196E8(work->grid, i + 0x11, 10, 2);
        previous = work->current;
        previous -= 2;
        if (previous >= 0) {
            ov18_021EE3AC(work, work->msgData, i + 8, ov18_021F04C0(work, previous), 0x48, 0, 4, 0xF0C00, 2);
        }
    }
    CopyWindowPixelsToVram_TextMode(&work->windows[i + 8]);
}
