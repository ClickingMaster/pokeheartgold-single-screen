#include "to50_overlay102_cleanup_callbacks_r18_internal.h"
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
void ov102_021EA078(void *task, u8 *state) {
#pragma unused(task)
    u8 *work = PTR(state, 0);
    ov102_021EAE40(PTR(work, 0x1E0), 2);
    ov102_021EB2EC(PTR(PTR(state, 0), 0x1E4), 0, 0);
    if (U8(work, 0x1F4) == 1) ov102_021EB50C(PTR(work, 0x1E4));
    else ov102_021EAF44(PTR(work, 0x1E0));
    ov102_021EB0C0(PTR(work, 0x1E0), ov102_021E9090(PTR(state, 4)));
    ov102_021E94A4(state);
}
void ov102_021EA0D0(void *task, u8 *state) {
#pragma unused(task)
    u8 *work = PTR(state, 0);
    ov102_021EAE40(PTR(work, 0x1E0), 3);
    ov102_021EB2EC(PTR(PTR(state, 0), 0x1E4), 0, 0);
    if (U8(work, 0x1F4) == 1) ov102_021EB50C(PTR(work, 0x1E4));
    else ov102_021EAF44(PTR(work, 0x1E0));
    ov102_021EB0C0(PTR(work, 0x1E0), ov102_021E9090(PTR(state, 4)));
    ov102_021E94A4(state);
}
