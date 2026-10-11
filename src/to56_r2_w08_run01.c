#include "global.h"

#include "to56_r2_w08_private.h"

int ov74_02229F90(void *manager) {
    WorkF90 *work = OverlayManager_GetData(manager);
    if (work->field15D4 != 2) {
        ov74_0222AA18(manager, (u8 *)work + 0x18, 3);
    }
    ov74_02229F60((u8 *)work + 0x48, 0);
    if (work->field15D4 != 2) {
        ov74_02229D1C(work, 15);
        work->field434 = 0x1c20;
        WirelessStatusIcon_Create();
        ov74_02229E28(work, 1);
        return 7;
    }
    work->field84 = 0x13;
    return 0x13;
}

int ov74_02229FF4(void *manager) {
    WorkF90 *work = OverlayManager_GetData(manager);
    int state = work->field80;
    if (state == 1) {
        return 0x31;
    }
    if ((u32)(state - 2) <= 2) {
        return 0x31;
    }
    if (state == 5) {
        return 0x31;
    }
    if (work->field15D4 == 1) {
        ov74_02229D50(work->field438);
    }
    ov74_0222AA18(manager, (u8 *)work + 0x18, 6);
    ov74_02229F60((u8 *)work + 0x48, 0);
    if (work->field15D4 == 1) {
        ov74_02229F28(work, 0);
        ov74_02229E28(work, 1);
    }
    if (work->field15D4 == 1) {
        sub_02037AC0(0xab);
        return 9;
    }
    return 0x15;
}
