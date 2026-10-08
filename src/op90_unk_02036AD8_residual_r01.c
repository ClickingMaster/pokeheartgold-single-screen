#pragma force_active on
#include "global.h"
typedef struct Rng {
    s64 x;
    s64 mul;
    s64 add;
} Rng;
typedef struct Work {
    u8 pad[0x62c];
    Rng rng;
    u8 pad644[0x18];
    u16 buttons;
    u16 pad65e;
    u8 mode;
    s8 timer;
    u16 direction;
} Work;
typedef struct Holder {
    u32 pad[2];
    Work *work;
} Holder;
extern Holder _021D4140;
void sub_02036BE4(void);
static inline u32 Rng_Next(Work *w, u32 max) {
    Rng *rng = &w->rng;
    rng->x = rng->mul * rng->x + rng->add;
    return (u32)((((u64)rng->x >> 32) * max) >> 32);
}
// @fn sub_02036BE4
void sub_02036BE4(void) {
    Work *w = _021D4140.work;
    u16 result = 0;
    if (w->mode == 0 || !(w->buttons & 0xf0)) {
        return;
    }
    if (w->mode == 2) {
        if (w->buttons & 0x20) {
            result |= 0x10;
        }
        if (w->buttons & 0x10) {
            result |= 0x20;
        }
        if (w->buttons & 0x40) {
            result |= 0x80;
        }
        if (w->buttons & 0x80) {
            result |= 0x40;
        }
    } else if (w->direction != 0) {
        result = w->direction;
        w->timer--;
        if (_021D4140.work->timer < 0) {
            _021D4140.work->direction = 0;
        }
    } else {
        switch (Rng_Next(w, 4)) {
        case 0:
            result = 0x20;
            break;
        case 1:
            result = 0x10;
            break;
        case 2:
            result = 0x40;
            break;
        case 3:
            result = 0x80;
            break;
        }
        w = _021D4140.work;
        w->timer = Rng_Next(w, 16);
        _021D4140.work->direction = result;
    }
    _021D4140.work->buttons &= ~0xf0;
    _021D4140.work->buttons += result;
}
