#include "wireless_connection_state_internal.h"

void sub_02034D60(void *unused, int failed) {
    if (failed != 0) {
        OS_Terminate();
    }
    _021D4134.state = 2;
}

void sub_02034D78(void *unused) {
    _021D4134.state = 0;
    Sys_ClearSleepDisableFlag(4);
}

void sub_02034D8C(void) {
    Sys_SetSleepDisableFlag(4);
    _021D4134.state = 1;
    if (WVR_StartUpAsync(8, sub_02034D60, NULL) != 1) {
        OS_Terminate();
    }
}

BOOL sub_02034DB8(void) {
    return _021D4134.state == 2;
}

BOOL sub_02034DCC(void) {
    return _021D4134.state != 0;
}

void sub_02034DE0(void) {
    WVR_TerminateAsync(sub_02034D78, NULL);
}

void sub_02034DF0(int value) {
    u8 *buffer;
    u32 misalignment;

    *(u32 *)(_021D4134.work + 0xD70) = 0;
    buffer = *(u8 **)(_021D4134.work + 0xD64);
    misalignment = (u32)buffer & 0x1F;
    Wireless_Init(buffer + (0x20 - misalignment), value);
    Wireless_SetGgid(*(void **)(_021D4134.work + 0xD80));
}

void sub_02034E2C(void) {
    int i;
    for (i = 0; i < 16; i++) {
        *(u16 *)(_021D4134.work + 0xD44 + i * 2) = 0;
    }
    MI_CpuFill8(_021D4134.work + 0x114, 0, 0xC00);
}

void sub_02034E64(BOOL value) {
    u8 bit = value;
    _021D4134.work[0xD95] = (_021D4134.work[0xD95] & ~8) | ((bit & 1) << 3);
}

void sub_02034E8C(void) {
    _021D4134.work[0xD74] = 0;
    _021D4134.work[0xD95] &= ~1;
    _021D4134.work[0xD95] &= ~4;
    _021D4134.work[0xD92] = 0;
    _021D4134.work[0xD95] &= ~0x10;
    _021D4134.work[0xD94] = 0;
    _021D4134.work[0xD93] = 0;
}

BOOL sub_02034EF0(int unused, BOOL active, BOOL flag) {
    u8 bit;
    (void)unused;
    sub_02034E8C();
    sub_02034E64(active);
    Wireless_ClearBeaconCount();
    if (_021D4134.work[0xD93] == 0) {
        Wireless_SetReceiveCallback(sub_02036904, 14);
        _021D4134.work[0xD93] = 1;
    }
    bit = flag;
    _021D4134.work[0xD95] = (_021D4134.work[0xD95] & ~0x20) | ((bit & 1) << 5);
    if (Wireless_GetState() == 1 && Wireless_StartMeasureChannels() != 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL sub_02034F64(int unused, BOOL configure) {
    Byte6 data;
    (void)unused;
    sub_02034E8C();
    if (configure) {
        sub_02034E2C();
    }
    if (_021D4134.work[0xD93] == 0) {
        Wireless_SetReceiveCallback(sub_020367A8, 14);
        _021D4134.work[0xD93] = 1;
    }
    if (Wireless_GetState() == 1) {
        data = _020F6A88;
        if (Wireless_StartScan((void (*)(void *))sub_02034C20, &data, 0) != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL sub_02034FE8(void) {
    if (_021D4134.work == NULL) {
        return TRUE;
    }
    switch (_021D4134.work[0xD94]) {
    case 0:
        if (Wireless_IsScanning() != 0) {
            Wireless_StopScan();
            _021D4134.work[0xD94] = 1;
        } else if (Wireless_IsBusy() == 0) {
            Wireless_Stop();
            _021D4134.work[0xD94] = 2;
        }
        break;
    case 1:
        if (Wireless_IsBusy() == 0) {
            Wireless_Stop();
            _021D4134.work[0xD94] = 2;
        }
        break;
    case 2:
        if (Wireless_IsIdle() != 0) {
            return TRUE;
        }
        if (Wireless_HasError() != 0) {
            _021D4134.work[0xD94] = 1;
        }
        break;
    }
    return FALSE;
}

BOOL sub_0203507C(void) {
    if (_021D4134.work != NULL && _021D4134.work[0xD92] == 0) {
        _021D4134.work[0xD92] = 1;
        Wireless_Stop();
        return 1;
    }
    return 0;
}

void sub_020350A8(BOOL active) {
    if (_021D4134.work != NULL) {
        if (active) {
            _021D4134.work[0xD92] = 2;
        } else {
            _021D4134.work[0xD92] = 0;
            sub_02034DF0(1);
        }
    }
}

void sub_020350D4(void) {
    Heap_Free(*(void **)(_021D4134.work + 0xD7C));
    Heap_Free(*(void **)(_021D4134.work + 0xD64));
    Heap_Free(*(void **)(_021D4134.work + 0xD84));
    Heap_Free(_021D4134.work);
    _021D4134.work = NULL;
}
