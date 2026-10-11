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

u32 ov14_021EA068(void *p) {
    void *q = PTR_AT(p, 0x34);
    switch (U16_AT(q, 0x10)) {
    case 0:
        if (BgCellManager_UpdateCellMove(PTR_AT(q, 0x2f0), 0xa) == 0) {
            U16_AT(q, 0x10)
            ++;
        }
        break;
    case 1: {
        u32 a = PCBox_UpdateActionMenuSlide(PTR_AT(q, 0x2f0));
        u32 b = PCBox_UpdateCursorSlide(p);
        if (a == 0 && b == 0) {
            U16_AT(q, 0x10) = 0;
            return 0;
        }
        break;
    }
    }
    return 1;
}

u32 ov14_021EA0B8(void *p) {
    void *q = PTR_AT(p, 0x34);
    switch (U16_AT(q, 0x10)) {
    case 0: {
        u32 a = BgCellManager_UpdateCellMove(PTR_AT(q, 0x2f0), 8);
        u32 b = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 9);
        u32 c = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xa);
        if (a == 0 && b == 0 && c == 0) {
            U16_AT(q, 0x10)
            ++;
        }
        break;
    }
    case 1: {
        u32 a = PCBox_UpdateCursorSlide(p);
        u32 b = PCBox_UpdateActionMenuSlide(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        if (a == 0 && b == 0) {
            U16_AT(q, 0x10) = 0;
            return 0;
        }
        break;
    }
    }
    return 1;
}

u32 ov14_021EA130(void *p) {
    void *q = PTR_AT(p, 0x34);
    switch (U16_AT(q, 0x10)) {
    case 0: {
        u32 a = PCBox_UpdateActionMenuSlide(PTR_AT(q, 0x2f0));
        u32 b = PCBox_UpdateCursorSlide(p);
        if (a == 0 && b == 0) {
            U16_AT(q, 0x10)
            ++;
        }
        break;
    }
    case 1:
        if (BgCellManager_UpdateCellMove(PTR_AT(q, 0x2f0), 0xa) == 0) {
            U16_AT(q, 0x10) = 0;
            return 0;
        }
        break;
    }
    return 1;
}

u32 ov14_021EA180(void *p) {
    void *q = PTR_AT(p, 0x34);
    switch (U16_AT(q, 0x10)) {
    case 0: {
        u32 a = PCBox_UpdateCursorSlide(p);
        u32 b = ov14_021E9434(p);
        if (a == 0 && b == 0) {
            U16_AT(q, 0x10)
            ++;
        }
        break;
    }
    case 1: {
        u32 a = BgCellManager_UpdateCellMove(PTR_AT(q, 0x2f0), 8);
        u32 b = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 9);
        u32 c = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xa);
        if (a == 0 && b == 0 && c == 0) {
            U16_AT(q, 0x10) = 0;
            return 0;
        }
        break;
    }
    }
    return 1;
}

void ov14_021EA1F0(void *p) {
    s16 a[2];
    PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 0xa);
    ManagedSprite_GetPositionXY(PTR_AT(PTR_AT(p, 0x34), 0x320), (s32 *)&a[1], (s32 *)&a[0]);
    ManagedSprite_SetPositionXY(PTR_AT((u8 *)PTR_AT(p, 0x34) + U8_AT((u8 *)PTR_AT(p, 0x34) + U8_AT(p, 0x21), 0x4094) * 4, 0x2fc), a[1], (s16)(a[0] + 4));
    PCBox_SetIconLayer(PTR_AT(p, 0x34), U8_AT(p, 0x21), 0);
}
