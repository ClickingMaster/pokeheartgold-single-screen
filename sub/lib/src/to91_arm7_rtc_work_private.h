#ifndef TO91_ARM7_RTC_WORK_PRIVATE_H
#define TO91_ARM7_RTC_WORK_PRIVATE_H
#include <nitro/os/message.h>
#include <nitro/pxi/fifo.h>

typedef struct Arm7RtcWork {
    OSMessageQueue queue;
    OSMessage messages[4];
    OSThread thread;
    u8 stack[256];
    u32 busy;
    u16 command;
    u8 remaining[0x1da];
} Arm7RtcWork;
typedef char Arm7RtcWorkSize[(sizeof(Arm7RtcWork)==0x3b4)?1:-1];
extern Arm7RtcWork rtcWork;
void RtcThread(void *);
void RtcPxiCallback(PXIFifoTag, u32, BOOL);
#endif
