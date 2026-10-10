#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "to91_arm7_rtc_work_private.h"
extern void RTC_Reset(void);
extern void RTC_SetHourFormat(int);
extern void RtcReturnResult(u32, u32);
extern void RTC_ReadDateTime(void *);
extern void RTC_ReadDate(void *);
extern void RTC_ReadTime(void *);
extern int RTC_ReadPulse(void *);
extern int RTC_ReadAlarm1(void *);
extern int RTC_ReadAlarm2(void *);
extern void RTC_ReadStatus1(void *);
extern void RTC_ReadStatus2(void *);
extern void RTC_ReadAdjust(void *);
extern void RTC_ReadFree(void *);
extern void RTC_WriteDateTime(void *);
extern void RTC_WriteTime(void *);
extern int RTC_WritePulse(void *);
extern int RTC_WriteAlarm1(void *);
extern int RTC_WriteAlarm2(void *);
extern void RTC_WriteStatus1(const void *);
extern void RTC_WriteStatus2(const void *);
extern void RTC_WriteAdjust(void *);
extern void RTC_WriteFree(void *);

typedef struct {
    u16 pad : 1, format : 1, tail : 14;
} Mode37004;

void RtcThread(void *);

void RtcThread(void *arg) {
    void *message;
    u8 *data = (u8 *)0x027ffde8;
    for (;;) {
        OS_ReceiveMessage(&rtcWork.queue, &message, 1);
        {
            u16 command = rtcWork.command;
            switch (command) {
            case 0:
                RTC_Reset();
                rtcWork.busy = 0;
                RtcReturnResult(0, 0);
                break;
            case 1:
                RTC_SetHourFormat(((Mode37004 *)data)->format);
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
