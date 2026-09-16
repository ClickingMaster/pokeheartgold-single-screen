#include "field_follow_shadow_slop_r1.h"

void ov01_021FD458(FollowShadow *work) {
    FieldSystem *fieldSystem = (FieldSystem *)ov01_021F1468(work->unk20);
    u32 priority = MapObjectManager_GetPriority(fieldSystem->mapObjectManager) - 1;
    work->unk24 = SysTask_CreateOnMainQueue((SysTaskFunc)ov01_021FD4F4, work, priority);
}
