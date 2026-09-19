#include "to53_overlay112_w01_private.h"


void ov112_021E7D7C(void *state) {
    ov112_021E7DFC(state, (u8 *)state + 0x10f4);
    ov112_021E8CB8((u8 *)state + 0xc910, state);
    ov112_021E7DC0(state);
}

void ov112_021E7DA4(void *state) {
    ov112_021E8CB8((u8 *)state + 0xc910, state);
    ov112_021E7DC0(state);
}

void ov112_021E7DC0(void *state) {
    void *p;
    ov112_021F3244(*(void **)((u8 *)state + 0x20), (u8 *)state + 0xf200);
    *(u32 *)((u8 *)state + 0xf1d0) = 1;
    *(u16 *)((u8 *)state + 0xf1d4) = GAME_VERSION;
    p = sub_02032728(*(void **)((u8 *)state + 0x1e440));
    MIi_CpuCopyFast(p, (u32 *)((u8 *)state + 0xf1d8), 0x28);
}
