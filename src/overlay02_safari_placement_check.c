#include "overlay02_4_landing_private.h"

WIP_LOCAL BOOL ov02_0224E35C(FieldSystem *fieldSystem) {
    void *varsFlags = Save_VarsFlags_Get(*(SaveData **)((u8 *)fieldSystem + 0xc));
    void *safariZone;
    int facing;
    int x;
    int z;
    int newX;
    int newZ;
    VecFx32 pos;
    u8 sel;
    fx32 h1;
    fx32 h2;
    f32 fx;
    f32 fz;
    int vx;
    int vz;

    LocalFieldData_GetCurrentPosition(Save_LocalFieldData_Get(*(SaveData **)((u8 *)fieldSystem + 0xc)));
    safariZone = Save_SafariZone_Get(*(SaveData **)((u8 *)fieldSystem + 0xc));
    if (ov02_0224E308(**(int **)((u8 *)fieldSystem + 0x20)) == 0) {
        return FALSE;
    }
    if (Save_VarsFlags_CheckSafariSysFlag(varsFlags) == 0) {
        return FALSE;
    }
    if (sub_0202F620(safariZone) != 0) {
        return FALSE;
    }
    if (SafariZone_GetObjectUnlockLevel(safariZone) == 0) {
        return FALSE;
    }
    facing = PlayerAvatar_GetFacingDirection(*(PlayerAvatar **)((u8 *)fieldSystem + 0x40));
    x = PlayerAvatar_GetXCoord(*(PlayerAvatar **)((u8 *)fieldSystem + 0x40));
    z = PlayerAvatar_GetZCoord(*(PlayerAvatar **)((u8 *)fieldSystem + 0x40));
    newX = x + GetDeltaXByFacingDirection(facing);
    newZ = z + GetDeltaYByFacingDirection(facing);
    if (newX < 0x20 || newX >= 0x80 || newZ < 0x20 || newZ >= 0x60) {
        return FALSE;
    }
    if (newX < 0x20 || newX >= 0x80 || newZ < 0x20 || newZ >= 0x60) {
        return FALSE;
    }
    PlayerAvatar_CopyPositionVector(*(PlayerAvatar **)((u8 *)fieldSystem + 0x40), &pos);
    h1 = sub_02054774(fieldSystem, pos.y, pos.x, pos.z, &sel);
    if (sel != 1) {
        return FALSE;
    }
    vz = (newZ << 4) + 8;
    if (vz > 0) {
        fz = (f32)(vz << 12) + 0.5f;
    } else {
        fz = (f32)(vz << 12) - 0.5f;
    }
    vx = (newX << 4) + 8;
    if (vx > 0) {
        fx = (f32)(vx << 12) + 0.5f;
    } else {
        fx = (f32)(vx << 12) - 0.5f;
    }
    h2 = sub_02054774(fieldSystem, pos.y, (s32)fx, (s32)fz, &sel);
    if (sel != 1) {
        return FALSE;
    }
    return h1 == h2;
}
