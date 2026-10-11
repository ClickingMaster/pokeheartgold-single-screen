#include "r40_overlay_49_residual_62_gap_1_private.h"

typedef struct Ov49R62Status {
    u8 value0;
    u8 value1;
    u8 value2;
    u8 selection;
    s16 value4;
    u8 enabled;
    u8 flags;
} Ov49R62Status;

BOOL ov49_022649F4(void *state, void *ctx) {
    void *object;
    void *message;

    ov49_0225EF84(state);
    object = WiFiPlazaRoom_GetLobby(ctx);
    switch (ov49_0225EF88(state)) {
    case 0:
        PlaySE(0x5DC);
        if (ov45_0222A330(object) == 0) {
            if (ov45_0222A374(object) == 1) {
                message = WiFiPlazaRoom_GetMsgString(ctx, 1, 0x4E);
                WiFiPlazaRoom_PrintMessage(ctx, message);
                ov49_0225EF8C(state, 1);
                goto done;
            }
        } else {
            message = WiFiPlazaRoom_GetMsgString(ctx, 1, 3);
            WiFiPlazaRoom_PrintMessage(ctx, message);
            ov49_0225EF8C(state, 1);
            goto done;
        }
        switch (ov45_0222B134(object)) {
        case 0:
            ov49_0225A37C(ctx, 0, 0);
            message = WiFiPlazaRoom_GetMsgString(ctx, 1, 0x49);
            break;
        case 1:
            ov49_0225A37C(ctx, 1, 0);
            message = WiFiPlazaRoom_GetMsgString(ctx, 1, 0x49);
            break;
        case 2:
            ov49_0225A37C(ctx, 2, 0);
            message = WiFiPlazaRoom_GetMsgString(ctx, 1, 0x49);
            break;
        case 3:
            ov49_0225A37C(ctx, 5, 0);
            message = WiFiPlazaRoom_GetMsgString(ctx, 1, 0x4D);
            break;
        case 4:
            ov49_0225A37C(ctx, 6, 0);
            message = WiFiPlazaRoom_GetMsgString(ctx, 1, 0x4D);
            break;
        case 5:
            message = WiFiPlazaRoom_GetMsgString(ctx, 1, 0x4A);
            break;
        case 6:
            message = WiFiPlazaRoom_GetMsgString(ctx, 1, 0x4F);
            break;
        case 7:
            message = WiFiPlazaRoom_GetMsgString(ctx, 1, 0x4B);
            break;
        case 8:
        default:
            message = WiFiPlazaRoom_GetMsgString(ctx, 1, 0x4C);
            break;
        }
        WiFiPlazaRoom_PrintMessage(ctx, message);
        ov49_0225EF8C(state, 1);
        break;
    case 1:
        if (WiFiPlazaRoom_IsMessageFinished(ctx)) {
            ov49_0225EF8C(state, 2);
        }
        break;
    case 2:
        WiFiPlazaRoom_CloseMessage(ctx);
        message = WiFiPlazaRoom_GetObjManager(ctx);
        object = ov49_02258DAC(message);
        ov49_02258EEC(message, object, 1);
        return TRUE;
    }
done:
    return FALSE;
}
