#include "to55_r2_t08_private.h"

void ov13_022224CC(void *a0, void *a1, int index, void *a3) {
    u8 packet[8];
    int value;

    ov13_02222978(packet, 0, sizeof(packet));
    packet[1] = 2;
    *(u16 *)(packet + 2) = ov13_02222A44(0x5790);
    value = ov13_02222A1C(_0224CF98.unk34);
    *(int *)(packet + 4) = value;
    if (index == 0xFF || (s8)ov13_0224CFB8[0x1C] == 0) {
        *(int *)(packet + 4) = -1;
    }
    ov13_022229EC(a3, a0, (int)a1, 0, packet, sizeof(packet));
}
