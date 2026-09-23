#include "to50_overlay102_state_callback_r9_internal.h"
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
void ov102_021E8E84(u8 *work, u32 *state) {
    switch (*state) {
    case 0:
        ov102_021E8F5C(work + 0x70, 1, 1);
        ov102_021E940C(PTR(work, 0x14), 0x16);
        *state = 1;
        break;
    case 1:
        switch (ov102_021EA258(PTR(work, 0x14))) {
        case 0:
            ov102_021E940C(PTR(work, 0x14), 0x19);
            *state = 2;
            break;
        case 1:
            ov102_021E7A4C(work, ov102_021E8D1C);
            *state = 2;
            break;
        }
        break;
    case 2:
        if (ov102_021E9464(PTR(work, 0x14))) ov102_021E8D58(work);
        break;
    }
}
