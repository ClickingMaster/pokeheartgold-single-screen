#include "to50_overlay80_command_helper_r30_private.h"

BOOL ov80_0222E198(void *c) {
    BOOL result;
    void *data;

    if (sub_02037454(c) < 2) {
        result = TRUE;
    } else {
        result = sub_02037B38((u8)U16(c, 0x78));
    }
    if (!result) {
        data = Frontier_GetData(PTR(PTR(c, 0), 0));
        if (data != NULL && U8(data, 0x6F) != 0) {
            if (U16(data, 0x74) == 1 || U16(data, 0x74) == 0xFFFE) {
                U8(PTR(c, 0), 0x39) = 1;
                result = TRUE;
            } else if (U16(data, 0x74) == 0) {
                if (PlayerProfile_GetVersion(sub_02034818(sub_0203769C() ^ 1)) == 12) {
                    U8(data, 0x6F) = 0;
                    sub_02096BF8(data, 0);
                }
            }
        }
    }
    return result;
}
