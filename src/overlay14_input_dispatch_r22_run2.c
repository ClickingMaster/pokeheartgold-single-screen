#include "overlay14_input_dispatch_r22_private.h"

u32 PCBox_State_ApplyMonMove(void *work)
{
    PlaySE(0x5ea);
    PCBox_ApplyMonMove(work);
    PCBox_FreeMoveWork(work);
    GridInputHandler_SetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c), 0x24);
    GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(work, 0x34), 0x2c), 1);
    return 0xc;
}

void ov14_021EB7E4(void *work)
{
    u32 msg;
    u8 *field;

    ov14_021F6688(PTR_AT(work, 0x34), 0x25);
    switch (U32_AT(PTR_AT(work, 0), 8)) {
    case 1:
        PCBox_DrawActionMenu(work, ov14_021F7D2C, 4);
        PCBox_PrintMonActionMessage(work, U8_AT(work, 0x21), 1, 0x27);
        msg = 0x5a;
        break;
    case 0:
        PCBox_DrawActionMenu(work, ov14_021F7D1C, 4);
        PCBox_PrintMonActionMessage(work, U8_AT(work, 0x21), 1, 0x27);
        msg = 0x72;
        break;
    case 2:
    default:
        PCBox_DrawActionMenu(work, ov14_021F7D3C, 5);
        msg = U8_AT(work, 0x21) < 0x1e ? 0x4d : 0x4e;
        break;
    }
    field = (u8 *)PTR_AT(work, 0x34) + 0x44e;
    if (((u32)(*field << 24) >> 31) == 1) {
        *field &= ~0x80;
        if (U8_AT(work, 0x21) >= 0x1e) {
            PCBox_SetIconsTranslucent(work, 2, 0);
        } else {
            PCBox_SetBoxArrowsVisible(PTR_AT(work, 0x34), 1);
            PCBox_SetIconsTranslucent(work, 1, 0);
        }
    }
    PCBox_SlideInActionMenu(PTR_AT(PTR_AT(work, 0x34), 0x2f0));
    PCBox_WaitForUpdateFunc(work, ov14_021E9434, msg);
}

void ov14_021EB8C0(void *work)
{
    u32 msg;

    PCBox_ShowSlotMonInfo(work, U8_AT(work, 0x21));
    if (U8_AT(work, 0x26) == 0) {
        if (U32_AT(PTR_AT(work, 0), 8) == 1) {
            PCBox_DrawActionMenu(work, ov14_021F7D2C, 4);
        } else if (U32_AT(PTR_AT(work, 0), 8) == 0) {
            PCBox_SetBoxArrowsVisible(PTR_AT(work, 0x34), 0);
            PCBox_SetIconsTranslucent(work, 1, 1);
            PCBox_DrawActionMenu(work, ov14_021F7D1C, 4);
        } else {
            if (U8_AT(work, 0x21) >= 0x1e) {
                PCBox_SetIconsTranslucent(work, 1, 1);
                if (U8_AT(work, 0x24) != 0) {
                    ov14_021E8664(work);
                }
            } else if (U8_AT(work, 0x24) != 0) {
                ov14_021E8664(work);
            }
            PCBox_DrawActionMenu(work, ov14_021F7D3C, 5);
            ov14_021E87F4(work);
        }
        ov14_021E83C4(PTR_AT(PTR_AT(work, 0x34), 0x2f0));
        if (U8_AT(work, 0x21) < 0x1e) {
            if (U32_AT(PTR_AT(work, 0), 8) == 1) {
                PCBox_PrintMonActionMessage(work, U8_AT(work, 0x21), 1, 0x27);
                msg = 0x51;
            } else {
                msg = 0xc;
            }
        } else if (U32_AT(PTR_AT(work, 0), 8) == 0) {
            ov14_021F0BF4(work);
            PCBox_PrintMonActionMessage(work, U8_AT(work, 0x21), 1, 0x27);
            msg = 0x5b;
        } else {
            ov14_021F0B70(work);
            PCBox_SetBoxArrowsVisible(PTR_AT(work, 0x34), 0);
            PCBox_CreateGridInput(work, 5, 9);
            msg = 0x24;
        }
        PCBox_CreateSelectedIconOutline(work);
        if (U32_AT(PTR_AT(work, 0), 8) != 3) {
            ov14_021E8874(PTR_AT(work, 0x34));
        }
    } else {
        ov14_021F0BB4(work);
        PCBox_ShowSummaryButton(PTR_AT(PTR_AT(work, 0x34), 0x2f0));
        PCBox_SetupBoxTraySprites(work);
        PCBox_UpdateCurrentBoxMarker(work);
        PCBox_UpdateTrayCursor(work);
        PCBox_DrawTrayBoxNameAndCount(work);
        PCBox_SlideInBoxTray(PTR_AT(PTR_AT(work, 0x34), 0x2f0));
        while (PCBox_UpdateBoxTraySlideIn(work) != 0) {
        }
        if (U8_AT(work, 0x27) == 0) {
            PCBox_CreateGridInput(work, 4, U8_AT(work, 0x21));
        } else {
            PCBox_CreateGridInput(work, 4, U8_AT(work, 0x28));
            U8_AT(PTR_AT(work, 0x34), 0x44b) = 1;
            ov14_021EA1F0(work);
        }
        U32_AT(PTR_AT(work, 0x34), 0x43c) = U8_AT(work, 0x25) % 6 + 0x25;
        if (U32_AT(PTR_AT(work, 0), 8) == 3) {
            PCBox_SetIconsTranslucent(work, 0x81, 1);
            PCBox_SetIconsTranslucent(work, 0x82, 1);
            msg = 0x82;
        } else if (U8_AT(work, 0x27) == 0) {
            msg = 0x29;
        } else {
            msg = 0x73;
        }
        if (U32_AT(PTR_AT(work, 0), 8) != 3 && U8_AT(work, 0x27) != 0) {
            ov14_021E8874(PTR_AT(work, 0x34));
        }
        if (U8_AT(work, 0x29) == 1) {
            PCBox_SetBoxArrowsVisible(PTR_AT(work, 0x34), 0);
        }
        U8_AT(work, 0x26) = 0;
        U8_AT(work, 0x28) = 0;
        U8_AT(work, 0x27) = 0;
    }
    PCBox_StartFadeIn(work, msg);
}

void ov14_021EBAEC(void *work)
{
    PCBox_CreateGridInput(work, 9, 10);
    U32_AT(PTR_AT(work, 0x34), 0x43c) = U8_AT(work, 0x25) % 6;
    ov14_021E8740(work);
    PCBox_PrintMenuPrompt(work, 0, 0x27);
    if (U32_AT(PTR_AT(work, 0), 8) == 3) {
        PCBox_SetIconsTranslucent(work, 0x81, 1);
    }
    PCBox_StartFadeIn(work, 0x3d);
}

void PCBox_State_GiveItemFromBag(void *work)
{
    u32 msg;
    u32 special;
    void *entry;
    u8 *row;
    u8 pos[2];

    if (U8_AT(work, 0x21) >= 0x1e) {
        ov14_021F0B70(work);
        PCBox_SetBoxArrowsVisible(PTR_AT(work, 0x34), 0);
        PCBox_SetIconsTranslucent(work, 1, 1);
    }
    if (U32_AT(PTR_AT(work, 0), 8) != 3) {
        PCBox_CreateSelectedIconOutline(work);
    }
    if ((u32)(U32_AT(PTR_AT(work, 0), 8) - 2) <= 1) {
        ov14_021E87F4(work);
        ov14_021E82FC(PTR_AT(PTR_AT(work, 0x34), 0x2f0));
    }
    if (U16_AT(work, 0x1c) == 0) {
        if (U32_AT(PTR_AT(work, 0), 8) == 3) {
            PCBox_DrawBagGiveButton(work, 0);
            if (U8_AT(work, 0x21) < 0x1e) {
                PCBox_SetIconsTranslucent(work, 0x81, 1);
                msg = 0x75;
            } else {
                PCBox_SetIconsTranslucent(work, 0x82, 1);
                PCBox_CreateGridInput(work, 7, 8);
                msg = 0x8b;
            }
        } else {
            PCBox_DrawActionMenu(work, ov14_021F7D3C, 5);
            if (U8_AT(work, 0x21) < 0x1e) {
                msg = 0xc;
            } else {
                PCBox_CreateGridInput(work, 5, 10);
                msg = 0x24;
            }
        }
        ov14_021E83C4(PTR_AT(PTR_AT(work, 0x34), 0x2f0));
    } else {
        special = 0;
        if (U16_AT(work, 0x1c) == 0x70 &&
            PCBox_GetSlotMonData(work, U8_AT(work, 0x21), 5, 0) != 0x1e7) {
            special = 1;
        } else {
            entry = PCBox_GetSlotBoxMon(work, U8_AT(work, 0x1f), U8_AT(work, 0x21));
            PCBox_SetSlotMonData(work, U8_AT(work, 0x21), 6, (u8 *)work + 0x1c);
            if (PCBox_UpdateArceusGiratinaForm(entry) == 1) {
                row = (u8 *)PTR_AT(work, 0x34) + U8_AT(work, 0x21);
                PCBox_RefreshSlotIcon(work, U8_AT(work, 0x1f), U8_AT(work, 0x21),
                    U8_AT(row, 0x4094));
            }
            Bag_TakeItem(PTR_AT(work, 0xc), U16_AT(work, 0x1c), 1, 10);
        }
        if (U8_AT(work, 0x21) >= 0x1e) {
            if (U32_AT(PTR_AT(work, 0), 8) == 3) {
                PCBox_CreateGridInput(work, 7, U8_AT(work, 0x21) - 0x1e);
            } else {
                PCBox_CreateGridInput(work, 5, 10);
            }
        } else if (U32_AT(PTR_AT(work, 0), 8) == 3) {
            GridInputHandler_SetNextInput(PTR_AT(PTR_AT(work, 0x34), 0x2c), U8_AT(work, 0x21));
            DpadMenuBox_GetPosition(
                GridInputHandler_GetDpadBox(PTR_AT(PTR_AT(work, 0x34), 0x2c), U8_AT(work, 0x21)),
                &pos[1], &pos[0]);
            ManagedSprite_SetPositionXY(PTR_AT(PTR_AT(work, 0x34), 0x320), pos[1], pos[0]);
        }
        PCBox_SetSpriteVisible(PTR_AT(work, 0x34), 9, 0);
        if (U32_AT(PTR_AT(work, 0), 8) == 3) {
            if (special == 0) {
                U16_AT(PTR_AT(work, 0x34), 0x88c8) = U16_AT(work, 0x1c);
                PCBox_LoadHeldItemIcon(PTR_AT(work, 0x34), U16_AT(PTR_AT(work, 0x34), 0x88c8));
                PCBox_SetSpriteVisible(PTR_AT(work, 0x34), 0xb, 1);
                if (U8_AT(work, 0x21) >= 0x1e) {
                    PCBox_SetIconsTranslucent(work, 0x82, 1);
                    PCBox_MoveHeldItemIconToSlot(PTR_AT(work, 0x34), U8_AT(work, 0x21), 1);
                } else {
                    PCBox_SetIconsTranslucent(work, 0x81, 1);
                    PCBox_MoveHeldItemIconToSlot(PTR_AT(work, 0x34), U8_AT(work, 0x21), 0);
                }
                PCBox_CreateHeldItemIconOutline(PTR_AT(work, 0x34));
                PCBox_ShowHeldItemIconOutline(PTR_AT(work, 0x34));
            } else {
                U16_AT(work, 0x1c) = 0;
                if (U8_AT(work, 0x21) >= 0x1e) {
                    PCBox_SetIconsTranslucent(work, 0x82, 1);
                } else {
                    PCBox_SetIconsTranslucent(work, 0x81, 1);
                }
            }
            msg = 0x7d;
        } else {
            if (special == 1) {
                U16_AT(work, 0x1c) = 0;
            }
            msg = 0x12;
        }
    }
    PCBox_ShowSlotMonInfo(work, U8_AT(work, 0x21));
    if (U32_AT(PTR_AT(work, 0), 8) != 3) {
        ov14_021E8874(PTR_AT(work, 0x34));
    } else if (U16_AT(work, 0x1c) != 0) {
        ov14_021E892C(PTR_AT(PTR_AT(work, 0x34), 0x2f0));
    }
    PCBox_StartFadeIn(work, msg);
}
