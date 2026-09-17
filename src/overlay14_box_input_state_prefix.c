#include "overlay14_box_input_state_prefix_private.h"

u32 ov14_021EC8D0(void *p) {
    u32 v = ov14_021F6A34();
    if (v != -1) {
        if (ov14_021E6070(p, v + 0x1e, 0xac, 0)) {
            PlaySE(0x5eb);
            System_GetTouchNewCoords((u32 *)((u8 *)PTR_AT(p, 0x34) + 0x40b8), (u32 *)((u8 *)PTR_AT(p, 0x34) + 0x40bc));
            ov14_021E7588(p, v + 0x1e);
            ov14_021F2A18(PTR_AT(p, 0x34), 9, 0);
            return ov14_021F083C(p, v + 0x1e);
        }
        ov14_021E765C(p);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(v + 0x1e));
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        if (ov14_021E85E4(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return ov14_021F0EE8(p, 0x29);
        }
        if (ov14_021E8648(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return ov14_021F0D34(p, 0x29);
        }
        return 0x29;
    }
    v = ov14_021F6A14();
    if (v != -1) {
        if (ov14_021E6070(p, v, 0xac, 0)) {
            PlaySE(0x5eb);
            System_GetTouchNewCoords((u32 *)((u8 *)PTR_AT(p, 0x34) + 0x40b8), (u32 *)((u8 *)PTR_AT(p, 0x34) + 0x40bc));
            ov14_021E7588(p, v);
            ov14_021F2A18(PTR_AT(p, 0x34), 9, 0);
            return ov14_021F083C(p, v);
        }
        ov14_021E765C(p);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)v);
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        if (ov14_021E85E4(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return ov14_021F0EE8(p, 0x29);
        }
        if (ov14_021E8648(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return ov14_021F0D34(p, 0x29);
        }
        return 0x29;
    }
    if (ov14_021F7B7C(p) == 1) {
        v = GridInputHandler_GetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c));
        if (ov14_021E6070(p, v, 0xac, 0)) {
            PlaySE(0x5dd);
            U8_AT(p, 0x21) = (u8)v;
            U8_AT(p, 0x26) = 1;
            return ov14_021F2330(p, 0xf, 0x97);
        }
        return 0x29;
    }
    v = ov14_021F70C0(p);
    switch (v) {
    case 0x25:
        PlaySE(0x5dd);
        ov14_021E76B8(p);
        return ov14_021F0D58(p, 0);
    case 0x26:
        PlaySE(0x5dd);
        ov14_021E76B8(p);
        return ov14_021F0D58(p, 1);
    case 0x27:
        PlaySE(0x5dd);
        ov14_021E76B8(p);
        return ov14_021F0D58(p, 2);
    case 0x28:
        PlaySE(0x5dd);
        ov14_021E76B8(p);
        return ov14_021F0D58(p, 3);
    case 0x29:
        PlaySE(0x5dd);
        ov14_021E76B8(p);
        return ov14_021F0D58(p, 4);
    case 0x2a:
        PlaySE(0x5dd);
        ov14_021E76B8(p);
        return ov14_021F0D58(p, 5);
    case 0x2b:
        PlaySE(0x5dc);
        ov14_021E76B8(p);
        ov14_021F1004(p, -1);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(p, 0x25) % 6 + 0x25));
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        if (ov14_021E85E4(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return ov14_021F0EE8(p, 0x29);
        }
        if (ov14_021E8648(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return ov14_021F0D34(p, 0x29);
        }
        return 0x29;
    case 0x2c:
        PlaySE(0x5dc);
        ov14_021E76B8(p);
        ov14_021F1004(p, 1);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(p, 0x25) % 6 + 0x25));
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        if (ov14_021E85E4(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return ov14_021F0EE8(p, 0x29);
        }
        if (ov14_021E8648(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
            return ov14_021F0D34(p, 0x29);
        }
        return 0x29;
    case 0x2d:
        PlaySE(0x5dc);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(p, 0x25) % 6 + 0x25));
        return ov14_021F2270(p, 0xe, 0xa0);
    case -3: {
        u32 n = GridInputHandler_GetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c));
        if (n < 0x25) {
            if (ov14_021E7588(p, n) == 1) {
                if (ov14_021E8648(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 0) {
                    ov14_021F6408(p, 0);
                    ov14_021E8620(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
                }
            } else if (ov14_021E8648(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
                ov14_021E8634(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            }
        } else {
            ov14_021E765C(p);
            if (ov14_021E8648(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 1) {
                ov14_021E8634(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            }
        }
        PlaySE(0x5dc);
        if (U32_AT(PTR_AT(p, 0x34), 8) != 0) {
            return ov14_021F0244(p, 0x4b);
        }
        break;
    }
    case 0x24:
        PlaySE(0x633);
        ov14_021F29E4(PTR_AT(p, 0x34), 9, 8);
        return ov14_021F2490(p, 1, 0xa1);
    case -2:
        if (ov14_021E85E4(PTR_AT(PTR_AT(p, 0x34), 0x2f0)) == 0) {
            PlaySE(0x633);
            return ov14_021F2490(p, 1, 0xa1);
        }
        PlaySE(0x5dc);
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(p, 0x25) % 6 + 0x25));
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        return ov14_021F0EE8(p, 0x29);
    default:
        if (ov14_021E6070(p, v, 0xac, 0)) {
            PlaySE(0x5eb);
            ov14_021E7588(p, v);
            return ov14_021F1580(p, v);
        }
        break;
    case -4:
    case -1:
        break;
    }
    return 0x29;
}
