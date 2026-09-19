#include "to50_overlay96_amount_r105_private.h"

u8 ov96_02210324(R105Work *work, u32 category, u16 value) {
    u16 delta;
    if (category == ov96_021E5F24(*(void **)work)) {
        if (value != work->amount) {
            if (work->amount > value) {
                work->ramp++;
            }
            {
                s32 adjusted = value + ((u32)work->ramp << 7);
                if (adjusted > 999) {
                    adjusted = 999;
                } else if (adjusted < 0) {
                    adjusted = 0;
                }
                ov96_0221058C(work, (u8)adjusted);
            }
            delta = value - work->amount;
            work->amount = value;
        }
    }
    return (u8)delta;
}
