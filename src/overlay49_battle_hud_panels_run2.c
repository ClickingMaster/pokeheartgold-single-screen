#include "overlay49_battle_hud_panels_private.h"

#pragma require_prototypes off

extern void GF_AssertFail(void);

extern u32 WiFiPlazaRoom_GetObjManager(void *p);
extern u32 WiFiPlazaObj_GetOwnPlayer(void *p);
extern u32 WiFiPlazaObj_GetParam(void *p, u32 a);
extern void ov49_0225A53C(void *p, u32 a);
extern void WiFiPlazaObj_GetPosition(void *p, VecFx32 *v);
extern void WiFiPlazaObj_SetPosition(void *p, const VecFx32 *v);
extern void WiFiPlazaCamera_SetTarget(void *p, u32 a, u32 b, u32 c);
extern void ov45_0222AE08(void *p, fx32 *a, fx32 *b);
extern void WiFiPlazaModels_GetMoverFrontPos(void *p, fx32 a, fx32 b, VecFx32 *v);
extern u32 ov49_0225EF88(void *p);
extern void ov49_0225EF90(void *p);
extern u32 WiFiPlazaRoom_GetLobby(void *p);
extern u32 ov45_0222A330(void *p);
extern u32 ov45_0222A374(void *p);
extern u32 WiFiPlazaRoom_GetMsgString(void *p, u32 a, u32 b);
extern void WiFiPlazaRoom_PrintMessage(void *p, u32 a);
extern void PlaySE(u32 id);
extern u32 WiFiPlazaRoom_IsMessageFinished(void *p);
extern void WiFiPlazaRoom_CloseMessage(void *p);
extern void WiFiPlazaObj_SetHandler(void *, void *, int);
extern u32 ov49_0225A520(void *p, fx32 a);
extern BOOL ov49_0225F438(void *p);
extern void ov49_0225F374(void *p);
extern void ov49_0225F430(void *p);
extern fx32 ov49_0225F394(void *p);
extern void ov49_0225A10C(void *p, u32 n);
extern void ov49_0225A40C(void *p, u32 a, u32 b);
extern void ov49_0225A144(void *p, u32 a, u32 b);
extern u32 ov49_0225A154(void *p);
extern const Ov49Blk ov49_02269C00;

u32 ov49_02260C58(u32 a) {
    switch (a) {
    case 22:
    case 23:
    case 24:
    case 25:
        return 2;
    case 26:
    case 27:
    case 28:
    case 29:
        return 3;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        return 0;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        return 1;
    }
    GF_AssertFail();
    return 0;
}

u32 ov49_02260CC0(u32 a) {
    switch (a) {
    case 22:
    case 23:
    case 24:
    case 25:
        return 1;
    case 26:
    case 27:
    case 28:
    case 29:
        return 2;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        return 6;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        return 7;
    }
    GF_AssertFail();
    return 0;
}

u32 ov49_02260D28(u32 a) {
    switch (a) {
    case 22:
    case 23:
    case 24:
    case 25:
        return 7;
    case 26:
    case 27:
    case 28:
    case 29:
        return 8;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        return 5;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
        return 6;
    }
    GF_AssertFail();
    return 0;
}

BOOL ov49_02260D90(void *a, void *b) {
    u32 v;
    void *t;
    switch (ov49_0225EF88(a)) {
    case 0:
        t = (void *)WiFiPlazaRoom_GetLobby(b);
        if (ov45_0222A330(t) == 1) {
            v = WiFiPlazaRoom_GetMsgString(b, 1, 3);
        } else if (ov45_0222A374(t) == 1) {
            v = WiFiPlazaRoom_GetMsgString(b, 1, 0x4E);
        } else {
            v = WiFiPlazaRoom_GetMsgString(b, 1, 2);
        }
        WiFiPlazaRoom_PrintMessage(b, v);
        PlaySE(0x5DC);
        ov49_0225EF90(a);
        break;
    case 1:
        if (WiFiPlazaRoom_IsMessageFinished(b)) {
            WiFiPlazaRoom_CloseMessage(b);
            t = (void *)WiFiPlazaRoom_GetObjManager(b);
            WiFiPlazaObj_SetHandler(t, (void*)(WiFiPlazaObj_GetOwnPlayer(t)), 1);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
