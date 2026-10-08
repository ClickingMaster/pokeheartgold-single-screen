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
typedef struct Range {
    u16 start, end;
} Range;
typedef struct State {
    Entry entries[16];
    Entry markers[8];
    s16 positions[5];
    Range ranges[5];
    u8 pad2be[0x2d0 - 0x2be];
    int count;
    u8 pad2d4[0x374 - 0x2d4];
    int flagB;
    u8 pad378[0x3bc - 0x378];
    int cnt[5];
    u8 pad3d0[0x3dc - 0x3d0];
    int x3dc;
    u8 pad3e0[0x3ec - 0x3e0];
    int markerCount;
    int x3f0;
} State;
void sub_02085604(State *s, int n);
void sub_02085688(State *s);
// @fn sub_02085688
void sub_02085688(State *s) {
    int i, g, j, sum2;
    u16 sum;
    s->flagB = 1;
    sum = 0;
    for (i = 0; i < 5; i++) {
        s->ranges[i].start = sum;
        sum += s->cnt[i];
        s->ranges[i].end = sum;
    }
    sub_02085604(s, s->x3dc + 1);
    for (i = 0; i < 4; i++) {
        if (s->cnt[i] == 0) {
            break;
        }
        s->count += s->cnt[i];
        s->markerCount++;
    }
    s->markerCount--;
    s->positions[0] = 0x70 - ((s->count + s->markerCount) * 8) / 2;
    for (i = 0; i < 4; i++) {
        s->positions[i + 1] = 0x70 - (s->markerCount * 8 + ((s->count - s->cnt[i]) * 8 + s->cnt[i] * 32)) / 2;
    }
    s->positions[1] += 12;
    sum2 = 0;
    for (i = 0; i < s->markerCount; i++) {
        sum2 += s->cnt[i];
        s->markers[i].index = sum2 - 1;
    }
    i = 0;
    g = 0;
    do {
        for (j = 0; j < s->cnt[g]; j++) {
            s->entries[i].b = g + 1;
            i++;
        }
        g++;
    } while (i < s->count);
    for (i = 0; i < s->x3dc; i++) {
        s->x3f0 += s->cnt[i];
    }
}
