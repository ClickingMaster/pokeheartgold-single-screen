#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/os/thread.h>
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int sMasterPan;
extern int sSurroundDecay;
extern u8 sOrgPan[];
extern u8 sOrgVolume[];
extern int CalcSurroundDecay(int, int);
#define R8(c, o)  (*(volatile u8 *)(0x04000000u + ((u32)(c) << 4) + (o)))
#define R16(c, o) (*(volatile u16 *)(0x04000000u + ((u32)(c) << 4) + (o)))
#define R32(c, o) (*(volatile u32 *)(0x04000000u + ((u32)(c) << 4) + (o)))
extern u32 u$3681;
u16 SND_CalcRandom(void);
extern int initialized$3619;
extern OSThread sndThread;
extern u8 sWeakLockChannel[];
void SND_CommandInit(void);
void SndThread(void *);
typedef char W08ThreadExtent[(sizeof(OSThread) == 0xa4) ? 1 : -1];
void SND_Init(int priority);

u16 SND_CalcRandom(void) {
    u$3681 = u$3681 * 0x19660du + 0x3c6ef35fu;
    return u$3681 >> 16;
}

void SND_Init(int priority) {
    if (!initialized$3619) {
        initialized$3619 = 1;
        SND_CommandInit();
        OS_CreateThread(&sndThread, SndThread, 0, sWeakLockChannel, 1024, (u32)priority);
        OS_WakeupThreadDirect(&sndThread);
    }
}
