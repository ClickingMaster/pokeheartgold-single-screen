#include "overlay49_sol_r9_60_private.h"

BOOL ov49_022618C0(void *state, void *ctx, u32 index) {
    void *object = WiFiPlazaRoom_GetObjManager(ctx);
    void *target = WiFiPlazaObj_FindById(object, index);

    switch (ov49_0225EF88(state)) {
    case 0:
        WiFiPlazaObj_SetHandler(object, (void*)((u32)target), 3);
        ov49_0225EF90(state);
        break;
    case 1:
        if (WiFiPlazaObj_IsFinished(target) != 0) {
            ov49_0225EF90(state);
        }
        break;
    case 2:
        WiFiPlazaObj_Delete(target);
        ov49_0225EF98(ov49_0225A010(ctx), index, ov49_02269B78, 0);
        break;
    }
    return FALSE;
}
