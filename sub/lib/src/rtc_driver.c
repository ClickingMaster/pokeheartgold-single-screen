#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/exi/ARM7/genPort.h>
#include <nitro/os/interrupt.h>
#include <nitro/os/message.h>
#include <nitro/os/system.h>
#include <nitro/pxi.h>
#include <nitro/pxi/fifo.h>
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// RTC chip commands and register addresses (sent as one byte by RTCi_GpioSendCommand).
#define RTC_GPIO_WRITE          0x06
#define RTC_GPIO_READ           0x86
#define RTC_REG_STATUS1         0x00
#define RTC_REG_STATUS2         0x40
#define RTC_REG_DATETIME        0x20
#define RTC_REG_TIME            0x60
#define RTC_REG_ALARM1_OR_PULSE 0x10
#define RTC_REG_ALARM2          0x50
#define RTC_REG_ADJUST          0x30
#define RTC_REG_FREE            0x70

// Date/time block shared with the ARM9 (the RTC command data buffer).
#define RTC_SHARED_DATA 0x027ffde8

// RTC transfers move one byte; every named field is in that byte, the upper byte is never serialized.
typedef struct RtcStatus1 {
    u16 reset : 1;
    u16 hour24 : 1;
    u16 unk2 : 2;
    u16 alarm1Flag : 1;
    u16 alarm2Flag : 1;
    u16 lowVoltage : 1;
    u16 powerLost : 1;
    u16 : 8;
} RtcStatus1;

typedef struct RtcStatus2 {
    u16 alarm1Mode : 4;
    u16 unk4 : 2;
    u16 alarm2Enable : 1;
    u16 testMode : 1;
    u16 : 8;
} RtcStatus2;

typedef struct RtcDate {
    u32 year : 8;
    u32 month : 5;
    u32 unk13 : 3;
    u32 day : 6;
    u32 unk22 : 2;
    u32 weekday : 3;
    u32 unk27 : 5;
    u32 time;
} RtcDate;

typedef struct RtcAlarm {
    u32 weekday : 8;
    u32 hour : 6;
    u32 pm : 1;
    u32 unk15 : 17;
} RtcAlarm;

typedef struct RtcWork {
    OSMessageQueue queue;
    OSMessage messages[4];
    OSThread thread;
    u8 stack[256];
    u32 busy;
    u16 command;
    u8 unk_1DA[0x1DA];
} RtcWork;

extern u16 rtcInitialized;
extern RtcWork rtcWork;

void RTC_Init(u32 priority);
void RtcPxiCallback(PXIFifoTag channel, u32 word, BOOL error);
void RtcReturnResult(u32 command, u32 status);
void RtcThread(void *arg);
void RtcAlarmIntr(void);
u32 RtcBCD2HEX(u32 value);
void RTC_Reset(void);
void RTC_SetHourFormat(u16 format);
void RTC_ReadDateTime(void *data);
void RTC_WriteDateTime(const void *data);
void RTC_ReadDate(void *data);
void RTC_ReadTime(void *data);
void RTC_WriteTime(const void *data);
int RTC_ReadPulse(void *data);
int RTC_WritePulse(const void *data);
int RTC_ReadAlarm1(void *data);
int RTC_WriteAlarm1(const void *data);
int RTC_ReadAlarm2(void *data);
int RTC_WriteAlarm2(const void *data);
void RTC_ReadStatus1(void *data);
void RTC_WriteStatus1(const void *data);
void RTC_ReadStatus2(void *data);
void RTC_WriteStatus2(const void *data);
void RTC_ReadAdjust(void *data);
void RTC_WriteAdjust(const void *data);
void RTC_ReadFree(void *data);
void RTC_WriteFree(const void *data);
void RtcChangeAlarmFormat24to12(RtcAlarm *alarm);
void RtcChangeAlarmFormat12to24(RtcAlarm *alarm);
void RtcGpioTransfer(u32 command, u32 reg, void *data, u32 size);
void RTCi_GpioStart(void);
void RTCi_GpioEnd(void);
void RTCi_GpioSendCommand(u32 command, u32 reg);
void RTCi_GpioSendData(const void *data, u32 size);
void RTCi_GpioReceiveData(void *data, u32 size);

static inline void InitializeHardwareStatuses(RtcStatus1 *status1, RtcStatus2 *status2) {
    rtcInitialized = 1;
    rtcWork.busy = 1;
    RTC_ReadStatus1(status1);
    RTC_ReadStatus2(status2);
}

void RTC_Init(u32 priority) {
    RtcStatus2 s2;
    RtcStatus1 s1;
    RtcDate *date = (RtcDate *)RTC_SHARED_DATA;
    u32 day, month, year, weekday;
    OSIntrMode irq;
    if (rtcInitialized) {
        return;
    }
    InitializeHardwareStatuses(&s1, &s2);
    if (s1.powerLost || s1.lowVoltage || s2.testMode) {
        s1.reset = 1;
        RTC_WriteStatus1(&s1);
    }
    if (s1.alarm1Flag || s1.alarm2Flag) {
        s2.alarm1Mode = 0;
        s2.alarm2Enable = 0;
        RTC_WriteStatus2(&s2);
    }
    RTC_ReadDateTime(date);
    day = RtcBCD2HEX(date->day);
    month = RtcBCD2HEX(date->month);
    year = RtcBCD2HEX(date->year) + 2000;
    if (month - 1 <= 1) {
        year--;
        month += 12;
    }
    weekday = (year + year / 4 - year / 100 + year / 400 + (13 * month + 8) / 5 + day) % 7;
    if (date->weekday != weekday) {
        date->weekday = weekday;
        RTC_WriteDateTime(date);
    }
    RTC_SetHourFormat(1);
    rtcWork.busy = 0;
    PXI_Init();
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_RTC, RtcPxiCallback);
    OS_InitMessageQueue(&rtcWork.queue, rtcWork.messages, 4);
    OS_CreateThread(&rtcWork.thread, RtcThread, 0, rtcWork.stack + sizeof rtcWork.stack, sizeof rtcWork.stack, priority);
    OS_WakeupThreadDirect(&rtcWork.thread);
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    EXIi_SetBitRcnt0L(0x40, 0);
    EXIi_SetBitRcnt0L(0x100, 0x100);
    irq = OS_DisableInterrupts();
    OS_SetIrqFunction(OS_IE_SIO, RtcAlarmIntr);
    OS_EnableIrqMask(OS_IE_SIO);
    OS_RestoreInterrupts(irq);
}

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void RtcPxiCallback(PXIFifoTag channel, u32 word, BOOL error) {
    u16 command;
    if (error) {
        return;
    }
    command = (word & 0x7f00) >> 8;
    switch (command) {
    case 0:
    case 1:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
        if (rtcWork.busy) {
            RtcReturnResult(command, 3);
        } else {
            rtcWork.busy = 1;
            rtcWork.command = command;
            if (!OS_SendMessage(&rtcWork.queue, 0, OS_MESSAGE_NOBLOCK)) {
                RtcReturnResult(command, 4);
            }
        }
        break;
    default:
        RtcReturnResult(command, 1);
        break;
    }
}

void RtcReturnResult(u32 command, u32 status) {
    u32 packet = ((command << 8) & 0x7f00) | 0x8000 | (status & 255);
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_RTC, packet, 0) < 0) {}
}

void RtcThread(void *arg) {
    void *message;
    u8 *data = (u8 *)RTC_SHARED_DATA;
    for (;;) {
        OS_ReceiveMessage(&rtcWork.queue, &message, OS_MESSAGE_BLOCK);
        {
            u16 command = rtcWork.command;
            switch (command) {
            case 0:
                RTC_Reset();
                rtcWork.busy = 0;
                RtcReturnResult(0, 0);
                break;
            case 1:
                RTC_SetHourFormat(((RtcStatus1 *)data)->hour24);
                rtcWork.busy = 0;
                RtcReturnResult(1, 0);
                break;
            case 16:
                RTC_ReadDateTime(data);
                rtcWork.busy = 0;
                RtcReturnResult(16, 0);
                break;
            case 17:
                RTC_ReadDate(data);
                rtcWork.busy = 0;
                RtcReturnResult(17, 0);
                break;
            case 18:
                RTC_ReadTime(data + 4);
                rtcWork.busy = 0;
                RtcReturnResult(18, 0);
                break;
            case 19:
                if (!RTC_ReadPulse(data + 4)) {
                    rtcWork.busy = 0;
                    RtcReturnResult(19, 2);
                } else {
                    rtcWork.busy = 0;
                    RtcReturnResult(19, 0);
                }
                break;
            case 20:
                if (!RTC_ReadAlarm1(data + 4)) {
                    rtcWork.busy = 0;
                    RtcReturnResult(20, 2);
                } else {
                    rtcWork.busy = 0;
                    RtcReturnResult(20, 0);
                }
                break;
            case 21:
                if (!RTC_ReadAlarm2(data + 4)) {
                    rtcWork.busy = 0;
                    RtcReturnResult(21, 2);
                } else {
                    rtcWork.busy = 0;
                    RtcReturnResult(21, 0);
                }
                break;
            case 22:
                RTC_ReadStatus1(data);
                rtcWork.busy = 0;
                RtcReturnResult(22, 0);
                break;
            case 23:
                RTC_ReadStatus2(data + 2);
                rtcWork.busy = 0;
                RtcReturnResult(23, 0);
                break;
            case 24:
                RTC_ReadAdjust(data + 4);
                rtcWork.busy = 0;
                RtcReturnResult(24, 0);
                break;
            case 25:
                RTC_ReadFree(data + 4);
                rtcWork.busy = 0;
                RtcReturnResult(25, 0);
                break;
            case 32:
                RTC_WriteDateTime(data);
                rtcWork.busy = 0;
                RtcReturnResult(32, 0);
                break;
            case 33:
                RTC_ReadTime(data + 4);
                RTC_WriteDateTime(data);
                rtcWork.busy = 0;
                RtcReturnResult(33, 0);
                break;
            case 34:
                RTC_WriteTime(data + 4);
                rtcWork.busy = 0;
                RtcReturnResult(34, 0);
                break;
            case 35:
                if (!RTC_WritePulse(data + 4)) {
                    rtcWork.busy = 0;
                    RtcReturnResult(35, 2);
                } else {
                    rtcWork.busy = 0;
                    RtcReturnResult(35, 0);
                }
                break;
            case 36:
                if (!RTC_WriteAlarm1(data + 4)) {
                    rtcWork.busy = 0;
                    RtcReturnResult(36, 2);
                } else {
                    rtcWork.busy = 0;
                    RtcReturnResult(36, 0);
                }
                break;
            case 37:
                if (!RTC_WriteAlarm2(data + 4)) {
                    rtcWork.busy = 0;
                    RtcReturnResult(37, 2);
                } else {
                    rtcWork.busy = 0;
                    RtcReturnResult(37, 0);
                }
                break;
            case 38:
                RTC_WriteStatus1(data);
                rtcWork.busy = 0;
                RtcReturnResult(38, 0);
                break;
            case 39:
                RTC_WriteStatus2(data + 2);
                rtcWork.busy = 0;
                RtcReturnResult(39, 0);
                break;
            case 40:
                RTC_WriteAdjust(data + 4);
                rtcWork.busy = 0;
                RtcReturnResult(40, 0);
                break;
            case 41:
                RTC_WriteFree(data + 4);
                rtcWork.busy = 0;
                RtcReturnResult(41, 0);
                break;
            default:
                rtcWork.busy = 0;
                RtcReturnResult(command, 1);
                break;
            }
        }
    }
}

void RtcAlarmIntr(void) {
    RtcStatus1 status1;
    RtcStatus2 status2;
    u32 flags;
    RTC_ReadStatus1(&status1);
    if (status1.alarm1Flag || status1.alarm2Flag) {
        RTC_ReadStatus2(&status2);
        flags = 0;
        if (status1.alarm1Flag) {
            flags |= 1;
            status2.alarm1Mode = 0;
        }
        if (status1.alarm2Flag) {
            flags |= 2;
            status2.alarm2Enable = 0;
        }
        RTC_WriteStatus2(&status2);
        RtcReturnResult(48, flags);
    }
}

u32 RtcBCD2HEX(u32 value) {
    u32 result = 0, multiplier;
    int i, j;
    for (i = 0; i < 8; i++) {
        if (((value >> (i * 4)) & 15) >= 10) {
            return 0;
        }
    }
    j = 0;
    multiplier = 1;
    for (; j < 8; j++) {
        result += multiplier * ((value >> (j * 4)) & 15);
        multiplier *= 10;
    }
    return result;
}

void RTC_Reset(void) {
    RtcStatus1 status;
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    status.reset = 1;
    RTCi_GpioStart();
    RTCi_GpioSendCommand(RTC_GPIO_WRITE, RTC_REG_STATUS1);
    RTCi_GpioSendData(&status, 1);
    RTCi_GpioEnd();
}

void RTC_SetHourFormat(u16 format) {
    u32 alarm;
    RtcStatus1 status;
    format &= 1;
    if (format != 1) {
        return;
    }
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_STATUS1, &status, 1);
    if (status.hour24 == format) {
        return;
    }
    status.hour24 = format;
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RTCi_GpioStart();
    RTCi_GpioSendCommand(RTC_GPIO_WRITE, RTC_REG_STATUS1);
    RTCi_GpioSendData(&status, 1);
    RTCi_GpioEnd();
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_ALARM1_OR_PULSE, &alarm, 3);
    if (format == 0) {
        RtcChangeAlarmFormat24to12((RtcAlarm *)&alarm);
    } else {
        RtcChangeAlarmFormat12to24((RtcAlarm *)&alarm);
    }
    RTCi_GpioStart();
    RTCi_GpioSendCommand(RTC_GPIO_WRITE, RTC_REG_ALARM1_OR_PULSE);
    RTCi_GpioSendData(&alarm, 3);
    RTCi_GpioEnd();
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_ALARM2, &alarm, 3);
    if (format == 0) {
        RtcChangeAlarmFormat24to12((RtcAlarm *)&alarm);
    } else {
        RtcChangeAlarmFormat12to24((RtcAlarm *)&alarm);
    }
    RTCi_GpioStart();
    RTCi_GpioSendCommand(RTC_GPIO_WRITE, RTC_REG_ALARM2);
    RTCi_GpioSendData(&alarm, 3);
    RTCi_GpioEnd();
}

void RTC_ReadDateTime(void *data) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_DATETIME, data, 7);
}

void RTC_WriteDateTime(const void *data) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RTCi_GpioStart();
    RTCi_GpioSendCommand(RTC_GPIO_WRITE, RTC_REG_DATETIME);
    RTCi_GpioSendData(data, 7);
    RTCi_GpioEnd();
}

void RTC_ReadDate(void *data) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_DATETIME, data, 4);
}

void RTC_ReadTime(void *data) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_TIME, data, 3);
}

void RTC_WriteTime(const void *data) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RTCi_GpioStart();
    RTCi_GpioSendCommand(RTC_GPIO_WRITE, RTC_REG_TIME);
    RTCi_GpioSendData(data, 3);
    RTCi_GpioEnd();
}

int RTC_ReadPulse(void *data) {
    RtcStatus2 status;
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_STATUS2, &status, 1);
    if ((status.alarm1Mode & 11) != 1) {
        return 0;
    }
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_ALARM1_OR_PULSE, data, 1);
    return 1;
}

int RTC_WritePulse(const void *data) {
    RtcStatus2 status;
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_STATUS2, &status, 1);
    if ((status.alarm1Mode & 11) != 1) {
        return 0;
    }
    RTCi_GpioStart();
    RTCi_GpioSendCommand(RTC_GPIO_WRITE, RTC_REG_ALARM1_OR_PULSE);
    RTCi_GpioSendData(data, 1);
    RTCi_GpioEnd();
    return 1;
}

int RTC_ReadAlarm1(void *data) {
    RtcStatus2 status;
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_STATUS2, &status, 1);
    if (status.alarm1Mode != 4) {
        return 0;
    }
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_ALARM1_OR_PULSE, data, 3);
    return 1;
}

int RTC_WriteAlarm1(const void *data) {
    RtcStatus2 status;
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_STATUS2, &status, 1);
    if (status.alarm1Mode != 4) {
        return 0;
    }
    RTCi_GpioStart();
    RTCi_GpioSendCommand(RTC_GPIO_WRITE, RTC_REG_ALARM1_OR_PULSE);
    RTCi_GpioSendData(data, 3);
    RTCi_GpioEnd();
    return 1;
}

int RTC_ReadAlarm2(void *data) {
    RtcStatus2 status;
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_STATUS2, &status, 1);
    if (!status.alarm2Enable) {
        return 0;
    }
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_ALARM2, data, 3);
    return 1;
}

int RTC_WriteAlarm2(const void *data) {
    RtcStatus2 status;
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_STATUS2, &status, 1);
    if (!status.alarm2Enable) {
        return 0;
    }
    RTCi_GpioStart();
    RTCi_GpioSendCommand(RTC_GPIO_WRITE, RTC_REG_ALARM2);
    RTCi_GpioSendData(data, 3);
    RTCi_GpioEnd();
    return 1;
}

void RTC_ReadStatus1(void *data) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_STATUS1, data, 1);
}

void RTC_WriteStatus1(const void *data) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RTCi_GpioStart();
    RTCi_GpioSendCommand(RTC_GPIO_WRITE, RTC_REG_STATUS1);
    RTCi_GpioSendData(data, 1);
    RTCi_GpioEnd();
}

void RTC_ReadStatus2(void *data) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_STATUS2, data, 1);
}

void RTC_WriteStatus2(const void *data) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RTCi_GpioStart();
    RTCi_GpioSendCommand(RTC_GPIO_WRITE, RTC_REG_STATUS2);
    RTCi_GpioSendData(data, 1);
    RTCi_GpioEnd();
}

void RTC_ReadAdjust(void *data) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_ADJUST, data, 1);
}

void RTC_WriteAdjust(const void *data) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RTCi_GpioStart();
    RTCi_GpioSendCommand(RTC_GPIO_WRITE, RTC_REG_ADJUST);
    RTCi_GpioSendData(data, 1);
    RTCi_GpioEnd();
}

void RTC_ReadFree(void *data) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RtcGpioTransfer(RTC_GPIO_READ, RTC_REG_FREE, data, 1);
}

void RTC_WriteFree(const void *data) {
    EXIi_SelectRcnt(EXI_GPIOIF_GPIO);
    RTCi_GpioStart();
    RTCi_GpioSendCommand(RTC_GPIO_WRITE, RTC_REG_FREE);
    RTCi_GpioSendData(data, 1);
    RTCi_GpioEnd();
}

void RtcChangeAlarmFormat24to12(RtcAlarm *alarm) {
    switch (alarm->hour) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 16:
    case 17:
        alarm->pm = 0;
        return;
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 34:
    case 35:
        alarm->pm = 1;
        alarm->hour -= 0x12;
        return;
    case 32:
    case 33:
        alarm->pm = 1;
        alarm->hour -= 0x18;
        return;
    default:
        alarm->pm = 0;
        alarm->hour = 0;
        return;
    }
}

void RtcChangeAlarmFormat12to24(RtcAlarm *alarm) {
    switch (alarm->hour) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 16:
    case 17:
        if (!alarm->pm) {
            return;
        }
        alarm->hour += 0x12;
        return;
    case 8:
    case 9:
        if (!alarm->pm) {
            return;
        }
        alarm->hour += 0x18;
        return;
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 32:
    case 33:
    case 34:
    case 35:
        alarm->pm = 1;
        return;
    default:
        alarm->pm = 0;
        alarm->hour = 0;
        return;
    }
}

void RtcGpioTransfer(u32 command, u32 reg, void *data, u32 size) {
    RTCi_GpioStart();
    RTCi_GpioSendCommand(command, reg);
    switch (command) {
    case RTC_GPIO_READ:
        RTCi_GpioReceiveData(data, size);
        break;
    case RTC_GPIO_WRITE:
        RTCi_GpioSendData(data, size);
        break;
    }
    RTCi_GpioEnd();
}

#pragma dont_inline reset

// clang-format off
asm void RTCi_GpioStart(void)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
    ldrh r0, [r12]
    bic r0, r0, #0x77
    orr r0, r0, #0x72
    strh r0, [r12]
    mov r3, #2
@wait1:
    subs r3, r3, #1
    bne @wait1
    bic r0, r0, #4
    orr r0, r0, #4
    strh r0, [r12]
    mov r3, #2
@wait2:
    subs r3, r3, #1
    bne @wait2
    bx lr
}
// clang-format on

// clang-format off
asm void RTCi_GpioEnd(void)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
    mov r3, #2
@wait1:
    subs r3, r3, #1
    bne @wait1
    ldrh r0, [r12]
    bic r0, r0, #4
    orr r0, r0, #0
    strh r0, [r12]
    mov r3, #2
@wait2:
    subs r3, r3, #1
    bne @wait2
    bx lr
}
// clang-format on

// clang-format off
asm void RTCi_GpioSendCommand(u32 command, u32 reg)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
    orr r1, r0, r1
    ldrh r0, [r12]
    bic r0, r0, #0x77
    orr r0, r0, #0x74
    mov r2, #0
@bit:
    bic r0, r0, #3
    orr r0, r0, #0
    mov r3, #1
    tst r3, r1, lsr r2
    movne r3, #1
    moveq r3, #0
    orr r0, r0, r3
    strh r0, [r12]
    mov r3, #9
@wait1:
    subs r3, r3, #1
    bne @wait1
    bic r0, r0, #2
    orr r0, r0, #2
    strh r0, [r12]
    mov r3, #9
@wait2:
    subs r3, r3, #1
    bne @wait2
    add r2, r2, #1
    cmp r2, #8
    bne @bit
    bx lr
}
// clang-format on

// clang-format off
asm void RTCi_GpioSendData(const void *data, u32 size)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
@byte:
    stmfd sp!, {r0, r1}
    tst r0, #1
    ldreqh r1, [r0]
    ldrneh r1, [r0, #-1]
    movne r1, r1, lsr #8
    ldrh r0, [r12]
    bic r0, r0, #0x77
    orr r0, r0, #0x74
    mov r2, #0
@bit:
    bic r0, r0, #3
    orr r0, r0, #0
    mov r3, #1
    tst r3, r1, lsr r2
    movne r3, #1
    moveq r3, #0
    orr r0, r0, r3
    strh r0, [r12]
    mov r3, #9
@wait1:
    subs r3, r3, #1
    bne @wait1
    bic r0, r0, #2
    orr r0, r0, #2
    strh r0, [r12]
    mov r3, #9
@wait2:
    subs r3, r3, #1
    bne @wait2
    add r2, r2, #1
    cmp r2, #8
    bne @bit
    ldmfd sp!, {r0, r1}
    add r0, r0, #1
    subs r1, r1, #1
    bne @byte
    bx lr
}
// clang-format on

// clang-format off
asm void RTCi_GpioReceiveData(void *data, u32 size)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
@byte:
    stmfd sp!, {r0, r1}
    ldrh r0, [r12]
    bic r0, r0, #0x77
    orr r0, r0, #0x64
    mov r2, #0
    mov r1, #0
@bit:
    bic r0, r0, #3
    orr r0, r0, #0
    strh r0, [r12]
    mov r3, #9
@wait1:
    subs r3, r3, #1
    bne @wait1
    ldrh r0, [r12]
    and r3, r0, #1
    cmp r3, #1
    moveq r3, #0x80
    movne r3, #0
    orr r2, r3, r2, lsr #1
    bic r0, r0, #2
    orr r0, r0, #2
    strh r0, [r12]
    mov r3, #9
@wait2:
    subs r3, r3, #1
    bne @wait2
    add r1, r1, #1
    cmp r1, #8
    bne @bit
    ldmfd sp!, {r0, r1}
    tst r0, #1
    beq @even
    ldrh r3, [r0, #-1]
    bic r3, r3, #0xff00
    mov r2, r2, lsl #8
    orr r3, r2, r3
    strh r3, [r0, #-1]
    b @next
@even:
    ldrh r3, [r0]
    bic r3, r3, #0xff
    orr r3, r3, r2
    strh r3, [r0]
@next:
    add r0, r0, #1
    subs r1, r1, #1
    bne @byte
    bx lr
}
// clang-format on
