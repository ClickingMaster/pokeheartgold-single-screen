#include "to50_overlay49_r59_f518_private.h"

BOOL ov49_0225F518(void *controller, void *ctx, u32 arg) {
    u8 *work;
    void *selection;
    void *object;
    void *entry;
    u32 choice;
    u32 message;
    BOOL done;

    selection = WiFiPlazaRoom_GetLobby(ctx);
    object = WiFiPlazaRoom_GetObjManager(ctx);
    entry = WiFiPlazaObj_GetOwnPlayer(object);
    work = ov49_0225EF84(controller);

    switch (ov49_0225EF88(controller)) {
    case 0:
        work = ov49_0225EF40(controller, sizeof(Ov49F518Work));
        ov49_022614CC(work, ctx);
        ov45_0222A4D0(selection);
        U16_AT(work, 2) = ov45_0222B1B4(selection);
        ov45_0222A72C(selection, U16_AT(work, 2));
        WiFiPlazaObj_SetHandler(object, ov49_0225F2FC(ctx, arg), 4);
        ov49_0225EF90(controller);
        break;
    case 1:
        if (ov49_0225A030(ctx) == 0 && WiFiPlazaObj_IsFinished(entry) == 1) {
            WiFiPlazaObj_SetHandler(object, entry, 0);
            WiFiPlazaCamera_SetFollowObject(ov49_0225A008(ctx), entry);
            U32_AT(work, 4) = 0x10;
            ov49_0225EF90(controller);
        }
        break;
    case 2:
        if (--U32_AT(work, 4) == 0) {
            ov49_0225A018(ctx, 1);
            ov49_0225EF90(controller);
        }
        break;
    case 3:
        WiFiPlazaObj_QueueCommand(object, entry, 2, 0);
        U16_AT(work, 0) = 4;
        ov49_0225EF8C(controller, 7);
        break;
    case 4:
        WiFiPlazaObj_QueueCommand(object, entry, 2, 0);
        U16_AT(work, 0) = 5;
        ov49_0225EF8C(controller, 7);
        break;
    case 5:
        WiFiPlazaObj_QueueCommand(object, entry, 1, 3);
        U16_AT(work, 0) = 6;
        ov49_0225EF8C(controller, 7);
        break;
    case 6:
        WiFiPlazaObj_QueueCommand(object, entry, 2, 3);
        U16_AT(work, 0) = 8;
        ov49_0225EF8C(controller, 7);
        break;
    case 7:
        if (WiFiPlazaObj_GetParam(entry, 5) == 0) {
            ov49_0225EF8C(controller, U16_AT(work, 0));
        }
        break;
    case 8:
        WiFiPlazaRoom_PrintMessage(ctx, WiFiPlazaRoom_GetMsgString(ctx, 1, 0));
        U16_AT(work, 0) = 0x10;
        ov49_0225EF8C(controller, 0x15);
        break;
    case 9:
        ov49_0225A09C(ctx, WiFiPlazaRoom_GetMsgString(ctx, 1, 5));
        ov49_0225A174(ctx, work + 8, 0, 0);
        ov49_0225A1F4(ctx, 1);
        ov49_0225EF8C(controller, 0xA);
        break;
    case 10:
        done = FALSE;
        choice = ov49_0225A1D4(ctx);
        switch (choice) {
        case (u32)-2:
            PlaySE(0x5DC);
        case 0:
            U16_AT(work, 0x28) = 0;
            done = TRUE;
            break;
        case (u32)-1:
            break;
        default:
            U16_AT(work, 0x28) = choice;
            done = TRUE;
            ov49_0225A40C(ctx, 0, choice);
            break;
        }
        if (done == TRUE) {
            ov49_0225A1E4(ctx, 0, 0);
            if (U16_AT(work, 0x28) != 0) {
                ov49_0225A09C(ctx, WiFiPlazaRoom_GetMsgString(ctx, 1, 8));
                WiFiPlazaRoom_OpenYesNoMenu(ctx);
                ov49_0225EF8C(controller, 0xB);
            } else {
                ov49_0225EF8C(controller, 0x11);
            }
        }
        break;
    case 11:
        switch (WiFiPlazaRoom_GetYesNoResult(ctx)) {
        case 0:
            WiFiPlazaRoom_CloseYesNoMenu(ctx);
            ov49_0225EF8C(controller, 0xC);
            ov45_0222A770(selection, U16_AT(work, 0x28), U16_AT(work, 0x2A));
            ov49_0225A490(ctx);
            PlaySE(0x5E5);
            break;
        case 1:
            WiFiPlazaRoom_CloseYesNoMenu(ctx);
            ov49_0225EF8C(controller, 9);
            break;
        }
        break;
    case 12:
        ov49_0225A09C(ctx, WiFiPlazaRoom_GetMsgString(ctx, 1, 0xA));
        ov49_0225A174(ctx, work + 8, 0, 0);
        ov49_0225A1F4(ctx, 1);
        ov49_0225EF8C(controller, 0xD);
        break;
    case 13:
        done = FALSE;
        choice = ov49_0225A1D4(ctx);
        switch (choice) {
        case (u32)-2:
            PlaySE(0x5DC);
        case 0:
            U16_AT(work, 0x2A) = 0;
            done = TRUE;
            break;
        case (u32)-1:
            break;
        default:
            U16_AT(work, 0x2A) = choice;
            done = TRUE;
            ov49_0225A40C(ctx, 0, choice);
            break;
        }
        if (done == TRUE) {
            ov49_0225A1E4(ctx, 0, 0);
            if (U16_AT(work, 0x2A) != 0) {
                ov49_0225A09C(ctx, WiFiPlazaRoom_GetMsgString(ctx, 1, 8));
                WiFiPlazaRoom_OpenYesNoMenu(ctx);
                ov49_0225EF8C(controller, 0xF);
            } else {
                ov49_0225EF8C(controller, 0x13);
            }
        }
        break;
    case 15:
        switch (WiFiPlazaRoom_GetYesNoResult(ctx)) {
        case 0:
            WiFiPlazaRoom_CloseYesNoMenu(ctx);
            ov49_0225EF8C(controller, 0x14);
            ov45_0222A770(selection, U16_AT(work, 0x28), U16_AT(work, 0x2A));
            ov49_0225A490(ctx);
            PlaySE(0x5E5);
            break;
        case 1:
            WiFiPlazaRoom_CloseYesNoMenu(ctx);
            ov49_0225EF8C(controller, 0xC);
            break;
        }
        break;
    case 16:
        PlaySE(0x5BF);
        ov49_0225A530(ctx);
        ov49_0225A334(ctx, arg, 0);
        ov49_0225A39C(ctx, U16_AT(work, 2), 1);
        if (ov45_0222AB28(selection, arg) == 0) {
            message = WiFiPlazaRoom_GetMsgString(ctx, 1, 1);
        } else {
            message = WiFiPlazaRoom_GetMsgString(ctx, 1, 0x7C);
        }
        WiFiPlazaRoom_PrintMessage(ctx, message);
        U16_AT(work, 0) = 0x12;
        ov49_0225EF8C(controller, 0x15);
        break;
    case 17:
        WiFiPlazaRoom_PrintMessage(ctx, WiFiPlazaRoom_GetMsgString(ctx, 1, 2));
        U16_AT(work, 0) = 0x16;
        ov49_0225EF8C(controller, 0x15);
        break;
    case 18:
        if (!IsSEPlaying(0x5BF) && (gSystem.newKeys & 3) != 0) {
            PlaySE(0x5DC);
            WiFiPlazaRoom_PrintMessage(ctx, WiFiPlazaRoom_GetMsgString(ctx, 1, 4));
            U16_AT(work, 0) = 9;
            ov49_0225EF8C(controller, 0x15);
            ov45_0222A770(selection, U16_AT(work, 0x28), U16_AT(work, 0x2A));
            ov49_0225A428(ctx, arg, 0);
        }
        break;
    case 19:
        ov49_0225A40C(ctx, 0, U16_AT(work, 0x28));
        WiFiPlazaRoom_PrintMessage(ctx, WiFiPlazaRoom_GetMsgString(ctx, 1, 0xC));
        U16_AT(work, 0) = 0x11;
        ov49_0225EF8C(controller, 0x15);
        break;
    case 20:
        ov49_0225A40C(ctx, 0, U16_AT(work, 0x28));
        ov49_0225A40C(ctx, 1, U16_AT(work, 0x2A));
        WiFiPlazaRoom_PrintMessage(ctx, WiFiPlazaRoom_GetMsgString(ctx, 1, 0xB));
        U16_AT(work, 0) = 0x11;
        ov49_0225EF8C(controller, 0x15);
        break;
    case 21:
        if (WiFiPlazaRoom_IsMessageFinished(ctx) == 1) {
            ov49_0225EF8C(controller, U16_AT(work, 0));
        }
        break;
    case 22:
        WiFiPlazaObj_SetHandler(object, entry, 1);
        WiFiPlazaRoom_CloseMessage(ctx);
        ov49_02261540(work, ctx);
        ov49_0225EF68(controller);
        ov49_0225EF98(ov49_0225A010(ctx), ov49_0225A02C(ctx), ov49_02269B38, 0);
        break;
    }
    return FALSE;
}
