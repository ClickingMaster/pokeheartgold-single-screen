#include "to52_r4_overlay90_r24_private.h"

typedef struct Ov90B6Work {
    u8 pad00[4];
    void *tilemap;
    Sprite *sprites[15];
    u8 pad44[4];
    u8 state;
    u8 timer;
    u8 current;
    u8 total;
} Ov90B6Work;

BOOL ov90_0225B6C4(Ov90B6Work *work, BgConfig **bg) {
    int i;
    switch (work->state) {
    case 0:
        ov90_02258DB0(work->tilemap, 0x8B);
        LoadRectToBgTilemapRect(*bg, 2, (u8 *)work->tilemap + 0xC, 0, 0, 0x20, 0x14);
        BgTilemapRectChangePalette(*bg, 2, 0, 0, 0x20, 0x14, 0xE);
        ScheduleBgTilemapBufferTransfer(*bg, 2);
        work->state++;
        break;
    case 1:
        if (work->current == work->total) {
            work->state = 3;
            break;
        }
        PlaySE(0x594);
        Sprite_SetDrawFlag(work->sprites[work->current % 15], TRUE);
        work->timer = 8;
        work->state++;
        break;
    case 2:
        work->timer--;
        if (work->timer != 0) {
            break;
        }
        work->current++;
        if (work->current % 15 == 0 && work->current != work->total) {
            for (i = 0; i < 15; i++) {
                Sprite_SetDrawFlag(work->sprites[i], FALSE);
                Sprite_SetAnimCtrlSeq(work->sprites[i], 2);
            }
        }
        work->state = 1;
        break;
    case 3:
        return TRUE;
    }
    return FALSE;
}
