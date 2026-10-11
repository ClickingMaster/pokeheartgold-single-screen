#include "to50_overlay15_bag_scroll_clamp_private.h"
void BagApp_ClampPocketScroll(s16 *scroll, const u16 *delta, s32 limit) {
    if (*scroll + *delta > limit) {
        *scroll = ((limit - 1) / 6) * 6;
    }
    if (*scroll < 0) {
        *scroll = 0;
    }
}
