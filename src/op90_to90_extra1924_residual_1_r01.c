#pragma force_active on
#include "global.h"
typedef struct { int a, b; void *p; } Entry;
typedef struct { s16 count, pad; u16 other, lock; int x, y; Entry *entries; } State;
typedef struct { u8 pad[0xf0]; void *p; u8 gap[12]; State *state; } Work;
void ov01_021FA668(Work *w, int value);
// @fn ov01_021FA668
void ov01_021FA668(Work *w, int value) {
    int i = 0;
    State *s = w->state;
    Entry *e = s->entries;
    s->lock = 1;
    for (; i < s->count; i++) {
        if (e->p == NULL) {
            e->p = w->p;
            e->b = value;
            e->a = 0;
            s->lock = 0;
            return;
        }
        e++;
    }
    GF_AssertFail();
}
