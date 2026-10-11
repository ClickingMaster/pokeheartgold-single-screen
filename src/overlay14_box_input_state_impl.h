#include "overlay14_box_input_state_private.h"

#ifndef OMIT_RUN1
u32 ov14_021EC354(void *p) {
    ov14_021E81A8(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 1);
    ov14_021E7EC0(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    PCBox_SlideInPartyPanel(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021F63F0(PTR_AT(p, 0x34));
    ov14_021F63A8(PTR_AT(p, 0x34));
    PCBox_DrawExitButton(p, 0);
    ov14_021F2FDC(p);
    return PCBox_WaitForUpdateFunc(p, ov14_021E9518, 0x23);
}

u32 ov14_021EC3A8(void *p) {
    PCBox_ShowSlotMonInfo(p, 0x1e);
    if (U32_AT(PTR_AT(p, 0), 8) == 3) {
        PCBox_CreateGridInput(p, 7, 0);
        return 0x8b;
    }
    PCBox_CreateGridInput(p, 5, 0);
    return 0x24;
}
#endif

#ifndef OMIT_RUN2
u32 ov14_021EC710(void *p) {
    PCBox_SlideOutPartyPanel(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    return PCBox_WaitForUpdateFunc(p, ov14_021E9518, 0x26);
}

u32 ov14_021EC730(void *p) {
    ov14_021E7E98(p);
    PCBox_SetBoxArrowsVisible(PTR_AT(p, 0x34), 1);
    PCBox_DrawPartyButton(p, 0);
    ov14_021F5E94(p, 0);
    if (U8_AT(p, 0x24) == 0) {
        PCBox_DrawReturnButton(p, 0);
    }
    PCBox_SlideInPartyButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E82A8(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    ov14_021E8328(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    if (U32_AT(PTR_AT(p, 0), 8) == 3) {
        PCBox_SetIconsTranslucent(p, 0x81, 1);
        PCBox_CreateGridInput(p, 6, 0x21);
        return PCBox_WaitForUpdateFunc(p, ov14_021E94BC, 0x75);
    }
    PCBox_SetIconsTranslucent(p, 1, 0);
    PCBox_CreateGridInput(p, 3, 0x21);
    return PCBox_WaitForUpdateFunc(p, ov14_021E94BC, 0xc);
}

u32 ov14_021EC7D4(void *p) {
    void *q = PTR_AT(p, 0x34);
    void *work = PTR_AT(q, 0xc);
    u32 selected = U32_AT(work, 0xe4);
    PCBox_CloseMessageWindow(q, 0x28);
    PlaySE(0x5ea);
    PCBox_ApplyMonMove(p);
    PCBox_RevertShayminFormOnMove(p, U32_AT(work, 0xe4), U32_AT(work, 0xe8));
    PCBox_FreeMoveWork(p);
    if (U8_AT(p, 0x21) == 0xff) {
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)(selected - 0x1e));
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
        PCBox_ShowSlotMonInfo(p, selected);
    } else {
        GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), 8);
        GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
    }
    return 0x24;
}

u32 ov14_021EC854(void *p) {
    s32 row = U8_AT(p, 0x25) % 6;
    U32_AT(PTR_AT(p, 0x34), 0x43c) = row + 0x25;
    if (U8_AT(p, 0x2a) != 0) {
        PCBox_CreateGridInput(p, 4, U8_AT(p, 0x2b));
        U8_AT(p, 0x21) = U8_AT(p, 0x2b);
        ov14_021F1580(p, U8_AT(p, 0x2b));
        row = 0;
        U8_AT(p, 0x2a) = row;
    } else {
        PCBox_CreateGridInput(p, 4, U8_AT(p, 0x2b));
        PCBox_ShowSlotMonInfo(p, U8_AT(p, 0x2b));
        return U32_AT(PTR_AT(p, 0), 8) == 3 ? 0x82 : 0x29;
    }
}
#endif

#ifndef LANDING_EXACT_ONLY
u32 ov14_021ECDA8(void *p) {
    void *work = PTR_AT(PTR_AT(p, 0x34), 0xc);
    u32 selected = U32_AT(work, 0xe8);
    u32 current = U32_AT(work, 0xec);
    u32 high;
    void *mon;

    PlaySE(0x5ea);
    PCBox_ApplyMonMove(p);
    high = selected & 0x80;
    if (high == 0) {
        PCBox_RevertShayminFormOnMove(p, U32_AT(work, 0xe4), U32_AT(work, 0xe8));
    }
    PCBox_FreeMoveWork(p);
    GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), U8_AT(p, 0x21));
    if (U8_AT(p, 0x24) != 0) {
        PCBox_DrawTrayBoxNameAndCount(p);
    }
    if (PCBox_GetSlotMonData(p, U8_AT(p, 0x21), 0xac, 0) == 0) {
        PCBox_ClearMonInfo(p);
        goto done;
    }
    if (selected != 0xff) {
        if (high == 0) {
            goto done;
        }
        PCBox_ShowSlotMonInfo(p, U8_AT(p, 0x21));
        goto done;
    } else {
        selected = U8_AT(p, 0x21);
        if (selected >= 0x1e && current != selected) {
            selected -= 0x1e;
            mon = Party_GetMonByIndex(PTR_AT(p, 8), selected);
            if (ItemIdIsMail((u16)GetMonData(mon, 6, 0)) == 1) {
                PlaySE(0x5f3);
                PCBox_PrintMonActionMessage(p, 0, 6, 0x25);
                U32_AT(p, 0x30) = 0x2c;
                return 6;
            }
            if (GetMonData(mon, 0xa2, 0) != 0) {
                PlaySE(0x5f3);
                PCBox_PrintMonActionMessage(p, 0, 5, 0x25);
                U32_AT(p, 0x30) = 0x2c;
                return 6;
            }
            if (PCBox_PartyHasOtherUsableMon(p, selected) == 0) {
                PlaySE(0x5f3);
                PCBox_PrintReleaseMessage(p, 6, 0x25);
                U32_AT(p, 0x30) = 0x2c;
                return 6;
            }
        }
    }
    if (current != 0xff && (current & 0x80) != 0) {
        u32 next = (current ^ 0x80) + (U8_AT(p, 0x25) / 6) * 6;
        if (U8_AT(p, 0x1f) != next) {
            u32 index = U8_AT(p, 0x21);
            if (index < 0x1e) {
                PCBox_PrintMonActionMessage(p, 0, 4, 0x25);
            } else if (PCBox_PartyHasOtherUsableMon(p, index - 0x1e) == 1) {
                PCBox_PrintMonActionMessage(p, 0, 4, 0x25);
            } else {
                PCBox_PrintReleaseMessage(p, 6, 0x25);
            }
            PlaySE(0x5f3);
            U32_AT(p, 0x30) = 0x2c;
            return 6;
        }
    }
done:
    GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
    return 0x29;
}
#endif

#ifndef OMIT_RUN3
u32 ov14_021ED1AC(void *p) {
    ov14_021F6688(PTR_AT(p, 0x34), 0x25);
    if (U8_AT(p, 0x2a) != 0) {
        PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 8);
        PCBox_ClearMonInfoAndSelection(p);
        return ov14_021F0CD8(p);
    }
    GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
    return 0x29;
}

u32 ov14_021ED1E8(void *p) {
    ov14_021F63B8(PTR_AT(p, 0x34));
    if (U32_AT(PTR_AT(p, 0), 8) == 3) {
        PCBox_SetIconsTranslucent(p, 0x81, 1);
    } else {
        PCBox_SetIconsTranslucent(p, 1, 0);
    }
    if (PCBox_GetSlotMonData(p, U8_AT(p, 0x2b), 0xac, 0) != 0) {
        PCBox_DrawSummaryButton(p, 0);
        PCBox_SlideInSummaryButton(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    }
    U8_AT(p, 0x24) = 1;
    U8_AT(p, 0x29) = 1;
    U8_AT(p, 0x25) = U8_AT(p, 0x1f);
    return PCBox_OpenBoxTray(p, 0x28);
}

u32 ov14_021ED258(void *p) {
    ov14_021F63A8(PTR_AT(p, 0x34));
    if (U32_AT(PTR_AT(p, 0), 8) == 3) {
        PCBox_CreateGridInput(p, 7, 0);
        PCBox_ShowSlotMonInfo(p, GridInputHandler_GetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c)) + 0x1e);
        return 0x8b;
    }
    if (U8_AT(p, 0x2a) != 0) {
        PCBox_CreateGridInput(p, 5, U8_AT(p, 0x2b) - 0x1e);
    } else {
        PCBox_CreateGridInput(p, 5, 0);
    }
    PCBox_ShowSlotMonInfo(p, GridInputHandler_GetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c)) + 0x1e);
    return 0x24;
}
#endif
