#include "global.h"

extern void BufferIntegerAsString(void *, u32, s32, u32, u32, u32);
extern void ClearFrameAndWindow2(void *, u32);
extern void ov73_021EA19C(void *, int);
void ov73_021EA218(void *, int, int);

void ov73_021EA218(void *work, int label, int value) {
    if (label == -1) {
        label = 11;
    }
    BufferIntegerAsString(
        *(void **)((u8 *)work + 0xBD0), 0, value, 5, 2, 1);
    ClearFrameAndWindow2((u8 *)work + 0xDD8, 1);
    ov73_021EA19C(work, label);
}
