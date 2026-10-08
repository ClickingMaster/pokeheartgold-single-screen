#pragma force_active on
#include "global.h"
typedef struct {
    void *unk0;
    void *objs[0x60];
    u8 count;
    u8 col;
    u8 row;
    u8 page;
    u8 done;
    u8 unk189;
    u8 timerReset;
    s8 timer;
    u8 unk18C;
    u8 active;
} ArcadeEffect;
extern const u8 ov80_0223DBEC[][16];
extern void ov80_0223AE14(void *obj, int tile, int tile2, int x, int y, u8 a, void *b, int w, int h, u8 c);
extern BOOL ov80_0223AE6C(void *obj);
BOOL ov80_0223AFC4(ArcadeEffect *e);
// @fn ov80_0223AFC4
BOOL ov80_0223AFC4(ArcadeEffect *e) {
    int i;
    BOOL ret;
    int tile;
    int tile2;
    int y;
    if (e->active == 0) {
        return TRUE;
    }
    if (e->count < 0x60) {
        e->timer--;
        if (e->timer <= 0) {
            e->timer = e->timerReset;
            tile = ov80_0223DBEC[e->page][e->col] * 32;
            tile2 = tile + 32;
            y = e->row * 16 + 8;
            ov80_0223AE14(e->objs[e->count], tile, tile2, y, y, e->unk189, e->unk0, 32, 16, e->unk18C);
            tile = ov80_0223DBEC[e->page ^ 1][e->col] * 32;
            tile2 = tile + 32;
            y = (5 - e->row) * 16 + 8;
            ov80_0223AE14(e->objs[e->count + 1], tile, tile2, y, y, e->unk189, e->unk0, 32, 16, e->unk18C);
            tile = ov80_0223DBEC[e->page][e->col] * 32;
            tile2 = tile + 32;
            y = (e->row + 6) * 16 + 8;
            ov80_0223AE14(e->objs[e->count + 2], tile, tile2, y, y, e->unk189, e->unk0, 32, 16, e->unk18C);
            tile = ov80_0223DBEC[e->page ^ 1][e->col] * 32;
            tile2 = tile + 32;
            y = (11 - e->row) * 16 + 8;
            ov80_0223AE14(e->objs[e->count + 3], tile, tile2, y, y, e->unk189, e->unk0, 32, 16, e->unk18C);
            e->count += 4;
            e->col++;
            if (e->col % 8 == 0) {
                e->page ^= 1;
                e->row++;
                e->col = 0;
            }
        }
    }
    for (i = e->done; i < e->count; i++) {
        ret = ov80_0223AE6C(e->objs[i]);
        if (ret == TRUE) {
            e->done++;
        }
    }
    if (e->done >= 0x60 && ret == TRUE) {
        e->active = 0;
        return TRUE;
    }
    return FALSE;
}
