#include "to56_battle_residual12_private.h"
void ov12_0225B3D4(void *arg0, const u8 *battle, const u8 *data) {
    int pan;
    if (battle[0x195] & 1) {
        pan = 0x75;
    } else {
        pan = -0x75;
    }
    switch (data[1]) {
    case 0:
        sub_0200602C(0x6FE, pan);
        break;
    case 2:
        sub_0200602C(0x6FC, pan);
        break;
    case 1:
        sub_0200602C(0x6FD, pan);
        break;
    }
    ov12_0226430C(arg0, battle[0x194], data[0]);
}
