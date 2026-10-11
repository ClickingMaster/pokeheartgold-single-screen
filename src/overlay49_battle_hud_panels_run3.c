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
extern void ov49_0225E420(void *p, fx32 a, fx32 b, VecFx32 *v);
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

void ov49_022611D4(Ov49Hud *p, void *a) {
    VecFx32 v;
    WiFiPlazaObj_GetPosition(p->unk0C, &v);
    WiFiPlazaCamera_SetTarget(a, v.x, v.y, v.z);
}

BOOL ov49_022611F4(Ov49Hud *p, void *a) {
    VecFx32 pos;
    VecFx32 v;
    fx32 x;
    fx32 y;
    ov45_0222AE08(p->unk00, &x, &y);
    ov49_0225E420(a, x, y, &v);
    WiFiPlazaObj_GetPosition(p->unk0C, &pos);
    if (pos.x + 0x10000 < v.x) {
        return FALSE;
    }
    return TRUE;
}
