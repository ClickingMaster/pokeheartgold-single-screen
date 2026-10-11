#include "global.h"

typedef struct {
 u32 state, flag, counter, limit, field10;
 u8 pad14[0x19B8];
 u8 f19CC;
 u8 pad19CD[0xE7];
 u8 f1AB4;
 u8 pad1AB5[0x19F];
 u8 f1C54;
 u8 pad1C55[0x31F];
 u8 f1F74;
 u8 pad1F75[0x6777];
 u8 f86EC;
 u8 pad86ED[0x43];
 u8 f8730;
 u8 pad8731[0x9B];
 u8 f87CC;
 u8 pad87CD[0x17];
 u8 f87E4;
} Work;

extern const u32 ov91_02261C44[];
void ov91_022619E8(void *);
void GF_TouchpadUpdate(void *, int, int);
void ov91_0225EA7C(void *);
int ov91_0225EF70(void *);
void ov91_0225F008(void *, void *);
int ov91_0225F3E8(void *, u32);
u32 ov91_0225F3E4(void *);
void ov91_02260370(void *, u32);
void ov91_0225F3D4(void *);
void ov91_0225E40C(void *);
void ov91_0225DCC4(void *, void *);
void ov91_0225F74C(void *, void *);
void ov91_0225E62C(void *);
void ov91_02261A00(void *, void *, void *);
int ov91_0225D1DC(Work *);

int ov91_0225D1DC(Work *w) {
    u32 state = w->state;
    u32 value;
    if (state < 2 && ov91_02261C44[state] <= w->counter) {
        w->state = state + 1;
        if (w->state == 1) {
            ov91_022619E8(&w->f87E4);
        }
    }
    GF_TouchpadUpdate(&w->f86EC, 1, 0);
    ov91_0225EA7C(w);
    if (!ov91_0225EF70(&w->f8730)) {
        ov91_0225F008(w, &w->f8730);
    }
    if (ov91_0225F3E8(&w->f87CC, w->field10) == 1) {
        value = ov91_0225F3E4(&w->f87CC);
        if (value != 0xFFFF) {
            ov91_02260370(&w->f1F74, value);
        }
        ov91_0225F3D4(&w->f87CC);
    }
    ov91_0225E40C(w);
    ov91_0225DCC4(w, &w->f19CC);
    ov91_0225F74C(w, &w->f1AB4);
    ov91_0225E62C(w);
    ov91_02261A00(&w->f87E4, &w->f1C54, &w->f1F74);
    if (w->flag == 1) {
        w->counter++;
    }
    if ((s32)w->counter >= (s32)w->limit) {
        w->counter = w->limit;
        return 1;
    }
    return 0;
}
