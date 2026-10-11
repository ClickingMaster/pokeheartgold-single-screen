#include "overlay14_scene_internal.h"

void ov14_021E8020(void *p) {
    BgCellManager_StartCellMove(p, 1, 1, 0, 0x13);
    BgCellManager_StartCellMove(p, 10, 1, 0, 0x13);
}

void ov14_021E8048(void *p) {
    BgCellManager_StartCellMove(p, 1, 1, 0, 0x13);
}

void ov14_021E805C(void *p) {
    BgCellManager_StartCellMove(p, 1, -1, 0, 0x13);
    BgCellManager_DrawCellAt(p, 10, 0x2B, 0x15);
    BgCellManager_StartCellMove(p, 10, -1, 0, 0x13);
}

void ov14_021E8094(void *p) {
    BgCellManager_StartCellMove(p, 1, -1, 0, 0x13);
}

u32 ov14_021E80A8(void *p) {
    s8 a[4];
    u32 x, y;
    BgCellManager_GetCellPosition(PP(PP(p, 0x34), 0x2F0), 1, &a[3], &a[2]);
    x = BgCellManager_UpdateCellMove(PP(PP(p, 0x34), 0x2F0), 1);
    y = BgCellManager_UpdateCellMove(PP(PP(p, 0x34), 0x2F0), 10);
    BgCellManager_UpdateCellMove(PP(PP(p, 0x34), 0x2F0), 15);
    BgCellManager_GetCellPosition(PP(PP(p, 0x34), 0x2F0), 1, &a[1], &a[0]);
    if (a[3] != a[1] || a[2] != a[0]) {
        ov14_021F32E0(p);
    }
    if (x == 0 && y == 0) {
        return 0;
    }
    return 1;
}

u32 ov14_021E813C(void *p) {
    s8 a[4];
    u32 x;
    BgCellManager_GetCellPosition(PP(PP(p, 0x34), 0x2F0), 1, &a[3], &a[2]);
    x = BgCellManager_UpdateCellMove(PP(PP(p, 0x34), 0x2F0), 1);
    BgCellManager_GetCellPosition(PP(PP(p, 0x34), 0x2F0), 1, &a[1], &a[0]);
    if (a[3] != a[1] || a[2] != a[0]) {
        ov14_021F32E0(p);
    }
    if (x != 0) {
        return 1;
    }
    return 0;
}
