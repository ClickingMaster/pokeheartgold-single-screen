#include "overlay49_battle_hud_state_r59_private.h"

BOOL ov49_02260254(void *controller, void *work, u32 arg) {
    void *object;
    void *entry;
    int flag;

    switch (ov49_0225EF88(controller)) {
    case 0:
        object = WiFiPlazaRoom_GetObjManager(work);
        entry = WiFiPlazaObj_GetOwnPlayer(object);
        WiFiPlazaObj_SetHandler(object, (void*)(entry), 0);
        ov45_0222A5E8(WiFiPlazaRoom_GetLobby(work), 0xB);
        PlaySE(0x5DD);
        WiFiPlazaRoom_PrintMessage(work, WiFiPlazaRoom_GetMsgString(work, 1, 0x41));
        ov49_0225EF90(controller);
        break;
    case 1:
        if (WiFiPlazaRoom_IsMessageFinished(work) == 1) {
            ov49_0225EF90(controller);
        }
        break;
    case 2:
        WiFiPlazaRoom_PrintMessage(work, WiFiPlazaRoom_GetMsgString(work, 2, 0x19));
        ov49_0225EF90(controller);
        break;
    case 3:
        if (WiFiPlazaRoom_IsMessageFinished(work) == 1) {
            ov49_0225EF90(controller);
        }
        break;
    case 4:
        ov49_0225A294(work);
        ov49_0225EF90(controller);
        break;
    case 5:
        flag = 0;
        switch (WiFiPlazaRoom_GetYesNoResult(work)) {
        case 0:
            ov49_0225EF8C(controller, 6);
            flag = 1;
            break;
        case 1:
            ov49_0225EF8C(controller, 8);
            flag = 1;
            break;
        case 2:
            break;
        }
        if (flag == 1) {
            WiFiPlazaRoom_CloseYesNoMenu(work);
            WiFiPlazaRoom_CloseMessage(work);
        }
        break;
    case 6:
        ov49_0225A018(work, 0);
        object = WiFiPlazaRoom_GetObjManager(work);
        entry = WiFiPlazaObj_GetOwnPlayer(object);
        WiFiPlazaCamera_ClearFollowObject(ov49_0225A008(work));
        WiFiPlazaObj_SetHandler(object, (void*)(entry), 3);
        ov49_0225EF8C(controller, 7);
        break;
    case 7:
        if (WiFiPlazaObj_IsFinished(WiFiPlazaObj_GetOwnPlayer(WiFiPlazaRoom_GetObjManager(work))) == 1) {
            ov49_0225A034(work, 1);
            ov49_0225A038(work, 0);
            ov45_0222A5E8(WiFiPlazaRoom_GetLobby(work), 0xB);
            return TRUE;
        }
        break;
    case 8:
        object = WiFiPlazaRoom_GetObjManager(work);
        entry = WiFiPlazaObj_GetOwnPlayer(object);
        ov49_02258EAC(object, entry, 2, 0);
        ov49_0225EF90(controller);
        break;
    case 9:
        ov45_0222A5E8(WiFiPlazaRoom_GetLobby(work), 1);
        object = WiFiPlazaRoom_GetObjManager(work);
        controller = ov49_0225A010(work);
        entry = WiFiPlazaObj_GetOwnPlayer(object);
        if (WiFiPlazaObj_GetParam(entry, 5) == 0) {
            WiFiPlazaObj_SetHandler(object, (void*)(entry), 1);
            ov49_0225EF98(controller, arg, ov49_02269B38, 0);
        }
        break;
    }
    return FALSE;
}
