#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Wireless register initialisation: register offset and value.
typedef struct WlRegInit {
    u16 offset;
    u16 value;
} WlRegInit;

extern const WlRegInit macInitRegs[];
// Offsets of the 16 registers InitRF loads from flash 0x44 (one halfword each).
extern const u16 macTxRxRegAdrs[16];

u64 _ll_udiv(u64 dividend, u64 divisor);

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// clang-format off
asm u64 __VENEER__ll_udiv(u64 dividend, u64 divisor) {
    ldr pc, =_ll_udiv
}
// clang-format on

void WStop(void) {
    WlBss *bss = &wlWorkPlain->bss;
    __VENEER_OS_CancelAlarm(&wlWorkPlain->periodicTimeOutAlarm);
    __VENEER_OS_CancelAlarm(&wlWorkPlain->timeOutAlarm);
    WSetStaState(32);
    bss->unk_a4 = 0;
    bss->unk_12 = 0;
    WL_REG(0x012) = 0;
    WL_REG(0x004) = 0;
    WL_REG(0x0EA) = 0;
    WL_REG(0x0E8) = 0;
    WL_REG(0x008) = 0;
    WL_REG(0x00A) = 0;
    if (bss->mode == 1) {
        StopBeaconFrame();
    }
    WL_REG(0x0AC) = 0xFFFF;
    WL_REG(0x0B4) = 0xFFFF;
    DeleteAllTxFrames();
    ReleaseAllWlHeapBuf();
}

u32 BBP_Read(u32 reg) {
    WL_REG(0x158) = reg | 0x6000;
    WaitLoop_BbpAccess();
    return WL_REG(0x15C);
}

// Returns 0, or -1 when the BBP did not become ready.
int BBP_Write(u32 reg, u32 value) {
    WL_REG(0x15A) = value;
    WL_REG(0x158) = reg | 0x5000;
    return (s32)(WaitLoop_BbpAccess() ? 0xFFFFFFFFu : 0u);
}

int RF_Write(u32 value) {
    WL_REG(0x17E) = value;
    WL_REG(0x17C) = value >> 16;
    return WaitLoop_RfAccess();
}

// CRC over the 105 BBP register values in the flash (from 0x64; only bit 7 of the second one counts).
u32 CalcBbpCRC(void) {
    u32 address, ordinal;
    int crc;
    u32 value;
    value = 0;
    crc = 0;
    ordinal = 0;
    address = 100;
    do {
        FLASH_Read(address, 1, &value);
        if (ordinal == 1) {
            value &= 0x80;
        }
        crc = calc_NextCRC((int)(value & 0xFF), crc);
        ordinal++;
        address++;
    } while (ordinal < 105);
    return (u32)crc;
}

void WConfigDevice(void) {
    WlRf *rf = &wlWorkPlain->rf;
    __VENEER_MIi_CpuClear16(0, rf, sizeof(WlRf));
    FLASH_Read(0x40, 1, &rf->type);
    FLASH_Read(0x41, 1, &rf->bits);
    FLASH_Read(0x42, 1, &rf->initCount);
    FLASH_Read(0x43, 1, &rf->rfChannelCount);
}

void InitMac(void) {
    u32 i = 0;
    do {
        WL_REG(macInitRegs[i].offset) = macInitRegs[i].value;
        i++;
    } while (i < 25);
}

// Loads the 105 BBP registers from the flash.
void InitBaseBand(void) {
    u32 value;
    u32 i;
    u32 address;
    WL_REG(0x160) = 0x100;
    value = 0;
    i = 0;
    address = 100;
    for (; i < 105; i++, address++) {
        FLASH_Read(address, 1, &value);
        BBP_Write(i, value);
    }
    BBP_Write(0x5A, 2);
}

// Loads the RF registers from the flash (type 3: one byte per register, else `bits` per write).
void InitRF(void) {
    u32 value;
    u32 data;
    u32 i;
    u32 addr;
    s32 bytes;
    u32 count;
    WlRf *rf;
    rf = &wlWorkPlain->rf;
    data = 0;
    for (i = 0; i < 16; i++) {
        FLASH_Read(0x44 + i * 2, 2, &data);
        WL_REG(macTxRxRegAdrs[i]) = data;
    }
    value = ((u32)rf->bits >> 7) << 8;
    value |= rf->bits & 0x7F;
    WL_REG(0x184) = value;
    addr = 0xCE;
    bytes = ((rf->bits & 0x7F) + 7) / 8;
    count = rf->initCount;
    if (rf->type == 3) {
        FLASH_Read(count + 0xCE, 1, &rf->bbpChannelCount);
        for (i = 0; i < count;) {
            value = 0;
            FLASH_Read(addr, 1, &value);
            value |= (i << 8) + 0x50000;
            RF_Write(value);
            i++;
            addr++;
        }
    } else {
        value = 0;
        while (count != 0) {
            FLASH_Read(addr, bytes, &value);
            RF_Write(value);
            if (rf->type == 2 && (value >> 18) == 9) {
                rf->unk_0c = value & ~0x7C00;
            }
            count--;
            addr += bytes;
        }
    }
}

void InitializeAlarm(void) {
    WlWork *w = wlWorkPlain;
    if (!__VENEER_OS_IsAlarmAvailable()) {
        w->param.errors |= 0x40;
        return;
    }
    __VENEER_OS_CreateAlarm(&w->periodicTimeOutAlarm);
    __VENEER_OS_CreateAlarm(&w->timeOutAlarm);
    __VENEER_OS_CreateAlarm(&w->ksidAlarm);
}

// clang-format off
asm BOOL __VENEER_OS_IsAlarmAvailable(void) {
    ldr pc, =OS_IsAlarmAvailable
}
// clang-format on

void TimeoutDummy(void *arg) {
    *(vu32 *)arg = 0;
}

void WWaitus(u32 us) {
    WaitLoop_Waitus(us, TimeoutDummy);
}

#pragma dont_inline reset
