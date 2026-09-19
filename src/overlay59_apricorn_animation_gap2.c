#include "overlay59_apricorn_animation_gap2_private.h"
#define U8(w, o)  (*(u8 *)((u8 *)(w) + (o)))
#define S8(w, o)  (*(s8 *)((u8 *)(w) + (o)))
#define U16(w, o) (*(u16 *)((u8 *)(w) + (o)))
BOOL ov59_0223B164(Ov59R10Gap2Work *work) {
    u32 value = ov59_0223C99C[U8(work, 0x36)];
    switch (U16(work, 0x42)) {
    case 0: {
        s8 delta;
        U8(work, 0x51) = 0;
        U8(work, 0x38) = 0;
        delta = S8((u8 *)work + U8(work, 0x36), 0x39);
        if (delta < 0) {
            U8(work, 0x37) = -delta;
            PlaySE(0x926);
        } else {
            U8(work, 0x37) = delta;
            PlaySE(0x925);
        }
        U16(work, 0x42)
        ++;
    }
    case 1:
        if (U8(work, 0x51)-- != 0) {
            break;
        }
        if (S8((u8 *)work + U8(work, 0x36), 0x39) < 0) {
            ov59_0223BE70(work, (u8)value, 1);
        } else {
            ov59_0223BE70(work, (u8)value, 0);
        }
        if (U8(work, 0x38) >= U8(work, 0x37)) {
            U16(work, 0x42)
            ++;
        } else {
            U8(work, 0x51) = 15;
        }
        break;
    case 2:
        if (U8(work, 0x4F) != 0) {
            break;
        }
        ov59_0223BD4C(work, (u8 *)work + 0x7C + U8(work, 0x4A) * 0x34, (u8)value);
        ov59_0223BBB0(work);
        if (S8((u8 *)work + U8(work, 0x36), 0x39) < 0) {
            ov59_0223BBD4(work, value + 7);
        } else {
            ov59_0223BBD4(work, value + 2);
        }
        U16(work, 0x42)
        ++;
        break;
    default:
        if (TextPrinterCheckActive(U8(work, 0x4D))) {
            break;
        }
        U16(work, 0x42) = 0;
        return TRUE;
    }
    return FALSE;
}
