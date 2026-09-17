#include "to50_overlay15_bag_quantity_branch_private.h"
s32 ov15_021FBCAC(void *work) {
    void *args;
    u8 *entry;
    U16(work, 0x680) = 1;
    ov15_021FFF24(work);
    args = PTR(work, 0x234);
    entry = (u8 *)args + U8(args, 0x64) * 12;
    if (Pocket_GetQuantity(PTR(entry, 4), U8(entry, 0xD), U16(args, 0x66), 6) == 1) {
        ov15_021FEEA4(work);
        ov15_021FD788(work, 0);
        return 8;
    }
    ov15_021FD574(work, 3, 0, 0);
    ov15_02200300(work, 3, U16(work, 0x682));
    ov15_021FEDEC(work, 3);
    ov15_021FF7C4(work);
    ov15_021FF29C(work, 1);
    return 5;
}
