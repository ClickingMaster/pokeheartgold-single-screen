#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/os/system.h>
#include <nitro/os/valarm.h>

#include "spi_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

void SPIi_ReturnResult(u16 command, u16 result);

void TP_ExecuteProcess(TpRequest *request);

// Consecutive valid / invalid samples, used to adapt tpw.range.
static u8 sInvalidCount;
static u8 sValidCount;

static inline void UpdateTouchThreshold(const TpSample *sample, u16 deviation) {
    if (!sample->bits.touch) {
        sInvalidCount = 0;
        sValidCount = 0;
    } else if (sample->bits.validity) {
        sValidCount = 0;
        if (++sInvalidCount >= 4) {
            sInvalidCount = 0;
            if (tpw.range < 35) {
                tpw.range++;
            }
        }
    } else {
        sInvalidCount = 0;
        if ((s32)deviation >= (tpw.range >> 1)) {
            sValidCount = 0;
        } else if (++sValidCount >= 4) {
            sValidCount = 0;
            if (tpw.range > tpw.minRange) {
                tpw.range--;
                sInvalidCount = 3;
            }
        }
    }
}

void TP_ExecuteProcess(TpRequest *request) {
    switch (request->command) {
    case 0x10:
        if (tpw.state != 2) {
            break;
        }
    case 0: {
        OSIntrMode irq;
        TpSample result;
        u16 deviation;
        irq = OS_DisableInterrupts();
        if (!SPIi_CheckException(0)) {
            OS_RestoreInterrupts(irq);
            SPIi_ReturnResult((u16)request->command, 4);
            break;
        }
        SPIi_GetException(0);
        OS_RestoreInterrupts(irq);
        TP_ExecSampling(&result, tpw.range, &deviation);
        UpdateTouchThreshold(&result, deviation);
        *(volatile u16 *)0x027fffaa = result.halves[0];
        *(volatile u16 *)0x027fffac = result.halves[1];
        if (request->command == 0) {
            SPIi_ReturnResult(request->command, 0);
        } else {
            SPIi_ReturnResult(request->command, request->count & 0xff);
        }
        SPIi_ReleaseException(0);
        break;
    }
    case 1:
        if (tpw.state == 1) {
            u32 i;
            for (i = 0; i < request->count; i++) {
                u16 line = (u16)((request->start + (i * 263u / request->count)) % 263u);
                // skip the lines around the start of VBlank
                if (line >= 200 && line < 215) {
                    line = 215;
                }
                tpw.lines[i] = (s16)line;
                OS_SetPeriodicVAlarm(&tpw.alarms[i], tpw.lines[i], 10, TpVAlarmHandler, (void *)i);
            }
            SPIi_ReturnResult((u16)request->command, 0);
            tpw.state = 2;
        } else {
            SPIi_ReturnResult((u16)request->command, 3);
        }
        break;
    case 2:
        if (tpw.state == 3) {
            OS_CancelVAlarms(TP_VALARM_TAG);
            SPIi_ReturnResult((u16)request->command, 0);
            tpw.state = 0;
        } else {
            SPIi_ReturnResult((u16)request->command, 3);
        }
        break;
    }
}
