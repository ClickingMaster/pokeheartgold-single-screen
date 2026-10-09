#pragma force_active on
#include "global.h"
#include "sprite_system.h"
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
    u8 pad218[0x244 - 0x218];
    u16 x244;
    u16 x246;
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
    u32 x3f0;
} State;
void PlaySE(u16 seq);
int sub_02086D98(int index, int c);
void sub_02086AB4(State *s, int a, int b);
void sub_02086B2C(State *s, int i);
void sub_02086B6C(State *s, int i, int j);
void sub_02085FFC(State *s);
void sub_02085F80(State *s);
void sub_02086180(u32 idx, int a1, State *s);
// @fn sub_02086180
void sub_02086180(u32 idx, int a1, State *s) {
    int cur;
    int b, nb;
    if (s->flagA != 1) {
        return;
    }
    if (s->flagB != 1) {
        s->flagB = 1;
    }
    if (a1 == 0) {
        if (idx < 16) {
            if (idx >= s->x3f0) {
                if (s->entries[idx].c == 1) {
                    s->x3ac = 2;
                    s->x3b0 = idx;
                } else {
                    s->x3ac = 1;
                    s->x3b0 = s->entries[idx].b;
                }
                PlaySE(0x5e5);
            }
            return;
        }
        if (idx == 26) {
            s->x244 = 0;
            s->x246 = 2;
            PlaySE(0x5e5);
        } else if (idx == 27) {
            s->x244 = 3;
            s->x246 = 2;
            PlaySE(0x5e2);
        } else {
            s->x244 = (idx - 16) % 5;
            s->x246 = (idx - 16) / 5;
            PlaySE(0x5e5);
        }
        if (idx >= 16 && idx <= 25) {
            if (s->x2d4 == 0) {
                return;
            }
            cur = s->cursor;
            s->entries[cur].index = idx - 15;
            ManagedSprite_SetAnim(s->entries[cur].sprite, sub_02086D98(s->entries[cur].index, s->entries[cur].c));
            sub_02086AB4(s, 1, 1);
            sub_02086B2C(s, idx - 16);
            sub_02086AB4(s, 1, 0);
            sub_02086AB4(s, 2, 1);
            sub_02086B6C(s, idx - 16, 2);
            ManagedSprite_SetAnim(s->sprite258, 3);
            b = s->entries[cur].b;
            if (cur + 1 == s->count) {
                s->x3ac = 1;
                s->x3b0 = 0;
                s->x3b4 = 0;
                return;
            }
            nb = s->entries[cur + 1].b;
            if (b != nb) {
                s->x3ac = 1;
                s->x3b0 = nb;
                s->x3b4 = 0;
            } else {
                s->x3ac = 2;
                s->x3b0 = cur + 1;
            }
        } else if (idx == 26) {
            sub_02085FFC(s);
        } else {
            sub_02085F80(s);
        }
    }
}
