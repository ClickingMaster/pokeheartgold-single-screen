#include "global.h"

extern void ov73_021E670C(void *, u32);
extern void ov73_021E66F0(void *);
void *ov73_021E66F4(void *, void *);

void *ov73_021E66F4(void *work, void *result) {
    ov73_021E670C(work, 2);
    ov73_021E66F0(work);
    return result;
}
