#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct {
    u16 b0 : 1;
    u16 hour24 : 1;
    u16 rest : 14;
} RtcStatus1;
extern void EXIi_SelectRcnt(u16);
extern BOOL RtcGpioTransfer(u32, u32, void *, u32);
extern void RTCi_GpioStart(void);
extern void RTCi_GpioEnd(void);
extern void RTCi_GpioSendCommand(u32, u32);
extern void RTCi_GpioSendData(void *, u32);
extern void RtcChangeAlarmFormat24to12(void *);
extern void RtcChangeAlarmFormat12to24(void *);
void RTC_SetHourFormat(u16 format);
// @fn RTC_SetHourFormat
void RTC_SetHourFormat(u16 format)
{
    u32 alarm;
    RtcStatus1 st;
    format &= 1;
    if (format != 1)
        return;
    EXIi_SelectRcnt(0x8000);
    RtcGpioTransfer(0x86, 0, &st, 1);
    if (st.hour24 == format)
        return;
    st.hour24 = format;
    EXIi_SelectRcnt(0x8000);
    RTCi_GpioStart();
    RTCi_GpioSendCommand(6, 0);
    RTCi_GpioSendData(&st, 1);
    RTCi_GpioEnd();
    RtcGpioTransfer(0x86, 0x10, &alarm, 3);
    if (format == 0)
        RtcChangeAlarmFormat24to12(&alarm);
    else
        RtcChangeAlarmFormat12to24(&alarm);
    RTCi_GpioStart();
    RTCi_GpioSendCommand(6, 0x10);
    RTCi_GpioSendData(&alarm, 3);
    RTCi_GpioEnd();
    RtcGpioTransfer(0x86, 0x50, &alarm, 3);
    if (format == 0)
        RtcChangeAlarmFormat24to12(&alarm);
    else
        RtcChangeAlarmFormat12to24(&alarm);
    RTCi_GpioStart();
    RTCi_GpioSendCommand(6, 0x50);
    RTCi_GpioSendData(&alarm, 3);
    RTCi_GpioEnd();
}
