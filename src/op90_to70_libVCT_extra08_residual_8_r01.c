#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on

typedef struct VctPacket {
    u8 pad0[0x18];
    u32 seq;
    u8 pad1c[0x440];
    u32 special;
    u32 stamp;
    u64 tick;
    u8 pad46c[0];
    u64 out;
} VctPacket;

typedef struct VctStream {
    u32 x0;
    u32 delay;
    u32 x8;
    s32 difference;
    u32 seq;
    u32 dropped;
    s32 jitter;
    u32 reorder;
    u8 pad20[0x24];
    u64 tick;
    u32 stamp;
    u32 pad50;
    u32 last;
    u32 pad58;
    u32 init;
    u8 pad60[0xc];
    u32 samples;
    u32 negative;
    s32 average;
    s32 start;
    s32 jitter2;
    s32 skew;
    s32 hold;
    s32 lower;
    u32 upper;
    u32 count;
} VctStream;

extern void vct_init_sequence(VctStream *s, u16 seq);
extern int vct_update_sequence(VctStream *s, VctPacket *p, u16 seq);
extern u64 OS_GetTick(void);
int vct_calc_jitter_and_skew(VctStream *s, VctPacket *p);

// @fn vct_calc_jitter_and_skew
int vct_calc_jitter_and_skew(VctStream *s, VctPacket *p) {
    u32 elapsed;
    s32 prev;
    s32 diff;
    s32 skew;
    int reset = 0;
    s32 delta;
    u64 now;
    s32 x;
    s32 d;

    if (s->init) {
        vct_init_sequence(s, p->seq);
    } else {
        if (vct_update_sequence(s, p, p->seq) == 0) {
            s->dropped++;
            return 0;
        }
        s->seq = p->seq;
        if (s->last == p->seq) {
            s->dropped++;
            return 0;
        }
        if (s->last + 1 != p->seq && p->special == 0) {
            delta = p->seq - s->last;
            if (delta < 0) {
                delta = -delta;
            }
            if (delta > 100) {
                s->last = p->seq;
                return 0;
            }
            if (s->last > p->seq) {
                s->reorder++;
            }
        }
    }
    s->last = p->seq;
    if (s->stamp == 0) {
        s->stamp = p->stamp;
        s->tick = p->tick;
        s->samples = 0;
    }
    elapsed = (p->stamp - s->stamp) << 6;
    skew = elapsed - (s64)(p->tick - s->tick);
    prev = s->skew;
    if (prev == 0 || p->special != 0) {
        s->skew = skew;
    } else {
        s->skew = skew;
        diff = skew - prev;
        if (diff < 0) {
            diff = -diff;
        }
        if (diff > (s64)((s->jitter2 + 35608) * 2) + 52365ULL) {
            s->hold = diff / 35608 + 2;
        }
        if (s->hold <= 0) {
            s->jitter2 = s->jitter2 + ((diff - s->jitter2) >> 4);
            s->jitter = s->jitter2;
        }
    }
    p->out = elapsed + s->tick + s->delay;
    now = OS_GetTick();
    if (p->out + 35608 < now) {
        return 0;
    }
    if (p->out > now + 523656) {
        return 0;
    }
    if (s->hold <= 0) {
        x = s->jitter2 * 3;
        d = x / 35608;
        if ((u32)x > s->upper) {
            s->upper = (d + 1) * 35608 + 79071;
            s->lower = s->upper - 53412;
            s->delay = d * 35608 + 79071;
            s->count = 0;
        } else if (x < s->lower) {
            if (++s->count > 70) {
                s->upper = (d + 1) * 35608 + 79071;
                s->lower = (s32)((u32)d * 35608 - 17804);
                if (s->lower < 0) {
                    s->lower = 0;
                }
                s->delay = d * 35608 + 79071;
                s->count = 0;
            }
        }
        if (s->delay > 427296) {
            s->delay = 427296;
        }
        if (s->samples < 16) {
            s->samples++;
            s->average = skew;
            s->start = skew;
        } else {
            s->average = (s->average * 31 + skew) / 32;
        }
        s->difference = s->start - s->average;
        if (s->difference > 34561) {
            reset = 1;
        }
        if (s->difference < -34561) {
            s->negative++;
            reset = -1;
        }
        if (reset) {
            s->samples = 0;
            s->stamp = 0;
        }
    }
    s->hold--;
    return 1;
}
