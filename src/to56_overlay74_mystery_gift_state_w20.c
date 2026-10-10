#include "to56_overlay74_mystery_gift_state_w20_private.h"

int ov74_0222E1F4(OverlayManager *arg0, int *arg1) {
    void (*temp_r1_2)(void *);
    int temp_r1;
    Ov74E1F4Work *temp_r4;

    temp_r4 = OverlayManager_GetData(arg0);
    temp_r1 = *arg1;
    switch (temp_r1) { /* irregular */
    case 0:
        temp_r4->saveData = M2C_FIELD(OverlayManager_GetArgs(arg0), void **, 8);
        temp_r4->mysteryGift = Save_MysteryGift_Get(temp_r4->saveData);
        temp_r4->options = Save_PlayerData_GetOptionsAddr(temp_r4->saveData);
        temp_r4->frame = Options_GetFrame(temp_r4->options);
        temp_r4->cards[0] = SaveMysteryGift_CardGetByIdx(temp_r4->mysteryGift, 0);
        temp_r4->cards[1] = SaveMysteryGift_CardGetByIdx(temp_r4->mysteryGift, 1);
        temp_r4->cards[2] = SaveMysteryGift_CardGetByIdx(temp_r4->mysteryGift, 2);
        temp_r4->index = ov74_0222DAF8(temp_r4, temp_r4->index, 1);
        *arg1 = 1;
        break;
    case 1:
        ov74_0222FCA4();
        ov74_0222FCC4(temp_r4->bgConfig);
        *arg1 = 2;
        break;
    case 2:
        ResetAllTextPrinters();
        ov74_0222DB70(temp_r4, 0);
        LoadFontPal0(0, 0x1E0, 0x55);
        LoadUserFrameGfx1(temp_r4->bgConfig, 0, 1, 0xD, 0, 0x55);
        LoadUserFrameGfx1(temp_r4->bgConfig, 0, 0xA, 0xE, 1, 0x55);
        LoadUserFrameGfx2(temp_r4->bgConfig, 0, 0x13, 0xA, (u8)M2C_FIELD(temp_r4, u32 *, 0x2BAC), 0x55);
        ov74_0222D824(temp_r4, 1, 0);
        ov74_0222D7F0(temp_r4, 1, 3, arg1);
        break;
    case 3: {
        s32 var_r6;
        var_r6 = temp_r4->index;
        if (gSystem.newKeys & 0x40) {
            var_r6 = ov74_0222DAF8(temp_r4, var_r6, -1);
        } else if (0x80 & gSystem.newKeys) {
            var_r6 = ov74_0222DAF8(temp_r4, var_r6, 1);
        } else if (2 & gSystem.newKeys) {
            PlaySE(SEQ_SE_DP_SELECT);
            ov74_0222D7F0(temp_r4, 0, 0x1D, arg1);
        } else if (1 & gSystem.newKeys) {
            PlaySE(SEQ_SE_DP_SELECT);
            *arg1 = 4;
        }
        if (temp_r4->index == var_r6) {

        } else {
            PlaySE(SEQ_SE_DP_CARD2);
            temp_r4->index = var_r6;
            ov74_0222D824(temp_r4, 1, 0);
        }
        break;
    }
    case 4:
        temp_r4->printer = ov74_0222D9E0(temp_r4, temp_r4->window2BD0, 7, 0x280);
        ov74_0222D9E0(temp_r4, temp_r4->window2BE0, 8, temp_r4->printer);
        *arg1 = 5;
        break;
    case 5:
        ov74_0222DC60(arg0, arg1, NULL);
        if (2 & gSystem.newKeys) {
            PlaySE(SEQ_SE_DP_SELECT);
            *arg1 = 0xB;
        }
        break;
    case 6:
        PlaySE(SEQ_SE_DP_CARD2);
        GfGfx_EngineATogglePlanes(0x10, 0);
        ov74_0222D104(temp_r4->window2BD0, 0);
        ov74_0222D0EC(temp_r4->window2BE0, 0);
        ov74_0222DE8C(temp_r4, 1, 0x1000, 0x66U);
        *arg1 = 7;
        break;
    case 7:
        if (ov74_0222DF2C() != 0) {
            ov74_0222D824(temp_r4, 1, 1);
            ov74_0222DE8C(temp_r4, 0, 0x708000, 0x708000U >> 1);
            *arg1 = 8;
        }
        break;
    case 8:
        ov74_0222DF2C();
        if (3 & gSystem.newKeys) {
            PlaySE(SEQ_SE_DP_CARD2);
            ov74_0222DE8C(temp_r4, 1, 0x1000, 0x66U);
            *arg1 = 9;
        }
        break;
    case 9:
        if (ov74_0222DF2C() != 0) {
            ov74_0222D824(temp_r4, 1, 0);
            ov74_0222DE8C(temp_r4, 0, 0x708000, 0x708000U >> 1);
            GfGfx_EngineATogglePlanes(0x10, 0);
            *arg1 = 0xA;
        }
        break;
    case 10:
        if (ov74_0222DF2C() == 0) {

        } else {
            DrawFrameAndWindow2(temp_r4->window2BD0, 0, 0x13, 0xA);
            DrawFrameAndWindow1(temp_r4->window2BE0, 0, 0xA, 0xE);
            GfGfx_EngineATogglePlanes(0x10, 1);
            ov74_0222DEF0(temp_r4);
            *arg1 = 5;
        }
        break;
    case 11:
        ov74_0222D448();
        ov74_0222D104(temp_r4->window2BD0, 0);
        ClearWindowTilemapAndCopyToVram(temp_r4->window2BD0);
        RemoveWindow(temp_r4->window2BD0);
        *arg1 = 3;
        break;
    case 12:
        ov74_0222DC60(arg0, arg1, ov74_0222D414);
        break;
    case 14:
        ov74_0222DC60(arg0, arg1, ov74_0222D414);
        break;
    case 15:
        ov74_0222D7A4(temp_r4, temp_r4->window2BE0, 0x10200);
        *arg1 = 0xE;
        break;
    case 16:
        ov74_0222D7F0(temp_r4, 0, 0x11, arg1);
        break;
    case 17:
        ov74_0222D448();
        ov74_0222D104(temp_r4->window2BD0, 0);
        ClearWindowTilemapAndCopyToVram(temp_r4->window2BD0);
        RemoveWindow(temp_r4->window2BD0);
        ov74_0222D824(temp_r4, 0, 3);
        ov74_0222D248(temp_r4->bgConfig);
        ov74_0222CFFC(temp_r4);
        ov74_0222D098(temp_r4);
        *arg1 = 0x12;
        break;
    case 18:
        ov74_0222E7EC();
        sub_0203A880();
        ov74_0222D7F0(temp_r4, 1, 0x13, arg1);
        break;
    case 19: {
        s32 var_r6;
        var_r6 = 0;
        if (sub_020373B4(0) != 0) {
            sub_020358B8((u8 *)temp_r4 + 0x90);
            var_r6 = ov74_0222E8B4(temp_r4, temp_r4->window2B08);
            if ((var_r6 == 0) && (temp_r4->shown != 0)) {
                FillWindowPixelBuffer(temp_r4->window2B08, 0);
                CopyWindowToVram(temp_r4->window2B08);
                temp_r4->shown = 0;
            }
            ov74_0222EA88(temp_r4, temp_r4->window2AF8, var_r6);
            temp_r4->shown = var_r6;
        }
        ov74_0222E0D4(temp_r4, var_r6, arg1);
        break;
    }
    case 20:
        ov74_0222D7F0(temp_r4, 0, 0x15, arg1);
        break;
    case 21:
        ov74_0222D024();
        ov74_0222D104(temp_r4->window2BD0, 0);
        ov74_0222D0EC(temp_r4->window2BE0, 0);
        ov74_0222D824(temp_r4, 1, 0);
        LoadFontPal0(0, 0x1E0, 0x55);
        ov74_0222D7F0(temp_r4, 1, 4, arg1);
        break;
    case 23:
        if (sub_02037D78() != 0) {

        } else {
            *arg1 = temp_r4->nextState;
        }
        break;
    case 22:
        temp_r4->timer = (s32)(temp_r4->timer - 1);
        if (temp_r4->timer == 0) {
            ov74_02235ED0((u8 *)temp_r4 + 0x90, temp_r4->buffer2DD8, *(s32 *)temp_r4);
            ov74_02229D0C(temp_r4->buffer2DD8, 0x358);
            ov74_0222E060(temp_r4);
            *arg1 = 0x18;
        }
        break;
    case 24:
        if (ov74_0223615C() == 4) {
            sub_02037AC0(0x93);
            sub_020398D4(1, 1);
            *arg1 = 0x19;
        }
        break;
    case 25:
        if ((ov74_0222E898() == 0) || (sub_02037B38(0x93) == 1)) {
            sub_020398D4(0, 0);
            ov74_02236128();
            ov74_0222D9E0(temp_r4, temp_r4->window2BD0, 0x12, 0x280);
            WaitingIcon_Delete(temp_r4->waitingIcon);
            ov74_0222EB28(temp_r4, arg1, 0x1A);
        }
        break;
    case 26:
        if (3 & gSystem.newKeys) {
            *arg1 = 0x14;
        }
        break;
    case 13:
        if (3 & gSystem.newKeys) {
            ov74_0222D104(temp_r4->window2BD0, 0);
            ClearWindowTilemapAndCopyToVram(temp_r4->window2BD0);
            RemoveWindow(temp_r4->window2BD0);
            *arg1 = 3;
        }
        break;
    case 27:
        ov74_0222D7F0(temp_r4, 0, 0x1D, arg1);
        break;
    case 28:
        if (IsPaletteFadeFinished() != 0) {
            *arg1 = temp_r4->nextState;
        }
        break;
    case 29:
        ov74_0222D024();
        ov74_0222DEF0(temp_r4);
        return 1;
    case 30:
        break;
    }
    if (temp_r4->renderSprites != 0) {
        SpriteList_RenderAndAnimateSprites();
    }
    ov74_022358BC();
    temp_r1_2 = temp_r4->callback;
    if (temp_r1_2 != NULL) {
        temp_r1_2(temp_r4);
    }
    return 0;
}
