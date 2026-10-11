#include "overlay14_box_transition_handlers_private.h"

u32 PCBox_State_TryRelease(void *p) {
    u32 slot;
    void *mon;
    if (U8_AT(p, 0x21) >= 0x1e) {
        PCBox_SetIconsTranslucent(p, 2, 1);
        PCBox_SetSlotIconTranslucent(PTR_AT(p, 0x34), U8_AT(p, 0x21), 0);
        slot = U8_AT(p, 0x21) - 0x1e;
        if (PCBox_PartyHasOtherUsableMon(p, slot) == 0) {
            PlaySE(0x5f3);
            PCBox_PrintReleaseMessage(p, 6, 0x25);
            U8_AT(PTR_AT(p, 0x34), 0x44e) |= 0x80;
            U32_AT(p, 0x30) = 0xe;
            return 6;
        }
        mon = Party_GetMonByIndex(PTR_AT(p, 8), slot);
        if (ItemIdIsMail((u16)GetMonData(mon, 6, 0)) == 1) {
            PlaySE(0x5f3);
            PCBox_PrintMonActionMessage(p, 0, 6, 0x25);
            U8_AT(PTR_AT(p, 0x34), 0x44e) |= 0x80;
            U32_AT(p, 0x30) = 0xe;
            return 6;
        }
        if (GetMonData(mon, 0xa2, 0) != 0) {
            PlaySE(0x5f3);
            PCBox_PrintMonActionMessage(p, 0, 5, 0x25);
            U8_AT(PTR_AT(p, 0x34), 0x44e) |= 0x80;
            U32_AT(p, 0x30) = 0xe;
            return 6;
        }
    } else {
        PCBox_SetBoxArrowsVisible(PTR_AT(p, 0x34), 0);
        PCBox_SetIconsTranslucent(p, 1, 1);
        PCBox_SetSlotIconTranslucent(PTR_AT(p, 0x34), U8_AT(p, 0x21), 0);
    }
    if (GetBoxMonData(PCBox_GetSlotBoxMon(p, U8_AT(p, 0x1f), U8_AT(p, 0x21)), 0x4c, 0) != 0) {
        PlaySE(0x5f3);
        PCBox_PrintReleaseMessage(p, 3, 0x25);
        U8_AT(PTR_AT(p, 0x34), 0x44e) |= 0x80;
        U32_AT(p, 0x30) = 0xe;
        return 6;
    }
    PCBox_PrintReleaseMessage(p, 0, 0x25);
    return PCBox_ShowYesNo(p, 1);
}

u32 PCBox_State_ReleaseMoveCheck(void *p) {
    void *x;
    PCBox_RunReleaseMoveCheck(p);
    x = PTR_AT(PTR_AT(p, 0x34), 0x88dc);
    if (PCBox_UpdateReleaseShrink(x) == 0) {
        x = PTR_AT(PTR_AT(p, 0x34), 0x88dc);
        if (U8_AT(x, 6) != 0) {
            return 0x1e;
        }
        PCBox_EndReleaseShrink(x);
        PCBox_FreeReleaseMoveCheck(p);
        return 0x1b;
    }
    return 0x1a;
}

u32 PCBox_State_PrintReleased(void *p) {
    PCBox_PrintReleaseMessage(p, 1, 0x25);
    U32_AT(p, 0x30) = 0x1c;
    return 6;
}
u32 PCBox_State_PrintByeBye(void *p) {
    PCBox_PrintReleaseMessage(p, 2, 0x25);
    U32_AT(p, 0x30) = 0x1d;
    return 6;
}

u32 PCBox_State_RemoveReleasedMon(void *p) {
    void (*next)(void);
    PCBox_DeleteSlotMon(p, U8_AT(p, 0x1f), U8_AT(p, 0x21));
    PCBox_CloseMessageWindow(PTR_AT(p, 0x34), 0x25);
    PCBox_ClearMonInfo(p);
    if (U8_AT(p, 0x21) < 0x1e) {
        PCBox_DrawBoxThumbnail(p, U8_AT(p, 0x1f));
        PCBox_RefreshTrayBoxThumbnail(p, U8_AT(p, 0x1f));
        if (U32_AT(PTR_AT(p, 0), 8) == 1) {
            PCBox_PrintMonActionMessage(p, 0, 0, 0x27);
            U32_AT(p, 0x30) = 0x51;
        } else {
            U32_AT(p, 0x30) = 0xc;
            PCBox_SlideInPartyButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
            ov14_021E82A8(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        }
        ov14_021E8328(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), U8_AT(p, 0x21));
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        PCBox_SetBoxArrowsVisible(PTR_AT(p, 0x34), 1);
        PCBox_SetIconsTranslucent(p, 1, 0);
        U8_AT(p, 0x21) = 0xff;
        next = ov14_021E9450;
    } else {
        PCBox_AllocMoveWork(p);
        U8_AT(p, 0x22) = 1;
        U32_AT(p, 0x30) = 0x21;
        ov14_021E8328(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
        PCBox_SetIconsTranslucent(p, 2, 0);
        next = ov14_021E9194;
    }
    return PCBox_WaitForUpdateFunc(p, next, U32_AT(p, 0x30));
}

u32 ov14_021EC23C(void *p) {
    PCBox_FreeMoveWork(p);
    if (U8_AT(p, 0x24) != 0) {
        PCBox_DrawTrayBoxNameAndCount(p);
    }
    PCBox_ShowSlotMonInfo(p, U8_AT(p, 0x21));
    GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(U8_AT(p, 0x21) - 0x1e));
    GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
    U8_AT(p, 0x21) = 0xff;
    if (U32_AT(PTR_AT(p, 0), 8) == 0) {
        PCBox_PrintMonActionMessage(p, 0, 0, 0x27);
        return 0x5b;
    }
    return 0x24;
}

u32 PCBox_State_ReleaseCameBack(void *p) {
    void *x = PTR_AT(PTR_AT(p, 0x34), 0x88dc);
    if (PCBox_UpdateReleaseGrow(x) == 0) {
        x = PTR_AT(PTR_AT(p, 0x34), 0x88dc);
        PCBox_ResetSpriteAffine(x);
        PCBox_FreeReleaseMoveCheck(p);
        PCBox_CreateSelectedIconOutline(p);
        PCBox_PrintReleaseMessage(p, 4, 0x25);
        ov14_021E8824(PTR_AT(p, 0x34));
        U32_AT(p, 0x30) = 0x1f;
        return 6;
    }
    return 0x1e;
}
u32 PCBox_State_PrintWorried(void *p) {
    PCBox_PrintReleaseMessage(p, 5, 0x25);
    U32_AT(p, 0x30) = 0x20;
    return 6;
}
u32 ov14_021EC300(void *p) {
    PCBox_SetSelectedIconOutlineVisible(p, 1);
    if (U8_AT(p, 0x21) < 0x1e) {
        PCBox_SetBoxArrowsVisible(PTR_AT(p, 0x34), 1);
        PCBox_SetIconsTranslucent(p, 1, 0);
    } else {
        PCBox_SetIconsTranslucent(p, 2, 0);
    }
    return PCBox_WaitForUpdateFunc(p, (void *)ov14_021E9450, 0xe);
}
