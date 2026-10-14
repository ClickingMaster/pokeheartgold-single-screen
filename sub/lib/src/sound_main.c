#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/os/alarm.h>
#include <nitro/os/message.h>
#include <nitro/os/thread.h>
#include <nitro/os/tick.h>
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#define SND_MESSAGE_TICK   1
#define SND_MESSAGE_WAKEUP 2

typedef enum SNDCaptureRepeat {
    SND_CAPTURE_REPEAT = 0,
    SND_CAPTURE_ONESHOT = 1,
} SNDCaptureRepeat;

// Function-local statics of the original code, allocated by the sound data object.
extern u32 u$3681;
extern int initialized$3619;

extern OSThread sndThread;
extern OSMessageQueue sndMesgQueue;
extern OSMessage sndMesgBuffer[8];
extern OSAlarm sndAlarm;
extern u8 sWeakLockChannel[];

void SND_CommandInit(void);
void SND_CommandProc(void);
void SND_AlarmInit(void);
void SND_Enable(void);
void SND_SetOutputSelector(u32 left, u32 right, u32 channel1, u32 channel3);
void SND_SetMasterVolume(s32 volume);
void SND_ExChannelInit(void);
void SND_UpdateExChannel(void);
void SND_ExChannelMain(int update);
void SND_SeqInit(void);
void SND_SeqMain(int update);
void SND_UpdateSharedWork(void);

u16 SND_CalcRandom(void);
void SND_Init(int priority);
void SND_StartIntervalTimer(void);
void SND_StopIntervalTimer(void);
void SND_SendWakeupMessage(void);
void SNDi_LockMutex(void);
void SNDi_UnlockMutex(void);
void SndAlarmCallback(void *arg);
void SndThread(void *arg);
void SND_SetupCapture(int capture, int format, u32 dest, int length, BOOL loop, int in, int out);
int SND_IsCaptureActive(u32 capture);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

u16 SND_CalcRandom(void) {
    u$3681 = u$3681 * 0x19660du + 0x3c6ef35fu;
    return u$3681 >> 16;
}

void SND_Init(int priority) {
    if (!initialized$3619) {
        initialized$3619 = 1;
        SND_CommandInit(); // The thread stack ends where sWeakLockChannel begins.
        OS_CreateThread(&sndThread, SndThread, 0, sWeakLockChannel, 1024, (u32)priority);
        OS_WakeupThreadDirect(&sndThread);
    }
}

void SND_StartIntervalTimer(void) {
    OS_SetPeriodicAlarm(&sndAlarm, OS_GetTick() + 65536, 2728, SndAlarmCallback, 0);
}

void SND_StopIntervalTimer(void) {
    OS_CancelAlarm(&sndAlarm);
}

void SND_SendWakeupMessage(void) {
    OS_SendMessage(&sndMesgQueue, (void *)SND_MESSAGE_WAKEUP, OS_MESSAGE_NOBLOCK);
}

void SNDi_LockMutex(void) {
}

void SNDi_UnlockMutex(void) {
}

void SndAlarmCallback(void *arg) {
    OS_SendMessage(&sndMesgQueue, (void *)SND_MESSAGE_TICK, OS_MESSAGE_NOBLOCK);
}

void SndThread(void *arg) {
    OSMessage message;
    OS_InitMessageQueue(&sndMesgQueue, sndMesgBuffer, 8);
    OS_CreateAlarm(&sndAlarm);
    SND_ExChannelInit();
    SND_SeqInit();
    SND_AlarmInit();
    SND_Enable();
    SND_SetOutputSelector(0, 0, 0, 0);
    SND_SetMasterVolume(127);
    OS_SetPeriodicAlarm(&sndAlarm, OS_GetTick() + 65536, 2728, SndAlarmCallback, 0);
    for (;;) {
        int tick = 0;
        OS_ReceiveMessage(&sndMesgQueue, &message, OS_MESSAGE_BLOCK);
        switch ((u32)message) {
        case SND_MESSAGE_TICK:
            tick = 1;
            break;
        case SND_MESSAGE_WAKEUP:
            break;
        }
        SND_UpdateExChannel();
        SND_CommandProc();
        SND_SeqMain(tick);
        SND_ExChannelMain(tick);
        SND_UpdateSharedWork();
        SND_CalcRandom();
    }
}

void SND_SetupCapture(int capture, int format, u32 dest, int length, BOOL loop, int in, int out) {
    int offset = capture * 8;
    SNDCaptureRepeat repeat = loop ? SND_CAPTURE_REPEAT : SND_CAPTURE_ONESHOT;
    *(vu8 *)(0x04000508 + capture) = (u8)((format << 3) | (repeat << 2) | (in << 1) | out);
    *(vu32 *)(0x04000510 + offset) = dest;
    *(vu16 *)(0x04000514 + offset) = (u16)length;
}

int SND_IsCaptureActive(u32 capture) {
    return (*(volatile u8 *)(capture + 0x04000508) & 0x80) != 0;
}

#pragma dont_inline reset
