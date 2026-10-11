#include "to41_overlay_14_residual_20_private.h"

u32 PCBox_UpdateBoxMenuSlide(void *p);
u32 ov14_021E9660(void *p);
void ov14_021E96A8(void *p);
u32 ov14_021E98AC(void *p);
u32 ov14_021E9920(void *p);
u32 ov14_021E9970(void *p);
u32 ov14_021E99A0(void *p);
u32 ov14_021E99F0(void *p);
u32 PCBox_RunStateMachine(void *p, u32 *state);

u32 PCBox_State_WaitFade(void *p);
u32 ov14_021EA068(void *p);
u32 ov14_021EA130(void *p);
u32 ov14_021EA728(void *p);
void ov14_021EA1F0(void *p);
u32 ov14_021EA0B8(void *p);
u32 ov14_021EA180(void *p);
u32 ov14_021EA378(void *p);
u32 ov14_021EAF08(void *p);
u32 PCBox_State_Exit(void *p);
u32 ov14_021EA674(void *p);

u32 ov14_021EA408(void *p);
u32 ov14_021EAC24(void *p);
u32 ov14_021EAB54(void *p);

u32 ov14_021EA378(void *p) {
    void *q = PTR_AT(p, 0x34);
    BgCellManager_UpdateCellMove(PTR_AT(q, 0x2f0), 0xf);
    switch (U16_AT(q, 0x10)) {
    case 0: {
        u32 v;
        PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 9);
        v = U8_AT(PTR_AT(p, 0x34), 0x44c);
        if (v >= 0x25 && v <= 0x2a) {
            v += 0x5b;
        }
        PCBox_SetupMonMove(p, U8_AT(p, 0x21), v);
        if ((v & 0x80) == 0) {
            U8_AT(p, 0x21) = (u8)PCBox_GetMoveEntryTarget(p, U8_AT(p, 0x21));
        }
        U16_AT(q, 0x10)
        ++;
    }
    case 1:
        if (PCBox_AnimateMonMove(p) == 0) {
            PCBox_ApplyMoveEntryIcons(p, PTR_AT(q, 0xc));
            PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 8);
            U16_AT(q, 0x10) = 0;
            return 0;
        }
        break;
    }
    return 1;
}

u32 ov14_021EA408(void *p) {
    void *q = PTR_AT(p, 0x34);
    switch (U16_AT(q, 0x10)) {
    case 0:
        if (U16_AT(q, 0x88c8) != 0) {
            ov14_021F391C(q, 1);
            PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 0xb, 1);
            PCBox_SetSpriteVisible(PTR_AT(p, 0x34), 0xb, 1);
        }
        U16_AT(q, 0x10)
        ++;
    case 1: {
        u32 a = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 8);
        u32 b = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 9);
        u32 c = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xa);
        if (a == 0 && b == 0 && c == 0) {
            U16_AT(q, 0x10)
            ++;
        }
        break;
    }
    case 2: {
        u32 a = PCBox_UpdateCursorSlide(p);
        u32 b = PCBox_UpdateActionMenuSlide(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        if (a == 0 && b == 0) {
            if (U16_AT(PTR_AT(p, 0x34), 0x88c8) != 0) {
                ov14_021F391C(PTR_AT(p, 0x34), 0);
                PCBox_ShowHeldItemIconOutline(PTR_AT(p, 0x34));
            }
            U16_AT(q, 0x10) = 0;
            return 0;
        }
        break;
    }
    }
    return 1;
}
