#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0x6c];
    int exists;
    u8 tail[0x50];
} ThreadData;

typedef struct {
    u8 p[0x1000];
    u8 status;
    u8 p1001[0x17];
    int mode;
    u8 p101c[0x114];
    int ssl;
    u32 gap;
    u8 socket[0x64];
    u8 p119c[0x830];
    void *sendbuf, *recvbuf;
    u8 seed[32];
    u8 p19f4[0x24];
    u8 mutex2[0x120];
    ThreadData thread;
    u8 mutex[24];
    int stop;
} Work;

extern int ov00_0221A6B4;
extern void ov00_021E9438(void *);
extern void CPS_SslAddRandomSeed(void *, u32);
extern void CPS_SocRegister(void *);
void ov00_021E9154(Work *, int);
void ov00_021E9210(Work *);
int ov00_021E926C(Work *);
void ov00_021E92E4(Work *);

void ov00_021E9154(Work *w, int priority) {
    w->stop = 0;
    OS_InitMutex((OSMutex *)w->mutex);
    OS_InitMutex((OSMutex *)w->mutex2);
    if (w->mode == 1) {
        ov00_0221A6B4 = 1;
    } else {
        ov00_0221A6B4 = 0;
    }
    if (w->thread.exists && !OS_IsThreadTerminated((OSThread *)&w->thread)) {
        return;
    }
    OS_CreateThread((OSThread *)&w->thread, ov00_021E9438, w, (u8 *)w + 0x1000, 4096, priority);
    OS_WakeupThreadDirect((OSThread *)&w->thread);
}

void ov00_021E9210(Work *w) {
    if (w->status != 255) {
        return;
    }
    OS_LockMutex((OSMutex *)w->mutex);
    w->stop = 1;
    OS_UnlockMutex((OSMutex *)w->mutex);
    if (w->thread.exists) {
        OS_JoinThread((OSThread *)&w->thread);
    }
}

int ov00_021E926C(Work *w) {
    if (w->ssl == 1) {
        OS_GetLowEntropyData((u32 *)w->seed);
        CPS_SslAddRandomSeed(w->seed, 32);
    }
    OS_LockMutex((OSMutex *)w->mutex);
    if (w->stop == 1) {
        OS_UnlockMutex((OSMutex *)w->mutex);
        return 0;
    }
    OS_UnlockMutex((OSMutex *)w->mutex);
    OS_Sleep(10);
    return 1;
}

void ov00_021E92E4(Work *w) {
    MI_CpuFill8(w->socket, 0, 100);
    *(u32 *)(w->socket + 0x3c) = 2920;
    *(void **)(w->socket + 0x40) = w->sendbuf;
    *(u32 *)(w->socket + 0x48) = 1514;
    *(void **)(w->socket + 0x4c) = w->recvbuf;
    CPS_SocRegister(w->socket);
}
