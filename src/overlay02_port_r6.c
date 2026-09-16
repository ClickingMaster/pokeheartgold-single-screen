#include "overlay02_port_private.h"

WIP_LOCAL void *ov02_02249458(FieldSystem *fieldSystem, int a1, Pokemon *a2, int a3) {
    void *work = ov02_0224955C(fieldSystem);
    *(void **)((u8 *)work + 0x5c) = a2;
    *(u16 *)((u8 *)work + 0xc) = a3;
    *(u16 *)((u8 *)work + 0xe) = *(u16 *)((u8 *)work + 0xc);
    *(int *)((u8 *)work + 0x20) = a1;
    *(LocalMapObject **)((u8 *)work + 0x208) = PlayerAvatar_GetMapObject((*(FieldSystem **)((u8 *)work + 0x60))->playerAvatar);
    if (a1 == 0) {
        return SysTask_CreateOnMainQueue(ov02_02249584, work, 0x86);
    } else if (a1 == 2) {
        *(LocalMapObject **)((u8 *)work + 0x20c) = FollowMon_GetMapObject(fieldSystem);
        return SysTask_CreateOnMainQueue(ov02_022499B8, work, 0x86);
    } else {
        return SysTask_CreateOnMainQueue(ov02_02249984, work, 0x86);
    }
}

WIP_LOCAL void ov02_022494C4(FieldSystem *fieldSystem, void *a1, void *a2, void *a3) {
    VecFx32 vec1;
    VecFx32 vec2;
    void *work = ov02_0224955C(fieldSystem);
    *(void **)((u8 *)work + 0x5c) = a1;
    *(u16 *)((u8 *)work + 0xc) = 0;
    *(u16 *)((u8 *)work + 0xe) = 2;
    *(int *)((u8 *)work + 0x20) = 3;
    *(void **)((u8 *)work + 0x208) = a2;
    *(void **)((u8 *)work + 0x20c) = a3;
    MapObject_CopyPositionVector(PlayerAvatar_GetMapObject((*(FieldSystem **)((u8 *)work + 0x60))->playerAvatar), &vec1);
    MapObject_CopyPositionVector(*(LocalMapObject **)((u8 *)work + 0x208), &vec2);
    *(fx32 *)((u8 *)work + 0x2ec) = *(fx32 *)((u8 *)work + 0x2ec) + FX_Div(vec2.x - vec1.x, 0x2000);
    *(fx32 *)((u8 *)work + 0x2f4) = vec2.z - vec1.z;
    SysTask_CreateOnMainQueue(ov02_022499B8, work, 0x86);
}
