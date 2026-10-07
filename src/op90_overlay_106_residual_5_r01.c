#pragma force_active on
#include "overlay106_cinematic_internal.h"
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
int ov106_021E62F4(void *state);
// @fn ov106_021E62F4
int ov106_021E62F4(void *state) {
    if (ov106_021E5D08(state) == 0 && IsPaletteFadeFinished() == 1) {
        U32(state, 0x414) = 0;
        StopSE(0x868);
        ov106_021E6064(PTR(state, 4));
        return 1;
    }
    if (U32(state, 0x414) == 0) {
        sub_02006118(0x868, 0);
    } else if (U32(state, 0x414) == 0x73) {
        PlaySE(0x931);
    } else if (U32(state, 0x414) == 0x91) {
        PlayCry(0xf9, 0);
        PTR(state, 4) = ov106_021E601C(5, 13);
    } else if (U32(state, 0x414) == 0xe8) {
        ov106_021E597C(state, 6, 1, 1);
    }
    if (U32(state, 0x414) != 0) {
        if (U32(state, 0x414) == 10) {
            GF_SetVolumeBySeqNo(0x868, 30);
        } else if (U32(state, 0x414) < 10) {
            GF_SetVolumeBySeqNo(0x868, U32(state, 0x414) * 0x300 / 0x100);
        }
    }
    U32(state, 0x414)++;
    return 3;
}
