#include "to50_communication_packet_builders_private.h"

int sub_02036E60(u8 *data) {
    ReceiveBuffer receive;
    int count = sub_0203772C((u16)sub_0203993C());

    sub_02033FC4((u16)sub_0203993C());
    if (_021D4140.work[0x6B4] == 0) {
        *data = 0;
    } else {
        *data = 1;
    }
    if (sub_02035FF0() == 0) {
        sub_02036DA0(data);
    }
    _021D4140.work[0x6B4] = 0;
    if (sub_02033C50(_021D4140.work + 0x580)) {
        *data |= 2;
        if (*data == 2) {
            return 0;
        }
    } else {
        receive.remaining = count - 1;
        receive.dest = data + 1;
        if (sub_02033ECC(_021D4140.work + 0x580, &receive, 1) == 0) {
            _021D4140.work[0x6B4] = 1;
        }
        if (sub_02035FF0() == 1) {
            _021D4140.work[0x6AA]++;
            *data |= (_021D4140.work[0x6AA] << 4) & 0xF0;
        }
    }
    return 1;
}

void sub_02036F30(u8 *data) {
    ReceiveBuffer receive;
    u16 value;

    data[0] = 0xB;
    if (_021D4140.work[0x6B5] == 0) {
        data[1] = 0;
    } else {
        data[1] = 1;
    }
    value = sub_02033250();
    data[2] = value >> 8;
    data[3] = value;
    receive.remaining = 0xBB;
    receive.dest = data + 5;
    if (sub_02033ECC(_021D4140.work + 0x5A0, &receive, 0)) {
        _021D4140.work[0x6B5] = 0;
        data[4] = 0xBB - receive.remaining;
    } else {
        _021D4140.work[0x6B5] = 1;
        data[4] = 0xBB;
    }
}
