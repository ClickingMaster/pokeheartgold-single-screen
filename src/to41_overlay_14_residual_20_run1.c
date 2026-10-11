#include "to41_overlay_14_residual_20_private.h"

u32 PCBox_UpdateBoxMenuSlide(void *p);
u32 ov14_021E9660(void *p);
void ov14_021E96A8(void *p);
u32 ov14_021E98AC(void *p);
u32 ov14_021E9920(void *p);
u32 ov14_021E9970(void *p);
u32 ov14_021E99A0(void *p);
u32 ov14_021E99F0(void *p);
u32 PCBox_RunStateMachine(void *p, u32 *state);

u32 PCBox_State_WaitFade(void *p);
u32 ov14_021EA068(void *p);
u32 ov14_021EA130(void *p);
u32 ov14_021EA728(void *p);
void ov14_021EA1F0(void *p);
u32 ov14_021EA0B8(void *p);
u32 ov14_021EA180(void *p);
u32 ov14_021EA378(void *p);
u32 ov14_021EAF08(void *p);
u32 PCBox_State_Exit(void *p);
u32 ov14_021EA674(void *p);

u32 ov14_021EA408(void *p);
u32 ov14_021EAC24(void *p);
u32 ov14_021EAB54(void *p);

u32 PCBox_UpdateBoxMenuSlide(void *p) {
    u32 a = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xc);
    u32 b = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 6);
    u32 c = BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 7);
    if (a == 0 && b == 0 && c == 0) {
        return 0;
    }
    return 1;
}

u32 ov14_021E9660(void *p) {
    void *q = PTR_AT(p, 0x34);
    switch (U16_AT(q, 0x10)) {
    case 0:
        if (PCBox_UpdateBoxMenuSlide(p) == 0) {
            U16_AT(q, 0x10)
            ++;
        }
        break;
    case 1:
        PCBox_MoveWallpaperMenuSprites(q, 8);
        if (BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xd) == 0) {
            U16_AT(q, 0x10) = 0;
            return 0;
        }
        break;
    }
    return 1;
}

void ov14_021E96A8(void *p) {
    PCBox_MoveWallpaperMenuSprites(PTR_AT(p, 0x34), ~7);
    BgCellManager_UpdateCellMove(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 0xd);
}
