#include "overlay02_port_private.h"

WIP_LOCAL BOOL ov02_02248D98(void *a0, void *obj) {
    *(UnkBlob16 *)((u8 *)obj + 0x58) = *(UnkBlob16 *)FieldEffect_GetUserData(a0);
    *(Sprite **)((u8 *)obj + 0x68) = ov02_02248CAC(*(void **)((u8 *)obj + 0x64));
    return TRUE;
}

WIP_LOCAL void ov02_02248DBC(void *a0) {
    void *d = FieldEffect_GetState(a0);
    int v6c = *(int *)((u8 *)d + 0x6c);
    if (v6c != 0) {
        FieldEffect_Destroy(v6c);
    }
    if (*(SysTask **)((u8 *)d + 0x70) != NULL) {
        FieldCameraZoom_End(*(SysTask **)((u8 *)d + 0x70));
    }
    FieldEffect_Destroy((int)a0);
}
