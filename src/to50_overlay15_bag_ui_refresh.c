#include "to50_overlay15_bag_ui_refresh_private.h"
s32 ov15_021FBFC0(void *work) {
    ov15_021FED3C(work);
    ov15_021FB518(work);
    ov15_02200428(work);
    ov15_021FF29C(work, 0);
    ManagedSprite_SetAnimationFrame(PTR(work, 0x29C), 0);
    ManagedSprite_SetAnim(PTR(work, 0x29C), 0x10);
    return 1;
}
