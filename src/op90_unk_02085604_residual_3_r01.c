#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "system.h"
typedef struct Entry {
    int index;
    int b, c;
    ManagedSprite *sprite;
    void *bounds;
    s16 vx, vy;
    u8 movement, delay;
    u8 pad[2];
} Entry;
typedef struct State {
    Entry entries[16];
    u8 pad1c0[0x214 - 0x1c0];
    int cursor;
    u8 pad218[0x230 - 0x218];
    int mode;
    u8 pad234[0x244 - 0x234];
    s16 col;
    s16 row;
    u8 pad248[0x258 - 0x248];
    ManagedSprite *sprite258;
    u8 pad25c[0x2c0 - 0x25c];
    int flagA;
    u8 pad2c4[0x2d0 - 0x2c4];
    int count;
    int x2d4;
    u8 pad2d8[0x374 - 0x2d8];
    int flagB;
    u8 pad378[0x3ac - 0x378];
    int x3ac;
    int x3b0;
    int x3b4;
    u8 pad3b8[0x3f0 - 0x3b8];
    int x2f0;
} State;
typedef struct Table {
    int v[3][5];
} Table;
extern const Table _0210272C;
void PlaySE(u16 seq);
int System_GetTouchHeld(void);
int sub_02086D98(int index, int c);
void sub_02086AB4(State *s, int a, int b);
void sub_02086B2C(State *s, int i);
void sub_02086B6C(State *s, int i, int j);
void sub_02085FFC(State *s);
void sub_02085F80(State *s);
void sub_02085C20(State *s);
// @fn sub_02085C20
void sub_02085C20(State *s) {
    Table t = _0210272C;
    BOOL moved = FALSE;
    s16 col = s->col;
    s16 row = s->row;
    int cell = t.v[row][col];
    int cur, b, nb;

    if (s->flagA != 1 || s->x3ac == 1) {
        return;
    }
    if (s->flagB == 1) {
        if (gSystem.newKeys == 0 || System_GetTouchHeld()) {
            return;
        }
        s->flagB = 0;
        sub_02086B2C(s, cell);
        if (cell == 10 || cell == 11) {
            if (s->mode != 2) {
                s->mode = 2;
                return;
            }
        } else {
            if (s->mode != 1) {
                s->mode = 1;
                return;
            }
        }
        return;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        if (row > 0) {
            s->row--;
        } else {
            s->row = 2;
        }
        moved = TRUE;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        s->row++;
        s->row %= 3;
        moved = TRUE;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        if (cell == 10) {
            s->col = 3;
        } else if (cell == 11) {
            s->col = 0;
        } else {
            s->col = col + 1;
            s->col %= 5;
        }
        moved = TRUE;
    } else if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        if (cell == 10) {
            s->col = 3;
        } else if (cell == 11) {
            s->col = 0;
        } else if (col > 0) {
            s->col = col - 1;
        } else {
            s->col = 4;
        }
        moved = TRUE;
    } else if (gSystem.newKeys & PAD_BUTTON_A) {
        if (cell == 10) {
            sub_02085FFC(s);
            PlaySE(0x5e5);
        } else if (cell == 11) {
            sub_02085F80(s);
            PlaySE(0x5e2);
        } else {
            if (s->x2d4 == 0) {
                return;
            }
            cur = s->cursor;
            s->entries[cur].index = cell + 1;
            sub_02086AB4(s, 1, 0);
            sub_02086AB4(s, 2, 1);
            sub_02086B6C(s, cell, 2);
            ManagedSprite_SetAnim(s->entries[cur].sprite, sub_02086D98(s->entries[cur].index, s->entries[cur].c));
            ManagedSprite_SetAnim(s->sprite258, 3);
            b = s->entries[cur].b;
            if (cur + 1 == s->count) {
                moved = TRUE;
                s->x3ac = 1;
                s->x3b0 = 0;
                s->col = 3;
                s->row = 2;
            } else {
                nb = s->entries[cur + 1].b;
                if (b != nb) {
                    s->x3ac = 1;
                    s->x3b0 = nb;
                } else {
                    s->x3ac = 2;
                    s->x3b0 = cur + 1;
                }
                PlaySE(0x5e5);
            }
        }
    } else if (gSystem.newKeys & PAD_BUTTON_B) {
        sub_02085FFC(s);
        PlaySE(0x5e5);
    } else if (gSystem.newAndRepeatedKeys & PAD_BUTTON_L) {
        if (s->cursor == s->x2f0) {
            s->cursor = s->count - 1;
        } else {
            s->cursor--;
        }
        cur = s->cursor;
        if (s->entries[cur].c == 1) {
            s->x3ac = 2;
            s->x3b0 = cur;
        } else {
            s->x3ac = 1;
            s->x3b0 = s->entries[cur].b;
            s->x3b4 = 1;
        }
        PlaySE(0x5e0);
    } else if (gSystem.newAndRepeatedKeys & PAD_BUTTON_R) {
        if (s->cursor == s->count - 1) {
            s->cursor = s->x2f0;
        } else {
            s->cursor++;
        }
        cur = s->cursor;
        if (s->entries[cur].c == 1) {
            s->x3ac = 2;
            s->x3b0 = cur;
        } else {
            s->x3ac = 1;
            s->x3b0 = s->entries[cur].b;
        }
        PlaySE(0x5e0);
    }
    if (moved == TRUE) {
        PlaySE(0x5e0);
        cell = t.v[s->row][s->col];
        sub_02086B2C(s, cell);
        if (cell == 10 || cell == 11) {
            if (s->mode != 2) {
                s->mode = 2;
                return;
            }
        } else {
            if (s->mode != 1) {
                s->mode = 1;
            }
        }
    }
}
