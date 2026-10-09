#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
#include <nitro/pxi/fifo.h>
#include <nitro/os/tick.h>
extern u8 Work[32];
extern volatile struct NvStatus { u16 error; u16 pad; s32 ready; } _021D43C8;
void DC_InvalidateRange(void *startAddr, u32 nBytes);
void SVC_WaitByLoop(s32 count);
BOOL NVRAMm_ExecuteCommand(int command, u32 address, u32 size, u32 buffer);

static inline BOOL SendRead(u32 address, u32 size, u32 buffer) {
    if (PXI_SendWordByFifo(4, 0x02002300 | ((address >> 16) & 0xFF), FALSE) < 0) return FALSE;
    if (PXI_SendWordByFifo(4, 0x10000 | (address & 0xFFFF), FALSE) < 0) return FALSE;
    if (PXI_SendWordByFifo(4, 0x20000 | (size >> 16), FALSE) < 0) return FALSE;
    if (PXI_SendWordByFifo(4, 0x30000 | (size & 0xFFFF), FALSE) < 0) return FALSE;
    if (PXI_SendWordByFifo(4, 0x40000 | (buffer >> 16), FALSE) < 0) return FALSE;
    if (PXI_SendWordByFifo(4, 0x01050000 | (buffer & 0xFFFF), FALSE) < 0) return FALSE;
    return TRUE;
}
static inline BOOL SendWren(void) {
    if (PXI_SendWordByFifo(4, 0x03002000, FALSE) < 0) return FALSE;
    return TRUE;
}
static inline BOOL SendWrite(u32 address, u32 size, u32 buffer) {
    if (PXI_SendWordByFifo(4, 0x02002500 | ((address >> 16) & 0xFF), FALSE) < 0) return FALSE;
    if (PXI_SendWordByFifo(4, 0x10000 | (address & 0xFFFF), FALSE) < 0) return FALSE;
    if (PXI_SendWordByFifo(4, 0x20000 | size, FALSE) < 0) return FALSE;
    if (PXI_SendWordByFifo(4, 0x30000 | (buffer >> 16), FALSE) < 0) return FALSE;
    if (PXI_SendWordByFifo(4, 0x01040000 | (buffer & 0xFFFF), FALSE) < 0) return FALSE;
    return TRUE;
}
static inline BOOL SendStatus(void *buf) {
    if (PXI_SendWordByFifo(4, 0x02002200 | ((u32)buf >> 24), FALSE) < 0) return FALSE;
    if (PXI_SendWordByFifo(4, 0x10000 | (((u32)buf >> 8) & 0xFFFF), FALSE) < 0) return FALSE;
    if (PXI_SendWordByFifo(4, 0x01020000 | (((u32)buf << 8) & 0xFF00), FALSE) < 0) return FALSE;
    return TRUE;
}
static inline BOOL SendWrdi(void) {
    if (PXI_SendWordByFifo(4, 0x03002D00, FALSE) < 0) return FALSE;
    return TRUE;
}
static inline BOOL SendCmd7(void) {
    if (PXI_SendWordByFifo(4, 0x03002100, FALSE) < 0) return FALSE;
    return TRUE;
}
#pragma opt_arithtransformation off
#pragma opt_loop_invariants off
// @fn NVRAMm_ExecuteCommand
BOOL NVRAMm_ExecuteCommand(int command, u32 address, u32 size, u32 buffer) {
    BOOL sent = FALSE;
    u8 *work = Work;
    OSTick start;
    while (TRUE) {
        if (!sent) {
            _021D43C8.ready = 0;
            switch (command) {
            case 1:
                sent = SendRead(address, size, buffer);
                break;
            case 2:
                sent = SendWren();
                break;
            case 3:
                sent = SendWrite(address, size, buffer);
                start = OS_GetTick();
                break;
            case 4:
            case 5:
                sent = SendStatus(work);
                break;
            case 6:
                sent = SendWrdi();
                break;
            case 7:
                sent = SendCmd7();
                break;
            }
        } else if (_021D43C8.ready == 1) {
            sent = FALSE;
            if (_021D43C8.error != 0) {
                break;
            }
            switch (command) {
            case 1:
                return TRUE;
            case 2:
                command = 4;
                break;
            case 3:
                command = 5;
                break;
            case 4:
            case 5:
                DC_InvalidateRange(work, 1);
                if (command == 4) {
                    if (work[0] & 2) {
                        command = 3;
                    } else {
                        return FALSE;
                    }
                } else {
                    if (!(work[0] & 1)) {
                        return TRUE;
                    }
                    if ((work[0] & 0x20) || (OS_GetTick() - start) * 64 / 0x82EA > 4000) {
                        command = 6;
                    } else {
                        SVC_WaitByLoop(0x4000);
                    }
                }
                break;
            case 6:
                return FALSE;
            case 7:
                return TRUE;
            }
        }
    }
    return FALSE;
}
#pragma opt_arithtransformation reset
#pragma opt_loop_invariants reset
