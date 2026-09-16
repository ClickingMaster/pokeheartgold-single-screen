#include "overlay02_port_private.h"

WIP_LOCAL BOOL ov02_02248D98(void *a0, void *obj) {
    *(UnkBlob16 *)((u8 *)obj + 0x58) = *(UnkBlob16 *)sub_02068D98(a0);
    *(Sprite **)((u8 *)obj + 0x68) = ov02_02248CAC(*(void **)((u8 *)obj + 0x64));
    return TRUE;
}

WIP_LOCAL void ov02_02248DBC(void *a0) {
    void *d = sub_02068D74(a0);
    int v6c = *(int *)((u8 *)d + 0x6c);
    if (v6c != 0) {
        sub_02068B48(v6c);
    }
    if (*(SysTask **)((u8 *)d + 0x70) != NULL) {
        ov01_021FCD78(*(SysTask **)((u8 *)d + 0x70));
    }
    sub_02068B48((int)a0);
}
