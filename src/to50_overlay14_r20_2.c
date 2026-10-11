#include "to50_overlay14_r20_2_private.h"

u32 PCBox_UpdateDepositDrag(void *p) {
    struct {
        s16 pos[2];
        u32 touchX;
        u32 touchY;
    } coords;
    void *work = PTR_AT(p, 0x34);
    u32 readyA;
    u32 readyB;
    u32 readyC;

    if (U16_AT(work, 0x10) <= 1) {
        ManagedSprite_GetPositionXY(
            PTR_AT((u8 *)work + U8_AT((u8 *)work + U8_AT(p, 0x21), 0x4094) * 4, 0x2fc),
            (s32 *)&coords.pos[1],
            (s32 *)&coords.pos[0]);
    }
    readyA = PCBox_UpdateActionMenuSlide(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    readyB = ov14_021E813C(p);
    readyC = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 10);

    switch (U16_AT(work, 0x10)) {
    case 0:
        U16_AT(work, 0x10) = 1;
    case 1:
        if (U8_AT(PTR_AT(p, 0x34), 0x44a) == 1 && readyB == 0) {
            U8_AT(PTR_AT(p, 0x34), 0x44a) = 2;
            ov14_021F69F0(p, 40);
        }
        if (!System_GetTouchHeldCoords(&coords.touchY, &coords.touchX)) {
            ManagedSprite_SetPositionXY(
                PTR_AT((u8 *)PTR_AT(p, 0x34) + U8_AT((u8 *)PTR_AT(p, 0x34) + U8_AT(p, 0x21), 0x4094) * 4, 0x2fc),
                coords.pos[1],
                coords.pos[0]);
            u32 value = 0xff;
            if (U8_AT(PTR_AT(p, 0x34), 0x44a) == 2) {
                value = PCBox_GetTouchedBoxSlot((s16)U32_AT(PTR_AT(p, 0x34), 0x40b8),
                    (s16)U32_AT(PTR_AT(p, 0x34), 0x40bc));
            }
            ov14_021E6F3C(p, U8_AT(p, 0x21), value);
            PCBox_SetSelectedIconOutlineVisible(p, 0);
            U8_AT(p, 0x21) = (u8)PCBox_GetMoveEntryTarget(p, U8_AT(p, 0x21));
            if (U8_AT(p, 0x21) < 30) {
                U16_AT(work, 0x10) = 5;
                break;
            }
            ov14_021E7FEC(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            PCBox_SlideInActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            ov14_021E8824(PTR_AT(p, 0x34));
            U16_AT(work, 0x10) = 2;
            break;
        }
        if (U8_AT(PTR_AT(p, 0x34), 0x44a) == 0 && readyA == 0 && readyC == 0 && (coords.touchY < 16 || coords.touchX < 48 || coords.touchY >= 104)) {
            U8_AT(PTR_AT(p, 0x34), 0x44a) = 1;
            ov14_021E7FB8(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        }
        ManagedSprite_SetPositionXY(
            PTR_AT((u8 *)PTR_AT(p, 0x34) + U8_AT((u8 *)PTR_AT(p, 0x34) + U8_AT(p, 0x21), 0x4094) * 4, 0x2fc),
            (s16)coords.touchY,
            (s16)((s16)coords.touchX - 8));
        PCBox_UpdateSelectedIconOutline(p);
        U32_AT(PTR_AT(p, 0x34), 0x40b8) = coords.touchY;
        U32_AT(PTR_AT(p, 0x34), 0x40bc) = coords.touchX;
        break;
    case 2:
    case 3:
    case 4:
        if (PCBox_AnimateMonMove(p) == 0 && readyB == 0 && readyA == 0) {
            PCBox_ApplyMoveEntryIcons(p, PTR_AT(work, 0xc));
            PCBox_UpdateSelectedIconOutline(p);
            PCBox_SetSelectedIconOutlineVisible(p, U32_AT(PTR_AT(p, 0x34), 0x40c4));
            U16_AT(work, 0x10) = 7;
        }
        break;
    case 5:
        if (ov14_021E66F4(p) == 0) {
            PCBox_ApplyMoveEntryIcons(p, PTR_AT(work, 0xc));
            PCBox_ClearMonInfo(p);
            PCBox_SlideInPartyPanel(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            U8_AT(p, 0x21) = 0xff;
            U16_AT(work, 0x10) = 6;
        }
        break;
    case 6:
        if (readyB == 0) {
            U16_AT(work, 0x10) = 7;
        }
        break;
    case 7:
        PCBox_SetIconsTranslucent(p, 1, 1);
        PCBox_SetIconsTranslucent(p, 2, 0);
        U8_AT(PTR_AT(p, 0x34), 0x44a) = 0;
        U16_AT(work, 0x10) = 0;
        return 0;
    }
    return 1;
}

u32 PCBox_UpdateWithdrawDrag(void *p) {
    u32 touchY;
    u32 touchX;
    s16 pos[2];
    void *work = PTR_AT(p, 0x34);
    u32 readyA = PCBox_UpdateActionMenuSlide(PTR_AT(work, 0x2f0));
    u32 readyB = PCBox_UpdatePartyPanelSlide(p);
    u32 readyC = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 10);

    switch (U16_AT(work, 0x10)) {
    case 0:
        U16_AT(work, 0x10) = 1;
    case 1:
        if (U8_AT(PTR_AT(p, 0x34), 0x44a) == 1 && readyB == 0) {
            U8_AT(PTR_AT(p, 0x34), 0x44a) = 2;
            ov14_021F69F0(p, 40);
        }
        if (!System_GetTouchHeldCoords(&touchY, &touchX)) {
            u32 value = 0xff;
            if (U8_AT(PTR_AT(p, 0x34), 0x44a) == 2) {
                value = PCBox_GetTouchedPartySlot((s16)U32_AT(PTR_AT(p, 0x34), 0x40b8),
                    (s16)U32_AT(PTR_AT(p, 0x34), 0x40bc),
                    ov14_021F7C08);
            }
            PCBox_SetupMonMoveToPartyEnd(p, U8_AT(p, 0x21), value);
            PCBox_SetSelectedIconOutlineVisible(p, 0);
            U8_AT(p, 0x21) = (u8)PCBox_GetMoveEntryTarget(p, U8_AT(p, 0x21));
            if (U8_AT(p, 0x21) >= 30) {
                U16_AT(work, 0x10) = 5;
                break;
            }
            if (U8_AT(PTR_AT(p, 0x34), 0x44a) != 0) {
                PCBox_SlideOutPartyPanel(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            }
            U16_AT(work, 0x10) = 2;
            break;
        }
        if (U8_AT(PTR_AT(p, 0x34), 0x44a) == 0 && readyA == 0 && readyC == 0) {
            PCBox_GetSlotPosition(U8_AT(p, 0x21), &pos[1], &pos[0], U8_AT(p, 0x22));
            if (touchY < (u32)(pos[1] - 16) || touchY >= (u32)(pos[1] + 16) || touchX < (u32)(pos[0] - 16) || touchX >= (u32)(pos[0] + 16)) {
                U8_AT(PTR_AT(p, 0x34), 0x44a) = 1;
                ov14_021E7ED0(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
                PCBox_SlideInPartyPanel(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            }
        }
        ManagedSprite_SetPositionXY(
            PTR_AT((u8 *)PTR_AT(p, 0x34) + U8_AT((u8 *)PTR_AT(p, 0x34) + U8_AT(p, 0x21), 0x4094) * 4, 0x2fc),
            (s16)touchY,
            (s16)((s16)touchX - 8));
        PCBox_UpdateSelectedIconOutline(p);
        U32_AT(PTR_AT(p, 0x34), 0x40b8) = touchY;
        U32_AT(PTR_AT(p, 0x34), 0x40bc) = touchX;
        break;
    case 2:
        if (readyB == 0) {
            PCBox_SlideInActionMenu(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            ov14_021E8824(PTR_AT(p, 0x34));
            U16_AT(work, 0x10) = 3;
        }
    case 3:
    case 4:
        if (PCBox_AnimateMonMove(p) == 0 && U16_AT(work, 0x10) == 4 && readyA == 0) {
            PCBox_ApplyMoveEntryIcons(p, PTR_AT(work, 0xc));
            PCBox_UpdateSelectedIconOutline(p);
            PCBox_SetSelectedIconOutlineVisible(p, U32_AT(PTR_AT(p, 0x34), 0x40c4));
            U16_AT(work, 0x10) = 8;
            break;
        }
        if (U16_AT(work, 0x10) == 3) {
            U16_AT(work, 0x10) = 4;
        }
        break;
    case 5:
        if (PCBox_AnimateMonMove(p) == 0) {
            PCBox_ApplyMoveEntryIcons(p, PTR_AT(work, 0xc));
            U8_AT(p, 0x21) = 0xff;
            PCBox_SlideOutPartyPanel(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            U16_AT(work, 0x10)
            ++;
        }
        break;
    case 6:
        if (readyB == 0) {
            PCBox_ClearMonInfo(p);
            ov14_021E8328(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            U16_AT(work, 0x10)
            ++;
        }
        break;
    case 7:
        if (readyC == 0) {
            U16_AT(work, 0x10) = 8;
        }
        break;
    case 8:
        PCBox_SetIconsTranslucent(p, 1, 0);
        PCBox_SetIconsTranslucent(p, 2, 0);
        U8_AT(PTR_AT(p, 0x34), 0x44a) = 0;
        U16_AT(work, 0x10) = 0;
        return 0;
    }
    return 1;
}
