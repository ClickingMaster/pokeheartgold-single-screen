#pragma force_active on
#include "global.h"
typedef struct { u8 pad[0x2C]; int busy; } Unk9E0;
extern Unk9E0 _021FF9E0;
extern u8 ov112_021FFA18[];
BOOL ov112_021E5A84(u32 mode);

// @fn ov112_021E5A84
BOOL ov112_021E5A84(u32 mode) {
    u32 i;
    if (mode == 1) {
        if (_021FF9E0.busy != 0) {
            return TRUE;
        }
        if (ov112_021FFA18[0] != 0xFC) {
            return TRUE;
        }
        return FALSE;
    }
    switch (mode) {
    case 2:
    case 3:
        for (i = 0; i < 4; i++) {
        }
        for (i = 0; i < mode; i++) {
        }
        return TRUE;
    }
    return FALSE;
}
