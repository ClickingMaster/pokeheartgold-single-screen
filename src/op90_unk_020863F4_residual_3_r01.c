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
    Entry markers[8];
    s16 positions[24];
    int count;
    int selection;
    int unused;
    int start;
    int end;
    u8 pad2e4[0x3ec - 0x2e4];
    int markerCount;
} State;
void sub_02086DE4(State *s, int animate);
// @fn sub_02086DE4
void sub_02086DE4(State *s, int animate) {
    s16 x, y;
    int i;
    int k;
    s16 pos;
    pos = s->positions[s->selection];
    k = 0;
    for (i = 0; i < s->count; i++) {
        if (i >= s->start && i < s->end) {
            if (i == s->start) {
                pos += 20;
            } else {
                pos += 32;
            }
        } else {
            if (i == 0) {
                pos += 20;
            } else {
                pos += 8;
            }
        }
        ManagedSprite_GetPositionXY(s->entries[i].sprite, &x, &y);
        if (!animate) {
            ManagedSprite_SetPositionXY(s->entries[i].sprite, pos, y);
        } else {
            s->entries[i].vx = (pos - x) / 2;
            s->entries[i].vy = 0;
            s->entries[i].movement = 2;
            s->entries[i].delay = 0;
        }
        if (i == s->markers[k].index && k != s->markerCount) {
            ManagedSprite_GetPositionXY(s->markers[k].sprite, &x, &y);
            if (s->start == s->end) {
                pos += 8;
            } else if (i > s->start && i < s->end) {
                pos += 20;
            } else {
                pos += 8;
            }
            if (!animate) {
                ManagedSprite_SetPositionXY(s->markers[k].sprite, pos, y);
            } else {
                s->markers[k].vx = (pos - x) / 2;
                s->markers[k].vy = 0;
                s->markers[k].movement = 2;
            }
            k++;
        }
    }
}
