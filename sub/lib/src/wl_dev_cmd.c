#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#pragma dollar_identifiers on

// Driver version string (8 bytes) reported by DEV_GetVerInfoReqCmd.
extern const u8 wlVersion$4017[];

// DEV command handlers (WlibCmdTbl_DEV): driver state changes, version and statistics, test signals.

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

int DEV_ShutdownReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    u32 state;
    cfm->header.length = 1;
    state = wlWork->bss.state;
    if (state != 0 && state != 16) {
        return 1;
    }
    WSetStaState(0);
    return 0;
}

int DEV_IdleReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    WlWork *w;
    int result = 1;
    cfm->header.length = result;
    w = wlWork;
    if (w->bss.state <= 32 && w->bss.unk_12 == 0) {
        if (FLASH_VerifyCheckSum(NULL)) {
            result = 14;
        } else {
            WSetStaState(16);
            result = 0;
        }
    }
    return result;
}

int DEV_Class1ReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    WlWork *w;
    u32 state;
    cfm->header.length = 1;
    w = wlWork;
    state = w->bss.state;
    if (state == 16 || (state == 32 && w->bss.unk_12 == 0)) {
        WSetStaState(32);
        return 0;
    } else {
        return 1;
    }
}

int DEV_RebootReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 1;
    if (wlWork->bss.state >= 32) {
        WStop();
    }
    WlessLibReboot();
    return 0;
}

int DEV_ClearWlInfoReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    if (wlWork->bss.state == 0) {
        return 1;
    }
    cfm->header.length = 1;
    WInitCounter();
    return 0;
}

// Confirm: driver version (8 bytes), MAC version (register 0x000), BBP version and register CRC
// (unk_690 bit 15; fixed values without it), RF type (unk_690 bit 14; 2 without it).
int DEV_GetVerInfoReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    WlWork *w;
    cfm->header.length = 9;
    __VENEER_MIi_CpuCopy16(wlVersion$4017, cfm->param, 8);
    cfm->param[4] = WL_REG(0x000);
    if (!(wlWork->unk_690 & 0x8000)) {
        cfm->param[5] = 0x6D;
        cfm->param[6] = 0x933D;
    } else {
        cfm->param[5] = BBP_Read(0);
        cfm->param[6] = CalcBbpCRC();
    }
    w = wlWork;
    if (w->unk_690 & 0x4000) {
        cfm->param[7] = w->rf.type;
    } else {
        cfm->param[7] = 2;
    }
    return 0;
}

// Confirm: the statistics counters (WlWork.counter, updated first).
int DEV_GetWlInfoReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    if (wlWork->bss.state == 0) {
        return 1;
    }
    cfm->header.length = 0x5C;
    WUpdateCounter();
    __VENEER_MIi_CpuCopy32(wlWork->counter, cfm->param + 1, 0xB4);
    return 0;
}

// clang-format off
asm void __VENEER_MIi_CpuCopy32(const void *src, void *dest, u32 size) {
    ldr pc, =MIi_CpuCopy32
}
// clang-format on

int DEV_GetStateReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->bss.state;
    return 0;
}

// param: action (0: stop, 1: start), signal (0..2: continuous wave variants, 3, 4: carrier suppression
// frames), rate (10 or 20), channel. Test states: 17 continuous signal, 18 carrier suppression.
int DEV_TestSignalReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    WlWork *w = wlWork;
    WlBss *bss = &w->bss;
    u32 raw, flash;
    cfm->header.length = 1;
    if ((bss->state & 0xF0) != 16) {
        return 1;
    }
    if (req->param[0] > 1) {
        return 5;
    }
    if (req->param[2] != 10 && req->param[2] != 20) {
        return 5;
    }
    if (req->param[1] > 4) {
        return 5;
    }
    if (FLASH_VerifyCheckSum(NULL)) {
        return 14;
    }
    switch (req->param[0]) {
    case 0:
        goto stop;
    case 1:
        break;
    default:
        goto done;
    }
    if (bss->state != 16) {
        return 1;
    }
    raw = 0;
    FLASH_DirectRead(0x65, 1, &raw);
    flash = raw;
    if (flash != BBP_Read(1)) {
        BBP_Write(1, flash);
        WWaitus(5000);
    }
    switch (req->param[1]) {
    case 0:
    case 1:
    case 2:
        bss->state = 17;
        WSetChannel(req->param[3], 1);
        WL_REG(0x040) = 0x8000;
        WWaitus(1500);
        bss->unk_16 = req->param[2];
        raw = BBP_Read(2);
        if (req->param[1] <= 1) {
            raw |= 16;
        }
        WL_REG(0x1A4) = req->param[2];
        if (req->param[1] == 1) {
            raw |= 32;
            WL_REG(0x1A2) = 3;
        } else {
            WL_REG(0x1A2) = req->param[1];
        }
        BBP_Write(2, raw);
        WL_REG(0x1A0) = 0x823;
        break;
    case 3:
    case 4: {
        vu16 *buf = (vu16 *)0x04804000, *dest;
        u32 i;
        bss = &wlWork->bss;
        WStart();
        WStop();
        bss->unk_ac = BBP_Read(6);
        if (req->param[1] == 4) {
            BBP_Write(6, 0);
        }
        __VENEER_MIi_CpuClear16(0, (void *)buf, 12);
        buf[4] = 20;
        buf[5] = 2000;
        dest = buf + 6;
        for (i = 0; i < 2028; i += 2) {
            *dest++ = 0x5555;
        }
        buf[6] = 8;
        WL_REG(0x194) = 6;
        bss->state = 18;
        bss->unk_18 = 1;
        WSetChannel(req->param[3], 1);
        WL_REG(0x040) = 0x8000;
        WWaitus(1500);
        WL_REG(0x012) = 2;
        WL_REG(0x004) = 1;
        WL_REG(0x0AE) = 1;
        WL_REG(0x0A0) = (u16)(((u32)buf & 0x3FFF) >> 1) | 0x8000;
        break;
    }
    }
    goto done;
stop:
    if (bss->state == 17) {
        ClearPeriodicTimeOut();
        WL_REG(0x1A0) = 0;
        WL_REG(0x1A2) = 1;
        WL_REG(0x040) = 0;
        raw = BBP_Read(2) & ~0x30u;
        BBP_Write(2, raw);
    } else if (bss->state == 18) {
        bss->unk_18 = 0;
        while (WL_REG(0x004)) {}
        BBP_Write(6, bss->unk_ac);
    } else {
        return 1;
    }
    bss->state = 16;
done:
    return 0;
}

// TX end in test state 18: sends the carrier suppression frame again until the test stops.
void IntrCarrierSuppresionSignal(void) {
    if (wlWork->bss.unk_18) {
        *(vu16 *)0x04804000 = 0;
        *(vu16 *)0x04804004 = 0;
        WL_REG(0x0A0) |= 0x8000;
    } else {
        WL_REG(0x0AC) = 1;
        WL_REG(0x004) = 0;
        WL_REG(0x012) = 2;
        WL_REG(0x010) = 0xFFFF;
        WL_REG(0x194) = 0;
        WL_REG(0x040) = 0;
    }
}

// param: action (0: stop, 1: start), channel. Test state 17: receive only.
int DEV_TestRxReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    WlBss *bss = &wlWork->bss;
    u32 state;
    cfm->header.length = 1;
    state = bss->state;
    if ((state & 0xF0) != 16) {
        return 1;
    }
    if (req->param[0] != 0) {
        if (req->param[0] == 1) {
            if (state != 16) {
                return 1;
            }
            WSetChannel(req->param[1], 1);
            bss->mode = 0;
            WStart();
            WSetForcePowerState(0x8000);
            bss->state = 17;
        }
    } else {
        if (state == 17) {
            WSetForcePowerState(0);
            WStop();
        } else {
            return 1;
        }
        bss->state = 16;
    }
    return 0;
}

#pragma dont_inline reset
