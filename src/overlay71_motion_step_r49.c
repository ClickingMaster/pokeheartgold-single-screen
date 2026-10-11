#include "global.h"
typedef struct Ov71R49 {
    u8 pad00[0x14];
    void *object;
    u8 pad18[0x34 - 0x18];
    u8 vector[2];
    s16 base;
    u8 pad38[2];
    s16 delta;
    s16 step;
    s16 target;
    u32 frames;
} Ov71R49;
void TradeSequence_SetModelRotation(void *, void *);
void ov71_0224BA64(Ov71R49 *work);
void ov71_0224BA64(Ov71R49 *work) {
    if (work->frames != 0) {
        work->delta += work->step;
        work->frames--;
        if (work->frames == 0) {
            work->delta = work->target;
        }
    }
    work->base += work->delta;
    TradeSequence_SetModelRotation(work->object, &work->vector);
}
